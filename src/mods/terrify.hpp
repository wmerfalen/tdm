#ifndef __MENTOC_MODS_terrify_HEADER__
#define __MENTOC_MODS_terrify_HEADER__
#include "../globals.hpp"

namespace mods::terrify {
	void terrify_for(player_ptr_t victim, uint32_t ticks);
	void process_players();
	constexpr uint8_t tick_resolution() {
		return 9;
	}
};
#endif
