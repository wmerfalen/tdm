#ifndef __MENTOC_MODS_VALUES_HEADER__
#define __MENTOC_MODS_VALUES_HEADER__
#include <iostream>
#include <map>
#include <variant>
#include "damage-event.hpp"
extern std::string IMPLODE(std::vector<std::string>,std::string);

namespace mods::values {
		using variants_t = std::variant<float,int,uint8_t,uint16_t,std::string>;
		extern std::map<std::string,variants_t> alt_value_map;
		void save_to_lmdb(std::string key,std::string value);
		void load_from_lmdb(std::string in_key);
		void revert_to_default(std::string in_key);
		template <typename T>
		static inline void CONSTSET(std::string m,T value){
			alt_value_map[m] = value;
		}
		template <typename T>
		static inline T CONSTGET(std::string m,T fallback){
			if(alt_value_map.find(m) != alt_value_map.end()){
				return std::get<T>(alt_value_map[m]);
			}
			return fallback;
		}
		std::string random_key_string(std::string value);
#ifndef CGET
#define CGET(A,KEY) \
		A KEY();
#endif
		CGET(int,FIRE_DAMAGE_TICK_RESOLUTION);
		CGET(int,FLASHBANG_COOLDOWN_TICKS);
		CGET(int,FIRE_EVERY_N_TICKS);
		CGET(int,FIRE_WOODEN_ADDITIONAL_TICKS);
		CGET(int,FIRE_CARPET_ADDITIONAL_TICKS);
		CGET(int,DAMAGE_DIVISOR);
		CGET(int,FRAG_GRENADE_TICKS);
		CGET(int,EMP_GRENADE_TICKS);
		CGET(int,SENSOR_GRENADE_TICKS);
		CGET(int,FLASHBANG_GRENADE_TICKS);
		CGET(int,SMOKE_GRENADE_TICKS);
		CGET(int,INCENDIARY_GRENADE_TICKS);
		CGET(int,FIRE_STATUS_KINDLING_DAMAGE);
		CGET(int,FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE);
		CGET(int,FIRE_STATUS_SMOLDERING_DAMAGE);
		CGET(int,FIRE_STATUS_SMOKING_DAMAGE);
		CGET(int,ADVANCE_LEVEL_BONUS_HP_DICE);
		CGET(int,ADVANCE_LEVEL_BONUS_HP_SIDES);
		CGET(int,ADVANCE_LEVEL_BONUS_HP_THRESHOLD);

