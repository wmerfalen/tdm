#include "scan.hpp"
#include <tuple>
#include "player.hpp"
#include "loops.hpp"
#ifdef __MENTOC_SHOW_LOS_SCAN_DEBUG__
#define LOS_SCAN_DIRECTION_DEBUG(a) { std::cerr << "[los_scan_debug]: " << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
#else
#define LOS_SCAN_DIRECTION_DEBUG(a) ;;
#endif

namespace mods::scan {
	int directions[] = { NORTH,EAST,SOUTH,WEST,UP,DOWN };
	constexpr static std::size_t MAX_DEPTH = 6;

	int los_find_player(player_ptr_t& player,player_ptr_t& victim,int depth) {
		vec_player_data vec_room_list;
		auto directions = world[player->room()].directions();
		for(auto i_d : directions) {
			vec_room_list.clear();
			los_scan_direction(player->cd(),depth,&vec_room_list,i_d,find_type_t::ANY);
			for(auto info : vec_room_list) {
				if(info.ch->uuid == victim->uuid()) {
					return i_d;
				}
			}
		}
		return -1;
	}
	void los_list_by_room(room_rnum& in_room,room_list_t& room_list,unsigned depth) {
		if(in_room == NOWHERE || in_room < 0) {
			return;
		}
		for(auto i_d : mods::scan::directions) {

			auto current_exit = world[in_room].dir_option[i_d];
			auto next_room = 0;

			if(current_exit) {
				next_room = current_exit->to_room;
			} else {
				continue;
			}

			auto room_dir = current_exit;
			std::vector<room_rnum> rooms_found;
			for(unsigned recursive_depth = depth; recursive_depth >= 0; --recursive_depth) {
				if(room_dir) {
					if(EXIT_FLAGGED(room_dir, EX_CLOSED)
					        ||
					        EXIT_FLAGGED(room_dir,EX_REINFORCED)) {
						break;
					} else {
						rooms_found.push_back(room_dir->to_room);
					}
				}
				room_dir = world[next_room].dir_option[i_d];

				if(!room_dir) {
					break;
				}

				next_room = room_dir->to_room;

				if(next_room == NOWHERE) {
					break;
				}
			}
			//mods::globals::get_room_list(i_d) = rooms_found;
			room_list[i_d] = rooms_found;
			rooms_found.clear();
		}
	}
	void los_scan_direction(char_data* ch,int depth,vec_player_data* vec_room_list,int direction) {
		los_scan_direction(ch,depth,vec_room_list,direction, find_type_t::ANY);
	}

	void los_scan_direction(char_data* ch,int in_depth,vec_player_data* vec_room_list,int direction, find_type_t type) {
		std::string s_dir;
		auto i_d = direction;
		LOS_SCAN_DIRECTION_DEBUG("scan directions: current dir: " << i_d);
		auto in_room = IN_ROOM(ch);

		if(in_room == NOWHERE || in_room < 0) {
			LOS_SCAN_DIRECTION_DEBUG("in_room is nowhere");
			return;
		}
		if(in_depth <= 0) {
			std::cerr << "los_scan_direction: received invalid depth value\n";
			return;
		}

		int depth = std::min(55,in_depth);
		auto current_exit = EXIT(ch,i_d);
		auto next_room = 0;

		if(current_exit) {
			LOS_SCAN_DIRECTION_DEBUG("current exit");
			next_room = current_exit->to_room;
		} else {
			LOS_SCAN_DIRECTION_DEBUG("continueing");
			return;
		}

		s_dir += " ";
		auto room_dir = current_exit;
		uint16_t ctr = 0;

		LOS_SCAN_DIRECTION_DEBUG("befor for");
		for(auto recursive_depth = depth; recursive_depth > -1; --recursive_depth) {
			LOS_SCAN_DIRECTION_DEBUG("within for");
			auto room_id = 0;
			if(room_dir) {
				if(EXIT_FLAGGED(room_dir, EX_CLOSED)
				        ||
				        EXIT_FLAGGED(room_dir,EX_REINFORCED)) {
					LOS_SCAN_DIRECTION_DEBUG("exit flagged reinforced/closed... breaking");
					break;
				}

				room_id = room_dir->to_room;

				if(type & find_type_t::OBJECTS) {
					mods::loops::foreach_object_in_room(room_id,[&](obj_ptr_t& object) -> bool {
						vec_room_list->push_back({});
						auto& pushed_item = vec_room_list->back();
						pushed_item.ch = nullptr;
						pushed_item.obj =object.get();
						pushed_item.uuid = object->uuid;
						pushed_item.distance = ++ctr;
						pushed_item.direction = i_d;
						pushed_item.room_rnum = room_id;
						LOS_SCAN_DIRECTION_DEBUG("moved item to list");
						return true;
					});

				}
				bool anyone = !(type & find_type_t::PLAYERS) && !(type & find_type_t::NPC);
				for(auto character : mods::globals::get_room_list(room_id)) {
					if(
					    (type & find_type_t::DEAD && character->position() == POS_DEAD)
					    ||
					    (type & find_type_t::ALIVE && character->position() > POS_DEAD)
					    ||
					    ((type & find_type_t::PLAYERS) && !IS_NPC(character->cd()))
					    ||
					    (type & find_type_t::NPC && IS_NPC(character->cd()))
					) {
						if(
						    ((type & find_type_t::PLAYERS) && !IS_NPC(character->cd())) ||
						    (type & find_type_t::NPC && IS_NPC(character->cd())) ||
						    anyone
						) {
							vec_room_list->push_back({});
							auto& pushed_item = vec_room_list->back();
							pushed_item.obj = nullptr;
							pushed_item.ch = character->cd();
							pushed_item.uuid = character->uuid();
							pushed_item.distance = ++ctr;
							pushed_item.direction = i_d;
							pushed_item.room_rnum = room_id;
							LOS_SCAN_DIRECTION_DEBUG("moved item to list");
						}
					}
				}
			}

			room_dir = world[next_room].dir_option[i_d];

			if(!room_dir) {
				LOS_SCAN_DIRECTION_DEBUG("breaking beacuse null room_dir");
				break;
			}

			next_room = room_dir->to_room;

			if(next_room == NOWHERE) {
				LOS_SCAN_DIRECTION_DEBUG("breaking because next room is nowhere");
				break;
			}
			ctr++;
		}//end for() recursive depth
	}

