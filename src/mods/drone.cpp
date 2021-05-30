#ifndef __MENTOC_MODS_DRONE_SOURCE__
#define  __MENTOC_MODS_DRONE_SOURCE__

#include "drone.hpp"
#include "../globals.hpp"
#include "../structs.h"
#include "../utils.h"
#include "../screen.h"
#include "weapon.hpp"
#include "acl/color.hpp"
#include "player.hpp"
#include "extern.hpp"
#include "send-to.hpp"
#include "scan.hpp"

extern player_ptr_t new_player();
extern void char_to_room(player_ptr_t,room_rnum);
namespace mods {
	drone_impl::drone_impl(uuid_t ch) : m_char_data(nullptr) { };

	namespace drone {
		player_ptr_t create(uuid_t owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			auto drone = new_player();
			mods::globals::register_player(drone);
			drone->set_socket(owner->socket());
			drone->desc().login_time = time(0);
			drone->desc().has_prompt = 1;  /* prompt is part of greetings */
			drone->set_state(CON_PLAYING);
			drone->cd()->drone = true;
			drone->cd()->desc = owner->cd()->desc;
			drone->cd()->drone_owner = owner->uuid();
			drone->cd()->player.name.assign("A drone");
			owner->drone_uuid() = drone->uuid();
			owner->drone_simulate() = true;
			IN_ROOM(drone->cd()) = owner->room();
			drone->cd()->points.move = 9999;
			drone->cd()->points.max_move = 9999;
			drone->char_specials().position = POS_STANDING;
			drone->char_specials().saved.idnum = drone->uuid();
			REMOVE_BIT(MOB_FLAGS(drone->cd()),MOB_ISNPC);
			SET_BIT(drone->char_specials().saved.affected_by,AFF_SNEAK);
			SET_BIT(drone->cd()->player_specials->saved.pref, PRF_AUTOEXIT);
			return drone;
		}
		void start(uuid_t  owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			auto drone = mods::drone::get_existing(owner_uuid);
			player_ptr_t drone_ptr = nullptr;

			if(drone == 0) {
				drone_ptr = mods::drone::create(owner_uuid);
			} else {
				drone_ptr = ptr_by_uuid(drone);
			}
			owner->drone_simulate() = true;
		}
		void stop(uuid_t  owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			owner->drone_simulate() = false;
		}
		bool started(uuid_t  owner_uuid) {
			return ptr_by_uuid(owner_uuid)->drone_simulate();
		}
		void simulate(uuid_t  owner_uuid,bool value) {
			auto owner = ptr_by_uuid(owner_uuid);
			owner->drone_simulate() = value;
		}
		void get_drone(uuid_t  owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			auto drone = ptr_by_uuid(owner->drone_uuid());
			if(owner->room() == drone->room()) {
				if(drone->room() != NOWHERE) {
					char_from_room(drone->cd());
				}
				char_to_room(drone->cd(),NOWHERE);
			} else {
				owner->sendln("Must be in the same room as your drone");
			}
		}
		uuid_t  get_existing(uuid_t  owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			return owner->drone_uuid();
		}
		std::pair<player_ptr_t,player_ptr_t> owner_drone(uuid_t owner_uuid) {
			auto owner = ptr_by_uuid(owner_uuid);
			auto drone = ptr_by_uuid(owner->drone_uuid());
			return {owner,drone};
		}

		bool interpret(uuid_t owner_uuid,const std::string& argument) {
			auto p = owner_drone(owner_uuid);
			auto owner = p.first;
			auto drone = p.second;

			if(mods::util::is_lower_match(argument, "drone stop")) {
				mods::drone::stop(owner_uuid);
				return true;
			}

			if(mods::util::is_lower_match(argument, "scan")) {
				mods::scan::drone_scan_room(drone->room());
				send_to_room_except(drone->room(), {drone,owner}, "You have been spotted.");
				return false;
			}

			switch(argument[0]) {
				case 'n':
				case 'e':
				case 's':
				case 'w':
				case 'N':
				case 'E':
				case 'S':
				case 'W':
				case 'l':
					command_interpreter(drone,argument.substr(0,1).c_str());
					return false;

				default:
					return false;
			}
		}
	};//end namespace drone
};

#endif
