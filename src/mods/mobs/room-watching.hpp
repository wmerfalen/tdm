#ifndef __MENTOC_MODS_MOBS_ROOM_WATCHING_HEADER__
#define  __MENTOC_MODS_MOBS_ROOM_WATCHING_HEADER__

#include "../../globals.hpp"

namespace mods::mobs::room_watching {
	void watch_direction(uuid_t mob,room_rnum room,direction_t direction,depth_t depth);
	void stop_watching(uuid_t mob);
	namespace events {
		void room_entry(room_rnum room,uuid_t player);
		void room_exit(room_rnum room,uuid_t player);
	};
};
#endif