		CGET(uint16_t,WSL_SHOTGUN_0_ALLOWABLE);
		CGET(uint16_t,WSL_SHOTGUN_0_ACCURACY);
		CGET(uint16_t,WSL_SHOTGUN_0_DAMAGE);
		CGET(uint16_t,WSL_SHOTGUN_1_ALLOWABLE);
		CGET(uint16_t,WSL_SHOTGUN_1_ACCURACY);
		CGET(uint16_t,WSL_SHOTGUN_1_DAMAGE);
		CGET(uint16_t,WSL_SHOTGUN_2_ALLOWABLE);
		CGET(uint16_t,WSL_SHOTGUN_2_ACCURACY);
		CGET(uint16_t,WSL_SHOTGUN_2_DAMAGE);
		CGET(uint16_t,WSL_SHOTGUN_3_ALLOWABLE);
		CGET(uint16_t,WSL_SHOTGUN_3_ACCURACY);
		CGET(uint16_t,WSL_SHOTGUN_3_DAMAGE);
		CGET(uint16_t,WSL_SHOTGUN_4_ALLOWABLE);
		CGET(uint16_t,WSL_SHOTGUN_4_ACCURACY);
		CGET(uint16_t,WSL_SHOTGUN_4_DAMAGE);
		CGET(uint16_t,WSL_AR_0_ALLOWABLE);
		CGET(uint16_t,WSL_AR_0_ACCURACY);
		CGET(uint16_t,WSL_AR_0_DAMAGE);
		CGET(uint16_t,WSL_AR_1_ALLOWABLE);
		CGET(uint16_t,WSL_AR_1_ACCURACY);
		CGET(uint16_t,WSL_AR_1_DAMAGE);
		CGET(uint16_t,WSL_AR_2_ALLOWABLE);
		CGET(uint16_t,WSL_AR_2_ACCURACY);
		CGET(uint16_t,WSL_AR_2_DAMAGE);
		CGET(uint16_t,WSL_AR_3_ALLOWABLE);
		CGET(uint16_t,WSL_AR_3_ACCURACY);
		CGET(uint16_t,WSL_AR_3_DAMAGE);
		CGET(uint16_t,WSL_AR_4_ALLOWABLE);
		CGET(uint16_t,WSL_AR_4_ACCURACY);
		CGET(uint16_t,WSL_AR_4_DAMAGE);
		CGET(uint16_t,WSL_SMG_0_ALLOWABLE);
		CGET(uint16_t,WSL_SMG_0_ACCURACY);
		CGET(uint16_t,WSL_SMG_0_DAMAGE);
		CGET(uint16_t,WSL_SMG_1_ALLOWABLE);
		CGET(uint16_t,WSL_SMG_1_ACCURACY);
		CGET(uint16_t,WSL_SMG_1_DAMAGE);
		CGET(uint16_t,WSL_SMG_2_ALLOWABLE);
		CGET(uint16_t,WSL_SMG_2_ACCURACY);
		CGET(uint16_t,WSL_SMG_2_DAMAGE);
		CGET(uint16_t,WSL_SMG_3_ALLOWABLE);
		CGET(uint16_t,WSL_SMG_3_ACCURACY);
		CGET(uint16_t,WSL_SMG_3_DAMAGE);
		CGET(uint16_t,WSL_SMG_4_ALLOWABLE);
		CGET(uint16_t,WSL_SMG_4_ACCURACY);
		CGET(uint16_t,WSL_SMG_4_DAMAGE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_ALLOWABLE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_ACCURACY);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_DAMAGE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_ALLOWABLE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_ACCURACY);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_DAMAGE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_ALLOWABLE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_ACCURACY);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_DAMAGE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_ALLOWABLE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_ACCURACY);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_DAMAGE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_ALLOWABLE);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_ACCURACY);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_DAMAGE);



		CGET(uint16_t,WSL_HANDGUN_0_ALLOWABLE);
		CGET(uint16_t,WSL_HANDGUN_0_ACCURACY);
		CGET(uint16_t,WSL_HANDGUN_0_DAMAGE);
		CGET(uint16_t,WSL_HANDGUN_1_ALLOWABLE);
		CGET(uint16_t,WSL_HANDGUN_1_ACCURACY);
		CGET(uint16_t,WSL_HANDGUN_1_DAMAGE);
		CGET(uint16_t,WSL_HANDGUN_2_ALLOWABLE);
		CGET(uint16_t,WSL_HANDGUN_2_ACCURACY);
		CGET(uint16_t,WSL_HANDGUN_2_DAMAGE);
		CGET(uint16_t,WSL_HANDGUN_3_ALLOWABLE);
		CGET(uint16_t,WSL_HANDGUN_3_ACCURACY);
		CGET(uint16_t,WSL_HANDGUN_3_DAMAGE);
		CGET(uint16_t,WSL_HANDGUN_4_ALLOWABLE);
		CGET(uint16_t,WSL_HANDGUN_4_ACCURACY);
		CGET(uint16_t,WSL_HANDGUN_4_DAMAGE);
		CGET(uint16_t,WSL_LMG_0_ALLOWABLE);
		CGET(uint16_t,WSL_LMG_0_ACCURACY);
		CGET(uint16_t,WSL_LMG_0_DAMAGE);
		CGET(uint16_t,WSL_LMG_1_ALLOWABLE);
		CGET(uint16_t,WSL_LMG_1_ACCURACY);
		CGET(uint16_t,WSL_LMG_1_DAMAGE);
		CGET(uint16_t,WSL_LMG_2_ALLOWABLE);
		CGET(uint16_t,WSL_LMG_2_ACCURACY);
		CGET(uint16_t,WSL_LMG_2_DAMAGE);
		CGET(uint16_t,WSL_LMG_3_ALLOWABLE);
		CGET(uint16_t,WSL_LMG_3_ACCURACY);
		CGET(uint16_t,WSL_LMG_3_DAMAGE);
		CGET(uint16_t,WSL_LMG_4_ALLOWABLE);
		CGET(uint16_t,WSL_LMG_4_ACCURACY);
		CGET(uint16_t,WSL_LMG_4_DAMAGE);
		CGET(uint16_t,WSL_SNIPER_0_ALLOWABLE);
		CGET(uint16_t,WSL_SNIPER_0_ACCURACY);
		CGET(uint16_t,WSL_SNIPER_0_DAMAGE);
		CGET(uint16_t,WSL_SNIPER_1_ALLOWABLE);
		CGET(uint16_t,WSL_SNIPER_1_ACCURACY);
		CGET(uint16_t,WSL_SNIPER_1_DAMAGE);
		CGET(uint16_t,WSL_SNIPER_2_ALLOWABLE);
		CGET(uint16_t,WSL_SNIPER_2_ACCURACY);
		CGET(uint16_t,WSL_SNIPER_2_DAMAGE);
		CGET(uint16_t,WSL_SNIPER_3_ALLOWABLE);
		CGET(uint16_t,WSL_SNIPER_3_ACCURACY);
		CGET(uint16_t,WSL_SNIPER_3_DAMAGE);
		CGET(uint16_t,WSL_SNIPER_4_ALLOWABLE);
		CGET(uint16_t,WSL_SNIPER_4_ACCURACY);
		CGET(uint16_t,WSL_SNIPER_4_DAMAGE);
		CGET(uint16_t,HEADSHOT_DIVISOR);
		CGET(uint16_t, SKILL_CAMERA_PLACEMENT);
		CGET(uint16_t, SKILL_DETECT_CAMERAS);
		CGET(uint16_t, SKILL_CAMERA_DESTRUCTION);
		CGET(uint16_t, SKILL_ELECTRIFIED_DAMAGE);
		CGET(uint16_t, SKILL_PROTECT_FROM_EMP);
		CGET(uint16_t, SKILL_CREATE_EMP_WEAPONS);
		CGET(uint16_t, SKILL_COUNTER_SHOCK);
		CGET(uint16_t, SKILL_TOGGLE_THERMAL);
		CGET(uint16_t, SKILL_TOGGLE_NIGHT_VISION);
		CGET(uint16_t, SKILL_HEADGEAR_INTROSPECTION);
		CGET(uint16_t, SKILL_PROXIMITY_ALARM);
		CGET(uint16_t, SKILL_DEPLOYABLE_TURRET);
		CGET(uint16_t, SKILL_DEPLOYABLE_FIRE_TURRET);
		CGET(uint16_t, SKILL_DOOR_CAMERA);
		CGET(uint16_t, SKILL_BASIC_ARMOR);
		CGET(uint16_t, SKILL_ADVANCED_ARMOR);
		CGET(uint16_t, SKILL_ELITE_ARMOR);
		CGET(uint16_t, SKILL_THIEF);
		CGET(uint16_t, SKILL_MOLD);
		CGET(uint16_t, SKILL_NO_FACTOR_BASIC);
		CGET(uint16_t, SKILL_NO_FACTOR_ADVANCED);
		CGET(uint16_t, SKILL_NO_FACTOR_ELITE);
		CGET(uint16_t, SKILL_EXPLOSIVE_RESISTANCE);
		CGET(uint16_t, SKILL_INJURE_RESISTANCE);
		CGET(uint16_t, SKILL_MUNITIONS_REFLECTOR);
		CGET(uint16_t, SKILL_SENTINEL_DISCIPLINE);
		CGET(uint16_t, SKILL_DEPLOYABLE_SHIELD);
		CGET(uint16_t, SKILL_SPRAY_CHANCE);
		CGET(uint16_t, SKILL_HEADSHOT_CHANCE);
		CGET(uint16_t, SKILL_LIMB_CHANCE);
		CGET(uint16_t, SKILL_BETTER_SNIPING_ACCURACY);
		CGET(uint16_t, SKILL_TARGET_LIMB);
		CGET(uint16_t, SKILL_ACCURACY);
		CGET(uint16_t, SKILL_VIP);
		CGET(uint16_t, SKILL_TRACKER);
		CGET(uint16_t, SKILL_QUIETER_SNIPER_SHOTS);
		CGET(uint16_t, SKILL_FASTER_BREACHES);
		CGET(uint16_t, SKILL_FURTHER_C4S);
		CGET(uint16_t, SKILL_STRONGER_FRAG_ARM);
		CGET(uint16_t, SKILL_BIGGER_CLAYMORES);
		CGET(uint16_t, SKILL_FASTER_THERMITES);
		CGET(uint16_t, SKILL_BIGGER_FIRE_NADES);
		CGET(uint16_t, SKILL_CRAFTY_C4);
		CGET(uint16_t, SKILL_BETTER_EXPLOSIONS);
		CGET(uint16_t, SKILL_RETROFIT_GRENADE_LAUNCHER);
		CGET(uint16_t, SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER);
		CGET(uint16_t, SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER);
		CGET(uint16_t, SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER);
		CGET(uint16_t, SKILL_RETROFIT_STUN_GRENADE_LAUNCHER);
		CGET(uint16_t, SKILL_STEROID_DEALER);
		CGET(uint16_t, SKILL_HGH_DEALER);
		CGET(uint16_t, SKILL_NOXIOUS_NADE_CRAFTER);
		CGET(uint16_t, SKILL_DUTCH_OVEN);
		CGET(uint16_t, SKILL_FIRE_NADE_DEALER);
		CGET(uint16_t, SKILL_PRO_WEAPON_HANDLER);
		CGET(uint16_t, SKILL_BETTER_WEAPON_ACCURACY);
		CGET(uint16_t, SKILL_FASTER_TRIGGER_FINGER);
		CGET(uint16_t, SKILL_FASTER_RELOADING);
		CGET(uint16_t, SKILL_BASIC_PISTOL);
		CGET(uint16_t, SKILL_BASIC_ASSAULT_RIFLE);
		CGET(uint16_t, SKILL_BASIC_SHOTGUN);
		CGET(uint16_t, SKILL_BASIC_SNIPER_RIFLE);
		CGET(uint16_t, SKILL_BASIC_SUB_MACHINE_GUN);
		CGET(uint16_t, SKILL_INTERMEDIATE_PISTOL);
		CGET(uint16_t, SKILL_INTERMEDIATE_ASSAULT_RIFLE);
		CGET(uint16_t, SKILL_INTERMEDIATE_SHOTGUN);
		CGET(uint16_t, SKILL_INTERMEDIATE_SNIPER_RIFLE);
		CGET(uint16_t, SKILL_INTERMEDIATE_SUB_MACHINE_GUN);
		CGET(uint16_t, SKILL_ADVANCED_PISTOL);
		CGET(uint16_t, SKILL_ADVANCED_ASSAULT_RIFLE);
		CGET(uint16_t, SKILL_ADVANCED_SHOTGUN);
		CGET(uint16_t, SKILL_ADVANCED_SNIPER_RIFLE);
		CGET(uint16_t, SKILL_ADVANCED_SUB_MACHINE_GUN);
		CGET(uint16_t, SKILL_EXPERT_PISTOL);
		CGET(uint16_t, SKILL_EXPERT_ASSAULT_RIFLE);
		CGET(uint16_t, SKILL_EXPERT_SHOTGUN);
		CGET(uint16_t, SKILL_EXPERT_SNIPER_RIFLE);
		CGET(uint16_t, SKILL_EXPERT_SUB_MACHINE_GUN);
		CGET(uint16_t, SKILL_ARMOR_PENETRATION_SHOT);
		CGET(uint16_t, SKILL_NONSTOP_PENETRATION_SHOT);
		CGET(uint16_t, SKILL_INCREASED_INJURE_CHANCE);
		CGET(uint16_t, SKILL_ASSAULT_RIFLE_SHRAPNEL);
		CGET(uint16_t, SKILL_ENTRY_DENIAL);
		CGET(uint16_t, SKILL_MISDIRECTION);
		CGET(uint16_t, SKILL_SNIPER_SUPPORT);
		CGET(uint16_t, SKILL_REDUCED_DETECTION_CHANCE);
		CGET(uint16_t, SKILL_INCREASED_AWARENESS);
		CGET(uint16_t, SKILL_TRICK_MAGAZINE);
		CGET(uint16_t, SKILL_CHAINABLE_BREACHING);
		CGET(uint16_t, SKILL_HGH_MORE_POWERFUL);
		CGET(uint16_t, SKILL_BASIC_HP_RECOVERY);
		CGET(uint16_t, SKILL_PARASITIC_HP_RECOVERY);
		CGET(uint16_t, SKILL_SUTURE);
		CGET(uint16_t, SKILL_ADRENALINE_BOOST);
		CGET(uint16_t, GHOST_FEIGN_DEATH_BLOCKED_FOR_TICKS);
		CGET(uint16_t,PYREXIA_FIRE_NADE_RECHARGE_TICKS_INITIATE);
		CGET(uint16_t,PYREXIA_FIRE_NADE_RECHARGE_TICKS_FAMILIAR);
		CGET(uint16_t,PYREXIA_FIRE_NADE_RECHARGE_TICKS_MASTER);
	 	CGET(uint16_t,GHOST_PLANT_CLAYMORE_INITIATE_MOD_CALL_COUNT);
	 	CGET(uint16_t,GHOST_PLANT_CLAYMORE_FAMILIAR_MOD_CALL_COUNT);
	 	CGET(uint16_t,GHOST_PLANT_CLAYMORE_MASTER_MOD_CALL_COUNT);
