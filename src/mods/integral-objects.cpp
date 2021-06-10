#include "integral-objects.hpp"
#include "integral-objects-db.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "screen-searcher.hpp"
#include "db.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "zone.hpp"
#include "builder/object-placement.hpp"
#include "rifle-attachments.hpp"
#include "query-objects.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
#define mo_debug(A) std::cerr << red_str("[mods::integral_objects][debug]:") << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::integral_objects {
	std::size_t weapon_locker_quota(const obj_ptr_t& object) {
		return 10;
	}
	std::size_t armor_locker_quota(const obj_ptr_t& object) {
		return 10;
	}
	void feed_weapon_locker(room_vnum room) {
		mo_debug("feeding weapon locker to room vnum:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker", std::to_string(room), values);
		auto locker = mods::integral_objects_db::first_or_create(room,"weapon-locker", ITEM_CONTAINER, "weapon-locker.yml");
		for(auto yaml : values) {
			if(!mods::object_utils::assert_sane_object(yaml)) {
				mo_debug("[feed_weapon_locker]: not feeding invalid yaml type: '" << yaml << "'");
				continue;
			}
			mo_debug("[feed_weapon_locker]: checking if quota hit for yaml file(not deconstructed): '" << yaml);
			auto yaml_file = mods::object_utils::get_yaml_file(yaml);
			mo_debug("[feed_weapon_locker]: extracted yaml: '" << yaml_file << "'");
			auto uuid_list = mods::query_objects::query_contents_by_yaml(locker,yaml_file);
			if(uuid_list.size() < weapon_locker_quota(locker)) {
				mo_debug("[feed_weapon_locker]: quota not hit for object:'" << yaml << "', feeding to locker");
				auto obj = create_object(mods::object_utils::get_yaml_type(yaml),yaml_file);
				obj_to_obj(obj,locker);
			}
		}
	}

	void feed_armor_locker(room_vnum room) {
		mo_debug("feeding armor locker to room vnum:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker", std::to_string(room), values);
		auto locker = mods::integral_objects_db::first_or_create(room,"armor-locker", ITEM_CONTAINER, "armor-locker.yml");
		for(auto yaml : values) {
			if(!mods::object_utils::assert_sane_object(yaml)) {
				mo_debug("[feed_armor_locker]: not feeding invalid yaml type: '" << yaml << "'");
				continue;
			}
			mo_debug("[feed_armor_locker]: feeding sane object:'" << yaml << "'");
			auto yaml_file = mods::object_utils::get_yaml_file(yaml);
			auto uuid_list = mods::query_objects::query_contents_by_yaml(locker,yaml_file);
			if(uuid_list.size() < armor_locker_quota(locker)) {
				auto obj = create_object(mods::object_utils::get_yaml_type(yaml),yaml_file);
				obj_to_obj(obj,locker);
			}
		}
	}

	void feed_camera_feed(room_vnum room_v_num) {
		mo_debug("feeding camera feed to room vnum:" << room_v_num << "| real room:" << real_room(room_v_num));
		std::vector<std::string> values;
		mods::db::get_section_vector("camera-feed", std::to_string(room_v_num), values);
		auto camera = mods::integral_objects_db::first_or_create(room_v_num,"camera-feed", ITEM_GADGET, "camera.yml");
		if(camera->has_gadget()) {
			auto& vlist = camera->gadget()->attributes->vnum_list;
			if(vlist.size()) {
				mo_debug("camera feed is already fed for room " << room_v_num << ", skipping");
				return;
			}
			vlist.clear();
			vlist.emplace_back(0);
			for(auto room_v_num_string : values) {
				auto opt = mods::util::stoi(room_v_num_string);
				room_rnum r = real_room(opt.value_or(-1));
				if(r == NOWHERE) {
					mo_debug("camera feed has invalid room_vnum:" << room_v_num << ", skipping");
					continue;
				}
				vlist.emplace_back(opt.value());
			}
			if(vlist.size() > 1) {
				vlist[0] = 1;
			}
		}
	}

	void rotate_camera_feed(room_vnum room_v_num) {
		mo_debug("feeding camera feed to room vnum:" << room_v_num << "| real room:" << real_room(room_v_num));
		std::vector<std::string> values;
		mods::db::get_section_vector("camera-feed", std::to_string(room_v_num), values);
		auto camera = mods::integral_objects_db::first_or_create(room_v_num,"camera-feed", ITEM_GADGET, "camera.yml");
		if(camera->has_gadget()) {
			auto& vlist = camera->gadget()->attributes->vnum_list;
			vlist[0] = vlist[0] + 1;
			if(vlist[0] >= vlist.size()) {
				vlist[0] = 1;
			}
		}
	}


	template <typename TAttachments>
	TAttachments instantiate_catchy_name(std::string_view identifier) {
		std::vector<std::string> values;
		mods::db::get_section_vector("catchy-name",identifier.data(),values);
		if(values.size() > 1) {
			return TAttachments(values[1]);
		}
		return TAttachments("");
	}
};

SUPERCMD(do_install_camera_feed) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("install_camera_feed");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::save_camera_feed(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"camera-feed");
	ADMIN_DONE();
}

