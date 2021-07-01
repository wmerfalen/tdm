#include "room-watching.hpp"
#include "../util.hpp"
#include "../scan.hpp"
#include "../mobs/mini-gunner.hpp"
#include "../mobs/car-thief.hpp"
#include <map>
#include <set>
namespace mods::mobs::room_watching {
	std::vector<room_rnum> watch_list;
	std::map<room_rnum,std::vector<uuid_t>> watch_map;
	void watch_direction(uuid_t mob,room_rnum room, direction_t direction,depth_t depth) {
		mods::scan::room_list_t room_list;
		mods::scan::los_list_by_room(room,room_list,depth);
		for(auto& room_id : room_list[direction]) {
			watch_map[room_id].emplace_back(mob);
			watch_list.emplace_back(room_id);
		}
		auto p = ptr_by_uuid(mob);
		p->set_watching(direction);
	}
	void watch_room(player_ptr_t& mob) {
		world[mob->room()].watchers.insert(mob);
	}
	void unwatch_room(player_ptr_t& mob) {
		world[mob->room()].watchers.erase(mob);
	}
	void stop_watching(uuid_t mob) {
		mods::scan::room_list_t room_list;
		for(auto& pair : watch_map) {
			mods::util::vector_erase(pair.second,mob);
		}
	}
	namespace events {
		void room_entry(player_ptr_t& player) {
			for(auto& watcher : world[player->room()].watchers) {
				auto c = car_thief_ptr(watcher->uuid());
				if(c) {
					c->door_entry_event(player);
				}
			}
			if(player->visibility() == 0) {
				return;
			}
			for(auto& mob_uuid : watch_map[player->room()]) {
				if(has_mini_gunner_ptr(mob_uuid)) {
					mini_gunner_ptr(mob_uuid)->enemy_spotted(player->room(),player->uuid());
				}
			}
		}
		void room_exit(player_ptr_t& player) {
		}
		void room_entry(const room_rnum& room,const uuid_t& player) {
			if(std::find(watch_list.begin(),watch_list.end(),room) != watch_list.end()) {
				auto player_ptr = ptr_by_uuid(player);
				if(!player_ptr) {
					return;
				}
			}
		}
		void room_exit(const room_rnum& room,const uuid_t& player) {
			std::cerr << "[room_exit] stub\n";
		}
	};
};
