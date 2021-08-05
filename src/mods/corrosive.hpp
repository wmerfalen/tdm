#ifndef __MENTOC_MODS_corrosive_HEADER__
#define __MENTOC_MODS_corrosive_HEADER__
#include "../globals.hpp"

namespace mods::corrosive {
	int corrode_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void disable_electronics(room_rnum room);
	void roll_melt_equipment_in_room(room_rnum room);
	void roll_melt_equipment_on_player(player_ptr_t victim);
	void corrode_clears_up(player_ptr_t victim);
	void corrode_clears_up(room_rnum room);
	void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
