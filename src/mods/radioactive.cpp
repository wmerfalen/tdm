#include "radioactive.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_RADIOACTIVE_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::radioactive::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


namespace mods::radioactive {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::radioactive::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int radiation_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * radioactive damage against a player has the following effects:
		 * - 15% slower hp regen
		 * - target becomes disoriented for 15 seconds
		 * - limbs fall off
		 * - vomit blood (10-25% hp drain over time)
		 * - deformities sprout on body and combust causing 25% hp/mana/move drain
		 */
		return 0;
	}
	void radiate_electronics(room_rnum room) {
		return;
	}
	void roll_radiate_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_radiate_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void radiation_clears_up(player_ptr_t victim) {
		return;
	}
	void radiation_clears_up(room_rnum room) {
		return;
	}
	void propagate_radiation_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
#undef m_error
};