CGET(uint16_t,SKILL_GHOST_DRONE_SCAN);
CGET(uint16_t,SKILL_GHOST_STEALTH);
CGET(uint16_t,SKILL_GHOST_SUMMON_EXTRACTION);
CGET(uint16_t,SKILL_GHOST_XRAY_SHOT);
CGET(uint16_t,SKILL_GHOST_FEIGN_DEATH);
CGET(uint16_t,SKILL_GHOST_PLANT_CLAYMORE);
CGET(uint16_t,SKILL_GHOST_PENETRATING_SHOT);
CGET(uint16_t,SKILL_GHOST_INTIMIDATION);
CGET(uint16_t,SKILL_GHOST_CRYOGENIC_GRENADE);
CGET(uint16_t,SKILL_GHOST_FLASH_UNDERBARREL);
			CGET(uint16_t,SKILL_PYREXIA_THROW_FIRE_NADE);
			CGET(uint16_t,SKILL_PYREXIA_DRENCH_IN_GASOLINE);
			CGET(uint16_t,SKILL_PYREXIA_DRENCH_ROOM_IN_GASOLINE);
			CGET(uint16_t,SKILL_PYREXIA_CONVERT_TO_FIRE_NADE);
			CGET(uint16_t,SKILL_PYREXIA_ATTACH_INC_LAUNCHER);
			CGET(uint16_t,SKILL_PYREXIA_ATTACH_FLAMETHROWER);
			CGET(uint16_t,SKILL_PYREXIA_CREATE_WALL_OF_FIRE);
			CGET(uint16_t,SKILL_PYREXIA_SEAL_OFF_ROOM);

		CGET(uint8_t,GHOST_CLASS_TRIADS_MELEE);
		CGET(uint8_t,GHOST_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,GHOST_CLASS_TRIADS_INTEL);
		CGET(uint8_t,GHOST_CLASS_TRIADS_SPEED);
		CGET(uint8_t,GHOST_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,MARKSMAN_CLASS_TRIADS_MELEE);
		CGET(uint8_t,MARKSMAN_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,MARKSMAN_CLASS_TRIADS_INTEL);
		CGET(uint8_t,MARKSMAN_CLASS_TRIADS_SPEED);
		CGET(uint8_t,MARKSMAN_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,BANDIT_CLASS_TRIADS_MELEE);
		CGET(uint8_t,BANDIT_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,BANDIT_CLASS_TRIADS_INTEL);
		CGET(uint8_t,BANDIT_CLASS_TRIADS_SPEED);
		CGET(uint8_t,BANDIT_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,BUTCHER_CLASS_TRIADS_MELEE);
		CGET(uint8_t,BUTCHER_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,BUTCHER_CLASS_TRIADS_INTEL);
		CGET(uint8_t,BUTCHER_CLASS_TRIADS_SPEED);
		CGET(uint8_t,BUTCHER_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,STRIKER_CLASS_TRIADS_MELEE);
		CGET(uint8_t,STRIKER_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,STRIKER_CLASS_TRIADS_INTEL);
		CGET(uint8_t,STRIKER_CLASS_TRIADS_SPEED);
		CGET(uint8_t,STRIKER_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,OBSTRUCTOR_CLASS_TRIADS_MELEE);
		CGET(uint8_t,OBSTRUCTOR_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,OBSTRUCTOR_CLASS_TRIADS_INTEL);
		CGET(uint8_t,OBSTRUCTOR_CLASS_TRIADS_SPEED);
		CGET(uint8_t,OBSTRUCTOR_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,MALADY_CLASS_TRIADS_MELEE);
		CGET(uint8_t,MALADY_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,MALADY_CLASS_TRIADS_INTEL);
		CGET(uint8_t,MALADY_CLASS_TRIADS_SPEED);
		CGET(uint8_t,MALADY_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,PYREXIA_CLASS_TRIADS_MELEE);
		CGET(uint8_t,PYREXIA_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,PYREXIA_CLASS_TRIADS_INTEL);
		CGET(uint8_t,PYREXIA_CLASS_TRIADS_SPEED);
		CGET(uint8_t,PYREXIA_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,DEALER_CLASS_TRIADS_MELEE);
		CGET(uint8_t,DEALER_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,DEALER_CLASS_TRIADS_INTEL);
		CGET(uint8_t,DEALER_CLASS_TRIADS_SPEED);
		CGET(uint8_t,DEALER_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,FORGE_CLASS_TRIADS_MELEE);
		CGET(uint8_t,FORGE_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,FORGE_CLASS_TRIADS_INTEL);
		CGET(uint8_t,FORGE_CLASS_TRIADS_SPEED);
		CGET(uint8_t,FORGE_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,SYNDROME_CLASS_TRIADS_MELEE);
		CGET(uint8_t,SYNDROME_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,SYNDROME_CLASS_TRIADS_INTEL);
		CGET(uint8_t,SYNDROME_CLASS_TRIADS_SPEED);
		CGET(uint8_t,SYNDROME_CLASS_TRIADS_ARMOR);
		CGET(uint8_t,MACHINIST_CLASS_TRIADS_MELEE);
		CGET(uint8_t,MACHINIST_CLASS_TRIADS_WEAPONS);
		CGET(uint8_t,MACHINIST_CLASS_TRIADS_INTEL);
		CGET(uint8_t,MACHINIST_CLASS_TRIADS_SPEED);
		CGET(uint8_t,MACHINIST_CLASS_TRIADS_ARMOR);

		CGET(uint8_t,GHOST_REPLENISH_PULSE);
		CGET(uint8_t,GHOST_CLAYMORE_MAX_COUNT);
		CGET(uint8_t,CAMERA_INSTALLATION_TICS);
		CGET(uint8_t,CLAYMORE_INSTALLATION_TICS);
		CGET(uint8_t,PYREXIA_FIRE_NADE_CHARGES_DEFAULT);
		CGET(uint8_t,PYREXIA_FIRE_NADE_CHARGES_INITIATE);
		CGET(uint8_t,PYREXIA_FIRE_NADE_CHARGES_FAMILIAR);
		CGET(uint8_t,PYREXIA_FIRE_NADE_CHARGES_MASTER);
		CGET(uint8_t,SINGLE_SHOT_SHOTGUN);
		CGET(uint8_t,SINGLE_SHOT_ASSAULT_RIFLE);
		CGET(uint8_t,SINGLE_SHOT_SUB_MACHINE_GUN);
		CGET(uint8_t,SINGLE_SHOT_SNIPER);
		CGET(uint8_t,SINGLE_SHOT_HANDGUN);
		CGET(uint8_t,SINGLE_SHOT_PISTOL);
		CGET(uint8_t,SINGLE_SHOT_MACHINE_PISTOL);
		CGET(uint8_t,SINGLE_SHOT_LIGHT_MACHINE_GUN);
		CGET(uint8_t,SPRAY_SHOT_SHOTGUN);
		CGET(uint8_t,SPRAY_SHOT_ASSAULT_RIFLE);
		CGET(uint8_t,SPRAY_SHOT_SUB_MACHINE_GUN);
		CGET(uint8_t,SPRAY_SHOT_SNIPER);
		CGET(uint8_t,SPRAY_SHOT_HANDGUN);
		CGET(uint8_t,SPRAY_SHOT_PISTOL);
		CGET(uint8_t,SPRAY_SHOT_MACHINE_PISTOL);
		CGET(uint8_t,SPRAY_SHOT_LIGHT_MACHINE_GUN);
		CGET(uint8_t,SPRAY_CHANCE);
		CGET(uint8_t,SPRAY_HEADSHOT_CHANCE);
		CGET(uint8_t,SPRAY_CRITICAL_CHANCE);
		CGET(uint8_t,SPRAY_CRITICAL_REDUCTION_DIVISOR);
		CGET(uint8_t,SPRAY_CHANCE_SKILL_MODIFIER);
		CGET(uint8_t,HEADSHOT_SKILL_MODIFIER);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_CHANCE);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_COUNT);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_SIDES);
		CGET(uint8_t,SHOW_UNIMPLEMENTED_SKILLS);
		CGET(uint8_t,GHOST_FEIGN_DEATH_INITIATE_DURATION);
		CGET(uint8_t,GHOST_FEIGN_DEATH_FAMILIAR_DURATION);
		CGET(uint8_t,GHOST_FEIGN_DEATH_MASTER_DURATION);

		CGET(std::string,CHARGEN_TRIADS_FORMAT);
		CGET(std::string,CAN_BE_SEARCHED);
		CGET(std::string,SANITY_CHECK);
		CGET(std::string,MINI_GUNNER_WEAR_LIGHT);
		CGET(std::string,MINI_GUNNER_WEAR_FINGER_R);
		CGET(std::string,MINI_GUNNER_WEAR_FINGER_L);
		CGET(std::string,MINI_GUNNER_WEAR_NECK_1);
		CGET(std::string,MINI_GUNNER_WEAR_NECK_2);
		CGET(std::string,MINI_GUNNER_WEAR_BODY);
		CGET(std::string,MINI_GUNNER_WEAR_HEAD);
		CGET(std::string,MINI_GUNNER_WEAR_LEGS);
		CGET(std::string,MINI_GUNNER_WEAR_FEET);
		CGET(std::string,MINI_GUNNER_WEAR_HANDS);
		CGET(std::string,MINI_GUNNER_WEAR_ARMS);
		CGET(std::string,MINI_GUNNER_WEAR_SHIELD);
		CGET(std::string,MINI_GUNNER_WEAR_ABOUT);
		CGET(std::string,MINI_GUNNER_WEAR_WAIST);
		CGET(std::string,MINI_GUNNER_WEAR_WRIST_R);
		CGET(std::string,MINI_GUNNER_WEAR_WRIST_L);
		CGET(std::string,MINI_GUNNER_WEAR_PRIMARY);
		CGET(std::string,MINI_GUNNER_WEAR_WIELD);
		CGET(std::string,MINI_GUNNER_WEAR_HOLD);
		CGET(std::string,MINI_GUNNER_WEAR_SECONDARY_WEAPON);
		CGET(std::string,MINI_GUNNER_WEAR_SECONDARY);
		CGET(std::string,MINI_GUNNER_WEAR_WEAPON_ATTACHMENT);
		CGET(std::string,MINI_GUNNER_WEAR_SHOULDERS_L);
		CGET(std::string,MINI_GUNNER_WEAR_SHOULDERS_R);
		CGET(std::string,MINI_GUNNER_WEAR_BACKPACK);
		CGET(std::string,MINI_GUNNER_WEAR_GOGGLES);
		CGET(std::string,MINI_GUNNER_WEAR_VEST_PACK);
		CGET(std::string,MINI_GUNNER_WEAR_ELBOW_L);
		CGET(std::string,MINI_GUNNER_WEAR_ELBOW_R);
		CGET(std::string,MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS);
		CGET(std::string,DEFAULT_PUBLIC_CHANNELS);
		CGET(std::string,MINI_GUNNER_RANDOM_DISORIENT_STRINGS);

		CGET(uint8_t,MINI_GUNNER_SCAN_DEPTH);

		CGET(std::string,LUNATIC_WEAR_LIGHT);
		CGET(std::string,LUNATIC_WEAR_FINGER_R);
		CGET(std::string,LUNATIC_WEAR_FINGER_L);
		CGET(std::string,LUNATIC_WEAR_NECK_1);
		CGET(std::string,LUNATIC_WEAR_NECK_2);
		CGET(std::string,LUNATIC_WEAR_BODY);
		CGET(std::string,LUNATIC_WEAR_HEAD);
		CGET(std::string,LUNATIC_WEAR_LEGS);
		CGET(std::string,LUNATIC_WEAR_FEET);
		CGET(std::string,LUNATIC_WEAR_HANDS);
		CGET(std::string,LUNATIC_WEAR_ARMS);
		CGET(std::string,LUNATIC_WEAR_SHIELD);
		CGET(std::string,LUNATIC_WEAR_ABOUT);
		CGET(std::string,LUNATIC_WEAR_WAIST);
		CGET(std::string,LUNATIC_WEAR_WRIST_R);
		CGET(std::string,LUNATIC_WEAR_WRIST_L);
		CGET(std::string,LUNATIC_WEAR_PRIMARY);
		CGET(std::string,LUNATIC_WEAR_WIELD);
		CGET(std::string,LUNATIC_WEAR_HOLD);
		CGET(std::string,LUNATIC_WEAR_SECONDARY_WEAPON);
		CGET(std::string,LUNATIC_WEAR_SECONDARY);
		CGET(std::string,LUNATIC_WEAR_WEAPON_ATTACHMENT);
		CGET(std::string,LUNATIC_WEAR_SHOULDERS_L);
		CGET(std::string,LUNATIC_WEAR_SHOULDERS_R);
		CGET(std::string,LUNATIC_WEAR_BACKPACK);
		CGET(std::string,LUNATIC_WEAR_GOGGLES);
		CGET(std::string,LUNATIC_WEAR_VEST_PACK);
		CGET(std::string,LUNATIC_WEAR_ELBOW_L);
		CGET(std::string,LUNATIC_WEAR_ELBOW_R);

		CGET(float,LEVELS_FIRST_TIER);
		CGET(float,ADVANCE_LEVEL_HP_FLOAT_MULTIPLIER);
		CGET(float,EXPLOSIVE_CRITICAL_MULTIPLIER);
		CGET(float,RIFLE_CRITICAL_MULTIPLIER);
		CGET(float,INJURED_MAX_HP_MULTIPLIER);
		CGET(float,MINI_GUNNER_DECREASED_SIGHT_MULTIPLIER);
		CGET(float,CHANCE_TO_INJURE_SKILL_MODIFIER);
		CGET(float,INJURE_RESISTANCE_SKILL_MODIFIER);
		/** is multiplied by the damage of the original attack and added as a separate attack */
		CGET(float,ASSAULT_RIFLE_SHRAPNEL_SKILL_DAMAGE_MULTIPLIER);


		CGET(uint8_t,REVIVE_TICKS);
		CGET(uint8_t,REVIVE_HP);
		CGET(uint8_t,INJURED_HP);
		CGET(std::string,MSG_CRITICAL);
		CGET(std::string,MSG_HEADSHOT);
		CGET(std::string,MSG_HIT_BY_HEADSHOT);
		CGET(std::string,MSG_HIT);
		CGET(std::string,MSG_OUT_OF_AMMO);
		CGET(std::string,MSG_MISSED_TARGET );
		CGET(std::string,MSG_TARGET_DEAD );
		CGET(std::string,MSG_YOU_INJURED_SOMEONE );
		CGET(std::string,MSG_YOURE_INJURED );
		CGET(std::string,MSG_FIRE_DAMAGE );
		CGET(std::string,MSG_SMOKE_DAMAGE );
		CGET(std::string,MSG_NARROWLY_MISSED_ME);
		CGET(std::string,MSG_YOU_ARE_INJURED);
		CGET(std::string,MSG_YOUR_TARGET_IS_DEAD);
		CGET(std::string,MSG_HIT_BY_RIFLE_ATTACK);
		CGET(std::string,MSG_HIT_BY_SPRAY_ATTACK);
		CGET(std::string,MSG_TARGET_IN_PEACEFUL_ROOM);
		CGET(std::string,MSG_NO_PRIMARY_WIELDED);
		CGET(std::string,MSG_COOLDOWN_IN_EFFECT);
		CGET(std::string,MSG_COULDNT_FIND_TARGET);
		CGET(std::string,EXTENDED_PREFERENCES);
		CGET(std::string,EXTENDED_PREFERENCES_DEFAULTS);
		CGET(std::string,SUPER_USERS_LIST);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_FMG9_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_P90_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_AUGPARA_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_TAR21_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_SCARH_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_UMP45_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_MK46_DESCRIPTION);
		CGET(std::string,PRIMARY_CHOICE_SCREEN_HK21_DESCRIPTION);
		CGET(std::string,CHARGEN_GHOST_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_MARKSMAN_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_BANDIT_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_BUTCHER_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_STRIKER_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_OBSTRUCTOR_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_MALADY_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_PYREXIA_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_DEALER_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_FORGE_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_SYNDROME_CLASS_DESCRIPTION);
		 	CGET(std::string,CHARGEN_MACHINIST_CLASS_DESCRIPTION);

		CGET(std::string,CHARGEN_UNDEFINED_CLASS_DESCRIPTION);
		CGET(std::string,MSG_YOU_GOT_HIT_BY_REFLECTED_MUNTIONS);
		CGET(std::string,MSG_YOU_REFLECTED_MUNITIONS);
		CGET(std::string,MSG_YOU_INFLICTED_AR_SHRAPNEL);
		CGET(std::string,MSG_YOU_GOT_HIT_BY_AR_SHRAPNEL);
		CGET(std::string,SUPER_USER_REJECT_MESSAGE);
		CGET(std::string,ADMIN_SUCCESS_MESSAGE);
		CGET(std::string,ADMIN_FAILURE_MESSAGE);
		CGET(std::string,UNIMPLEMENTED_MESSAGE);

		CGET(std::string,STOCK_STARS_MESSAGE);
		CGET(std::string,STOCK_BLIND_MESSAGE);
		CGET(std::string,STOCK_PITCH_BLACK_MESSAGE);
		CGET(std::string,STOCK_READ_WHAT_MESSAGE);
		CGET(std::string,STOCK_EXAMINE_MESSAGE);
		CGET(std::string,STOCK_LOOK_INSIDE_MESSAGE);
		CGET(std::string,STOCK_GOLD_BROKE_MESSAGE);
		CGET(std::string,STOCK_GOLD_ONE_MISERABLE_MESSAGE);
		CGET(std::string,STOCK_GOLD_PREFIX_MESSAGE);
		CGET(std::string,STOCK_GOLD_SUFFIX_MESSAGE);

		CGET(std::string,MSG_YOU_FEIGN_DEATH);
		CGET(std::string,MSG_YOU_FAIL_FEIGN_DEATH);
		CGET(std::string,PYREXIA_REPLENISH_MESSAGE);
		CGET(std::string,ADVANCE_LEVEL_CONGRATS);
		namespace msg {
			template <typename T>
			static inline void crit(T& p){
				p->send(MSG_CRITICAL().c_str());
			}
			template <typename T>
			static inline void headshot(T& p){
				p->send(MSG_HEADSHOT().c_str());
			}
			template <typename T>
			static inline void hit(T& p){
				p->send(MSG_HIT().c_str());
			}
			template <typename T>
			static inline void outofammo(T& p){
				p->sendln(MSG_OUT_OF_AMMO().c_str());
			}
			template <typename T>
			static inline void missed(T& p){
				p->sendln(MSG_MISSED_TARGET().c_str());
			}
			template <typename T>
			static inline void target_dead(T& p){
				p->sendln(MSG_TARGET_DEAD().c_str());
			}
			template <typename T>
			static inline void youre_injured(T& p){
				p->sendln(MSG_YOURE_INJURED().c_str());
			}
		};
#undef CGET
};

using namespace mods::values;
#endif
