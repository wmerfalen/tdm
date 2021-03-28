#ifndef __MENTOC_MODS_BUILDER_BOOKMARKS_HEADER__
#define __MENTOC_MODS_BUILDER_BOOKMARKS_HEADER__
#include "../structs.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../interpreter.h"

namespace mods::builder::bookmarks {
	void add_room(player_ptr_t& player,const room_rnum& room_id);
	std::vector<room_rnum>& get_rooms(player_ptr_t& player);
	void teleport_to(player_ptr_t& player,const room_rnum& room);
};

#endif
