#ifndef __MENTOC_MODS_cryogenic_HEADER__
#define __MENTOC_MODS_cryogenic_HEADER__
#include "../globals.hpp"

namespace mods::cryogenic {
	int cryo_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item);
	void cryo_electronics(room_rnum room);
	void roll_cryo_equipment_in_room(room_rnum room);
	void roll_cryo_equipment_on_player(player_ptr_t victim);
	void cryo_clears_up(player_ptr_t victim);
	void cryo_clears_up(room_rnum room);
	void propagate_cryo_blast(room_rnum& room_id,obj_ptr_t device);
};
#endif
