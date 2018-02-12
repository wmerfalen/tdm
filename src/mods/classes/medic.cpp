#include "medic.hpp"

namespace mods::classes {
	void medic::heal_player(std::shared_ptr<mods::player> & target){
		if(target->cd()->in_room != player_obj->cd()->in_room){
			*player_obj << "You can't find your target.\r\n";
			return;
		}
		if(stim_pistol_ammo == 0){
			*player_obj << "Out of ammo.\r\n";
			return;
		}
		if(target->cd() == player_obj->cd()){
			const char* self_name = (player_obj->cd().player.sex == MALE ? "himself" : "herself" ); 
			const char* short_self_name = (player_obj->cd().player.sex == MALE ? "his" : "her"); 
			*player_obj << "You aim your stim pistol at yourself...\r\n";
			send_to_room_except(IN_ROOM(player_obj->cd()),
					{player_obj->cd()},
					"%s aims a stim pistol at %s\r\n",
					player_obj->player.name,self_name);
			*player_obj << "You fire your stim pistol.\r\n";
			send_to_room_except(IN_ROOM(player_obj->cd()),
					{player_obj->cd()},
					"%s fires a stim pistol into %s arm\r\n",
					player_obj->player.name,short_self_name);
			//TODO: scale this
			player_obj->cd()->points.hit += 30;
			*player_obj << "You gain 30 hit points.\r\n";
			return;
		}
		*player_obj << "You aim your stim pistol at " << 
			target->cd()->player.name << " carefully...\r\n";
		const char* gender = (player_obj->cd().player.sex == MALE ? "his" : "her" ); 
		send_to_room_except(IN_ROOM(player_obj->cd()),
				{player_obj->cd()},
				"%s aims a stim pistol at %s\r\n",
					player_obj->player.name,
					target->player.name);
		*target << player_obj->cd()->player.name << " aims " << gender << 
			" stim pistol at you...\r\n";
		*player_obj << "You fire your stim pistol.\r\n";
		*target << player_obj->cd()->player.name << " fires!\r\n";
		send_to_room_except(IN_ROOM(player_obj->cd()),
				{player_obj->cd()},
				"%s fires a stim pistol at %s\r\n",
				player_obj->player.name,
				target->player.name);
		//TODO: scale this
		target->cd()->points.hit += 30;
		*target << "You gain 30 hit points.\r\n";
	}
};

#endif
