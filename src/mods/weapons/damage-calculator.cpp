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

	/**
	 * affects how far away a target can be.
	 * powered by the zoom_multiplier on attachments
	 */
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
	/**
	 * @brief calculates how big your magazine is when you reload
	 *
	 * @param attacker
	 * @param
	 *
	 * @return
	 */
	int16_t ammunition_amount(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		/** TODO: add ammunition count from rifle_attachment */
		return weapon->rifle_instance->ammo;
	}
	/**
	 * @brief pass in attacker, weapon, and amount you would like to reduce ammo by. this function will roll your free ammo chance and regenerate ammo chance. it will take the result of those rolls and give you the amount of ammo you should really deduct. may return negative number or zero when regenerated ammo or free ammo dice rolls succeed.
	 *
	 * @param attacker
	 * @param weapon
	 * @param
	 *
	 * @return
	 */
	int16_t reduce_ammo(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    int16_t wants_to_deduct
	) {
		int16_t ammo_reduction = wants_to_deduct;
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(!rifle_attachment) {
			return wants_to_deduct;
		}
		if(auto free_ammo = mods::rand::chance(rifle_attachment->free_ammo_chance)) {
			dty_debug("Player rolled free ammo " << (free_ammo ?  green_str("SUCCESS") : red_str("FAIL")) <<
			          "Chances were: " << rifle_attachment->free_ammo_chance << "%");
			ammo_reduction = 0;
		}
		if(auto regenerated_ammo = mods::rand::chance(rifle_attachment->regenerate_ammo_chance)) {
			dty_debug("Player rolled regenerated ammo " << (regenerated_ammo ?  green_str("SUCCESS") : red_str("FAIL")) <<
			          "Chances were: " << rifle_attachment->regenerate_ammo_chance << "%");
			int16_t regen = rand_number(
			                    mods::values::REGENERATED_AMMO_LOW(),
			                    mods::values::REGENERATED_AMMO_HIGH()
			                );
			dty_debug("Random amount of regenerated ammo: " << regen);
			return ammo_reduction - regen;
		}
		return ammo_reduction;
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
			auto multiply = 0.01;
			if(rifle_attachment->armor_penetration_amount > 0) {
				/** TODO: honor this in a better way. amount should probably scale */
				multiply += 0.001;
			}
			return (rifle_attachment->damage_percent_bonus * multiply * requested_damage) + requested_damage;
		}
		return requested_damage;
	}
	bool attack_disorients(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	) {
		auto rifle_attachment = attacker->rifle_attachment_by_uuid(weapon->uuid);
		if(rifle_attachment && mods::rand::chance(rifle_attachment->disorient_amount)) {
			/** TODO: calculate disorient resistance of victim */
			dty_debug("Attack disorients player!");
			return true;
		}
		dty_debug("Attack fails to disorient player");
		return false;
	}
	int16_t disorient_ticks(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	) {
		return mods::values::DEFAULT_DISORIENT_TICKS_FROM_RIFLE_ATTACK();
	}

};

