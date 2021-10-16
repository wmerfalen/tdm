#include "combat-composer.hpp"
#include "rifle.hpp"
#include "scan.hpp"
#include "player.hpp"
#include "ranged-combat-totals.hpp"
#include "damage-event.hpp"
#include "rooms.hpp"
#include "object-utils.hpp"
#include "player-utils.hpp"
#include "calc-visibility.hpp"
#include "classes/ghost.hpp"
#include "levels.hpp"

#include "../fight.hpp"
#include "mobs/damage-event.hpp"

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
		bool roll_accuracy(player_ptr_t& attacker,acquired_target_t& target,obj_ptr_t& weapon);
		std::pair<uint16_t,uint16_t> roll_critical(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon);
		calculated_damage_t calculate_weapon_damage(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon,std::pair<uint16_t,uint16_t> hsc);

		namespace state {
			static std::shared_ptr<mods::ranged_combat_totals> current;
#define RCT state::current
		};
		namespace legacy {
			void solo_gain(player_ptr_t& attacker,player_ptr_t& victim);
			void death_cry(char_data *ch) {
				int door;

				act("\r\nYour blood freezes as you hear $n's death cry.", FALSE, ch, 0, 0, TO_ROOM);

				for(door = 0; door < NUM_OF_DIRS; door++) {
					std::string from_dir = mods::projectile::fromdirstr(OPPOSITE_DIR(door),true,true) + "\r\n";
					if(CAN_GO(ch, door)) {
						std::string s = "Your blood freezes as you hear someone's death cry " + from_dir;
						send_to_room(world[IN_ROOM(ch)].dir_option[door]->to_room, s.c_str());
					}
				}
			}

			void die(char_data *ch) {
				auto p = ptr(ch);
				mods::levels::reduce_exp_from_dying(p);
				p->get_affect_dissolver().clear_all();
				mods::affects::player_died(p);
				auto uuid = p->uuid();

				if(!IS_NPC(ch)) {
					REMOVE_BIT(PLR_FLAGS(ch), PLR_KILLER | PLR_THIEF);
				}

				death_cry(ch);

				mods::corpse::make_corpse(p);
				extract_char_final(ch);
				mods::corrosive::unqueue_player(uuid);
			}

			void die(char_data* killer,char_data *victim) {
				/* check if mob death trigger is active */

				if(victim->drone) {
					auto room = IN_ROOM(victim);
					char_from_room(victim);
					mods::drone::stop(mods::globals::player_list[victim->drone_owner]->uuid());
					send_to_room(room,"A drone is destroyed.");
					char_to_room(victim,NOWHERE);
				}
				if(FIGHTING(killer) == victim) {
					stop_fighting(killer);
				}

				auto p = ptr(killer);
				auto v = ptr(victim);


				if(killer && !IS_NPC(killer)) {
					mods::loot::events::player_killed_npc(p, v);
				}
				die(victim);
				if(killer && !IS_NPC(killer)) {
					solo_gain(p,v);
				}
			}
			void solo_gain(player_ptr_t& attacker,player_ptr_t& victim) {
				attacker->sendln(CAT("{grn}You gain ",mods::levels::gain_exp_from_killing(attacker, victim)," experience points.{/grn}"));
				auto mp = mods::levels::gain_mp_from_killing(attacker, victim);
				if(mp) {
					attacker->sendln(CAT("{grn}You gain ",mp," mission points.{/grn}"));
				}
				change_alignment(attacker->cd(), victim->cd());
			}


			int step_one(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					m_debug("player is super user (from step_one)");
					return 0;
				}
#endif
				/* peaceful rooms */
				if(ch && ch != victim && ROOM_FLAGGED(IN_ROOM(ch), ROOM_PEACEFUL)) {
					send_to_char(ch, "This room just has such a peaceful, easy feeling...\r\n");
					return (0);
				}

				/* shopkeeper protection */
#if 0
				/** THIS IS BROKEN WITH CAR THIEF AND POSSIBLY OTHER NPCS */
				if(ch && !ok_damage_shopkeeper(ch, victim)) {
					m_debug("cannot damage shopkeeper");
					return (0);
				}
#endif
				m_debug("step_one okay");
				return 1;
			}
			int step_two(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					m_debug("player is super user (step 2)");
					return 0;
				}
