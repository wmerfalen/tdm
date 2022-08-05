#ifndef __MENTOC_MODS_QUERY_OBJECTS_HEADER__
#define __MENTOC_MODS_QUERY_OBJECTS_HEADER__

#include "../globals.hpp"

namespace mods::query_objects {
	obj_ptr_t query_room_for_object(const room_rnum& room, std::string_view name);
	std::size_t query_count_contents_by_yaml(obj_ptr_t& found_object,std::string_view file);
	std::vector<uuid_t> query_inventory_for_object(player_ptr_t& player, std::string_view name);
	std::vector<uuid_t> query_room_for_object_by_yaml(const room_rnum& room, std::string_view name);
	std::vector<uuid_t> query_room_for_object_by_yaml_multi(const room_rnum& room, const std::vector<std::string>& files);
	std::vector<uuid_t> query_contents_by_yaml(obj_ptr_t& found_object,std::string_view file);
	std::vector<uuid_t> query_contents_by_yaml_multi(obj_ptr_t& found_object,const std::vector<std::string>& files);
	std::vector<uuid_t> query_contents_by_yaml_multi(room_rnum room,std::string_view container,const std::vector<std::string>& files);
	std::vector<uuid_t> query_inventory_by_yaml(player_ptr_t& player, std::string_view yaml_file);
	std::vector<uuid_t> query_inventory_container_by_yaml(player_ptr_t& player, std::string_view container, std::string_view yaml_file);
	std::size_t in_container_by_yaml(uuid_t container,std::string_view yaml);
	std::vector<uuid_t> query_container(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_room(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_inventory(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_zone(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_equipment_by_yaml(player_ptr_t& player, std::string_view yaml_file);
	bool room_has_object_uuid(const room_rnum&,const uuid_t& obj_uuid);
	bool room_has_vehicle(const room_rnum& room_id);
	void init();
};
#endif
