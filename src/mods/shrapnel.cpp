#include "shrapnel.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "rand.hpp"

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

	static constexpr uint8_t SHRAPNEL_HITS_MINIMUM = 3;
	static constexpr uint8_t SHRAPNEL_HITS_MAXIMUM = 13;

	bool deploy_shrapnel_damage_to(player_ptr_t& victim,obj_ptr_t& device) {
		feedback_t f;
		f.from_direction = NORTH;
		f.hits = rand_number(SHRAPNEL_HITS_MINIMUM,SHRAPNEL_HITS_MAXIMUM);
		f.damage = roll(device->explosive());
		f.attacker = device->get_owner();
		f.damage_event = HIT_BY_SHRAPNEL_DAMAGE;
		victim->damage_event(f);

		update_pos(victim);

		f.damage_event = YOU_INFLICTED_SHRAPNEL_DAMAGE;
		auto attacker = ptr_by_uuid(f.attacker);
		if(attacker) {
			attacker->damage_event(f);
		}

		return victim->is_dead();
	}

	int shrapnel_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * Shrapnel has the one or more of the following effects on it's target:
		 * - can cause bleeding
		 *   - bleeding drains HP over a period of time
		 * - cause target to take N % more movement points to move
		 * - armor is less effective in absorbing subsequent attacks
		 * - chance to cause target to flee room
		 *
		 * */
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
