#include "shrapnel.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_SHRAPNEL_EXPLODE_IN_FUTURE_DEBUG_OUTPUT__
#define eif_debug(MSG) mentoc_prefix_debug("[mods::shrapnel::explode_in_future]")  << MSG << "\n";
#else
#define eif_debug(MSG) ;;
#endif


namespace mods::shrapnel {
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::shrapnel::ERROR]"))  << MSG << "\n";
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int shrapnel_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		return 0;
	}
	void damage_electronics(room_rnum room) {
		return;
	}
	void roll_damage_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_damage_equipment_on_player(player_ptr_t victim) {
		return;
	}
#undef m_error
};
