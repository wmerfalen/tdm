#ifndef __MENTOC_MODS_combat_composer_HEADER__
#define __MENTOC_MODS_combat_composer_HEADER__
#include "../globals.hpp"

namespace mods::combat_composer {
	using vpd = mods::scan::vec_player_data;
	enum attack_type_t {
		RANGED,
		PROXY, /** i.e.: though a drone */
		MELEE_WEAPON,
		CQC, /** fists, elbows, knees, kicks */
		JUDO_THROW,
		SUBMISSION,
		WRESTLING,
		DEMOLITIONS,
		_MANA, /** i.e.: a magic missile */
		MELEE_MANA, /** same room melee attack */
	};
	/**
	 */
	/** @@@@@@======================================================@@@@@@@@@@@@@@
	 		@@@@@@            Phases of RANGED combat                   @@@@@@@@@@@@@@
			@@@@@@======================================================@@@@@@@@@@@@@@
	    || For each phase, it is assumed that buffs/nerfs and PC abilities are  ||
	    || calculated and fed to each step to determine success and if flow     ||
	    || continues, stops, or gets redirected (i.e.: if a miss)               ||
			||======================================================================||

	   1) Target acquisition
	   		 Yaml file dependants:
	   			rifle:
	   				- zoom magnification
	   				- max range
	   				- critical range
	  				- range multiplier
	  				- effective firing range
	   			attachments:
	   				- zoom multiplier
	   				- aimed limb accuracy points
	   			armor:
	   				- stat weapon handling
	   				- stat sniping
	   			consumable:
	   				- adds room range
	   				- adds critical range
	   				- adds max range
	  
	   		 GHOST abilities:
	   			- marked/tracked enemy
	   			- snipe doors, objects, cars, etc
	  
	   		 CONTAGION abilities:
	   			- Shadow sight
	   			- Morbid Insight
	   				- player can detect nearby enemies if a corpse is nearby
	  
	   		 MARINE abilities:
	   			- Assault rifles
	    			- Assault rifle effective range increased by 2 rooms
	  
	   		-- Combat phase ends here if...
	   		 - cannot see target
	   		 - ranged attack and target is out of range of weapon
	   		 - melee/cqc/grappling attack and target not in same room
	   		 - is mana attack and not enough mana
	   		 - target is dead (with exceptions from contagion)
	  
	   2) Roll accuracy check
	   		 YAML file dependants
	   			rifle:
	   				- effective firing range
	   					- boosts percentage of hit
	   				- stat marksmanship
	   				- stat sniping
	   				- stat weapon handling
	   				- stat strategy
	   			attachments:
	   				- accuracy points
	   				- recoil reduction
	   				- aimed limb accuracy
	   			armor:
	   				- stat marksmanship
	   				- stat sniping
	   				- stat weapon handling
	   				- stat strategy
	   		2A) Roll hit or miss
	   			- If miss, Go immediately to step 8
	  
	   3) Roll headshot and Critical
	   		 YAML file dependants
	   			rifle:
	   				- headshot bonus
	   				- critical chance
	   				- critical range
	   			consumable:
	   				- adds headshot bonus
	   		 MARINE abilities:
	   			Assault rifles:
	   				- 10% critical chance
	  
	   4) Calculate damage (including elemental)
	   		 YAML file dependants
	   			rifle:
	   				- base damage
	   				- headshot bonus (if headshot)
	   				- muzzle velocity
	   				- damage dice
	   				- * all elemental damage
	   				- stat marksmanship
	   				- stat sniping
	   				- stat weapon handling
	   				- stat strategy
	   			attachments:
	   				- * all elemental damage
	   				- armor penetration amount
	   				- damage percent bonus
	  			armor:
	   				- stat marksmanship
	   				- stat sniping
	   				- stat weapon handling
	   				- stat strategy
	   			consumable:
	   				- adds hitroll
	   				- adds damage roll
	   				- adds fire dmg
	   				- adds headshot bonus (if headshot)
	   				- adds muzzle velocity
	   				- adds damage dice count/sides

				 GHOST abilities:
					Sniper rifles:
				 		- Extra 10.5% damage per tier 
				 		- tracked enemies take more damage
	  
	  		 MARINE abilities:
	  			- Assault rifles
	  				- do 25% more damage
	  				- have a 10% chance of dealing incendiary damage
	  			- Shotguns deal same room damage at 2-3 rooms away

				 BREACHER abilities:
				 	- all wielded weapons get +10% explosive damage
				  - can remotely seal a room for the purpose of amplifying detonations
				  - breaching doors causes room inhabitants to take damage from explosion
				  -- "breach and clear"
				   	-- breach a door and immediatel throw a flash bang into the room
				   	-- inhabitants get disoriented and take more damage
				   -- "knockdown"
				   	-- fire a shot that knocks the enemy to the ground
				   	-- knocked down enemies take more damage and are vulnerable to "detain"
				   -- Shotguns deal 10% extra damage
				   -- SMG's deal 10% extra damage
				   -- SMG's have corrossive damage
				   	-- corrossive damage from SMG's cause enemy armor to be less effective
				  -- SMG's in same-room engagements have a chance of dealing shotgun damage
	  
	   5) Calculate damage applied to victim
	   	THe following are buffs/nerfs in regard to 
	   	the victim's stats, gear, and PC abilities
	   		Absorbing  YAML file dependants
	   			armor:
	   				- thac0
	   				- fire/ballistic resistance percent
	   				- durability profile
	   				- hp
	   				- classification
	   				- * all elemental resistance percents
	   				- stat strength
	   				- stat constitution
	   				- stat armor
	   			consumable:
	   				- adds hp
	   				- adds strength
	   				- adds constitution
	   				- adds armor class
	   		 GHOST abilities:
	   			- adrenaline shot	
	  
	   		 CONTAGION abilities:
	   			- Hellfire Circle
	   			- particle deceleration
	   			- ghastly double
	   			- minor shielding
	   			- cursed ballistics
	   			- neutron shield
	   			- bladed array
	   		 MARINE abilities:
	   			- received shrapnel + incendiary damage reduced by 25%
	   			- second wind
	   			- received disorient attacks reduced by 50%
				 BREACHER abilities:
				  - received shrapnel and explosive damage reduced by 25%

		6) Calculate armor penetration amount
			- Affect HP of armor with respect to armor durability profile
	  
	   8) Consume ammunition or stats (if needed)
		 		yaml file attributes
		 			attachments:
						- free ammo chance
						- regenerate ammo chance
	   		 MARINE abilities:
	   			Assault rifles:
	   				- 15% free ammunition
	  
	 */
	namespace phases {
		namespace target_acquisition {
			struct calculation_t {
				uint8_t max_range;
				uint8_t critical_range;
				std::pair<uint8_t,uint8_t> effective_range;
				uint16_t stat_boosts;
			};

