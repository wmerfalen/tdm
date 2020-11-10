#ifndef __MENTOC_MODS_TARGET_PRACTICE_DB_HEADER__
#define __MENTOC_MODS_TARGET_PRACTICE_DB_HEADER__

#include "../globals.hpp"
#include "db.hpp"

namespace mods::target_practice_db {
	void save_dummy(player_ptr_t& player, std::vector<std::string>& args);
	void remove_dummy(player_ptr_t& player, std::vector<std::string>& args);
	void instantiate_dummy(player_ptr_t& player, std::vector<std::string>& args);
	void queue_dummy_on_room(room_vnum room);
};
#endif
