#ifndef __MENTOC_MODS_PFIND_HEADER__
#define  __MENTOC_MODS_PFIND_HEADER__

#include "../globals.hpp"
#include "player.hpp"
#include <optional>

namespace mods::pfind {
	player_ptr_t by_name(const char* player_name);
	player_ptr_t by_uuid(uuid_t player_uuid);
	std::optional<player_ptr_t> optby_name(const char* player_name);
	std::optional<player_ptr_t> optby_uuid(uuid_t player_uuid);
};

#endif
