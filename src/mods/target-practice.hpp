#ifndef __MENTOC_MODS_TARGET_PRACTICE_HEADER__
#define __MENTOC_MODS_TARGET_PRACTICE_HEADER__

#include "../globals.hpp"

namespace mods::target_practice {
	struct dummy_t {

	};
	void init();
	void new_room(room_data* room);
	void instantiate_dummy(player_ptr_t& player,std::string_view identifier);
};
#endif
