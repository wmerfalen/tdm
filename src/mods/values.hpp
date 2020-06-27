#ifndef __MENTOC_MODS_VALUES_HEADER__
#define __MENTOC_MODS_VALUES_HEADER__
#include <iostream>

namespace mods::values {
		constexpr static int FLASHBANG_COOLDOWN_TICKS = 3;	/** approx 2 seconds */
		constexpr static int FIRE_EVERY_N_TICKS = 400;
		constexpr static int DAMAGE_DIVISOR = 3;
		constexpr static int FRAG_GRENADE_TICKS = 6;
		constexpr static int EMP_GRENADE_TICKS = 6;
		constexpr static int SENSOR_GRENADE_TICKS = 6;
		constexpr static int FLASHBANG_GRENADE_TICKS = 6;
		constexpr static int SMOKE_GRENADE_TICKS = 6;
		constexpr static int INCENDIARY_GRENADE_TICKS = 6;

		constexpr static uint16_t WSL_SHOTGUN_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SHOTGUN_0_ACCURACY = 80;
		constexpr static uint16_t WSL_SHOTGUN_0_DAMAGE = 80;

		constexpr static uint16_t WSL_SHOTGUN_1_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SHOTGUN_1_ACCURACY = 15;
		constexpr static uint16_t WSL_SHOTGUN_1_DAMAGE = 15;

		constexpr static uint16_t WSL_SHOTGUN_2_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SHOTGUN_2_ACCURACY = 0;
		constexpr static uint16_t WSL_SHOTGUN_2_DAMAGE = 0;

		constexpr static uint16_t WSL_SHOTGUN_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SHOTGUN_3_ACCURACY = 0;
		constexpr static uint16_t WSL_SHOTGUN_3_DAMAGE = 0;

		constexpr static uint16_t WSL_SHOTGUN_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SHOTGUN_4_ACCURACY = 0;
		constexpr static uint16_t WSL_SHOTGUN_4_DAMAGE = 0;

		constexpr static uint16_t WSL_AR_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_AR_0_ACCURACY = 40;
		constexpr static uint16_t WSL_AR_0_DAMAGE = 40;

		constexpr static uint16_t WSL_AR_1_ALLOWABLE = 1;
		constexpr static uint16_t WSL_AR_1_ACCURACY = 40;
		constexpr static uint16_t WSL_AR_1_DAMAGE = 40;

		constexpr static uint16_t WSL_AR_2_ALLOWABLE = 1;
		constexpr static uint16_t WSL_AR_2_ACCURACY = 40;
		constexpr static uint16_t WSL_AR_2_DAMAGE = 40;

		constexpr static uint16_t WSL_AR_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_AR_3_ACCURACY = 0;
		constexpr static uint16_t WSL_AR_3_DAMAGE = 0;

		constexpr static uint16_t WSL_AR_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_AR_4_ACCURACY = 0;
		constexpr static uint16_t WSL_AR_4_DAMAGE = 0;



		constexpr static uint16_t WSL_SMG_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SMG_0_ACCURACY = 90;
		constexpr static uint16_t WSL_SMG_0_DAMAGE = 40;

		constexpr static uint16_t WSL_SMG_1_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SMG_1_ACCURACY = 0;
		constexpr static uint16_t WSL_SMG_1_DAMAGE = 0;

		constexpr static uint16_t WSL_SMG_2_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SMG_2_ACCURACY = 0;
		constexpr static uint16_t WSL_SMG_2_DAMAGE = 0;

		constexpr static uint16_t WSL_SMG_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SMG_3_ACCURACY = 0;
		constexpr static uint16_t WSL_SMG_3_DAMAGE = 0;

		constexpr static uint16_t WSL_SMG_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_SMG_4_ACCURACY = 0;
		constexpr static uint16_t WSL_SMG_4_DAMAGE = 0;


