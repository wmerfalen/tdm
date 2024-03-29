/* ************************************************************************
*   File: mobact.c                                      Part of CircleMUD *
*  Usage: Functions for generating intelligent (?) behavior in mobiles    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"


#include "structs.h"
#include "utils.h"
#include "db.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "spells.h"
#include "constants.h"
#include "globals.hpp"
#include "mods/loops.hpp"
#include "mods/behaviour_tree_impl.hpp"
#include "mods/mob-roam.hpp"
#include "mods/mobs/roam-pattern.hpp"
#include "mods/interpreter-include.hpp"

/* external globals */
extern int no_specials;
extern char_data* character_list;

/* external functions */
ACMD(do_get);
ACMD(do_action);

/* local functions */
void mobile_activity(void);
void clearMemory(char_data *ch);
bool aggressive_mob_on_a_leash(char_data *slave,char_data *master,char_data *attack);

#define MOB_AGGR_TO_ALIGN (MOB_AGGR_EVIL | MOB_AGGR_NEUTRAL | MOB_AGGR_GOOD)

#ifdef __MENTOC_MOBACT_ROAM_DEBUG__
#define m_can_roam(A) std::cerr << "[mobile_activity][can_rom]:" << A << "\n";
#else
#define m_can_roam(A)
#endif

#ifdef __MENTOC_MUTE_BEHAVIOUR_TREE_OUTPUT__
#define bht_debug(a) /**/
#else
#define bht_debug(a){ std::cerr << "[mobile_activity][behaviour_trees]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
#endif

