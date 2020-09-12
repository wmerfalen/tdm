#include "values.hpp"
#include "lmdb.hpp"
#include <map>
#include <string>
#include <vector>

#define __MENTOC_MODS_VALUES_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_VALUES_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::values]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

extern bool key_exists(std::string key);
namespace mods::values {
		std::map<std::string,variants_t> alt_value_map;
		static const std::vector<std::string> float_types = {
		"EXPLOSIVE_CRITICAL_MULTIPLIER",
		"RIFLE_CRITICAL_MULTIPLIER",
		"INJURED_MAX_HP_MULTIPLIER",
		"MINI_GUNNER_DECREASED_SIGHT_MULTIPLIER",
		"CHANCE_TO_INJURE_SKILL_MODIFIER",
		"INJURE_RESISTANCE_SKILL_MODIFIER",
		"ASSAULT_RIFLE_SHRAPNEL_SKILL_DAMAGE_MULTIPLIER",
		};
		static const std::vector<std::string> int_types = {
		"FIRE_DAMAGE_TICK_RESOLUTION",
		"FLASHBANG_COOLDOWN_TICKS",
		"FIRE_EVERY_N_TICKS",
		"FIRE_WOODEN_ADDITIONAL_TICKS",
		"FIRE_CARPET_ADDITIONAL_TICKS",
		"DAMAGE_DIVISOR",
		"FRAG_GRENADE_TICKS",
		"EMP_GRENADE_TICKS",
		"SENSOR_GRENADE_TICKS",
		"FLASHBANG_GRENADE_TICKS",
		"SMOKE_GRENADE_TICKS",
		"INCENDIARY_GRENADE_TICKS",
		"FIRE_STATUS_KINDLING_DAMAGE",
		"FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE",
		"FIRE_STATUS_SMOLDERING_DAMAGE",
		"FIRE_STATUS_SMOKING_DAMAGE",
		};
		static const std::vector<std::string> uint8_types = {
		"SINGLE_SHOT_SHOTGUN",
		"SINGLE_SHOT_ASSAULT_RIFLE",
		"SINGLE_SHOT_SUB_MACHINE_GUN",
		"SINGLE_SHOT_SNIPER",
		"SINGLE_SHOT_HANDGUN",
		"SINGLE_SHOT_PISTOL",
		"SINGLE_SHOT_MACHINE_PISTOL",
		"SINGLE_SHOT_LIGHT_MACHINE_GUN",
		"SPRAY_SHOT_SHOTGUN",
		"SPRAY_SHOT_ASSAULT_RIFLE",
		"SPRAY_SHOT_SUB_MACHINE_GUN",
		"SPRAY_SHOT_SNIPER",
		"SPRAY_SHOT_HANDGUN",
		"SPRAY_SHOT_PISTOL",
		"SPRAY_SHOT_MACHINE_PISTOL",
		"SPRAY_SHOT_LIGHT_MACHINE_GUN",
		"SPRAY_CHANCE",
		"SPRAY_HEADSHOT_CHANCE",
		"SPRAY_CRITICAL_CHANCE",
		"SPRAY_CRITICAL_REDUCTION_DIVISOR",
		"REVIVE_TICKS",
		"REVIVE_HP",
		"INJURED_HP",
		"EXPLOSIVE_CRITICAL_MULTIPLIER",
		"RIFLE_CRITICAL_MULTIPLIER",
		"MINI_GUNNER_SCAN_DEPTH",
		"SPRAY_CHANCE_SKILL_MODIFIER",
		"HEADSHOT_SKILL_MODIFIER",
		"ASSAULT_RIFLE_SHRAPNEL_SKILL_CHANCE",
		"ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_COUNT",
		"ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_SIDES",
		"SHOW_UNIMPLEMENTED_SKILLS",
		};


