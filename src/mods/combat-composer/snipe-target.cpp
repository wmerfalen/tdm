#include "snipe-target.hpp"
#include "../rifle.hpp"
#include "../scan.hpp"
#include "../player.hpp"
#include "../ranged-combat-totals.hpp"
#include "../damage-event.hpp"
#include "../rooms.hpp"
#include "../object-utils.hpp"
#include "../player-utils.hpp"
#include "../calc-visibility.hpp"
#include "../classes/ghost.hpp"
#include "../levels.hpp"

#include "../../fight.hpp"
#include "../mobs/damage-event.hpp"
#include "../weapons/elemental.hpp"
#include "../weapons/legacy-combat.hpp"
#include "../interpreter.hpp"

namespace mods::combat_composer {
	using vpd = mods::scan::vec_player_data;
	using de = damage_event_t;
	using namespace mods::object_utils;
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

	ACMD(do_print_rct) {
		player->calculate_ranged_combat_totals()->report(player);
	}
	void init() {
		mods::interpreter::add_user_command("print_rct",do_print_rct);
	}


	/**
	 * Handles spraying only
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {

	}

	/**
	 * Handles launching only
	 */
	void launch_towards(player_ptr_t& attacker,direction_t direction,uint8_t distance,obj_ptr_t& weapon) {

	}
	namespace ammunition {
		uint16_t reduce_ammo(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    uint16_t wants_to_deduct
		) {
			if(weapon->has_rifle() == false) {
				return 0;/** TODO: make sure this doesnt screw something up */
			}
			uint16_t ammo_reduction = wants_to_deduct;
			auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
			if(!rifle_attachment) {
				return wants_to_deduct;
			}
			if(mods::rand::chance(rifle_attachment->free_ammo_chance)) {
				m_debug("Player rolled free ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->free_ammo_chance << "%");
				ammo_reduction = 0;
			}
			if(mods::rand::chance(rifle_attachment->regenerate_ammo_chance)) {
				m_debug("Player rolled regenerated ammo " << green_str("SUCCESS") << "Chances were: " << rifle_attachment->regenerate_ammo_chance << "%");
				auto regen = rand_number(
				                 mods::values::REGENERATED_AMMO_LOW(),
				                 mods::values::REGENERATED_AMMO_HIGH()
				             );
				m_debug("Random amount of regenerated ammo: " << regen);
				return ammo_reduction - regen;
			}
			return ammo_reduction;
		}
	};
	void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) ;
	namespace phases {
		/**
		 */
		/** @@@@@@======================================================@@@@@@@@@@@@@@
		 		@@@@@@            Phases of RANGED combat                   @@@@@@@@@@@@@@
				@@@@@@======================================================@@@@@@@@@@@@@@
		    || For each phase, it is assumed that buffs/nerfs and PC abilities are  ||
		    || calculated and fed to each step to determine success and if flow     ||
		    || continues, stops, or gets redirected (i.e.: if a miss)               ||
				||======================================================================||
				*/

		/**
		 *
		   TODO: verify all of these are being accounted for
			 in the ranged combat totals function in mods::player (player.cpp)
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

		   		 MARINE abilities: [ FULFILLED IN PLAYER.CPP ]
		   			- Assault rifles
		    			- Assault rifle effective range increased by 2 rooms

		   		-- Combat phase ends here if...
		   		 - cannot see target
		   		 - ranged attack and target is out of range of weapon
		   		 - melee/cqc/grappling attack and target not in same room
		   		 - is mana attack and not enough mana
		   		 - target is dead (with exceptions from contagion)
		 *
		 */



