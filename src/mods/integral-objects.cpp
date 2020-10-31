#include "integral-objects.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "zone.hpp"

#define mo_debug(A) std::cerr << "[mods::integral_objects][debug]:" << A <<"\n";

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::integral_objects {
	int write_db_values(std::string_view section_name, std::string_view prefix, const std::vector<std::string>& values){
		using namespace mods::db;
		std::string value = "";
		return put_section_vector(section_name.data(),prefix.data(), values);
	}
	/*
CREATE TABLE integral_object (
	object_id SERIAL,
	object_room_vnum INTEGER NOT NULL,
	object_type VARCHAR(16) NOT NULL,
	object_vnum INTEGER NOT NULL,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE camera_feed (
	feed_id SERIAL,
	feed_type VARCHAR(16) NOT NULL,
	feed_vnum INTEGER NOT NULL,
	feed_room_vnum INTEGER NOT NULL,
	feed_order INTEGER NOT NULL DEFAULT 0,
	created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
*/
	void save_item_to_db(player_ptr_t& player, std::string section_name, std::vector<std::string>& args){
		std::string value = "";
		std::string prefix = std::to_string(world[player->room()].number);
		auto status = mods::db::put_section_vector(section_name,prefix, args);
		player->send("status: %d\r\n",status);
		std::vector<std::string> values;
		status = mods::db::get_section_vector(section_name,prefix,values);
		player->send("get status: %d\r\nTo confirm, we placed these values...\r\n",status);
		for(auto line : values){
			player->send("[item]: '%s'\r\n",line.c_str());
		}
		player->sendln("Done listing.");
	}
	void save_weapon_locker(player_ptr_t& player, std::vector<std::string>& args){
		mo_debug("saving weapon locker");
		save_item_to_db(player, "weapon-locker", args);
	}
	void save_armor_locker(player_ptr_t& player, std::vector<std::string>& args){
		mo_debug("saving armor locker");
		save_item_to_db(player, "armor-locker", args);
	}

	void save_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		write_db_values("camera-feed",std::to_string(world[player->room()].number), args);
	}
	obj_ptr_t first_or_create(room_vnum room,std::string query, int type, std::string yaml_file){
		std::cerr << green_str("[first_or_create]: room: ") << room << "| real:" << real_room(room) << "\n";
		for(auto obj = world[real_room(room)].contents; obj != nullptr; obj = obj->next_content){
			if(obj->feed_file().compare(yaml_file.c_str()) == 0){
				mo_debug(green_str("found object of feed_file:'") << obj->feed_file() << "' " << yaml_file << "' in room:" << real_room(room));
				return optr_by_uuid(obj->uuid);
			}
		}
		mo_debug(red_str("creating object because we couldnt find one:'") << yaml_file << "' in room:" << room);
		auto obj = create_object(type, yaml_file);
		obj_to_room(obj.get(),real_room(room));
		return obj;
	}
	void feed_weapon_locker(room_vnum room){
		mo_debug("feeding weapon locker to room vnum:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker", std::to_string(room), values);
		auto locker = mods::integral_objects::first_or_create(room,"weapon-locker", ITEM_CONTAINER, "weapon-locker.yml");
		for(auto yaml : values){
			if(!mods::object_utils::assert_sane_object(yaml)){
				std::cerr << "[feed_weapon_locker]: not feeding invalid yaml type: '" << yaml << "'\n";
				continue;
			}
			mo_debug("[feed_weapon_locker]: feeding sane object:'" << yaml << "'");
			auto obj = create_object(mods::object_utils::get_yaml_type(yaml),EXPLODE(yaml.c_str(),'/')[1]);
			obj_to_obj(obj,locker);
		}
	}

	void feed_armor_locker(room_vnum room){
		mo_debug("feeding armor locker to room vnum:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker", std::to_string(room), values);
		auto locker = mods::integral_objects::first_or_create(room,"armor-locker", ITEM_CONTAINER, "armor-locker.yml");
		for(auto yaml : values){
			if(!mods::object_utils::assert_sane_object(yaml)){
				std::cerr << "[feed_armor_locker]: not feeding invalid yaml type: '" << yaml << "'\n";
				continue;
			}
			mo_debug("[feed_armor_locker]: feeding sane object:'" << yaml << "'");
			auto obj = create_object(mods::object_utils::get_yaml_type(yaml),EXPLODE(yaml.c_str(),'/')[1]);
			obj_to_obj(obj,locker);
		}
	}

	void feed_camera_feed(room_vnum room_v_num){
		mo_debug("feeding camera feed to room vnum:" << room_v_num << "| real room:" << real_room(room_v_num));
		std::vector<std::string> values;
		mods::db::get_section_vector("camera-feed", std::to_string(room_v_num), values);
		auto camera = mods::integral_objects::first_or_create(room_v_num,"camera-feed", ITEM_GADGET, "camera.yml");
		if(camera->has_gadget()){
			auto & vlist = camera->gadget()->attributes->vnum_list;
			if(vlist.size()){
				mo_debug("camera feed is already fed for room " << room_v_num << ", skipping");
				return;
			}
			vlist.clear();
			vlist.emplace_back(0);
			for(auto room_v_num_string : values){
				auto opt = mods::util::stoi(room_v_num_string);
				room_rnum r = real_room(opt.value_or(-1));
				if(r == NOWHERE){
					mo_debug("camera feed has invalid room_vnum:" << room_v_num << ", skipping");
					continue;
				}
				vlist.emplace_back(opt.value());
			}
			if(vlist.size() > 1){
				vlist[0] = 1;
			}
		}
	}

	void rotate_camera_feed(room_vnum room_v_num){
		mo_debug("feeding camera feed to room vnum:" << room_v_num << "| real room:" << real_room(room_v_num));
		std::vector<std::string> values;
		mods::db::get_section_vector("camera-feed", std::to_string(room_v_num), values);
		auto camera = mods::integral_objects::first_or_create(room_v_num,"camera-feed", ITEM_GADGET, "camera.yml");
		if(camera->has_gadget()){
			auto & vlist = camera->gadget()->attributes->vnum_list;
			vlist[0] = vlist[0] + 1;
			if(vlist[0] >= vlist.size()){
				vlist[0] = 1;
			}
		}
	}

	void remove_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("camera-feed",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
	void remove_weapon_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("weapon-locker",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}
	void remove_armor_locker(player_ptr_t& player, std::vector<std::string>& args){
		using namespace mods::db;
		auto status = delete_section_vector("armor-locker",std::to_string(world[player->room()].number));
		player->send("delete status: %d\r\n",status);
	}

#if 0
	void edit_object(player_ptr_t& player, const std::vector<std::string>& args){
	}

	void list_objects(player_ptr_t& player, const std::vector<std::string>& args){
		//int ctr = 0;
		player->sendln("Listing...");
		//for(auto & contents = world[player->room()].contents; contents != nullptr; contents = contents->next,++ctr){
	//		player->send("[%d] - %s\r\n", ctr, contents->name.c_str());
//		}
		player->sendln("Done listing.");
	}

	void mark_object(player_ptr_t& player, const std::vector<std::string>& args){
		//int ctr = 0;
		player->sendln("Listing...");
		//for(auto & contents = world[player->room()].contents; contents != nullptr; contents = contents->next,++ctr){
			//player->send("[%d] - %s\r\n", ctr, contents->name.c_str());
		//}
		player->sendln("Done listing.");
	}
#endif
};

