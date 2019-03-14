#ifndef __MENTOC_MODS_OVERHEAD_MAP_HEADER__
#define  __MENTOC_MODS_OVERHEAD_MAP_HEADER__

#include "../structs.h"
#include <vector>

extern std::vector<room_data> world;
namespace mods::overhead_map {
	template <typename OutputDevice>
	std::string_view generate(OutputDevice out,const room_rnum& room_number){
		return world[room_number].overhead(out->get_lense());
	}
};

#endif
