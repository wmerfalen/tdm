#ifndef __MENTOC_MODS_RESTING_HEADER__
#define __MENTOC_MODS_RESTING_HEADER__

#include "../globals.hpp"

namespace mods::resting {
	void add_player_resting(player_ptr_t& player);
	void process_players_resting();
	void remove_player_resting(player_ptr_t player);
};
#endif
