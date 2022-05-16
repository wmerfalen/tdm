#ifndef __MENTOC_MODS_no_drop_HEADER__
#define __MENTOC_MODS_no_drop_HEADER__
#include "../globals.hpp"

namespace mods::no_drop {
	bool npc_is_no_drop(const player_ptr_t& npc);
	bool should_drop(const player_ptr_t& npc);
};
#endif
