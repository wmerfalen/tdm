#ifndef __MENTOC_MODS_MOBS_HELPERS_HEADER__
#define __MENTOC_MODS_MOBS_HELPERS_HEADER__

#include <iostream>
#include "room-watching.hpp"
#include "../doors.hpp"
#include "../rand.hpp"

//#define __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_HELPERS_SHOW_DEBUG_OUTPUT__
#define mmh_debug(a) mentoc_prefix_debug("mods::mobs::helpers") << a << "\n";
#else
#define mmh_debug(a) ;;
#endif
extern int perform_move(char_data *ch, int dir, int need_specials_check);
namespace mods::mobs::helpers {
	template <typename Tuuid>
	static inline void clear_watching(Tuuid u){
		mods::mobs::room_watching::stop_watching(u);
	}
	/**
	 * @brief mob helper function to ease movement operations
	 *
	 * @tparam TMob
	 * @param direction
	 * @param mob
	 */
	template <typename TMob>
	void go(uint8_t direction,TMob& mob){
		mmh_debug("mob:'" << mob.uuid() << "', going->" << dirstr(direction));
		mob.cd()->mob_specials.previous_room = IN_ROOM(mob.cd());
		perform_move(mob.cd(),direction,1);
	}
	/**
	 * @brief watch a direction for depth
	 *
	 * @param direction
	 * @param mob
	 * @param depth
	 */
	template <typename TMob>
	static inline void watch(uint8_t direction,TMob* ptr,uint8_t depth){
		mmh_debug("[mini_gunner] watching:" << dirstr(direction) << "uuid:" << ptr->uuid << " room:" << IN_ROOM(ptr));
		mods::mobs::room_watching::stop_watching(ptr->uuid);
		mods::mobs::room_watching::watch_direction(ptr->uuid,IN_ROOM(ptr),direction,depth);
	}

	/**
	 * @brief watch multiple directions
	 *
	 * @tparam TMob
	 * @param directions
	 * @param ptr
	 * @param depth
	 */
	template <typename TMob>
	static inline void watch_multiple(vec_t<uint8_t> directions,TMob* ptr,uint8_t depth){
		mods::mobs::room_watching::stop_watching(ptr->uuid);
		for(auto direction : directions){
			mmh_debug("[mini_gunner] watching:" << dirstr(direction) << "uuid:" << ptr->uuid << " room:" << IN_ROOM(ptr));
			mods::mobs::room_watching::watch_direction(ptr->uuid,IN_ROOM(ptr),direction,depth);
		}
	}

	/**
	 * @brief go direction then immediately watch in the direction
	 *
	 * @tparam TMob
	 * @param direction
	 * @param mob
	 */
	template <typename TMob>
	void go_and_watch(uint8_t direction,TMob& mob){
		go(direction,mob);
		watch(direction,mob.cd(),MINI_GUNNER_SCAN_DEPTH());
	}
	/**
	 * @brief go random direction, taking peaceful and open doors into consideration
	 *
	 * @tparam TMob
	 * @param mob
	 *
	 * @return 
	 */
	template <typename TMob>
	uint8_t go_random_direction(TMob& mob){
		auto room = IN_ROOM(mob.cd());
		vec_t<uint8_t> possible;
		for(auto & dir : world[room].directions()){
			auto to_room = world[room].dir_option[dir]->to_room;
			if(mods::doors::is_open(room,dir) && !mods::rooms::is_peaceful(to_room)){
				possible.emplace_back(dir);
			}
		}
		assert(possible.size() > 0);
		auto random_index = mods::rand::roll(6,6) % possible.size();
		assert(random_index < possible.size());
		go(possible[random_index],mob);
		return possible[random_index];
	}
};//end namespace
#endif
