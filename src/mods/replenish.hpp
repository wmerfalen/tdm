#ifndef __MENTOC_MODS_REPLENISH_HEADER__
#define __MENTOC_MODS_REPLENISH_HEADER__

#include "../globals.hpp"
#include "classes/ghost.hpp"
#include "classes/pyrexia.hpp"
#include "classes/forge.hpp"
#include "classes/syndrome.hpp"
#include "classes/contagion.hpp"
#include <memory>

namespace mods::replenish {
	void destroy_player(uuid_t player_uuid);
	void register_sniper(uuid_t);
	void register_marine(uuid_t);
	void register_breacher(uuid_t);
	void register_engineer(uuid_t);
	void register_ghost(uuid_t);
	void register_contagion(uuid_t);
	void register_medic(uuid_t);
	void register_support(uuid_t);
	void run();
};
#endif
