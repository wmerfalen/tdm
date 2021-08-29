#ifndef __MENTOC_MODS_LOOT_CONTAINER_HEADER__
#define __MENTOC_MODS_LOOT_CONTAINER_HEADER__
#include <iostream>
#include "forge-engine/generator.hpp"
#include "lmdb.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "forge-engine/generator.hpp"
#include "forge-engine/generated-rifle.hpp"
#include "forge-engine/generated-armor.hpp"

namespace mods::loot_container {
	void store(player_ptr_t& player, obj_ptr_t&& obj);
};

#endif
