#ifndef __MENTOC_MODS_shrapnel_HEADER__
#define __MENTOC_MODS_shrapnel_HEADER__
#include "../globals.hpp"

namespace mods::shrapnel {
	int shrapnel_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void damage_electronics(room_rnum room);
	void roll_damage_equipment_in_room(room_rnum room);
	void roll_damage_equipment_on_player(player_ptr_t victim);
	bool deploy_shrapnel_damage_to(player_ptr_t& victim,obj_ptr_t& device);
};
#endif
