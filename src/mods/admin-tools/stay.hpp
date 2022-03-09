#ifndef __MENTOC_MODS_admin_tools_stay_HEADER__
#define __MENTOC_MODS_admin_tools_stay_HEADER__
#include "../../globals.hpp"

namespace mods::admin_tools::stay {
	void stay_player(player_ptr_t& player, room_vnum r_vnum);
	//std::tuple<bool,std::string> unstay_player(std::string_view username);
};
#endif
