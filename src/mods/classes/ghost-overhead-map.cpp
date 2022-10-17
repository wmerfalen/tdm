#include "ghost-overhead-map.hpp"
#include "../player.hpp"

namespace mods::classes::ghost_overhead_map {
	static constexpr uint8_t LENSE_AERIAL_DRONE_SCAN = 1;
	template <>
	std::string generate(mods::player* out,const room_rnum& room_number,const uint8_t& lense) {
		if(out->camera_viewing()) {
			return "";
		}
		uint8_t player_width = out->get_overhead_map_width(), player_height = out->get_overhead_map_height();
		if(width == 0 || height == 0) {
			player_width = mods::classes::ghost_overhead_map::width;
			player_height = mods::classes::ghost_overhead_map::height;
		}

		std::string horizontal_border = std::string("+") +
		    std::string(player_width,'=') + "+\r\n";
		std::vector<std::vector<std::string>> map_coordinates;
		map_coordinates.resize(player_height);
		for(auto& row : map_coordinates) {
			row.resize(player_width);
			std::fill(row.begin(),row.end(),"   ");
		}
		uint8_t middle_x = player_width / 2;
		uint8_t middle_y = player_height / 2;
		map_coordinates[middle_y][middle_x] = "[x]";
		crawl_record_t crawled;

		for(auto direction : {
		        NORTH,EAST,SOUTH,WEST
		    }) {
			if(world[out->room()].dir_option[direction] == nullptr) {
				continue;
			}
			crawl_lambda(map_coordinates,direction,
			    middle_x,middle_y,
			    player_width,player_height,
			    out->room(),&crawled,lense);
		}

		std::string overhead_map = "";
		for(auto& row : map_coordinates) {
			overhead_map += "===";
			for(auto& column : row) {
				overhead_map += column;
			}
			overhead_map += "===\r\n";
		}
		return overhead_map;
	}

	void crawl_lambda(
	    std::vector<std::vector<std::string>>& map_coordinates,
	    uint8_t direction,
	    coordinate_t in_x,
	    coordinate_t in_y,
	    coordinate_t in_width,
	    coordinate_t in_height,
	    room_rnum room,
	    crawl_record_t* crawled,
	    const uint8_t& lense
	) {

		if(crawled->find(std::make_pair<>(room,direction)) != crawled->end()) {
			return;
		}
		crawled->insert(std::make_pair<>(room,direction));
		while(in_y < in_height && in_y >= 0 &&
		    in_x < in_width && in_x >= 0 &&
		    world.size() > room
		) {
			if(map_coordinates[in_y][in_x].compare("[x]") != 0) {
				auto textures = world[room].textures();
				map_coordinates[in_y][in_x] = "[ ]";
				if(std::find(
				        textures.begin(),
				        textures.end(),
				        room_data::texture_type_t::SCANNED) != textures.end() &&
				    mods::globals::room_list[room].size() > 0) {
					map_coordinates[in_y][in_x] = "[n]";
				}
				if((lense & LENSE_AERIAL_DRONE_SCAN)) {
					auto size = mods::globals::room_list[room].size();
					if(size >= 10) {
						map_coordinates[in_y][in_x] = "[{grn}+{/grn}]";
					} else if(size) {
						map_coordinates[in_y][in_x] = CAT("[{grn}",size,"{/grn}]");
					}
				} else {
					map_coordinates[in_y][in_x] = "[ ]";
				}
			}

			switch(direction) {
				case NORTH:
					if(world[room].dir_option[EAST]) {
						crawl_lambda(map_coordinates,EAST,in_x + 1,in_y,in_width,in_height, world[room].dir_option[EAST]->to_room,crawled,lense);
					}
					if(world[room].dir_option[WEST]) {
						crawl_lambda(map_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room,crawled,lense);
					}
					break;
				case SOUTH:
					if(world[room].dir_option[EAST]) {
						crawl_lambda(map_coordinates,EAST,in_x + 1,in_y, in_width,in_height,world[room].dir_option[EAST]->to_room,crawled,lense);
					}
					if(world[room].dir_option[WEST]) {
						crawl_lambda(map_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room,crawled,lense);
					}
					break;
				case EAST:
					if(world[room].dir_option[NORTH]) {
						crawl_lambda(map_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room,crawled,lense);
					}
					if(world[room].dir_option[SOUTH]) {
						crawl_lambda(map_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room,crawled,lense);
					}
					break;
				case WEST:
					if(world[room].dir_option[NORTH]) {
						crawl_lambda(map_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room,crawled,lense);
					}
					if(world[room].dir_option[SOUTH]) {
						crawl_lambda(map_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room,crawled,lense);
					}
					break;
				default:
					return;
			}
			if(world[room].dir_option[direction] && world[room].dir_option[direction]->to_room != NOWHERE) {

				room = world[room].dir_option[direction]->to_room;
				switch(direction) {
					case NORTH:
						--in_y;
						break;
					case SOUTH:
						++in_y;
						break;
					case EAST:
						++in_x;
						break;
					case WEST:
						--in_x;
						break;
				}
			} else {
				return;
			}
		}
	}
};
