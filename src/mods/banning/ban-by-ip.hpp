#ifndef __MENTOC_MODS_BANNING_BAN_BY_IP_HEADER__
#define  __MENTOC_MODS_BANNING_BAN_BY_IP_HEADER__

#include "../../globals.hpp"
#include <functional>

namespace mods::banning::by_ip {
	using reporter_t = std::function<void(int64_t,std::string_view)>;
	void set_reporter_lambda(reporter_t f);
	void extract_character(player_ptr_t& player);
	int16_t load_char_pkid(player_ptr_t& player);
	int16_t delete_char(player_ptr_t&);
	void game_entry(player_ptr_t& player);
};

#endif
