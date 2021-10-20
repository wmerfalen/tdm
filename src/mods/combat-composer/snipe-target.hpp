#ifndef __MENTOC_MODS_combat_composer_snipe_target_HEADER__
#define __MENTOC_MODS_combat_composer_snipe_target_HEADER__
#include "../../globals.hpp"
#include "../rifle.hpp"

namespace mods::combat_composer {
	/**
	 * Handles both ranged and immediate targets
	 */
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon);

	/**
	 * Handles spraying only
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon);

	/**
	 * Handles launching only
	 */
	void launch_towards(player_ptr_t& attacker,direction_t direction,uint8_t distance,obj_ptr_t& weapon);

};
#endif
