#ifndef __MENTOC_MODS_PFIND_HEADER__
#define  __MENTOC_MODS_PFIND_HEADER__

#include "../globals.hpp"
#include "player.hpp"

namespace mods::pfind {
	player_ptr_t by_name(const char* player_name);
	player_ptr_t by_uuid(uuid_t player_uuid);
};

#endif
