#ifndef __MENTOC_MODS_WORLD_CONFIGURATION_HEADER__
#define __MENTOC_MODS_WORLD_CONFIGURATION_HEADER__
#include "db.hpp"
#include "pq.hpp"
#include "sql.hpp"
#include <map>
#include <utility>
#include <string>
#include <sys/stat.h>
#include <string_view>

namespace mods::world_conf {
	constexpr const char* START_ROOM_TABLE = "world_configuration_start_rooms";

	using mutable_map_t = std::map<std::string,std::string>;
	std::pair<bool,std::string> update_or_insert(std::string_view table,
			mutable_map_t values,std::string_view where_column,
			std::string_view where_value);
	std::pair<bool,std::string> set_immortal_start_room(room_vnum room_id);
	std::pair<bool,std::string> set_mortal_start_room(room_vnum room_id);
};

#endif
