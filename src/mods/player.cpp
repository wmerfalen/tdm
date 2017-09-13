#ifndef __MENTOC_MODS_PLAYER_SOURCE__
#define  __MENTOC_MODS_PLAYER_SOURCE__

#include "player.hpp"
#include "../structs.h"
namespace mods {
	player::player(char_data* ch) : m_char_data(ch) { };
	bool player::can_snipe(char_data *target){
		return true;
	}
};

#endif
