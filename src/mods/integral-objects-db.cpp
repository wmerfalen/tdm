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

#ifdef __MENTOC_MODS_INTEGRAL_OBJECTS_DEBUG__
#define mo_debug(A) std::cerr << "[mods::integral_objects][debug]:" << A <<"\n";
#else
#define mo_debug(A)
#endif

extern std::string sanitize_key(std::string key);
extern void obj_to_obj(obj_ptr_t from_object, obj_ptr_t to_object);
namespace mods::integral_objects_db {
	int write_db_values(std::string_view section_name, std::string_view prefix, const std::vector<std::string>& values){
		using namespace mods::db;
		std::string value = "";
		return put_section_vector(section_name.data(),prefix.data(), values);
	}
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
	void save_weapon_locker_quota(player_ptr_t& player, std::vector<std::string>& args){
		mo_debug("saving weapon locker quota");
		save_item_to_db(player, "weapon-locker-quota", args);
	}
	void save_armor_locker_quota(player_ptr_t& player, std::vector<std::string>& args){
		mo_debug("saving armor locker quota");
		save_item_to_db(player, "armor-locker-quota", args);
	}

	void save_catchy_name(player_ptr_t& player, std::string_view identifier, std::string_view name,std::string_view deep_object_description){
		mo_debug("saving catchy name for item");
		std::string value = "";
		std::string prefix = identifier.data();
		auto status = mods::db::put_section_vector("catchy-name",prefix, {name.data(),deep_object_description.data()});
		player->send("status: %d\r\n",status);
		std::vector<std::string> values;
		status = mods::db::get_section_vector("catchy-name",prefix,values);
		player->send("get status: %d\r\nTo confirm, we placed these values...\r\n",status);
		for(auto line : values){
			player->send("[item]: '%s'\r\n",line.c_str());
		}
		player->sendln("Done listing.");
	}

	void save_camera_feed(player_ptr_t& player, std::vector<std::string>& args){
		write_db_values("camera-feed",std::to_string(world[player->room()].number), args);
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

	obj_ptr_t first_or_create(room_vnum room,std::string query, int type, std::string yaml_file){
		mo_debug(green_str("[first_or_create]: room: ") << room << "| real:" << real_room(room));
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

	int armor_quota(room_vnum room){
		mo_debug("getting armor locker quota:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("armor-locker-quota", std::to_string(room), values);
		for(auto packed_yaml_info : values){
			return mods::util::stoi_optional<int>(packed_yaml_info).value_or(10);
		}
		return 10;
	}
	int weapon_quota(room_vnum room){
		mo_debug("getting weapon locker quota:" << room << "| real room:" << real_room(room));
		std::vector<std::string> values;
		mods::db::get_section_vector("weapon-locker-quota", std::to_string(room), values);
		for(auto packed_yaml_info : values){
			return mods::util::stoi_optional<int>(packed_yaml_info).value_or(10);
		}
		return 10;
	}

};

#undef mo_debug
