#include "explosive.hpp"
#include "glass.hpp"
#include "affects.hpp"
#include "skills.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"

#define __MENTOC_SHOW_MODS_EXPLOSIVE_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_EXPLOSIVE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::explosive::explode_in_future]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::explosive::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::explosive {
	using affect_vector_t = mods::affects::affect_vector_t;
	using affect_t = mods::affects::affect_t;
	using texture_type_t = room_data::texture_type_t;

	int burn_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item) {
		/**
		 * explosive damage against a player has the following effects:
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
	void deploy_volatile_explosion(const room_rnum& room_id) {
		mods::rooms::start_fire_dissolver(room_id);
		if(world[room_id].has_texture(room_data::texture_type_t::GLASS_WINDOWS) ||
		        world[room_id].has_texture(room_data::texture_type_t::GLASS_CONTENTS)) {
			mods::glass::scatter_room(room_id);
		}
	}
	static std::map<room_rnum,int32_t> volatile_damage;
	void append_explosive_damage_to_room(const room_rnum& room_id, int32_t damage) {
		if(!world[room_id].has_texture(room_data::texture_type_t::VOLATILE)) {
			return;
		}
		/** room has volatile. if damage is greater than threshold, light it on fire */
		volatile_damage[room_id] += damage;
		if(volatile_damage[room_id] >= EXPLOSIVE_VOLATILE_ROOM_DAMAGE_THRESHOLD()) {
			m_debug("starting room on fire as it exceeds *explosive* damage threshold");
			volatile_damage[room_id] /= 2;
			deploy_volatile_explosion(room_id);
		}
	}
	void append_incendiary_damage_to_room(const room_rnum& room_id, int32_t damage) {
		if(!world[room_id].has_texture(room_data::texture_type_t::VOLATILE)) {
			return;
		}
		/** room has volatile. if damage is greater than threshold, light it on fire */
		volatile_damage[room_id] += damage;
		if(volatile_damage[room_id] >= INCENDIARY_VOLATILE_ROOM_DAMAGE_THRESHOLD()) {
			m_debug("starting room on fire as it exceeds *incendiary* damage threshold");
			volatile_damage[room_id] /= 2;
			deploy_volatile_explosion(room_id);
		}
	}
	void burn_electronics(room_rnum room) {
		return;
	}
	void roll_burn_equipment_in_room(room_rnum room) {
		return;
	}
	void roll_burn_equipment_on_player(player_ptr_t victim) {
		return;
	}
	void burning_clears_up(player_ptr_t victim) {
		return;
	}
	void burning_clears_up(room_rnum room) {
		return;
	}
	void propagate_burning_blast(room_rnum& room_id,obj_ptr_t device) {
		return;
	}
#undef m_error
#undef m_debug
};
