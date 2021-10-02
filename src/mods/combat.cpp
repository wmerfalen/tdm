#include "combat.hpp"
#include "combat-composer.hpp"

namespace mods::combat {
	/**
	 * Handles both ranged and immediate targets
	 */
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		mods::combat_composer::snipe_target(attacker,target,direction,distance,weapon);
	}

	/**
	 * Handles spraying only
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {
		mods::combat_composer::spray_target(attacker,direction,weapon);

	}

	/**
	 * Handles launching only
	 */
	void launch_towards(player_ptr_t& attacker,direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		mods::combat_composer::launch_towards(attacker,direction,distance,weapon);

	}

};
