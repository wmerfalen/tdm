#ifndef __MENTOC_MODS_combat_composer_engage_target_HEADER__
#define __MENTOC_MODS_combat_composer_engage_target_HEADER__
#include "../../globals.hpp"
#include "../rifle.hpp"

namespace mods::combat_composer {
	/**
	 * Handles same room targets
	 */
	bool engage_target(player_ptr_t& attacker,std::string_view target,obj_ptr_t& weapon);
	bool engage_target(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon);

};
#endif
