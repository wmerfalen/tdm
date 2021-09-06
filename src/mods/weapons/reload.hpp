#ifndef __MENTOC_MODS_WEAPONS_RELOAD_HEADER__
#define __MENTOC_MODS_WEAPONS_RELOAD_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::weapons::reload {
	std::pair<obj_ptr_t,obj_ptr_t> reload_weapon(player_ptr_t& player,obj_ptr_t weapon);
};
#endif

