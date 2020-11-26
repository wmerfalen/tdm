#ifndef __MENTOC_MODS_PLAYERS_DB_LOAD_HEADER__
#define  __MENTOC_MODS_PLAYERS_DB_LOAD_HEADER__

#include "../../globals.hpp"
#include <functional>

namespace mods::players::db_load {
	void set_class(player_ptr_t& player, player_class_t class_type);
	using reporter_t = std::function<void(int64_t,std::string_view)>;
	void set_reporter_lambda(reporter_t f);
	void save(player_ptr_t& player);
	void load_base_abilities(player_ptr_t& player);
	int16_t save_new_char(player_ptr_t& player);
	int16_t load_char_pkid(player_ptr_t& player);
	void feed_player_inventory(player_ptr_t& player);
	int16_t delete_char(player_ptr_t&);
};

#endif
