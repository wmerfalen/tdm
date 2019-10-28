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
		enum projectile_event_t {
			EV_TRAVEL,
			EV_SHRAPNEL,
			EV_EXPLODE,
			EV_PENETRATE,
			EV_IGNITE,
			EV_BURN,
			EV_SHRED,
			EV_PROPAGATE_EMP,
			EV_PROPAGATE_SMOKE,
			EV_PROPAGATE_CHAFF,
			EV_MELT,
			EV_CORRODE,
			EV_BLINDING_LIGHT,
			EV_STAIN,
			EV_DEFORM,
			__EV_FIRST = EV_TRAVEL,
			__EV_LAST = EV_DEFORM
		};
		using player_ptr_t = std::shared_ptr<mods::player>;
		using rooms_away_t = std::array<std::optional<room_rnum>,NUM_OF_DIRS>;
		typedef std::function<void (room_rnum,int)> callback;
		room_rnum cast_finite(struct char_data* source_char,room_rnum source_room,int direction,std::size_t depth,obj_data* nade);
		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth);
		int explosive_damage(player_ptr_t victim, obj_data* item);
		int chemical_damage(player_ptr_t victim, obj_data* item);
		int grenade_damage(player_ptr_t victim,obj_data* projectile);
		int fire_damage(player_ptr_t victim,obj_data* projectile);
		void travel_to(room_rnum from, int direction, std::size_t depth, obj_data* object);
		std::string todirstr(const char* direction,bool prefix,bool suffix);
		std::string fromdirstr(int direction,bool prefix, bool suffix);
		rooms_away_t calculate_shrapnel_rooms(room_rnum room, obj_data* held_object,std::size_t blast_radius);
		int8_t to_direction(const std::string&);
		void explode(room_rnum room,obj_data* object);
		void disable_electronics(room_rnum room);
		void smoke_room(room_rnum room);
		void blind_target(player_ptr_t victim);
		void disorient_target(player_ptr_t victim);
		void blindness_clears_up(player_ptr_t victim);
		void disorient_clears_up(player_ptr_t victim);
		void propagate_chemical_blast(room_rnum& room_id,obj_data* device);
	};
};
#endif
