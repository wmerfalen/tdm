#include "sensor-grenade.hpp"
#include <unordered_map>
#include <string>
#include "events.hpp"
#include "util-map.hpp"
#include "scan.hpp"
#include "player.hpp"

extern void obj_from_room(obj_ptr_t);
namespace mods::projectile {
	extern std::string fromdirstr(int direction,bool prefix, bool suffix);
};
namespace mods::sensor_grenade {
	using uuid_distance_list_t = std::vector<std::tuple<uuid_t,std::size_t>>;
	using player_gathering_t = std::array<uuid_distance_list_t,NUM_OF_DIRS>;
	using direction_count_t = std::array<int,NUM_OF_DIRS>;
	using count_gathering_t = std::unordered_map<uuid_t,direction_count_t>;
	static count_gathering_t count_gathering;
	static constexpr uint64_t SCANNED_AFFECT_DURATION = 400; /** in ticks */

	bool player_can_do_range_modifier(player_ptr_t& player) {
		return player->level() >= 25; /** FIXME: maybe */
	}
	bool can_see_player(uuid_t nade,player_ptr_t& target) {
		return true; /** TODO: return false if player has stealth */
	}
	void crawl_blast_radius(
	    std::vector<int>& map_coordinates,
	    int direction,int in_x,int in_y,int in_width,int in_height,int room) {
		while(in_y < in_height && in_y >= 0 &&
		        in_x < in_width && in_x >= 0 &&
		        world.size() > room
		     ) {
			map_coordinates.emplace_back(room);
			switch(direction) {
				case NORTH:
					if(world[room].dir_option[EAST]) {
						crawl_blast_radius(map_coordinates,EAST,in_x + 1,in_y,in_width,in_height, world[room].dir_option[EAST]->to_room);
					}
					if(world[room].dir_option[WEST]) {
						crawl_blast_radius(map_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room);
					}
					break;
				case SOUTH:
					if(world[room].dir_option[EAST]) {
						crawl_blast_radius(map_coordinates,EAST,in_x + 1,in_y, in_width,in_height,world[room].dir_option[EAST]->to_room);
					}
					if(world[room].dir_option[WEST]) {
						crawl_blast_radius(map_coordinates,WEST,in_x - 1,in_y, in_width,in_height,world[room].dir_option[WEST]->to_room);
					}
					break;
				case EAST:
					if(world[room].dir_option[NORTH]) {
						crawl_blast_radius(map_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room);
					}
					if(world[room].dir_option[SOUTH]) {
						crawl_blast_radius(map_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room);
					}
					break;
				case WEST:
					if(world[room].dir_option[NORTH]) {
						crawl_blast_radius(map_coordinates,NORTH,in_x,in_y - 1, in_width,in_height,world[room].dir_option[NORTH]->to_room);
					}
					if(world[room].dir_option[SOUTH]) {
						crawl_blast_radius(map_coordinates,SOUTH,in_x,in_y + 1, in_width,in_height,world[room].dir_option[SOUTH]->to_room);
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
	std::vector<int> scan_from_room(int blast_radius,const room_rnum& room_number) {
		int player_width = blast_radius;
		int player_height = blast_radius;
		int middle_x = player_width / 2;
		int middle_y = player_height / 2;
		std::vector<int> map_coordinates;
		for(auto direction : {
		            NORTH,EAST,SOUTH,WEST
		        }) {
			if(world[room_number].dir_option[direction] == nullptr) {
				continue;
			}
			crawl_blast_radius(map_coordinates,
			                   direction,
			                   middle_x,middle_y,
			                   player_width,player_height,
			                   room_number);
		}
		return map_coordinates;
	}
	void gather_room(uuid_t nade_uuid,room_rnum& room,uint8_t from_direction) {
		auto opt_nade = optr_opt(nade_uuid);
		if(opt_nade.has_value() == false) {
			log("WARNING: gather_room was passed an invalid nade uuid. couldn't find it!");
			return;
		}
		d("found nade by uuid");
		auto nade = opt_nade.value();
		int blast_radius = nade->explosive()->attributes->blast_radius * 2;
		d("proposed blast radius: " << blast_radius);
		for(auto& room_number : scan_from_room(blast_radius,room)) {
			world[room_number].add_texture(room_data::texture_type_t::SCANNED);
		}
	}
	void consume(uuid_t nade_uuid) {
		mods::util::maps::remove_if_exists<count_gathering_t,uuid_t>(&count_gathering,nade_uuid);
	}
	void send_results(uuid_t nade_uuid,uuid_t player_uuid) {
		auto player = ptr_by_uuid(player_uuid);
		if(player->is_nobody()) {
			log("mods::sensor_grenade::send_results() passed nobody. not sending results.");
			return;
		}
		for(auto dir : {
		            NORTH,EAST,SOUTH,WEST,UP,DOWN
		        }) {
			auto size = count_gathering[nade_uuid][dir];
			player->psendln(mods::projectile::fromdirstr(dir,0,0) + "->" + std::to_string(size));
		}
	}
	void send_results_multi(uuid_t nade_uuid,const std::vector<uuid_t>& players) {
		for(auto& player_uuid : players) {
			send_results(nade_uuid,player_uuid);
		}
	}
	void handle_explosion(uuid_t nade_uuid, uuid_t player_uuid, room_rnum room_id, int from_direction) {
		auto optional_object = optr_opt(nade_uuid);
		if(!optional_object.has_value()) {
			log("mods::sensor_grenade::handle_explosion passed a bad nade uuid");
			return;
		}
		auto player = ptr_by_uuid(player_uuid);
		auto& object = optional_object.value();
		if(player_can_do_range_modifier(player) && object->explosive()->attributes->range_modifier > 0) {
			int blast_radius = object->explosive()->attributes->blast_radius * 2;
			std::size_t scan_count = 0;
			for(auto& room_number : scan_from_room(blast_radius,room_id)) {
				for(auto& player_in_room : mods::globals::get_room_list(room_number)) {
					if(player_in_room->is_npc()) {
						mods::affects::affect_player_for({mods::affects::affect_t::SCANNED},player_in_room,SCANNED_AFFECT_DURATION);
						++scan_count;
					}
				}
			}
			//"\r\n[%d] target%s scanned.\r\n",scan_count, scan_count != 1 ? "s" : "");
			player->sendln(
			    CAT(
			        "\r\n[",scan_count,"] ",
			        " target",scan_count != 1 ? "s" : "",
			        " scanned."
			    )
			);
		} else {
			gather_room(nade_uuid,room_id,from_direction);
			send_results(nade_uuid,player_uuid);
			consume(nade_uuid);
		}
		obj_from_room(object);
	}
};
