#ifndef __MENTOC_MODS_MELEE_muay_thai_HEADER__
#define __MENTOC_MODS_MELEE_muay_thai_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::melee::muay_thai {
	int teep(
	    player_ptr_t& attacker,
	    player_ptr_t& victim,
	    int16_t dam
	);

	float roll_critical_damage_reduction(
	    player_ptr_t& attacker,
	    player_ptr_t& victim,
	    int16_t dam
	);
};
#endif

