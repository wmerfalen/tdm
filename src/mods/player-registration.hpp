#ifndef __MENTOC_MODS_PLAYER_REGISTRATION_HEADER__
#define __MENTOC_MODS_PLAYER_REGISTRATION_HEADER__

#include "../globals.hpp"

namespace mods::player_registration {
	void allow(bool on_off);
	bool allowed();
};
#endif
