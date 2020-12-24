#include "generator.hpp"

namespace mods::forge_engine {
	template <typename TOriginalValue>
	int16_t fetch(std::variant<uint32_t,float>& v,TOriginalValue original) {
		if(std::holds_alternative<uint32_t> (v)) {
			return std::get<uint32_t>(v);
		}
		return std::get<float>(v) * (original);
	}

	std::string get_requirements_string(requirements_t& requirements);

	void send_requirements(requirements_t& requirements, player_ptr_t& player);

	template <typename TAttributeType,typename TAttributesPointer>
	void fill_stats_generic(obj_ptr_t& obj,TAttributeType& stats,TAttributesPointer* attributes) {
		for(auto& attr : stats) {
			switch(attr.first) {
				case SKILL_STR:
				case SKILL_STR_ADD:
					attributes->stat_strength = fetch(attr.second,attributes->stat_strength);
					break;
				case SKILL_INTEL:
					attributes->stat_intelligence = fetch(attr.second,attributes->stat_intelligence);
					break;
				case SKILL_WIS:
					attributes->stat_wisdom = fetch(attr.second,attributes->stat_wisdom);
					break;
				case SKILL_DEX:
					attributes->stat_dexterity = fetch(attr.second,attributes->stat_dexterity);
					break;
				case SKILL_CON:
					attributes->stat_constitution = fetch(attr.second,attributes->stat_constitution);
					break;
				case SKILL_CHA:
					break;
				case SKILL_ELECTRONICS:
					attributes->stat_electronics = fetch(attr.second,attributes->stat_electronics);
					break;
				case SKILL_ARMOR:
					attributes->stat_armor = fetch(attr.second,attributes->stat_armor);
					break;
				case SKILL_MARKSMANSHIP:
					attributes->stat_marksmanship = fetch(attr.second,attributes->stat_marksmanship);
					break;
				case SKILL_SNIPING:
					attributes->stat_sniping = fetch(attr.second,attributes->stat_sniping);
					break;
				case SKILL_DEMOLITIONS:
					attributes->stat_demolitions = fetch(attr.second,attributes->stat_demolitions);
					break;
				case SKILL_CHEMISTRY:
					attributes->stat_chemistry = fetch(attr.second,attributes->stat_chemistry);
					break;
				case SKILL_WEAPON_HANDLING:
					attributes->stat_weapon_handling = fetch(attr.second,attributes->stat_weapon_handling);
					break;
				case SKILL_STRATEGY:
					attributes->stat_strategy = fetch(attr.second,attributes->stat_strategy);
					break;
				case SKILL_MEDICAL:
					attributes->stat_medical = fetch(attr.second,attributes->stat_medical);
					break;
				default:
					break;
			}
		}
	}
	template <typename TAttributeContainer,typename TObjDataExtendedTypePointer>
	void fill_elemental_damage_generic(obj_ptr_t& obj,TAttributeContainer& damages, TObjDataExtendedTypePointer* attributes) {
		for(auto& req : damages) {
			switch(req.first) {
				case ELEM_INCENDIARY:
					attributes->incendiary_damage = fetch(req.second,attributes->incendiary_damage);
					break;
				case ELEM_EXPLOSIVE:
					attributes->explosive_damage = fetch(req.second,attributes->explosive_damage);
					break;
				case ELEM_SHRAPNEL:
					attributes->shrapnel_damage = fetch(req.second,attributes->shrapnel_damage);
					break;
				case ELEM_CORROSIVE:
					attributes->corrosive_damage = fetch(req.second,attributes->corrosive_damage);
					break;
				case ELEM_CRYOGENIC:
					attributes->cryogenic_damage = fetch(req.second,attributes->cryogenic_damage);
					break;
				case ELEM_RADIOACTIVE:
					attributes->radioactive_damage = fetch(req.second,attributes->radioactive_damage);
					break;
				case ELEM_EMP:
					attributes->emp_damage = fetch(req.second,attributes->emp_damage);
					break;
				case ELEM_SHOCK:
					attributes->shock_damage = fetch(req.second,attributes->shock_damage);
					break;
				case ELEM_ANTI_MATTER:
					attributes->anti_matter_damage = fetch(req.second,attributes->anti_matter_damage);
				default:
					break;
			}
		}
	}

