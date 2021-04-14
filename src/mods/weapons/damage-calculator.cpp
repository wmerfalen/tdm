#include "damage-calculator.hpp"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "elemental.hpp"
#include "../rifle-attachments.hpp"
#include "../rand.hpp"

#define __MENTOC_SHOW_DAMAGE_CALCULATOR_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_DAMAGE_CALCULATOR_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::weapons::damage_calculator][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::weapons::damage_calculator {
	using de = damage_event_t;
	using vpd = mods::scan::vec_player_data;

	int16_t max_range(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment) {
			return rifle_attachment->zoom_multiplier * weapon->rifle()->attributes->max_range;
		}
		return weapon->rifle()->attributes->max_range;
	}
	int16_t ammunition_amount(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		/** TODO: add ammunition count from rifle_attachment */
		return weapon->rifle_instance->ammo;
	}
	bool roll_free_ammo_chance(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment) {
			bool free_ammo = mods::rand::chance(rifle_attachment->free_ammo_chance);
			dty_debug("Player rolled free ammo " << (free_ammo ?  green_str("SUCCESS") : red_str("FAIL")) <<
			          "Chances were: " << rifle_attachment->free_ammo_chance << "%");
			return free_ammo;
		}
		dty_debug("No free ammo because weapon isnt rifle attachment");
		return false;
	}
	int16_t regenerate_ammo(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment) {
			bool regenerated_ammo = mods::rand::chance(rifle_attachment->regenerate_ammo_chance);
			dty_debug("Player rolled regenerated ammo " << (regenerated_ammo ?  green_str("SUCCESS") : red_str("FAIL")) <<
			          "Chances were: " << rifle_attachment->regenerate_ammo_chance << "%");
			int16_t ammo_count = rand_number(
			                         mods::values::REGENERATED_AMMO_LOW(),
			                         mods::values::REGENERATED_AMMO_HIGH()
			                     );
			dty_debug("Random amount of regenerated ammo: " << ammo_count);
			return ammo_count;
		}
		dty_debug("No regenerated ammo because weapon isnt rifle attachment");
		return 0;
	}
	int16_t reduce_ammo(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    int16_t wants_to_deduct
	) {
		int16_t ammo_reduction = wants_to_deduct;
		if(roll_free_ammo_chance(attacker,weapon)) {
			ammo_reduction = 0;
		}
		auto regen = regenerate_ammo(attacker,weapon);
		return ammo_reduction - regen;
	}
	int16_t reduce_durability_points(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment) {
			/** TODO: honor durability points */
		}
		return 0;/** FIXME */
	};
	int16_t calculate_bonus_damage(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    int16_t requested_damage
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment) {
			return (rifle_attachment->damage_percent_bonus * 0.10 * requested_damage) + requested_damage;
		}
		return requested_damage;
	}

};

