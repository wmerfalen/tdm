#include "room-watching.hpp"
#include "../util.hpp"
#include "../scan.hpp"
#include "../mobs/mini-gunner.hpp"
#include "../mobs/car-thief.hpp"
#include "../mobs/orthos-agent.hpp"
#include "../mobs/defiler.hpp"
#include <map>
#include <set>

#ifdef __MENTOC_SHOW_WATCH_ROOMS_HEARTBEAT__
#define m_report(a) mentoc_prefix_debug("mods::mobs::room_watching::REPORT:") << a << "\n";
#else
#define m_report(a)
#endif


//#define __MENTOC_MODS_MOBS_ROOM_WATCHING_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_MOBS_ROOM_WATCHING_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("mods::mobs::room_watching") << a << "\n";
#else
#define m_debug(a)
#endif
namespace mods::mobs::orthos_callbacks {
	extern bool dispatch_watcher(const uuid_t& orthos_agent_uuid,player_ptr_t& player, const room_rnum& room_id);
};

namespace mods::mobs::room_watching {
	std::set<room_rnum> watch_list;
	std::map<room_rnum,std::vector<uuid_t>> watch_map;
	std::map<uuid_t,std::set<room_rnum>> player_to_room_map;
	void watch_direction(uuid_t mob,room_rnum room, direction_t direction,depth_t depth) {
		static int call_count = 0;
		if((++call_count % 20) == 0) {
			call_count = 0;
		}
		mods::scan::room_list_t room_list;
		mods::scan::los_list_by_room(room,room_list,depth);
		for(auto& room_id : room_list[direction]) {
			watch_map[room_id].emplace_back(mob);
			watch_list.insert(room_id);
			player_to_room_map[mob].insert(room_id);
		}
		auto p = ptr_by_uuid(mob);
		p->set_watching(direction);
		m_report("room_watching usages. watch_map: " << watch_map.size() << " watch_list: " << watch_list.size());

	}
	void watch_room(player_ptr_t& mob) {
		m_debug("watch room 1");
		m_debug(mob->name().c_str() << " watching room: (vnum)" << mob->vnum());
		world[mob->room()].watchers.insert(mob);
		player_to_room_map[mob->uuid()].insert(mob->room());
		m_report("watch_room world[" << mob->room() << "]: watchers size: " << world[mob->room()].watchers.size());
	}
	void unwatch_room(player_ptr_t& mob) {
		m_debug(mob->name().c_str() << " unwatch room (vnum)" << mob->vnum());
		world[mob->room()].watchers.erase(mob);
		player_to_room_map[mob->uuid()].erase(mob->room());
	}
	void stop_watching(uuid_t mob) {
		mods::scan::room_list_t room_list;
		for(auto& pair : watch_map) {
			mods::util::vector_erase(pair.second,mob);
			player_to_room_map[mob].erase(pair.first);
		}
	}
	void destroy_player(uuid_t player_uuid) {
		stop_watching(player_uuid);
		player_to_room_map.erase(player_uuid);
	}

	void heartbeat() {
#ifdef __MENTOC_SHOW_WATCH_ROOMS_HEARTBEAT__
		static std::set<std::string> names;
		for(const auto& pair : player_to_room_map) {
			auto p = ptr_by_uuid(pair.first);
			if(p) {
				names.insert(p->name().c_str());
			}
		}
		for(const auto& name : names) {
			m_report(name.c_str() << " is watching some rooms");
		}
#endif
	}


	namespace events {

		/**
		 *
		 * This is horribly inefficient. There has to be a better way to do this.
		 * There really should be a smart_mob pointer stored with each watcher.
		 *
		 *
		 */
		void room_entry(player_ptr_t& player) {
			m_debug("room entry 1");
			for(auto& watcher : world[player->room()].watchers) {
				if(mods::mobs::defiler_callbacks::dispatch_watcher(watcher->uuid(),player,player->room())) {
					continue;
				}
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
				if(mods::mobs::defiler_callbacks::dispatch_watcher(mob_uuid,player,player->room())) {
					continue;
				}
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
#undef m_report
