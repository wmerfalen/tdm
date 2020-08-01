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