			struct target_t {
				direction_t direction;
				std::string target_name;
			};

			struct acquired_target_t {
				player_ptr_t target;
				direction_t direction;
				uint16_t distance;
			};
			calculation_t calculate_range(player_ptr_t& attacker,obj_ptr_t& weapon){
				calculation_t c;
				auto & rats = weapon->rifle()->attributes;
				auto zoom = rats->zoom_magnification;
				c.max_range = rats->max_range;
				c.critical_range = rats->critical_range;
				if(rats->range_multiplier){
					c.max_range *= rats->range_multiplier;
				}
				c.effective_range = {rats->effective_firing_range,rats->critical_range};
				c.max_range += 0.05 * attacker->stat_weapon_handling();
				c.critical_range -= 0.05 * attacker->stat_sniping();
				return c;
				
				/** TODO: honor attachments:
	   			attachments:
	   				- zoom multiplier
	   				- aimed limb accuracy points
				*/
				/** TODO: honor consumed items
	   			consumable:
	   				- adds room range
	   				- adds critical range
	   				- adds max range
				*/

				/** TODO: honor class abilities
	   		 GHOST abilities:
	   			- marked/tracked enemy
	   			- snipe doors, objects, cars, etc
	  
	   		 CONTAGION abilities:
	   			- Shadow sight
	   			- Morbid Insight
	   				- player can detect nearby enemies if a corpse is nearby
	  
	   		 MARINE abilities:
	   			- Assault rifles
	    			- Assault rifle effective range increased by 2 rooms
						*/
	  
			}

			std::optional<acquired_target_t> aquire_ranged_target(player_ptr_t& attacker,target_t target){
				mods::scan::los_scan_direction(attacker->cd(),max_range,&scan,direction,mods::scan::find_type_t::ALIVE);
				if(attacker->ghost()){
					/* TODO:
	   		 GHOST abilities:
	   			- marked/tracked enemy
	   			- snipe doors, objects, cars, etc
					*/
				}
	  
				/** TODO: 
	   		 CONTAGION abilities:
	   			- Shadow sight
	   			- Morbid Insight
	   				- player can detect nearby enemies if a corpse is nearby
	  
	   		 MARINE abilities:
	   			- Assault rifles
	    			- Assault rifle effective range increased by 2 rooms
				*/

				return std::nullopt;
			}
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
