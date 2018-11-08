#include "player-scaffolding.hpp"
//#include "../globals.hpp"
//#include "../structs.h"
//#include "../utils.h"
//#include "../screen.h"
//#include "weapon.hpp"
//#include "acl/color.hpp"
//#include "player.hpp"
//#define DRONE_OWNER_KEY "{player}:player_scaffolding"
//#include "extern.hpp"
//
namespace mods {
	player_scaffolding::player_scaffolding(mods::player& ch){
		m_player = ch;
	};
	std::shared_ptr<mods::player> player_scaffolding::create_target() {

	}
	auto player = mods::globals::player_list.emplace_back(
			std::make_shared<mods::player>()
	);
	player->set_desc(owner->desc);
	player->desc().character = player->cd();
	/** TODO: !uncertainty! -> question: "Does this need to be uncommented?" */
	//player->set_char_on_descriptor();
		auto ch = player->cd();
		ch->player_scaffolding = true;
		ch->desc = owner->desc;
		ch->player_scaffolding_owner = owner->uuid;
		ch->player.name.assign("A player_scaffolding");
		owner->player_scaffolding_uuid = ch->uuid;
		owner->player_scaffolding_simulate = true;
		IN_ROOM(ch) = IN_ROOM(owner);
		ch->points.move = 9999;
		ch->points.max_move = 9999;
		//memset(&ch->char_specials,0,sizeof(ch->char_specials));
		ch->char_specials.position = POS_STANDING;
		ch->char_specials.saved.idnum = ch->uuid;
		REMOVE_BIT(MOB_FLAGS(ch),MOB_ISNPC);
		char_from_room(ch);
		char_to_room(ch,IN_ROOM(owner));
		SET_BIT(ch->char_specials.saved.affected_by,AFF_SNEAK);
		SET_BIT(ch->player_specials->saved.pref, PRF_AUTOEXIT);
		return player->cd();
	}
	 void player_scaffolding::start(struct char_data* owner) {
		auto player_scaffolding = mods::player_scaffolding::get_existing(owner);

		if(player_scaffolding == nullptr) {
			player_scaffolding = mods::player_scaffolding::create(owner);
		}

		if(IN_ROOM(player_scaffolding) != NOWHERE) {
			char_from_room(player_scaffolding);
		}

		char_to_room(player_scaffolding,IN_ROOM(owner));
		owner->player_scaffolding_simulate = true;
	}
	 void player_scaffolding::stop(struct char_data* owner) {
		owner->player_scaffolding_simulate = false;
	}
	 bool player_scaffolding::started(struct char_data* owner) {
		return owner->player_scaffolding_simulate;
	}
	 void player_scaffolding::simulate(struct char_data* owner,bool value) {
		owner->player_scaffolding_simulate = value;
	}
	 void player_scaffolding::get_player_scaffolding(struct char_data* owner) {
		auto player_scaffolding = mods::globals::player_list[owner->player_scaffolding_uuid]->cd();

		if(IN_ROOM(owner) == IN_ROOM(player_scaffolding)) {
			if(IN_ROOM(player_scaffolding) != NOWHERE) {
				char_from_room(player_scaffolding);
			}

			char_to_room(player_scaffolding,NOWHERE);
		} else {
			send_to_char(owner,"Must be in the same room as your player_scaffolding\r\n");
		}
	}
	 struct char_data * player_scaffolding::get_existing(struct char_data* owner) {
		if(owner->player_scaffolding_uuid == 0 || owner->player_scaffolding_uuid > mods::globals::player_list.size() -1) {
			return nullptr;
		}

		return mods::globals::player_list[owner->player_scaffolding_uuid]->cd();
	}

	 bool player_scaffolding::interpret(struct char_data *owner,const std::string& argument) {
		auto player_scaffolding = mods::globals::player_list[owner->player_scaffolding_uuid];

		if(argument.compare("player_scaffolding stop") == 0) {
			mods::player_scaffolding::stop(owner);
			return true;
		}

		if(argument.compare("scan") == 0) {
			for(auto & ch : mods::globals::room_list[player_scaffolding->cd()->in_room]){
				mods::player(ch) << "You have been spotted.\r\n";
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
				command_interpreter(player_scaffolding->cd(),argument.substr(0,1).c_str());
				return false;

			default:
				return false;
		}
	}
};

