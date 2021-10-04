#ifndef __MENTOC_MODS_combat_HEADER__
#define __MENTOC_MODS_combat_HEADER__
#include "../globals.hpp"

namespace mods::combat {
	namespace immediate {
		/**
		 * For attacking targets (with a rifle) in the same room.
		 * If primary is a ranged only rifle, then it will check the underbarrel.
		 * 	- if the underbarrel is a shotgun, use that
		 * Next, if the primary is ranged only and player has secondary, it will use
		 * the secondary.
		 * If all else fails, it will fallback to melee combat.
		 */
		void kill_target(player_ptr_t& attacker,std::string_view target);
	};
	namespace ranged {

		/**
		 * Handles ranged targets only
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

};
#endif
