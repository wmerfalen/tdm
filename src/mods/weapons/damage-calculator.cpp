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
	static bool use_static_roll_weapon_amount = false;
	static int16_t static_roll_weapon_amount = 0;
	void set_static_roll_weapon_amount(int16_t amount) {
		use_static_roll_weapon_amount = true;
		static_roll_weapon_amount = amount;
	}
	void clear_static_roll_weapon_amount() {
		use_static_roll_weapon_amount = false;
	}

	namespace algorithm_A {
		float get_base_damage(player_ptr_t& attacker) {
			static float base_damage[] = {
				LEVEL_0_BASE_DAMAGE(),
				LEVEL_1_BASE_DAMAGE(),
				LEVEL_2_BASE_DAMAGE(),
				LEVEL_3_BASE_DAMAGE(),
				LEVEL_4_BASE_DAMAGE(),
				LEVEL_5_BASE_DAMAGE(),
				LEVEL_6_BASE_DAMAGE(),
				LEVEL_7_BASE_DAMAGE(),
				LEVEL_8_BASE_DAMAGE(),
				LEVEL_9_BASE_DAMAGE(),
				LEVEL_10_BASE_DAMAGE(),
				LEVEL_11_BASE_DAMAGE(),
				LEVEL_12_BASE_DAMAGE(),
				LEVEL_13_BASE_DAMAGE(),
				LEVEL_14_BASE_DAMAGE(),
				LEVEL_15_BASE_DAMAGE(),
				LEVEL_16_BASE_DAMAGE(),
				LEVEL_17_BASE_DAMAGE(),
				LEVEL_18_BASE_DAMAGE(),
				LEVEL_19_BASE_DAMAGE(),
				LEVEL_20_BASE_DAMAGE(),
				LEVEL_21_BASE_DAMAGE(),
				LEVEL_22_BASE_DAMAGE(),
				LEVEL_23_BASE_DAMAGE(),
				LEVEL_24_BASE_DAMAGE(),
				LEVEL_25_BASE_DAMAGE(),
				LEVEL_26_BASE_DAMAGE(),
				LEVEL_27_BASE_DAMAGE(),
				LEVEL_28_BASE_DAMAGE(),
				LEVEL_29_BASE_DAMAGE(),
				LEVEL_30_BASE_DAMAGE()
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
				return LEVEL_DIFF_GREATER_THAN_SIX();
			}
			switch(level_difference) {
				case 5:
					return LEVEL_DIFF_5();
				case 4:
					return LEVEL_DIFF_4();
				case 3:
					return LEVEL_DIFF_3();
				case 2:
					return LEVEL_DIFF_2();
				case 1:
					return LEVEL_DIFF_1();
				case 0:
					return LEVEL_DIFF_0();
				case -1:
					return LEVEL_DIFF_NEG_1();
				case -2:
					return LEVEL_DIFF_NEG_2();
				case -3:
					return LEVEL_DIFF_NEG_3();
				case -4:
					return LEVEL_DIFF_NEG_4();
				case -5:
					return LEVEL_DIFF_NEG_5();
				default:
					return LEVEL_DIFF_DEFAULT();
			}
		}
		int16_t roll_weapon(
		    obj_ptr_t& weapon
		) {
			if(mods::weapons::damage_calculator::use_static_roll_weapon_amount) {
				return mods::weapons::damage_calculator::static_roll_weapon_amount;
			}
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
					             (rifle_attachment->get_level() * RIFLE_ATTACHMENT_LEVEL_MULTIPLIER());
					sides = rifle_attachment->base_object->rifle()->attributes->damage_dice_sides *
					        (rifle_attachment->get_level() * RIFLE_ATTACHMENT_LEVEL_MULTIPLIER());
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
			if(attacker->marine() && mods::object_utils::is_assault_rifle(weapon)) {
				int additional_damage = (MARINE_AR_PASSIVE_BONUS_DAMAGE() * 0.01) * weapon_damage;
				attacker->sendln(CAT("{grn}[MARINE PASSIVE BONUS DAMAGE]: ",additional_damage,"{/grn}"));
				weapon_damage += additional_damage;
			}
			float damage_nerf = victim->get_damage_nerf();
			auto total_damage = base_damage + (level_scaler * weapon_damage);
			if(total_damage > 0 && damage_nerf > 0) {
				total_damage -= (total_damage * 0.01 * damage_nerf);
				return total_damage;
			}
			return total_damage;
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
			std::size_t additional_range = 0;
			if(attacker->marine() && mods::object_utils::is_assault_rifle(weapon)) {
				additional_range = MARINE_AR_PASSIVE_RANGE_BONUS();
			}
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(rifle_attachment) {
				return (rifle_attachment->zoom_multiplier * weapon->rifle()->attributes->max_range) + additional_range;
			}
			return (weapon->rifle()->attributes->max_range) + additional_range;
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
		auto ammo = weapon->rifle_instance->ammo;
		return ammo;
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
		if(mods::rand::chance(rifle_attachment->free_ammo_chance)) {
			dty_debug("Player rolled free ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->free_ammo_chance << "%");
			ammo_reduction = 0;
		}
		if(mods::rand::chance(rifle_attachment->regenerate_ammo_chance)) {
			dty_debug("Player rolled regenerated ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->regenerate_ammo_chance << "%");
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
				multiplier += TIER_ONE_STRENGTH_BONUS_MULTIPLIER();
				break;
			case 2:
				multiplier += TIER_TWO_STRENGTH_BONUS_MULTIPLIER();
				break;
			case 3:
				multiplier += TIER_THREE_STRENGTH_BONUS_MULTIPLIER();
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
				multiplier += TIER_ONE_CONSTITUTION_RESISTANCE_MULTIPLIER();
				break;
			case 2:
				multiplier += TIER_TWO_CONSTITUTION_RESISTANCE_MULTIPLIER();
				break;
			case 3:
				multiplier += TIER_THREE_CONSTITUTION_RESISTANCE_MULTIPLIER();
				break;
		}
		return player->constitution() * multiplier;
	}
	int calculate_sniper_extra_damage(player_ptr_t& attacker,obj_ptr_t& weapon,player_ptr_t& victim,int dam) {
		float multiplier = SNIPER_CLASS_EXPERT_BONUS_DAMAGE_PER_TIER() * 0.01;
		auto attacker_tier = tier(attacker);
		return (multiplier * attacker_tier) * dam;
	}
	explosive_damage_t calculate_explosive_damage(player_ptr_t victim, obj_ptr_t item) {
		explosive_damage_t e;
		if(mods::super_users::player_is(victim)) {
			return e;
		}

		auto& attr = item->explosive()->attributes;
		if(attr->alternate_explosion_type.compare("SCAN") == 0) {
			return e;
		}
		uint8_t chance = attr->chance_to_injure;
		uint8_t critical_chance = attr->critical_chance;
		if(mods::skills::player_can(victim,"INJURE_RESISTANCE")) {
			chance -= INJURE_RESISTANCE_SKILL_MODIFIER();
		}
		e.injured = mods::injure::do_injure_roll(chance);

		/** TODO handle critical range attribute */
		/** TODO handle blast radius attribute */
		/** TODO handle loudness type */
		e.damage = dice(attr->damage_dice_count,attr->damage_dice_sides);

		if(dice(1,100) <= critical_chance) {
			e.critical = e.damage * EXPLOSIVE_CRITICAL_MULTIPLIER();
		}

		if(attr->chemical_damage_dice_count) {
			e.chemical = dice(attr->chemical_damage_dice_count,attr->chemical_damage_dice_sides);
		}
		if(attr->incendiary_damage_dice_count) {
			e.fire = dice(attr->incendiary_damage_dice_count,attr->incendiary_damage_dice_sides);
		}
		if(attr->radiation_damage_dice_count) {
			e.radiation = dice(attr->radiation_damage_dice_count,attr->radiation_damage_dice_sides);
		}
		if(attr->electric_damage_dice_count) {
			e.electric = dice(attr->electric_damage_dice_count,attr->electric_damage_dice_sides);
		}
		if(attr->armor_penetration_damage_dice_count) {
			e.armor_pen = dice(attr->armor_penetration_damage_dice_count,attr->armor_penetration_damage_dice_sides);
		}

		return e;
	}
};

