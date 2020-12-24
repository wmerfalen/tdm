#include "generator.hpp"

namespace mods::forge_engine {
	//mods::forge_engine::generator item_generator;
	template <typename TOriginalValue>
	int16_t fetch(std::variant<uint32_t,float>& v,TOriginalValue original) {
		if(std::holds_alternative<uint32_t> (v)) {
			return std::get<uint32_t>(v);
		}
		return std::get<float>(v) * (original);
	}

	std::string to_string(explosive_attributes_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(EXPLOSIVE_ATTR_ALTERNATE_EXPLOSION_TYPE);
				M_LAZY(EXPLOSIVE_ATTR_CHANCE_TO_INJURE);
				M_LAZY(EXPLOSIVE_ATTR_CRITICAL_CHANCE);
				M_LAZY(EXPLOSIVE_ATTR_CRITICAL_RANGE);
				M_LAZY(EXPLOSIVE_ATTR_BLAST_RADIUS);
				M_LAZY(EXPLOSIVE_ATTR_DAMAGE_PER_SECOND);
				M_LAZY(EXPLOSIVE_ATTR_DISORIENT_AMOUNT);
				M_LAZY(EXPLOSIVE_ATTR_RANGE_MODIFIER);
				M_LAZY(EXPLOSIVE_ATTR_LOUDNESS_TYPE);
				M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_CHEMICAL_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_CHEMICAL_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_ELECTRIC_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_ELECTRIC_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_ARMOR_PENETRATION_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_ARMOR_PENETRATION_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_DAMAGE_DICE_COUNT);
				M_LAZY(EXPLOSIVE_ATTR_DAMAGE_DICE_SIDES);
				M_LAZY(EXPLOSIVE_ATTR_INCENDIARY_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_EXPLOSIVE_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_SHRAPNEL_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_CORROSIVE_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_CRYOGENIC_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_RADIATION_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_EMP_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_SHOCK_DAMAGE_PERCENT);
				M_LAZY(EXPLOSIVE_ATTR_ANTI_MATTER_DAMAGE_PERCENT);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}

	std::string to_string(player_class_types_t t) {
		switch(t) {
			case PLCLASS_SNIPER:
				return "SNIPER";

			case PLCLASS_MARINE:
				return "MARINE";

			case PLCLASS_BREACHER:
				return "BREACHER";

			case PLCLASS_ENGINEER:
				return "ENGINEER";

			case PLCLASS_MEDIC:
				return "MEDIC";

			case PLCLASS_GHOST:
				return "GHOST";

			case PLCLASS_SUPPORT:
				return "SUPPORT";

			default:
				return "<unknown>";
		}
	}
	std::string to_string(player_class_t t) {
		switch(t) {
			case CLASS_SNIPER:
				return "SNIPER";

			case CLASS_MARINE:
				return "MARINE";

			case CLASS_BREACHER:
				return "BREACHER";

			case CLASS_ENGINEER:
				return "ENGINEER";

			case CLASS_MEDIC:
				return "MEDIC";

			case CLASS_GHOST:
				return "GHOST";

			case CLASS_SUPPORT:
				return "SUPPORT";

			default:
				return "<unknown>";
		}
	}
	std::string to_string(attachment_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(ATTACHMENT_TYPE_SIGHT);
				M_LAZY(ATTACHMENT_TYPE_UNDER_BARREL);
				M_LAZY(ATTACHMENT_TYPE_GRIP);
				M_LAZY(ATTACHMENT_TYPE_BARREL);
				M_LAZY(ATTACHMENT_TYPE_MUZZLE);
				M_LAZY(ATTACHMENT_TYPE_MAGAZINE);
				M_LAZY(ATTACHMENT_TYPE_STOCK);
				M_LAZY(ATTACHMENT_TYPE_STRAP);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}

	std::string to_string(explosive_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(EXPLOSIVE_TYPE_FRAG_GRENADE);
				M_LAZY(EXPLOSIVE_TYPE_INCENDIARY_GRENADE);
				M_LAZY(EXPLOSIVE_TYPE_REMOTE_EXPLOSIVE);
				M_LAZY(EXPLOSIVE_TYPE_REMOTE_CHEMICAL);
				M_LAZY(EXPLOSIVE_TYPE_EMP_GRENADE);
				M_LAZY(EXPLOSIVE_TYPE_CLAYMORE_MINE);
				M_LAZY(EXPLOSIVE_TYPE_SMOKE_GRENADE);
				M_LAZY(EXPLOSIVE_TYPE_C4);
				M_LAZY(EXPLOSIVE_TYPE_BREACH_CHARGE);
				M_LAZY(EXPLOSIVE_TYPE_THERMITE_CHARGE);
				M_LAZY(EXPLOSIVE_TYPE_FLASHBANG_GRENADE);
				M_LAZY(EXPLOSIVE_TYPE_SENSOR_GRENADE);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}
	std::string to_string(armor_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(ARMOR_ITEM_WEAR_FINGER);
				M_LAZY(ARMOR_ITEM_WEAR_NECK);
				M_LAZY(ARMOR_ITEM_WEAR_BODY);
				M_LAZY(ARMOR_ITEM_WEAR_HEAD);
				M_LAZY(ARMOR_ITEM_WEAR_LEGS);
				M_LAZY(ARMOR_ITEM_WEAR_FEET);
				M_LAZY(ARMOR_ITEM_WEAR_HANDS);
				M_LAZY(ARMOR_ITEM_WEAR_ARMS);
				M_LAZY(ARMOR_ITEM_WEAR_SHIELD);
				M_LAZY(ARMOR_ITEM_WEAR_ABOUT);
				M_LAZY(ARMOR_ITEM_WEAR_WAIST);
				M_LAZY(ARMOR_ITEM_WEAR_WRIST);
				M_LAZY(ARMOR_ITEM_WEAR_WIELD);
				M_LAZY(ARMOR_ITEM_WEAR_HOLD);
				M_LAZY(ARMOR_ITEM_WEAR_SECONDARY);
				M_LAZY(ARMOR_ITEM_WEAR_SHOULDERS);
				M_LAZY(ARMOR_ITEM_WEAR_VEST_PACK);
				M_LAZY(ARMOR_ITEM_WEAR_ELBOW);
				M_LAZY(ARMOR_ITEM_WEAR_BACKPACK);
				M_LAZY(ARMOR_ITEM_WEAR_GOGGLES);
				M_LAZY(ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}
	std::string to_string(elemental_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(ELEM_INCENDIARY);
				M_LAZY(ELEM_EXPLOSIVE);
				M_LAZY(ELEM_SHRAPNEL);
				M_LAZY(ELEM_CORROSIVE);
				M_LAZY(ELEM_CRYOGENIC);
				M_LAZY(ELEM_RADIOACTIVE);
				M_LAZY(ELEM_EMP);
				M_LAZY(ELEM_SHOCK);
				M_LAZY(ELEM_ANTI_MATTER);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}
	std::string to_string(stat_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(SKILL_STR);
				M_LAZY(SKILL_STR_ADD);
				M_LAZY(SKILL_INTEL);
				M_LAZY(SKILL_WIS);
				M_LAZY(SKILL_DEX);
				M_LAZY(SKILL_CON);
				M_LAZY(SKILL_CHA);
				M_LAZY(SKILL_ELECTRONICS);
				M_LAZY(SKILL_ARMOR);
				M_LAZY(SKILL_MARKSMANSHIP);
				M_LAZY(SKILL_SNIPING);
				M_LAZY(SKILL_DEMOLITIONS);
				M_LAZY(SKILL_CHEMISTRY);
				M_LAZY(SKILL_WEAPON_HANDLING);
				M_LAZY(SKILL_STRATEGY);
				M_LAZY(SKILL_MEDICAL);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}

	std::string to_string(item_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(TYPE_ITEM_RIFLE);
				M_LAZY(TYPE_ITEM_EXPLOSIVE);
				M_LAZY(TYPE_ITEM_GADGET);
				M_LAZY(TYPE_ITEM_DRONE);
				M_LAZY(TYPE_ITEM_WEAPON);
				M_LAZY(TYPE_ITEM_ATTACHMENT);
				M_LAZY(TYPE_ITEM_ARMOR);
				M_LAZY(TYPE_ITEM_CONSUMABLE);
				M_LAZY(TYPE_ITEM_TRAP);
#undef M_LAZY

			default:
				std::cerr << "unknown: " << (int)t << "\n";
				return "<unknown>";
		}
	}
	std::string to_string(rifle_types_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(RIFLE_TYPE_SHOTGUN);
				M_LAZY(RIFLE_TYPE_ASSAULT_RIFLE);
				M_LAZY(RIFLE_TYPE_SUB_MACHINE_GUN);
				M_LAZY(RIFLE_TYPE_SNIPER);
				M_LAZY(RIFLE_TYPE_HANDGUN);
				M_LAZY(RIFLE_TYPE_PISTOL);
				M_LAZY(RIFLE_TYPE_MACHINE_PISTOL);
				M_LAZY(RIFLE_TYPE_LIGHT_MACHINE_GUN);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}

	std::string to_string(rifle_attributes_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(RIFLE_ATTRIBUTES_AMMO_MAX);
				M_LAZY(RIFLE_ATTRIBUTES_CHANCE_TO_INJURE);
				M_LAZY(RIFLE_ATTRIBUTES_CLIP_SIZE);
				M_LAZY(RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS);
				M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_CHANCE);
				M_LAZY(RIFLE_ATTRIBUTES_CRITICAL_RANGE);
				M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND);
				M_LAZY(RIFLE_ATTRIBUTES_DISORIENT_AMOUNT);
				M_LAZY(RIFLE_ATTRIBUTES_HEADSHOT_BONUS);
				M_LAZY(RIFLE_ATTRIBUTES_MAX_RANGE);
				M_LAZY(RIFLE_ATTRIBUTES_RANGE_MULTIPLIER);
				M_LAZY(RIFLE_ATTRIBUTES_RELOAD_TIME);
				M_LAZY(RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE);
				M_LAZY(RIFLE_ATTRIBUTES_MUZZLE_VELOCITY);
				M_LAZY(RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE);
				M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT);
				M_LAZY(RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES);
#undef M_LAZY

			default:
				return "<unknown>";
		}
	}

	std::string get_requirements_string(requirements_t& requirements) {
		std::string output;

		if(requirements.minimum_player_level) {
			output += CAT("minimum player level: ", requirements.minimum_player_level, "\r\n");
		}

		for(auto& pclass : requirements.player_classes) {
			output += CAT("class requirement: ", to_string(pclass), "\r\n");
		}

		for(auto& req : requirements.stats) {
			output += CAT("requirement: ", to_string(req.first), ": ", req.second, "\r\n");
		}

		return std::move(output);
	}

	void send_requirements(requirements_t& requirements, player_ptr_t& player) {
		player->send("%s\r\n", get_requirements_string(requirements).c_str());
	}

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
					attributes->radioactive_resistance_percent = fetch(req.second,attributes->radioactive_resistance_percent);
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
};//end namespace mods::forge_engine::util
