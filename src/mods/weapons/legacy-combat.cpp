#include "legacy-combat.hpp"

namespace mods::weapons::legacy {

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
		if(vplayer && mods::super_users::player_is(vplayer)) {
			return;
		}
#endif
		if(vplayer) {
			vplayer->hp() -= dam;
		}
	}

	void update_position(char_data* ch) {
		auto victim = ptr(ch);
		if(!victim) {
			return;
		}
		if((victim->hp() > 0) && (victim->position() > POS_STUNNED)) {
			return;
		} else if(victim->hp() > 0) {
			victim->position() = POS_STANDING;
		} else if(victim->hp() <= -11) {
			victim->position() = POS_DEAD;
		} else if(victim->hp() <= -6) {
			victim->position() = POS_MORTALLYW;
		} else if(victim->hp() <= -3) {
			victim->position() = POS_INCAP;
		} else {
			victim->position() = POS_STUNNED;
		}

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
		if(!vplayer) {
			return -1;
		}
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
					log("SYSERR: invalid rifle type %s name('%s')",__FUNCTION__,weapon->name.c_str());
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
