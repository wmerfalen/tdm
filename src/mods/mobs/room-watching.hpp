#ifndef __MENTOC_MODS_MOBS_ROOM_WATCHING_HEADER__
#define  __MENTOC_MODS_MOBS_ROOM_WATCHING_HEADER__

#include "../../globals.hpp"

namespace mods::mobs::room_watching {
	void watch_direction(uuid_t mob,room_rnum room,direction_t direction,depth_t depth);
	void stop_watching(uuid_t mob);
	void watch_room(const uuid_t& mob,const room_rnum& room);
	void unwatch_room(const uuid_t& mob,const room_rnum& room);
	void watch_room(player_ptr_t&);
	void unwatch_room(player_ptr_t&);

	namespace events {
		void room_entry(const room_rnum& room,const uuid_t& player);
		void room_entry(player_ptr_t& player);
		void room_exit(player_ptr_t& player);
		void room_exit(const room_rnum& room,const uuid_t& player);
	};
};
#endif
