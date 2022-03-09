#ifndef __MENTOC_MODS_sneak_HEADER__
#define __MENTOC_MODS_sneak_HEADER__
#include "player.hpp"

namespace mods::sneak {
	void sneak(player_ptr_t& player);
	void apply_absolute_sneak(player_ptr_t& player, uint8_t duration);
	void clear_sneak(player_ptr_t& player);
};

#endif
