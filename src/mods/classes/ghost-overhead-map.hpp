#ifndef  __MENTOC_MODS_CLASSES_GHOST_OVERHEAD_MAP_HEADER__
#define  __MENTOC_MODS_CLASSES_GHOST_OVERHEAD_MAP_HEADER__

#include "../../structs.h"
#include "../../room-data.hpp"
#include <vector>
#include <set>
#include <tuple>

extern std::deque<room_data> world;
namespace mods::classes::ghost_overhead_map {
	constexpr static uint8_t width = 48;
	constexpr static uint8_t height = 16;

	using coordinate_t = int8_t;
	using room_pair_t = std::pair<room_rnum,int8_t>;
	using crawl_record_t = std::set<room_pair_t>;
	void crawl_lambda(
	    std::vector<std::vector<std::string>>& map_coordinates,
	    uint8_t direction,
	    coordinate_t in_x,
	    coordinate_t in_y,
	    coordinate_t original_x,
	    coordinate_t original_y,
	    room_rnum room,
	    crawl_record_t* crawled,
	    const uint8_t& lense
	);

	template <typename OutputDevice>
	std::string generate(OutputDevice out,const room_rnum& room_number,const uint8_t& lense) {
		std::string overhead_map = "";
		std::string horizontal_border = std::string("{grn}+{/grn}{red}") +
		    std::string(mods::classes::ghost_overhead_map::width,'=') + "{/red}{grn}+{/grn}";
		overhead_map = horizontal_border + "\r\n";
		std::vector<std::vector<std::string>> map_coordinates;
		map_coordinates.resize(mods::classes::ghost_overhead_map::height);
		for(unsigned k = 0; k < height; k++) {
			map_coordinates[k].resize(width);
			std::fill(map_coordinates[k].begin(),map_coordinates[k].end()," ");
		}
		map_coordinates[height / 2][width / 2] = "[x]";
		auto middle_x = width / 2;
		auto middle_y = height / 2;
		room_rnum room = out->room();
		crawl_record_t crawled;
		for(auto direction : {
		        NORTH,EAST,SOUTH,WEST
		    }) {
			if(world[room].dir_option[direction] == nullptr) {
				continue;
			}
			crawl_lambda(map_coordinates,direction,middle_x,middle_y,middle_x,middle_y,room,&crawled,lense);
		}

		for(unsigned k=0; k <height; k++) {
			overhead_map += "{red}={/red}";
			for(unsigned i=0; i < width; i++) {
				overhead_map += map_coordinates[k][i];
			}
			overhead_map += "{red}={/red}\r\n";
		}
		overhead_map += horizontal_border + "\r\n";
		return overhead_map;
	}
};

#endif
