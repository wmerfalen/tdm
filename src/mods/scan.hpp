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
#include <tuple>

namespace mods {
	namespace scan {
		typedef struct {
			char_data* ch;
			uint16_t distance;
		} vec_player_data_element;
		using chptr = char_data*;
		using vec_player_data = std::vector<vec_player_data_element>;
		using los_scan_foreach_callback = std::function<bool (room_rnum,int,vec_player_data)>;
		using distance_t = uint16_t;
		struct find_results_t{
			bool found;
			int direction;
			uint16_t distance;
		};
		extern int directions[];
		constexpr unsigned num_directions = 6;
		using room_list_t = std::array<std::vector<room_rnum>,num_directions>;

		void los_scan(char_data* ch,int depth,vec_player_data* vec_room_list);
		void los_scan_foreach(char_data* ch,int depth,los_scan_foreach_callback lambda_cb);

		/**
		 * Simply list rooms within line of sight for recursive_depth depth
		 */
		void los_list_rooms(chptr hunter,room_list_t & room_list,unsigned depth);
		/**
		 * Line of sight scan for hunted starting at hunter's current position
		 */
		find_results_t los_find(chptr hunter,chptr hunted);
	};
};
#endif
