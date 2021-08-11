#ifndef __MENTOC_MODS_radioactive_HEADER__
#define __MENTOC_MODS_radioactive_HEADER__
#include "../globals.hpp"

namespace mods::radioactive {
	int radiation_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void radiate_electronics(room_rnum room);
	void roll_radiate_equipment_in_room(room_rnum room);
	void roll_radiate_equipment_on_player(player_ptr_t victim);
	void radiation_clears_up(player_ptr_t victim);
	void radiation_clears_up(room_rnum room);
	void propagate_radiation_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