		static const std::vector<std::string> uint16_types = {
		"WSL_SHOTGUN_0_ALLOWABLE",
		"WSL_SHOTGUN_0_ACCURACY",
		"WSL_SHOTGUN_0_DAMAGE",
		"WSL_SHOTGUN_1_ALLOWABLE",
		"WSL_SHOTGUN_1_ACCURACY",
		"WSL_SHOTGUN_1_DAMAGE",
		"WSL_SHOTGUN_2_ALLOWABLE",
		"WSL_SHOTGUN_2_ACCURACY",
		"WSL_SHOTGUN_2_DAMAGE",
		"WSL_SHOTGUN_3_ALLOWABLE",
		"WSL_SHOTGUN_3_ACCURACY",
		"WSL_SHOTGUN_3_DAMAGE",
		"WSL_SHOTGUN_4_ALLOWABLE",
		"WSL_SHOTGUN_4_ACCURACY",
		"WSL_SHOTGUN_4_DAMAGE",
		"WSL_AR_0_ALLOWABLE",
		"WSL_AR_0_ACCURACY",
		"WSL_AR_0_DAMAGE",
		"WSL_AR_1_ALLOWABLE",
		"WSL_AR_1_ACCURACY",
		"WSL_AR_1_DAMAGE",
		"WSL_AR_2_ALLOWABLE",
		"WSL_AR_2_ACCURACY",
		"WSL_AR_2_DAMAGE",
		"WSL_AR_3_ALLOWABLE",
		"WSL_AR_3_ACCURACY",
		"WSL_AR_3_DAMAGE",
		"WSL_AR_4_ALLOWABLE",
		"WSL_AR_4_ACCURACY",
		"WSL_AR_4_DAMAGE",
		"WSL_SMG_0_ALLOWABLE",
		"WSL_SMG_0_ACCURACY",
		"WSL_SMG_0_DAMAGE",
		"WSL_SMG_1_ALLOWABLE",
		"WSL_SMG_1_ACCURACY",
		"WSL_SMG_1_DAMAGE",
		"WSL_SMG_2_ALLOWABLE",
		"WSL_SMG_2_ACCURACY",
		"WSL_SMG_2_DAMAGE",
		"WSL_SMG_3_ALLOWABLE",
		"WSL_SMG_3_ACCURACY",
		"WSL_SMG_3_DAMAGE",
		"WSL_SMG_4_ALLOWABLE",
		"WSL_SMG_4_ACCURACY",
		"WSL_SMG_4_DAMAGE",
		"WSL_MACHINE_PISTOL_0_ALLOWABLE",
		"WSL_MACHINE_PISTOL_0_ACCURACY",
		"WSL_MACHINE_PISTOL_0_DAMAGE",
		"WSL_MACHINE_PISTOL_1_ALLOWABLE",
		"WSL_MACHINE_PISTOL_1_ACCURACY",
		"WSL_MACHINE_PISTOL_1_DAMAGE",
		"WSL_MACHINE_PISTOL_2_ALLOWABLE",
		"WSL_MACHINE_PISTOL_2_ACCURACY",
		"WSL_MACHINE_PISTOL_2_DAMAGE",
		"WSL_MACHINE_PISTOL_3_ALLOWABLE",
		"WSL_MACHINE_PISTOL_3_ACCURACY",
		"WSL_MACHINE_PISTOL_3_DAMAGE",
		"WSL_MACHINE_PISTOL_4_ALLOWABLE",
		"WSL_MACHINE_PISTOL_4_ACCURACY",
		"WSL_MACHINE_PISTOL_4_DAMAGE",
		"WSL_HANDGUN_0_ALLOWABLE",
		"WSL_HANDGUN_0_ACCURACY",
		"WSL_HANDGUN_0_DAMAGE",
		"WSL_HANDGUN_1_ALLOWABLE",
		"WSL_HANDGUN_1_ACCURACY",
		"WSL_HANDGUN_1_DAMAGE",
		"WSL_HANDGUN_2_ALLOWABLE",
		"WSL_HANDGUN_2_ACCURACY",
		"WSL_HANDGUN_2_DAMAGE",
		"WSL_HANDGUN_3_ALLOWABLE",
		"WSL_HANDGUN_3_ACCURACY",
		"WSL_HANDGUN_3_DAMAGE",
		"WSL_HANDGUN_4_ALLOWABLE",
		"WSL_HANDGUN_4_ACCURACY",
		"WSL_HANDGUN_4_DAMAGE",
		"WSL_LMG_0_ALLOWABLE",
		"WSL_LMG_0_ACCURACY",
		"WSL_LMG_0_DAMAGE",
		"WSL_LMG_1_ALLOWABLE",
		"WSL_LMG_1_ACCURACY",
		"WSL_LMG_1_DAMAGE",
		"WSL_LMG_2_ALLOWABLE",
		"WSL_LMG_2_ACCURACY",
		"WSL_LMG_2_DAMAGE",
		"WSL_LMG_3_ALLOWABLE",
		"WSL_LMG_3_ACCURACY",
		"WSL_LMG_3_DAMAGE",
		"WSL_LMG_4_ALLOWABLE",
		"WSL_LMG_4_ACCURACY",
		"WSL_LMG_4_DAMAGE",
		"WSL_SNIPER_0_ALLOWABLE",
		"WSL_SNIPER_0_ACCURACY",
		"WSL_SNIPER_0_DAMAGE",
		"WSL_SNIPER_1_ALLOWABLE",
		"WSL_SNIPER_1_ACCURACY",
		"WSL_SNIPER_1_DAMAGE",
		"WSL_SNIPER_2_ALLOWABLE",
		"WSL_SNIPER_2_ACCURACY",
		"WSL_SNIPER_2_DAMAGE",
		"WSL_SNIPER_3_ALLOWABLE",
		"WSL_SNIPER_3_ACCURACY",
		"WSL_SNIPER_3_DAMAGE",
		"WSL_SNIPER_4_ALLOWABLE",
		"WSL_SNIPER_4_ACCURACY",
		"WSL_SNIPER_4_DAMAGE",
		"HEADSHOT_DIVISOR",
"SKILL_CAMERA_PLACEMENT",
"SKILL_DETECT_CAMERAS",
"SKILL_CAMERA_DESTRUCTION",
"SKILL_ELECTRIFIED_DAMAGE",
"SKILL_PROTECT_FROM_EMP",
"SKILL_CREATE_EMP_WEAPONS",
"SKILL_COUNTER_SHOCK",
"SKILL_TOGGLE_THERMAL",
"SKILL_TOGGLE_NIGHT_VISION",
"SKILL_HEADGEAR_INTROSPECTION",
"SKILL_PROXIMITY_ALARM",
"SKILL_DEPLOYABLE_TURRET",
"SKILL_DEPLOYABLE_FIRE_TURRET",
"SKILL_DOOR_CAMERA",
"SKILL_BASIC_ARMOR",
"SKILL_ADVANCED_ARMOR",
"SKILL_ELITE_ARMOR",
"SKILL_THIEF",
"SKILL_MOLD",
"SKILL_NO_FACTOR_BASIC",
"SKILL_NO_FACTOR_ADVANCED",
"SKILL_NO_FACTOR_ELITE",
"SKILL_EXPLOSIVE_RESISTANCE",
"SKILL_INJURE_RESISTANCE",
"SKILL_MUNITIONS_REFLECTOR",
"SKILL_SENTINEL_DISCIPLINE",
"SKILL_DEPLOYABLE_SHIELD",
"SKILL_SPRAY_CHANCE",
"SKILL_HEADSHOT_CHANCE",
"SKILL_LIMB_CHANCE",
"SKILL_BETTER_SNIPING_ACCURACY",
"SKILL_TARGET_LIMB",
"SKILL_ACCURACY",
"SKILL_VIP",
"SKILL_TRACKER",
"SKILL_QUIETER_SNIPER_SHOTS",
"SKILL_FASTER_BREACHES",
"SKILL_FURTHER_C4S",
"SKILL_STRONGER_FRAG_ARM",
"SKILL_BIGGER_CLAYMORES",
"SKILL_FASTER_THERMITES",
"SKILL_BIGGER_FIRE_NADES",
"SKILL_CRAFTY_C4",
"SKILL_BETTER_EXPLOSIONS",
"SKILL_RETROFIT_GRENADE_LAUNCHER",
"SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER",
"SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER",
"SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER",
"SKILL_RETROFIT_STUN_GRENADE_LAUNCHER",
"SKILL_STEROID_DEALER",
"SKILL_HGH_DEALER",
"SKILL_NOXIOUS_NADE_CRAFTER",
"SKILL_DUTCH_OVEN",
"SKILL_FIRE_NADE_DEALER",
"SKILL_PRO_WEAPON_HANDLER",
"SKILL_BETTER_WEAPON_ACCURACY",
"SKILL_FASTER_TRIGGER_FINGER",
"SKILL_FASTER_RELOADING",
"SKILL_BASIC_PISTOL",
"SKILL_BASIC_ASSAULT_RIFLE",
"SKILL_BASIC_SHOTGUN",
"SKILL_BASIC_SNIPER_RIFLE",
"SKILL_BASIC_SUB_MACHINE_GUN",
"SKILL_INTERMEDIATE_PISTOL",
"SKILL_INTERMEDIATE_ASSAULT_RIFLE",
"SKILL_INTERMEDIATE_SHOTGUN",
"SKILL_INTERMEDIATE_SNIPER_RIFLE",
"SKILL_INTERMEDIATE_SUB_MACHINE_GUN",
"SKILL_ADVANCED_PISTOL",
"SKILL_ADVANCED_ASSAULT_RIFLE",
"SKILL_ADVANCED_SHOTGUN",
"SKILL_ADVANCED_SNIPER_RIFLE",
"SKILL_ADVANCED_SUB_MACHINE_GUN",
"SKILL_EXPERT_PISTOL",
"SKILL_EXPERT_ASSAULT_RIFLE",
"SKILL_EXPERT_SHOTGUN",
"SKILL_EXPERT_SNIPER_RIFLE",
"SKILL_EXPERT_SUB_MACHINE_GUN",
"SKILL_ARMOR_PENETRATION_SHOT",
"SKILL_NONSTOP_PENETRATION_SHOT",
"SKILL_INCREASED_INJURE_CHANCE",
"SKILL_ASSAULT_RIFLE_SHRAPNEL",
"SKILL_ENTRY_DENIAL",
"SKILL_MISDIRECTION",
"SKILL_SNIPER_SUPPORT",
"SKILL_REDUCED_DETECTION_CHANCE",
"SKILL_INCREASED_AWARENESS",
"SKILL_TRICK_MAGAZINE",
"SKILL_CHAINABLE_BREACHING",
"SKILL_HGH_MORE_POWERFUL",
"SKILL_BASIC_HP_RECOVERY",
"SKILL_PARASITIC_HP_RECOVERY",
"SKILL_SUTURE",
"SKILL_ADRENALINE_BOOST",
		};
			static const std::vector<std::string> string_types = {
		"SANITY_CHECK",
		"MINI_GUNNER_WEAR_LIGHT",
		"MINI_GUNNER_WEAR_FINGER_R",
		"MINI_GUNNER_WEAR_FINGER_L",
		"MINI_GUNNER_WEAR_NECK_1",
		"MINI_GUNNER_WEAR_NECK_2",
		"MINI_GUNNER_WEAR_BODY",
		"MINI_GUNNER_WEAR_HEAD",
		"MINI_GUNNER_WEAR_LEGS",
		"MINI_GUNNER_WEAR_FEET",
		"MINI_GUNNER_WEAR_HANDS",
		"MINI_GUNNER_WEAR_ARMS",
		"MINI_GUNNER_WEAR_SHIELD",
		"MINI_GUNNER_WEAR_ABOUT",
		"MINI_GUNNER_WEAR_WAIST",
		"MINI_GUNNER_WEAR_WRIST_R",
		"MINI_GUNNER_WEAR_WRIST_L",
		"MINI_GUNNER_WEAR_PRIMARY",
		"MINI_GUNNER_WEAR_WIELD",
		"MINI_GUNNER_WEAR_HOLD",
		"MINI_GUNNER_WEAR_SECONDARY_WEAPON",
		"MINI_GUNNER_WEAR_SECONDARY",
		"MINI_GUNNER_WEAR_WEAPON_ATTACHMENT",
		"MINI_GUNNER_WEAR_SHOULDERS_L",
		"MINI_GUNNER_WEAR_SHOULDERS_R",
		"MINI_GUNNER_WEAR_BACKPACK",
		"MINI_GUNNER_WEAR_GOGGLES",
		"MINI_GUNNER_WEAR_VEST_PACK",
		"MINI_GUNNER_WEAR_ELBOW_L",
		"MINI_GUNNER_WEAR_ELBOW_R",
		"LUNATIC_WEAR_LIGHT",
		"LUNATIC_WEAR_FINGER_R",
		"LUNATIC_WEAR_FINGER_L",
		"LUNATIC_WEAR_NECK_1",
		"LUNATIC_WEAR_NECK_2",
		"LUNATIC_WEAR_BODY",
		"LUNATIC_WEAR_HEAD",
		"LUNATIC_WEAR_LEGS",
		"LUNATIC_WEAR_FEET",
		"LUNATIC_WEAR_HANDS",
		"LUNATIC_WEAR_ARMS",
		"LUNATIC_WEAR_SHIELD",
		"LUNATIC_WEAR_ABOUT",
		"LUNATIC_WEAR_WAIST",
		"LUNATIC_WEAR_WRIST_R",
		"LUNATIC_WEAR_WRIST_L",
		"LUNATIC_WEAR_PRIMARY",
		"LUNATIC_WEAR_WIELD",
		"LUNATIC_WEAR_HOLD",
		"LUNATIC_WEAR_SECONDARY_WEAPON",
		"LUNATIC_WEAR_SECONDARY",
		"LUNATIC_WEAR_WEAPON_ATTACHMENT",
		"LUNATIC_WEAR_SHOULDERS_L",
		"LUNATIC_WEAR_SHOULDERS_R",
		"LUNATIC_WEAR_BACKPACK",
		"LUNATIC_WEAR_GOGGLES",
		"LUNATIC_WEAR_VEST_PACK",
		"LUNATIC_WEAR_ELBOW_L",
		"LUNATIC_WEAR_ELBOW_R",
		"MSG_CRITICAL",
		"MSG_HEADSHOT",
		"MSG_HIT",
		"MSG_OUT_OF_AMMO",
		"MSG_MISSED_TARGET",
		"MSG_TARGET_DEAD",
		"MSG_YOURE_INJURED",
		"MSG_FIRE_DAMAGE",
		"MSG_SMOKE_DAMAGE",
		"MSG_NARROWLY_MISSED_ME",
		"MSG_YOU_ARE_INJURED",
		"MSG_YOUR_TARGET_IS_DEAD",
		"MSG_HIT_BY_RIFLE_ATTACK",
		"MSG_HIT_BY_SPRAY_ATTACK",
		"MSG_TARGET_IN_PEACEFUL_ROOM",
		"MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS",
		"MSG_NO_PRIMARY_WIELDED",
		"MSG_OUT_OF_AMMO",
		"MSG_COOLDOWN_IN_EFFECT",
		"MSG_COULDNT_FIND_TARGET",
		"MSG_HIT_BY_HEADSHOT",
		"MSG_YOU_INJURED_SOMEONE",
		"DEFAULT_PUBLIC_CHANNELS",
		"EXTENDED_PREFERENCES",
		"EXTENDED_PREFERENCES_DEFAULTS",
		"SUPER_USERS_LIST",
		"MINI_GUNNER_RANDOM_DISORIENT_STRINGS",
		"PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_FMG9_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_P90_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_AUGPARA_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_TAR21_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_SCARH_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_UMP45_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_MK46_DESCRIPTION",
		"PRIMARY_CHOICE_SCREEN_HK21_DESCRIPTION",
		"CHARGEN_SNIPER_CLASS_DESCRIPTION",
		"CHARGEN_MARINE_CLASS_DESCRIPTION",
		"CHARGEN_SENTINEL_CLASS_DESCRIPTION",
		"CHARGEN_CONTAGION_CLASS_DESCRIPTION",
		"CHARGEN_ENGINEER_CLASS_DESCRIPTION",
		"CHARGEN_MEDIC_CLASS_DESCRIPTION",
		"CHARGEN_PSYOP_CLASS_DESCRIPTION",
		"CHARGEN_SUPPORT_CLASS_DESCRIPTION",
		"CHARGEN_UNDEFINED_CLASS_DESCRIPTION",
		"MSG_YOU_GOT_HIT_BY_REFLECTED_MUNTIONS",
		"MSG_YOU_REFLECTED_MUNITIONS",
		"MSG_YOU_INFLICTED_AR_SHRAPNEL",
		"MSG_YOU_GOT_HIT_BY_AR_SHRAPNEL",
		"SUPER_USER_REJECT_MESSAGE",
		"ADMIN_SUCCESS_MESSAGE",
		"ADMIN_FAILURE_MESSAGE",
		"UNIMPLEMENTED_MESSAGE",
			};
		bool is_int(std::string key){
			return (std::find(int_types.begin(),int_types.end(),key) != int_types.end());
		}
		bool is_uint8(std::string key){
			return (std::find(uint8_types.begin(),uint8_types.end(),key) != uint8_types.end());
		}
		bool is_uint16(std::string key){
			return (std::find(uint16_types.begin(),uint16_types.end(),key) != uint16_types.end());
		}
		bool is_string(std::string key){
			return (std::find(string_types.begin(),string_types.end(),key) != string_types.end());
		}
		bool is_float(std::string key){
			return (std::find(float_types.begin(),float_types.end(),key) != float_types.end());
		}

