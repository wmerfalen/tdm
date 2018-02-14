#ifndef __MENTOC_MODS_SCAN_HEADER__
#define __MENTOC_MODS_SCAN_HEADER__
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
	namespace scan {
		typedef struct {
			char_data* ch;
			uint16_t distance;
		} vec_player_data_element;
		typedef std::vector<vec_player_data_element> vec_player_data;
		typedef std::function<void (room_rnum,int,vec_player_data)> los_scan_foreach_callback;
		void los_scan(struct char_data* ch,int depth,vec_player_data* vec_room_list);
		void los_scan_foreach(struct char_data* ch,int depth,los_scan_foreach_callback lambda_cb);
	};
};
#endif