#endif
				/* You can't damage an immortal! */
				if(!IS_NPC(victim) && (GET_LEVEL(victim) >= LVL_IMMORT)) {
					m_debug("victim isnt npc and victim >= IMMORT");
					dam = 0;
				}
				if(victim != ch) {
					/* Start the attacker fighting the victim */
					if(GET_POS(ch) > POS_STUNNED && (FIGHTING(ch) == NULL)) {
						m_debug("set fighting");
						set_fighting(ch, victim);
					}

					/* Start the victim fighting the attacker */
					if(GET_POS(victim) > POS_STUNNED && (FIGHTING(victim) == NULL)) {
						set_fighting(victim, ch);

						if(MOB_FLAGGED(victim, MOB_MEMORY) && !IS_NPC(ch)) {
							remember(victim, ch);
						}
					}
					/* If you attack a pet, it hates your guts */
					if(victim->master == ch) {
						stop_follower(victim);
					}

					/* If the attacker is invisible, he becomes visible */
					if(AFF_FLAGGED(ch, AFF_INVISIBLE | AFF_HIDE)) {
						m_debug("appear");
						appear(ch);
					}
					m_debug("step_two okay");
					return 1;
				}
				return -1;
			}
			int get_damage(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					return 0;
				}
#endif
				/* Cut damage in half if victim has sanct, to a minimum 1 */
				if(AFF_FLAGGED(victim, AFF_SANCTUARY) && dam >= 2) {
					dam /= 2;
				}

				/* Check for PK if this is not a PK MUD */
				if(!pk_allowed) {
					check_killer(ch, victim);

					if(PLR_FLAGGED(ch, PLR_KILLER) && (ch != victim)) {
						dam = 0;
					}
				}
				return dam;
			}
			void deal_damage(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					return;
				}
#endif
				if(vplayer) {
					vplayer->hp() -= dam;
				}
			}

			void update_position(char_data* ch) {
				update_pos(ch);
			}

			void send_combat_messages(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					return;
				}
#endif
				MENTOC_PREAMBLE();
				/*
				 * skill_message sends a message from the messages file in lib/misc.
				 * dam_message just sends a generic "You hit $n extremely hard.".
				 * skill_message is preferable to dam_message because it is more
				 * descriptive.
				 *
				 * If we are _not_ attacking with a weapon (i.e. a spell), always use
				 * skill_message. If we are attacking with a weapon: If this is a miss or a
				 * death blow, send a skill_message if one exists; if not, default to a
				 * dam_message. Otherwise, always send a dam_message.
				 */
				if(ch) {
					if(!IS_WEAPON(attacktype)) {
						skill_message(dam, ch, victim, attacktype);
					} else {
						if(GET_POS(victim) == POS_DEAD || dam == 0) {
							if(!skill_message(dam, ch, victim, attacktype)) {
								dam_message(dam, ch, victim, attacktype);
							}
						} else {
							dam_message(dam, ch, victim, attacktype);
						}
					}
				}
				/* Use send_to_char -- act() doesn't send message if you are DEAD. */
				switch(GET_POS(victim)) {
					case POS_MORTALLYW:
						act("$n is mortally wounded, and will die soon, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
						send_to_char(victim, "You are mortally wounded, and will die soon, if not aided.\r\n");
						break;

					case POS_INCAP:
						act("$n is incapacitated and will slowly die, if not aided.", TRUE, victim, 0, 0, TO_ROOM);
						send_to_char(victim, "You are incapacitated an will slowly die, if not aided.\r\n");
						break;

					case POS_STUNNED:
						act("$n is stunned, but will probably regain consciousness again.", TRUE, victim, 0, 0, TO_ROOM);
						send_to_char(victim, "You're stunned, but will probably regain consciousness again.\r\n");
						break;

					case POS_DEAD:
						act("$n is dead!  R.I.P.", FALSE, victim, 0, 0, TO_ROOM);
						send_to_char(victim, "You are dead!  Sorry...\r\n");
						break;

					default:			/* >= POSITION SLEEPING */
						if(dam > (GET_MAX_HIT(victim) / 4)) {
							send_to_char(victim, "That really did HURT!\r\n");
						}

						if(GET_HIT(victim) < (GET_MAX_HIT(victim) / 4)) {
							send_to_char(victim, "{red}You wish that your wounds would stop BLEEDING so much!{/red}\r\n");

							if(ch != victim && MOB_FLAGGED(victim, MOB_WIMPY)) {
								do_flee(victim, NULL, 0, 0,player);
							}
						}

						if(!IS_NPC(victim) && GET_WIMP_LEV(victim) && (victim != ch) &&
						        GET_HIT(victim) < GET_WIMP_LEV(victim) && GET_HIT(victim) > 0) {
							send_to_char(victim, "You wimp out, and attempt to flee!\r\n");
							do_flee(victim, NULL, 0, 0,player);
						}

						break;
				}
			}
			void help_linkless(char_data *ch, char_data *victim, int dam, int attacktype) {
				MENTOC_PREAMBLE();
				/* Help out poor linkless people who are attacked */
				if(!IS_NPC(victim) && !(victim->has_desc) && GET_POS(victim) > POS_STUNNED) {
					do_flee(victim, NULL, 0, 0,player);

					if(!FIGHTING(victim)) {
						act("$n is rescued by divine forces.", FALSE, victim, 0, 0, TO_ROOM);
						GET_WAS_IN(victim) = IN_ROOM(victim);
						char_from_room(victim);
						char_to_room(victim, 0);
					}
				}
			}
			int perform_damage_cleanup(char_data *ch, char_data *victim, int dam, int attacktype) {
				auto victim_ptr = ptr(victim);
				auto attacker = ptr(ch);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(victim_ptr)) {
					return 0;
				}
