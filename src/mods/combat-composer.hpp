#ifndef __MENTOC_MODS_combat_composer_HEADER__
#define __MENTOC_MODS_combat_composer_HEADER__
#include "../globals.hpp"

namespace mods::combat_composer {
			enum attack_type_t {
				RANGED,
				PROXY, /** i.e.: though a drone */
				MELEE_WEAPON,
				CQC, /** fists, elbows, knees, kicks */
				JUDO_THROW,
				SUBMISSION,
				WRESTLING,
				DEMOLITIONS,
				RANGED_MANA, /** i.e.: a magic missile */
				MELEE_MANA, /** same room melee attack */
			};
	/**
	 * Phases of combat
	 */
	/**
	 * -- For each phase, it is assumed that buffs/nerfs and PC abilities are
	 *  calculated and fed to each step to determine success and if flow
	 *  continues, stops, or gets redirected (i.e.: if a miss)
	 * -- 
	 * 1) Target acquisition
	 * 		RANGED Yaml file dependants:
	 * 			rifle:
	 * 				- zoom magnification
	 * 				- max range
	 * 				- critical range
	 *				- range multiplier
	 *				- effective firing range
	 * 			attachments:
	 * 				- zoom multiplier
	 * 				- aimed limb accuracy points
	 * 			armor:
	 * 				- offensive damage amount
	 * 				- stat weapon handling
	 * 				- stat sniping
	 * 			consumable:
	 * 				- adds room range
	 * 				- adds critical range
	 * 				- adds max range
	 *
	 * 		RANGED GHOST abilities:
	 * 			- marked/tracked enemy
	 * 			- snipe doors, objects, cars, etc
	 *
	 * 		RANGED CONTAGION abilities:
	 * 			- Shadow sight
	 * 			- Morbid Insight
	 * 				- player can detect nearby enemies if a corpse is nearby
	 *
	 * 		-- Combat phase ends here if...
	 * 		 - cannot see target
	 * 		 - ranged attack and target is out of range of weapon
	 * 		 - melee/cqc/grappling attack and target not in same room
	 * 		 - is mana attack and not enough mana
	 * 		 - target is dead (with exceptions from contagion)
	 *
	 * 2) Roll accuracy check
	 * 		RANGED YAML file dependants
	 * 			rifle:
	 * 				- effective firing range
	 * 					- boosts percentage of hit
	 * 				- stat marksmanship
	 * 				- stat sniping
	 * 				- stat weapon handling
	 * 				- stat strategy
	 * 			attachments:
	 * 				- accuracy points
	 * 				- recoil reduction
	 * 				- aimed limb accuracy
	 * 			armor:
	 * 				- stat marksmanship
	 * 				- stat sniping
	 * 				- stat weapon handling
	 * 				- stat strategy
	 * 		2A) Roll hit or miss
	 * 			- If miss, Go immediately to step 8
	 *
	 * 3) Roll headshot and Critical
	 * 		RANGED YAML file dependants
	 * 			rifle:
	 * 				- headshot bonus
	 * 				- critical chance
	 * 				- critical range
	 * 			consumable:
	 * 				- adds headshot bonus
	 *
	 * 4) Calculate damage
	 * 		RANGED YAML file dependants
	 * 			rifle:
	 * 				- base damage
	 * 				- headshot bonus (if headshot)
	 * 				- muzzle velocity
	 * 				- damage dice
	 * 				- * all elemental damage
	 * 				- stat marksmanship
	 * 				- stat sniping
	 * 				- stat weapon handling
	 * 				- stat strategy
	 * 			attachments:
	 * 				- * all elemental damage
	 * 				- armor penetration amount
	 * 				- damage percent bonus
	 *			armor:
	 * 				- stat marksmanship
	 * 				- stat sniping
	 * 				- stat weapon handling
	 * 				- stat strategy
	 * 			consumable:
	 * 				- adds hitroll
	 * 				- adds damage roll
	 * 				- adds fire dmg
	 * 				- adds headshot bonus (if headshot)
	 * 				- adds muzzle velocity
	 * 				- adds damage dice count/sides
	 *
	 * 5) Calculate damage applied to victim
	 * 	THe following are buffs/nerfs in regard to 
	 * 	the victim's stats, gear, and PC abilities
	 * 		Absorbing RANGED YAML file dependants
	 * 			armor:
	 * 				- thac0
	 * 				- fire/ballistic resistance percent
	 * 				- durability profile
	 * 				- hp
	 * 				- classification
	 * 				- * all elemental resistance percents
	 * 				- stat strength
	 * 				- stat constitution
	 * 				- stat armor
	 * 			consumable:
	 * 				- adds hp
	 * 				- adds strength
	 * 				- adds constitution
	 * 				- adds armor class
	 * 		RANGED GHOST abilities:
	 * 			- adrenaline shot	
	 *
	 * 		RANGED CONTAGION abilities:
	 * 			- Hellfire Circle
	 * 			- particle deceleration
	 * 			- ghastly double
	 * 			- minor shielding
	 * 			- cursed ballistics
	 * 			- neutron shield
	 * 			- bladed array
	 * 		RANGED MARINE abilities:
	 * 			- received shrapnel + incendiary damage reduced by 25%
	 * 			- second wind
	 * 			- received disorient attacks reduced by 50%
	 *
	 * 8) Consume ammunition or stats (if needed)
	 *
	 * 9) Roll victim response
	 * 		- Run this step only if victim has an ability
	 * 			that allows them to counter attack immediately
	 *
	 * 10) Resolve combat phase
	 * 		- Report damage to player's class object (i.e.: mods/classes/contagion::combat_resolved(...context...))
	 */
	namespace phases {
		namespace target_acquisition {
			struct target_info_t {
				std::optional<direction_t> direction;
				std::optional<std::string> target_name;
				std::forward_list<obj_ptr_t> attachments;
			};


			std::tuple<bool,std::string> aquire_target(player_ptr_t& attacker,const attack_type_t& type, 
		};

	};//end namespace phases
	struct attack {
		/** We want:
		 * 	1) to roll dice for a base weapon (ranged or melee)
		 * 	2) take into account the player class
		 * 	3) add buffs/nerfs from player class abilities that are active
		 * 	4) consume ammo or stats (if needed)
		 * 	5) calculate consumed ammo or stats with player class ability buffs/nerfs
		 * 	6) calculate consumed ammo or stats with player armor/attachment buffs/nerfs
		 * 	7) deal the damage to target
		 */
		attack() = delete;
		attack(player_ptr_t& attacker);
	};
};
#endif