		/**
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
		struct calculation_t {
			int16_t max_range;
			int16_t critical_range;
			std::pair<int16_t,int16_t> effective_range;
			uint16_t stat_boosts;
		};

		struct target_t {
			std::string target_name;
			direction_t direction;
			bool is_corpse;
			bool is_object;
			bool is_character;
			target_t(std::string_view _target_name,
			         direction_t _direction,
			         bool _is_corpse,
			         bool _is_object,
			         bool _is_character
			        ) : target_name(_target_name),
				direction(_direction),
				is_corpse(_is_corpse),
				is_object(_is_object),
				is_character(_is_character)
			{}
			target_t(std::string_view _target_name,
			         direction_t _direction) :
				target_name(_target_name),
				direction(_direction),
				is_corpse(0),
				is_object(0),
				is_character(1)
			{}
			target_t() = delete;
			~target_t() = default;
		};

		struct acquired_target_t {
			player_ptr_t target;
			direction_t direction;
			uint16_t distance;
		};

		struct calculated_damage_t {
			int damage;
			int headshot_damage;
			int critical_damage;
			int incendiary_damage;
			int explosive_damage;
			int shrapnel_damage;
			int corrosive_damage;
			int cryogenic_damage;
			int radioactive_damage;
			int emp_damage;
			int shock_damage;
			int anti_matter_damage;
			calculated_damage_t() :
				damage(0),
				headshot_damage(0),
				critical_damage(0),
				incendiary_damage(0),
				explosive_damage(0),
				shrapnel_damage(0),
				corrosive_damage(0),
				cryogenic_damage(0),
				radioactive_damage(0),
				emp_damage(0),
				shock_damage(0),
				anti_matter_damage(0) {}
			~calculated_damage_t() = default;
		};
		calculation_t calculate_range(player_ptr_t& attacker,obj_ptr_t& weapon);
		bool roll_accuracy(player_ptr_t& attacker,acquired_target_t& target,obj_ptr_t& weapon);
		std::pair<int,int> roll_critical(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon);
		calculated_damage_t calculate_weapon_damage(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon,std::pair<int,int> hsc);

		namespace state {
			static player_ptr_t attacker;
			static std::shared_ptr<mods::ranged_combat_totals> current;
#define RCT state::current
#define ATKR state::attacker
		};
		bool attack_disorients(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		) {
			if(!weapon) {
				return false;
			}
			if(weapon->has_rifle()) {
				auto rifle_attachment = mods::rifle_attachments::by_uuid(weapon->uuid);
				if(rifle_attachment && mods::rand::chance(rifle_attachment->disorient_amount)) {
					/** TODO: calculate disorient resistance of victim */
					m_debug("Attack disorients player!");
					return true;
				}
				m_debug("Attack fails to disorient player");
				return false;
			} else if(weapon->has_melee()) {
				/** TODO */
				if(mods::rand::chance(weapon->melee()->attributes->disorient_amount)) {
					/** TODO: calculate disorient resistance of victim */
					m_debug("Attack disorients player!");
					return true;
				}
			}
			return false;
		}
		uint16_t disorient_ticks(
		    player_ptr_t& attacker,
		    obj_ptr_t& weapon,
		    player_ptr_t& victim
		) {
			return mods::values::DEFAULT_DISORIENT_TICKS_FROM_RIFLE_ATTACK();
		}
		void remember_event(player_ptr_t& victim,player_ptr_t& attacker) {
			if(IS_NPC(victim->cd())) {
				remember(victim->cd(),attacker->cd());
			}
		}


		int calculate_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback) {
			return attackers_weapon->rifle()->attributes->cooldown_between_shots;
		}

		void set_player_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback) {
			auto cooldown = calculate_weapon_cooldown(attacker,victim,attackers_weapon,feedback);
			if(cooldown <= 0) {
				attacker->weapon_cooldown_clear();
			} else {
				attacker->weapon_cooldown_start(cooldown);
			}
		}

		/**
		 * Usage:
		 * -- firing at a drone to the north 2 rooms away using the primary weapon  --
		 * acquire_ranged_target(attacker,{"drone",NORTH,2}, attacker->primary());
		 *
		 * -- firing same-room secondary pistol at a target (direction used to push mob that way if force is great enough) --
		 * acquire_ranged_target(attacker,{"drone",NORTH,0}, attacker->secondary());
		 *
		 *
		 * -- firing a shotgun underbarrel at a drone within the same room (direction used to push mob) --
		 * acquire_ranged_target(attacker,{"drone",NORTH,0}, attacker->ghost()->underbarrel());
		 */
		std::optional<acquired_target_t> acquire_target(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon);

		void perform_cleanup(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon) {
			feedback_t feedback;
			m_debug("decreasing ammo");
			decrease_single_shot_ammo(attacker,weapon);
			attacker->set_fight_timestamp();
			set_player_weapon_cooldown(attacker,found_target.target,weapon, feedback);
			m_debug("cooldown set");
			return;
		}
		/**
		 * Usage:
		 * -- roll accuracy check for sniping target 2 rooms away using primary weapon --
		 * auto status = roll_accuracy(attacker,{"drone",NORTH,2},attacker->primary());
		 * if(!status){
		 * 		attacker->sendln("You aim and MISS your target!");
		 * 		return;
		 * }
		 */
		bool roll_accuracy(player_ptr_t& attacker,acquired_target_t& target,obj_ptr_t& weapon) {
#ifdef send_attacker_accuracy_stats
#define md(A) attacker->sendln(CAT("Debug: ",A));
#else
#define md(A) /** */
#endif
			/** Weapon handling trait adds 2% chance of hit per tier */
			/** If GHOST class, add 2% chance of hit per tier */
			/** Take into account RCT values:
			 * effective range
			 * critical range
			 * recoil reduction
			 * muzzle velocity
			 */
			int32_t success_chance = 0;
			auto& victim = target.target;
			int adjust_max_roll = 0;
			const auto& distance = target.distance;
			if(distance >= RCT->effective_range.first && distance <= RCT->effective_range.second) {
				md("Target in effective range");
				success_chance += 50;
			}
			if(distance >= RCT->critical_range.first && distance <= RCT->critical_range.second) {
				md("Target in critical range!");
				success_chance += 50;
			}
			if(attacker->ghost()) {
				if(is_sniper_rifle(weapon)) {
					md("You are a ghost and wielding a sniper rifle");
					success_chance += 100;
				}
				if(is_smg(weapon)) {
					if(distance <= RCT->effective_range.second) {
						md("You are a ghost and within SMG effective range");
						success_chance += 50;
					}
				}
			}
			if(attacker->marine()) {
				if(is_assault_rifle(weapon)) {
					success_chance += 70;
				}
				if(is_shotgun(weapon) && RCT->effective_range.second >= distance) {
					success_chance += 150;
				}
			}
			if(victim->ghost()) {
				if(victim->ghost()->is_dissipated()) {
					md("Your target is dissipated");
					success_chance -= 80;
					auto range = mods::player_utils::get_thermal_vision_range(attacker);
					if(range.second >= distance) {
						success_chance += 80;
					}
				}
			}
			if(attacker->breacher()) {
				bool shotty = is_shotgun(weapon);
				bool smg = is_smg(weapon);
				if(shotty) {
					if(distance <= 3) {
						success_chance += 50;
					}
				}
				if(smg) {
					if(distance <= 5) {
						success_chance += 50;
					}
				}
				if(smg || shotty) {
					adjust_max_roll = 20;
				}
			}

			if(mods::calc_visibility::is_visible(attacker,victim)) {
				md("Is visible");
				success_chance += 50;
			} else {
				success_chance -= 100;
				md("Not visible!");
			}
			auto roll = rand_number(1,100 + adjust_max_roll);
			md("success_chance is.. ");
			md(success_chance);
			md("roll is...");
			md(roll);
			return success_chance > roll;
		}
		void report_headshot(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction) {
			feedback_t feedback;
			feedback.hits = 1;
			feedback.damage = dam;
			feedback.attacker = attacker->uuid();
			feedback.damage_event = de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK;
			attacker->damage_event(feedback);

			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event =de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK;
			victim->damage_event(feedback);

			attacker->send(MSG_HEADSHOT().c_str());
		}
		void report_crit(player_ptr_t& attacker, player_ptr_t& victim, int dam, direction_t& direction) {
			feedback_t feedback;
			attacker->send(MSG_CRITICAL().c_str());
			feedback.damage = dam;
			feedback.hits = 1;

			feedback.damage_event = de::YOU_DEALT_CRITICAL_RIFLE_ATTACK;
			attacker->damage_event(feedback);

			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event =de::HIT_BY_CRITICAL_RIFLE_ATTACK;
			victim->damage_event(feedback);
		}

		/**
		 * Usage:
		 * -- calculate headshot, critical chance --
		 * auto crits = roll_critical(attacker,{"drone",NORTH,2},attacker->primary());
		 *
		 */

		/** Rolls headshot and critical damage (respectively) */
		std::pair<int,int> roll_critical(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon) {
			/*
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
						*/
			auto& victim = found_target.target;
			const auto& distance = found_target.distance;
			feedback_t feedback;
			int headshot = 0, crit = 0;
			auto crit_roll = dice(1,100);
			if(distance <= RCT->critical_range.second && distance >= RCT->critical_range.first && crit_roll < (RCT->critical_chance % 100)) {
				crit = dice(
				           RCT->damage_dice_count,
				           RCT->damage_dice_sides
				       );
				report_crit(attacker,victim,crit,found_target.direction);
			}
			/** calculate headshot */
			auto headshot_roll = dice(1,100) + (mods::skills::player_can(attacker,"HEADSHOT_CHANCE") ? HEADSHOT_SKILL_MODIFIER() : 0);
			if(headshot_roll >= 95) {
				/** TODO: evaluate damage if wearing super strong headgear */
				headshot = victim->hp() / HEADSHOT_DIVISOR();
				report_headshot(attacker,victim,headshot,found_target.direction);
				m_debug("headshot");
			}
			m_debug("checking crit range");
			return {headshot,crit};
		}

		/**
		 * Methods of using a rifle:
		 * - Ranged
		 *   - several rooms away
		 *   - can only hit friendlies if specified explicitly
		 *   - expects: target name match, distance, and direction
		 *   - LMG's can snipe but only a single shot at a time
		 *   - Shotguns can snipe but..
		 *   	- accuracy and damage decay significantly faster
		 *
		 * - Immediate
		 *   - Same room
		 *   - expects: target name
		 *   - AR's, SMG's, Shotguns, Pistols, are fine
		 *   - LMG's cannot do same room damage
		 *   - Sniper rifles cannot do same room damage
		 *
		 * - Spraying
		 *   - sniper rifles cannot spray
		 *   - pistols cannot spray
		 *   - machine pistols _can_ spray
		 *   - shotguns cannot spray
		 *   - several rooms away
		 *   - hits multiple targets in room
		 *   - not as accurate
		 *   - can hit friendlies
		 *   - only LMG's and AR's can spray
		 *
		 * - Launching
		 *   - lobs a projectile several rooms away
		 *   - cannot be same room
		 *   - expects: distance, direction
		 */


		/**
		 * -- firing fragmentation underbarrel at an entire room of targets 3 rooms away to the north --
		 * acquire_room(attacker,{"",NORTH,3}, attacker->ghost()->underbarrel());
		 *
		 * -- spraying an LMG at an entire room of targets 3 rooms away to the north --
		 * acquire_room(attacker,{"",NORTH,3}, attacker->primary());
		 */
		std::vector<acquired_target_t> acquire_room(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon);


		std::optional<acquired_target_t> acquire_ranged_target(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon) {

			state::current = attacker->calculate_ranged_combat_totals(weapon);
			state::attacker = attacker;
			vpd scan;
			/**
			 * TODO: will have to change find_type_t::ANY to include DEAD for contagion's
			 * corpse sniping capabilities
			 */
			mods::scan::los_scan_direction(attacker->cd(),RCT->max_range,&scan,target.direction);//,mods::scan::find_type_t::NPC_AND_PLAYER);
			//if(attacker->ghost()) {
			//	/* TODO:
			//	GHOST abilities:
			//	- marked/tracked enemy
			//	- snipe doors, objects, cars, etc
			//	*/
			//}

			/** TODO:
			CONTAGION abilities:
			- Shadow sight
			- Morbid Insight
				- player can detect nearby enemies if a corpse is nearby

			MARINE abilities:
			- Assault rifles
				- Assault rifle effective range increased by 2 rooms
			*/
			//auto max_range = mods::weapons::damage_calculator::max_range(attacker,weapon);

			acquired_target_t found;
			for(auto& scanned_target : scan) {
				player_ptr_t victim = nullptr;
				if(scanned_target.ch && scanned_target.uuid) {
					victim = ptr_by_uuid(scanned_target.uuid);
				}
				if(!victim) {
					continue;
				}
				if(mods::util::fuzzy_match(target.target_name,victim->name())) {
					if(scanned_target.distance > RCT->max_range) {
						attacker->sendln("That target is out of range!");
						return std::nullopt;
					}
					found.target = victim;
					found.direction = target.direction;
					found.distance = scanned_target.distance;
					return found;
				}
			}

			return std::nullopt;
		}

		calculated_damage_t calculate_weapon_damage(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon,std::pair<int,int> hsc) {
			calculated_damage_t d;
			/**
			4) Calculate damage (including elemental)
			 YAML file dependants
				rifle:
					- [x] base damage
					- [x] headshot bonus (if headshot)
					- muzzle velocity
					- [x] damage dice
					- [x] * all elemental damage
					- stat marksmanship
					- stat sniping
					- stat weapon handling
					- stat strategy
				attachments:
					- [x] * all elemental damage
					- armor penetration amount
					- [x] damage percent bonus
				armor:
					- stat marksmanship
					- stat sniping
					- stat weapon handling
					- stat strategy
				consumable:
					- adds hitroll
					- adds damage roll
					- adds fire dmg
					- [x] adds headshot bonus (if headshot)
						- applied via RCT->headshot_bonus
					- adds muzzle velocity
					- [x] adds damage dice count/sides
						- applied via RCT->damage_dice_*

				 GHOST abilities:
					Sniper rifles:
				 		- [x] Extra 10.5% damage per tier
				 		- [ ] tracked enemies take more damage

			 MARINE abilities:
				- Assault rifles
					- [x] do 25% more damage
					- [x] have a 10% chance of dealing incendiary damage
				- [x] Shotguns deal same room damage at 2-3 rooms away

				 BREACHER abilities:
				 	- [x] all wielded weapons get +10% explosive damage
				  - can remotely seal a room for the purpose of amplifying detonations
				  - breaching doors causes room inhabitants to take damage from explosion
				  -- "breach and clear"
				   	-- breach a door and immediatel throw a flash bang into the room
				   	-- inhabitants get disoriented and take more damage
				   -- [ ] "knockdown"
				   	-- [ ] fire a shot that knocks the enemy to the ground
				   	-- [ ] knocked down enemies take more damage and are vulnerable to "detain"
				   -- [x] Shotguns deal 10% extra damage
				   -- [x] SMG's deal 10% extra damage
				   -- [x] SMG's have corrossive damage
				   	-- [x] corrossive damage from SMG's cause enemy armor to be less effective
				  -- [x] SMG's in same-room engagements have a chance of dealing shotgun damage
					*/
			const auto& distance = found_target.distance;
			md("base damage");
#ifdef report_rct_to_attacker
			RCT->report(attacker);
#endif
			d.damage = RCT->base_damage;
			d.damage += dice(RCT->damage_dice_count,RCT->damage_dice_sides);
			if(RCT->damage_percent_bonus) {
				d.damage += (0.01 * RCT->damage_percent_bonus) * d.damage;
			}
			d.headshot_damage = hsc.first;
			if(d.headshot_damage && RCT->headshot_bonus) {
				d.headshot_damage += RCT->headshot_bonus;
			}
			d.critical_damage = hsc.second;
			if(attacker->ghost()) {
				if(is_sniper_rifle(weapon)) {
					d.damage += ((0.105 * tier(attacker)) * d.damage);//TODO: find the values equiv to this
				}
				/** TODO: if enemy is tracked, it takes more damaged */
			}
			if(attacker->marine()) {
				if(is_assault_rifle(weapon)) {
					d.damage += (0.25 * d.damage); //TODO: find values equiv to this
					if(mods::rand::chance(10)) {//TODO values
						d.incendiary_damage += dice(tier(attacker) * 10,tier(attacker) * 10);//TODO values
					}
				}
				if(is_shotgun(weapon)) {
					if(distance > 1 && distance <= 3) { //TODO: values
						//TODO: need to define same room damage of shotgun
						d.damage += d.damage * distance;
					}
				}
			}
			if(attacker->breacher()) {
				d.explosive_damage += (0.10 * d.damage);
				if(is_shotgun(weapon)) {
					d.damage += (0.10 * d.damage);
				}
				if(is_smg(weapon)) {
					d.damage += (0.10 * d.damage);
					if(distance <= 1 && mods::rand::chance(BREACHER_SMG_SHOTGUN_CHANCE())) {
						d.shrapnel_damage += d.damage / 4; //TODO: make values
					}
					d.corrosive_damage += dice(tier(attacker) * 3, tier(attacker) * 6);//TODO values
				}
				/** TODO: roll chance to kockdown opponent */
			}


			if(RCT->incendiary_damage) {
				d.incendiary_damage += RCT->incendiary_damage;
			}
			if(RCT->explosive_damage) {
				d.explosive_damage += RCT->explosive_damage;
			}
			if(RCT->shrapnel_damage) {
				d.shrapnel_damage += RCT->shrapnel_damage;
			}
			if(RCT->corrosive_damage) {
				d.corrosive_damage += RCT->corrosive_damage;
			}
			if(RCT->cryogenic_damage) {
				d.cryogenic_damage += RCT->cryogenic_damage;
			}
			if(RCT->radioactive_damage) {
				d.radioactive_damage += RCT->radioactive_damage;
			}
			if(RCT->anti_matter_damage) {
				d.anti_matter_damage += RCT->anti_matter_damage;
			}
			if(RCT->emp_damage) {
				d.emp_damage += RCT->emp_damage;
			}
			if(RCT->shock_damage) {
				d.shock_damage += RCT->shock_damage;
			}
			if(RCT->incendiary_percent) {
				d.incendiary_damage += d.damage * (0.01 * RCT->incendiary_percent);
			}
			if(RCT->explosive_percent) {
				d.explosive_damage += d.damage * (0.01 * RCT->explosive_percent);
			}
			if(RCT->shrapnel_percent) {
				d.shrapnel_damage += d.damage * (0.01 * RCT->shrapnel_percent);
			}
			if(RCT->corrosive_percent) {
				d.corrosive_damage += d.damage * (0.01 * RCT->corrosive_percent);
			}
			if(RCT->cryogenic_percent) {
				d.cryogenic_damage += d.damage * (0.01 * RCT->cryogenic_percent);
			}
			if(RCT->radioactive_percent) {
				d.radioactive_damage += d.damage * (0.01 * RCT->radioactive_percent);
			}
			if(RCT->anti_matter_percent) {
				d.anti_matter_damage += d.damage * (0.01 * RCT->anti_matter_percent);
			}
			if(RCT->emp_percent) {
				d.emp_damage += d.damage * (0.01 * RCT->emp_percent);
			}
			if(RCT->shock_percent) {
				d.shock_damage += d.damage * (0.01 * RCT->shock_percent);
			}
			return d;
		}
		void apply_damage_to_victim(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon,calculated_damage_t& d) {
			/**
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
							*/
			auto& victim = found_target.target;
			auto& direction = found_target.direction;
			int dam = d.damage;
			dam += d.headshot_damage;
			dam += d.critical_damage;
			feedback_t feedback;
			mods::weapons::legacy::damage(attacker->cd(),victim->cd(),dam,mods::weapons::legacy::get_legacy_attack_type(weapon));
			feedback.attacker = attacker->uuid();
			if(dam == 0) {
				m_debug("damage is zero");
				feedback.damage = dam;
				feedback.hits = 0;

				feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
				attacker->damage_event(feedback);

				feedback.from_direction = OPPOSITE_DIR(direction);
				feedback.damage_event =de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
				victim->damage_event(feedback);

			} else if(dam > 0) {
				m_debug("damage greater than zero");
				feedback.hits = 1;
				feedback.damage = dam;
				feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
				feedback.from_direction = OPPOSITE_DIR(direction);
				feedback.attacker = attacker->uuid();
				victim->set_attacker(attacker->uuid());
				feedback.damage_event = de::HIT_BY_RIFLE_ATTACK;
				victim->damage_event(feedback);

				feedback.damage_event = de::YOU_INFLICTED_SNIPE_DAMAGE;
				attacker->damage_event(feedback);
				if(victim->is_npc()) {
					mods::mobs::damage_event::sniped(victim,feedback);
				}
				m_debug("checking disorient");
				if(attack_disorients(attacker,weapon,victim)) {
					m_debug("disorients");
					mods::affects::affect_player_for({mods::affects::affect_t::DISORIENT},victim,disorient_ticks(attacker,weapon,victim));
					feedback.damage_event= de::YOU_ARE_DISORIENTED_EVENT;
					/** TODO: maybe make this random to disorient the attacker ? >:) EVIL GENIUS */
					feedback.from_direction = OPPOSITE_DIR(direction);
					victim->damage_event(feedback);
					if(victim->is_npc()) {
						mods::mobs::damage_event::disoriented(victim,feedback);
					}

					feedback.damage_event= de::YOU_DISORIENTED_SOMEONE_EVENT;
					attacker->damage_event(feedback);
				}
				m_debug("remembering");
				remember_event(victim,attacker);
			}
			if(victim->position() == POS_DEAD) {
				mods::weapons::legacy::player_died(attacker,victim);
			}
			if(d.incendiary_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.incendiary_damage,ELEM_INCENDIARY);
			}
			if(d.explosive_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.explosive_damage,ELEM_EXPLOSIVE);
			}
			if(d.shrapnel_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.shrapnel_damage,ELEM_SHRAPNEL);
			}
			if(d.corrosive_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.corrosive_damage,ELEM_CORROSIVE);
			}
			if(d.cryogenic_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.cryogenic_damage,ELEM_CRYOGENIC);
			}
			if(d.radioactive_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.radioactive_damage,ELEM_RADIOACTIVE);
			}
			if(d.anti_matter_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.anti_matter_damage,ELEM_ANTI_MATTER);
			}
			if(d.emp_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.emp_damage,ELEM_EMP);
			}
			if(d.shock_damage) {
				mods::weapons::elemental::perform_elemental_damage(attacker,victim,d.shock_damage,ELEM_SHOCK);
			}
