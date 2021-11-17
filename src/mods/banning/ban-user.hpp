#ifndef __MENTOC_MODS_BANNING_BAN_USER_HEADER__
#define  __MENTOC_MODS_BANNING_BAN_USER_HEADER__

#include "../../globals.hpp"
#include <functional>

namespace mods::banning::user {
	using reporter_t = std::function<void(int64_t,std::string_view)>;
	void set_reporter_lambda(reporter_t f);
	void set_to_banned(player_ptr_t& player_ptr);
	void remove_banned_status(player_ptr_t& player_ptr);
	void game_entry(player_ptr_t& player);
	int16_t load_char_pkid(player_ptr_t& player);
};

#endif
