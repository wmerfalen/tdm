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
#include "drone.hpp"

namespace mods {
	namespace scan {
		struct vec_player_data_element {
			char_data* ch;
			obj_data* obj;
			uint16_t distance;
			uuid_t uuid;
			uint8_t direction;
			uint32_t room_rnum;
		};
		using chptr = player_ptr_t;
		using vec_player_data = std::vector<vec_player_data_element>;
		using los_scan_foreach_callback = std::function<bool (room_rnum,int,vec_player_data)>;
		using distance_t = uint16_t;
		struct find_results_t {
			bool found;
			int direction;
			uint16_t distance;
			uint32_t room_rnum;
		};
		extern int directions[];
		constexpr unsigned num_directions = 6;
		using room_list_t = std::array<std::vector<room_rnum>,num_directions>;
		enum find_type_t {
			NPC = (1 << 1),
			ALIVE = (1 << 2),
			DEAD = (1 << 3),
			PLAYERS = (1 << 4),
			OBJECTS = (1 << 5),
			ANY = NPC | ALIVE | DEAD | PLAYERS | OBJECTS,
		};

		void los_scan(char_data* ch,int depth,vec_player_data* vec_room_list);
		void los_scan_for_players(char_data* ch,int depth,vec_player_data* vec_room_list);
		void los_scan_direction(char_data* ch,int depth,vec_player_data* vec_room_list,int direction,find_type_t f);
		void los_scan_direction(char_data* ch,int depth,vec_player_data* vec_room_list,int direction);
		int los_find_player(player_ptr_t& player,player_ptr_t& victim,int depth);

		/**
		 * Simply list rooms within line of sight for recursive_depth depth
		 */
		void los_list_rooms(chptr hunter,room_list_t& room_list,unsigned depth);
		/**
		 * Line of sight scan for hunted starting at hunter's current position
		 */
		find_results_t los_find(chptr hunter,chptr hunted);
		std::tuple<bool,distance_t> room_distance(room_rnum& source,room_rnum& dest);
		void los_list_by_room(room_rnum& in_room,room_list_t& room_list,unsigned depth);
		std::vector<uuid_t> drone_scan_room(room_rnum);
	};
};
#endif
