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

namespace mods::globals {
	extern std::unique_ptr<mods::deferred> defer_queue;
};
namespace mods {
	namespace projectile {
		using obj_ptr_t = std::shared_ptr<obj_data>;
		constexpr static int FLASHBANG_COOLDOWN_TICKS = 4; /** FIXME: a wild guess */
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
			EV_EXPOSED,
			__EV_FIRST = EV_TRAVEL,
			__EV_LAST = EV_EXPOSED
		};
		using player_ptr_t = std::shared_ptr<mods::player>;
		using rooms_away_t = std::array<std::optional<room_rnum>,NUM_OF_DIRS>;
		typedef std::function<void (room_rnum,int)> callback;
		room_rnum cast_finite(char_data* source_char,room_rnum source_room,int direction,std::size_t depth,obj_ptr_t nade);
		room_rnum resolve_room(room_rnum source_room,int direction,std::size_t depth);
		int explosive_damage(player_ptr_t victim, obj_ptr_t item);
		int chemical_damage(player_ptr_t victim, obj_ptr_t item);
		int grenade_damage(player_ptr_t victim,obj_ptr_t projectile);
		int fire_damage(player_ptr_t victim,obj_ptr_t projectile);
		int travel_to(room_rnum from, int direction, std::size_t depth, obj_ptr_t object);
		void explode_in_future(int room_id, int ticks, uuid_t object_uuid,uuid_t player_uuid);
		std::string todirstr(int direction,bool prefix,bool suffix);
		std::string fromdirstr(int direction,bool prefix, bool suffix);
		rooms_away_t calculate_shrapnel_rooms(room_rnum room, obj_ptr_t held_object,std::size_t blast_radius);
		int to_direction(const std::string&);
		int8_t explode(room_rnum room,uuid_t object_uuid,uuid_t player_uuid);
		void disable_electronics(room_rnum room);
		void smoke_room(room_rnum room);
		void blind_target(player_ptr_t victim);
		void disorient_target(player_ptr_t victim);
		void blindness_clears_up(player_ptr_t victim);
		void disorient_clears_up(player_ptr_t victim);
		void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device);
		int deploy_shrapnel_at(player_ptr_t& victim, int dice_count,int dice_sides,int from_direction);

		void throw_object(player_ptr_t& player, int direction, std::size_t depth,
		                  obj_ptr_t& object, std::string_view verb);
	};
};
#endif
