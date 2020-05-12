#include "scan.hpp"
#include <tuple>

namespace mods::scan {
	int directions[] = { NORTH,EAST,SOUTH,WEST,UP,DOWN };
	constexpr static std::size_t MAX_DEPTH = 6;

	void los_list_by_room(room_rnum& in_room,room_list_t & room_list,unsigned depth){
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
			//mods::globals::get_room_list(i_d) = rooms_found;
			room_list[i_d] = rooms_found;
			rooms_found.clear();
		}
	}

	void los_scan(char_data* ch,int depth,vec_player_data* vec_room_list) {
		std::string s_dir;

#define LSD(a) { std::cerr << "[los_scan_debug]: " << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
		for(auto i_d : mods::scan::directions) {
			LSD("scan directions: current dir: " << i_d);
			auto in_room = IN_ROOM(ch);

			if(in_room == NOWHERE || in_room < 0) {
				LSD("in_room is nowhere");
				continue;
			}

			auto current_exit = EXIT(ch,i_d);
			auto next_room = 0;

			if(current_exit) {
				LSD("current exit");
				next_room = current_exit->to_room;
			} else {
				LSD("continueing");
				continue;
			}

			s_dir += " ";
			auto room_dir = current_exit;
			uint16_t ctr = 0;

			LSD("befor for");
			for(auto recursive_depth = depth; recursive_depth > -1; --recursive_depth) {
				LSD("within for");
				auto room_id = 0;
				if(room_dir) {
					if(EXIT_FLAGGED(room_dir, EX_CLOSED)
							||
							EXIT_FLAGGED(room_dir,EX_REINFORCED)) {
						LSD("exit flagged reinforced/closed... breaking");
						break;
					}

					room_id = room_dir->to_room;

					for(auto character : mods::globals::get_room_list(room_id)) {
						vec_room_list->push_back({});
						auto & pushed_item = vec_room_list->back();
						pushed_item.ch = character->cd();
						pushed_item.uuid = character->uuid();
						pushed_item.distance = ++ctr;
						pushed_item.direction = i_d;
						LSD("moved item to list");
					}
				}

				room_dir = world[next_room].dir_option[i_d];

				if(!room_dir) {
					LSD("breaking beacuse null room_dir");
					break;
				}

				next_room = room_dir->to_room;

				if(next_room == NOWHERE) {
					LSD("breaking because next room is nowhere");
					break;
				}
				ctr++;
			}//end for() recursive depth
		}//end outer for() on directions
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
	std::tuple<bool,distance_t> room_distance(room_rnum& source,room_rnum& dest){
		auto cached_distance = cached_room_distance(source,dest);
		if(std::get<0>(cached_distance)){
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
			if(next_room == dest){
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
				if(next_room == dest){
					distance = recursive_depth;
					found = true;
					break;
				}

				if(next_room == NOWHERE) {
					break;
				}
			}
			if(found){ break; }
		}
		if(found){
			save_room_distance_to_cache(source,dest,distance);
			return {1,distance};
		}
		return {0,0};
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
							mods::globals::get_room_list(room_id).begin(),
							mods::globals::get_room_list(room_id).end(),
							hunted) != 
						mods::globals::get_room_list(room_id).end()){
					find_results.found = true;
					return find_results;
				}
			}
		}
		return find_results;
	}

	void los_list_rooms(chptr player_hunter,room_list_t & room_list,unsigned depth){
		auto hunter = player_hunter->cd();
		auto in_room = IN_ROOM(hunter);
		los_list_by_room(in_room, room_list,depth);
	}
	void los_scan_foreach(char_data* ch,int depth,los_scan_foreach_callback lambda_cb) {
		/* Check if enemy is within 'depth' rooms n,e,s,w,u,d */
		std::string s_dir;

					vec_player_data list;
		for(auto i_d : mods::scan::directions) {
			LSD("scan directions: current dir: " << i_d);
			auto in_room = IN_ROOM(ch);

			if(in_room == NOWHERE || in_room < 0) {
				LSD("in_room is nowhere");
				continue;
			}

			auto current_exit = EXIT(ch,i_d);
			auto next_room = 0;

			if(current_exit) {
				LSD("current exit");
				next_room = current_exit->to_room;
			} else {
				LSD("continueing");
				continue;
			}

			s_dir += " ";
			auto room_dir = current_exit;
			uint16_t ctr = 0;

			LSD("befor for");
			for(auto recursive_depth = depth; recursive_depth > -1; --recursive_depth) {
				LSD("within for");
				auto room_id = 0;
				if(room_dir) {
					if(EXIT_FLAGGED(room_dir, EX_CLOSED)
							||
							EXIT_FLAGGED(room_dir,EX_REINFORCED)) {
						LSD("exit flagged reinforced/closed... breaking");
						break;
					}

					room_id = room_dir->to_room;

					if(room_id == -1 || static_cast<std::size_t>(room_id) > mods::globals::room_list.size()) {
						LSD("room id == -1 or greater than room list .size()");
						continue;
					}

					vec_player_data_element item{};
					for(auto character : mods::globals::get_room_list(room_id)) {
						item.uuid = character->uuid();
						item.distance = ctr;
						item.direction = i_d;
						list.push_back(item);
						LSD("moved item to list");
					}

					/**
					 * If the lambda returns false, that means it wants to stop looping
					 */
					if(!lambda_cb(room_id,i_d,list)){
						LSD("didn't return truthy from lambda_cb");
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
					LSD("breaking beacuse null room_dir");
					break;
				}

				next_room = room_dir->to_room;

				if(next_room == NOWHERE) {
					LSD("breaking because next room is nowhere");
					break;
				}

				ctr++;
			}//end for() recursive depth
		}//end outer for() on directions
	}//end function
};//end namespace
