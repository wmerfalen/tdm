#include "values.hpp"
#include "lmdb.hpp"
#include <map>
#include <string>
#include <vector>

extern bool key_exists(std::string key);
namespace mods::values {
		std::map<std::string,variants_t> alt_value_map;
		static const std::vector<std::string> float_types = {
		"EXPLOSIVE_CRITICAL_MULTIPLIER",
		"RIFLE_CRITICAL_MULTIPLIER",
		"INJURED_MAX_HP_MULTIPLIER",
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
		"MINI_GUNNER_SCAN_DEPTH"
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
		};
			static const std::vector<std::string> string_types = {
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
			if(!key_exists(key)){
				std::cerr << "[load_from_lmdb][values.hpp]: not loading since key doesnt exist for '" << in_key << "', dbkey:'" << key << "'\n";
				mods::db::lmdb_commit();
				return;
			}
			std::string value = mods::db::lmdb_get(key);
			if(is_int(key)){
				alt_value_map[key] = static_cast<int>(atoi(value.c_str()));
				return;
			}
			if(is_uint8(key)){
				alt_value_map[key] = static_cast<uint8_t>(atoi(value.c_str()));
				return;
			}
			if(is_uint16(key)){
				alt_value_map[key] = static_cast<uint16_t>(atoi(value.c_str()));
				return;
			}
			if(is_string(key)){
				alt_value_map[key] = value;
				return;
			}
			if(is_float(key)){
				alt_value_map[key] = static_cast<float>(atof(value.c_str()));
				return;
			}
			std::cerr << "[WARNING] key doesnt exist in int or string types!->'" << in_key << "'\n";
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

		CGET_DEF(float,EXPLOSIVE_CRITICAL_MULTIPLIER,0.75);
		CGET_DEF(float,RIFLE_CRITICAL_MULTIPLIER,0.75);
		CGET_DEF(float,INJURED_MAX_HP_MULTIPLIER,0.10);

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
		CGET_DEF(std::string,MSG_YOU_INJURED_SOMEONE ,"{red}*** YOU INJURED %s{/red}!");
#undef CGET_DEF
};
