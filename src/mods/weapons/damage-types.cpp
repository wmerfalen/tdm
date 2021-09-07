#include "damage-types.hpp"
#include "../../spells.h"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "elemental.hpp"
#include "damage-calculator.hpp"
#include "../mobs/damage-event.hpp"
#include "../calc-visibility.hpp"
#include "../loot.hpp"


#include <variant>
#include "damage-event-broadcaster.hpp"
#include "damage-decisions.hpp"
#include "../classes/ghost.hpp"
#include "../classes/breacher.hpp"
#include "../classes/marine.hpp"
#include "../corrosive.hpp"

#ifdef __MENTOC_DAMAGE_TYPES_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::weapons::damage_types][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifdef __MENTOC_SHOW_DAMAGE_TYPES_DEBUG__
#define md(A) std::cerr << green_str("damage_types_debug:") << A << "\n";
#else
#define md(A)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

extern void extract_char_final(char_data *ch);
extern void make_corpse(char_data *ch);
extern void death_cry(char_data *ch);
extern void change_alignment(char_data *ch, char_data *victim);
extern void obj_to_char(obj_ptr_t  object, player_ptr_t player);
extern int ok_damage_shopkeeper(char_data *ch, char_data *victim);
extern void set_fighting(char_data *ch, char_data *vict);
extern void appear(char_data *ch);
extern int pk_allowed;
extern void check_killer(char_data *ch, char_data *vict);
extern int skill_message(int dam, char_data *ch, char_data *vict, int attacktype);
extern void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
extern void send_to_room(room_rnum room, const char *messg, ...);
extern ACMD(do_flee);
#if __MENTOC_USE_GROUP_GAIN__
extern void group_gain(char_data *ch, char_data *victim);
#endif
extern void forget(char_data *ch,char_data *victim);
#ifndef IS_WEAPON
#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
#endif
extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

#include "../affects.hpp"

namespace mods::weapons::damage_types {
	using de = damage_event_t;
	using vpd = mods::scan::vec_player_data;
#define MFEEDBACK(HITS,DAMAGE,EVENT)\
		{\
				feedback_t f;\
				f.hits = HITS;\
				f.damage = DAMAGE;\
				f.from_direction = OPPOSITE_DIR(direction);\
				f.attacker = player->uuid();\
				f.damage_event = EVENT;\
			victim->damage_event(f);\
		}

	void reflect_munitions(player_ptr_t& attacker,player_ptr_t& victim,int dam,int from_direction);
	bool can_be_injured(player_ptr_t& victim);
	void handle_assault_rifle_shrapnel_skill(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t& feedback);

