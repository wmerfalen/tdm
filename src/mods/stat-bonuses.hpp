#ifndef __MENTOC_MODS_STAT_BONUSES_HEADER__
#define __MENTOC_MODS_STAT_BONUSES_HEADER__
#include <iostream>
#include <map>
#include <variant>
#include "forge-engine/generator.hpp"

namespace mods::stat_bonuses {
	void player_carry(player_ptr_t& player, obj_ptr_t& object);
	void player_uncarry(player_ptr_t& player, obj_ptr_t& object);
	void player_wear(player_ptr_t& player,obj_ptr_t& object);
	void player_unwear(player_ptr_t& player,obj_ptr_t& object);
};

#endif
