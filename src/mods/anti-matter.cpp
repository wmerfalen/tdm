#include "incendiary.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_CORROSIVE_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::incendiary::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


namespace mods::incendiary {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::incendiary::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int proton_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * anti-matter (proton) damage against a player has the following effects:
		 * - disable equipment permanently
		 * - headshot causes explosion that damages surrounding players
		 * - anti-matter damage penetrates through multiple rooms of enemies
		 * -
		 */
		return 0;
	}
	void proton_electronics(room_rnum room) {
		return;
	}
	void roll_proton_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_proton_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void proton_clears_up(player_ptr_t victim) {
		return;
	}
	void proton_clears_up(room_rnum room) {
		return;
	}
	void propagate_proton_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
#undef m_error
};