	void rifle_attack_object(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    obj_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	);
	feedback_t rifle_attack_object_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    obj_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	);



	namespace legacy {
		void solo_gain(player_ptr_t& attacker,player_ptr_t& victim);
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

			make_corpse(ch);
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
			attacker->sendln(CAT("You gain ",mods::levels::gain_exp_from_killing(attacker, victim)," experience points."));
			auto mp = mods::levels::gain_mp_from_killing(attacker, victim);
			if(mp) {
				attacker->sendln(CAT("You gain ",mp," mission points."));
			}
			change_alignment(attacker->cd(), victim->cd());
		}


		int step_one(char_data *ch, char_data *victim, int dam, int attacktype) {
			auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
			if(mods::super_users::player_is(vplayer)) {
				md("player is super user (from step_one)");
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
				md("cannot damage shopkeeper");
				return (0);
			}
#endif
			md("step_one okay");
			return 1;
		}
		int step_two(char_data *ch, char_data *victim, int dam, int attacktype) {
			auto vplayer = ptr(victim);
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
			if(mods::super_users::player_is(vplayer)) {
				md("player is super user (step 2)");
				return 0;
			}
#endif
			/* You can't damage an immortal! */
			if(!IS_NPC(victim) && (GET_LEVEL(victim) >= LVL_IMMORT)) {
				md("victim isnt npc and victim >= IMMORT");
				dam = 0;
			}
			if(victim != ch) {
				/* Start the attacker fighting the victim */
				if(GET_POS(ch) > POS_STUNNED && (FIGHTING(ch) == NULL)) {
					md("set fighting");
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
					md("appear");
					appear(ch);
				}
				md("step_two okay");
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
			ptr(victim)->hp() -= dam;
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
				md("player is super user");
				return -1;
			}
#endif
			if(step_one(attacker,victim,dam,attacktype) <= 0) {
				md("step one failed");
				return -2;
			}
			if(step_two(attacker,victim,dam,attacktype) <= 0) {
				md("step two failed");
				return -3;
			}
			dam = get_damage(attacker,victim,dam,attacktype);
			md("get_damage:" << dam);
			deal_damage(attacker,victim,dam,attacktype);
			md("deal_damage'd");
			auto p = ptr(attacker);
			md("updating pos of victim");
			update_position(victim);
			send_combat_messages(attacker,victim,dam,attacktype);
			help_linkless(attacker,victim,dam,attacktype);
			return perform_damage_cleanup(attacker,victim,dam,attacktype);
		}
		void player_died(player_ptr_t& attacker,player_ptr_t& victim) {
			md("target dead!");
			feedback_t feedback;
			feedback.damage_event= de::TARGET_DEAD_EVENT;
			feedback.attacker = victim->uuid();
			attacker->damage_event(feedback);
			stop_fighting(attacker->cd());
			stop_fighting(victim->cd());
			mods::weapons::damage_types::legacy::die(attacker->cd(),victim->cd());
		}
		void player_died(player_ptr_t& victim) {
			stop_fighting(victim->cd());
			mods::weapons::damage_types::legacy::die(victim->cd());
		}
	};//end namespace legacy

	feedback_t perform_damage(
	    player_ptr_t& player,
	    player_ptr_t& victim,
	    obj_ptr_t weapon,
	    int dam,
	    direction_t direction
	) {
		feedback_t feedback;
		if(victim->position() > POS_DEAD) {
			md("victim position > DEAD");
			damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
			feedback.attacker = player->uuid();
			if(dam == 0) {
				md("damage is zero");
				feedback.damage = dam;
				feedback.hits = 0;

				feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
				player->damage_event(feedback);

				feedback.from_direction = OPPOSITE_DIR(direction);
				feedback.damage_event =de::ATTACKER_NARROWLY_MISSED_YOU_EVENT;
				victim->damage_event(feedback);

			} else if(dam > 0) {
				md("damage greater than zero");
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
				md("checking disorient");
				if(mods::weapons::damage_calculator::attack_disorients(player,weapon,victim)) {
					md("disorients");
					mods::affects::affect_player_for({mods::affects::affect_t::DISORIENT},victim,mods::weapons::damage_calculator::disorient_ticks(player,weapon,victim));
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
			md("remembering");
			remember_event(victim,player);
		}
		if(victim->position() == POS_DEAD) {
			legacy::player_died(player,victim);
		}
		return feedback;
	}


	void deal_hp_damage(player_ptr_t& victim, uint16_t damage) {
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
		if(mods::super_users::player_is(victim)) {
			return;
		}
#endif
		if(legacy::step_one(nullptr,victim->cd(),damage,0) <= 0) {
			return;
		}

		legacy::deal_damage(nullptr,victim->cd(),damage,0);
		legacy::update_position(victim->cd());
		legacy::send_combat_messages(nullptr,victim->cd(),damage,0);
	}


	/**
	 * @brief This is needed because we feed the damage and the attacktype to damage()
	 * Unfortunately, the damage() function does so much and is such a core part of legacy cm
	 * that I do not want to reinvent it...
	 * @param weapon
	 *
	 * @return
	 */
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

	/**
	 * @brief sprays using the primary weapon
	 *
	 * @param player
	 * @param direction
	 */
	void spray_direction(player_ptr_t& player,int direction) {
		spray_direction_with_feedback(player,direction);
	}

	feedback_t spray_direction_with_feedback(player_ptr_t& player,int direction) {
		using de = damage_event_t;
		feedback_t feedback;
		auto weapon = player->primary();

		auto s = can_continue(player,weapon,-1,OPPOSITE_DIR(direction),ITEM_RIFLE);
		if(std::get<0>(s) == false) {
			return std::get<1>(s);
		}
		feedback = std::get<1>(s);

		/** FIXME : grab weapon's accuracy and apply accurace modifications */

		int dam = 0;
		int damage_dice = weapon->rifle()->attributes->damage_dice_count;
		int damage_sides = weapon->rifle()->attributes->damage_dice_sides;
		int crit_range = weapon->rifle()->attributes->critical_range;
		int crit_chance = weapon->rifle()->attributes->critical_chance;
		if(player->marine() && mods::object_utils::is_assault_rifle(weapon)) {
			crit_chance += MARINE_AR_PASSIVE_CRITICAL_CHANCE_BONUS();
		}
		int critical_bonus = 0;
#ifdef __MENTOC_SHOW_DAMAGE_DUMP_DEBUG_OUTPUT__
#define DMG_DUMP() \
		std::cerr << "dam:" << dam << "\n";\
		std::cerr << "damage_dice: ";\
		std::cerr << damage_dice  <<"\n";\
		std::cerr << "damage_sides";\
		std::cerr << damage_sides << "\n";\
		std::cerr << "crit_range ";\
		std::cerr << crit_range << "\n";\
		std::cerr << "crit_chance"; \
		std::cerr << crit_chance << "\n";\
		std::cerr << "critical_bonus";\
		std::cerr << critical_bonus << "\n";\
		std::cerr << "damage_sides:";\
		std::cerr << damage_sides << "\n";
#else
#define DMG_DUMP()
#endif




		vpd scan;
		auto max_range = mods::weapons::damage_calculator::max_range(player,weapon);
		mods::scan::los_scan_direction(player->cd(),max_range,&scan,direction,mods::scan::find_type_t::ALIVE);

		player->set_fight_timestamp();
		decrease_spray_shot_ammo(player,weapon);
		for(auto&& scanned_target : scan) {
			dam = 0;
			auto victim = ptr(scanned_target.ch);
			if(!victim) {
				continue;
			}
			if(mods::rooms::is_peaceful(victim->room())) {
				player->sendln(MSG_TARGET_IN_PEACEFUL_ROOM());
				continue;
			}
			if(dice(1,(100 * scanned_target.distance)) <= calculate_spray_chance(player)) {
				player->send(MSG_HIT().c_str());
				int spray_damage = 0;
				spray_damage = dice(damage_dice,damage_sides);
				//spray_damage += weapon->rifle()->attributes->base_stat_list->at(scanned_target.distance).damage;
				dam += spray_damage;

				MFEEDBACK(1,dam,de::HIT_BY_SPRAY_ATTACK);
			}
			/** calculate headshot */
			if(dice(1,(100 * scanned_target.distance)) <= mods::values::SPRAY_HEADSHOT_CHANCE()) {
				player->send(MSG_HEADSHOT().c_str());
				dam = victim->hp();
				MFEEDBACK(1,dam,de::YOU_DEALT_HEADSHOT_WITH_SPRAY_ATTACK);
			}
			if(scanned_target.distance == crit_range) {
				if(dice(1,(100 * scanned_target.distance)) <= crit_chance) {
					player->send(MSG_CRITICAL().c_str());
					critical_bonus = dice(
					                     damage_dice,
					                     damage_sides
					                 );
					critical_bonus /= mods::values::SPRAY_CRITICAL_REDUCTION_DIVISOR();
					dam += critical_bonus;
					MFEEDBACK(1,dam,de::HIT_BY_CRITICAL_SPRAY_ATTACK);
				}
			}
			DMG_DUMP();
			dam = calculate_tracked_damage(player,dam);

			feedback.damage = dam;
			if(victim->position() > POS_DEAD) {
				damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
				if(dam == 0) {
					missed(player,victim);
				} else if(dam > 0) {
					send_rifle_damage(1,dam,direction,player,victim,weapon, feedback);
				}
				remember_event(victim,player);
				set_player_weapon_cooldown(player,victim,weapon, feedback);
			}
			if(victim->position() == POS_DEAD) {
				legacy::player_died(player,victim);
			}
		}//end for loop

		return feedback;

	}//end spray_direction function

	void rifle_attack_object_by_name(player_ptr_t& player,std::string_view target_object,int direction) {
		feedback_t feedback;
		auto weapon = player->primary();

		auto s = can_continue(player,weapon,-1,OPPOSITE_DIR(direction),ITEM_RIFLE);
		if(std::get<0>(s) == false) {
			return;
		}
		feedback = std::get<1>(s);

		vpd scan;
		auto max_range = mods::weapons::damage_calculator::max_range(player,weapon);
		mods::scan::los_scan_direction(player->cd(),max_range,&scan,direction,mods::scan::find_type_t::OBJECTS);
		obj_ptr_t victim = nullptr;

		for(auto&& scanned_target : scan) {
			if(mods::util::fuzzy_match(target_object.data(),scanned_target.obj->name.c_str())) {
				if(scanned_target.distance > max_range) {
					player->sendln("That target is out of range!");
					return;
				}
				auto obj = optr_by_uuid(scanned_target.obj->uuid);
				rifle_attack_object(player,weapon,obj,scanned_target.distance,direction);
				return;
			}
		}
	}


	/**
	 * @brief if you know your target's name and direction, then use this to initiate the attack
	 *
	 * @param player
	 * @param victim_name
	 * @param direction
	 */
	void rifle_attack_by_name(player_ptr_t& player,std::string_view victim_name,int direction) {
		feedback_t feedback;
		auto weapon = player->primary();

		auto s = can_continue(player,weapon,-1,OPPOSITE_DIR(direction),ITEM_RIFLE);
		if(std::get<0>(s) == false) {
			return;
		}
		feedback = std::get<1>(s);
		vpd scan;
		auto max_range = mods::weapons::damage_calculator::max_range(player,weapon);
		mods::scan::los_scan_direction(player->cd(),max_range,&scan,direction,mods::scan::find_type_t::ALIVE);
		player_ptr_t victim = nullptr;

		for(auto&& scanned_target : scan) {
			victim = ptr(scanned_target.ch);
			if(!victim) {
				continue;
			}
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
			if(mods::super_users::player_is(victim)) {
				continue;
			}
#endif
			if(mods::util::fuzzy_match(victim_name.data(),victim->name())) {
				if(scanned_target.distance > max_range) {
					player->sendln("That target is out of range!");
					return;
				}
				if(!mods::calc_visibility::is_visible(player,victim)) {
					feedback.damage_event = de::COULDNT_FIND_TARGET_EVENT;
					player->damage_event(feedback);
					return;
				}
				rifle_attack(player,weapon,victim,scanned_target.distance,direction);
				return;
			}
		}
	}

	/**
	 * @brief reduces the amount of ammo used from spraying
	 *
	 * @param weapon
	 */
	void decrease_spray_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon) {
		/** TODO: if weapon has a bullet printer mod, calculate ammo */
		if(weapon->rifle_instance->ammo == 0) {
			return;
		}
		int16_t deduct = 0;
		switch((mw_rifle)weapon->rifle()->attributes->type) {
			case mw_rifle::LIGHT_MACHINE_GUN:
				deduct = mods::values::SPRAY_SHOT_LIGHT_MACHINE_GUN();
				break;

			case mw_rifle::SUB_MACHINE_GUN:
				deduct = mods::values::SPRAY_SHOT_SUB_MACHINE_GUN();
				break;

			case mw_rifle::SHOTGUN:
				deduct = mods::values::SPRAY_SHOT_SHOTGUN();
				break;

			case mw_rifle::SNIPER:
				deduct = mods::values::SPRAY_SHOT_SNIPER();
				break;

			case mw_rifle::ASSAULT_RIFLE:
				deduct = mods::values::SPRAY_SHOT_ASSAULT_RIFLE();
				break;
			case mw_rifle::HANDGUN:
			case mw_rifle::PISTOL:
				deduct = mods::values::SPRAY_SHOT_HANDGUN();
				break;
			case mw_rifle::MACHINE_PISTOL:
				deduct = mods::values::SPRAY_SHOT_MACHINE_PISTOL();
				break;
			default:
				deduct = 1;
				log("SYSERR: warning, no rifle type given for decrease_spray_shot_ammo, default to 1");
				break;
		}
		int ammo = weapon->rifle_instance->ammo - mods::weapons::damage_calculator::reduce_ammo(attacker,weapon,deduct);
		if(ammo < 0) {
			weapon->rifle_instance->ammo = 0;
		} else {
			weapon->rifle_instance->ammo = ammo;
		}
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
		int ammo = weapon->rifle_instance->ammo - mods::weapons::damage_calculator::reduce_ammo(attacker,weapon,deduct);
		if(ammo < 0) {
			weapon->rifle_instance->ammo = 0;
		} else {
			weapon->rifle_instance->ammo = ammo;
		}
	}

	/**
	 * @brief attack using any obj_data structure that has a rifle_data_t pointer
	 *
	 * @param player
	 * @param weapon
	 * @param victim
	 * @param distance
	 */
	void rifle_attack(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	) {
#ifdef __MENTOC_SUPER_USERS_ARE_INVINCIBLE__
		if(mods::super_users::player_is(victim)) {
			return;
		}
#endif
		auto feedback = rifle_attack_with_feedback(player,weapon,victim,distance,direction);
		mods::weapons::elemental::process_elemental_damage(player,weapon,victim,feedback);
	}

	void rifle_attack_object(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    obj_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	) {
		rifle_attack_object_with_feedback(player,weapon,victim,distance,direction);
	}

	feedback_t rifle_attack_object_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    obj_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	) {
		std::tuple<int,uuid_t> sentinel;

		feedback_t feedback;

		auto s = can_continue(player,weapon,victim,OPPOSITE_DIR(direction),ITEM_RIFLE);
		if(std::get<0>(s) == false) {
			return std::get<1>(s);
		}
		feedback = std::get<1>(s);

		/** FIXME : grab weapon's accuracy and apply accurace modifications */

		int dam = weapon->rifle()->attributes->base_stat_list->at(distance).damage;
		int damage_dice = weapon->rifle()->attributes->damage_dice_count;
		int damage_sides = weapon->rifle()->attributes->damage_dice_sides;
		int crit_range = weapon->rifle()->attributes->critical_range;
		int crit_chance = weapon->rifle()->attributes->critical_chance;
		int critical_bonus = 0;

		if(player->marine() && mods::object_utils::is_assault_rifle(weapon)) {
			crit_chance += MARINE_AR_PASSIVE_CRITICAL_CHANCE_BONUS();
		}
		/** TODO: honor accuracy bonus */
		/** TODO: honor max_range calculations */
		/** calculate headshot */
		if(distance == crit_range && dice(1,100) <= crit_chance) {
			player->send(MSG_CRITICAL().c_str());
			critical_bonus = dice(
			                     damage_dice,
			                     damage_sides
			                 );
			feedback.damage = dam;
			feedback.hits = 1;

			feedback.damage_event = de::YOU_DEALT_CRITICAL_RIFLE_ATTACK;
			player->damage_event(feedback);
			/** TODO: process damage on object here */
		}
		dam += critical_bonus;

		auto dice_roll = dice(
		                     damage_dice,
		                     damage_sides
		                 );
		dam += dice_roll;
		dam = calculate_tracked_damage(player,dam);
		auto bonus_dam = mods::weapons::damage_calculator::calculate_bonus_damage(player,weapon,dam);
		if(bonus_dam > dam) {
			dty_debug("bonus damage: " << bonus_dam);
			dam = bonus_dam;
		}

#ifdef __MENTOC_SHOW_SNIPE_HIT_STATS__
		player->send(
		    "dice roll[%d]\r\n"
		    "damage: [%d]\r\n"
		    "critical_bonus: [%d]\r\n"
		    "damage_dice [%d]\r\n"
		    "damage_sides [%d]\r\n"
		    "crit_range [%d]\r\n"
		    "crit_chance [%d]\r\n",
		    dice_roll,
		    dam,
		    critical_bonus,
		    damage_dice,
		    damage_sides,
		    crit_range,
		    crit_chance
		);
#endif

		/** TODO: honor accuracy bonuses */
		if(dice(2,6) <= 3) {
			feedback.damage = dam;
			feedback.hits = 0;
			feedback.damage_event = de::YOU_MISSED_YOUR_TARGET_EVENT;
			player->damage_event(feedback);
			return feedback;
		}

		/** TODO: process damage on object here */
		decrease_single_shot_ammo(player,weapon);
		player->set_fight_timestamp();
		return feedback;
	}

	feedback_t melee_damage_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim
	) {
		using de = damage_event_t;
		std::tuple<int,uuid_t> sentinel;
		feedback_t feedback;

		auto s = can_continue(player,weapon,victim,NORTH,ITEM_MELEE);
		if(std::get<0>(s) == false) {
			return std::get<1>(s);
		}
		feedback = std::get<1>(s);

		feedback.hits = 1;
		feedback.damage = 0;
		feedback.from_direction = NORTH;

		if(!victim || !mods::calc_visibility::is_visible(player,victim)) {
			feedback.damage_event = de::COULDNT_FIND_TARGET_EVENT;
			player->damage_event(feedback);
			md("victim not present. ignoring");
			return feedback;
		}

		int dam = 0;
		md("doing damage calculate");
		auto dice_roll = mods::weapons::damage_calculator::calculate(player,weapon,victim);
		md("dice_roll: " << dice_roll << ", adding onto damage of:" << dam);
		dam += dice_roll;

		int str_bonus = mods::weapons::damage_calculator::calculate_strength_bonus(player,weapon,victim,dam);
		md("strength bonus: " << str_bonus);

		int con_dampener = mods::weapons::damage_calculator::calculate_constitution_resistance(player,weapon,victim,dam);
		md("constitution dampener amount: " << con_dampener);

		dam = dam + str_bonus - con_dampener;

		md("dice_roll: " << dice_roll);

		dam = calculate_tracked_damage(player,dam);
		md("tracked damage: " << dam);

		auto bonus_dam = mods::weapons::damage_calculator::calculate_bonus_damage(player,weapon,dam);
		if(bonus_dam > dam) {
			md("bonus damage:" << bonus_dam);
			dam = bonus_dam;
		}

		if(victim->position() > POS_DEAD) {
			md("deploying damage: " << dam);
			damage(player->cd(),victim->cd(),dam,get_legacy_attack_type(weapon));
			if(dam == 0) {
				md("missed victim");
				missed(player,victim);
			} else if(dam > 0) {
				md("sending melee damage: " << feedback.dump());
				auto f = send_melee_damage(feedback.hits, dam, player, victim, weapon);
				feedback = std::get<0>(f);
				if(victim->is_npc()) {
					mods::mobs::damage_event::meleed(victim,feedback);
				}
			}
			md("remembering");
			remember_event(victim,player);
		}
		if(victim->position() == POS_DEAD) {
			legacy::player_died(player,victim);
		}

		md("setting fight timestamp");
		player->set_fight_timestamp();
		md("processing elemental damage");
		mods::weapons::elemental::process_elemental_damage(player,weapon,victim,feedback);
		md("processed elemental damage");
		set_player_weapon_cooldown(player,victim,weapon, feedback);
		return feedback;
	}// end melee_damage_with_feedback

	feedback_t underbarrel_shotgun_attack_with_feedback(
	    player_ptr_t& player,
	    player_ptr_t victim) {

		feedback_t feedback;

		auto ub = player->ghost()->underbarrel();
		int dam = tier(player) * 65.314712 * (rand_number(1,7));
		//auto headshot_roll = dice(1,100) + (mods::skills::player_can(player,"HEADSHOT_CHANCE") ? mods::values::HEADSHOT_SKILL_MODIFIER() : 0);
		//if(headshot_roll >= 95) {
		//	/** TODO: evaluate dam if wearing super strong headgear */
		//	int headshot_damage = victim->hp() / HEADSHOT_DIVISOR();
		//	dam = headshot_damage;
		//	feedback.hits = 1;
		//	feedback.dam = dam;
		//	feedback.attacker = nullptr;
		//	feedback.damage_event = de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK;
		//	player->damage_event(feedback);

		//	feedback.attacker = player->uuid();
		//	feedback.from_direction = 0;
		//	feedback.damage_event =de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK;
		//	victim->damage_event(feedback);

		//	player->send(MSG_HEADSHOT().c_str());
		//	md("headshot");
		//}

		damage(player->cd(),victim->cd(),dam,TYPE_SHOTGUN);
		feedback.hits = 1;
		feedback.damage = dam;
		feedback.damage_info.emplace_back(victim->uuid(),dam,victim->room());
		feedback.from_direction = 0;
		feedback.attacker = player->uuid();
		victim->set_attacker(player->uuid());
		feedback.damage_event = de::HIT_BY_SHOTGUN_BLAST;
		victim->damage_event(feedback);

		feedback.damage_info.clear();
		feedback.attacker = 0;
		feedback.damage_event = de::YOU_INFLICTED_SHOTGUN_BLAST;
		player->damage_event(feedback);
		player->set_attacker(victim->uuid());
		remember_event(victim,player);
		if(victim->position() == POS_DEAD) {
			legacy::player_died(player,victim);
		}
		player->ghost()->consume_shotgun_underbarrel_ammo();
		return feedback;
	}

	feedback_t rifle_attack_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	) {
		using de = damage_event_t;
		std::tuple<int,uuid_t> sentinel;

		if(distance == 0 && player->ghost()) {
			auto ub = player->ghost()->underbarrel();
			if(ub) {
				return underbarrel_shotgun_attack_with_feedback(player,victim);
			}
		}


		feedback_t feedback;
		auto s = can_continue(player,weapon,victim,OPPOSITE_DIR(direction),ITEM_RIFLE);
		if(std::get<0>(s) == false) {
			md("can't continue");
			return std::get<1>(s);
		}
		feedback = std::get<1>(s);
		md("can continue. feedback:" << feedback.dump());

		auto max_range = mods::weapons::damage_calculator::max_range(player,weapon);
		if(distance > max_range) {
			feedback.damage_event = de::TARGET_IS_OUT_OF_RANGE;
			player->damage_event(feedback);
			md("out of max range");
			return feedback;
		}
		/** FIXME : grab weapon's accuracy and apply accurace modifications */
		int dam = 0;
		if(distance < weapon->rifle()->attributes->base_stat_list->size()) {
			dam = weapon->rifle()->attributes->base_stat_list->at(distance).damage;
			md("using base stat list: " << dam);
		}
		int damage_dice = weapon->rifle()->attributes->damage_dice_count;
		int damage_sides = weapon->rifle()->attributes->damage_dice_sides;
		int crit_range = weapon->rifle()->attributes->critical_range;
		int crit_chance = weapon->rifle()->attributes->critical_chance;
		int critical_bonus = 0;
		if(mods::rooms::is_peaceful(victim->room())) {
			feedback.damage_event = de::TARGET_IN_PEACEFUL_ROOM_EVENT;
			player->damage_event(feedback);
			md("is peaceful");
			return feedback;
		}
		if(player->marine() && mods::object_utils::is_assault_rifle(weapon)) {
			crit_chance += MARINE_AR_PASSIVE_CRITICAL_CHANCE_BONUS();
		}

		/** calculate headshot */
		auto headshot_roll = dice(1,100) + (mods::skills::player_can(player,"HEADSHOT_CHANCE") ? mods::values::HEADSHOT_SKILL_MODIFIER() : 0);
		if(headshot_roll >= 95) {
			/** TODO: evaluate damage if wearing super strong headgear */
			int headshot_damage = victim->hp() / HEADSHOT_DIVISOR();
			dam = headshot_damage;
			feedback.hits = 1;
			feedback.damage = dam;
			feedback.attacker = player->uuid();
			feedback.damage_event = de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK;
			player->damage_event(feedback);

			feedback.from_direction = OPPOSITE_DIR(direction);
			feedback.damage_event =de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK;
			victim->damage_event(feedback);

			player->send(MSG_HEADSHOT().c_str());
			md("headshot");
		}
		md("checking crit range");
		if(distance == crit_range) {
			md("is crit range");
			if(dice(1,100) <= crit_chance) {
				player->send(MSG_CRITICAL().c_str());
				critical_bonus = dice(
				                     damage_dice,
				                     damage_sides
				                 );
				feedback.damage = dam;
				feedback.hits = 1;

				feedback.damage_event = de::YOU_DEALT_CRITICAL_RIFLE_ATTACK;
				player->damage_event(feedback);

				feedback.from_direction = OPPOSITE_DIR(direction);
				feedback.damage_event =de::HIT_BY_CRITICAL_RIFLE_ATTACK;
				victim->damage_event(feedback);
			}
		}
		dam += critical_bonus;

		auto dice_roll = mods::weapons::damage_calculator::calculate(player,weapon,victim);
		dam += dice_roll;

		if(weapon->rifle()->attributes->type == mw_rifle::SNIPER && player->ghost()) {
			int sniper_class_damage = mods::weapons::damage_calculator::calculate_sniper_extra_damage(player,weapon,victim,dam);
			classes::sniper::send_innate_bonus(sniper_class_damage,direction,player,victim,weapon);
			dam += sniper_class_damage;
		}
#ifdef __MENTOC_SHOW_SNIPE_HIT_STATS__
		player->send(
		    "dice roll[%d]\r\n"
		    "damage: [%d]\r\n"
		    "critical_bonus: [%d]\r\n"
		    "damage_dice [%d]\r\n"
		    "damage_slides [%d]\r\n"
		    "crit_range [%d]\r\n"
		    "crit_chance [%d]\r\n",
		    dice_roll,
		    dam,
		    critical_bonus,
		    damage_dice,
		    damage_sides,
		    crit_range,
		    crit_chance
		);
#endif
		md("dice_roll: " << dice_roll << ", dam:" << dam << "critical_bonus: " << critical_bonus <<
		   ", " << damage_dice << "d" << damage_sides);
		dam = calculate_tracked_damage(player,dam);
		md("tracked damage: " << dam);
		auto bonus_dam = mods::weapons::damage_calculator::calculate_bonus_damage(player,weapon,dam);
		md("bonus damage: " << bonus_dam);
		if(bonus_dam > dam) {
			dam = bonus_dam;
		}

		feedback = perform_damage(player,victim,weapon,dam,direction);

		md("decreasing ammo");
		decrease_single_shot_ammo(player,weapon);
		player->set_fight_timestamp();
		set_player_weapon_cooldown(player,victim,weapon, feedback);
		md("cooldown set");
		return feedback;
	}//end rifle_attack_with_feedback

	SUPERCMD(do_inc_damage) {
		auto vec_args = PARSE_ARGS();
		static const char* usage = "usage: inc_damage <resistance_percent> <damage>\r\n";
		if(vec_args.size() < 2) {
			player->sendln(usage);
			return;
		}
		int resistance = mods::util::stoi(vec_args[0]).value_or(-1);
		if(resistance == -1) {
			player->errorln("Invalid resistance");
			return;
		}
		int damage = mods::util::stoi(vec_args[1]).value_or(0);
		if(damage == -1) {
			player->errorln("Invalid damage");
			return;
		}
		player->incendiary_resistance_percent() = resistance;
		player->send("Your resistance: %f\r\n",player->incendiary_resistance_percent());
		mods::weapons::elemental::incendiary_damage(player,player,damage);
	}

	void init() {
		ADD_INTERPRETER_COMMAND(inc_damage,LVL_BUILDER);
	}

	void update_and_process_death(player_ptr_t attacker,player_ptr_t victim) {
		mods::weapons::damage_types::legacy::update_position(victim->cd());
		if(victim->position() == POS_DEAD) {
			if(attacker) {
				mods::weapons::damage_types::legacy::player_died(attacker,victim);
			} else {
				mods::weapons::damage_types::legacy::player_died(victim);
			}
		}
	}
};
#ifdef DMG_DEBUG
#undef DMG_DEBUG
#undef md
#endif
