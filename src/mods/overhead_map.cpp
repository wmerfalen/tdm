#include "overhead_map.hpp"
#include "player.hpp"

//extern std::vector<room_data> world;
namespace mods::overhead_map {
	//template <>
	//std::string_view generate(mods::player out,const room_rnum& room_number){
	//	return world[room_number].overhead(out.get_lense());
	//}
	//template <>
	//std::string_view generate(mods::player* out,const room_rnum& room_number){
	//	return mods::overhead_map::generate<mods::player>(*out,room_number);
	//}
	template <>
		std::string generate(mods::player* out,const room_rnum& room_number){
			uint8_t player_width = out->get_overhead_map_width(), player_height = out->get_overhead_map_height();
			if(width == 0 || height == 0){
				player_width = mods::overhead_map::width;
				player_height = mods::overhead_map::height;
			}
			printf("width: %d, height: %d\n",player_width,player_height);
			
			std::string horizontal_border = std::string("+") + 
				std::string(player_width,'=') + "+\r\n";
			std::vector<std::vector<std::string>> map_coordinates;
			map_coordinates.resize(player_height);
			for(auto & row : map_coordinates){
				row.resize(player_width);
				std::fill(row.begin(),row.end(),"   ");
			}
			uint8_t middle_x = player_width / 2;
			uint8_t middle_y = player_height / 2;
			map_coordinates[middle_y][middle_x] = "[x]";
			for(auto direction : {NORTH,EAST,SOUTH,WEST}){
				if(world[out->room()].dir_option[direction] == nullptr){ continue; }
				crawl_lambda(map_coordinates,map_coordinates,direction,
						middle_x,middle_y,
						player_width,player_height,
						out->room());
			}

			std::string overhead_map = "";
			for(auto & row : map_coordinates){
				overhead_map += "===";
				for(auto & column : row){
					overhead_map += column;
				}
				overhead_map += "===\r\n";
			}
			return overhead_map;
		}
	void crawl_lambda(std::vector<std::vector<std::string>>& map_coordinates,
		std::vector<std::vector<std::string>>& draw_coordinates,
		int direction,int in_x,int in_y,int in_width,int in_height,int room){

		while(in_y < in_height && in_y >= 0 &&
				in_x < in_width && in_x >= 0 &&
				world.size() > room
		){
#if 0
			/** 
			 * This code is so close to being done. Just needs some
			 * more work and it'll look good. for now, this commit
			 * doesn't get to have this code in it. 
			 */
			/*
				 char* f = "┌┉┉┉┐";
				 char* g = "│   │";
				 char* m = "└┉┉┉┘";
				 */

			int draw_y = (in_y - original_y) * 2;
			int draw_x = (in_x - original_x) * 2;

			draw_y += in_y;
			draw_x += in_x;

			/*
			 * top left (north west)
			 */
			if(draw_y -1 >= 0 && 
					draw_y -1 < mods::overhead_map::height &&
					draw_x - 1 >= 0 &&
					draw_x -1 < mods::overhead_map::width
				){
				//draw_coordinates[draw_y - 1][draw_x - 1] = "┼┉┼"; "┉";
				if(draw_x - 3 >= 0 && draw_x -3 < mods::overhead_map::width &&
						draw_y -1 >= 0 && draw_y -1 < mods::overhead_map::height ){
					if(draw_coordinates[draw_y - 1][draw_x -3].compare("──") == 0){
						if(world[room].dir_option[WEST] != nullptr){
							draw_coordinates[draw_y - 1][draw_x - 1] = "──"; 
						}
					}
				}
				if(world[room].dir_option[NORTH] == nullptr &&
						world[room].dir_option[WEST] == nullptr){
					draw_coordinates[draw_y - 1][draw_x - 1] = "┌─"; 
				}else if(world[room].dir_option[NORTH] != nullptr &&
						world[room].dir_option[WEST] == nullptr){
					draw_coordinates[draw_y - 1][draw_x - 1] = "│░"; 
				}else if(world[room].dir_option[NORTH] == nullptr &&
						world[room].dir_option[WEST] != nullptr){
					draw_coordinates[draw_y - 1][draw_x - 1] = "──"; 
				}
			}
			/*
			 * top middle (north)
			 */
			if(draw_y -1 >= 0 && 
					draw_y -1 < mods::overhead_map::height &&
					draw_x  < mods::overhead_map::width &&
					draw_x >= 0){
				if(world[room].dir_option[NORTH] == nullptr){
					draw_coordinates[draw_y -1][draw_x] = "{blu}─{/blu}";
				}else{
					draw_coordinates[draw_y -1][draw_x] = "░";
				}
			}
			/*
			 * top right (north east)
			 */
			if(draw_y -1 >= 0 && 
					draw_y -1 < mods::overhead_map::height &&
					draw_x + 1 < mods::overhead_map::width && 
					draw_x + 1 >= 0){
				if(world[room].dir_option[NORTH] == nullptr &&
						world[room].dir_option[EAST] == nullptr){
					draw_coordinates[draw_y -1][draw_x +1] = "─┐";
				}else if(world[room].dir_option[NORTH] != nullptr &&
						world[room].dir_option[EAST] == nullptr){
					draw_coordinates[draw_y -1][draw_x +1] = "░│";
				}else if(world[room].dir_option[NORTH] == nullptr &&
						world[room].dir_option[EAST] != nullptr){
					room_rnum go_east = world[room].dir_option[EAST]->to_room;
					if(world[go_east].dir_option[NORTH] != nullptr){
						draw_coordinates[draw_y -1][draw_x +1] = "{grn}┘░{/grn}";
					//	draw_coordinates[draw_y -1][draw_x +1] = "{grn}┕─{/grn}";
					}else{
							draw_coordinates[draw_y -1][draw_x +1] = "{grn}──{/grn}";
					}
				}
			}
			/**
			 * Bottom left (south west)
			 */
			if(draw_y + 1 < mods::overhead_map::height && 
					draw_y + 1 >= 0 &&
					draw_x -1 >= 0 &&
					draw_x - 1 < mods::overhead_map::width){
				if(world[room].dir_option[WEST] == nullptr &&
						world[room].dir_option[SOUTH] == nullptr){
					draw_coordinates[draw_y +1][draw_x -1] = "└─";
				}else if(world[room].dir_option[WEST] == nullptr &&
						world[room].dir_option[SOUTH] != nullptr){
					draw_coordinates[draw_y +1][draw_x -1] = "│░";
				}else if(world[room].dir_option[WEST] != nullptr &&
						world[room].dir_option[SOUTH] == nullptr){
					draw_coordinates[draw_y +1][draw_x -1] = "──";
				}
			}
			/**
			 * Bottom middle (south)
			 */
			if(draw_y + 1 < mods::overhead_map::height && 
					draw_y + 1 >= 0 &&
					draw_x < mods::overhead_map::width &&
					draw_x >= 0){
				if(world[room].dir_option[SOUTH] == nullptr){
					draw_coordinates[draw_y + 1][draw_x] ="{red}─{/red}"; //"┉";
				}else{
					draw_coordinates[draw_y + 1][draw_x] ="{red}░{/red}"; //"┉";
				}

			}
			/*
			 * right middle (east)
			 */
			if(draw_x + 1 < mods::overhead_map::width &&
					draw_y < mods::overhead_map::height &&
					draw_y >= 0){
				if(world[room].dir_option[EAST] == nullptr){
					draw_coordinates[draw_y][draw_x + 1] = "░│";
				}else{
					draw_coordinates[draw_y][draw_x + 1] = "░░";
				}
			}

			/*
			 * left middle (west)
			 */
			if(draw_x - 1 >= 0 &&
					draw_x -1 < mods::overhead_map::width &&
					draw_y >= 0 &&
					draw_y < mods::overhead_map::height){
				if(world[room].dir_option[WEST] == nullptr){
					draw_coordinates[draw_y][draw_x - 1] = "│░";
				}else{
					draw_coordinates[draw_y][draw_x - 1] = "░░";
				}
			}
			/*
			 * 
			 * bottom right (south east)
			 */
			if(draw_y + 1 >= 0 &&
					draw_y + 1 < mods::overhead_map::height && 
					draw_x +1 >= 0 &&
					draw_x + 1 < mods::overhead_map::width){
				if(world[room].dir_option[SOUTH] == nullptr &&
						world[room].dir_option[EAST] == nullptr){
					draw_coordinates[draw_y + 1][draw_x + 1] = "─┘";
				}else if(world[room].dir_option[SOUTH] != nullptr &&
						world[room].dir_option[EAST] == nullptr){
					draw_coordinates[draw_y + 1][draw_x + 1] = "░│";
				}else if(world[room].dir_option[SOUTH] == nullptr &&
						world[room].dir_option[EAST] != nullptr){
					//draw_coordinates[draw_y + 1][draw_x + 1] = "──";
				}
			}
#else
			if(map_coordinates[in_y][in_x].compare("[x]") != 0){
				map_coordinates[in_y][in_x] = "[ ]";
			}
#endif


			switch(direction){
				case NORTH: 
					if(world[room].dir_option[EAST]){
						crawl_lambda(map_coordinates,draw_coordinates,EAST,in_x + 1,in_y,in_width,in_height, world[room].dir_option[EAST]->to_room);
					}
					if(world[room].dir_option[WEST]){
						crawl_lambda(map_coordinates,draw_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room);
					}
					break;
				case SOUTH: 
					if(world[room].dir_option[EAST]){
						crawl_lambda(map_coordinates,draw_coordinates,EAST,in_x + 1,in_y, in_width,in_height,world[room].dir_option[EAST]->to_room);
					}
					if(world[room].dir_option[WEST]){
						crawl_lambda(map_coordinates,draw_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room);
					}
					break;
				case EAST:
					if(world[room].dir_option[NORTH]){
						crawl_lambda(map_coordinates,draw_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room);
					}
					if(world[room].dir_option[SOUTH]){
						crawl_lambda(map_coordinates,draw_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room);
					}
					break;
				case WEST:
					if(world[room].dir_option[NORTH]){
						crawl_lambda(map_coordinates,draw_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room);
					}
					if(world[room].dir_option[SOUTH]){
						crawl_lambda(map_coordinates,draw_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room);
					}
					break;
				default: return;
			}
			if(world[room].dir_option[direction] && world[room].dir_option[direction]->to_room != NOWHERE){
				room = world[room].dir_option[direction]->to_room;
				switch(direction){
					case NORTH: --in_y; break;
					case SOUTH: ++in_y; break;
					case EAST: ++in_x; break;
					case WEST: --in_x; break;
				}
			}else{
				return;
			}
		}
	}
	/*
	template <>
		std::string generate(player_ptr_t out,const room_rnum& room_number){
			return generate(out.get(),room_number);
		}
		*/
};
