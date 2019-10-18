#ifndef __MENTOC_MODS_PROJECTILE_HEADER__
#define __MENTOC_MODS_PROJECTILE_HEADER__
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"

namespace mods {
	namespace projectile {
		using player_ptr_t = std::shared_ptr<mods::player>;
		typedef std::function<void (room_rnum,int)> callback;
		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,std::size_t depth,obj_data* nade);
		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth);
		int grenade_damage(player_ptr_t victim,obj_data* projectile);
		void travel_to(room_rnum from, int direction, std::size_t depth, obj_data* object);
		std::string todirstr(const char* direction,bool prefix,bool suffix);
		std::string fromdirstr(int direction,bool prefix, bool suffix);
	};
};
#endif