		void revert_to_default(std::string in_key){
			alt_value_map.erase(in_key);
		}
		void save_to_lmdb(std::string key,std::string value){
			mods::db::lmdb_renew();
			std::string lmdb_key = db_key({"values",key});
			mods::db::lmdb_put(lmdb_key,value);
			mods::db::lmdb_commit();
		}

		void load_from_lmdb(std::string in_key){
			mods::db::lmdb_renew();
			std::string key = db_key({"values",in_key});
			m_debug("load from lmdb: '" << key << "'");
			if(!key_exists(key)){
				m_debug(key << " doesn't exist... returning...");
				mods::db::lmdb_commit();
				return;
			}
			std::string value = mods::db::lmdb_get(key);
			if(is_int(in_key)){
				alt_value_map[in_key] = static_cast<int>(atoi(value.c_str()));
				return;
			}
			if(is_uint8(in_key)){
				alt_value_map[in_key] = static_cast<uint8_t>(atoi(value.c_str()));
				return;
			}
			if(is_uint16(in_key)){
				alt_value_map[in_key] = static_cast<uint16_t>(atoi(value.c_str()));
				m_debug(key << " uint16_t exists.... value:'" << value.c_str() << "'");
				return;
			}
			if(is_string(in_key)){
				alt_value_map[in_key] = value;
				return;
			}
			if(is_float(key)){
				alt_value_map[in_key] = static_cast<float>(atof(value.c_str()));
				return;
			}
			log("[WARNING] key doesnt exist in int or string types!->'%s'",key);
			mods::db::lmdb_commit();
		}

