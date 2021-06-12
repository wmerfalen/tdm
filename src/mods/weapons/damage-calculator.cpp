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
#include <math.h>

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

	namespace algorithm_A {
		float get_base_damage(player_ptr_t& attacker) {
			static float base_damage[] = {
				10.0,	/* level 0 */
				10.0,	/* level 1 */
				15.0, /* level 2 */
				18.0, /* level 3 */
				20.0, /* level 4 */
				24.0, /* level 5 */
				26.0, /* level 6 */
				28.0, /* level 7 */
				30.0, /* level 9 */
				32.0, /* level 9 */
				34.0, /* level 10 */
				36.0, /* level 11 */
				38.0, /* level 12 */
				40.0, /* level 13 */
				42.0, /* level 14 */
				45.0, /* level 15 */
				50.0, /* level 16 */
				54.0, /* level 17 */
				60.0, /* level 18 */
				68.0, /* level 19 */
				80.0, /* level 20 */
				100.0, /* level 21 */
				115.0, /* level 22 */
				130.0, /* level 23 */
				150.0, /* level 24 */
				170.0, /* level 25 */
				190.0, /* level 26 */
				200.0, /* level 27 */
				250.0, /* level 28 */
				280.0, /* level 29 */
				300.0, /* level 30 */
			};
			std::size_t level = attacker->level();
			if(level >= 30) {
				level = 30;
			}
			return base_damage[level];
		}
		float get_level_damage(
		    player_ptr_t& attacker,
		    player_ptr_t& victim
		) {
			std::size_t level_difference = attacker->level() - victim->level();
			if(level_difference >= 6) {
				return 2;
			}
			switch(level_difference) {
				case 5:
					return 1.6;
				case 4:
					return 1.4;
				case 3:
					return 1.3;
				case 2:
					return 1.2;
				case 1:
					return 1.1;
				case 0:
					return 1.0;
				case -1:
					return 0.8;
				case -2:
					return 0.6;
				case -3:
					return 0.4;
				case -4:
					return 0.2;
				case -5:
					return 0.1;
				default:
					return 0.05;
			}
		}
		int16_t roll_weapon(
		    obj_ptr_t& weapon
		) {
			int16_t dice_count = 0, sides = 0;
			if(weapon->has_rifle() == false) {
				if(weapon->has_melee()) {
					dice_count = weapon->melee()->attributes->damage_dice_count;
					sides = weapon->melee()->attributes->damage_dice_sides;
					dty_debug("Weapon has melee obj type, so rolling for that:" << dice_count << "d" << sides);
					return dice(dice_count,sides);
				}
				dty_debug("Warning: rolling default 1d20 because player doesn't have rifle or melee extended obj types");
				return dice(1,20);
			} else {
				dty_debug("Doing roll for rifle attachment");
				auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
				if(rifle_attachment) {
					dice_count = rifle_attachment->base_object->rifle()->attributes->damage_dice_count *
					             (rifle_attachment->get_level() * 0.3);
					sides = rifle_attachment->base_object->rifle()->attributes->damage_dice_sides *
					        (rifle_attachment->get_level() * 0.3);
				} else {
					dice_count = weapon->rifle()->attributes->damage_dice_count;
					sides = weapon->rifle()->attributes->damage_dice_sides;
				}
				return dice(dice_count,sides);
			}
		}
		int16_t get_weapon_damage(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon
		) {
			return roll_weapon(weapon);
		}
		int16_t calculate(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim) {
			auto base_damage = get_base_damage(attacker);
			auto level_scaler = get_level_damage(attacker,victim);
			auto weapon_damage = get_weapon_damage(attacker,weapon);
			return base_damage + (level_scaler * weapon_damage);
		}
	};

	/**
	 * affects how far away a target can be.
	 * powered by the zoom_multiplier on attachments
	 */
	int16_t max_range(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	) {
		if(weapon && weapon->has_rifle()) {
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(rifle_attachment) {
				return rifle_attachment->zoom_multiplier * weapon->rifle()->attributes->max_range;
			}
			return weapon->rifle()->attributes->max_range;
		} else {
			return 0;
		}
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
		if(weapon->has_rifle() == false) {
			return 0;
		}
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
		if(weapon->has_rifle() == false) {
			return 0;/** TODO: make sure this doesnt screw something up */
		}
		int16_t ammo_reduction = wants_to_deduct;
		auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
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
		if(weapon->has_rifle() == false) {
			/** TODO: reduce from melee weapon */
			return 0; /** FIXME */
		}
		auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
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
		if(weapon && weapon->has_rifle()) {
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(rifle_attachment) {
				auto multiply = 0.01;
				if(rifle_attachment->armor_penetration_amount > 0) {
					/** TODO: honor this in a better way. amount should probably scale */
					multiply += 0.001;
				}
				return (rifle_attachment->damage_percent_bonus * multiply * requested_damage) + requested_damage;
			}
		} else if(weapon && weapon->has_melee()) {
			/** TODO fill me */
		}
		return requested_damage;

	}
	bool attack_disorients(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	) {
		if(!weapon) {
			return false;
		}
		if(weapon->has_rifle()) {
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(rifle_attachment && mods::rand::chance(rifle_attachment->disorient_amount)) {
				/** TODO: calculate disorient resistance of victim */
				dty_debug("Attack disorients player!");
				return true;
			}
			dty_debug("Attack fails to disorient player");
			return false;
		} else if(weapon->has_melee()) {
			/** TODO */
			if(mods::rand::chance(weapon->melee()->attributes->disorient_amount)) {
				/** TODO: calculate disorient resistance of victim */
				dty_debug("Attack disorients player!");
				return true;
			}
		}
		return false;
	}
	int16_t disorient_ticks(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	) {
		return mods::values::DEFAULT_DISORIENT_TICKS_FROM_RIFLE_ATTACK();
	}

	int16_t calculate(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	) {
		return algorithm_A::calculate(attacker,weapon,victim);
	}
	int calculate_strength_bonus(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	) {
		float multiplier = BONUS_STRENGTH_MULTIPLIER();
		switch(mods::levels::player_tier(player)) {
			default:
			case 1:
				multiplier += 0.10;
				break;
			case 2:
				multiplier += 0.12;
				break;
			case 3:
				multiplier += 0.18;
				break;
		}
		return player->strength() * multiplier;
	}
	int calculate_constitution_resistance(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	) {
		float multiplier = CONSTITUTION_STRENGTH_DAMPENER_MULTIPLIER();
		switch(mods::levels::player_tier(victim)) {
			default:
			case 1:
				multiplier += 0.11;
				break;
			case 2:
				multiplier += 0.15;
				break;
			case 3:
				multiplier += 0.19;
				break;
		}
		return player->constitution() * multiplier;
	}
};

