#include "integral-objects-db.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "orm/integral-object.hpp"
#include "screen-searcher.hpp"
#include "util.hpp"
#include "object-utils.hpp"
#include "zone.hpp"
#include "builder/object-placement.hpp"
#include "rifle-attachments.hpp"
#include "orm/locker.hpp"

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
	#define mo_debug(A) std::cerr << "[mods::integral_objects][debug]:" << A <<"\n";
#else
	#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::integral_objects_db {
	void save_ammo_locker(player_ptr_t& player, std::vector<std::string>& args) {
		mo_debug("saving ammo locker");
		mods::orm::locker::place_locker("ammo",world[player->room()].number, args);
	}
	void save_weapon_locker(player_ptr_t& player, std::vector<std::string>& args) {
		mo_debug("saving weapon locker");
		mods::orm::locker::place_locker("weapon",world[player->room()].number, args);
	}

	bool save_locker_item_by_type(player_ptr_t& player,std::string_view type,std::string_view yaml, uint16_t count) {
		auto s = mods::orm::locker::place_locker_item(type,world[player->room()].number,yaml,count);
		if(!std::get<0>(s)) {
			player->errorln(CAT("Encountered issue: '",std::get<1>(s),"'").c_str());
			return false;
		}
		player->admin_success("saved");
		return true;
	}
	void save_armor_locker(player_ptr_t& player, std::vector<std::string>& args) {
		mo_debug("saving armor locker");
		mods::orm::locker::place_locker("armor",world[player->room()].number, args);
	}
	void set_quota(player_ptr_t& player, std::string_view type,std::vector<std::string>& args) {
		mo_debug("saving weapon locker quota");
		if(args.size() == 0) {
			player->errorln("Must specify atleast one argument which is the size");
			return;
		}
		auto opt_count = mods::util::stoi(args[0]);
		if(opt_count.has_value() == false) {
			player->errorln("Must specify a valid integer as count");
			return;
		}
		mods::orm::locker::set_quota(type,world[player->room()].number,opt_count.value());
	}
	void set_quota_matching(player_ptr_t& player, std::string_view type,std::vector<std::string>& args) {
		mo_debug("saving weapon locker quota");
		if(args.size() == 0) {
			player->errorln("Must specify atleast one argument which is the size");
			return;
		}
		auto opt_count = mods::util::stoi(args[0]);
		if(opt_count.has_value() == false) {
			player->errorln("Must specify a valid integer as count");
			return;
		}
		mods::orm::locker::set_quota(type,world[player->room()].number,opt_count.value());
	}
	void save_weapon_locker_quota(player_ptr_t& player, std::vector<std::string>& args) {
		mo_debug("saving weapon locker quota");
		set_quota(player,"weapon",args);
	}

	void save_armor_locker_quota(player_ptr_t& player, std::vector<std::string>& args) {
		mo_debug("saving armor locker quota");
		set_quota(player,"armor",args);
	}

	void save_catchy_name(player_ptr_t& player, std::string_view identifier, std::string_view name,std::string_view deep_object_description) {
		mo_debug("saving catchy name for item");
		std::string value = "";
		std::string prefix = identifier.data();
		auto status = mods::db::put_section_vector("catchy-name",prefix, {name.data(),deep_object_description.data()});
		player->sendln(
		    CAT(
		        "status: ",status
		    )
		);
		std::vector<std::string> values;
		status = mods::db::get_section_vector("catchy-name",prefix,values);
		player->sendln(
		    CAT(
		        "get status: ",status,"\r\nTo confirm, we placed these values..."
		    )
		);
		for(auto line : values) {
			player->sendln(CAT("[item]: '",line.c_str(),"'"));
		}
		player->sendln("Done listing.");
	}

	void save_camera_feed(player_ptr_t& player, std::vector<std::string>& args) {
		write_db_values("camera-feed",std::to_string(world[player->room()].number), args);
	}
	void remove_camera_feed(player_ptr_t& player, std::vector<std::string>& args) {
		using namespace mods::db;
		auto status = delete_section_vector("camera-feed",std::to_string(world[player->room()].number));
		player->sendln(
		    CAT(
		        "delete status: ",status
		    )
		);
	}
	void remove_weapon_locker(player_ptr_t& player, std::vector<std::string>& args) {
		using namespace mods::db;
		auto status = delete_section_vector("weapon-locker",std::to_string(world[player->room()].number));
		player->sendln(
		    CAT(
		        "delete status: ",status
		    )
		);
		mods::orm::locker::remove_locker("weapon",world[player->room()].number);
	}
	void remove_ammo_locker(player_ptr_t& player, std::vector<std::string>& args) {
		using namespace mods::db;
		auto status = delete_section_vector("ammo-locker",std::to_string(world[player->room()].number));
		player->sendln(
		    CAT(
		        "delete status: ",status
		    )
		);
		mods::orm::locker::remove_locker("ammo",world[player->room()].number);
	}
	void remove_armor_locker(player_ptr_t& player, std::vector<std::string>& args) {
		using namespace mods::db;
		auto status = delete_section_vector("armor-locker",std::to_string(world[player->room()].number));
		player->sendln(
		    CAT(
		        "delete status: ",status
		    )
		);
		mods::orm::locker::remove_locker("armor",world[player->room()].number);
	}

	obj_ptr_t first_or_create(room_vnum room,std::string query, int type, std::string yaml_file) {
		mo_debug(green_str("[first_or_create]: room: ") << room << "| real:" << real_room(room));
		for(auto obj = world[real_room(room)].contents; obj != nullptr; obj = obj->next_content) {
			if(obj->feed_file().compare(yaml_file.c_str()) == 0) {
				mo_debug(green_str("found object of feed_file:'") << obj->feed_file() << "' " << yaml_file << "' in room:" << real_room(room));
				return optr_by_uuid(obj->uuid);
			}
		}
		mo_debug(red_str("creating object because we couldnt find one:'") << yaml_file << "' in room:" << room);
		auto obj = create_object(type, yaml_file);
		obj_to_room(obj.get(),real_room(room));
		return obj;
	}

	int armor_quota(room_vnum room) {
		mo_debug("getting armor locker quota:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker-quota", std::to_string(room), values);
		for(auto packed_yaml_info : values) {
			return mods::util::stoi_optional<int>(packed_yaml_info).value_or(10);
		}
		return ARMOR_LOCKER_QUOTA();
	}
	int weapon_quota(room_vnum room) {
		mo_debug("getting weapon locker quota:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker-quota", std::to_string(room), values);
		for(auto packed_yaml_info : values) {
			return mods::util::stoi_optional<int>(packed_yaml_info).value_or(10);
		}
		return WEAPON_LOCKER_QUOTA();
	}

};

#undef mo_debug
