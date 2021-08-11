#ifndef __MENTOC_MODS_incendiary_HEADER__
#define __MENTOC_MODS_incendiary_HEADER__
#include "../globals.hpp"

namespace mods::incendiary {
	int burn_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void burn_electronics(room_rnum room);
	void roll_burn_equipment_in_room(room_rnum room);
	void roll_burn_equipment_on_player(player_ptr_t victim);
	void burning_clears_up(player_ptr_t victim);
	void burning_clears_up(room_rnum room);
	void propagate_burning_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
