#include "scan.hpp"

extern struct char_data* character_list;
namespace mods{
	namespace scan {
		void los_scan(struct char_data* ch,int depth,vec_player_data* vec_room_list){
			los_scan_foreach(
				ch,depth,
				[&](room_rnum room_id,int direction,vec_player_data _char_data){
					for(auto ch : _char_data){
						vec_room_list->push_back(ch);
					}
				}
			);
		}

		void los_scan_foreach(struct char_data* ch,int depth,los_scan_foreach_callback lambda_cb){
			/* Check if enemy is within 'depth' rooms n,e,s,w,u,d */
			std::string s_dir;
			
			for(auto i_d : {NORTH,EAST,SOUTH,WEST,UP,DOWN}){
				auto in_room = IN_ROOM(ch);
				if(in_room == NOWHERE || in_room < 0){ continue; }
				auto current_exit = EXIT(ch,i_d);
				auto next_room = 0;
				if(current_exit){
					next_room = current_exit->to_room;
				}else{
					continue;
				}
				s_dir += " ";
				auto room_dir = current_exit;
				uint16_t ctr = 0;
				for(auto recursive_depth = depth;recursive_depth > -1;--recursive_depth){
					auto room_id = 0;
					if(world[next_room].people){
						if(room_dir){
							if (EXIT_FLAGGED(room_dir, EX_CLOSED)
							|| 
							EXIT_FLAGGED(room_dir,EX_REINFORCED)){
								break;
							}
							room_id = room_dir->to_room;
							vec_player_data list;
							if(room_id == -1 || room_id > mods::globals::room_list.size()){
								std::cerr << "requested room_id: " << room_id << " but room_list.size() is " << mods::globals::room_list.size() << "\n";
								continue;
							}
							for(auto character : mods::globals::room_list[room_id]){
								vec_player_data_element item;
								item.ch = character;
								item.distance = ctr;
								list.push_back(item);
							}
							lambda_cb(room_id,i_d,list);
							/*
							if(auto people = world[room_id].people){
								for(; people->next_in_room; people = people->next_in_room){
									lambda_cb(room_id,i_d,people);
								}
							}*/
						}
					}
					room_dir = world[next_room].dir_option[i_d];
					if(!room_dir){ break; }
					next_room = room_dir->to_room;
					if(next_room == NOWHERE){ break; }
					ctr++;
				}
			}
		}

	};
	};
