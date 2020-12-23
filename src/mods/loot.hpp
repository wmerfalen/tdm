#ifndef __MENTOC_MODS_LOOT_HEADER__
#define __MENTOC_MODS_LOOT_HEADER__
#include <iostream>
#include "forge-engine/generator.hpp"

namespace mods::forge_engine {
	extern mods::forge_engine::generator item_generator;
};
namespace mods::loot {
	obj_ptr_t reward_player(player_ptr_t& player);
};

#endif
