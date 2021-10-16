#ifndef __MENTOC_RANGED_COMBAT_TOTALS__
#define __MENTOC_RANGED_COMBAT_TOTALS__
#include "elemental.hpp"
#include <vector>

namespace mods {
	/**
			 * 1 -> can hit same room
			 * 2 -> can hit ranged
			 * 4 -> can hit doors
			 * 8 -> can hit objects
			 * 16 -> can hit cars
			 */
	enum viable_targets_t : uint8_t {
		VIABLE_NONE = 1,
		CAN_HIT_SAME_ROOM = (1 << 1),
		CAN_HIT_RANGED = (1 << 2),
		CAN_HIT_DOORS = (1 << 3),
		CAN_HIT_OBJECTS = (1 << 4),
		CAN_HIT_CARS = (1 << 5),
	};
	/*
		 * 1 -> has night vision
		 * 2 -> has thermals
		 * 4 -> has shadow sight
		 * 8 -> has morbid insight
		 */
	enum vision_t : uint8_t {
		VISION_NONE = 1,
		HAS_NIGHT_VISION = (1 << 0),
		HAS_THERMALS = (1 << 1),
		HAS_SHADOW_SIGHT = (1 << 2),
		HAS_MORBID_INSIGHT = (1 << 3),
	};

	struct ranged_combat_totals {
		float zoom_magnification;
		int max_range;
		int critical_chance;
		int chance_to_injure;
		float zoom_multiplier;
		int aimed_limb_accuracy_percent;
		int accuracy;

		std::pair<int,int> critical_range;
		std::pair<int,int> effective_range;
		std::vector<uuid_t> tracked;
		std::vector<uuid_t> marked;
		/**
		 * VIABLE TARGETS FLAGS
		 * ----------------------------
		 * bits:
		 * 1 -> can hit same room
		 * 2 -> can hit ranged
		 * 4 -> can hit doors
		 * 8 -> can hit objects
		 * 16 -> can hit cars
		 *
		 */
		viable_targets_t viable_targets;
		/**
		 * VISION FLAGS
		 * ----------------------------
		 * bits:
		 * 1 -> has night vision
		 * 2 -> has thermals
		 * 4 -> has shadow sight
		 * 8 -> has morbid insight
		 */
		vision_t vision;
		int recoil_reduction;
		int muzzle_velocity;

		int reload_time;
		int cooldown_between_shots;
		int ammo_max;
		int clip_size;

		int base_damage;
		int base_percent;
		int damage_dice_count;
		int damage_dice_sides;
		int armor_penetration;

		int damage_percent_bonus;
		int headshot_bonus;

		int hitroll;
		int damage_roll;

		int incendiary_percent;
		int explosive_percent;
		int shrapnel_percent;
		int corrosive_percent;
		int cryogenic_percent;
		int radioactive_percent;
		int emp_percent;
		int shock_percent;
		int anti_matter_percent;

		int incendiary_damage;
		int explosive_damage;
		int shrapnel_damage;
		int corrosive_damage;
		int cryogenic_damage;
		int radioactive_damage;
		int emp_damage;
		int shock_damage;
		int anti_matter_damage;

		int disorient_amount;

		int stat_strength;
		int stat_intelligence;
		int stat_wisdom;
		int stat_dexterity;
		int stat_constitution;
		int stat_electronics;
		int stat_armor;
		int stat_marksmanship;
		int stat_sniping;
		int stat_demolitions;
		int stat_chemistry;
		int stat_weapon_handling;
		int stat_strategy;
		int stat_medical;

		std::vector<std::pair<uint8_t,mods::elemental_types_t>> elemental_chances;
		ranged_combat_totals() :
			zoom_magnification(0),
			max_range(0),
			critical_chance(0),
			chance_to_injure(0),
			zoom_multiplier(0),
			aimed_limb_accuracy_percent(0),
			accuracy(0),
			critical_range(std::make_pair<>(0,0)),
			effective_range(std::make_pair<>(0,0)),
			viable_targets((viable_targets_t)0),
			vision((vision_t)0),
			recoil_reduction(0),
			muzzle_velocity(0),
			reload_time(0),
			cooldown_between_shots(0),
			ammo_max(0),
			clip_size(0),
			base_damage(0),
			base_percent(0),
			damage_dice_count(0),
			damage_dice_sides(0),
			armor_penetration(0),
			damage_percent_bonus(0),
			headshot_bonus(0),
			hitroll(0),
			damage_roll(0),

			incendiary_percent(0),
			explosive_percent(0),
			shrapnel_percent(0),
			corrosive_percent(0),
			cryogenic_percent(0),
			radioactive_percent(0),
			emp_percent(0),
			shock_percent(0),
			anti_matter_percent(0),

			incendiary_damage(0),
			explosive_damage(0),
			shrapnel_damage(0),
			corrosive_damage(0),
			cryogenic_damage(0),
			radioactive_damage(0),
			emp_damage(0),
			shock_damage(0),
			anti_matter_damage(0),

			disorient_amount(0),

			stat_strength(0),
			stat_intelligence(0),
			stat_wisdom(0),
			stat_dexterity(0),
			stat_constitution(0),
			stat_electronics(0),
			stat_armor(0),
			stat_marksmanship(0),
			stat_sniping(0),
			stat_demolitions(0),
			stat_chemistry(0),
			stat_weapon_handling(0),
			stat_strategy(0),
			stat_medical(0) {
			tracked.clear();
			marked.clear();
		}
		~ranged_combat_totals() = default;
	};
};//end namespace mods

#endif