		std::string random_key_string(std::string value){
					std::vector<std::string> strings;
					std::string current = "";
					for(auto ch : value){
						if(ch == '|' && current.length()){
							strings.emplace_back(current);
							current = "";
							continue;
						}
						current += ch;
					}
					if(current.length()){
						strings.emplace_back(current);
					}
				return strings[rand_number(0,strings.size()-1)];
		}

#if 0
		value_setters[#KEY] = [&alt_value_map](std::string value){\
			std::string type = #A;\
			if(type.compare("int") == 0 || type.compare("int") == 0 || type.compare("int") == 0){\
				alt_value_map[#KEY] = atoi(value.c_str());\
			}\
			if(type.compare("std::string") == 0){\
				alt_value_map[#KEY] = value;\
			}\
		};
#endif
#define CGET_DEF(A,KEY,MENTOC_DEFAULT)\
		A KEY(){\
			return CONSTGET<A>(#KEY,MENTOC_DEFAULT);\
		}

		CGET_DEF(int,FIRE_DAMAGE_TICK_RESOLUTION,30);
		CGET_DEF(int,FLASHBANG_COOLDOWN_TICKS,3);
		CGET_DEF(int,FIRE_EVERY_N_TICKS,400);
		CGET_DEF(int,FIRE_WOODEN_ADDITIONAL_TICKS,80);
		CGET_DEF(int,FIRE_CARPET_ADDITIONAL_TICKS,60);
		CGET_DEF(int,DAMAGE_DIVISOR,3);
		CGET_DEF(int,FRAG_GRENADE_TICKS,6);
		CGET_DEF(int,EMP_GRENADE_TICKS,6);
		CGET_DEF(int,SENSOR_GRENADE_TICKS,6);
		CGET_DEF(int,FLASHBANG_GRENADE_TICKS,6);
		CGET_DEF(int,SMOKE_GRENADE_TICKS,6);
		CGET_DEF(int,INCENDIARY_GRENADE_TICKS,6);
		CGET_DEF(int,FIRE_STATUS_KINDLING_DAMAGE,10);
		CGET_DEF(int,FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE,70);
		CGET_DEF(int,FIRE_STATUS_SMOLDERING_DAMAGE,40);
		CGET_DEF(int,FIRE_STATUS_SMOKING_DAMAGE,15);

		CGET_DEF(uint16_t,WSL_SHOTGUN_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SHOTGUN_0_ACCURACY,80);
		CGET_DEF(uint16_t,WSL_SHOTGUN_0_DAMAGE,80);
		CGET_DEF(uint16_t,WSL_SHOTGUN_1_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SHOTGUN_1_ACCURACY,15);
		CGET_DEF(uint16_t,WSL_SHOTGUN_1_DAMAGE,15);
		CGET_DEF(uint16_t,WSL_SHOTGUN_2_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_2_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_2_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SHOTGUN_4_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_AR_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_AR_0_ACCURACY,40);
		CGET_DEF(uint16_t,WSL_AR_0_DAMAGE,40);
		CGET_DEF(uint16_t,WSL_AR_1_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_AR_1_ACCURACY,40);
		CGET_DEF(uint16_t,WSL_AR_1_DAMAGE,40);
		CGET_DEF(uint16_t,WSL_AR_2_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_AR_2_ACCURACY,40);
		CGET_DEF(uint16_t,WSL_AR_2_DAMAGE,40);
		CGET_DEF(uint16_t,WSL_AR_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_AR_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_AR_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_AR_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_AR_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_AR_4_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SMG_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SMG_0_ACCURACY,90);
		CGET_DEF(uint16_t,WSL_SMG_0_DAMAGE,40);
		CGET_DEF(uint16_t,WSL_SMG_1_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SMG_1_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SMG_1_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SMG_2_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SMG_2_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SMG_2_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SMG_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SMG_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SMG_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SMG_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_SMG_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_SMG_4_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_0_ACCURACY,25);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_0_DAMAGE,15);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_1_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_1_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_1_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_2_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_2_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_2_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_MACHINE_PISTOL_4_DAMAGE,0);
		CGET_DEF(uint16_t,HEADSHOT_DIVISOR,3);



		CGET_DEF(uint16_t,WSL_HANDGUN_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_HANDGUN_0_ACCURACY,55);
		CGET_DEF(uint16_t,WSL_HANDGUN_0_DAMAGE,10);
		CGET_DEF(uint16_t,WSL_HANDGUN_1_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_1_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_1_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_2_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_2_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_2_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_HANDGUN_4_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_LMG_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_LMG_0_ACCURACY,40);
		CGET_DEF(uint16_t,WSL_LMG_0_DAMAGE,40);
		CGET_DEF(uint16_t,WSL_LMG_1_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_LMG_1_ACCURACY,35);
		CGET_DEF(uint16_t,WSL_LMG_1_DAMAGE,35);
		CGET_DEF(uint16_t,WSL_LMG_2_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_LMG_2_ACCURACY,30);
		CGET_DEF(uint16_t,WSL_LMG_2_DAMAGE,30);
		CGET_DEF(uint16_t,WSL_LMG_3_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_LMG_3_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_LMG_3_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_LMG_4_ALLOWABLE,0);
		CGET_DEF(uint16_t,WSL_LMG_4_ACCURACY,0);
		CGET_DEF(uint16_t,WSL_LMG_4_DAMAGE,0);
		CGET_DEF(uint16_t,WSL_SNIPER_0_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SNIPER_0_ACCURACY,10);
		CGET_DEF(uint16_t,WSL_SNIPER_0_DAMAGE,10);
		CGET_DEF(uint16_t,WSL_SNIPER_1_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SNIPER_1_ACCURACY,35);
		CGET_DEF(uint16_t,WSL_SNIPER_1_DAMAGE,35);
		CGET_DEF(uint16_t,WSL_SNIPER_2_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SNIPER_2_ACCURACY,60);
		CGET_DEF(uint16_t,WSL_SNIPER_2_DAMAGE,60);
		CGET_DEF(uint16_t,WSL_SNIPER_3_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SNIPER_3_ACCURACY,75);
		CGET_DEF(uint16_t,WSL_SNIPER_3_DAMAGE,75);
		CGET_DEF(uint16_t,WSL_SNIPER_4_ALLOWABLE,1);
		CGET_DEF(uint16_t,WSL_SNIPER_4_ACCURACY,75);
		CGET_DEF(uint16_t,WSL_SNIPER_4_DAMAGE,75);
		CGET_DEF(uint16_t, SKILL_CAMERA_PLACEMENT, 50 );
		CGET_DEF(uint16_t, SKILL_DETECT_CAMERAS, 50 );
		CGET_DEF(uint16_t, SKILL_CAMERA_DESTRUCTION, 100 );
		CGET_DEF(uint16_t, SKILL_ELECTRIFIED_DAMAGE, 100 );
		CGET_DEF(uint16_t, SKILL_PROTECT_FROM_EMP, 200 );
		CGET_DEF(uint16_t, SKILL_CREATE_EMP_WEAPONS, 200 );
		CGET_DEF(uint16_t, SKILL_COUNTER_SHOCK, 4 );
		CGET_DEF(uint16_t, SKILL_TOGGLE_THERMAL, 2050 );
		CGET_DEF(uint16_t, SKILL_TOGGLE_NIGHT_VISION, 2050 );
		CGET_DEF(uint16_t, SKILL_HEADGEAR_INTROSPECTION, 3050 );
		CGET_DEF(uint16_t, SKILL_PROXIMITY_ALARM, 3050 );
		CGET_DEF(uint16_t, SKILL_DEPLOYABLE_TURRET, 3050 );
		CGET_DEF(uint16_t, SKILL_DEPLOYABLE_FIRE_TURRET, 3050 );
		CGET_DEF(uint16_t, SKILL_DOOR_CAMERA, 3050 );
		CGET_DEF(uint16_t, SKILL_BASIC_ARMOR, 50 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_ARMOR, 50 );
		CGET_DEF(uint16_t, SKILL_ELITE_ARMOR, 50 );
		CGET_DEF(uint16_t, SKILL_THIEF, 200 );
		CGET_DEF(uint16_t, SKILL_MOLD, 350 );
		CGET_DEF(uint16_t, SKILL_NO_FACTOR_BASIC, 400 );
		CGET_DEF(uint16_t, SKILL_NO_FACTOR_ADVANCED, 850 );
		CGET_DEF(uint16_t, SKILL_NO_FACTOR_ELITE, 900 );
		CGET_DEF(uint16_t, SKILL_EXPLOSIVE_RESISTANCE, 2050 );
		CGET_DEF(uint16_t, SKILL_INJURE_RESISTANCE, 3050 );
		CGET_DEF(uint16_t, SKILL_MUNITIONS_REFLECTOR, 4 );
		CGET_DEF(uint16_t, SKILL_SENTINEL_DISCIPLINE, 3 );
		CGET_DEF(uint16_t, SKILL_DEPLOYABLE_SHIELD, 8050 );
		CGET_DEF(uint16_t, SKILL_SPRAY_CHANCE, 150 );
		CGET_DEF(uint16_t, SKILL_HEADSHOT_CHANCE, 250 );
		CGET_DEF(uint16_t, SKILL_LIMB_CHANCE, 350 );
		CGET_DEF(uint16_t, SKILL_BETTER_SNIPING_ACCURACY, 450 );
		CGET_DEF(uint16_t, SKILL_TARGET_LIMB, 850 );
		CGET_DEF(uint16_t, SKILL_ACCURACY, 150 );
		CGET_DEF(uint16_t, SKILL_VIP, 250 );
		CGET_DEF(uint16_t, SKILL_TRACKER, 350 );
		CGET_DEF(uint16_t, SKILL_QUIETER_SNIPER_SHOTS, 350 );
		CGET_DEF(uint16_t, SKILL_FASTER_BREACHES, 50 );
		CGET_DEF(uint16_t, SKILL_FURTHER_C4S, 4 );
		CGET_DEF(uint16_t, SKILL_STRONGER_FRAG_ARM, 50 );
		CGET_DEF(uint16_t, SKILL_BIGGER_CLAYMORES, 50 );
		CGET_DEF(uint16_t, SKILL_FASTER_THERMITES, 50 );
		CGET_DEF(uint16_t, SKILL_BIGGER_FIRE_NADES, 50 );
		CGET_DEF(uint16_t, SKILL_CRAFTY_C4, 4 );
		CGET_DEF(uint16_t, SKILL_BETTER_EXPLOSIONS, 33 );
		CGET_DEF(uint16_t, SKILL_RETROFIT_GRENADE_LAUNCHER, 50 );
		CGET_DEF(uint16_t, SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER, 50 );
		CGET_DEF(uint16_t, SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER, 50 );
		CGET_DEF(uint16_t, SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER, 50 );
		CGET_DEF(uint16_t, SKILL_RETROFIT_STUN_GRENADE_LAUNCHER, 50 );
		CGET_DEF(uint16_t, SKILL_STEROID_DEALER, 50 );
		CGET_DEF(uint16_t, SKILL_HGH_DEALER, 50 );
		CGET_DEF(uint16_t, SKILL_NOXIOUS_NADE_CRAFTER, 50 );
		CGET_DEF(uint16_t, SKILL_DUTCH_OVEN, 50 );
		CGET_DEF(uint16_t, SKILL_FIRE_NADE_DEALER, 50 );
		CGET_DEF(uint16_t, SKILL_PRO_WEAPON_HANDLER, 50 );
		CGET_DEF(uint16_t, SKILL_BETTER_WEAPON_ACCURACY, 50 );
		CGET_DEF(uint16_t, SKILL_FASTER_TRIGGER_FINGER, 50 );
		CGET_DEF(uint16_t, SKILL_FASTER_RELOADING, 50 );
		CGET_DEF(uint16_t, SKILL_BASIC_PISTOL, 150 );
		CGET_DEF(uint16_t, SKILL_BASIC_ASSAULT_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_BASIC_SHOTGUN, 150 );
		CGET_DEF(uint16_t, SKILL_BASIC_SNIPER_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_BASIC_SUB_MACHINE_GUN, 150 );
		CGET_DEF(uint16_t, SKILL_INTERMEDIATE_PISTOL, 150 );
		CGET_DEF(uint16_t, SKILL_INTERMEDIATE_ASSAULT_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_INTERMEDIATE_SHOTGUN, 150 );
		CGET_DEF(uint16_t, SKILL_INTERMEDIATE_SNIPER_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_INTERMEDIATE_SUB_MACHINE_GUN, 150 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_PISTOL, 150 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_ASSAULT_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_SHOTGUN, 150 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_SNIPER_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_ADVANCED_SUB_MACHINE_GUN, 150 );
		CGET_DEF(uint16_t, SKILL_EXPERT_PISTOL, 150 );
		CGET_DEF(uint16_t, SKILL_EXPERT_ASSAULT_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_EXPERT_SHOTGUN, 150 );
		CGET_DEF(uint16_t, SKILL_EXPERT_SNIPER_RIFLE, 150 );
		CGET_DEF(uint16_t, SKILL_EXPERT_SUB_MACHINE_GUN, 150 );
		CGET_DEF(uint16_t, SKILL_ARMOR_PENETRATION_SHOT, 150 );
		CGET_DEF(uint16_t, SKILL_NONSTOP_PENETRATION_SHOT, 150 );
		CGET_DEF(uint16_t, SKILL_INCREASED_INJURE_CHANCE, 150 );
		CGET_DEF(uint16_t, SKILL_ASSAULT_RIFLE_SHRAPNEL, 150 );
		CGET_DEF(uint16_t, SKILL_ENTRY_DENIAL, 2050 );
		CGET_DEF(uint16_t, SKILL_MISDIRECTION, 2050 );
		CGET_DEF(uint16_t, SKILL_SNIPER_SUPPORT, 2050 );
		CGET_DEF(uint16_t, SKILL_REDUCED_DETECTION_CHANCE, 4 );
		CGET_DEF(uint16_t, SKILL_INCREASED_AWARENESS, 2050 );
		CGET_DEF(uint16_t, SKILL_TRICK_MAGAZINE, 2050 );
		CGET_DEF(uint16_t, SKILL_CHAINABLE_BREACHING, 1 );
		CGET_DEF(uint16_t, SKILL_HGH_MORE_POWERFUL, 2050 );
		CGET_DEF(uint16_t, SKILL_BASIC_HP_RECOVERY, 2050 );
		CGET_DEF(uint16_t, SKILL_PARASITIC_HP_RECOVERY, 2050 );
		CGET_DEF(uint16_t, SKILL_SUTURE, 2050 );
		CGET_DEF(uint16_t, SKILL_ADRENALINE_BOOST, 2050 );

		CGET_DEF(uint8_t,SINGLE_SHOT_SHOTGUN,1);
		CGET_DEF(uint8_t,SINGLE_SHOT_ASSAULT_RIFLE,3);
		CGET_DEF(uint8_t,SINGLE_SHOT_SUB_MACHINE_GUN,3);
		CGET_DEF(uint8_t,SINGLE_SHOT_SNIPER,1);
		CGET_DEF(uint8_t,SINGLE_SHOT_HANDGUN,1);
		CGET_DEF(uint8_t,SINGLE_SHOT_PISTOL,1);
		CGET_DEF(uint8_t,SINGLE_SHOT_MACHINE_PISTOL,6);
		CGET_DEF(uint8_t,SINGLE_SHOT_LIGHT_MACHINE_GUN,8);
		CGET_DEF(uint8_t,SPRAY_SHOT_SHOTGUN,2);
		CGET_DEF(uint8_t,SPRAY_SHOT_ASSAULT_RIFLE,9);
		CGET_DEF(uint8_t,SPRAY_SHOT_SUB_MACHINE_GUN,6);
		CGET_DEF(uint8_t,SPRAY_SHOT_SNIPER,1);
		CGET_DEF(uint8_t,SPRAY_SHOT_HANDGUN,1);
		CGET_DEF(uint8_t,SPRAY_SHOT_PISTOL,1);
		CGET_DEF(uint8_t,SPRAY_SHOT_MACHINE_PISTOL,12);
		CGET_DEF(uint8_t,SPRAY_SHOT_LIGHT_MACHINE_GUN,16);
		CGET_DEF(uint8_t,SPRAY_CHANCE,25);
		CGET_DEF(uint8_t,SPRAY_HEADSHOT_CHANCE,3);
		CGET_DEF(uint8_t,SPRAY_CRITICAL_CHANCE,2);
		CGET_DEF(uint8_t,SPRAY_CRITICAL_REDUCTION_DIVISOR,10);
		CGET_DEF(uint8_t,REVIVE_TICKS,40);
		CGET_DEF(uint8_t,REVIVE_HP,50);
		CGET_DEF(uint8_t,INJURED_HP,5);
		CGET_DEF(uint8_t,MINI_GUNNER_SCAN_DEPTH,5);
		CGET_DEF(uint8_t,SPRAY_CHANCE_SKILL_MODIFIER,17);
		CGET_DEF(uint8_t,HEADSHOT_SKILL_MODIFIER,5);
		CGET_DEF(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_CHANCE,10);
		CGET_DEF(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_COUNT,3);
		CGET_DEF(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_SIDES,30);
		CGET_DEF(uint8_t,SHOW_UNIMPLEMENTED_SKILLS,1);

		CGET_DEF(float,EXPLOSIVE_CRITICAL_MULTIPLIER,0.75);
		CGET_DEF(float,RIFLE_CRITICAL_MULTIPLIER,0.75);
		CGET_DEF(float,INJURED_MAX_HP_MULTIPLIER,0.10);
		CGET_DEF(float,MINI_GUNNER_DECREASED_SIGHT_MULTIPLIER,0.75);
		CGET_DEF(float,CHANCE_TO_INJURE_SKILL_MODIFIER,3.75);
		CGET_DEF(float,INJURE_RESISTANCE_SKILL_MODIFIER,2.75);
		CGET_DEF(float,ASSAULT_RIFLE_SHRAPNEL_SKILL_DAMAGE_MULTIPLIER,0.33);

		CGET_DEF(std::string,SANITY_CHECK,"sanity-check");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_LIGHT,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_FINGER_R,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_FINGER_L,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_NECK_1,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_NECK_2,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_BODY,"objects/armor/vanguard-spiked-vest.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_HEAD,"objects/armor/ballistic-helmet.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_LEGS,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_FEET,"objects/armor/phagia-corps-ballistic-boots.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_HANDS,"objects/armor/weight-lifting-gloves.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_ARMS,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_SHIELD,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_ABOUT,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_WAIST,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_WRIST_R,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_WRIST_L,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_PRIMARY,"objects/rifle/belt-fed-minigun.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_WIELD,"objects/rifle/belt-fed-minigun.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_HOLD,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_SECONDARY_WEAPON,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_SECONDARY,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_WEAPON_ATTACHMENT,"objects/attachment/belt-feeder.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_SHOULDERS_L,"objects/armor/vanguard-shoulder-pad.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_SHOULDERS_R,"objects/armor/vanguard-shoulder-pad.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_BACKPACK,"objects/armor/ammo-backpack.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_GOGGLES,"objects/armor/ballistic-eye-gaurd.yml");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_VEST_PACK,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_ELBOW_L,"");
		CGET_DEF(std::string,MINI_GUNNER_WEAR_ELBOW_R,"objects/armor/vanguard-elbow-guard.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_LIGHT,"");
		CGET_DEF(std::string,LUNATIC_WEAR_FINGER_R,"");
		CGET_DEF(std::string,LUNATIC_WEAR_FINGER_L,"objects/armor/spiked-skull-ring.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_NECK_1,"objects/armor/qi-wol-clock-chain.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_NECK_2,"");
		CGET_DEF(std::string,LUNATIC_WEAR_BODY,"objects/armor/weak-tanktop.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_HEAD,"objects/armor/generic-hat.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_LEGS,"objects/armor/generic-red-jeans.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_FEET,"objects/armor/iconic-shoes.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_HANDS,"");
		CGET_DEF(std::string,LUNATIC_WEAR_ARMS,"");
		CGET_DEF(std::string,LUNATIC_WEAR_SHIELD,"");
		CGET_DEF(std::string,LUNATIC_WEAR_ABOUT,"");
		CGET_DEF(std::string,LUNATIC_WEAR_WAIST,"objects/armor/plain-black-belt.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_WRIST_R,"");
		CGET_DEF(std::string,LUNATIC_WEAR_WRIST_L,"");
		CGET_DEF(std::string,LUNATIC_WEAR_PRIMARY,"objects/rifle/uzi-machine-pistol.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_WIELD,"objects/rifle/uzi-machine-pistol.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_HOLD,"");
		CGET_DEF(std::string,LUNATIC_WEAR_SECONDARY_WEAPON,"");
		CGET_DEF(std::string,LUNATIC_WEAR_SECONDARY,"");
		CGET_DEF(std::string,LUNATIC_WEAR_WEAPON_ATTACHMENT,"objects/attachment/high-velocity-magazine.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_SHOULDERS_L,"");
		CGET_DEF(std::string,LUNATIC_WEAR_SHOULDERS_R,"");
		CGET_DEF(std::string,LUNATIC_WEAR_BACKPACK,"");
		CGET_DEF(std::string,LUNATIC_WEAR_GOGGLES,"objects/armor/idiots-sunglasses.yml");
		CGET_DEF(std::string,LUNATIC_WEAR_VEST_PACK,"");
		CGET_DEF(std::string,LUNATIC_WEAR_ELBOW_L,"");
		CGET_DEF(std::string,LUNATIC_WEAR_ELBOW_R,"");
		CGET_DEF(std::string,MSG_CRITICAL,"{red}***CRITICAL***{/red} --");
		CGET_DEF(std::string,MSG_HEADSHOT,"{red}***HEADSHOT***{/red} -- ");
		CGET_DEF(std::string,MSG_HIT_BY_HEADSHOT,"{red}*** YOU'VE BEEN HEADSHOTTED ***{/red} -- ");
		CGET_DEF(std::string,MSG_HIT,"{yel}--[HIT]--{/yel}");
		CGET_DEF(std::string,MSG_OUT_OF_AMMO,"{gld}*CLICK*{/gld} Your weapon is out of ammo!");
		CGET_DEF(std::string,MSG_MISSED_TARGET ,"You missed your target!");
		CGET_DEF(std::string,MSG_TARGET_DEAD ,"It appears that your target is dead.");
		CGET_DEF(std::string,MSG_YOURE_INJURED ,"You are injured!");
		CGET_DEF(std::string,MSG_FIRE_DAMAGE ,"[%d] You suffer burns from the roaring fire!\r\n");
		CGET_DEF(std::string,MSG_SMOKE_DAMAGE ,"[%d] You struggle to breath as smoke fills your lungs!\r\n");
		CGET_DEF(std::string,MSG_NARROWLY_MISSED_ME,"You just barely get out of the way of a potentially fatal shot!\r\n");
		CGET_DEF(std::string,MSG_YOU_ARE_INJURED,"{red}***YOU ARE INJURED***{/red} --");
		CGET_DEF(std::string,MSG_YOUR_TARGET_IS_DEAD,"{grn}*** YOUR TARGET IS DEAD ***{/grn}\r\n");
		CGET_DEF(std::string,MSG_HIT_BY_RIFLE_ATTACK,"{red}*** YOUR ARE HIT ***{/red} -- ");
		CGET_DEF(std::string,MSG_HIT_BY_SPRAY_ATTACK,"{red}*** YOUR ARE HIT BY A SPRAY OF BULLETS ***{/red} -- ");
		CGET_DEF(std::string,MSG_TARGET_IN_PEACEFUL_ROOM,"{gld}Your target is in a peaceful room{/gld}");
		CGET_DEF(std::string,MSG_NO_PRIMARY_WIELDED,"{gld}You aren't wielding a primary weapon.{/gld}");
		CGET_DEF(std::string,MSG_COOLDOWN_IN_EFFECT,"{gld}Weapon cooldown in effect.{/gld}");
		CGET_DEF(std::string,MSG_COULDNT_FIND_TARGET,"You couldn't find your target!");
		CGET_DEF(std::string,MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS,mini_gunner_random_attack);
		CGET_DEF(std::string,MINI_GUNNER_RANDOM_DISORIENT_STRINGS,mini_gunner_random_disorient_stings);
		CGET_DEF(std::string,MSG_YOU_INJURED_SOMEONE ,"{red}*** YOU INJURED %s{/red}!");
		CGET_DEF(std::string,DEFAULT_PUBLIC_CHANNELS,default_public_channels);
		CGET_DEF(std::string,EXTENDED_PREFERENCES,IMPLODE({"nochat","nogossip","nonewbie","nogratz"},"|"));
		CGET_DEF(std::string,EXTENDED_PREFERENCES_DEFAULTS,IMPLODE({"0","0","0","0"},"|"));
		CGET_DEF(std::string,SUPER_USERS_LIST,IMPLODE({"far"},"|"));
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION,"A bolt-action semi-automatic sniper rifle.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION,"A reliable semi-automatic sniper rifle that works well in very cold conditions.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION,"Standard issue marine corps automatic rifle.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION,"Lighter and shorter variant of the M16A2.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION,"Widely used in SWAT teams across the world, the MP5 is *the* SMG for tactical operations.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION,"Semi-automatic shotgun with large spread, but limited range.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION,"A dual-mode shotgun with pump-action and semi-automatic modes.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION,"Assault rifle known for it's high rate of fire.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_FMG9_DESCRIPTION,"A new prototype of foldable machine guns.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_P90_DESCRIPTION,"Assault rifle known for it's high rate of fire.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_AUGPARA_DESCRIPTION,"A sub-machine-gun form of the popular AUG-A3 assault rifle.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_TAR21_DESCRIPTION,"The TAR-21 is a reliable assault rifle with two firing modes.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_SCARH_DESCRIPTION,"Gas-operated assault rifle used by SOCOM operatives.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_UMP45_DESCRIPTION,"Dubbed the 'Universal Sub Machinegun', the UMP-45 is a versatile and reliable SMG.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_MK46_DESCRIPTION,"The MK-46 LMG is a perfect balance of firepower, portability, and accuracy.\r\n");
		CGET_DEF(std::string,PRIMARY_CHOICE_SCREEN_HK21_DESCRIPTION,"General purpose LMG widely used in tactical teams all over the world.\r\n");
		CGET_DEF(std::string,CHARGEN_SNIPER_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Sniper ] = -              #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "The sniper is a specialty class that \r\n",
							 "focuses on the SNIPER RIFLE weapon type. While any\r\n",
							 "class can wield and use a sniper rifle, the sniper has\r\n",
							 "extra proficiencies and perks associated with the use\r\n",
							 "of ranged weaponry.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
			 )
		);
		CGET_DEF(std::string,CHARGEN_MARINE_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Marine ] = -              #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "A well-rounded soldier built for many\r\n",
							 "tasks. The marine utilizes every aspect of warfare\r\n",
							 "and composes destructive technology for use against\r\n",
							 "insurgents. The ideal candidate is always itching\r\n",
							 "to get back into the battlefield. Marines have their\r\n",
							 "own specialized martial arts training program which\r\n",
							 "unlocks special melee and lethal weapon abilities.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
				)
			);
		CGET_DEF(std::string,CHARGEN_SENTINEL_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Sentinel ] = -             #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "The sentinel is your ideal security \r\n",
							 "task force. The longer you secure an area, the \r\n",
							 "more perks you unlock. This occurs by strategically \r\n",
							 "utilizing technology to impose your will upon the enemy.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
							 )
		);
		CGET_DEF(std::string,CHARGEN_CONTAGION_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Contagion ] = -           #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "Highly trained in the art of chemical \r\n",
							 "warfare, the Contagion takes chemistry to unprecedented\r\n",
							 "levels in order to support and clear out forces behind \r\n",
							 "enemy lines. A strategically placed detonation can be\r\n",
							 "the determining factor in wiping out resistance in the \r\n",
							 "local vicinity.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
				)
		);
		CGET_DEF(std::string,CHARGEN_ENGINEER_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Engineer ] = -              #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "Skilled in advanced methods of offensive \r\n",
							 "electronics, the Engineer supplies his squad with\r\n",
							 "useful intelligence. A carefully \r\n",
							 "placed Engineer can subvert radio communications and\r\n",
							 "cause the opposing force to disrupt or even harm one \r\n",
							 "another by leveraging advanced hacking techniques.\r\n",
							 "The Engineer can employ offensive air, land, or water \r\n",
							 "autonomous drones to discover, steal, and disrupt the\r\n",
							 "opposing force's well-guarded secrets.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
			)
		);
		CGET_DEF(std::string,CHARGEN_MEDIC_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The Medic ] = -               #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "The only class with the ability to \r\n",
							 "revive downed teammates from a few rooms away.\r\n",
							 "The Medic can bandage and repair broken limbs. Squads \r\n",
							 "with a Medic in their team gain a passive HP regen\r\n",
							 "bonus and will take less damage.\r\n",
							 yellow_str(
							 "=====================================================\r\n")
				)
		);
		CGET_DEF(std::string,CHARGEN_PSYOP_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ The PSYOP ] = -               #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "PSYOP specialists utilize techniques\r\n"
							 "of advanced warfare that simply can't be explained. \r\n"
							 "The techniques employed by a PSYOP involve the use of\r\n"
							 "inexplicably advanced technology.\r\n",
							 red_str("This is an advanced class.\r\n"),
							 yellow_str(
							 "====================================================\r\n")
				)
		);
		CGET_DEF(std::string,CHARGEN_SUPPORT_CLASS_DESCRIPTION,
				CAT(green_str(
							 "=====================================================\r\n"
						   "#               - = [ Support ] = -                 #\r\n"
							 "=====================================================\r\n"),
							 yellow_str("[description]:"),
							 "Operatives in the Supporting role can\r\n"
							 "carry an obscene amount of equipment long distances. \r\n"
							 "They can revitalize wounded soldiers with medkits,\r\n"
							 "or supply ammo to fellow squad members. A support \r\n"
							 "specialist has a natural affinity for the light machine gun\r\n"
							 "weapon type, and can surpress an enemy while team mates advance.\r\n",
							 yellow_str(
							 "===============================================================\r\n")
				)
		);
		CGET_DEF(std::string,CHARGEN_UNDEFINED_CLASS_DESCRIPTION,red_str("huh? that's not a class...\r\n"));
		CGET_DEF(std::string,MSG_YOU_GOT_HIT_BY_REFLECTED_MUNTIONS,"*** [ You are HIT by {red} REFLECTED MUNITIONS {/red}] *** ");
		CGET_DEF(std::string,MSG_YOU_REFLECTED_MUNITIONS,"*** [ You {grn} REFLECTED part of that attack {/grn}] *** ");
		CGET_DEF(std::string,MSG_YOU_INFLICTED_AR_SHRAPNEL,"{grn}*** [ You INFLICTED A.R. ::SHRAPNEL:: ] *** {/grn}");
		CGET_DEF(std::string,MSG_YOU_GOT_HIT_BY_AR_SHRAPNEL,"{red}*** [ You got hit by A.R. ::SHRAPNEL:: ] *** {/red}");
		CGET_DEF(std::string,SUPER_USER_REJECT_MESSAGE,"{red}You are not a super user.\r\nA man with no teeth spits on you.{/red}");
		CGET_DEF(std::string,ADMIN_SUCCESS_MESSAGE,"{grn}[+] It is done.{/grn}");
		CGET_DEF(std::string,ADMIN_FAILURE_MESSAGE,"{red}[-] Failure.{/red}");
		CGET_DEF(std::string,UNIMPLEMENTED_MESSAGE,"this command is unimplemented");
#undef CGET_DEF
};
