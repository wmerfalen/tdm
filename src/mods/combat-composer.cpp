#include "combat-composer.hpp"
#include "rifle.hpp"
#include "scan.hpp"
#include "player.hpp"
#include "ranged-combat-totals.hpp"
#include "damage-event.hpp"
#include "rooms.hpp"
#include "object-utils.hpp"

#ifdef __MENTOC_SHOW_DAMAGE_TYPES_DEBUG__
#define m_debug(A) std::cerr << green_str("combat_composer_debug:") << A << "\n";
#else
#define m_debug(A)
#endif
namespace mods::combat_composer {
	using vpd = mods::scan::vec_player_data;
	using de = damage_event_t;
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
	 * Handles spraying only
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {

	}

	/**
	 * Handles launching only
	 */
	void launch_towards(player_ptr_t& attacker,direction_t direction,uint8_t distance,obj_ptr_t& weapon) {

	}
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
			uint16_t damage;
			uint16_t headshot_damage;
			uint16_t critical_damage;
			uint16_t incendiary_damage;
			uint16_t explosive_damage;
			uint16_t shrapnel_damage;
			uint16_t corrosive_damage;
			uint16_t cryogenic_damage;
			uint16_t radioactive_damage;
			uint16_t emp_damage;
			uint16_t shock_damage;
			uint16_t anti_matter_damage;
		};
		calculation_t calculate_range(player_ptr_t& attacker,obj_ptr_t& weapon);

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

		/**
		 * Usage:
		 * -- roll accuracy check for sniping target 2 rooms away using primary weapon --
		 * auto status = roll_accuracy(attacker,{"drone",NORTH,2},attacker->primary());
		 * if(!status){
		 * 		attacker->sendln("You aim and MISS your target!");
		 * 		return;
		 * }
		 */
		bool roll_accuracy(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon);

		/**
		 * Usage:
		 * -- calculate headshot, critical chance --
		 * auto crits = roll_critical(attacker,{"drone",NORTH,2},attacker->primary());
		 *
		 */

		/** Rolls headshot and critical damage (respectively) */
		std::pair<int16_t,int16_t> roll_critical(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon);

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


		calculation_t calculate_range(player_ptr_t& attacker,obj_ptr_t& weapon) {
			calculation_t c;
			using namespace mods::rifle;
			auto zoom = get_zoom_magnification(weapon);
			c.max_range = get_max_range(weapon);
			c.critical_range = get_critical_range(weapon);
			auto range_mult = get_range_multiplier(weapon);
			if(range_mult > 0.0) {
				c.max_range *= range_mult;
			}
			if(zoom > 0.0) {
				c.max_range *= zoom;
			}
			c.effective_range = {get_effective_firing_range(weapon),c.critical_range};
			c.max_range += 0.05 * attacker->weapon_handling();
			c.critical_range -= 0.05 * attacker->sniping();
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


		std::optional<acquired_target_t> acquire_ranged_target(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon) {
			auto rct = attacker->calculate_ranged_combat_totals(weapon);
			vpd scan;
			/**
			 * TODO: will have to change find_type_t::ANY to include DEAD for contagion's
			 * corpse sniping capabilities
			 */
			mods::scan::los_scan_direction(attacker->cd(),rct->max_range,&scan,target.direction,mods::scan::find_type_t::NPC_AND_PLAYER);
			obj_ptr_t victim = nullptr;
			if(attacker->ghost()) {
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
			//auto max_range = mods::weapons::damage_calculator::max_range(attacker,weapon);

			acquired_target_t found;
			for(auto&& scanned_target : scan) {
				if(mods::util::fuzzy_match(target.target_name,scanned_target.obj->name.c_str())) {
					if(scanned_target.distance > rct->max_range) {
						attacker->sendln("That target is out of range!");
						return std::nullopt;
					}
					found.target = ptr(scanned_target.ch);
					found.direction = target.direction;
					found.distance = scanned_target.distance;
					return found;
				}
			}

			return std::nullopt;
		}

		std::vector<acquired_target_t> acquire_room(player_ptr_t& attacker,target_t target,obj_ptr_t& weapon) {
			std::vector<acquired_target_t> targets;
			auto calculation = calculate_range(attacker,weapon);
			vpd scan;
			mods::scan::los_scan_direction(attacker->cd(),calculation.max_range,&scan,target.direction,mods::scan::find_type_t::ALIVE);

			return targets;
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
	 * Handles both ranged and immediate targets
	 */
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,obj_ptr_t& weapon) {

		/**
		 * Phase 1: Target acquisition.
		 */
		auto opt_target = acquire_ranged_target(attacker, {target,direction},weapon);
		bool cant_find_target = !opt_target.has_value();

		if(cant_find_target) {
			m_debug("couldn't find target!");
			attacker->damage_event(feedback_t(de::COULDNT_FIND_TARGET_EVENT));
			m_debug(feedback.dump());
			return;
		}
		auto victim = std::move(opt_target.value().target);

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
		if(mods::object_utils::get_ammo(weapon) == 0) {
			attacker->damage_event(feedback_t(de::OUT_OF_AMMO_EVENT));
			m_debug("out of ammo");
			return;
		}

		/**
		 * Phase 2: accuracy roll
		 */

		//feedback_t feedback;
		//feedback.from_direction = OPPOSITE_DIR(direction);
		//feedback.attacker = attacker->uuid();

	}
};//end namespace combat_composer