void mobile_activity(void) {
	//auto player = mods::globals::current_player;
	std::size_t mob_list_size = mob_list.size();
	for(auto& npc : mob_list) {
		if(mob_list_size != mob_list.size()) {
			return mobile_activity();
		}
		auto uuid = npc->uuid();
		if(!ptr_by_uuid(uuid)) {
			log("SYSERR: possible memory leak in mob_list via mobile_activity");
			continue;
		}
		auto ch = npc->cd();
		player_ptr_t player = npc;
		char_data *vict;
		struct obj_data *obj, *best_obj;
		int door, found, max;
		bht_debug("FOREACH_MOB(" << MOBTOSTR(ch));



		/* Examine call for special procedure */
#ifdef __MENTOC_USE_MOB_SPECIALS_IN_MOBILE_ACTIVITY__
		if(MOB_FLAGGED(ch, MOB_SPEC) && !no_specials) {
			if(mob_index[GET_MOB_RNUM(ch)].func == NULL) {
				log("SYSERR: %s (#%d): Attempting to call non-existing mob function.",
				    GET_NAME(ch).c_str(), GET_MOB_VNUM(ch));
				REMOVE_BIT(MOB_FLAGS(ch), MOB_SPEC);
			} else {
				char actbuf[MAX_INPUT_LENGTH] = "";

				if((mob_index[GET_MOB_RNUM(ch)].func)(ch, ch, 0, actbuf, player)) {
				}
			}
		}
#endif

		/* If the mob has no specproc, do the default actions */
		if(FIGHTING(ch) || !AWAKE(ch)) {
			continue;
		}

		/* Scavenger (picking up objects) */
		if(MOB_FLAGGED(ch, MOB_SCAVENGER)) {
			if(world[IN_ROOM(ch)].contents && !rand_number(0, 10)) {
				max = 1;
				best_obj = NULL;

				for(obj = world[IN_ROOM(ch)].contents; obj; obj = obj->next_content)
					if(CAN_GET_OBJ(ch, obj) && GET_OBJ_COST(obj) > max) {
						best_obj = obj;
						max = GET_OBJ_COST(obj);
					}

				if(best_obj != NULL) {
					obj_from_room(best_obj);
					obj_to_char(best_obj, ch);
					act("$n gets $p.", FALSE, ch, best_obj, 0, TO_ROOM);
				}
			}
		}

		/* Mob Movement */
		if(!npc->has_tree() && !MOB_FLAGGED(ch, MOB_SENTINEL) && (GET_POS(ch) == POS_STANDING) &&
		        ((door = rand_number(0, 18)) < NUM_OF_DIRS) && CAN_GO(ch, door) &&
		        !ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_NOMOB | ROOM_DEATH) &&
		        (!MOB_FLAGGED(ch, MOB_STAY_ZONE) ||
		         (world[EXIT(ch, door)->to_room].zone == world[IN_ROOM(ch)].zone))) {
			if(mods::mob_roam::can_roam_to(ch,EXIT(ch,door)->to_room)) {
				m_can_roam(green_str("Can roam to area"));
				perform_move(ch, door, 1);
			}
		}

		/* Aggressive Mobs */
		if(MOB_FLAGGED(ch, MOB_AGGRESSIVE | MOB_AGGR_TO_ALIGN)) {
			found = FALSE;

			//std::cerr << green_str("Mob (uuid:") << npc->uuid() << "), npc->room():'" << npc->room() << "', plr->room():'" << npc->room() << "'\n";
			for(auto& plr : mods::globals::get_room_list(npc->room())) {
				vict = plr->cd();
				if(IS_NPC(vict) || !CAN_SEE(ch, vict) || PRF_FLAGGED(vict, PRF_NOHASSLE)) {
					continue;
				}

				if(MOB_FLAGGED(ch, MOB_WIMPY) && AWAKE(vict)) {
					continue;
				}

				if(MOB_FLAGGED(ch, MOB_AGGRESSIVE) ||
				        (MOB_FLAGGED(ch, MOB_AGGR_EVIL) && IS_EVIL(vict)) ||
				        (MOB_FLAGGED(ch, MOB_AGGR_NEUTRAL) && IS_NEUTRAL(vict)) ||
				        (MOB_FLAGGED(ch, MOB_AGGR_GOOD) && IS_GOOD(vict))) {

					/* Can a master successfully control the charmed monster? */
					if(aggressive_mob_on_a_leash(ch, ch->master, vict)) {
						continue;
					}

					hit(ch, vict, TYPE_UNDEFINED);
					found = TRUE;
				}
				if(found) {
					break;
				}
			}
		}// end if mob flagged aggro

		/* Mob Memory */
		if(MOB_FLAGGED(ch, MOB_MEMORY) && MEMORY(ch).size()) {
			//std::cerr << "mobile_activity: mob memory\n";

			found = FALSE;
			for(auto& player_vict : mods::globals::get_room_list(IN_ROOM(ch))) {
				auto vict = player_vict->cd();
				if(IS_NPC(vict) || !CAN_SEE(ch, vict) || PRF_FLAGGED(vict, PRF_NOHASSLE)) {
					continue;
				}

				if(ch->mob_specials.memory.end() !=
				        ch->mob_specials.memory.find(player_vict->uuid())) {
					/* Can a master successfully control the charmed monster? */
					if(aggressive_mob_on_a_leash(ch, ch->master, vict)) {
						continue;
					}
					act("$n screams at $N, \"Hey! You're the fiend that attacked me!\"", FALSE, ch, 0, vict, TO_ROOM);
					hit(ch, vict, TYPE_UNDEFINED);
					break;
				}
				continue;
			}
		}

		/*
		 * Charmed Mob Rebellion
		 *
		 * In order to rebel, there need to be more charmed monsters
		 * than the person can feasibly control at a time.  Then the
		 * mobiles have a chance based on the charisma of their leader.
		 *
		 * 1-4 = 0, 5-7 = 1, 8-10 = 2, 11-13 = 3, 14-16 = 4, 17-19 = 5, etc.
		 */
		if(AFF_FLAGGED(ch, AFF_CHARM) && ch->master && num_followers_charmed(ch->master) > (GET_CHA(ch->master) - 2) / 3) {
			if(!aggressive_mob_on_a_leash(ch, ch->master, ch->master)) {
				if(CAN_SEE(ch, ch->master) && !PRF_FLAGGED(ch->master, PRF_NOHASSLE)) {
					hit(ch, ch->master, TYPE_UNDEFINED);
				}

				stop_follower(ch);
			}
		}

		/* Helper Mobs */
		if(MOB_FLAGGED(ch, MOB_HELPER) && !AFF_FLAGGED(ch, AFF_BLIND | AFF_CHARM)) {
			found = FALSE;

			FOR_ROOM(vict) {
				if(vict->is(ch) || !IS_NPC(vict->cd()) || !FIGHTING(vict->cd())) {
					continue;
				}

				if(IS_NPC(FIGHTING(vict->cd())) || ch == FIGHTING(vict->cd())) {
					continue;
				}

				act("$n jumps to the aid of $N!", FALSE, ch, 0, vict->cd(), TO_ROOM);
				hit(ch, FIGHTING(vict->cd()), TYPE_UNDEFINED);
				found = TRUE;
			}
		}
	} // END FOREACH_MOB
}



