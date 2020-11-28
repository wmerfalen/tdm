#ifndef __MENTOC_MODS_REPLENISH_HEADER__
#define __MENTOC_MODS_REPLENISH_HEADER__

#include "../globals.hpp"
#include "classes/ghost.hpp"
#include "classes/pyrexia.hpp"
#include "classes/forge.hpp"
#include "classes/syndrome.hpp"
#include <memory>

namespace mods::replenish {
	void destroy_player(uuid_t player_uuid);
	void register_ghost(uuid_t);
	void register_pyrexia(uuid_t);
	void register_forge(uuid_t);
	void register_syndrome(uuid_t);
	void run();
};
#endif