		constexpr static uint16_t WSL_MACHINE_PISTOL_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_MACHINE_PISTOL_0_ACCURACY = 25;
		constexpr static uint16_t WSL_MACHINE_PISTOL_0_DAMAGE = 15;

		constexpr static uint16_t WSL_MACHINE_PISTOL_1_ALLOWABLE = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_1_ACCURACY = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_1_DAMAGE = 0;

		constexpr static uint16_t WSL_MACHINE_PISTOL_2_ALLOWABLE = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_2_ACCURACY = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_2_DAMAGE = 0;

		constexpr static uint16_t WSL_MACHINE_PISTOL_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_3_ACCURACY = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_3_DAMAGE = 0;

		constexpr static uint16_t WSL_MACHINE_PISTOL_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_4_ACCURACY = 0;
		constexpr static uint16_t WSL_MACHINE_PISTOL_4_DAMAGE = 0;



		constexpr static uint16_t WSL_HANDGUN_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_HANDGUN_0_ACCURACY = 55;
		constexpr static uint16_t WSL_HANDGUN_0_DAMAGE = 10;

		constexpr static uint16_t WSL_HANDGUN_1_ALLOWABLE = 0;
		constexpr static uint16_t WSL_HANDGUN_1_ACCURACY = 0;
		constexpr static uint16_t WSL_HANDGUN_1_DAMAGE = 0;

		constexpr static uint16_t WSL_HANDGUN_2_ALLOWABLE = 0;
		constexpr static uint16_t WSL_HANDGUN_2_ACCURACY = 0;
		constexpr static uint16_t WSL_HANDGUN_2_DAMAGE = 0;

		constexpr static uint16_t WSL_HANDGUN_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_HANDGUN_3_ACCURACY = 0;
		constexpr static uint16_t WSL_HANDGUN_3_DAMAGE = 0;

		constexpr static uint16_t WSL_HANDGUN_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_HANDGUN_4_ACCURACY = 0;
		constexpr static uint16_t WSL_HANDGUN_4_DAMAGE = 0;


		constexpr static uint16_t WSL_LMG_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_LMG_0_ACCURACY = 40;
		constexpr static uint16_t WSL_LMG_0_DAMAGE = 40;

		constexpr static uint16_t WSL_LMG_1_ALLOWABLE = 1;
		constexpr static uint16_t WSL_LMG_1_ACCURACY = 35;
		constexpr static uint16_t WSL_LMG_1_DAMAGE = 35;

		constexpr static uint16_t WSL_LMG_2_ALLOWABLE = 1;
		constexpr static uint16_t WSL_LMG_2_ACCURACY = 30;
		constexpr static uint16_t WSL_LMG_2_DAMAGE = 30;

		constexpr static uint16_t WSL_LMG_3_ALLOWABLE = 0;
		constexpr static uint16_t WSL_LMG_3_ACCURACY = 0;
		constexpr static uint16_t WSL_LMG_3_DAMAGE = 0;

		constexpr static uint16_t WSL_LMG_4_ALLOWABLE = 0;
		constexpr static uint16_t WSL_LMG_4_ACCURACY = 0;
		constexpr static uint16_t WSL_LMG_4_DAMAGE = 0;


		constexpr static uint16_t WSL_SNIPER_0_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SNIPER_0_ACCURACY = 10;
		constexpr static uint16_t WSL_SNIPER_0_DAMAGE = 10;

		constexpr static uint16_t WSL_SNIPER_1_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SNIPER_1_ACCURACY = 35;
		constexpr static uint16_t WSL_SNIPER_1_DAMAGE = 35;

		constexpr static uint16_t WSL_SNIPER_2_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SNIPER_2_ACCURACY = 60;
		constexpr static uint16_t WSL_SNIPER_2_DAMAGE = 60;

		constexpr static uint16_t WSL_SNIPER_3_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SNIPER_3_ACCURACY = 75;
		constexpr static uint16_t WSL_SNIPER_3_DAMAGE = 75;

