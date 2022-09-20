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
#include "orm/locker.hpp"
#include "../handler.h"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
	#define mo_debug(A) std::cerr << "[mods::integral_objects]:" << A <<"\n";
#else
	#define mo_debug(A)
#endif


#ifdef __MENTOC_MODS_WEAPONS_INTEGRAL_OBJECTS_DEBUG__
	#define mw_debug(A) std::cerr << red_str("[mods::integral_objects::weapons_locker]:") << A <<"\n";
#else
	#define mw_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::integral_objects {
	void generic_feed(std::string_view type,const room_vnum& room,std::string_view query,std::string_view container_yaml) {
		mw_debug("generic_feed " << type << " entry. room: " << room);
		auto locker = mods::integral_objects_db::first_or_create(room,query.data(), ITEM_CONTAINER, container_yaml.data());
		const std::vector<std::pair<uint16_t,std::string>>& list = mods::orm::locker::contents(type,room);
		for(const auto& pair : list) {
			auto yaml = pair.second;
			if(!mods::object_utils::assert_sane_object(yaml)) {
				log("Not a sane yaml file (generic_feed): '%s'",yaml.c_str());
				continue;
			}
			auto yaml_file = mods::object_utils::get_yaml_file(yaml);
			auto uuid_list = mods::query_objects::query_contents_by_yaml(locker,yaml_file);
			if(uuid_list.size() < pair.first) {
				mw_debug("Filling with: " << yaml);
				auto obj = create_object(mods::object_utils::get_yaml_type(yaml),yaml_file);
				obj_to_obj(obj,locker);
			}
		}
	}
	void feed_ammo_locker(room_vnum room) {
		generic_feed("ammo",room,"ammo-locker","ammo-locker.yml");
	}
	void feed_weapon_locker(room_vnum room) {
		mw_debug("feed_weapon_locker entry. room: " << room);
		generic_feed("weapon",room,"weapon-locker","weapon-locker.yml");
	}

	void feed_armor_locker(room_vnum room) {
		generic_feed("armor",room,"armor-locker","armor-locker.yml");
	}

	void feed_camera_feed(room_vnum room_v_num) {
		std::vector<std::string> values;
		mods::db::get_section_vector("camera-feed", std::to_string(room_v_num), values);
		auto camera = mods::integral_objects_db::first_or_create(room_v_num,"camera-feed", ITEM_GADGET, "camera.yml");
		if(camera->has_gadget()) {
			auto& vlist = camera->gadget()->attributes->vnum_list;
			if(vlist.size()) {
				return;
			}
			vlist.clear();
			vlist.emplace_back(0);
			for(auto room_v_num_string : values) {
				auto opt = mods::util::stoi(room_v_num_string);
				room_rnum r = real_room(opt.value_or(-1));
				if(r == NOWHERE) {
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
	static constexpr std::string_view usage = "Usage: admin:armor-locker:install <quota> <yaml>...[yaml-N]";
	DO_HELP_WITH_ZERO("admin:armor-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	if(!intat(0)) {
		player->errorln(CAT("Expecting quota as first argument.", usage.data()).c_str());
		return;
	}
	if(!argshave()->size_gt(1)->passed()) {
		player->errorln(usage.data());
		return;
	}
	auto count = intat(0);
	for(unsigned i=1; i < vec_args.size(); i++) {
		auto status = mods::orm::locker::place_locker_item("armor",world[player->room()].number, vec_args[i],count);
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Saved: '",vec_args[i],"'"));
		}
	}

	mods::zone::remove_replenish(world[player->room()].number,"armor-locker");
	mods::zone::register_replenish(world[player->room()].number,"armor-locker");
	ADMIN_DONE();
}

SUPERCMD(do_install_weapon_locker) {
	ADMIN_REJECT();
	static constexpr std::string_view usage = "Usage: admin:weapon-locker:install <quota> <yaml>...[yaml-N]";
	DO_HELP_WITH_ZERO("admin:weapon-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	if(!intat(0)) {
		player->errorln(CAT("Expecting quota as first argument.", usage.data()).c_str());
		return;
	}
	if(!argshave()->size_gt(1)->passed()) {
		player->errorln(usage.data());
		return;
	}
	auto count = intat(0);
	for(unsigned i=1; i < vec_args.size(); i++) {
		auto status = mods::orm::locker::place_locker_item("weapon",world[player->room()].number, vec_args[i],count);
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Saved: '",vec_args[i],"'"));
		}
	}

	mods::zone::remove_replenish(world[player->room()].number,"weapon-locker");
	mods::zone::register_replenish(world[player->room()].number,"weapon-locker");
	ADMIN_DONE();
}
SUPERCMD(do_install_weapon_locker_object) {
	mods::zone::build_weapon_locker(world[player->room()].number);
}
SUPERCMD(do_install_ammo_locker_object) {
	mods::zone::build_ammo_locker(world[player->room()].number);
}
SUPERCMD(do_install_armor_locker_object) {
	mods::zone::build_armor_locker(world[player->room()].number);
}
SUPERCMD(do_install_ammo_locker) {
	ADMIN_REJECT();
	static constexpr std::string_view usage = "Usage: admin:ammo-locker:install <quota> <yaml>...[yaml-N]";
	DO_HELP_WITH_ZERO("admin:ammo-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	if(!intat(0)) {
		player->errorln(CAT("Expecting quota as first argument.", usage.data()).c_str());
		return;
	}
	if(!argshave()->size_gt(1)->passed()) {
		player->errorln(usage.data());
		return;
	}
	auto count = intat(0);
	for(unsigned i=1; i < vec_args.size(); i++) {
		auto status = mods::orm::locker::place_locker_item("ammo",world[player->room()].number, vec_args[i],count);
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Saved: '",vec_args[i],"'"));
		}
	}

	mods::zone::remove_replenish(world[player->room()].number,"ammo-locker");
	mods::zone::register_replenish(world[player->room()].number,"ammo-locker");
	ADMIN_DONE();
}
SUPERCMD(do_list_weapon_locker) {
	ADMIN_REJECT();
	player->sendln("[Listing]...");
	for(const auto& line : mods::orm::locker::list_locker_by_type("weapon",world[player->room()].number)) {
		player->sendln(line);
	}
	player->sendln("Done listing.");
	ADMIN_DONE();
}
SUPERCMD(do_list_ammo_locker) {
	ADMIN_REJECT();
	player->sendln("[Listing]...");
	for(const auto& line : mods::orm::locker::list_locker_by_type("ammo",world[player->room()].number)) {
		player->sendln(line);
	}
	player->sendln("Done listing.");
	ADMIN_DONE();
}
SUPERCMD(do_list_armor_locker) {
	ADMIN_REJECT();
	player->sendln("[Listing]...");
	for(const auto& line : mods::orm::locker::list_locker_by_type("armor",world[player->room()].number)) {
		player->sendln(line);
	}
	player->sendln("Done listing.");
	ADMIN_DONE();
}
SUPERCMD(do_remove_weapon_locker_item) {
	ADMIN_REJECT();
	static constexpr std::string_view usage = "Usage: admin:weapon-locker:remove:item <id>...[item-N]";
	//DO_HELP_WITH_ZERO("admin:weapon-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	for(unsigned i=0; i < vec_args.size(); i++) {
		auto opt_id = mods::util::stoi_optional<uint64_t>(vec_args[i]);
		if(opt_id.has_value() == false) {
			player->errorln(CAT("Expecting integer but got: '",vec_args[i],"' for argument number:",i + 1,". Please look at the output of admin:weapon-locker:list for the list of ID's.. ", usage.data()).c_str());
			continue;
		}
		auto status = mods::orm::locker::remove_item_by_id(opt_id.value());
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Removed: '",vec_args[i],"'"));
		}
	}
	mods::orm::locker::perform_cleanup();

	ADMIN_DONE();
}
SUPERCMD(do_remove_ammo_locker_item) {
	ADMIN_REJECT();
	static constexpr std::string_view usage = "Usage: admin:ammo-locker:remove:item <id>...[item-N]";
	//DO_HELP_WITH_ZERO("admin:ammo-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	for(unsigned i=0; i < vec_args.size(); i++) {
		auto opt_id = mods::util::stoi_optional<uint64_t>(vec_args[i]);
		if(opt_id.has_value() == false) {
			player->errorln(CAT("Expecting integer but got: '",vec_args[i],"' for argument number:",i + 1,". Please look at the output of admin:ammo-locker:list for the list of ID's.. ", usage.data()).c_str());
			continue;
		}
		auto status = mods::orm::locker::remove_item_by_id(opt_id.value());
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Removed: '",vec_args[i],"'"));
		}
	}
	mods::orm::locker::perform_cleanup();

	ADMIN_DONE();
}
SUPERCMD(do_remove_armor_locker_item) {
	ADMIN_REJECT();
	static constexpr std::string_view usage = "Usage: admin:armor-locker:remove:item <id>...[item-N]";
	//DO_HELP_WITH_ZERO("admin:armor-locker:install");
	/** code here */
	auto vec_args = PARSE_ARGS();
	for(unsigned i=0; i < vec_args.size(); i++) {
		auto opt_id = mods::util::stoi_optional<uint64_t>(vec_args[i]);
		if(opt_id.has_value() == false) {
			player->errorln(CAT("Expecting integer but got: '",vec_args[i],"' for argument number:",i + 1,". Please look at the output of admin:armor-locker:list for the list of ID's.. ", usage.data()).c_str());
			continue;
		}
		auto status = mods::orm::locker::remove_item_by_id(opt_id.value());
		if(!std::get<0>(status)) {
			player->errorln(CAT("Error: '",std::get<1>(status),"'").c_str());
		} else {
			player->admin_success(CAT("Removed: '",vec_args[i],"'"));
		}
	}
	mods::orm::locker::perform_cleanup();

	ADMIN_DONE();
}
SUPERCMD(do_uninstall_armor_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_armor_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::orm::locker::remove_locker("armor",world[player->room()].number);
	mods::zone::remove_replenish(world[player->room()].number,"armor-locker");
	std::vector<uuid_t> list = mods::query_objects::query_room_for_object_by_yaml(player->room(),"armor-locker.yml");
	for(const auto& uuid : list) {
		mods::globals::dispose_object(uuid);
		player->sendln(CAT("Removed ",uuid,"..."));
	}
	ADMIN_DONE();
}