ACMD(do_install_camera_feed){
	DO_HELP("install_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::save_camera_feed(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"camera-feed");
	ADMIN_DONE();
}

ACMD(do_uninstall_camera_feed){
	DO_HELP("uninstall_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::remove_camera_feed(player,vec_args);
	mods::zone::remove_replenish(world[player->room()].number,"camera-feed");
	ADMIN_DONE();
}

ACMD(do_install_computer_choice){
	DO_HELP("install_computer_choice");
	ADMIN_REJECT();
	/** code here */
	ADMIN_DONE();
}

ACMD(do_install_armor_locker){
	DO_HELP("install_armor_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::save_armor_locker(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"armor-locker");
	mods::zone::remove_replenish(world[player->room()].number,"armor-locker");
	ADMIN_DONE();
}

ACMD(do_install_weapon_locker){
	DO_HELP("install_weapon_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::save_weapon_locker(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"weapon-locker");
	mods::zone::remove_replenish(world[player->room()].number,"weapon-locker");
	ADMIN_DONE();
}
ACMD(do_uninstall_armor_locker){
	DO_HELP("uninstall_armor_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::remove_armor_locker(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_uninstall_weapon_locker){
	DO_HELP("uninstall_weapon_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::remove_weapon_locker(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_edit_object){
	DO_HELP("edit_object");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	//mods::integral_objects::edit_object(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_list_objects){
	DO_HELP("list_objects");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	//mods::integral_objects::list_objects(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_mark_object){
	DO_HELP("mark_object");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	//mods::integral_objects::mark_object(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_list_wear_flags){
	static const std::vector<std::string> flags = {
			"ABOUT", "ARMS", "BACKPACK",
			"BODY", "ELBOW_L", "ELBOW_R",
			"FEET", "FINGER_L", "FINGER_R",
			"GOGGLES", "HANDS", "HEAD",
			"HOLD", "LEGS", "LIGHT",
			"NECK_1", "NECK_2", "PRIMARY",
			"SECONDARY", "SHIELD", "SHOULDERS_L",
			"SHOULDERS_R", "VEST_PACK", "WAIST",
			"WEAPON_ATTACHMENT", "WIELD", "WRIST_L",
			"WRIST_R"
	};
	player->sendln("Listing...");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size()){
		mods::search_screen(player, flags, vec_args, 64);
		player->sendln("Done listing.");
		return;
	}
	for(auto & f : flags){
		player->sendln(f);
	}
	player->sendln("Done listing.");
	player->sendln(CAN_BE_SEARCHED());
}

namespace mods::integral_objects {
	void init(){
			//mods::interpreter::add_command("edit_object", POS_RESTING, do_edit_object, LVL_BUILDER,0);
			//mods::interpreter::add_command("list_objects", POS_RESTING, do_list_objects, LVL_BUILDER,0);
			mods::interpreter::add_command("list_wear_flags", POS_RESTING, do_list_wear_flags, LVL_BUILDER,0);

			mods::interpreter::add_command("install_weapon_locker", POS_RESTING, do_install_weapon_locker, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_weapon_locker", POS_RESTING, do_uninstall_weapon_locker, LVL_BUILDER,0);

			mods::interpreter::add_command("install_camera_feed", POS_RESTING, do_install_camera_feed, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_camera_feed", POS_RESTING, do_uninstall_camera_feed, LVL_BUILDER,0);

			mods::interpreter::add_command("install_armor_locker", POS_RESTING, do_install_armor_locker, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_armor_locker", POS_RESTING, do_uninstall_armor_locker, LVL_BUILDER,0);
	}
};
#undef mo_debug