	void los_scan(char_data* ch,int depth,vec_player_data* vec_room_list) {
		for(auto i_d : mods::scan::directions) {
			los_scan_direction(ch,depth,vec_room_list,i_d,(find_type_t)find_type_t::ANY);
		}
	}
	void los_scan_for_npcs(char_data* ch,int depth,vec_player_data* vec_room_list) {
		for(auto i_d : mods::scan::directions) {
			los_scan_direction(ch,depth,vec_room_list,i_d,(find_type_t)(find_type_t::ALIVE | find_type_t::NPC));
		}
	}
	void los_scan_for_players(char_data* ch,int depth,vec_player_data* vec_room_list) {
		for(auto i_d : mods::scan::directions) {
			los_scan_direction(ch,depth,vec_room_list,i_d,(find_type_t)(find_type_t::PLAYERS | find_type_t::ALIVE));
		}
	}
	static std::map<std::pair<room_rnum,room_rnum>,distance_t> distance_cache;
	std::tuple<bool,distance_t> cached_room_distance(const room_rnum& hunters_room,
	                                                 const room_rnum& hunted_room) {
		auto search = distance_cache.find({hunters_room,hunted_room});
		if(search != distance_cache.end()) {
			return {true,search->second};
		}
		return {false,0};
	}
	void save_room_distance_to_cache(const room_rnum& hunters_room,
	                                 const room_rnum& hunted_room,const distance_t& in_distance) {
		std::pair<room_rnum,room_rnum> dataset{hunters_room,hunted_room};
		distance_cache[dataset] = in_distance;
	}
	std::tuple<bool,distance_t> distance_to(chptr player_hunter,chptr player_hunted) {
		auto hunter = player_hunter->cd();
		auto hunted = player_hunted->cd();
		auto hunters_room = IN_ROOM(hunter);
		auto hunted_room = IN_ROOM(hunted);
		auto cached_distance = cached_room_distance(hunters_room,hunted_room);
		if(std::get<0>(cached_distance)) {
			return cached_distance;
		}
		auto results = los_find(player_hunter,player_hunted);
		save_room_distance_to_cache(hunters_room,hunted_room,results.distance);
		return {results.found,results.distance};
	}
	std::tuple<bool,distance_t> room_distance(room_rnum& source,room_rnum& dest) {
		auto cached_distance = cached_room_distance(source,dest);
		if(std::get<0>(cached_distance)) {
			return cached_distance;
		}

		int distance = -1;
		bool found = false;
		for(auto i_d : mods::scan::directions) {
			auto current_exit = world[source].dir_option[i_d];
			auto next_room = 0;

			if(current_exit) {
				next_room = current_exit->to_room;
			} else {
				continue;
			}
			if(next_room == dest) {
				distance = 0;
				found = true;
				break;
			}

			auto room_dir = current_exit;
			//std::vector<room_rnum> rooms_found;
			distance = 0;
			for(unsigned recursive_depth = 0; mods::scan::MAX_DEPTH >= recursive_depth ; ++recursive_depth) {
				room_dir = world[next_room].dir_option[i_d];

				if(!room_dir) {
					break;
				}

				next_room = room_dir->to_room;
				if(next_room == dest) {
					distance = recursive_depth;
					found = true;
					break;
				}

				if(next_room == NOWHERE) {
					break;
				}
			}
			if(found) {
				break;
			}
		}
		if(found) {
			save_room_distance_to_cache(source,dest,distance);
			return {1,distance};
		}
		return {0,0};
	}

	find_results_t los_find(chptr hunter,chptr hunted) {
		room_list_t scan_results;
		los_list_rooms(hunter,scan_results,2);
		find_results_t find_results;
		find_results.found = false;
		find_results.distance = 0;
		find_results.direction = 0;
		for(auto direction : directions) {
			find_results.distance = 0;
			find_results.direction = direction;
			for(auto& room_id : scan_results[direction]) {
				++find_results.distance;
				if(std::find(
				            mods::globals::get_room_list(room_id).begin(),
				            mods::globals::get_room_list(room_id).end(),
				            hunted) !=
				        mods::globals::get_room_list(room_id).end()) {
					find_results.found = true;
					return find_results;
				}
			}
		}
		return find_results;
	}

	void los_list_rooms(chptr player_hunter,room_list_t& room_list,unsigned depth) {
		auto hunter = player_hunter->cd();
		auto in_room = IN_ROOM(hunter);
		los_list_by_room(in_room, room_list,depth);
	}
	std::vector<uuid_t> drone_scan_room(room_rnum room) {
		std::vector<uuid_t> scanned;
		for(const auto& player : room_list(room)) {
			scanned.emplace_back(player->uuid());
		}
		return scanned;
	}
};//end namespace