SUPERCMD(do_uninstall_weapon_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_weapon_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::orm::locker::remove_locker("weapon",world[player->room()].number);
	mods::zone::remove_replenish(world[player->room()].number,"weapon-locker");
	std::vector<uuid_t> list = mods::query_objects::query_room_for_object_by_yaml(player->room(),"weapon-locker.yml");
	for(const auto& uuid : list) {
		mods::globals::dispose_object(uuid);
		player->sendln(CAT("Removed ",uuid,"..."));
	}
	ADMIN_DONE();
}
SUPERCMD(do_uninstall_ammo_locker) {
	ADMIN_REJECT();
	DO_HELP_WITH_ZERO("uninstall_ammo_locker");
	/** code here */
	auto vec_args = PARSE_ARGS();
	mods::orm::locker::remove_locker("ammo",world[player->room()].number);
	mods::zone::remove_replenish(world[player->room()].number,"ammo-locker");
	std::vector<uuid_t> list = mods::query_objects::query_room_for_object_by_yaml(player->room(),"ammo-locker.yml");
	for(const auto& uuid : list) {
		mods::globals::dispose_object(uuid);
		player->sendln(CAT("Removed ",uuid,"..."));
	}
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
	player->sendln(CAT("delete status: ",status));
	ADMIN_DONE();
}
static const std::vector<std::string> weapon_locker = {
	"admin:weapon-locker:install",
	"admin:weapon-locker:install:container",
	"admin:weapon-locker:list",
	"admin:weapon-locker:remove:item",
	"admin:weapon-locker:uninstall",
};
SUPERCMD(do_weapon_locker_help) {
	ADMIN_REJECT();
	for(const auto& command : weapon_locker) {
		player->sendln(command);
	}
	ADMIN_DONE();
}
static const std::vector<std::string> ammo_locker = {
	"admin:ammo-locker:install",
	"admin:ammo-locker:install:container",
	"admin:ammo-locker:list",
	"admin:ammo-locker:remove:item",
	"admin:ammo-locker:uninstall",
};
SUPERCMD(do_ammo_locker_help) {
	ADMIN_REJECT();
	for(const auto& command : ammo_locker) {
		player->sendln(command);
	}
	ADMIN_DONE();
}
static const std::vector<std::string> armor_locker = {
	"admin:armor-locker:install",
	"admin:armor-locker:install:container",
	"admin:armor-locker:list",
	"admin:armor-locker:remove:item",
	"admin:armor-locker:uninstall",
};
SUPERCMD(do_armor_locker_help) {
	ADMIN_REJECT();
	for(const auto& command : armor_locker) {
		player->sendln(command);
	}
	ADMIN_DONE();
}