		constexpr static uint16_t WSL_SNIPER_4_ALLOWABLE = 1;
		constexpr static uint16_t WSL_SNIPER_4_ACCURACY = 75;
		constexpr static uint16_t WSL_SNIPER_4_DAMAGE = 75;

		constexpr static uint8_t SINGLE_SHOT_SHOTGUN = 1;
		constexpr static uint8_t SINGLE_SHOT_ASSAULT_RIFLE = 3;
		constexpr static uint8_t SINGLE_SHOT_SUB_MACHINE_GUN = 3;
		constexpr static uint8_t SINGLE_SHOT_SNIPER = 1;
		constexpr static uint8_t SINGLE_SHOT_HANDGUN = 1;
		constexpr static uint8_t SINGLE_SHOT_PISTOL = 1;
		constexpr static uint8_t SINGLE_SHOT_MACHINE_PISTOL = 6;
		constexpr static uint8_t SINGLE_SHOT_LIGHT_MACHINE_GUN = 8;

		constexpr static uint8_t SPRAY_SHOT_SHOTGUN = 2;
		constexpr static uint8_t SPRAY_SHOT_ASSAULT_RIFLE = 9;
		constexpr static uint8_t SPRAY_SHOT_SUB_MACHINE_GUN = 6;
		constexpr static uint8_t SPRAY_SHOT_SNIPER = 1;
		constexpr static uint8_t SPRAY_SHOT_HANDGUN = 1;
		constexpr static uint8_t SPRAY_SHOT_PISTOL = 1;
		constexpr static uint8_t SPRAY_SHOT_MACHINE_PISTOL = 12;
		constexpr static uint8_t SPRAY_SHOT_LIGHT_MACHINE_GUN = 16;

		constexpr static uint8_t SPRAY_CHANCE = 25;
		constexpr static uint8_t SPRAY_HEADSHOT_CHANCE = 3;
		constexpr static uint8_t SPRAY_CRITICAL_CHANCE = 2;
		constexpr static uint8_t SPRAY_CRITICAL_REDUCTION_DIVISOR = 10;

		constexpr static uint8_t REVIVE_TICKS = 40;
		constexpr static uint8_t REVIVE_HP = 50;
		constexpr static uint8_t INJURED_HP = 5;
		constexpr static uint8_t EXPLOSIVE_CRITICAL_MULTIPLIER = 0.75;
		constexpr static uint8_t RIFLE_CRITICAL_MULTIPLIER = 0.75;
		namespace msg {
			constexpr static const char* CRITICAL= "{red}***CRITICAL***{/red} --";
			constexpr static const char* HEADSHOT= "{red}***HEADSHOT***{/red} -- ";
			constexpr static const char* HIT= "{yel}--[HIT]--{/yel}";
			constexpr static const char* OUT_OF_AMMO= "{gld}*CLICK*{/gld} Your weapon is out of ammo!";
			constexpr static const char* MISSED_TARGET = "You missed your target!";
			constexpr static const char* TARGET_DEAD = "It appears that your target is dead.";
			constexpr static const char* YOURE_INJURED = "You are injured!";
			template <typename T>
			static inline void crit(T& p){
				p->send(CRITICAL);
			}
			template <typename T>
			static inline void headshot(T& p){
				p->send(HEADSHOT);
			}
			template <typename T>
			static inline void hit(T& p){
				p->send(HIT);
			}
			template <typename T>
			static inline void outofammo(T& p){
				p->sendln(OUT_OF_AMMO);
			}
			template <typename T>
			static inline void missed(T& p){
				p->sendln(MISSED_TARGET);
			}
			template <typename T>
			static inline void target_dead(T& p){
				p->sendln(TARGET_DEAD);
			}
			template <typename T>
			static inline void youre_injured(T& p){
				p->sendln(YOURE_INJURED);
			}
		};
};

using namespace mods::values;
#endif
