#ifndef __MENTOC_MODS_combat_composer_spray_target_HEADER__
#define __MENTOC_MODS_combat_composer_spray_target_HEADER__
#include "../../globals.hpp"
#include "../rifle.hpp"

namespace mods::combat_composer {
	/**
	 * Handles ranged combat
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon);

};
#endif
