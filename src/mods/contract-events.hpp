#ifndef __MENTOC_MODS_contract_events_HEADER__
#define __MENTOC_MODS_contract_events_HEADER__
#include "player.hpp"
#include <vector>
#include <string_view>
#include <iostream>

namespace mods::contract_events {
	void install_item(const uuid_t& player_uuid,const uuid_t& obj_uuid);
	void uninstall_item(const uuid_t& player_uuid,const uuid_t& obj_uuid);

	void perform_give(char_data* ch, char_data* target,obj_data* object);
	void perform_get(char_data* receiver,obj_data* object);

	void perform_give(player_ptr_t& giver, player_ptr_t& receiver,obj_ptr_t& object);
	void perform_get(player_ptr_t& receiver,obj_ptr_t& object);
	void perform_get(char_data* ch,obj_data* object);
	void find_mob(player_ptr_t& player,player_ptr_t& mob);
	void find_room(player_ptr_t& player,const room_rnum& room_id);
	void find_item(player_ptr_t& player,obj_ptr_t& object);
	void gain_entry_to_room(player_ptr_t& player,const room_rnum& room_id);
	void find_door(player_ptr_t& player,const room_rnum& room_id,const direction_t& dir);
};

#endif
