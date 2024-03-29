#ifndef __MENTOC_MODS_CALC_VISIBILITY_HEADER__
#define  __MENTOC_MODS_CALC_VISIBILITY_HEADER__

#include <iostream>
#include "../globals.hpp"
#include "damage-event.hpp"

namespace mods::calc_visibility {
	bool is_visible(uuid_t observer,uuid_t target,uint8_t distance);
	bool is_visible(player_ptr_t& observer,player_ptr_t& target,uint8_t distance);
	std::tuple<bool,std::string> can_see_object(player_ptr_t& attacker,obj_ptr_t& object);
	bool can_see_room(player_ptr_t&);
	bool roll_victim_spots_attacker(player_ptr_t& victim,player_ptr_t& attacker,const feedback_t& feedback);
	//bool player_can_see_obj(player_ptr_t& player,obj_ptr_t & obj);
	//bool obj_can_see_player(obj_ptr_t& obj,player_ptr_t& target);
	//bool sense_player(obj_ptr_t& device, player_ptr_t& target);
};

#endif