static const std::vector<std::string> catchy_name = {
	"admin:catchy-name:create",
	"admin:catchy-name:instantiate",
};
SUPERCMD(do_catchy_name_help) {
	ADMIN_REJECT();
	for(const auto& command : catchy_name) {
		player->sendln(command);
	}
	ADMIN_DONE();
}

static const std::vector<std::string> sign = {
	"admin:sign:install",
	"admin:sign:uninstall",
};

SUPERCMD(do_sign_help) {
	ADMIN_REJECT();
	for(const auto& command : sign) {
		player->sendln(command);
	}
	ADMIN_DONE();
}

static const std::vector<std::string> camera_feed = {
	"admin:camera-feed:install",
	"admin:camera-feed:uninstall",
};

SUPERCMD(do_camera_feed_help) {
	ADMIN_REJECT();
	for(const auto& command : camera_feed) {
		player->sendln(command);
	}
	ADMIN_DONE();
}


namespace mods::integral_objects {
	void init() {
		mods::interpreter::add_command("admin:wear-flags:list", POS_RESTING, do_list_wear_flags, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:weapon-locker:install", POS_RESTING, do_install_weapon_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:weapon-locker:install:container", POS_RESTING, do_install_weapon_locker_object, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:weapon-locker:list", POS_RESTING, do_list_weapon_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:weapon-locker:remove:item", POS_RESTING, do_remove_weapon_locker_item, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:weapon-locker:uninstall", POS_RESTING, do_uninstall_weapon_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:weapon-locker:help", POS_RESTING, do_weapon_locker_help, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:ammo-locker:install", POS_RESTING, do_install_ammo_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:ammo-locker:install:container", POS_RESTING, do_install_ammo_locker_object, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:ammo-locker:list", POS_RESTING, do_list_ammo_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:ammo-locker:remove:item", POS_RESTING, do_remove_ammo_locker_item, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:ammo-locker:uninstall", POS_RESTING, do_uninstall_ammo_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:ammo-locker:help", POS_RESTING, do_ammo_locker_help, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:armor-locker:install", POS_RESTING, do_install_armor_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:armor-locker:install:container", POS_RESTING, do_install_armor_locker_object, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:armor-locker:list", POS_RESTING, do_list_armor_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:armor-locker:remove:item", POS_RESTING, do_remove_armor_locker_item, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:armor-locker:uninstall", POS_RESTING, do_uninstall_armor_locker, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:armor-locker:help", POS_RESTING, do_armor_locker_help, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:camera-feed:install", POS_RESTING, do_install_camera_feed, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:camera-feed:uninstall", POS_RESTING, do_uninstall_camera_feed, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:camera-feed:help", POS_RESTING, do_camera_feed_help, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:catchy-name:create", POS_RESTING, do_create_catchy_name, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:catchy-name:instantiate", POS_RESTING, do_instantiate_catchy_name, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:catchy-name:help", POS_RESTING, do_catchy_name_help, LVL_BUILDER,0);

		mods::interpreter::add_command("admin:sign:install", POS_RESTING, do_install_sign, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:sign:uninstall", POS_RESTING, do_uninstall_sign, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:sign:help", POS_RESTING, do_sign_help, LVL_BUILDER,0);
	}
};
#undef mo_debug
