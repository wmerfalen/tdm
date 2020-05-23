#ifndef __MENTOC_MODS_SENSOR_GRENADE_HEADER__
#define __MENTOC_MODS_SENSOR_GRENADE_HEADER__

#include <iostream>
#include "yaml.hpp"
#include "item-types.hpp"
#include "../globals.hpp"

namespace mods::sensor_grenade {
	//void affect_player(obj_ptr_t& nade, player_ptr_t& target);
	bool can_see_player(uuid_t nade_uuid,player_ptr_t& target);
	void gather_room(uuid_t nade_uuid,room_rnum& room,uint8_t from_direction);
	void send_results(uuid_t nade_uuid,uuid_t player_uuid);
	void send_results_multi(uuid_t nade_uuid,const std::vector<uuid_t>& player_uuid);
	void consume(uuid_t nade_uuid);
	void handle_explosion(uuid_t nade_uuid, uuid_t player_uuid,room_rnum room,int from_direction);
};
#endif

