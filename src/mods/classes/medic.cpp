#include "medic.hpp"

namespace mods::classes {
	int scaled_heal(std::shared_ptr<mods::player>& target){
		return (33 * (target->level() * 0.5));
	}
	void medic::heal_player(std::shared_ptr<mods::player>& player_obj,std::shared_ptr<mods::player>& target) {
		/*
		if(target->cd()->in_room != player_obj->cd()->in_room) {
			*player_obj << "You can't find your target.\r\n";
			return;
		}

		if(stim_pistol_ammo == 0) {
			*player_obj << "Out of ammo.\r\n";
			return;
		}

		if(target->cd() == player_obj->cd()) {
			const char* self_name = (player_obj->sex() == SEX_MALE ? "himself" : "herself");
			const char* short_self_name = (player_obj->sex() == SEX_MALE ? "his" : "her");
			*player_obj << "You aim your stim pistol at yourself...\r\n";
			send_to_room_except(player_obj->room(),
				{player_obj->cd()},
				"%s aims a stim pistol at %s\r\n",
				player_obj->name().c_str(),self_name);
			*player_obj << "You fire your stim pistol.\r\n";
			send_to_room_except(player_obj->room(),
				{player_obj->cd()},
				"%s fires a stim pistol into %s arm\r\n",
				player_obj->name().c_str(),short_self_name);
			auto amount = scaled_heal(player_obj);
			player_obj->hp() += amount;
			*player_obj << "You gain " << amount << " hit points.\r\n";
			return;
		}

		*player_obj << "You aim your stim pistol at " <<
		            target->name() << " carefully...\r\n";
		const char* gender = (player_obj->cd()->player.sex == SEX_MALE ? "his" : "her");
		send_to_room_except(player_obj->room(),
			{player_obj->cd(),target->cd()},
			"%s aims a stim pistol at %s\r\n",
			player_obj->name().c_str(),
			target->name().c_str()
		);
		*target << player_obj->name() << " aims " << gender <<
		        " stim pistol at you...\r\n";
		*player_obj << "You fire your stim pistol.\r\n";
		*target << player_obj->name() << " fires!\r\n";
		send_to_room_except(player_obj->room(),
			{player_obj->cd(),target->cd()},
			"%s fires a stim pistol at %s\r\n",
			player_obj->name().c_str(),
			target->name().c_str()
		);
		auto amount = scaled_heal(target); 
		target->hp() += amount;
		*target << "You gain " << amount << " hit points.\r\n";
		*/
	}
};
