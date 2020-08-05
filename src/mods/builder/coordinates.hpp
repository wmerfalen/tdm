#ifndef __MENTOC_MODS_BUILDER_COORDINATES_HEADER__
#define __MENTOC_MODS_BUILDER_COORDINATES_HEADER__
#include "../../globals.hpp"

namespace mods::builder::coordinates {
	std::string_view explain_make();
	void make_center(room_rnum r);
	void send_rbuild_help(player_ptr_t& player);
	bool handled_rbuild_command(player_ptr_t& player,str_t arguments);
};

#endif
