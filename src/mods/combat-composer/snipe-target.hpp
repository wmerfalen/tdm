#ifndef __MENTOC_MODS_combat_composer_snipe_target_HEADER__
#define __MENTOC_MODS_combat_composer_snipe_target_HEADER__
#include "../../globals.hpp"
#include "../rifle.hpp"
#include "shared.hpp"

namespace mods::combat_composer {
	/**
	 * Handles ranged combat
	 */
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon);

};
#endif
