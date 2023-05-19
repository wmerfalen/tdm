#include "spray-target.hpp"
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
#include "../classes/includes.hpp"
#include "../levels.hpp"

#include "../../fight.hpp"
#include "../mobs/damage-event.hpp"
#include "../weapons/elemental.hpp"
#include "../weapons/legacy-combat.hpp"
#include "../weapons/unique-weapons.hpp"
#include "../armor/unique-armor.hpp"
#include "../interpreter.hpp"
#include "../suppress.hpp"
#include "skill-increment.hpp"
#include "shared.hpp"

namespace mods::combat_composer {
	namespace spray_state {
		static player_ptr_t attacker;
		static std::shared_ptr<::mods::ranged_combat_totals> current;
		static direction_t from_direction;
#ifdef RCT
	#undef RCT
	#undef ATKR
	#undef STATE_FROM_DIR
#endif

#define RCT spray_state::current
#define ATKR spray_state::attacker
#define STATE_FROM_DIR spray_state::from_direction
#define INIT_RCT() \
	RCT = attacker->calculate_ranged_combat_totals(weapon);\
	ATKR = attacker;\
	STATE_FROM_DIR = OPPOSITE_DIR(direction);
	};

	namespace phases {
		std::forward_list<player_ptr_t> collect_spray_targets(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon);
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

		/*

		   1) Calculate damage (including elemental)
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

		   2) Calculate damage applied to victim
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

			3) Calculate armor penetration amount
				- Affect HP of armor with respect to armor durability profile

		   4) Consume ammunition or stats (if needed)
			 		yaml file attributes
			 			attachments:
							- free ammo chance
							- regenerate ammo chance
		   		 MARINE abilities:
		   			Assault rifles:
		   				- 15% free ammunition

		 */
		using target_list_t = std::vector<player_ptr_t>;
		calculated_damage_t calculate_weapon_damage(player_ptr_t& attacker,const target_list_t& targets,obj_ptr_t& weapon);

		void perform_cleanup(player_ptr_t& attacker,obj_ptr_t& weapon) {
			feedback_t feedback;
			m_debug("decreasing ammo");
			mods::combat_composer::phases::decrease_spray_shot_ammo(attacker,weapon);
			attacker->set_fight_timestamp();
			mods::combat_composer::phases::set_player_spray_weapon_cooldown(attacker,weapon);
			mods::combat_composer::skill_increment::increment(attacker,weapon);
		}

