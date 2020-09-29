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
#define CGET(A,KEY,MENTOC_DEFAULT) \
		A KEY();
#endif
		CGET(int,FIRE_DAMAGE_TICK_RESOLUTION,30);
		CGET(int,FLASHBANG_COOLDOWN_TICKS,3);
		CGET(int,FIRE_EVERY_N_TICKS,400);
		CGET(int,FIRE_WOODEN_ADDITIONAL_TICKS,80);
		CGET(int,FIRE_CARPET_ADDITIONAL_TICKS,60);
		CGET(int,DAMAGE_DIVISOR,3);
		CGET(int,FRAG_GRENADE_TICKS,6);
		CGET(int,EMP_GRENADE_TICKS,6);
		CGET(int,SENSOR_GRENADE_TICKS,6);
		CGET(int,FLASHBANG_GRENADE_TICKS,6);
		CGET(int,SMOKE_GRENADE_TICKS,6);
		CGET(int,INCENDIARY_GRENADE_TICKS,6);
		CGET(int,FIRE_STATUS_KINDLING_DAMAGE,10);
		CGET(int,FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE,70);
		CGET(int,FIRE_STATUS_SMOLDERING_DAMAGE,40);
		CGET(int,FIRE_STATUS_SMOKING_DAMAGE,15);

		CGET(uint16_t,WSL_SHOTGUN_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_SHOTGUN_0_ACCURACY,80);
		CGET(uint16_t,WSL_SHOTGUN_0_DAMAGE,80);
		CGET(uint16_t,WSL_SHOTGUN_1_ALLOWABLE,1);
		CGET(uint16_t,WSL_SHOTGUN_1_ACCURACY,15);
		CGET(uint16_t,WSL_SHOTGUN_1_DAMAGE,15);
		CGET(uint16_t,WSL_SHOTGUN_2_ALLOWABLE,0);
		CGET(uint16_t,WSL_SHOTGUN_2_ACCURACY,0);
		CGET(uint16_t,WSL_SHOTGUN_2_DAMAGE,0);
		CGET(uint16_t,WSL_SHOTGUN_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_SHOTGUN_3_ACCURACY,0);
		CGET(uint16_t,WSL_SHOTGUN_3_DAMAGE,0);
		CGET(uint16_t,WSL_SHOTGUN_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_SHOTGUN_4_ACCURACY,0);
		CGET(uint16_t,WSL_SHOTGUN_4_DAMAGE,0);
		CGET(uint16_t,WSL_AR_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_AR_0_ACCURACY,40);
		CGET(uint16_t,WSL_AR_0_DAMAGE,40);
		CGET(uint16_t,WSL_AR_1_ALLOWABLE,1);
		CGET(uint16_t,WSL_AR_1_ACCURACY,40);
		CGET(uint16_t,WSL_AR_1_DAMAGE,40);
		CGET(uint16_t,WSL_AR_2_ALLOWABLE,1);
		CGET(uint16_t,WSL_AR_2_ACCURACY,40);
		CGET(uint16_t,WSL_AR_2_DAMAGE,40);
		CGET(uint16_t,WSL_AR_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_AR_3_ACCURACY,0);
		CGET(uint16_t,WSL_AR_3_DAMAGE,0);
		CGET(uint16_t,WSL_AR_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_AR_4_ACCURACY,0);
		CGET(uint16_t,WSL_AR_4_DAMAGE,0);
		CGET(uint16_t,WSL_SMG_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_SMG_0_ACCURACY,90);
		CGET(uint16_t,WSL_SMG_0_DAMAGE,40);
		CGET(uint16_t,WSL_SMG_1_ALLOWABLE,0);
		CGET(uint16_t,WSL_SMG_1_ACCURACY,0);
		CGET(uint16_t,WSL_SMG_1_DAMAGE,0);
		CGET(uint16_t,WSL_SMG_2_ALLOWABLE,0);
		CGET(uint16_t,WSL_SMG_2_ACCURACY,0);
		CGET(uint16_t,WSL_SMG_2_DAMAGE,0);
		CGET(uint16_t,WSL_SMG_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_SMG_3_ACCURACY,0);
		CGET(uint16_t,WSL_SMG_3_DAMAGE,0);
		CGET(uint16_t,WSL_SMG_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_SMG_4_ACCURACY,0);
		CGET(uint16_t,WSL_SMG_4_DAMAGE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_ACCURACY,25);
		CGET(uint16_t,WSL_MACHINE_PISTOL_0_DAMAGE,15);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_ALLOWABLE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_ACCURACY,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_1_DAMAGE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_ALLOWABLE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_ACCURACY,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_2_DAMAGE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_ACCURACY,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_3_DAMAGE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_ACCURACY,0);
		CGET(uint16_t,WSL_MACHINE_PISTOL_4_DAMAGE,0);



		CGET(uint16_t,WSL_HANDGUN_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_HANDGUN_0_ACCURACY,55);
		CGET(uint16_t,WSL_HANDGUN_0_DAMAGE,10);
		CGET(uint16_t,WSL_HANDGUN_1_ALLOWABLE,0);
		CGET(uint16_t,WSL_HANDGUN_1_ACCURACY,0);
		CGET(uint16_t,WSL_HANDGUN_1_DAMAGE,0);
		CGET(uint16_t,WSL_HANDGUN_2_ALLOWABLE,0);
		CGET(uint16_t,WSL_HANDGUN_2_ACCURACY,0);
		CGET(uint16_t,WSL_HANDGUN_2_DAMAGE,0);
		CGET(uint16_t,WSL_HANDGUN_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_HANDGUN_3_ACCURACY,0);
		CGET(uint16_t,WSL_HANDGUN_3_DAMAGE,0);
		CGET(uint16_t,WSL_HANDGUN_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_HANDGUN_4_ACCURACY,0);
		CGET(uint16_t,WSL_HANDGUN_4_DAMAGE,0);
		CGET(uint16_t,WSL_LMG_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_LMG_0_ACCURACY,40);
		CGET(uint16_t,WSL_LMG_0_DAMAGE,40);
		CGET(uint16_t,WSL_LMG_1_ALLOWABLE,1);
		CGET(uint16_t,WSL_LMG_1_ACCURACY,35);
		CGET(uint16_t,WSL_LMG_1_DAMAGE,35);
		CGET(uint16_t,WSL_LMG_2_ALLOWABLE,1);
		CGET(uint16_t,WSL_LMG_2_ACCURACY,30);
		CGET(uint16_t,WSL_LMG_2_DAMAGE,30);
		CGET(uint16_t,WSL_LMG_3_ALLOWABLE,0);
		CGET(uint16_t,WSL_LMG_3_ACCURACY,0);
		CGET(uint16_t,WSL_LMG_3_DAMAGE,0);
		CGET(uint16_t,WSL_LMG_4_ALLOWABLE,0);
		CGET(uint16_t,WSL_LMG_4_ACCURACY,0);
		CGET(uint16_t,WSL_LMG_4_DAMAGE,0);
		CGET(uint16_t,WSL_SNIPER_0_ALLOWABLE,1);
		CGET(uint16_t,WSL_SNIPER_0_ACCURACY,10);
		CGET(uint16_t,WSL_SNIPER_0_DAMAGE,10);
		CGET(uint16_t,WSL_SNIPER_1_ALLOWABLE,1);
		CGET(uint16_t,WSL_SNIPER_1_ACCURACY,35);
		CGET(uint16_t,WSL_SNIPER_1_DAMAGE,35);
		CGET(uint16_t,WSL_SNIPER_2_ALLOWABLE,1);
		CGET(uint16_t,WSL_SNIPER_2_ACCURACY,60);
		CGET(uint16_t,WSL_SNIPER_2_DAMAGE,60);
		CGET(uint16_t,WSL_SNIPER_3_ALLOWABLE,1);
		CGET(uint16_t,WSL_SNIPER_3_ACCURACY,75);
		CGET(uint16_t,WSL_SNIPER_3_DAMAGE,75);
		CGET(uint16_t,WSL_SNIPER_4_ALLOWABLE,1);
		CGET(uint16_t,WSL_SNIPER_4_ACCURACY,75);
		CGET(uint16_t,WSL_SNIPER_4_DAMAGE,75);
		CGET(uint16_t,HEADSHOT_DIVISOR,3);
		CGET(uint16_t, SKILL_CAMERA_PLACEMENT, 50 );
		CGET(uint16_t, SKILL_DETECT_CAMERAS, 50 );
		CGET(uint16_t, SKILL_CAMERA_DESTRUCTION, 100 );
		CGET(uint16_t, SKILL_ELECTRIFIED_DAMAGE, 100 );
		CGET(uint16_t, SKILL_PROTECT_FROM_EMP, 200 );
		CGET(uint16_t, SKILL_CREATE_EMP_WEAPONS, 200 );
		CGET(uint16_t, SKILL_COUNTER_SHOCK, 4 );
		CGET(uint16_t, SKILL_TOGGLE_THERMAL, 2050 );
		CGET(uint16_t, SKILL_TOGGLE_NIGHT_VISION, 2050 );
		CGET(uint16_t, SKILL_HEADGEAR_INTROSPECTION, 3050 );
		CGET(uint16_t, SKILL_PROXIMITY_ALARM, 3050 );
		CGET(uint16_t, SKILL_DEPLOYABLE_TURRET, 3050 );
		CGET(uint16_t, SKILL_DEPLOYABLE_FIRE_TURRET, 3050 );
		CGET(uint16_t, SKILL_DOOR_CAMERA, 3050 );
		CGET(uint16_t, SKILL_BASIC_ARMOR, 50 );
		CGET(uint16_t, SKILL_ADVANCED_ARMOR, 50 );
		CGET(uint16_t, SKILL_ELITE_ARMOR, 50 );
		CGET(uint16_t, SKILL_THIEF, 200 );
		CGET(uint16_t, SKILL_MOLD, 350 );
		CGET(uint16_t, SKILL_NO_FACTOR_BASIC, 400 );
		CGET(uint16_t, SKILL_NO_FACTOR_ADVANCED, 850 );
		CGET(uint16_t, SKILL_NO_FACTOR_ELITE, 900 );
		CGET(uint16_t, SKILL_EXPLOSIVE_RESISTANCE, 2050 );
		CGET(uint16_t, SKILL_INJURE_RESISTANCE, 3050 );
		CGET(uint16_t, SKILL_MUNITIONS_REFLECTOR, 4 );
		CGET(uint16_t, SKILL_SENTINEL_DISCIPLINE, 3 );
		CGET(uint16_t, SKILL_DEPLOYABLE_SHIELD, 8050 );
		CGET(uint16_t, SKILL_SPRAY_CHANCE, 150 );
		CGET(uint16_t, SKILL_HEADSHOT_CHANCE, 250 );
		CGET(uint16_t, SKILL_LIMB_CHANCE, 350 );
		CGET(uint16_t, SKILL_BETTER_SNIPING_ACCURACY, 450 );
		CGET(uint16_t, SKILL_TARGET_LIMB, 850 );
		CGET(uint16_t, SKILL_ACCURACY, 150 );
		CGET(uint16_t, SKILL_VIP, 250 );
		CGET(uint16_t, SKILL_TRACKER, 350 );
		CGET(uint16_t, SKILL_QUIETER_SNIPER_SHOTS, 350 );
		CGET(uint16_t, SKILL_FASTER_BREACHES, 50 );
		CGET(uint16_t, SKILL_FURTHER_C4S, 4 );
		CGET(uint16_t, SKILL_STRONGER_FRAG_ARM, 50 );
		CGET(uint16_t, SKILL_BIGGER_CLAYMORES, 50 );
		CGET(uint16_t, SKILL_FASTER_THERMITES, 50 );
		CGET(uint16_t, SKILL_BIGGER_FIRE_NADES, 50 );
		CGET(uint16_t, SKILL_CRAFTY_C4, 4 );
		CGET(uint16_t, SKILL_BETTER_EXPLOSIONS, 33 );
		CGET(uint16_t, SKILL_RETROFIT_GRENADE_LAUNCHER, 50 );
		CGET(uint16_t, SKILL_RETROFIT_FIRE_GRENADE_LAUNCHER, 50 );
		CGET(uint16_t, SKILL_RETROFIT_SMOKE_GRENADE_LAUNCHER, 50 );
		CGET(uint16_t, SKILL_RETROFIT_SENSOR_GRENADE_LAUNCHER, 50 );
		CGET(uint16_t, SKILL_RETROFIT_STUN_GRENADE_LAUNCHER, 50 );
		CGET(uint16_t, SKILL_STEROID_DEALER, 50 );
		CGET(uint16_t, SKILL_HGH_DEALER, 50 );
		CGET(uint16_t, SKILL_NOXIOUS_NADE_CRAFTER, 50 );
		CGET(uint16_t, SKILL_DUTCH_OVEN, 50 );
		CGET(uint16_t, SKILL_FIRE_NADE_DEALER, 50 );
		CGET(uint16_t, SKILL_PRO_WEAPON_HANDLER, 50 );
		CGET(uint16_t, SKILL_BETTER_WEAPON_ACCURACY, 50 );
		CGET(uint16_t, SKILL_FASTER_TRIGGER_FINGER, 50 );
		CGET(uint16_t, SKILL_FASTER_RELOADING, 50 );
		CGET(uint16_t, SKILL_BASIC_PISTOL, 150 );
		CGET(uint16_t, SKILL_BASIC_ASSAULT_RIFLE, 150 );
		CGET(uint16_t, SKILL_BASIC_SHOTGUN, 150 );
		CGET(uint16_t, SKILL_BASIC_SNIPER_RIFLE, 150 );
		CGET(uint16_t, SKILL_BASIC_SUB_MACHINE_GUN, 150 );
		CGET(uint16_t, SKILL_INTERMEDIATE_PISTOL, 150 );
		CGET(uint16_t, SKILL_INTERMEDIATE_ASSAULT_RIFLE, 150 );
		CGET(uint16_t, SKILL_INTERMEDIATE_SHOTGUN, 150 );
		CGET(uint16_t, SKILL_INTERMEDIATE_SNIPER_RIFLE, 150 );
		CGET(uint16_t, SKILL_INTERMEDIATE_SUB_MACHINE_GUN, 150 );
		CGET(uint16_t, SKILL_ADVANCED_PISTOL, 150 );
		CGET(uint16_t, SKILL_ADVANCED_ASSAULT_RIFLE, 150 );
		CGET(uint16_t, SKILL_ADVANCED_SHOTGUN, 150 );
		CGET(uint16_t, SKILL_ADVANCED_SNIPER_RIFLE, 150 );
		CGET(uint16_t, SKILL_ADVANCED_SUB_MACHINE_GUN, 150 );
		CGET(uint16_t, SKILL_EXPERT_PISTOL, 150 );
		CGET(uint16_t, SKILL_EXPERT_ASSAULT_RIFLE, 150 );
		CGET(uint16_t, SKILL_EXPERT_SHOTGUN, 150 );
		CGET(uint16_t, SKILL_EXPERT_SNIPER_RIFLE, 150 );
		CGET(uint16_t, SKILL_EXPERT_SUB_MACHINE_GUN, 150 );
		CGET(uint16_t, SKILL_ARMOR_PENETRATION_SHOT, 150 );
		CGET(uint16_t, SKILL_NONSTOP_PENETRATION_SHOT, 150 );
		CGET(uint16_t, SKILL_INCREASED_INJURE_CHANCE, 150 );
		CGET(uint16_t, SKILL_ASSAULT_RIFLE_SHRAPNEL, 150 );
		CGET(uint16_t, SKILL_ENTRY_DENIAL, 2050 );
		CGET(uint16_t, SKILL_MISDIRECTION, 2050 );
		CGET(uint16_t, SKILL_SNIPER_SUPPORT, 2050 );
		CGET(uint16_t, SKILL_REDUCED_DETECTION_CHANCE, 4 );
		CGET(uint16_t, SKILL_INCREASED_AWARENESS, 2050 );
		CGET(uint16_t, SKILL_TRICK_MAGAZINE, 2050 );
		CGET(uint16_t, SKILL_CHAINABLE_BREACHING, 1 );
		CGET(uint16_t, SKILL_HGH_MORE_POWERFUL, 2050 );
		CGET(uint16_t, SKILL_BASIC_HP_RECOVERY, 2050 );
		CGET(uint16_t, SKILL_PARASITIC_HP_RECOVERY, 2050 );
		CGET(uint16_t, SKILL_SUTURE, 2050 );
		CGET(uint16_t, SKILL_ADRENALINE_BOOST, 2050 );

		CGET(uint8_t,SINGLE_SHOT_SHOTGUN,1);
		CGET(uint8_t,SINGLE_SHOT_ASSAULT_RIFLE,3);
		CGET(uint8_t,SINGLE_SHOT_SUB_MACHINE_GUN,3);
		CGET(uint8_t,SINGLE_SHOT_SNIPER,1);
		CGET(uint8_t,SINGLE_SHOT_HANDGUN,1);
		CGET(uint8_t,SINGLE_SHOT_PISTOL,1);
		CGET(uint8_t,SINGLE_SHOT_MACHINE_PISTOL,6);
		CGET(uint8_t,SINGLE_SHOT_LIGHT_MACHINE_GUN,8);
		CGET(uint8_t,SPRAY_SHOT_SHOTGUN,2);
		CGET(uint8_t,SPRAY_SHOT_ASSAULT_RIFLE,9);
		CGET(uint8_t,SPRAY_SHOT_SUB_MACHINE_GUN,6);
		CGET(uint8_t,SPRAY_SHOT_SNIPER,1);
		CGET(uint8_t,SPRAY_SHOT_HANDGUN,1);
		CGET(uint8_t,SPRAY_SHOT_PISTOL,1);
		CGET(uint8_t,SPRAY_SHOT_MACHINE_PISTOL,12);
		CGET(uint8_t,SPRAY_SHOT_LIGHT_MACHINE_GUN,16);
		CGET(uint8_t,SPRAY_CHANCE,25);
		CGET(uint8_t,SPRAY_HEADSHOT_CHANCE,3);
		CGET(uint8_t,SPRAY_CRITICAL_CHANCE,2);
		CGET(uint8_t,SPRAY_CRITICAL_REDUCTION_DIVISOR,10);
		CGET(uint8_t,SPRAY_CHANCE_SKILL_MODIFIER,17);
		CGET(uint8_t,HEADSHOT_SKILL_MODIFIER,5);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_CHANCE,10);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_COUNT,3);
		CGET(uint8_t,ASSAULT_RIFLE_SHRAPNEL_SKILL_DICE_SIDES,30);
		CGET(uint8_t,SHOW_UNIMPLEMENTED_SKILLS,1);

		CGET(std::string,SANITY_CHECK,"sanity-check");
		CGET(std::string,MINI_GUNNER_WEAR_LIGHT,"");
		CGET(std::string,MINI_GUNNER_WEAR_FINGER_R,"");
		CGET(std::string,MINI_GUNNER_WEAR_FINGER_L,"");
		CGET(std::string,MINI_GUNNER_WEAR_NECK_1,"");
		CGET(std::string,MINI_GUNNER_WEAR_NECK_2,"");
		CGET(std::string,MINI_GUNNER_WEAR_BODY,"objects/armor/vanguard-spiked-vest.yml");
		CGET(std::string,MINI_GUNNER_WEAR_HEAD,"objects/armor/ballistic-helmet.yml");
		CGET(std::string,MINI_GUNNER_WEAR_LEGS,"");
		CGET(std::string,MINI_GUNNER_WEAR_FEET,"objects/armor/phagia-corps-ballistic-boots.yml");
		CGET(std::string,MINI_GUNNER_WEAR_HANDS,"objects/armor/weight-lifting-gloves.yml");
		CGET(std::string,MINI_GUNNER_WEAR_ARMS,"");
		CGET(std::string,MINI_GUNNER_WEAR_SHIELD,"");
		CGET(std::string,MINI_GUNNER_WEAR_ABOUT,"");
		CGET(std::string,MINI_GUNNER_WEAR_WAIST,"");
		CGET(std::string,MINI_GUNNER_WEAR_WRIST_R,"");
		CGET(std::string,MINI_GUNNER_WEAR_WRIST_L,"");
		CGET(std::string,MINI_GUNNER_WEAR_PRIMARY,"objects/rifle/belt-fed-minigun.yml");
		CGET(std::string,MINI_GUNNER_WEAR_WIELD,"objects/rifle/belt-fed-minigun.yml");
		CGET(std::string,MINI_GUNNER_WEAR_HOLD,"");
		CGET(std::string,MINI_GUNNER_WEAR_SECONDARY_WEAPON,"");
		CGET(std::string,MINI_GUNNER_WEAR_SECONDARY,"");
		CGET(std::string,MINI_GUNNER_WEAR_WEAPON_ATTACHMENT,"objects/attachment/belt-feeder.yml");
		CGET(std::string,MINI_GUNNER_WEAR_SHOULDERS_L,"objects/armor/vanguard-shoulder-pad.yml");
		CGET(std::string,MINI_GUNNER_WEAR_SHOULDERS_R,"objects/armor/vanguard-shoulder-pad.yml");
		CGET(std::string,MINI_GUNNER_WEAR_BACKPACK,"objects/armor/ammo-backpack.yml");
		CGET(std::string,MINI_GUNNER_WEAR_GOGGLES,"objects/armor/ballistic-eye-gaurd.yml");
		CGET(std::string,MINI_GUNNER_WEAR_VEST_PACK,"");
		CGET(std::string,MINI_GUNNER_WEAR_ELBOW_L,"");
		CGET(std::string,MINI_GUNNER_WEAR_ELBOW_R,"objects/armor/vanguard-elbow-guard.yml");
		static std::string mini_gunner_random_attack = IMPLODE({"It's a dangerous game!","Have I ever told you the path to insanity?",
				"Come out, come out, wherever you are...",
				"The ones who hate me the most are the ones who don't scare me.",
				"Do you have the slightest clue how easily I could kill you right now?"
				},"|");
		CGET(std::string,MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS,mini_gunner_random_attack);
		static std::string default_public_channels = IMPLODE({"chat","gossip","newbie","gratz"},"|");
		CGET(std::string,DEFAULT_PUBLIC_CHANNELS,default_public_channels);
		static std::string mini_gunner_random_disorient_stings = IMPLODE(
				{
					"My EYES! My EYESSSSSSSS!!!!",
					"Enough of your tricks! Come out and fight me like a MAN!!!",
					"You're only delaying the inevitable!",
					"Tricks?! You're trying to defeat me with... TRICKS!?!"
				},"|");
		CGET(std::string,MINI_GUNNER_RANDOM_DISORIENT_STRINGS,mini_gunner_random_disorient_stings);

		CGET(uint8_t,MINI_GUNNER_SCAN_DEPTH,5);

		CGET(std::string,LUNATIC_WEAR_LIGHT,"");
		CGET(std::string,LUNATIC_WEAR_FINGER_R,"");
		CGET(std::string,LUNATIC_WEAR_FINGER_L,"objects/armor/spiked-skull-ring.yml");
		CGET(std::string,LUNATIC_WEAR_NECK_1,"objects/armor/qi-wol-clock-chain.yml");
		CGET(std::string,LUNATIC_WEAR_NECK_2,"");
		CGET(std::string,LUNATIC_WEAR_BODY,"objects/armor/weak-tanktop.yml");
		CGET(std::string,LUNATIC_WEAR_HEAD,"objects/armor/generic-hat.yml");
		CGET(std::string,LUNATIC_WEAR_LEGS,"objects/armor/generic-red-jeans.yml");
		CGET(std::string,LUNATIC_WEAR_FEET,"objects/armor/iconic-shoes.yml");
		CGET(std::string,LUNATIC_WEAR_HANDS,"");
		CGET(std::string,LUNATIC_WEAR_ARMS,"");
		CGET(std::string,LUNATIC_WEAR_SHIELD,"");
		CGET(std::string,LUNATIC_WEAR_ABOUT,"");
		CGET(std::string,LUNATIC_WEAR_WAIST,"objects/armor/plain-black-belt.yml");
		CGET(std::string,LUNATIC_WEAR_WRIST_R,"");
		CGET(std::string,LUNATIC_WEAR_WRIST_L,"");
		CGET(std::string,LUNATIC_WEAR_PRIMARY,"objects/rifle/uzi-machine-pistol.yml");
		CGET(std::string,LUNATIC_WEAR_WIELD,"objects/rifle/uzi-machine-pistol.yml");
		CGET(std::string,LUNATIC_WEAR_HOLD,"");
		CGET(std::string,LUNATIC_WEAR_SECONDARY_WEAPON,"");
		CGET(std::string,LUNATIC_WEAR_SECONDARY,"");
		CGET(std::string,LUNATIC_WEAR_WEAPON_ATTACHMENT,"objects/attachment/high-velocity-magazine.yml");
		CGET(std::string,LUNATIC_WEAR_SHOULDERS_L,"");
		CGET(std::string,LUNATIC_WEAR_SHOULDERS_R,"");
		CGET(std::string,LUNATIC_WEAR_BACKPACK,"");
		CGET(std::string,LUNATIC_WEAR_GOGGLES,"objects/armor/idiots-sunglasses.yml");
		CGET(std::string,LUNATIC_WEAR_VEST_PACK,"");
		CGET(std::string,LUNATIC_WEAR_ELBOW_L,"");
		CGET(std::string,LUNATIC_WEAR_ELBOW_R,"");

		CGET(float,EXPLOSIVE_CRITICAL_MULTIPLIER,0.75);
		CGET(float,RIFLE_CRITICAL_MULTIPLIER,0.75);
		CGET(float,INJURED_MAX_HP_MULTIPLIER,0.10);
		CGET(float,MINI_GUNNER_DECREASED_SIGHT_MULTIPLIER,0.75);
		CGET(float,CHANCE_TO_INJURE_SKILL_MODIFIER,3.75);
		CGET(float,INJURE_RESISTANCE_SKILL_MODIFIER,2.75);
		/** is multiplied by the damage of the original attack and added as a separate attack */
		CGET(float,ASSAULT_RIFLE_SHRAPNEL_SKILL_DAMAGE_MULTIPLIER,0.33);


		CGET(uint8_t,REVIVE_TICKS,40);
		CGET(uint8_t,REVIVE_HP,50);
		CGET(uint8_t,INJURED_HP,5);
		CGET(std::string,MSG_CRITICAL,"{red}***CRITICAL***{/red} --");
		CGET(std::string,MSG_HEADSHOT,"{red}***HEADSHOT***{/red} -- ");
		CGET(std::string,MSG_HIT_BY_HEADSHOT,"{red}*** YOU'VE BEEN HEADSHOTTED ***{/red} -- ");
		CGET(std::string,MSG_HIT,"{yel}--[HIT]--{/yel}");
		CGET(std::string,MSG_OUT_OF_AMMO,"{gld}*CLICK*{/gld} Your weapon is out of ammo!");
		CGET(std::string,MSG_MISSED_TARGET ,"You missed your target!");
		CGET(std::string,MSG_TARGET_DEAD ,"It appears that your target is dead.");
		CGET(std::string,MSG_YOU_INJURED_SOMEONE ,"{red}*** YOU INJURED %s{/red}!");
		CGET(std::string,MSG_YOURE_INJURED ,"You are injured!");
		CGET(std::string,MSG_FIRE_DAMAGE ,"[%d] You suffer burns from the roaring fire!\r\n");
		CGET(std::string,MSG_SMOKE_DAMAGE ,"[%d] You struggle to breath as smoke fills your lungs!\r\n");
		CGET(std::string,MSG_NARROWLY_MISSED_ME,"You just barely get out of the way of a potentially fatal shot! -- ");
		CGET(std::string,MSG_YOU_ARE_INJURED,"{red}***YOU ARE INJURED***{/red} --");
		CGET(std::string,MSG_YOUR_TARGET_IS_DEAD,"{grn}*** YOUR TARGET IS DEAD ***{/grn}\r\n");
		CGET(std::string,MSG_HIT_BY_RIFLE_ATTACK,"{red}*** YOUR ARE HIT ***{/red} -- ");
		CGET(std::string,MSG_HIT_BY_SPRAY_ATTACK,"{red}*** YOUR ARE HIT BY A SPRAY OF BULLETS ***{/red} -- ");
		CGET(std::string,MSG_TARGET_IN_PEACEFUL_ROOM,"{gld}Your target is in a peaceful room{/gld}");
		CGET(std::string,MSG_NO_PRIMARY_WIELDED,"{gld}You aren't wielding a primary weapon.{/gld}");
		CGET(std::string,MSG_COOLDOWN_IN_EFFECT,"{gld}Weapon cooldown in effect.{/gld}");
		CGET(std::string,MSG_COULDNT_FIND_TARGET,"You couldn't find your target!");
		CGET(std::string,EXTENDED_PREFERENCES,IMPLODE({"nochat","nogossip","nonewbie","nogratz"},'|'));
		CGET(std::string,EXTENDED_PREFERENCES_DEFAULTS,IMPLODE({"0","0","0","0"},'|'));
		CGET(std::string,SUPER_USERS_LIST,IMPLODE({"far"},"|"));
		CGET(std::string,PRIMARY_CHOICE_SCREEN_PSG1_DESCRIPTION,"A bolt-action semi-automatic sniper rifle.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_L96AW_DESCRIPTION,"A reliable semi-automatic sniper rifle that works well in very cold conditions.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M16A4_DESCRIPTION,"Standard issue marine corps automatic rifle.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M4_DESCRIPTION,"Lighter and shorter variant of the M16A2.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_MP5_DESCRIPTION,"Widely used in SWAT teams across the world, the MP5 is *the* SMG for tactical operations.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_SASG12_DESCRIPTION,"Semi-automatic shotgun with large spread, but limited range.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_M3_DESCRIPTION,"A dual-mode shotgun with pump-action and semi-automatic modes.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_FAMAS_DESCRIPTION,"Assault rifle known for it's high rate of fire.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_FMG9_DESCRIPTION,"A new prototype of foldable machine guns.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_P90_DESCRIPTION,"Assault rifle known for it's high rate of fire.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_AUGPARA_DESCRIPTION,"A sub-machine-gun form of the popular AUG-A3 assault rifle.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_TAR21_DESCRIPTION,"The TAR-21 is a reliable assault rifle with two firing modes.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_SCARH_DESCRIPTION,"Gas-operated assault rifle used by SOCOM operatives.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_UMP45_DESCRIPTION,"Dubbed the 'Universal Sub Machinegun', the UMP-45 is a versatile and reliable SMG.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_MK46_DESCRIPTION,"The MK-46 LMG is a perfect balance of firepower, portability, and accuracy.\r\n");
		CGET(std::string,PRIMARY_CHOICE_SCREEN_HK21_DESCRIPTION,"General purpose LMG widely used in tactical teams all over the world.\r\n");
		CGET(std::string,CHARGEN_SNIPER_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_MARINE_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_SENTINEL_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_CONTAGION_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_ENGINEER_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_MEDIC_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_PSYOP_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_SUPPORT_CLASS_DESCRIPTION,
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
		CGET(std::string,CHARGEN_UNDEFINED_CLASS_DESCRIPTION,red_str("huh? that's not a class...\r\n"));
		CGET(std::string,MSG_YOU_GOT_HIT_BY_REFLECTED_MUNTIONS,"{red}*** [ You are HIT by {yel}REFLECTED MUNITIONS{/yel}] *** {/red}");
		CGET(std::string,MSG_YOU_REFLECTED_MUNITIONS,"{grn}*** [ You REFLECTED part of that attack ] *** {/grn}");
		CGET(std::string,MSG_YOU_INFLICTED_AR_SHRAPNEL,"{grn}*** [ You INFLICTED A.R. ::SHRAPNEL:: ] *** {/grn}");
		CGET(std::string,MSG_YOU_GOT_HIT_BY_AR_SHRAPNEL,"{red}*** [ You got hit by A.R. ::SHRAPNEL:: ] *** {/red}");
		CGET(std::string,SUPER_USER_REJECT_MESSAGE,"{red}You are not a super user.\r\nA man with no teeth spits on you.{/red}");
		CGET(std::string,ADMIN_SUCCESS_MESSAGE,"{grn}[+] It is done.{/grn}");
		CGET(std::string,ADMIN_FAILURE_MESSAGE,"{red}[-] Failure.{/red}");
		CGET(std::string,UNIMPLEMENTED_MESSAGE,"{red}this command is unimplemented{/red}");

		CGET(std::string,STOCK_STARS_MESSAGE,"You can't see anything but stars!");
		CGET(std::string,STOCK_BLIND_MESSAGE,"You can't see a damned thing, you're blind!");
		CGET(std::string,STOCK_PITCH_BLACK_MESSAGE,"It is pitch black...");
		CGET(std::string,STOCK_READ_WHAT_MESSAGE,"Read what?");
		CGET(std::string,STOCK_EXAMINE_MESSAGE,"Examine what?");
		CGET(std::string,STOCK_LOOK_INSIDE_MESSAGE, "When you look inside, you see:");
		CGET(std::string,STOCK_GOLD_BROKE_MESSAGE,"You're broke!");
		CGET(std::string,STOCK_GOLD_ONE_MISERABLE_MESSAGE,"You have one miserable little gold coin.");
		CGET(std::string,STOCK_GOLD_PREFIX_MESSAGE,"You have ");
		CGET(std::string,STOCK_GOLD_SUFFIX_MESSAGE," gold coins.");
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
