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
		ranged_combat_totals(const ranged_combat_totals& other) :
			zoom_magnification(other.zoom_magnification),
			max_range(other.max_range),
			critical_chance(other.critical_chance),
			chance_to_injure(other.chance_to_injure),
			zoom_multiplier(other.zoom_multiplier),
			aimed_limb_accuracy_percent(other.aimed_limb_accuracy_percent),
			accuracy(other.accuracy),
			critical_range(std::make_pair<>(0,0)),
			effective_range(std::make_pair<>(0,0)),
			viable_targets((viable_targets_t)0),
			vision((vision_t)0),
			recoil_reduction(other.recoil_reduction),
			muzzle_velocity(other.muzzle_velocity),
			reload_time(other.reload_time),
			cooldown_between_shots(other.cooldown_between_shots),
			ammo_max(other.ammo_max),
			clip_size(other.clip_size),
			base_damage(other.base_damage),
			base_percent(other.base_percent),
			damage_dice_count(other.damage_dice_count),
			damage_dice_sides(other.damage_dice_sides),
			armor_penetration(other.armor_penetration),
			damage_percent_bonus(other.damage_percent_bonus),
			headshot_bonus(other.headshot_bonus),
			hitroll(other.hitroll),
			damage_roll(other.damage_roll),

			incendiary_percent(other.incendiary_percent),
			explosive_percent(other.explosive_percent),
			shrapnel_percent(other.shrapnel_percent),
			corrosive_percent(other.corrosive_percent),
			cryogenic_percent(other.cryogenic_percent),
			radioactive_percent(other.radioactive_percent),
			emp_percent(other.emp_percent),
			shock_percent(other.shock_percent),
			anti_matter_percent(other.anti_matter_percent),

			incendiary_damage(other.incendiary_damage),
			explosive_damage(other.explosive_damage),
			shrapnel_damage(other.shrapnel_damage),
			corrosive_damage(other.corrosive_damage),
			cryogenic_damage(other.cryogenic_damage),
			radioactive_damage(other.radioactive_damage),
			emp_damage(other.emp_damage),
			shock_damage(other.shock_damage),
			anti_matter_damage(other.anti_matter_damage),

			disorient_amount(other.disorient_amount),

			stat_strength(other.stat_strength),
			stat_intelligence(other.stat_intelligence),
			stat_wisdom(other.stat_wisdom),
			stat_dexterity(other.stat_dexterity),
			stat_constitution(other.stat_constitution),
			stat_electronics(other.stat_electronics),
			stat_armor(other.stat_armor),
			stat_marksmanship(other.stat_marksmanship),
			stat_sniping(other.stat_sniping),
			stat_demolitions(other.stat_demolitions),
			stat_chemistry(other.stat_chemistry),
			stat_weapon_handling(other.stat_weapon_handling),
			stat_strategy(other.stat_strategy),
			stat_medical(other.stat_medical) {
			tracked = other.tracked;
			marked = other.marked;
		}
		ranged_combat_totals& operator=(ranged_combat_totals& other) {
			ranged_combat_totals();
			zoom_magnification = other.zoom_magnification;
			max_range = other.max_range;
			critical_chance = other.critical_chance;
			chance_to_injure = other.chance_to_injure;
			zoom_multiplier = other.zoom_multiplier;
			aimed_limb_accuracy_percent = other.aimed_limb_accuracy_percent;
			accuracy = other.accuracy;
			critical_range = other.critical_range;
			effective_range = other.effective_range;
			viable_targets = other.viable_targets;
			vision = other.vision;
			recoil_reduction = other.recoil_reduction;
			muzzle_velocity = other.muzzle_velocity;
			reload_time = other.reload_time;
			cooldown_between_shots = other.cooldown_between_shots;
			ammo_max = other.ammo_max;
			clip_size = other.clip_size;
			base_damage = other.base_damage;
			base_percent = other.base_percent;
			damage_dice_count = other.damage_dice_count;
			damage_dice_sides = other.damage_dice_sides;
			armor_penetration = other.armor_penetration;
			damage_percent_bonus = other.damage_percent_bonus;
			headshot_bonus = other.headshot_bonus;
			hitroll = other.hitroll;
			damage_roll = other.damage_roll;

			incendiary_percent = other.incendiary_percent;
			explosive_percent = other.explosive_percent;
			shrapnel_percent = other.shrapnel_percent;
			corrosive_percent = other.corrosive_percent;
			cryogenic_percent = other.cryogenic_percent;
			radioactive_percent = other.radioactive_percent;
			emp_percent = other.emp_percent;
			shock_percent = other.shock_percent;
			anti_matter_percent = other.anti_matter_percent;

			incendiary_damage = other.incendiary_damage;
			explosive_damage = other.explosive_damage;
			shrapnel_damage = other.shrapnel_damage;
			corrosive_damage = other.corrosive_damage;
			cryogenic_damage = other.cryogenic_damage;
			radioactive_damage = other.radioactive_damage;
			emp_damage = other.emp_damage;
			shock_damage = other.shock_damage;
			anti_matter_damage = other.anti_matter_damage;

			disorient_amount = other.disorient_amount;

			stat_strength = other.stat_strength;
			stat_intelligence = other.stat_intelligence;
			stat_wisdom = other.stat_wisdom;
			stat_dexterity = other.stat_dexterity;
			stat_constitution = other.stat_constitution;
			stat_electronics = other.stat_electronics;
			stat_armor = other.stat_armor;
			stat_marksmanship = other.stat_marksmanship;
			stat_sniping = other.stat_sniping;
			stat_demolitions = other.stat_demolitions;
			stat_chemistry = other.stat_chemistry;
			stat_weapon_handling = other.stat_weapon_handling;
			stat_strategy = other.stat_strategy;
			stat_medical = other.stat_medical;
			tracked = other.tracked;
			marked = other.marked;
			return *this;
		}
		~ranged_combat_totals() = default;
		template <typename T>
		void report(T p) {
			if(zoom_magnification > 0) {
				p->sendln(CAT("zoom_magnification:",zoom_magnification));
			}
			if(max_range > 0) {
				p->sendln(CAT("max_range:",max_range));
			}
			if(critical_chance > 0) {
				p->sendln(CAT("critical_chance:",critical_chance));
			}
			if(chance_to_injure > 0) {
				p->sendln(CAT("chance_to_injure:",chance_to_injure));
			}
			if(zoom_multiplier > 0) {
				p->sendln(CAT("zoom_multiplier:",zoom_multiplier));
			}
			if(aimed_limb_accuracy_percent > 0) {
				p->sendln(CAT("aimed_limb_accuracy_percent:",aimed_limb_accuracy_percent));
			}
			if(accuracy > 0) {
				p->sendln(CAT("accuracy:",accuracy));
			}
			if(critical_range.second > 0) {
				p->sendln(CAT("critical_range:",critical_range.first,"->",critical_range.second));
			}
			if(effective_range.second > 0) {
				p->sendln(CAT("effective_range:",effective_range.first,"->",effective_range.second));
			}
			if(viable_targets > 0) {
				p->sendln(CAT("viable_targets:",viable_targets));
			}
			if(vision > 0) {
				p->sendln(CAT("vision:",vision));
			}
			if(recoil_reduction > 0) {
				p->sendln(CAT("recoil_reduction:",recoil_reduction));
			}
			if(muzzle_velocity > 0) {
				p->sendln(CAT("muzzle_velocity:",muzzle_velocity));
			}
			if(reload_time > 0) {
				p->sendln(CAT("reload_time:",reload_time));
			}
			if(cooldown_between_shots > 0) {
				p->sendln(CAT("cooldown_between_shots:",cooldown_between_shots));
			}
			if(ammo_max > 0) {
				p->sendln(CAT("ammo_max:",ammo_max));
			}
			if(clip_size > 0) {
				p->sendln(CAT("clip_size:",clip_size));
			}
			if(base_damage > 0) {
				p->sendln(CAT("base_damage:",base_damage));
			}
			if(base_percent > 0) {
				p->sendln(CAT("base_percent:",base_percent));
			}
			if(damage_dice_count > 0) {
				p->sendln(CAT("damage_dice_count:",damage_dice_count));
			}
			if(damage_dice_sides > 0) {
				p->sendln(CAT("damage_dice_sides:",damage_dice_sides));
			}
			if(armor_penetration > 0) {
				p->sendln(CAT("armor_penetration:",armor_penetration));
			}
			if(damage_percent_bonus > 0) {
				p->sendln(CAT("damage_percent_bonus:",damage_percent_bonus));
			}
			if(headshot_bonus > 0) {
				p->sendln(CAT("headshot_bonus:",headshot_bonus));
			}
			if(hitroll > 0) {
				p->sendln(CAT("hitroll:",hitroll));
			}
			if(damage_roll > 0) {
				p->sendln(CAT("damage_roll:",damage_roll));
			}
			if(incendiary_percent > 0) {
				p->sendln(CAT("incendiary_percent:",incendiary_percent));
			}
			if(explosive_percent > 0) {
				p->sendln(CAT("explosive_percent:",explosive_percent));
			}
			if(shrapnel_percent > 0) {
				p->sendln(CAT("shrapnel_percent:",shrapnel_percent));
			}
			if(corrosive_percent > 0) {
				p->sendln(CAT("corrosive_percent:",corrosive_percent));
			}
			if(cryogenic_percent > 0) {
				p->sendln(CAT("cryogenic_percent:",cryogenic_percent));
			}
			if(radioactive_percent > 0) {
				p->sendln(CAT("radioactive_percent:",radioactive_percent));
			}
			if(emp_percent > 0) {
				p->sendln(CAT("emp_percent:",emp_percent));
			}
			if(shock_percent > 0) {
				p->sendln(CAT("shock_percent:",shock_percent));
			}
			if(anti_matter_percent > 0) {
				p->sendln(CAT("anti_matter_percent:",anti_matter_percent));
			}
			if(incendiary_damage > 0) {
				p->sendln(CAT("incendiary_damage:",incendiary_damage));
			}
			if(explosive_damage > 0) {
				p->sendln(CAT("explosive_damage:",explosive_damage));
			}
			if(shrapnel_damage > 0) {
				p->sendln(CAT("shrapnel_damage:",shrapnel_damage));
			}
			if(corrosive_damage > 0) {
				p->sendln(CAT("corrosive_damage:",corrosive_damage));
			}
			if(cryogenic_damage > 0) {
				p->sendln(CAT("cryogenic_damage:",cryogenic_damage));
			}
			if(radioactive_damage > 0) {
				p->sendln(CAT("radioactive_damage:",radioactive_damage));
			}
			if(emp_damage > 0) {
				p->sendln(CAT("emp_damage:",emp_damage));
			}
			if(shock_damage > 0) {
				p->sendln(CAT("shock_damage:",shock_damage));
			}
			if(anti_matter_damage > 0) {
				p->sendln(CAT("anti_matter_damage:",anti_matter_damage));
			}
			if(disorient_amount > 0) {
				p->sendln(CAT("disorient_amount:",disorient_amount));
			}
			if(stat_strength > 0) {
				p->sendln(CAT("stat_strength:",stat_strength));
			}
			if(stat_intelligence > 0) {
				p->sendln(CAT("stat_intelligence:",stat_intelligence));
			}
			if(stat_wisdom > 0) {
				p->sendln(CAT("stat_wisdom:",stat_wisdom));
			}
			if(stat_dexterity > 0) {
				p->sendln(CAT("stat_dexterity:",stat_dexterity));
			}
			if(stat_constitution > 0) {
				p->sendln(CAT("stat_constitution:",stat_constitution));
			}
			if(stat_electronics > 0) {
				p->sendln(CAT("stat_electronics:",stat_electronics));
			}
			if(stat_armor > 0) {
				p->sendln(CAT("stat_armor:",stat_armor));
			}
			if(stat_marksmanship > 0) {
				p->sendln(CAT("stat_marksmanship:",stat_marksmanship));
			}
			if(stat_sniping > 0) {
				p->sendln(CAT("stat_sniping:",stat_sniping));
			}
			if(stat_demolitions > 0) {
				p->sendln(CAT("stat_demolitions:",stat_demolitions));
			}
			if(stat_chemistry > 0) {
				p->sendln(CAT("stat_chemistry:",stat_chemistry));
			}
			if(stat_weapon_handling > 0) {
				p->sendln(CAT("stat_weapon_handling:",stat_weapon_handling));
			}
			if(stat_strategy > 0) {
				p->sendln(CAT("stat_strategy:",stat_strategy));
			}
			if(stat_medical > 0) {
				p->sendln(CAT("stat_medical:",stat_medical));
			}
		}
	};
};//end namespace mods

#endif
