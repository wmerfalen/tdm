#ifndef __MENTOC_MODS_LOOT_HEADER__
#define __MENTOC_MODS_LOOT_HEADER__
#include <iostream>
#include "forge-engine/generator.hpp"
#include "lmdb.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"
#include "forge-engine/generator.hpp"
#include "forge-engine/generated-rifle.hpp"
#include "forge-engine/generated-armor.hpp"

namespace mods::forge_engine {
	extern mods::forge_engine::generator item_generator;
};
namespace mods::loot {
	obj_ptr_t reward_player(player_ptr_t& player);
};

namespace mods::loot::events {
	void player_killed_npc(player_ptr_t& player,player_ptr_t& npc);
};
#endif
