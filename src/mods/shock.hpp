#ifndef __MENTOC_MODS_shock_HEADER__
#define __MENTOC_MODS_shock_HEADER__
#include "../globals.hpp"

namespace mods::shock {
	int shock_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void shock_electronics(room_rnum room);
	void roll_shock_equipment_in_room(room_rnum room);
	void roll_shock_equipment_on_player(player_ptr_t victim);
	void shocking_clears_up(player_ptr_t victim);
	void shocking_clears_up(room_rnum room);
	void propagate_shocking_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
