#include "util.hpp"

namespace mods::forge_engine {
	std::string random_yaml(const std::vector<std::string>& files) {
		if(files.size() == 0) {
			return "";
		}
		return files.at(roll_between<std::size_t>(0,files.size()));
	}
	std::string to_string(armor_attributes_t t) {
		switch(t) {
#define M_LAZY(A) case A: return #A;
				M_LAZY(ARMOR_ATTR_THAC0);
				M_LAZY(ARMOR_ATTR_WEIGHT);
				M_LAZY(ARMOR_ATTR_INCENDIARY_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_EXPLOSIVE_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_SHRAPNEL_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_CORROSIVE_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_CRYOGENIC_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_RADIATION_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_EMP_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_SHOCK_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_ANTI_MATTER_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_BALISTIC_RESISTANCE_PERCENT);
				M_LAZY(ARMOR_ATTR_SPEED_PROFILE);
				M_LAZY(ARMOR_ATTR_DURABLITY_PROFILE);
				M_LAZY(ARMOR_ATTR_REPAIR_COST_PROFILE);
				M_LAZY(ARMOR_ATTR_HP);
				M_LAZY(ARMOR_ATTR_CLASSIFICATION);
				M_LAZY(ARMOR_ATTR_WORTH);
#undef M_LAZY
			default:
				return "<unknown>";
		}
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

};//end namespace mods::forge_engine::util
