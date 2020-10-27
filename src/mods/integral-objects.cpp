#include "integral-objects.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"

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
		using namespace mods::db;
		std::string value = "";
		std::string prefix = std::to_string(world[player->room()].number);
		auto status = put_section_vector(section_name,prefix, args);
		player->send("status: %d\r\n",status);
		std::vector<std::string> values;
		status = get_section_vector(section_name,prefix,values);
		player->send("get status: %d\r\nTo confirm, we placed these values...\r\n",status);
		for(auto line : values){
			player->send("[item]: '%s'\r\n",line.c_str());
		}
		player->sendln("Done listing.");
	}
	void save_weapon_locker(player_ptr_t& player, std::vector<std::string>& args){
		save_item_to_db(player, "weapon-locker", args);
	}
	void save_armor_locker(player_ptr_t& player, std::vector<std::string>& args){
		save_item_to_db(player, "armor-locker", args);
	}
	int get_yaml_type(std::string& yaml){
		auto exploded = EXPLODE(yaml.c_str(),'/');
		return mods::util::yaml_string_to_int(exploded[0]);
	}
	obj_ptr_t first_or_create(room_rnum room,std::string query, int type, std::string yaml_file){
		for(auto obj = world[room].contents; obj != nullptr; obj = obj->next){
			if(obj->matches_query(query)){
				return optr(obj);
			}
		}
		return create_object(type, yaml_file);
	}
	void save_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		write_db_values("camera-feed",std::to_string(world[player->room()].number), args);
	}
	void feed_weapon_locker(room_vnum room){
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker", std::to_string(room), values);
		auto locker = first_or_create(real_room(room),"weapon-locker", ITEM_CONTAINER, "weapon-locker.yml");
		for(auto yaml : values){
			for(int i=0; i < 10;i++){
				auto obj = create_object(get_yaml_type(yaml),yaml);
				obj_to_obj(obj,locker);
			}
		}
	}
	void feed_armor_locker(room_vnum room){
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker", std::to_string(room), values);
		auto locker = first_or_create(real_room(room),"armor-locker", ITEM_CONTAINER, "armor-locker.yml");
		for(auto yaml : values){
			auto obj = create_object(get_yaml_type(yaml),yaml);
			for(int i=0; i < 10;i++){
				obj_to_obj(obj,locker);
			}
		}
	}
	void feed_weapon_locker(std::vector<std::string>& values){

	}
	void feed_armor_locker(std::vector<std::string>& values){

	}
	void feed_camera_feed(room_vnum room_id,std::vector<std::string>& values){

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

	void edit_object(player_ptr_t& player, const std::vector<std::string>& args){
	}

	void list_objects(player_ptr_t& player, const std::vector<std::string>& args){
		int ctr = 0;
		player->sendln("Listing...");
		for(auto & contents = world[player->room()].contents; contents != nullptr; contents = contents->next,++ctr){
			player->send("[%d] - %s\r\n", ctr, contents->name.c_str());
		}
		player->sendln("Done listing.");
	}

	void mark_object(player_ptr_t& player, const std::vector<std::string>& args){
		int ctr = 0;
		player->sendln("Listing...");
		for(auto & contents = world[player->room()].contents; contents != nullptr; contents = contents->next,++ctr){
			player->send("[%d] - %s\r\n", ctr, contents->name.c_str());
		}
		player->sendln("Done listing.");
	}
};

ACMD(do_install_camera_feed){
	DO_HELP("install_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::save_camera_feed(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_uninstall_camera_feed){
	DO_HELP("uninstall_camera_feed");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::remove_camera_feed(player,vec_args);
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
	ADMIN_DONE();
}

ACMD(do_install_weapon_locker){
	DO_HELP("install_weapon_locker");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::save_weapon_locker(player,vec_args);
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
	mods::integral_objects::edit_object(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_list_objects){
	DO_HELP("list_objects");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::list_objects(player,vec_args);
	ADMIN_DONE();
}

ACMD(do_mark_object){
	DO_HELP("mark_object");
	ADMIN_REJECT();
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects::mark_object(player,vec_args);
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
			mods::interpreter::add_command("edit_object", POS_RESTING, do_edit_object, LVL_BUILDER,0);
			mods::interpreter::add_command("list_objects", POS_RESTING, do_list_objects, LVL_BUILDER,0);
			mods::interpreter::add_command("list_wear_flags", POS_RESTING, do_list_wear_flags, LVL_BUILDER,0);

			mods::interpreter::add_command("install_weapon_locker", POS_RESTING, do_install_weapon_locker, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_weapon_locker", POS_RESTING, do_uninstall_weapon_locker, LVL_BUILDER,0);

			mods::interpreter::add_command("install_camera_feed", POS_RESTING, do_install_camera_feed, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_camera_feed", POS_RESTING, do_uninstall_camera_feed, LVL_BUILDER,0);

			mods::interpreter::add_command("install_armor_locker", POS_RESTING, do_install_armor_locker, LVL_BUILDER,0);
			mods::interpreter::add_command("uninstall_armor_locker", POS_RESTING, do_uninstall_armor_locker, LVL_BUILDER,0);
	}
};
