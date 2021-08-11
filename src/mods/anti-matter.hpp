#ifndef __MENTOC_MODS_anti_matter_HEADER__
#define __MENTOC_MODS_anti_matter_HEADER__
#include "../globals.hpp"

namespace mods::anti_matter {
	int proton_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void proton_electronics(room_rnum room);
	void roll_proton_equipment_in_room(room_rnum room);
	void roll_proton_equipment_on_player(player_ptr_t victim);
	void proton_clears_up(player_ptr_t victim);
	void proton_clears_up(room_rnum room);
	void propagate_proton_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