SUPERCMD(do_uninstall_camera_feed) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_camera_feed");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::remove_camera_feed(player,vec_args);
	mods::zone::remove_replenish(world[player->room()].number,"camera-feed");
	ADMIN_DONE();
}

SUPERCMD(do_install_computer_choice) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("install_computer_choice");
	/** code here */
	ADMIN_DONE();
}

SUPERCMD(do_install_armor_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("install_armor_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::save_armor_locker(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"armor-locker");
	mods::zone::remove_replenish(world[player->room()].number,"armor-locker");
	ADMIN_DONE();
}

SUPERCMD(do_install_weapon_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("install_weapon_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::save_weapon_locker(player,vec_args);
	mods::zone::register_replenish(world[player->room()].number,"weapon-locker");
	mods::zone::remove_replenish(world[player->room()].number,"weapon-locker");
	ADMIN_DONE();
}
SUPERCMD(do_uninstall_armor_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_armor_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::remove_armor_locker(player,vec_args);
	ADMIN_DONE();
}

SUPERCMD(do_uninstall_weapon_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_weapon_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::remove_weapon_locker(player,vec_args);
	ADMIN_DONE();
}

SUPERCMD(do_list_wear_flags) {
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
	if(vec_args.size()) {
		mods::search_screen(player, flags, vec_args, 64);
		player->sendln("Done listing.");
		return;
	}
	for(auto& f : flags) {
		player->sendln(f);
	}
	player->sendln("Done listing.");
	player->sendln(CAN_BE_SEARCHED());
}

SUPERCMD(do_armor_locker_quota) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("armor_locker_quota");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::save_armor_locker_quota(player,vec_args);
	ADMIN_DONE();
}
SUPERCMD(do_weapon_locker_quota) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("weapon_locker_quota");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::integral_objects_db::save_weapon_locker_quota(player,vec_args);
	ADMIN_DONE();
}

SUPERCMD(do_create_catchy_name) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("create_catchy_name");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char* usage = "usage: create_catchy_name <identifier-with-no-spaces> <catchy-name> <deep-object-description>";
	if(vec_args.size() < 3) {
		player->errorln(usage);
		return;
	}
	mods::integral_objects_db::save_catchy_name(player,vec_args[0],vec_args[1],vec_args[2]);
	ADMIN_DONE();
}

SUPERCMD(do_instantiate_catchy_name) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("instantiate_catchy_name");
	/** code here */
	auto vec_args = PARSE_ARGS();
	static constexpr const char* usage = "usage: instantiate_catchy_name <identifier-with-no-spaces>";
	if(vec_args.size() < 1) {
		player->errorln(usage);
		return;
	}
	auto rifle = mods::integral_objects::instantiate_catchy_name<mods::rifle_attachments_t>(vec_args[0]);
	ADMIN_DONE();
}

SUPERCMD(do_install_sign) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("install_sign");
	/** code here */
	auto vec_args = PARSE_ARGS();
	save_item_to_db(player, "sign", vec_args);
	ADMIN_DONE();
}
SUPERCMD(do_uninstall_sign) {
	DO_HELP_WITH_ZERO("uninstall_sign");
	/** code here */
	auto vec_args = PARSE_ARGS();
	auto status = mods::db::delete_section_vector("sign",std::to_string(world[player->room()].number));
	player->send("delete status: %d\r\n",status);
	ADMIN_DONE();
}

namespace mods::integral_objects {
	void init() {
		mods::interpreter::add_command("list_wear_flags", POS_RESTING, do_list_wear_flags, LVL_BUILDER,0);

		mods::interpreter::add_command("install_weapon_locker", POS_RESTING, do_install_weapon_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("uninstall_weapon_locker", POS_RESTING, do_uninstall_weapon_locker, LVL_BUILDER,0);

		mods::interpreter::add_command("install_camera_feed", POS_RESTING, do_install_camera_feed, LVL_BUILDER,0);
		mods::interpreter::add_command("uninstall_camera_feed", POS_RESTING, do_uninstall_camera_feed, LVL_BUILDER,0);

		mods::interpreter::add_command("install_armor_locker", POS_RESTING, do_install_armor_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("uninstall_armor_locker", POS_RESTING, do_uninstall_armor_locker, LVL_BUILDER,0);

		mods::interpreter::add_command("armor_locker_quota", POS_RESTING, do_armor_locker_quota, LVL_BUILDER,0);
		mods::interpreter::add_command("weapon_locker_quota", POS_RESTING, do_weapon_locker_quota, LVL_BUILDER,0);

		mods::interpreter::add_command("create_catchy_name", POS_RESTING, do_create_catchy_name, LVL_BUILDER,0);
		mods::interpreter::add_command("instantiate_catchy_name", POS_RESTING, do_instantiate_catchy_name, LVL_BUILDER,0);

		mods::interpreter::add_command("install_sign", POS_RESTING, do_install_sign, LVL_BUILDER,0);
		mods::interpreter::add_command("uninstall_sign", POS_RESTING, do_uninstall_sign, LVL_BUILDER,0);
	}
};
#undef mo_debug
