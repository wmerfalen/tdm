#ifndef __MENTOC_MODS_RATE_LIMITING_HEADER__
#define __MENTOC_MODS_RATE_LIMITING_HEADER__

#include "../globals.hpp"

namespace mods::rate_limiting {
	enum action_t {
		UNKNOWN_ACTION = -1,
		PLAYER_SAVE = 0,
	};
	bool should_throttle(action_t, player_ptr_t& player);
	void rate_limit_hit(action_t,player_ptr_t& player);
};
#endif
