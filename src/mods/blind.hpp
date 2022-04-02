#ifndef __MENTOC_MODS_blind_HEADER__
#define __MENTOC_MODS_blind_HEADER__
#include "../globals.hpp"

namespace mods::blind {
	void blind_for(player_ptr_t& victim, uint16_t ticks);
	void process_players();
	constexpr uint8_t tick_resolution() {
		return 12;
	}
};
#endif