#endif
				/* stop someone from fighting if they're stunned or worse */
				if(ch && GET_POS(victim) <= POS_STUNNED && FIGHTING(victim) != NULL) {
					stop_fighting(victim);
				}

				return dam;
			}
			int damage(char_data *attacker, char_data *victim, int dam, int attacktype) {
				auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
				if(mods::super_users::player_is(vplayer)) {
					m_debug("player is super user");
					return -1;
				}
#endif
				if(step_one(attacker,victim,dam,attacktype) <= 0) {
					m_debug("step one failed");
					return -2;
				}
				if(step_two(attacker,victim,dam,attacktype) <= 0) {
					m_debug("step two failed");
					return -3;
				}
				dam = get_damage(attacker,victim,dam,attacktype);
				m_debug("get_damage:" << dam);
				deal_damage(attacker,victim,dam,attacktype);
				m_debug("deal_damage'd");
				auto p = ptr(attacker);
				m_debug("updating pos of victim");
				update_position(victim);
				send_combat_messages(attacker,victim,dam,attacktype);
				help_linkless(attacker,victim,dam,attacktype);
				return perform_damage_cleanup(attacker,victim,dam,attacktype);
			}
			void player_died(player_ptr_t& attacker,player_ptr_t& victim) {
				m_debug("target dead!");
				feedback_t feedback;
				feedback.damage_event= de::TARGET_DEAD_EVENT;
				feedback.attacker = victim->uuid();
				attacker->damage_event(feedback);
				stop_fighting(attacker->cd());
				stop_fighting(victim->cd());
				die(attacker->cd(),victim->cd());
			}
			void player_died(player_ptr_t& victim) {
				stop_fighting(victim->cd());
				die(victim->cd());
			}
			int get_legacy_attack_type(obj_ptr_t& weapon) {
				if(weapon->has_rifle()) {
					switch((mw_rifle)weapon->rifle()->attributes->type) {
						case mw_rifle::SNIPER:
							return TYPE_SNIPE;
						case mw_rifle::SHOTGUN:
							return TYPE_SHOTGUN;
						case mw_rifle::SUB_MACHINE_GUN:
							return TYPE_SUB_MACHINE_GUN;
						case mw_rifle::LIGHT_MACHINE_GUN:
							return TYPE_LIGHT_MACHINE_GUN;
						default:
							log("SYSERR: invalid rifle type");
							return TYPE_SNIPE;
					}
				}
				if(weapon->has_melee()) {
					switch((mw_melee)weapon->melee()->attributes->type) {
						case mw_melee::MACHETE:
						case mw_melee::KNIFE:
						case mw_melee::AXE:
						case mw_melee::KATANA:
						case mw_melee::LONGSWORD:
						case mw_melee::SCYTHE:
						case mw_melee::CHAINSAW:
							return TYPE_SLASH;

						case mw_melee::PICKAXE:
						case mw_melee::NAGINATA:
						case mw_melee::DAGGER:
						case mw_melee::SHURIKEN:
						case mw_melee::SPEAR:
						case mw_melee::SICKLE:
						case mw_melee::ICE_PICK:
						case mw_melee::PIKE:
						case mw_melee::HALBERD:
						case mw_melee::LANCE:
						case mw_melee::TRIDENT:
							return TYPE_STAB;

						case mw_melee::CROWBAR:
						case mw_melee::CHAIN:
						case mw_melee::WOODEN_BASEBALL_BAT:
						case mw_melee::ALUMINUM_BASEBALL_BAT:
						case mw_melee::HAMMER:
						case mw_melee::SLEDGEHAMMER:
						case mw_melee::BATON:
						case mw_melee::HOCKEY_STICK:
						case mw_melee::SHOVEL:
						case mw_melee::MACE:
						case mw_melee::NUNCHAKU:
						case mw_melee::BRASS_KNUCKLES:
						case mw_melee::QUARTERSTAFF:
						case mw_melee::REBAR:
							return TYPE_BLUDGEON;
						default:
							break;
					}
				}
				return TYPE_HIT;
			}
		};//end namespace legacy
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


		feedback_t perform_damage(
		    player_ptr_t& player,
		    player_ptr_t& victim,
		    obj_ptr_t weapon,
		    int dam,
		    direction_t direction
		) {
			feedback_t feedback;
			if(victim->position() > POS_DEAD) {
				m_debug("victim position > DEAD");
				legacy::damage(player->cd(),victim->cd(),dam,legacy::get_legacy_attack_type(weapon));
				feedback.attacker = player->uuid();
				if(dam == 0) {
					m_debug("damage is zero");
					feedback.damage = dam;
					feedback.hits = 0;

					feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
					player->damage_event(feedback);

					feedback.from_direction = OPPOSITE_DIR(direction);
					feedback.damage_event =de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
					victim->damage_event(feedback);

				} else if(dam > 0) {
					m_debug("damage greater than zero");
					feedback.hits = 1;
					feedback.damage = dam;
					feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
					feedback.from_direction = OPPOSITE_DIR(direction);
					victim->set_attacker(player->uuid());
					feedback.damage_event = de::HIT_BY_RIFLE_ATTACK;
					victim->damage_event(feedback);
					if(victim->is_npc()) {
						mods::mobs::damage_event::sniped(victim,feedback);
					}
#ifdef __MENTOC_DISABLE_INJURE_DYNAMICS__
#else
					if(attack_injures(player,victim,weapon,feedback)) {
						feedback.injured.emplace_back(victim->uuid());
						feedback.damage_event= de::YOU_ARE_INJURED_EVENT;
						feedback.from_direction = OPPOSITE_DIR(direction);
						feedback.attacker = player->uuid();
						victim->damage_event(feedback);
						if(victim->is_npc()) {
							mods::mobs::damage_event::injured(victim,feedback);
						}

						feedback.damage_event= de::YOU_INJURED_SOMEONE_EVENT;
						player->damage_event(feedback);
						mods::injure::injure_player(victim);
					}
#endif
					m_debug("checking disorient");
					if(attack_disorients(player,weapon,victim)) {
						m_debug("disorients");
						mods::affects::affect_player_for({mods::affects::affect_t::DISORIENT},victim,disorient_ticks(player,weapon,victim));
						feedback.damage_event= de::YOU_ARE_DISORIENTED_EVENT;
						/** TODO: maybe make this random to disorient the player ? >:) EVIL GENIUS */
						feedback.from_direction = OPPOSITE_DIR(direction);
						victim->damage_event(feedback);
						if(victim->is_npc()) {
							mods::mobs::damage_event::disoriented(victim,feedback);
						}

						feedback.damage_event= de::YOU_DISORIENTED_SOMEONE_EVENT;
						player->damage_event(feedback);
					}
				}
				m_debug("remembering");
				remember_event(victim,player);
			}
			if(victim->position() == POS_DEAD) {
				legacy::player_died(player,victim);
			}
			return feedback;
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

		bool roll_just_bad_luck(player_ptr_t& player) {
			static constexpr uint32_t STARTING_POINT = 1;
			static constexpr uint32_t ENDING_POINT = 4000000;
			static constexpr uint32_t BAD_LUCK_VALUE = ENDING_POINT - 1;

			bool bad_luck = rand_number(STARTING_POINT,ENDING_POINT) == BAD_LUCK_VALUE;
			if(!bad_luck) {
				player->luck() += 1;
				return false;
			} else {
				player->luck() -= 1;
			}
			return rand_number(1,10) > player->luck();
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
			const auto& distance = target.distance;
			if(distance >= RCT->effective_range.first && distance <= RCT->effective_range.second) {
				success_chance += 50;
			}
			if(distance >= RCT->critical_range.first && distance <= RCT->critical_range.second) {
				success_chance += 50;
			}
			if(attacker->ghost()) {
				if(mods::object_utils::is_sniper_rifle(weapon)) {
					success_chance += 100;
				}
				if(mods::object_utils::is_smg(weapon)) {
					if(distance <= RCT->effective_range.second) {
						success_chance += 50;
					}
				}
			}
			if(attacker->marine()) {
				if(mods::object_utils::is_assault_rifle(weapon)) {
					success_chance += 70;
				}
				if(mods::object_utils::is_shotgun(weapon) && RCT->effective_range.second >= distance) {
					success_chance += 150;
				}
			}
			if(victim->ghost()) {
				if(victim->ghost()->is_dissipated()) {
					success_chance -= 80;
					auto range = mods::player_utils::get_thermal_vision_range(attacker);
					if(range.second >= distance) {
						success_chance += 80;
					}
				}
			}

			if(mods::calc_visibility::is_visible(attacker,victim)) {
				success_chance += 50;
			}
			auto bad_luck = roll_just_bad_luck(attacker);
			if(bad_luck) {
				attacker->damage_event(de::YOU_WERE_INFLICTED_WITH_BAD_LUCK);
				return false;
			}
			return success_chance > (rand_number(1,100) + rand_number(1,10));
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
		std::pair<uint16_t,uint16_t> roll_critical(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon) {
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
			uint16_t headshot = 0, crit = 0;
			if(distance <= RCT->critical_range.second && distance >= RCT->critical_range.first && dice(1,100) < RCT->critical_chance) {
				crit = dice(
				           RCT->damage_dice_count,
				           RCT->damage_dice_sides
				       );
			}
			/** calculate headshot */
			auto headshot_roll = dice(1,100) + (mods::skills::player_can(attacker,"HEADSHOT_CHANCE") ? HEADSHOT_SKILL_MODIFIER() : 0);
			if(headshot_roll >= 95) {
				/** TODO: evaluate damage if wearing super strong headgear */
				headshot = victim->hp() / HEADSHOT_DIVISOR();
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
			state::current = attacker->calculate_ranged_combat_totals(weapon);
			vpd scan;
			/**
			 * TODO: will have to change find_type_t::ANY to include DEAD for contagion's
			 * corpse sniping capabilities
			 */
			mods::scan::los_scan_direction(attacker->cd(),RCT->max_range,&scan,target.direction,mods::scan::find_type_t::NPC_AND_PLAYER);
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
				auto victim = ptr(scanned_target.ch);
				if(!victim) {
					continue;
				}
				if(mods::util::fuzzy_match(target.target_name,victim->name())) {
					if(scanned_target.distance > RCT->max_range) {
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

		calculated_damage_t calculate_weapon_damage(player_ptr_t& attacker,acquired_target_t& found_target,obj_ptr_t& weapon,std::pair<uint16_t,uint16_t> hsc) {
			calculated_damage_t d;
			/**
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
					*/
			const auto& distance = found_target.distance;
			if(hsc.first) { //headshot

			}
			if(hsc.second) { //critical

			}
			d.damage = RCT->base_damage;
			d.headshot_damage = hsc.first;
			d.critical_damage = hsc.second;
			if(attacker->ghost()) {
				if(mods::object_utils::is_sniper_rifle(weapon)) {
					d.damage += ((0.105 * tier(attacker)) * d.damage);//TODO: find the values equiv to this
				}
			}
			if(attacker->marine()) {
				if(mods::object_utils::is_assault_rifle(weapon)) {
					d.damage += (0.25 * d.damage); //TODO: find values equiv to this
					if(mods::rand::chance(10)) {//TODO values
						d.incendiary_damage = dice(tier(attacker) * 10,tier(attacker) * 10);//TODO values
					}
				}
				if(mods::object_utils::is_shotgun(weapon)) {
					if(distance > 1 && distance <= 3) { //TODO: values
						//TODO: need to define same room damage of shotgun
						d.damage = d.damage * distance;;
					}
				}
			}
			/** TODO: do breacher */
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
			const auto& dam = d.damage;
			feedback_t feedback;
			m_debug("victim position > DEAD");
			legacy::damage(attacker->cd(),victim->cd(),dam,legacy::get_legacy_attack_type(weapon));
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
				if(d.headshot_damage) {
					report_headshot(attacker,victim,d.headshot_damage,direction);
				}
				if(d.critical_damage) {
					report_crit(attacker,victim,d.critical_damage,direction);
				}
				m_debug("damage greater than zero");
				feedback.hits = 1;
				feedback.damage = dam;
				feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
				feedback.from_direction = OPPOSITE_DIR(direction);
				victim->set_attacker(attacker->uuid());
				feedback.damage_event = de::HIT_BY_RIFLE_ATTACK;
				victim->damage_event(feedback);
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
				legacy::player_died(attacker,victim);
			}
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

		if(cant_find_target) {
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
		if(mods::object_utils::get_ammo(weapon) == 0) {
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

	}
	void snipe_target(player_ptr_t& attacker,std::string_view target, direction_t direction,uint8_t distance,obj_ptr_t& weapon) {
		snipe_target(attacker,phases::target_t(target,direction),weapon);
	}
};//end namespace combat_composer
