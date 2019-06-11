#include "scan.hpp"
#include <tuple>

namespace mods::scan {
	int directions[] = { NORTH,EAST,SOUTH,WEST,UP,DOWN };
	void los_scan(struct char_data* ch,int depth,vec_player_data* vec_room_list) {
		los_scan_foreach(
				ch,depth,
				[&](room_rnum room_id,int direction,vec_player_data _char_data) -> bool {
				for(auto ch : _char_data) {
				vec_room_list->push_back(ch);
				}
				return true;
				}
				);
	}
	static std::map<std::pair<room_rnum,room_rnum>,distance_t> distance_cache;
	std::tuple<bool,distance_t> cached_room_distance(const room_rnum& hunters_room,
			const room_rnum& hunted_room) {
		auto search = distance_cache.find({hunters_room,hunted_room});
		if(search != distance_cache.end()){
			return {true,search->second};
		}
		return {false,0};
	}
	void save_room_distance_to_cache(const room_rnum& hunters_room,
			const room_rnum& hunted_room,const distance_t& in_distance){
		std::pair<room_rnum,room_rnum> dataset{hunters_room,hunted_room};
		distance_cache[dataset] = in_distance;
	}
	std::tuple<bool,distance_t> distance_to(chptr player_hunter,chptr player_hunted){
		auto hunter = player_hunter->cd();
		auto hunted = player_hunted->cd();
		auto hunters_room = IN_ROOM(hunter);
		auto hunted_room = IN_ROOM(hunted);
		auto cached_distance = cached_room_distance(hunters_room,hunted_room);
		if(std::get<0>(cached_distance)){
			return cached_distance;
		}
		auto results = los_find(player_hunter,player_hunted);
		save_room_distance_to_cache(hunters_room,hunted_room,results.distance);
		return {results.found,results.distance};
	}

	find_results_t los_find(chptr hunter,chptr hunted){
		room_list_t scan_results;
		los_list_rooms(hunter,scan_results,2);
		find_results_t find_results;
		find_results.found = false;
		find_results.distance = 0;
		find_results.direction = 0;
		for(auto direction : directions){
			find_results.distance = 0;
			find_results.direction = direction;
			for(auto & room_id : scan_results[direction]){
				++find_results.distance;
				if(std::find(
							mods::globals::room_list[room_id].begin(),
							mods::globals::room_list[room_id].end(),
							hunted) != 
						mods::globals::room_list[room_id].end()){
					find_results.found = true;
					return find_results;
				}
			}
		}
		return find_results;
	}

	void los_list_rooms(chptr player_hunter,room_list_t & room_list,unsigned depth){
		auto hunter = player_hunter->cd();
		for(auto i_d : { NORTH,EAST,SOUTH,WEST,UP,DOWN }) {
			auto in_room = IN_ROOM(hunter);

			if(in_room == NOWHERE || in_room < 0) {
				return;
			}

			auto current_exit = EXIT(hunter,i_d);
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
					}else{
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
			room_list[i_d] = rooms_found;
			rooms_found.clear();
		}
	}

	void los_scan_foreach(struct char_data* ch,int depth,los_scan_foreach_callback lambda_cb) {
		/* Check if enemy is within 'depth' rooms n,e,s,w,u,d */
		std::string s_dir;

		for(auto i_d : { NORTH,EAST,SOUTH,WEST,UP,DOWN }) {
			auto in_room = IN_ROOM(ch);

			if(in_room == NOWHERE || in_room < 0) {
				std::cerr << "los_scan_foreach: in_room failure: " << in_room << "- continuing\n";
				continue;
			}

			auto current_exit = EXIT(ch,i_d);
			auto next_room = 0;

			if(current_exit) {
				next_room = current_exit->to_room;
			} else {
				continue;
			}

			s_dir += " ";
			auto room_dir = current_exit;
			uint16_t ctr = 0;

			for(auto recursive_depth = depth; recursive_depth > -1; --recursive_depth) {
				auto room_id = 0;
					if(room_dir) {
						if(EXIT_FLAGGED(room_dir, EX_CLOSED)
								||
								EXIT_FLAGGED(room_dir,EX_REINFORCED)) {
							std::cerr << "los_scan_foreach::EXIT_FLAGGED closed|reinforced\n";
							break;
						}

						room_id = room_dir->to_room;
						vec_player_data list;

						if(room_id == -1 || static_cast<std::size_t>(room_id) > mods::globals::room_list.size()) {
							std::cerr << "requested room_id: " << room_id << " but room_list.size() is " << mods::globals::room_list.size() << "\n";
							continue;
						}

						for(auto character : mods::globals::room_list[room_id]) {
							vec_player_data_element item;
							item.ch = character->cd();
							item.distance = ctr;
							list.push_back(item);
						}

						/**
						 * If the lambda returns false, that means it wants to stop looping
						 */
						if(!lambda_cb(room_id,i_d,list)){
							std::cerr << "los_scan_foreach::lambda_cb!\n";
							return;
						}
						/*
							 if(auto people = world[room_id].people){
							 for(; people->next_in_room; people = people->next_in_room){
							 lambda_cb(room_id,i_d,people);
							 }
							 }*/
					}

				room_dir = world[next_room].dir_option[i_d];

				if(!room_dir) {
					std::cerr << "los_scan_foreach::room_dir == nullptr\n";
					break;
				}

				next_room = room_dir->to_room;

				if(next_room == NOWHERE) {
					std::cerr << "los_scan_foreach::next_room == NOWHERE\n";
					break;
				}

				ctr++;
			}//end for() recursive depth
		}//end outer for() on directions
	}//end function
};//end namespace
