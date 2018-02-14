#ifndef __MENTOC_MODS_DRONE_SOURCE__
#define  __MENTOC_MODS_DRONE_SOURCE__

#include "drone.hpp"
#include "../structs.h"
#include "../utils.h"
#include "../screen.h"
#include "weapon.hpp"
#include "../globals.hpp"
#include "acl/color.hpp"
#include "player.hpp"
#define DRONE_OWNER_KEY "{player}:drone"
#include "extern.hpp"

namespace mods {
	drone::drone(char_data* ch) : m_char_data(ch) { };
	static struct char_data* drone::create(struct char_data* owner) {
		struct char_data *ch = mods::globals::create_char();
		ch->drone = true;
		ch->desc = owner->desc;
		ch->drone_owner = owner->uuid;
		ch->player.name = strdup("A drone");
		mods::globals::register_player(ch);
		owner->drone_uuid = ch->uuid;
		owner->drone_simulate = true;
		IN_ROOM(ch) = IN_ROOM(owner);
		ch->points.move = 9999;
		ch->points.max_move = 9999;
		memset(&ch->char_specials,0,sizeof(ch->char_specials));
		ch->char_specials.position = POS_STANDING;
		ch->char_specials.saved.idnum = ch->uuid;
		REMOVE_BIT(MOB_FLAGS(ch),MOB_ISNPC);
		char_from_room(ch);
		char_to_room(ch,IN_ROOM(owner));
		SET_BIT(ch->char_specials.saved.affected_by,AFF_SNEAK);
		SET_BIT(ch->player_specials->saved.pref, PRF_AUTOEXIT);
		return ch;
	}
	static void drone::start(struct char_data* owner) {
		auto drone = mods::drone::get_existing(owner);

		if(drone == nullptr) {
			drone = mods::drone::create(owner);
		}

		if(IN_ROOM(drone) != NOWHERE) {
			char_from_room(drone);
		}

		char_to_room(drone,IN_ROOM(owner));
		owner->drone_simulate = true;
	}
	static void drone::stop(struct char_data* owner) {
		owner->drone_simulate = false;
	}
	static bool drone::started(struct char_data* owner) {
		return owner->drone_simulate;
	}
	static void drone::simulate(struct char_data* owner,bool value) {
		owner->drone_simulate = value;
	}
	static void drone::get_drone(struct char_data* owner) {
		auto drone = mods::globals::player_list[owner->drone_uuid]->cd();

		if(IN_ROOM(owner) == IN_ROOM(drone)) {
			if(IN_ROOM(drone) != NOWHERE) {
				char_from_room(drone);
			}

			char_to_room(drone,NOWHERE);
		} else {
			send_to_char(owner,"Must be in the same room as your drone\r\n");
		}
	}
	static struct char_data * drone::get_existing(struct char_data* owner) {
		if(owner->drone_uuid == 0 || owner->drone_uuid > mods::globals::player_list.size() -1) {
			return nullptr;
		}

		return mods::globals::player_list[owner->drone_uuid]->cd();
	}

	static bool drone::interpret(struct char_data *owner,const std::string& argument) {
		auto drone = mods::globals::player_list[owner->drone_uuid];

		if(argument.compare("drone stop") == 0) {
			mods::drone::stop(owner);
			return true;
		}

		if(argument.compare("scan") == 0) {
			command_interpreter(drone->cd(),"scan");
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
				command_interpreter(drone->cd(),argument.substr(0,1).c_str());
				return false;

			default:
				return false;
		}
	}
};

#endif
