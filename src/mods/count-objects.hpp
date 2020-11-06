#ifndef __MENTOC_MODS_COUNT_OBJECTS_HEADER__
#define __MENTOC_MODS_COUNT_OBJECTS_HEADER__

#include "../globals.hpp"

namespace mods::count_objects {
	std::size_t in_container_by_yaml(uuid_t container,std::string_view yaml);
	std::vector<uuid_t> query_container(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_room(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_inventory(player_ptr_t& player, std::vector<std::string>& vec_args);
	std::vector<uuid_t> query_zone(player_ptr_t& player, std::vector<std::string>& vec_args);
	void init();
};
#endif
