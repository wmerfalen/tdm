#ifndef __MENTOC_MODS_REPLENISH_HEADER__
#define __MENTOC_MODS_REPLENISH_HEADER__

#include "../globals.hpp"
#include "classes/ghost.hpp"
#include <memory>

namespace mods::replenish {
	void destroy_player(uuid_t player_uuid);
	void register_ghost(uuid_t);
	void run();
};
#endif