/* make ch remember victim */
void remember(char_data *ch,char_data *victim) {
	auto ov = ptr_opt(victim);
	if(!ov.has_value()) {
		return;
	}
	auto op = ptr_opt(ch);
	if(!op.has_value()) {
		return;
	}
	auto p = op.value();
	auto v = ov.value();
	if(!p->is_npc() || v->is_npc() || PRF_FLAGGED(victim, PRF_NOHASSLE)) {
		ch->mob_specials.memory.erase(v->uuid());
		return;
	}
	ch->mob_specials.memory.emplace(v->uuid());
}


/* make ch forget victim */
void forget(char_data *ch,char_data *victim) {
	auto ov = ptr_opt(victim);
	if(!ov.has_value()) {
		return;
	}
	ch->mob_specials.memory.erase(ov.value()->uuid());
}


/* erase ch's memory */
void clearMemory(char_data *ch) {
	ch->mob_specials.memory.clear();
}


/*
 * An aggressive mobile wants to attack something.  If
 * they're under the influence of mind altering PC, then
 * see if their master can talk them out of it, eye them
 * down, or otherwise intimidate the slave.
 */
bool aggressive_mob_on_a_leash(char_data *slave,char_data *master,char_data *attack) {
	static int snarl_cmd;
	int dieroll;

	if(!master || !AFF_FLAGGED(slave, AFF_CHARM)) {
		return (FALSE);
	}

	if(!snarl_cmd) {
		snarl_cmd = find_command("snarl");
	}

	/* Sit. Down boy! HEEEEeeeel! */
	dieroll = rand_number(1, 20);

	if(dieroll != 1 && (dieroll == 20 || dieroll > 10 - GET_CHA(master) + GET_INT(slave))) {
		if(snarl_cmd > 0 && attack && !rand_number(0, 3)) {
			char victbuf[MAX_NAME_LENGTH + 1];

			strncpy(victbuf, GET_NAME(attack), sizeof(victbuf));	/* strncpy: OK */
			victbuf[sizeof(victbuf) - 1] = '\0';

			auto ch = slave;
			MENTOC_PREAMBLE();
			do_action(slave, victbuf, snarl_cmd, 0,player);
		}

		/* Success! But for how long? Hehe. */
		return (TRUE);
	}

	/* So sorry, now you're a player killer... Tsk tsk. */
	return (FALSE);
}

namespace mods::mobact {
	SUPERCMD(do_mobact) {
		ADMIN_REJECT();
		player->sendx("Running...");
		mobile_activity();
		ADMIN_DONE();
	}
	void init() {
		mods::interpreter::add_command("mobact", POS_RESTING, do_mobact, LVL_BUILDER,0);
	}
};
#undef m_can_roam
