#include "room-watching.hpp"
#include "../util.hpp"
#include "../scan.hpp"
#include "../mobs/mini-gunner.hpp"
#include <map>
#include <set>
namespace mods::mobs::room_watching {
	std::vector<room_rnum> watch_list;
	std::map<room_rnum,std::vector<uuid_t>> watch_map;
	void watch_direction(uuid_t mob,room_rnum room, direction_t direction,depth_t depth){
		mods::scan::room_list_t room_list;
		mods::scan::los_list_by_room(room,room_list,depth);
		for(auto & room_id : room_list[direction]){
			watch_map[room_id].emplace_back(mob);
			watch_list.emplace_back(room_id);
		}
		auto p = ptr_by_uuid(mob);
		p->set_watching(direction);
	}
	void stop_watching(uuid_t mob){
		mods::scan::room_list_t room_list;
		for(auto & pair : watch_map){
			mods::util::vector_erase(pair.second,mob);
		}
	}
	namespace events {
		void room_entry(room_rnum room,uuid_t player){
			if(std::find(watch_list.begin(),watch_list.end(),room) == watch_list.end()){
				return;
			}
			auto player_ptr = ptr_by_uuid(player);
			if(!player_ptr){
				return;
			}
			for(auto & mob_uuid : watch_map[room]){
				if(has_mini_gunner_ptr(mob_uuid)){
					mini_gunner_ptr(mob_uuid)->enemy_spotted(room,player);
				}
			}
		}
		void room_exit(room_rnum room,uuid_t player){
			std::cerr << "[room_exit] stub\n";
		}
	};
};
