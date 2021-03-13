#ifndef __MENTOC_MODS_contract_events_HEADER__
#define __MENTOC_MODS_contract_events_HEADER__
#include "player.hpp"
#include <vector>
#include <string_view>
#include <iostream>

namespace mods::contract_events {
	void player_found_item(player_ptr_t& player,uuid_t item_uuid);
	void player_found_mob(player_ptr_t& player,uuid_t mob_uuid);
	void player_found_room(player_ptr_t& player,room_rnum room_id);
	void player_found_door(player_ptr_t& player,room_rnum room_id, int8_t direction);
	void player_destroyed_item(player_ptr_t& player,uuid_t item_uuid);
	void player_destroyed_door(player_ptr_t& player,room_rnum room_id,int8_t direction);
	void player_retrieved_item(player_ptr_t& player,uuid_t item_uuid);
	void player_quota_item_find_increase(player_ptr_t& player,uuid_t item_uuid);
	void player_quota_mob_kill_increase(player_ptr_t& player,uuid_t mob_uuid);
	void player_quota_door_destroyed_increase(player_ptr_t& player,room_rnum room_id,int8_t direction);
	void player_kill_mob(player_ptr_t& player,uuid_t m_vnum);
	void player_gain_entry(player_ptr_t& player,room_rnum room_id);
	void player_talk_to(player_ptr_t& player,uuid_t mob_uuid);
	void player_install_item(player_ptr_t& player,uuid_t item_uuid);
};

#endif
