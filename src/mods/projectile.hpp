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

		template <typename TObjectPtr>
		void throw_object(player_ptr_t player, int direction, std::size_t depth,
				TObjectPtr object, std::string_view verb){
			std::array<char,MAX_INPUT_LENGTH> str_dir_buffer;
			std::string str_dir = mods::projectile::todirstr(static_cast<const char*>(&str_dir_buffer[0]),1,0);
			int8_t dir = mods::projectile::to_direction(&str_dir_buffer[0]);
			if(dir < 0){
				player->sendln("Use a valid direction!");
				return;
			}
			std::string object_name = "";
			int ticks = 0;
			switch(object->explosive()->type) {
				default:
				case mw_explosive::REMOTE_EXPLOSIVE:
				case mw_explosive::REMOTE_CHEMICAL:
				case mw_explosive::CLAYMORE_MINE:
				case mw_explosive::EXPLOSIVE_NONE:
					player->sendln("This type of explosive is not throwable!");
					return;
					break;
				case mw_explosive::FRAG_GRENADE:
					ticks = 2;
					object_name = "frag grenade";
					break;
				case mw_explosive::INCENDIARY_GRENADE:
					object_name = "incendiary grenade";
					ticks = 2;
					break;
				case mw_explosive::EMP_GRENADE:
					object_name = "emp grenade";
					ticks = 2;
					break;
				case mw_explosive::SMOKE_GRENADE:
					object_name = "smoke grenade";
					ticks = 3;
					break;
				case mw_explosive::FLASHBANG_GRENADE:
					object_name = "flashbang grenade";
					ticks = 1;
					break;
			}
			obj_from_char(object);
			send_to_room_except(player->room(), player, "%s %ss a %s%s!\r\n",
					player->ucname().c_str(), 
					verb.data(),
					object_name.c_str(),
					str_dir.c_str());
			player->sendln("You " + std::string(verb.data()) + " a " + object->explosive()->name + str_dir);
			auto room_id = mods::projectile::cast_finite(player->cd(),player->room(),direction,depth,object);
			mods::projectile::travel_to(player->room(), direction, depth, object);
			obj_to_room(object,room_id);
			mods::globals::defer_queue->push(ticks, [room_id,&object]() {
				mods::projectile::explode(room_id,object);
			});
		}
	};
};
#endif
