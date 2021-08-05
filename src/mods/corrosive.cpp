#include "corrosive.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_CORROSIVE_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::corrosive::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


namespace mods::corrosive {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::corrosive::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int corrode_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		return 0;
	}
	void disable_electronics(room_rnum room) {
		return;
	}
	void roll_melt_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_melt_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void corrode_clears_up(player_ptr_t victim) {
		return;
	}
	void corrode_clears_up(room_rnum room) {
		return;
	}
	void propagate_chemical_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
#undef m_error
};
