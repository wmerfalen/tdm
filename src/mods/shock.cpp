#include "shock.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_SHOCK_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::shock::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


namespace mods::shock {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::shock::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int shock_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * shock damage against a player has the following effects:
		 * - causes room inhabitants to want to flee in the opposite direction of attack
		 * - causes target to remove armor
		 * - causes room to fill with smoke which causes blindness
		 * - can trigger claymores to explode prematurely
		 * - causes to scream in agony and cause nearby enemies to FEAR
		 * 		- fear causes:
		 * 			- 15% damage nerf
		 * 			- 25% chance to not attack due to being frozen by fear
		 * 			- 50% chance of not moving if trying to flee
		 */
		return 0;
	}
	void shock_electronics(room_rnum room) {
		return;
	}
	void roll_shock_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_shock_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void shocking_clears_up(player_ptr_t victim) {
		return;
	}
	void shocking_clears_up(room_rnum room) {
		return;
	}
	void propagate_shocking_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
#undef m_error
};
