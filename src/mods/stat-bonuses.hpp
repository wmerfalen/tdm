#ifndef __MENTOC_MODS_STAT_BONUSES_HEADER__
#define __MENTOC_MODS_STAT_BONUSES_HEADER__
#include <iostream>
#include <map>
#include <variant>
#include "forge-engine/generator.hpp"

namespace mods::stat_bonuses {
	void player_carry(uuid_t player, uuid_t object);
	void player_uncarry(uuid_t player, uuid_t object);
	void player_wear(uuid_t player,uuid_t object);
	void player_unwear(uuid_t player,uuid_t object);
};

#endif