	template <typename TAttributeContainer,typename TObjDataExtendedTypePointer>
	void fill_elemental_resistances_generic(obj_ptr_t& obj,TAttributeContainer& resistances, TObjDataExtendedTypePointer* attributes) {
		for(auto& req : resistances) {
			switch(req.first) {
				case ELEM_INCENDIARY:
					attributes->incendiary_resistance_percent = fetch(req.second,attributes->incendiary_resistance_percent);
					break;
				case ELEM_EXPLOSIVE:
					attributes->explosive_resistance_percent = fetch(req.second,attributes->explosive_resistance_percent);
					break;
				case ELEM_SHRAPNEL:
					attributes->shrapnel_resistance_percent = fetch(req.second,attributes->shrapnel_resistance_percent);
					break;
				case ELEM_CORROSIVE:
					attributes->corrosive_resistance_percent = fetch(req.second,attributes->corrosive_resistance_percent);
					break;
				case ELEM_CRYOGENIC:
					attributes->cryogenic_resistance_percent = fetch(req.second,attributes->cryogenic_resistance_percent);
					break;
				case ELEM_RADIOACTIVE:
					attributes->radiation_resistance_percent = fetch(req.second,attributes->radiation_resistance_percent);
					break;
				case ELEM_EMP:
					attributes->emp_resistance_percent = fetch(req.second,attributes->emp_resistance_percent);
					break;
				case ELEM_SHOCK:
					attributes->shock_resistance_percent = fetch(req.second,attributes->shock_resistance_percent);
					break;
				case ELEM_ANTI_MATTER:
					attributes->anti_matter_resistance_percent = fetch(req.second,attributes->anti_matter_resistance_percent);
				default:
					break;
			}
		}
	}
	template <typename TAttributeContainer,typename TObjDataExtendedTypePointer>
	void fill_rifle_attributes_generic(obj_ptr_t& obj,TAttributeContainer& attribute_list,TObjDataExtendedTypePointer* attributes) {
		for(auto& attr : attribute_list) {
			switch(attr.first) {
				case RIFLE_ATTRIBUTES_AMMO_MAX:
					attributes->ammo_max = fetch(attr.second,attributes->ammo_max);
					break;
				case RIFLE_ATTRIBUTES_CHANCE_TO_INJURE:
					attributes->chance_to_injure = fetch(attr.second,attributes->chance_to_injure);
					break;
				case RIFLE_ATTRIBUTES_CLIP_SIZE:
					attributes->clip_size = fetch(attr.second,attributes->clip_size);
					break;
					attributes->cooldown_between_shots = fetch(attr.second,attributes->cooldown_between_shots);
					break;
				case RIFLE_ATTRIBUTES_CRITICAL_CHANCE:
					attributes->critical_chance = fetch(attr.second,attributes->critical_chance);
					break;
				case RIFLE_ATTRIBUTES_CRITICAL_RANGE:
					attributes->critical_range = fetch(attr.second,attributes->critical_range);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND:
					attributes->damage_per_second = fetch(attr.second,attributes->damage_per_second);
					break;
				case RIFLE_ATTRIBUTES_DISORIENT_AMOUNT:
					attributes->disorient_amount = fetch(attr.second,attributes->disorient_amount);
					break;
				case RIFLE_ATTRIBUTES_HEADSHOT_BONUS:
					attributes->headshot_bonus = fetch(attr.second,attributes->headshot_bonus);
					break;
				case RIFLE_ATTRIBUTES_MAX_RANGE:
					attributes->max_range = fetch(attr.second,attributes->max_range);
					break;
				case RIFLE_ATTRIBUTES_RANGE_MULTIPLIER:
					attributes->range_multiplier = fetch(attr.second,attributes->range_multiplier);
					break;
				case RIFLE_ATTRIBUTES_RELOAD_TIME:
					attributes->reload_time = fetch(attr.second,attributes->reload_time);
					break;
				case RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE:
					attributes->rounds_per_minute = fetch(attr.second,attributes->rounds_per_minute);
					break;
				case RIFLE_ATTRIBUTES_MUZZLE_VELOCITY:
					attributes->muzzle_velocity = fetch(attr.second,attributes->muzzle_velocity);
					break;
				case RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE:
					attributes->effective_firing_range = fetch(attr.second,attributes->effective_firing_range);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT:
					attributes->damage_dice_count = fetch(attr.second,attributes->damage_dice_count);
					break;
				case RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES:
					attributes->damage_dice_sides = fetch(attr.second,attributes->damage_dice_sides);
					break;
				default:
					break;
			}
		}
	}
	template <typename TAttributeContainer,typename TObjDataExtendedTypePointer>
	void fill_armor_attributes_generic(obj_ptr_t& obj,TAttributeContainer& attribute_list,TObjDataExtendedTypePointer* attributes) {
		for(auto& attr : attribute_list) {
			switch(attr.first) {
				case ARMOR_ATTR_THAC0:
					attributes->thac0 = fetch(attr.second,attributes->thac0);
					break;
				case ARMOR_ATTR_WEIGHT:
					attributes->weight_in_lbs = fetch(attr.second,attributes->weight_in_lbs);
					break;
				case ARMOR_ATTR_INCENDIARY_RESISTANCE_PERCENT:
					attributes->incendiary_resistance_percent = fetch(attr.second,attributes->incendiary_resistance_percent);
					break;
				case ARMOR_ATTR_EXPLOSIVE_RESISTANCE_PERCENT:
					attributes->explosive_resistance_percent = fetch(attr.second,attributes->explosive_resistance_percent);
					break;
				case ARMOR_ATTR_SHRAPNEL_RESISTANCE_PERCENT:
					attributes->shrapnel_resistance_percent = fetch(attr.second,attributes->shrapnel_resistance_percent);
					break;
				case ARMOR_ATTR_CORROSIVE_RESISTANCE_PERCENT:
					attributes->corrosive_resistance_percent = fetch(attr.second,attributes->corrosive_resistance_percent);
					break;
				case ARMOR_ATTR_CRYOGENIC_RESISTANCE_PERCENT:
					attributes->cryogenic_resistance_percent = fetch(attr.second,attributes->cryogenic_resistance_percent);
					break;
				case ARMOR_ATTR_RADIATION_RESISTANCE_PERCENT:
					attributes->radiation_resistance_percent = fetch(attr.second,attributes->radiation_resistance_percent);
					break;
				case ARMOR_ATTR_EMP_RESISTANCE_PERCENT:
					attributes->emp_resistance_percent = fetch(attr.second,attributes->emp_resistance_percent);
					break;
				case ARMOR_ATTR_SHOCK_RESISTANCE_PERCENT:
					attributes->shock_resistance_percent = fetch(attr.second,attributes->shock_resistance_percent);
					break;
				case ARMOR_ATTR_ANTI_MATTER_RESISTANCE_PERCENT:
					attributes->anti_matter_resistance_percent = fetch(attr.second,attributes->anti_matter_resistance_percent);
					break;
				case ARMOR_ATTR_BALISTIC_RESISTANCE_PERCENT:
					attributes->balistic_resistance_percent = fetch(attr.second,attributes->balistic_resistance_percent);
					break;
				case ARMOR_ATTR_SPEED_PROFILE:
					attributes->speed_profile = fetch(attr.second,attributes->speed_profile);
					break;
				case ARMOR_ATTR_DURABLITY_PROFILE:
					//FIXME attributes->durability_profile = std::to_string(fetch(attr.second,attributes->durability_profile));
					break;
				case ARMOR_ATTR_REPAIR_COST_PROFILE:
					attributes->repair_cost_profile = fetch(attr.second,attributes->repair_cost_profile);
					break;
				case ARMOR_ATTR_HP:
					attributes->hp = fetch(attr.second,attributes->hp);
					break;
				case ARMOR_ATTR_CLASSIFICATION:
					//FIXME: attributes->classification = std::to_string(fetch(attr.second,attributes->classification));
					break;
				case ARMOR_ATTR_WORTH:
					attributes->worth = fetch(attr.second,attributes->worth);
					break;
				default:
					break;
			}
		}
	}
};//end namespace mods::forge_engine::util
