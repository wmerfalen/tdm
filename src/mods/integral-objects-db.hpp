#ifndef __MENTOC_MODS_INTEGRAL_OBJECTS_DB_HEADER__
#define __MENTOC_MODS_INTEGRAL_OBJECTS_DB_HEADER__

#include "../globals.hpp"
#include "orm/integral-object.hpp"
#include "db.hpp"

namespace mods::integral_objects_db {
	void save_weapon_locker(player_ptr_t& player, std::vector<std::string>& args);
	void save_armor_locker(player_ptr_t& player, std::vector<std::string>& args);
	void save_weapon_locker_quota(player_ptr_t& player, std::vector<std::string>& args);
	void save_armor_locker_quota(player_ptr_t& player, std::vector<std::string>& args);
	void save_catchy_name(player_ptr_t& player, std::string_view identifier, std::string_view name,std::string_view deep_object_description);

	void save_camera_feed(player_ptr_t& player, std::vector<std::string>& args);
	void remove_camera_feed(player_ptr_t& player, std::vector<std::string>& args);
	void remove_weapon_locker(player_ptr_t& player, std::vector<std::string>& args);
	void remove_armor_locker(player_ptr_t& player, std::vector<std::string>& args);
	obj_ptr_t first_or_create(room_vnum room,std::string query, int type, std::string yaml_file);
	int armor_quota(room_vnum room);
	int weapon_quota(room_vnum room);
};
#endif
