#ifndef __MENTOC_MODS_OVERHEAD_MAP_HEADER__
#define  __MENTOC_MODS_OVERHEAD_MAP_HEADER__

#include "../structs.h"
#include <vector>

extern std::vector<room_data> world;
namespace mods::overhead_map {
	constexpr static uint8_t width = 48;
	constexpr static uint8_t height = 16;
	void crawl_lambda(std::vector<std::vector<std::string>>& map_coordinates,
			std::vector<std::vector<std::string>>& raw_coordinates,
			int direction,int in_x,int in_y,int original_x,int original_y,int room);
	
	template <typename OutputDevice>
		std::string_view generate(OutputDevice out,const room_rnum& room_number){
			std::string overhead_map;
			std::string horizontal_border = std::string("{grn}+{/grn}{red}") + 
				std::string(mods::overhead_map::width,'=') + "{/red}{grn}+{/grn}";
			overhead_map = horizontal_border + "\r\n";
			std::vector<std::vector<std::string>> map_coordinates,draw_coordinates;
			map_coordinates.resize(mods::overhead_map::height);
			draw_coordinates.resize(mods::overhead_map::height);
			for(unsigned k = 0; k < height; k++){
				map_coordinates[k].resize(width);
				std::fill(map_coordinates[k].begin(),map_coordinates[k].end()," ");
				draw_coordinates[k].resize(width);
				std::fill(draw_coordinates[k].begin(),draw_coordinates[k].end()," ");
			}
			draw_coordinates[height / 2][width / 2] = "x";
			auto middle_x = width / 2;
			auto middle_y = height / 2;
			room_rnum room = out->room();
			for(auto direction : {NORTH,EAST,SOUTH,WEST}){
				if(world[room].dir_option[direction] == nullptr){ continue; }
				crawl_lambda(map_coordinates,draw_coordinates,direction,middle_x,middle_y,middle_x,middle_y,room);
			}

			for(unsigned k=0; k <height; k++){
				overhead_map += "{red}={/red}";
				for(unsigned i=0; i < width;i++){
					overhead_map += draw_coordinates[k][i];
				}
				overhead_map += "{red}={/red}\r\n";
			}
			overhead_map += horizontal_border + "\r\n";
			return overhead_map;
		}
};

#endif
