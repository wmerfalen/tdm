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
#define DRONE_OWNER_KEY "{player}:drone"
#include "extern.hpp"

extern player_ptr_t new_player();
extern void char_to_room(player_ptr_t,room_rnum);
namespace mods {
	drone_impl::drone_impl(uuid_t ch) : m_char_data(nullptr) { };

	namespace drone {
	player_ptr_t create(uuid_t owner_uuid) {
		auto owner = ptr_by_uuid(owner_uuid);
		auto player = new_player();
		player->set_socket(owner->socket());
		player->desc().login_time = time(0);
		player->desc().has_prompt = 1;  /* prompt is part of greetings */
		player->set_state(CON_PLAYING);
		mods::globals::register_player(player);
		//player->set_desc(owner->cd()->desc);
		//player->desc().character = player->cd();
		/** TODO: !uncertainty! -> question: "Does this need to be uncommented?" */
		//player->set_char_on_descriptor();
		auto ch = player->cd();
		ch->drone = true;
		ch->desc = owner->cd()->desc;
		ch->drone_owner = owner->uuid();
		ch->player.name.assign("A drone");
		owner->cd()->drone_uuid = ch->uuid;
		owner->cd()->drone_simulate = true;
		IN_ROOM(ch) = owner->room();
		ch->points.move = 9999;
		ch->points.max_move = 9999;
		//memset(&ch->char_specials,0,sizeof(ch->char_specials));
		ch->char_specials.position = POS_STANDING;
		ch->char_specials.saved.idnum = ch->uuid;
		REMOVE_BIT(MOB_FLAGS(ch),MOB_ISNPC);
		//char_from_room(ch);
		std::cerr << "room:" << owner->room() << "\n";
		//char_to_room(player->cd(),owner->room());
		SET_BIT(ch->char_specials.saved.affected_by,AFF_SNEAK);
		SET_BIT(ch->player_specials->saved.pref, PRF_AUTOEXIT);
		return player;
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

		//if(drone_ptr->room() != NOWHERE) {
		//	char_from_room(drone_ptr->cd());
		//}

		//char_to_room(drone_ptr->cd(),owner->room());
		owner->cd()->drone_simulate = true;
	}
	void stop(uuid_t  owner_uuid) {
		auto owner = ptr_by_uuid(owner_uuid);
		owner->cd()->drone_simulate = false;
	}
	bool started(uuid_t  owner_uuid) {
		auto owner = ptr_by_uuid(owner_uuid);
		std::cerr << "started " << (owner->cd()->drone_simulate ? " yes" : "no") << "\n";
		return owner->cd()->drone_simulate;
	}
	void simulate(uuid_t  owner_uuid,bool value) {
		auto owner = ptr_by_uuid(owner_uuid);
		owner->cd()->drone_simulate = value;
	}
	void get_drone(uuid_t  owner_uuid) {
		//auto drone = mods::globals::player_list[owner->drone_uuid]->cd();

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

	bool interpret(uuid_t owner_uuid,const std::string& argument) {
		auto owner = ptr_by_uuid(owner_uuid);
		auto drone = ptr_by_uuid(owner->drone_uuid());

		if(argument.compare("drone stop") == 0) {
			mods::drone::stop(owner_uuid);
			return true;
		}

		if(argument.compare("scan") == 0) {
			for(auto & player_ptr : mods::globals::get_room_list(drone->cd()->in_room)){
				*player_ptr << "You have been spotted.\r\n";
			}
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
