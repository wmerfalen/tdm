#ifndef __MENTOC_MODS_suppress_HEADER__
#define __MENTOC_MODS_suppress_HEADER__
#include "../globals.hpp"
/**
 * The effects of being suppressed should be:
 * - A movement penalty.
 *   	- it becomes harder to move
 *   	- it might become impossible to move for some NPC's
 * - An accuracy penalty.
 *   	- it should be harder for an NPC to retaliate with ranged attacks
 * - A time to retaliate penalty
 *   	- should a suppressed target try to attack, they may be delayed
 *   		- by a couple of ticks
 *
 *
 */

namespace mods::suppress {
	void suppress_for(const uuid_t& victim_uuid, const uint64_t& ticks);
	void process_players();
	static inline constexpr uint16_t tick_resolution() {
		return 10;
	}
	bool is_suppressed(const uuid_t& victim_uuid);
};
#endif
