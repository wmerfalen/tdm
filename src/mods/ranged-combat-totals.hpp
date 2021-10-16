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
		int16_t max_range;
		int16_t critical_chance;
		int16_t chance_to_injure;
		float zoom_multiplier;
		int16_t aimed_limb_accuracy_percent;
		int16_t accuracy;

		std::pair<int8_t,int8_t> critical_range;
		std::pair<int8_t,int8_t> effective_range;
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
		int16_t recoil_reduction;
		int16_t muzzle_velocity;

		int16_t reload_time;
		int16_t cooldown_between_shots;
		int16_t ammo_max;
		int16_t clip_size;

		int16_t base_damage;
		int16_t base_percent;
		int16_t damage_dice_count;
		int16_t damage_dice_sides;
		int16_t armor_penetration;

		int16_t damage_percent_bonus;
		int16_t headshot_bonus;

		int16_t hitroll;
		int16_t damage_roll;

		int16_t incendiary_percent;
		int16_t explosive_percent;
		int16_t shrapnel_percent;
		int16_t corrosive_percent;
		int16_t cryogenic_percent;
		int16_t radioactive_percent;
		int16_t emp_percent;
		int16_t shock_percent;
		int16_t anti_matter_percent;

		int16_t disorient_amount;

		int16_t stat_strength;
		int16_t stat_intelligence;
		int16_t stat_wisdom;
		int16_t stat_dexterity;
		int16_t stat_constitution;
		int16_t stat_electronics;
		int16_t stat_armor;
		int16_t stat_marksmanship;
		int16_t stat_sniping;
		int16_t stat_demolitions;
		int16_t stat_chemistry;
		int16_t stat_weapon_handling;
		int16_t stat_strategy;
		int16_t stat_medical;

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
