#ifndef __MENTOC_RANGED_COMBAT_TOTALS__
#define __MENTOC_RANGED_COMBAT_TOTALS__
#include "elemental.hpp"
#include <vector>
#include "rifle-stats.hpp"

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
		uint16_t max_range;
		uint16_t critical_chance;
		uint16_t chance_to_injure;
		float zoom_multiplier;
		uint16_t aimed_limb_accuracy_percent;
		int accuracy;

		std::pair<uint8_t,uint8_t> critical_range;
		std::pair<uint8_t,uint8_t> effective_range;
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

		uint8_t reload_time;
		uint8_t cooldown_between_shots;
		uint16_t ammo_max;
		uint8_t clip_size;

		uint16_t base_damage;
		uint16_t base_percent;
		uint16_t damage_dice_count;
		uint16_t damage_dice_sides;
		uint16_t armor_penetration;

		uint16_t damage_percent_bonus;
		uint16_t headshot_bonus;

		uint16_t hitroll;
		uint16_t damage_roll;

		uint16_t incendiary_percent;
		uint16_t explosive_percent;
		uint16_t shrapnel_percent;
		uint16_t corrosive_percent;
		uint16_t cryogenic_percent;
		uint16_t radioactive_percent;
		uint16_t emp_percent;
		uint16_t shock_percent;
		uint16_t anti_matter_percent;

		uint16_t incendiary_damage;
		uint16_t explosive_damage;
		uint16_t shrapnel_damage;
		uint16_t corrosive_damage;
		uint16_t cryogenic_damage;
		uint16_t radioactive_damage;
		uint16_t emp_damage;
		uint16_t shock_damage;
		uint16_t anti_matter_damage;

		uint16_t disorient_amount;

		uint16_t stat_strength;
		uint16_t stat_intelligence;
		uint16_t stat_wisdom;
		uint16_t stat_dexterity;
		uint16_t stat_constitution;
		uint16_t stat_electronics;
		uint16_t stat_armor;
		uint16_t stat_marksmanship;
		uint16_t stat_sniping;
		uint16_t stat_demolitions;
		uint16_t stat_chemistry;
		uint16_t stat_weapon_handling;
		uint16_t stat_strategy;
		uint16_t stat_medical;
		int16_t thermal_range;
		int16_t night_vision_range;

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
			stat_medical(0),
			thermal_range(0),
			night_vision_range(0) {
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
			thermal_range = other.thermal_range;
			night_vision_range = other.night_vision_range;
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
			thermal_range = other.thermal_range;
			night_vision_range = other.night_vision_range;
			return *this;
		}
		void assign(ranged_combat_totals* other) {
			ranged_combat_totals();
			zoom_magnification = other->zoom_magnification;
			max_range = other->max_range;
			critical_chance = other->critical_chance;
			chance_to_injure = other->chance_to_injure;
			zoom_multiplier = other->zoom_multiplier;
			aimed_limb_accuracy_percent = other->aimed_limb_accuracy_percent;
			accuracy = other->accuracy;
			critical_range = other->critical_range;
			effective_range = other->effective_range;
			viable_targets = other->viable_targets;
			vision = other->vision;
			recoil_reduction = other->recoil_reduction;
			muzzle_velocity = other->muzzle_velocity;
			reload_time = other->reload_time;
			cooldown_between_shots = other->cooldown_between_shots;
			ammo_max = other->ammo_max;
			clip_size = other->clip_size;
			base_damage = other->base_damage;
			base_percent = other->base_percent;
			damage_dice_count = other->damage_dice_count;
			damage_dice_sides = other->damage_dice_sides;
			armor_penetration = other->armor_penetration;
			damage_percent_bonus = other->damage_percent_bonus;
			headshot_bonus = other->headshot_bonus;
			hitroll = other->hitroll;
			damage_roll = other->damage_roll;

			incendiary_percent = other->incendiary_percent;
			explosive_percent = other->explosive_percent;
			shrapnel_percent = other->shrapnel_percent;
			corrosive_percent = other->corrosive_percent;
			cryogenic_percent = other->cryogenic_percent;
			radioactive_percent = other->radioactive_percent;
			emp_percent = other->emp_percent;
			shock_percent = other->shock_percent;
			anti_matter_percent = other->anti_matter_percent;

			incendiary_damage = other->incendiary_damage;
			explosive_damage = other->explosive_damage;
			shrapnel_damage = other->shrapnel_damage;
			corrosive_damage = other->corrosive_damage;
			cryogenic_damage = other->cryogenic_damage;
			radioactive_damage = other->radioactive_damage;
			emp_damage = other->emp_damage;
			shock_damage = other->shock_damage;
			anti_matter_damage = other->anti_matter_damage;

			disorient_amount = other->disorient_amount;

			stat_strength = other->stat_strength;
			stat_intelligence = other->stat_intelligence;
			stat_wisdom = other->stat_wisdom;
			stat_dexterity = other->stat_dexterity;
			stat_constitution = other->stat_constitution;
			stat_electronics = other->stat_electronics;
			stat_armor = other->stat_armor;
			stat_marksmanship = other->stat_marksmanship;
			stat_sniping = other->stat_sniping;
			stat_demolitions = other->stat_demolitions;
			stat_chemistry = other->stat_chemistry;
			stat_weapon_handling = other->stat_weapon_handling;
			stat_strategy = other->stat_strategy;
			stat_medical = other->stat_medical;
			tracked = other->tracked;
			marked = other->marked;
			thermal_range = other->thermal_range;
			night_vision_range = other->night_vision_range;
		}
		~ranged_combat_totals() = default;
		template <typename T>
		void report(T p) {
			std::map<std::string,std::string> m;
			if(zoom_magnification > 0) {
				m["rifle_zoom_magnification"] = std::to_string(zoom_magnification);
			}
			if(max_range > 0) {
				m["rifle_max_range"] = std::to_string(max_range);
			}
			if(critical_chance > 0) {
				m["rifle_critical_chance"] = std::to_string(critical_chance);
			}
			if(chance_to_injure > 0) {
				m["rifle_chance_to_injure"] = std::to_string(chance_to_injure);
			}
			if(zoom_multiplier > 0) {
				m["rifle_zoom_multiplier"] = std::to_string(zoom_multiplier);
			}
			if(aimed_limb_accuracy_percent > 0) {
				m["rifle_aimed_limb_accuracy_percent"] = std::to_string(aimed_limb_accuracy_percent);
			}
			if(accuracy > 0) {
				m["rifle_accuracy"] = std::to_string(accuracy);
			}
			if(critical_range.second > 0) {
				m["rifle_critical_range"] = CAT(critical_range.first,"->",critical_range.second);
			}
			if(effective_range.second > 0) {
				m["rifle_effective_range"] = CAT(effective_range.first,"->",effective_range.second);
			}
			if(viable_targets > 0) {
				m["rifle_viable_targets"] = std::to_string(viable_targets);
			}
			if(vision > 0) {
				m["rifle_vision"] = std::to_string(vision);
			}
			if(recoil_reduction > 0) {
				m["rifle_recoil_reduction"] = std::to_string(recoil_reduction);
			}
			if(muzzle_velocity > 0) {
				m["rifle_muzzle_velocity"] = std::to_string(muzzle_velocity);
			}
			if(reload_time > 0) {
				m["rifle_reload_time"] = std::to_string(reload_time);
			}
			if(cooldown_between_shots > 0) {
				m["rifle_cooldown_between_shots"] = std::to_string(cooldown_between_shots);
			}
			if(ammo_max > 0) {
				m["rifle_ammo_max"] = std::to_string(ammo_max);
			}
			if(clip_size > 0) {
				m["rifle_clip_size"] = std::to_string(clip_size);
			}
			if(base_damage > 0) {
				m["rifle_base_damage"] = std::to_string(base_damage);
			}
			if(base_percent > 0) {
				m["rifle_base_percent"] = std::to_string(base_percent);
			}
			if(damage_dice_count > 0) {
				m["rifle_damage_dice_count"] = std::to_string(damage_dice_count);
			}
			if(damage_dice_sides > 0) {
				m["rifle_damage_dice_sides"] = std::to_string(damage_dice_sides);
			}
			if(armor_penetration > 0) {
				m["rifle_armor_penetration"] = std::to_string(armor_penetration);
			}
			if(damage_percent_bonus > 0) {
				m["rifle_damage_percent_bonus"] = std::to_string(damage_percent_bonus);
			}
			if(headshot_bonus > 0) {
				m["rifle_headshot_bonus"] = std::to_string(headshot_bonus);
			}
			if(hitroll > 0) {
				m["rifle_hitroll"] = std::to_string(hitroll);
			}
			if(damage_roll > 0) {
				m["rifle_damage_roll"] = std::to_string(damage_roll);
			}
			if(incendiary_percent > 0) {
				m["rifle_incendiary_percent"] = std::to_string(incendiary_percent);
			}
			if(explosive_percent > 0) {
				m["rifle_explosive_percent"] = std::to_string(explosive_percent);
			}
			if(shrapnel_percent > 0) {
				m["rifle_shrapnel_percent"] = std::to_string(shrapnel_percent);
			}
			if(corrosive_percent > 0) {
				m["rifle_corrosive_percent"] = std::to_string(corrosive_percent);
			}
			if(cryogenic_percent > 0) {
				m["rifle_cryogenic_percent"] = std::to_string(cryogenic_percent);
			}
			if(radioactive_percent > 0) {
				m["rifle_radioactive_percent"] = std::to_string(radioactive_percent);
			}
			if(emp_percent > 0) {
				m["rifle_emp_percent"] = std::to_string(emp_percent);
			}
			if(shock_percent > 0) {
				m["rifle_shock_percent"] = std::to_string(shock_percent);
			}
			if(anti_matter_percent > 0) {
				m["rifle_anti_matter_percent"] = std::to_string(anti_matter_percent);
			}
			if(incendiary_damage > 0) {
				m["rifle_incendiary_damage"] = std::to_string(incendiary_damage);
			}
			if(explosive_damage > 0) {
				m["rifle_explosive_damage"] = std::to_string(explosive_damage);
			}
			if(shrapnel_damage > 0) {
				m["rifle_shrapnel_damage"] = std::to_string(shrapnel_damage);
			}
			if(corrosive_damage > 0) {
				m["rifle_corrosive_damage"] = std::to_string(corrosive_damage);
			}
			if(cryogenic_damage > 0) {
				m["rifle_cryogenic_damage"] = std::to_string(cryogenic_damage);
			}
			if(radioactive_damage > 0) {
				m["rifle_radioactive_damage"] = std::to_string(radioactive_damage);
			}
			if(emp_damage > 0) {
				m["rifle_emp_damage"] = std::to_string(emp_damage);
			}
			if(shock_damage > 0) {
				m["rifle_shock_damage"] = std::to_string(shock_damage);
			}
			if(anti_matter_damage > 0) {
				m["rifle_anti_matter_damage"] = std::to_string(anti_matter_damage);
			}
			if(disorient_amount > 0) {
				m["rifle_disorient_amount"] = std::to_string(disorient_amount);
			}
			if(stat_strength > 0) {
				m["rifle_stat_strength"] = std::to_string(stat_strength);
			}
			if(stat_intelligence > 0) {
				m["rifle_stat_intelligence"] = std::to_string(stat_intelligence);
			}
			if(stat_wisdom > 0) {
				m["rifle_stat_wisdom"] = std::to_string(stat_wisdom);
			}
			if(stat_dexterity > 0) {
				m["rifle_stat_dexterity"] = std::to_string(stat_dexterity);
			}
			if(stat_constitution > 0) {
				m["rifle_stat_constitution"] = std::to_string(stat_constitution);
			}
			if(stat_electronics > 0) {
				m["rifle_stat_electronics"] = std::to_string(stat_electronics);
			}
			if(stat_armor > 0) {
				m["rifle_stat_armor"] = std::to_string(stat_armor);
			}
			if(stat_marksmanship > 0) {
				m["rifle_stat_marksmanship"] = std::to_string(stat_marksmanship);
			}
			if(stat_sniping > 0) {
				m["rifle_stat_sniping"] = std::to_string(stat_sniping);
			}
			if(stat_demolitions > 0) {
				m["rifle_stat_demolitions"] = std::to_string(stat_demolitions);
			}
			if(stat_chemistry > 0) {
				m["rifle_stat_chemistry"] = std::to_string(stat_chemistry);
			}
			if(stat_weapon_handling > 0) {
				m["rifle_stat_weapon_handling"] = std::to_string(stat_weapon_handling);
			}
			if(stat_strategy > 0) {
				m["rifle_stat_strategy"] = std::to_string(stat_strategy);
			}
			if(stat_medical > 0) {
				m["rifle_stat_medical"] = std::to_string(stat_medical);
			}
			p->sendln(mods::rifle_stats::format_rifle_stats_page(&m));
			p->sendln("Viable targets: ");
			if(viable_targets & CAN_HIT_SAME_ROOM) {
				p->sendln("Same room");
			}
			if(viable_targets & CAN_HIT_RANGED) {
				p->sendln("Ranged");
			}
			if(viable_targets & CAN_HIT_DOORS) {
				p->sendln("Doors");
			}
			if(viable_targets & CAN_HIT_OBJECTS) {
				p->sendln("Objects");
			}
			if(viable_targets & CAN_HIT_CARS) {
				p->sendln("Cars");
			}
			p->sendln("Vision modes: ");
			if(vision & HAS_NIGHT_VISION) {
				p->sendln("Night vision");
			}
			if(vision & HAS_THERMALS) {
				p->sendln("Thermals");
			}
			if(vision & HAS_SHADOW_SIGHT) {
				p->sendln("Shadow sight");
			}
			if(vision & HAS_MORBID_INSIGHT) {
				p->sendln("Morbid insight");
			}
			p->sendln(CAT("Thermal range: ",thermal_range));
			p->sendln(CAT("Night vision range: ",night_vision_range));
		}
	};
};//end namespace mods

#endif