		std::forward_list<player_ptr_t> collect_spray_targets(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {
			std::forward_list<player_ptr_t> collection;
			vpd scan;
			/**
			 * TODO: will have to change find_type_t::ANY to include DEAD for contagion's
			 * corpse sniping capabilities
			 */
			mods::scan::los_scan_direction(attacker->cd(),RCT->max_range,&scan,direction);//,mods::scan::find_type_t::NPC_AND_PLAYER);
			for(const auto& scanned_target : scan) {
				player_ptr_t victim = nullptr;
				if(scanned_target.ch && scanned_target.uuid) {
					victim = ptr_by_uuid(scanned_target.uuid);
				}
				if(!victim) {
					continue;
				}
				if(IS_JEFE(victim)) {
					continue;
				}
				collection.push_front(victim);
			}
			return collection;
		}

		using room_damage_t = std::map<uuid_t,calculated_damage_t>;
		room_damage_t calculate_weapon_damage(player_ptr_t& attacker,direction_t direction,obj_ptr_t& weapon) {
			room_damage_t room_damages;
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
#ifdef report_rct_to_attacker
			RCT->report(attacker);
#endif
			if(is_sniper_rifle(weapon)) {
				return room_damages;
			}
			uint8_t max_targets_hit = tier(attacker) + 2;
			uint8_t targets_hit = 0;
			for(const auto& target : collect_spray_targets(attacker,direction,weapon)) {
				if(targets_hit++ >= max_targets_hit) {
					break;
				}
				calculated_damage_t d;
				d.damage = RCT->base_damage / SPRAY_BASE_DAMAGE_DIVISOR();
				d.damage += dice(RCT->damage_dice_count / SPRAY_DICE_COUNT_DIVISOR(),RCT->damage_dice_sides / SPRAY_DICE_SIDES_DIVISOR());
				if(RCT->damage_percent_bonus) {
					d.damage += (0.01 * RCT->damage_percent_bonus) * d.damage;
				}
				/** Headshots and criticals are NOT possible when spraying */
				d.headshot_damage = 0;
				d.critical_damage = 0;
				if(attacker->ghost()) {
					/** TODO: if enemy is tracked, it takes more damaged */
				}
				if(attacker->marine()) {
					if(is_assault_rifle(weapon)) {
						d.damage += (MARINE_AR_SPRAY_BASE_DAMAGE_MULTIPLIER() * d.damage);
						if(mods::rand::chance(MARINE_AR_SPRAY_INCENDIARY_CHANCE())) {
							d.incendiary_damage += dice(tier(attacker) * MARINE_AR_SPRAY_INC_TIER_DICE_COUNT_MULTIPLIER(),tier(attacker) * MARINE_AR_SPRAY_INC_TIER_DICE_SIDES_MULTIPLIER());
						}
					}
					if(is_lmg(weapon)) {
						d.damage += (MARINE_AR_SPRAY_BASE_DAMAGE_MULTIPLIER() * d.damage);
						if(mods::rand::chance(MARINE_AR_SPRAY_INCENDIARY_CHANCE())) {
							d.incendiary_damage += dice(tier(attacker) * MARINE_AR_SPRAY_INC_TIER_DICE_COUNT_MULTIPLIER(),tier(attacker) * MARINE_AR_SPRAY_INC_TIER_DICE_SIDES_MULTIPLIER());
						}
					}
					if(is_shotgun(weapon)) {
						d.damage += d.damage * MARINE_AR_SPRAY_SHOTGUN_BONUS();
					}
				}
				if(attacker->breacher()) {
					d.explosive_damage += (BREACHER_SPRAY_EXPLOSIVE_DAMAGE_MULTIPLIER() * d.damage);
					if(is_shotgun(weapon)) {
						d.damage += (BREACHER_SPRAY_SHOTGUN_BONUS_DAMAGE_MULTIPLIER() * d.damage);
					}
					if(is_smg(weapon)) {
						d.damage += (BREACHER_SPRAY_SMG_DAMAGE_MULTIPLIER() * d.damage);
						if(mods::rand::chance(BREACHER_SMG_SHOTGUN_CHANCE())) {
							d.shrapnel_damage += d.damage / BREACHER_SMG_SHRAPNEL_DAMAGE_DIVISOR();
						}
						d.corrosive_damage += dice(tier(attacker) * BREACHER_SMG_CORROSIVE_TIER_DICE_COUNT_MULTIPLIER(), tier(attacker) * BREACHER_SMG_CORROSIVE_TIER_DICE_SIDES_MULTIPLIER());
					}
					/** TODO: roll chance to kockdown opponent */
				}
				static const float spray_elemental_divisor = SPRAY_ELEMENTAL_DAMAGE_DIVISOR();


				if(RCT->incendiary_damage) {
					d.incendiary_damage += RCT->incendiary_damage;
					d.incendiary_damage /= spray_elemental_divisor;
				}
				if(RCT->explosive_damage) {
					d.explosive_damage += RCT->explosive_damage;
					d.explosive_damage /= spray_elemental_divisor;
				}
				if(RCT->shrapnel_damage) {
					d.shrapnel_damage += RCT->shrapnel_damage;
					d.shrapnel_damage /= spray_elemental_divisor;
				}
				if(RCT->corrosive_damage) {
					d.corrosive_damage += RCT->corrosive_damage;
					d.corrosive_damage /= spray_elemental_divisor;
				}
				if(RCT->cryogenic_damage) {
					d.cryogenic_damage += RCT->cryogenic_damage;
					d.cryogenic_damage /= spray_elemental_divisor;
				}
				if(RCT->radioactive_damage) {
					d.radioactive_damage += RCT->radioactive_damage;
					d.radioactive_damage /= spray_elemental_divisor;
				}
				if(RCT->anti_matter_damage) {
					d.anti_matter_damage += RCT->anti_matter_damage;
					d.anti_matter_damage /= spray_elemental_divisor;
				}
				if(RCT->emp_damage) {
					d.emp_damage += RCT->emp_damage;
					d.emp_damage /= spray_elemental_divisor;
				}
				if(RCT->shock_damage) {
					d.shock_damage += RCT->shock_damage;
					d.shock_damage /= spray_elemental_divisor;
				}
				if(RCT->incendiary_percent) {
					d.incendiary_damage += d.damage * (0.01 * RCT->incendiary_percent);
					d.incendiary_damage /= spray_elemental_divisor;
				}
				if(RCT->explosive_percent) {
					d.explosive_damage += d.damage * (0.01 * RCT->explosive_percent);
					d.explosive_damage /= spray_elemental_divisor;
				}
				if(RCT->shrapnel_percent) {
					d.shrapnel_damage += d.damage * (0.01 * RCT->shrapnel_percent);
					d.shrapnel_damage /= spray_elemental_divisor;
				}
				if(RCT->corrosive_percent) {
					d.corrosive_damage += d.damage * (0.01 * RCT->corrosive_percent);
					d.corrosive_damage /= spray_elemental_divisor;
				}
				if(RCT->cryogenic_percent) {
					d.cryogenic_damage += d.damage * (0.01 * RCT->cryogenic_percent);
					d.cryogenic_damage /= spray_elemental_divisor;
				}
				if(RCT->radioactive_percent) {
					d.radioactive_damage += d.damage * (0.01 * RCT->radioactive_percent);
					d.radioactive_damage /= spray_elemental_divisor;
				}
				if(RCT->anti_matter_percent) {
					d.anti_matter_damage += d.damage * (0.01 * RCT->anti_matter_percent);
					d.anti_matter_damage /= spray_elemental_divisor;
				}
				if(RCT->emp_percent) {
					d.emp_damage += d.damage * (0.01 * RCT->emp_percent);
					d.emp_damage /= spray_elemental_divisor;
				}
				if(RCT->shock_percent) {
					d.shock_damage += d.damage * (0.01 * RCT->shock_percent);
					d.shock_damage /= spray_elemental_divisor;
				}
				room_damages[target->uuid()] = d;
			}
			return room_damages;
		}
		void apply_damage_to_victim(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon,calculated_damage_t d) {
			if(IS_JEFE(victim)) {
				return;
			}
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

				feedback.from_direction = STATE_FROM_DIR;
				feedback.damage_event =de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
				victim->damage_event(feedback);

				mods::weapons::dispatch_unique_ranged_weapon_event(attacker->uuid(), damage_event_t::YOU_MISSED_YOUR_TARGET_EVENT, victim);
			} else if(dam > 0) {
				m_debug("damage greater than zero");
				feedback.hits = 1;
				feedback.damage = dam;
				feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
				feedback.from_direction = STATE_FROM_DIR;
				feedback.attacker = attacker->uuid();
				victim->set_attacker(attacker->uuid());
				feedback.damage_event = de::HIT_BY_RIFLE_ATTACK;
				victim->damage_event(feedback);

				feedback.damage_event = de::YOU_INFLICTED_SNIPE_DAMAGE;
				attacker->damage_event(feedback);

				mods::weapons::dispatch_unique_ranged_weapon_event(attacker->uuid(), damage_event_t::YOU_HIT_ARMOR,victim);
				mods::armor::dispatch_unique_armor_event(attacker->uuid(), damage_event_t::YOU_HIT_ARMOR, victim);
				mods::weapons::dispatch_unique_ranged_weapon_event(attacker->uuid(), damage_event_t::YOU_HIT_FLESH,victim);
				if(victim->is_npc()) {
					// FIXME: wrongly dispatching sniped event
					mods::mobs::damage_event::sniped(victim,feedback);
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


	bool can_spray(player_ptr_t& attacker,obj_ptr_t weapon) {
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
		if(weapon->rifle() && weapon->rifle()->attributes->type == mw_rifle::SNIPER) {
			return false;
		}
		if(mods::rooms::is_peaceful(attacker->room())) {
			attacker->damage_event(feedback_t(de::YOURE_IN_PEACEFUL_ROOM));
			m_debug("is_peaceful room");
			return false;
		}
		if(weapon->rifle_instance->ammo == 0) {
			attacker->damage_event(feedback_t(de::OUT_OF_AMMO_EVENT));
			mods::weapons::dispatch_unique_ranged_weapon_event(attacker->uuid(), damage_event_t::YOUR_CLIP_IS_DEPLETED,nullptr);
			m_debug("Out of ammo!");
			return false;
		}
		return true;
	}

	/**
	 * Handles both ranged and immediate targets
	 */
	void spray_target(player_ptr_t& attacker,direction_t direction, obj_ptr_t& weapon) {
		INIT_RCT();
		/**
		 * First check that the weapon can spray
		 */
		if(!can_spray(attacker,weapon)) {
			/** can_spray sends DE messages for us */
			return;
		}

		auto room_damage_map = mods::combat_composer::phases::calculate_weapon_damage(attacker,direction,weapon);
		for(const auto& pair : room_damage_map) {
			player_ptr_t victim = nullptr;
			victim = nullptr;
			victim = ptr_by_uuid(pair.first);
			if(!victim) {
				continue;
			}
			if(IS_JEFE(victim)) {
				continue;
			}
			/**
			 * FIXME: This may need to have some severe balancing
			 */
			mods::weapons::dispatch_unique_ranged_weapon_event(attacker->uuid(), damage_event_t::PULL_TRIGGER, victim);
			/**
			 * Phase 2: Apply damage to victim
			 */
			mods::combat_composer::phases::apply_damage_to_victim(attacker,victim,weapon,pair.second);

			if(attacker->marine() && attacker->marine()->is_surpressing(pair.first)) {
				mods::suppress::suppress_for(pair.first, tier(attacker) * MARINE_SUPPRESSION_TICKS_PER_TIER());
			}
		}
		mods::combat_composer::phases::perform_cleanup(attacker,weapon);
	}

};//end namespace combat_composer
#undef RCT
#undef ATKR
#undef STATE_FROM_DIR
#undef INIT_RCT
