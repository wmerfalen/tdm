#include "room-watching.hpp"
#include "../util.hpp"
#include "../scan.hpp"
#include "../mobs/mini-gunner.hpp"
#include "../mobs/car-thief.hpp"
#include "../mobs/orthos-agent.hpp"
#include <map>
#include <set>
//#define __DEBUG__
#ifdef __DEBUG__
#define m_debug(a) log(a)
#else
#define m_debug(a)
#endif
namespace mods::mobs::orthos_callbacks {
	extern bool dispatch_watcher(const uuid_t& orthos_agent_uuid,player_ptr_t& player, const room_rnum& room_id);
};

namespace mods::mobs::room_watching {
	std::vector<room_rnum> watch_list;
	std::map<room_rnum,std::vector<uuid_t>> watch_map;
	void watch_direction(uuid_t mob,room_rnum room, direction_t direction,depth_t depth) {
		static int call_count = 0;
		if((++call_count % 20) == 0) {
			call_count = 0;
			log("room_watching usages. watch_map: %d watch_list: %d",watch_map.size(),watch_list.size());
		}
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
		m_debug("watch room 1");
		world[mob->room()].watchers.insert(mob);
	}
	void unwatch_room(player_ptr_t& mob) {
		m_debug("unwatch room 1");
		world[mob->room()].watchers.erase(mob);
	}
	void stop_watching(uuid_t mob) {
		m_debug(" stop watch room 1");
		mods::scan::room_list_t room_list;
		for(auto& pair : watch_map) {
			mods::util::vector_erase(pair.second,mob);
		}
	}
	namespace events {
		void room_entry(player_ptr_t& player) {
			m_debug("room entry 1");
			for(auto& watcher : world[player->room()].watchers) {
				if(mods::mobs::orthos_callbacks::dispatch_watcher(watcher->uuid(),player,player->room())) {
					m_debug("found orthos agent in room entry");
					continue;
				}
				auto c = car_thief_ptr(watcher->uuid());
				if(c) {
					c->door_entry_event(player);
				}
			}
			//if(player->visibility() == 0) {
			//	return;
			//}
			m_debug("map time");
			for(auto& mob_uuid : watch_map[player->room()]) {
				//if(mods::mobs::orthos_agent::is_orthos_agent(mob_uuid)) {
				if(mods::mobs::orthos_callbacks::dispatch_watcher(mob_uuid,player,player->room())) {
					m_debug("found orthos agent in room entry");
					continue;
				}
				//}
				if(has_mini_gunner_ptr(mob_uuid)) {
					mini_gunner_ptr(mob_uuid)->enemy_spotted(player->room(),player->uuid());
				}
			}
		}
		void room_exit(player_ptr_t& player) {
			m_debug("room exit 1");
		}
		void room_entry(const room_rnum& room,const uuid_t& player) {
			m_debug("room entry 2");
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
