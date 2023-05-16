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
	void suppress_for(player_ptr_t& victim, uint32_t ticks);
	void process_players();
	constexpr uint8_t tick_resolution() {
		return 10;
	}
};
#endif
