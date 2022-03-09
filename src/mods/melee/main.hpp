#ifndef __MENTOC_MODS_MELEE_main_HEADER__
#define __MENTOC_MODS_MELEE_main_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "combat-order.hpp"

namespace mods::melee::main {
	void dispatch_player(player_ptr_t& attacker,player_ptr_t& victim);
};
#endif