#ifdef report_rct_to_attacker
			RCT->report(attacker);
#endif

		}
	};//end namespace phases


	bool can_snipe(player_ptr_t& attacker,obj_ptr_t weapon) {
		if(!weapon) {
			attacker->damage_event(feedback_t(de::NO_PRIMARY_WIELDED_EVENT));
			m_debug("no primary!");
			return false;
		}
		if(!weapon->has_rifle()) {
			attacker->damage_event(feedback_t(de::NO_PRIMARY_WIELDED_EVENT));
			m_debug("no primary rifle(2)");
			return false;
		}
		return true;
	}

	/**
	 * @brief subtracts 1 from weapon ammo
	 *
	 * @param weapon
	 */
	void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) {
		/** TODO: if weapon has a bullet printer mod, calculate ammo */
		if(weapon->rifle_instance->ammo == 0) {
			return;
		}
		int16_t deduct = 0;
		switch((mw_rifle)weapon->rifle()->attributes->type) {
			case mw_rifle::LIGHT_MACHINE_GUN:
				deduct = mods::values::SINGLE_SHOT_LIGHT_MACHINE_GUN();
				break;

			case mw_rifle::SUB_MACHINE_GUN:
				deduct = mods::values::SINGLE_SHOT_SUB_MACHINE_GUN();
				break;

			case mw_rifle::SHOTGUN:
				deduct = mods::values::SINGLE_SHOT_SHOTGUN();
				break;

			case mw_rifle::SNIPER:
				deduct = mods::values::SINGLE_SHOT_SNIPER();
				break;

			case mw_rifle::ASSAULT_RIFLE:
				deduct = mods::values::SINGLE_SHOT_ASSAULT_RIFLE();
				break;
			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				deduct = mods::values::SINGLE_SHOT_HANDGUN();
				break;
			case mw_rifle::MACHINE_PISTOL:
				deduct = mods::values::SINGLE_SHOT_MACHINE_PISTOL();
				break;
			default:
				deduct = 1;
				log("SYSERR: warning, no rifle type given for decrease_single_shot_ammo, default to 1");
				break;
		}
		int ammo = weapon->rifle_instance->ammo - ammunition::reduce_ammo(attacker,weapon,deduct);
		if(ammo < 0) {
			weapon->rifle_instance->ammo = 0;
		} else {
			weapon->rifle_instance->ammo = ammo;
		}
	}
	/**
	 * Handles both ranged and immediate targets
	 */
	void snipe_target(player_ptr_t& attacker,phases::target_t target, obj_ptr_t& weapon) {
		/**
		 * First check that the weapon can snipe
		 */
		if(!can_snipe(attacker,weapon)) {
			/** can_snipe sends DE messages for us */
			return;
		}
		/**
		 * Phase 1: Target acquisition.
		 */
		auto opt_target = phases::acquire_ranged_target(attacker, target,weapon);
		bool cant_find_target = !opt_target.has_value();

		if((opt_target.has_value() && opt_target.value().target == nullptr) || cant_find_target) {
			m_debug("couldn't find target!");
			attacker->damage_event(feedback_t(de::COULDNT_FIND_TARGET_EVENT));
			m_debug(feedback.dump());
			return;
		}

		auto found_target = opt_target.value();
		auto& victim = found_target.target;

		if(mods::rooms::is_peaceful(attacker->room())) {
			attacker->damage_event(feedback_t(de::YOURE_IN_PEACEFUL_ROOM));
			m_debug("is_peaceful room");
			return;
		}

		if(mods::rooms::is_peaceful(victim->room())) {
			attacker->damage_event(feedback_t(de::TARGET_IN_PEACEFUL_ROOM_EVENT));
			m_debug("target is in peaceful room");
			return;
		}

		if(!attacker->can_attack_again()) {
			attacker->damage_event(feedback_t(de::COOLDOWN_IN_EFFECT_EVENT));
			m_debug("can't attack again");
			return;
		}

		/* Check ammo */
		if(get_ammo(weapon) == 0) {
			attacker->damage_event(feedback_t(de::OUT_OF_AMMO_EVENT));
			m_debug("out of ammo");
			return;
		}

		/**
		 * Phase 2: accuracy roll
		 */
		if(!roll_accuracy(attacker,found_target,weapon)) {
			attacker->damage_event(feedback_t(de::YOU_MISSED_YOUR_TARGET_EVENT));
			victim->damage_event(feedback_t(de::GUNFIRE_WHIZZED_BY_FROM,OPPOSITE_DIR(target.direction)));
			decrease_single_shot_ammo(attacker,weapon);
			m_debug("Roll accuracy check failed for " << attacker->name());
			return;
		}

		/**
		 * Phase 3: headshot + critical roll
		 */
		auto extra_damage = roll_critical(attacker,found_target,weapon);

		/**
		 * Phase 4: Calculate damage
		 */
		auto damage = calculate_weapon_damage(attacker,found_target,weapon,extra_damage);

		/**
		 * Phase 5: Apply damage to victim
		 */
		apply_damage_to_victim(attacker,found_target,weapon,damage);
		perform_cleanup(attacker,found_target,weapon);

	}
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		snipe_target(attacker,phases::target_t(target,direction),weapon);
	}
};//end namespace combat_composer
