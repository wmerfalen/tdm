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
#include "../mods/utils.hpp"
#include "../globals.hpp"

namespace mods {
	namespace projectile {
		typedef std::function<void (room_rnum,int)> callback;
		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,int depth);
		room_rnum resolve_room(room_rnum source_room,int direction,int depth);
		int grenade_damage(struct char_data* source_char,struct char_data* victim,int damage,int attack_type);
	};
};
#endif
