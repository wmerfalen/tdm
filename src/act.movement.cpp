/* ************************************************************************
 *   File: act.movement.c                                Part of CircleMUD *
 *  Usage: movement commands, door handling, & sleep/rest/etc state        *
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
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include "house.h"
#include "constants.h"
#include "mods/classes/breacher.hpp"
#include "mods/classes/ghost.hpp"
#include "mods/contract-events.hpp"
#include "mods/resting.hpp"

/* external variables  */
extern int tunnel_size;

/* external functions */
int special(char_data *ch, int cmd, char *arg);
void death_cry(char_data *ch);
int find_eq_pos(char_data *ch, struct obj_data *obj, char *arg);

/* local functions */
int has_boat(char_data *ch);
int find_door(char_data *ch, const char *type, char *dir, const char *cmdname);
int has_key(char_data *ch, obj_vnum key);
void do_doorcmd(char_data *ch, struct obj_data *obj, int door, int scmd);
int ok_pick(char_data *ch, obj_vnum keynum, int pickproof, int scmd);
ACMD(do_gen_door);
ACMD(do_enter);
ACMD(do_leave);
ACMD(do_stand);
ACMD(do_sit);
ACMD(do_rest);
ACMD(do_sleep);
ACMD(do_wake);
ACMD(do_follow);


/* simple function to determine if char can walk on water */
int has_boat(char_data *ch) {
	struct obj_data *obj;
	int i;

	/*
		 if (ROOM_IDENTITY(IN_ROOM(ch)) == DEAD_SEA)
		 return (1);
		 */

	if(GET_LEVEL(ch) > LVL_IMMORT) {
		return (1);
	}

	/* non-wearable boats in inventory will do it */
	for(obj = ch->carrying; obj; obj = obj->next_content)
		if(GET_OBJ_TYPE(obj) == ITEM_BOAT && (find_eq_pos(ch, obj, NULL) < 0)) {
			return (1);
		}

	/* and any boat you're wearing will do it too */
	for(i = 0; i < NUM_WEARS; i++)
		if(GET_EQ(ch, i) && GET_OBJ_TYPE(GET_EQ(ch, i)) == ITEM_BOAT) {
			return (1);
		}

	return (0);
}



/* do_simple_move assumes
 *    1. That there is no master and no followers.
 *    2. That the direction exists.
 *
 *   Returns :
 *   1 : If succes.
 *   0 : If fail
 */
int do_simple_move(char_data *ch, int dir, int need_specials_check) {
	MENTOC_PREAMBLE();
	room_rnum was_in = player->room();
	if(player->room_pave_mode() && player->is_executing_js()) {
		if(world[was_in].dir_option[dir]) {
			auto to_room = world[was_in].dir_option[dir]->to_room;
			mods::globals::rooms::char_from_room(ch);
			mods::globals::rooms::char_to_room(to_room,ch);
			return 1;
		}
	}
	char throwaway[MAX_INPUT_LENGTH] = ""; /* Functions assume writable. */
	int need_movement;
	bool ghost_dissipated = player->ghost() && player->ghost()->is_dissipated();

	/*
	 * Check for special routines (North is 1 in command list, but 0 here) Note
	 * -- only check if following; this avoids 'double spec-proc' bug
	 */
	if(need_specials_check && special(ch, dir + 1, throwaway)) {
		return (0);
	}

	/* charmed? */
	if(AFF_FLAGGED(ch, AFF_CHARM) && ch->master && IN_ROOM(ch) == IN_ROOM(ch->master)) {
		send_to_char(ch, "The thought of leaving your master makes you weep.");
		act("$n bursts into tears.", FALSE, ch, 0, 0, TO_ROOM);
		return (0);
	}

	/* if this room or the one we're going to needs a boat, check for one */
	if((SECT(IN_ROOM(ch)) == SECT_WATER_NOSWIM) ||
	    (SECT(EXIT(ch, dir)->to_room) == SECT_WATER_NOSWIM)) {
		if(!has_boat(ch)) {
			send_to_char(ch, "You need a boat to go there.");
			return (0);
		}
	}

	/* move points needed is avg. move loss for src and destination sect type */
	need_movement = (movement_loss[SECT(IN_ROOM(ch))] +
	        movement_loss[SECT(EXIT(ch, dir)->to_room)]) / 2;

	if(!ghost_dissipated && GET_MOVE(ch) < need_movement && !IS_NPC(ch)) {
		if(need_specials_check && ch->master) {
			send_to_char(ch, "You are too exhausted to follow.");
		} else {
			send_to_char(ch, "You are too exhausted.");
		}

		return (0);
	}

	if(ROOM_FLAGGED(IN_ROOM(ch), ROOM_ATRIUM)) {
		if(!House_can_enter(ch, GET_ROOM_VNUM(EXIT(ch, dir)->to_room))) {
			send_to_char(ch, "That's private property -- no trespassing!");
			return (0);
		}
	}

	if(ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_TUNNEL) &&
	    num_pc_in_room(&(world[EXIT(ch, dir)->to_room])) >= tunnel_size) {
		if(tunnel_size > 1) {
			send_to_char(ch, "There isn't enough room for you to go there!");
		} else {
			send_to_char(ch, "There isn't enough room there for more than one person!");
		}

		return (0);
	}

	/* Mortals and low level gods cannot enter greater god rooms. */
	if(ROOM_FLAGGED(EXIT(ch, dir)->to_room, ROOM_GODROOM) &&
	    GET_LEVEL(ch) < LVL_GRGOD) {
		send_to_char(ch, "You aren't godly enough to use that room!");
		return (0);
	}

	/* Now we know we're allow to go into the room. */
	if(GET_LEVEL(ch) < LVL_IMMORT && !IS_NPC(ch)) {
		GET_MOVE(ch) -= need_movement;
	}

	was_in = IN_ROOM(ch);
	auto to_room = world[was_in].dir_option[dir]->to_room;
	mods::globals::rooms::char_from_room(ch);
	if(!AFF_FLAGGED(ch, AFF_SNEAK)) {
		char buf2[MAX_STRING_LENGTH];

		if(!ghost_dissipated) {
			snprintf(buf2, sizeof(buf2), "$n leaves %s.", dirs[dir]);
			act(buf2, TRUE, ch, 0, 0, TO_ROOM);
		}
	}
	if(!mods::globals::rooms::char_to_room(to_room,ch)) {
		return 0;
	}


	if(world[player->room()].has_texture(room_data::texture_type_t::ELEVATOR) && (dir == UP || dir == DOWN)) {
		player->sendln(
		    CAT(
		        "You move the elevator ",dir == UP ? "UP" : "DOWN",
		        "..."
		    )
		);
	}
	if(!AFF_FLAGGED(ch, AFF_SNEAK) && !ghost_dissipated) {
		//if(player->is_fighting(i)){
		//	act("$n has arrived {red}[!]{/red}.", TRUE, ch, 0, 0, TO_ROOM);
		//} else {
		act("$n has arrived$f.", TRUE, ch, 0, 0, TO_ROOM);
		//}
	}


	if(ch->has_desc) {
		look_at_room(ch, 0);
	}
	if(!IS_NPC(ch) && ch->contract) {
		mods::contract_events::find_room(player,to_room);
		mods::contract_events::gain_entry_to_room(player,to_room);
	}

	if(ROOM_FLAGGED(IN_ROOM(ch), ROOM_DEATH) && GET_LEVEL(ch) < LVL_IMMORT) {
		log_death_trap(ch);
		death_cry(ch);
		extract_char(ch);
		return (0);
	}

	return (1);
}


int perform_move(char_data *ch, int dir, int need_specials_check) {
	MENTOC_PREAMBLE();
	if(player->builder_data) {
		do_simple_move(ch, dir, 0);
		return 1;
	}
	room_rnum was_in;
	follow_type *k, *next;
	bool ghost_dissipated = player->ghost() && player->ghost()->is_dissipated();
	bool is_breacher = player->breacher() != nullptr;
	bool is_exit = EXIT(ch, dir);

	if(ch == NULL || dir < 0 || dir >= NUM_OF_DIRS) {
		log("SYSERR: perform_move received invalid parameters");
		return (0);
	}
	if(!player->can_move()) {
		return 0;
	}

	bool exit_closed = is_exit && EXIT_FLAGGED(EXIT(ch, dir), EX_CLOSED) /* !mods */
	    && !EXIT_FLAGGED(EXIT(ch,dir),EX_BREACHED) &&
	    !IS_SET(world[EXIT(ch,dir)->to_room].dir_option[OPPOSITE_DIR(dir)]->exit_info,EX_BREACHED);
	if(exit_closed && ghost_dissipated) {
		was_in = IN_ROOM(ch);
		do_simple_move(ch, dir, need_specials_check);
		return 0;
	}

	if((!EXIT(ch, dir) || EXIT(ch, dir)->to_room == NOWHERE)) {
		if(is_breacher) {
			player->breacher()->attempt_direction(dir);
			return 0;
		}
		player->sendln("Alas, you cannot go that way.");
		return 0;
	} else if(exit_closed) {
		if(EXIT(ch, dir)->keyword) {
			send_to_char(ch, "The %s seems to be closed.", fname(EXIT(ch, dir)->keyword));
		} else {
			send_to_char(ch, "It seems to be closed.");
		}
	}
	/* !mods */
	else if(EXIT_FLAGGED(EXIT(ch,dir),EX_REINFORCED)) {
		send_to_char(ch,"Door is reinforced. You must use {red}thermite{/red} to breach\r\n");
		return 0;
	} else {
		if(!ch->followers) {
			return (do_simple_move(ch, dir, need_specials_check));
		}

		was_in = IN_ROOM(ch);

		if(!do_simple_move(ch, dir, need_specials_check)) {
			return (0);
		}

		for(k = ch->followers; k; k = next) {
			next = k->next;

			if(IN_ROOM(k->follower) == was_in) {
				if(GET_POS(k->follower) >= POS_STANDING) {
					act("You follow $N.\r\n", FALSE, k->follower, 0, ch, TO_CHAR);
					perform_move(k->follower, dir, 1);
				}
			}

			return (1);
		}

		return (0);
	}
	return 0;
}

ACMD(do_move) {
	/*
	 * This is basically a mapping of cmd numbers to perform_move indices.
	 * It cannot be done in perform_move because perform_move is called
	 * by other functions which do not require the remapping.
	 */
	perform_move(ch, subcmd - 1, 0);
	return;
}


int find_door(char_data *ch, const char *type, char *dir, const char *cmdname) {
	int door;

	if(*dir) {			/* a direction was specified */
		if((door = search_block(dir, dirs, FALSE)) == -1) {	/* Partial Match */
			send_to_char(ch, "That's not a direction.");
			return (-1);
		}

		if(EXIT(ch, door)) {	/* Braces added according to indent. -gg */
			if(EXIT(ch, door)->keyword) {
				if(isname(type, EXIT(ch, door)->keyword)) {
					return (door);
				} else {
					send_to_char(ch, "I see no %s there.", type);
					return (-1);
				}
			} else {
				return (door);
			}
		} else {
			send_to_char(ch, "I really don't see how you can %s anything there.", cmdname);
			return (-1);
		}
	} else {			/* try to locate the keyword */
		if(!*type) {
			send_to_char(ch, "What is it you want to %s?", cmdname);
			return (-1);
		}

		for(door = 0; door < NUM_OF_DIRS; door++)
			if(EXIT(ch, door))
				if(EXIT(ch, door)->keyword)
					if(isname(type, EXIT(ch, door)->keyword)) {
						return (door);
					}

		send_to_char(ch, "There doesn't seem to be %s %s here.", AN(type), type);
		return (-1);
	}
}


int has_key(char_data *ch, obj_vnum key) {
	struct obj_data *o;

	for(o = ch->carrying; o; o = o->next_content)
		if(GET_OBJ_VNUM(o) == key) {
			return (1);
		}

	if(GET_EQ(ch, WEAR_HOLD))
		if(GET_OBJ_VNUM(GET_EQ(ch, WEAR_HOLD)) == key) {
			return (1);
		}

	return (0);
}



#define NEED_OPEN	(1 << 0)
#define NEED_CLOSED	(1 << 1)
#define NEED_UNLOCKED	(1 << 2)
#define NEED_LOCKED	(1 << 3)

const char *cmd_door[] = {
	"open",
	"close",
	"unlock",
	"lock",
	"pick"
};

const int flags_door[] = {
	NEED_CLOSED | NEED_UNLOCKED,
	NEED_OPEN,
	NEED_CLOSED | NEED_LOCKED,
	NEED_CLOSED | NEED_UNLOCKED,
	NEED_CLOSED | NEED_LOCKED
};


#define EXITN(room, door)		(world[room].dir_option[door])
#define OPEN_DOOR(room, obj, door)	((obj) ?\
    (REMOVE_BIT(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) :\
    (REMOVE_BIT(EXITN(room, door)->exit_info, EX_CLOSED)))
#define CLOSE_DOOR(room, obj, door)	((obj) ?\
    (SET_BIT(GET_OBJ_VAL(obj, 1), CONT_CLOSED)) :\
    (SET_BIT(EXITN(room, door)->exit_info, EX_CLOSED)))
#define LOCK_DOOR(room, obj, door)	((obj) ?\
    (SET_BIT(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) :\
    (SET_BIT(EXITN(room, door)->exit_info, EX_LOCKED)))
#define UNLOCK_DOOR(room, obj, door)	((obj) ?\
    (REMOVE_BIT(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) :\
    (REMOVE_BIT(EXITN(room, door)->exit_info, EX_LOCKED)))
#define TOGGLE_LOCK(room, obj, door)	((obj) ?\
    (TOGGLE_BIT(GET_OBJ_VAL(obj, 1), CONT_LOCKED)) :\
    (TOGGLE_BIT(EXITN(room, door)->exit_info, EX_LOCKED)))

void do_doorcmd(char_data *ch, struct obj_data *obj, int door, int scmd) {
	char buf[MAX_STRING_LENGTH];
	size_t len;
	room_rnum other_room = NOWHERE;
	struct room_direction_data *back = NULL;

	len = snprintf(buf, sizeof(buf), "$n %ss ", cmd_door[scmd]);

	if(!obj && ((other_room = EXIT(ch, door)->to_room) != NOWHERE))
		if((back = world[other_room].dir_option[rev_dir[door]]) != NULL)
			if(back->to_room != IN_ROOM(ch)) {
				back = NULL;
			}

	switch(scmd) {
		case SCMD_OPEN:
			OPEN_DOOR(IN_ROOM(ch), obj, door);

			if(back) {
				OPEN_DOOR(other_room, obj, rev_dir[door]);
			}

			send_to_char(ch, "%s", OK);
			break;

		case SCMD_CLOSE:
			CLOSE_DOOR(IN_ROOM(ch), obj, door);

			if(back) {
				CLOSE_DOOR(other_room, obj, rev_dir[door]);
			}

			send_to_char(ch, "%s", OK);
			break;

		case SCMD_LOCK:
			LOCK_DOOR(IN_ROOM(ch), obj, door);

			if(back) {
				LOCK_DOOR(other_room, obj, rev_dir[door]);
			}

			send_to_char(ch, "*Click*");
			break;

		case SCMD_UNLOCK:
			UNLOCK_DOOR(IN_ROOM(ch), obj, door);

			if(back) {
				UNLOCK_DOOR(other_room, obj, rev_dir[door]);
			}

			send_to_char(ch, "*Click*");
			break;

		case SCMD_PICK:
			TOGGLE_LOCK(IN_ROOM(ch), obj, door);

			if(back) {
				TOGGLE_LOCK(other_room, obj, rev_dir[door]);
			}

			send_to_char(ch, "The lock quickly yields to your skills.");
			len = strlcpy(buf, "$n skillfully picks the lock on ", sizeof(buf));
			break;
	}

	/* Notify the room. */
	if(len < sizeof(buf))
		snprintf(buf + len, sizeof(buf) - len, "%s%s.",
		    obj ? "" : "the ", obj ? "$p" : EXIT(ch, door)->keyword ? "$F" : "door");

	/** This doesn't look right... -mentoc */
	if(!obj || IN_ROOM(obj) != NOWHERE) {
		act(buf, FALSE, ch, obj, obj ? 0 : static_cast<char*>(EXIT(ch, door)->keyword.ptr()), TO_ROOM);
	}

	/* Notify the other room */
	if(back && (scmd == SCMD_OPEN || scmd == SCMD_CLOSE))
		send_to_room(EXIT(ch, door)->to_room, "The %s is %s%s from the other side.",
		    back->keyword ? fname(back->keyword) : "door", cmd_door[scmd],
		    scmd == SCMD_CLOSE ? "d" : "ed");
}


int ok_pick(char_data *ch, obj_vnum keynum, int pickproof, int scmd) {
	int percent, skill_lvl;

	if(scmd != SCMD_PICK) {
		return (1);
	}

	percent = rand_number(1, 101);
	skill_lvl = GET_SKILL(ch, SKILL_PICK_LOCK) + dex_app_skill[GET_DEX(ch)].p_locks;

	if(keynum == NOTHING) {
		send_to_char(ch, "Odd - you can't seem to find a keyhole.");
	} else if(pickproof) {
		send_to_char(ch, "It resists your attempts to pick it.");
	} else if(percent > skill_lvl) {
		send_to_char(ch, "You failed to pick the lock.");
	} else {
		return (1);
	}

	return (0);
}


#define DOOR_IS_OPENABLE(ch, obj, door)	((obj) ? \
    ((GET_OBJ_TYPE(obj) == ITEM_CONTAINER) && \
        OBJVAL_FLAGGED(obj, CONT_CLOSEABLE)) :\
    (EXIT_FLAGGED(EXIT(ch, door), EX_ISDOOR)))
#define DOOR_IS_OPEN(ch, obj, door)	((obj) ? \
    (!OBJVAL_FLAGGED(obj, CONT_CLOSED)) :\
    (!EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED)))
#define DOOR_IS_UNLOCKED(ch, obj, door)	((obj) ? \
    (!OBJVAL_FLAGGED(obj, CONT_LOCKED)) :\
    (!EXIT_FLAGGED(EXIT(ch, door), EX_LOCKED)))
#define DOOR_IS_PICKPROOF(ch, obj, door) ((obj) ? \
    (OBJVAL_FLAGGED(obj, CONT_PICKPROOF)) : \
    (EXIT_FLAGGED(EXIT(ch, door), EX_PICKPROOF)))
#define DOOR_IS_BREACHED(ch, obj, door) (EXIT_FLAGGED(EXIT(ch, door), EX_BREACHED))
/* !mods */
#define DOOR_IS_THERMITE(ch, obj, door) EXIT_FLAGGED(EXIT(ch, door), EX_REINFORCED)

#define DOOR_IS_CLOSED(ch, obj, door)	(!(DOOR_IS_OPEN(ch, obj, door)))
#define DOOR_IS_LOCKED(ch, obj, door)	(!(DOOR_IS_UNLOCKED(ch, obj, door)))
#define DOOR_KEY(ch, obj, door)		((obj) ? (GET_OBJ_VAL(obj, 2)) : \
    (EXIT(ch, door)->key))


ACMD(do_gen_door) {

	int door = -1;
	obj_vnum keynum;
	struct obj_data *obj = NULL;
	char_data *victim = NULL;

	auto vec_args = PARSE_ARGS();

	if(vec_args.size() < 2) {
		send_to_char(ch, "%c%s what?", UPPER(*cmd_door[subcmd]), cmd_door[subcmd] + 1);
		return;
	}

	if(!generic_find((char*)vec_args[0].c_str(), FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &victim, &obj)) {
		door = find_door(ch, (char*)vec_args[0].c_str(), (char*)vec_args[1].c_str(), cmd_door[subcmd]);
	}

	if((obj) || (door >= 0)) {
		keynum = DOOR_KEY(ch, obj, door);

		/* TODO: !mods Add thermite and regularly breached doors */
		if(DOOR_IS_THERMITE(ch,obj,door)) {
			*player << "Door is re-inforced. {red}Requires thermite charge{/red}\r\n";
			return;
		} else if(DOOR_IS_BREACHED(ch,obj,door)) {
			OPEN_DOOR(IN_ROOM(ch), obj, door);
			return;
		} else if(!(DOOR_IS_OPENABLE(ch, obj, door))) {
			void* vict_obj = const_cast<char*>(cmd_door[subcmd]);
			act("You can't $F that!", FALSE, ch, 0, vict_obj, TO_CHAR);
		} else if(!DOOR_IS_OPEN(ch, obj, door) &&
		    IS_SET(flags_door[subcmd], NEED_OPEN)) {
			send_to_char(ch, "But it's already closed!");
		} else if(!DOOR_IS_CLOSED(ch, obj, door) &&
		    IS_SET(flags_door[subcmd], NEED_CLOSED)) {
			send_to_char(ch, "But it's currently open!");
		} else if(!(DOOR_IS_LOCKED(ch, obj, door)) &&
		    IS_SET(flags_door[subcmd], NEED_LOCKED)) {
			send_to_char(ch, "Oh.. it wasn't locked, after all..");
		} else if(!(DOOR_IS_UNLOCKED(ch, obj, door)) &&
		    IS_SET(flags_door[subcmd], NEED_UNLOCKED)) {
			send_to_char(ch, "It is boarded up. Use {grn}'breach'{/grn} to break it open");
		} else if(!has_key(ch, keynum) && (GET_LEVEL(ch) < LVL_GOD) &&
		    ((subcmd == SCMD_LOCK) || (subcmd == SCMD_UNLOCK))) {
			send_to_char(ch, "You don't seem to have the proper key. Use {grn}'breach'{/grn} to {red}break{/red} it open");
		}
		/* TODO: Thermite door */
		else if(ok_pick(ch, keynum, DOOR_IS_PICKPROOF(ch, obj, door), subcmd)) {
			do_doorcmd(ch, obj, door, subcmd);
		}
	}

	return;
}



ACMD(do_enter) {
	char buf[MAX_INPUT_LENGTH];
	int door;

	one_argument(argument, buf);

	if(*buf) {
		/* an argument was supplied, search for door
		 * keyword */
		for(door = 0; door < NUM_OF_DIRS; door++)
			if(EXIT(ch, door))
				if(EXIT(ch, door)->keyword)
					if(!str_cmp(EXIT(ch, door)->keyword, buf)) {
						perform_move(ch, door, 1);
						return;
					}

		send_to_char(ch, "There is no %s here.", buf);
	} else if(ROOM_FLAGGED(IN_ROOM(ch), ROOM_INDOORS)) {
		send_to_char(ch, "You are already indoors.");
	} else {
		/* try to locate an entrance */
		for(door = 0; door < NUM_OF_DIRS; door++)
			if(EXIT(ch, door))
				if(EXIT(ch, door)->to_room != NOWHERE)
					if(!EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED) &&
					    ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_INDOORS)) {
						perform_move(ch, door, 1);
						return;
					}

		send_to_char(ch, "You can't seem to find anything to enter.");
	}
}


ACMD(do_leave) {
	int door;

	if(OUTSIDE(ch)) {
		send_to_char(ch, "You are outside.. where do you want to go?");
	} else {
		for(door = 0; door < NUM_OF_DIRS; door++)
			if(EXIT(ch, door))
				if(EXIT(ch, door)->to_room != NOWHERE)
					if(!EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED) &&
					    !ROOM_FLAGGED(EXIT(ch, door)->to_room, ROOM_INDOORS)) {
						perform_move(ch, door, 1);
						return;
					}

		send_to_char(ch, "I see no obvious exits to the outside.");
	}
}


ACMD(do_stand) {
	switch(GET_POS(ch)) {
		case POS_STANDING:
			send_to_char(ch, "You are already standing.");
			break;

		case POS_SITTING:
			send_to_char(ch, "You stand up.");
			act("$n clambers to $s feet.", TRUE, ch, 0, 0, TO_ROOM);
			/* Will be sitting after a successful bash and may still be fighting. */
			GET_POS(ch) = FIGHTING(ch) ? POS_FIGHTING : POS_STANDING;
			break;

		case POS_RESTING:
			send_to_char(ch, "You stop resting, and stand up.");
			act("$n stops resting, and clambers on $s feet.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_STANDING;
			break;

		case POS_SLEEPING:
			send_to_char(ch, "You have to wake up first!");
			break;

		case POS_FIGHTING:
			send_to_char(ch, "Do you not consider fighting as standing?");
			break;

		default:
			send_to_char(ch, "You stop floating around, and put your feet on the ground.");
			act("$n stops floating around, and puts $s feet on the ground.",
			    TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_STANDING;
			break;
	}
}


ACMD(do_sit) {
	switch(GET_POS(ch)) {
		case POS_STANDING:
			send_to_char(ch, "You sit down.");
			act("$n sits down.", FALSE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SITTING;
			break;

		case POS_SITTING:
			send_to_char(ch, "You're sitting already.");
			break;

		case POS_RESTING:
			send_to_char(ch, "You stop resting, and sit up.");
			act("$n stops resting.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SITTING;
			break;

		case POS_SLEEPING:
			send_to_char(ch, "You have to wake up first.");
			break;

		case POS_FIGHTING:
			send_to_char(ch, "Sit down while fighting? Are you MAD?");
			break;

		default:
			send_to_char(ch, "You stop floating around, and sit down.");
			act("$n stops floating around, and sits down.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SITTING;
			break;
	}
}


ACMD(do_rest) {
	switch(GET_POS(ch)) {
		case POS_STANDING:
			send_to_char(ch, "You sit down and rest your tired bones.");
			act("$n sits down and rests.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_RESTING;
			break;

		case POS_SITTING:
			send_to_char(ch, "You rest your tired bones.");
			act("$n rests.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_RESTING;
			break;

		case POS_RESTING:
			send_to_char(ch, "You are already resting.");
			break;

		case POS_SLEEPING:
			send_to_char(ch, "You have to wake up first.");
			break;

		case POS_FIGHTING:
			send_to_char(ch, "Rest while fighting?  Are you MAD?");
			break;

		default:
			send_to_char(ch, "You stop floating around, and stop to rest your tired bones.");
			act("$n stops floating around, and rests.", FALSE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SITTING;
			break;
	}
}


ACMD(do_sleep) {
	switch(GET_POS(ch)) {
		case POS_STANDING:
		case POS_SITTING:
		case POS_RESTING:
			send_to_char(ch, "You go to sleep.");
			act("$n lies down and falls asleep.", TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SLEEPING;
			mods::resting::add_player_resting(player);
			break;

		case POS_SLEEPING:
			send_to_char(ch, "You are already sound asleep.");
			break;

		case POS_FIGHTING:
			send_to_char(ch, "Sleep while fighting?  Are you MAD?");
			break;

		default:
			send_to_char(ch, "You stop floating around, and lie down to sleep.");
			act("$n stops floating around, and lie down to sleep.",
			    TRUE, ch, 0, 0, TO_ROOM);
			GET_POS(ch) = POS_SLEEPING;
			break;
	}
}


ACMD(do_wake) {
	char arg[MAX_INPUT_LENGTH];
	char_data *vict;
	int self = 0;

	one_argument(argument, arg);

	if(*arg) {
		if(GET_POS(ch) == POS_SLEEPING) {
			send_to_char(ch, "Maybe you should wake yourself up first.");
		} else if((vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM)) == NULL) {
			send_to_char(ch, "%s", NOPERSON);
		} else if(vict == ch) {
			self = 1;
		} else if(AWAKE(vict)) {
			act("$E is already awake.", FALSE, ch, 0, vict, TO_CHAR);
		} else if(AFF_FLAGGED(vict, AFF_SLEEP)) {
			act("You can't wake $M up!", FALSE, ch, 0, vict, TO_CHAR);
		} else if(GET_POS(vict) < POS_SLEEPING) {
			act("$E's in pretty bad shape!", FALSE, ch, 0, vict, TO_CHAR);
		} else {
			act("You wake $M up.", FALSE, ch, 0, vict, TO_CHAR);
			act("You are awakened by $n.", FALSE, ch, 0, vict, TO_VICT | TO_SLEEP);
			GET_POS(vict) = POS_SITTING;
			mods::resting::remove_player_resting(player);
		}

		if(!self) {
			return;
		}
	}

	if(AFF_FLAGGED(ch, AFF_SLEEP)) {
		send_to_char(ch, "You can't wake up!");
	} else if(GET_POS(ch) > POS_SLEEPING) {
		send_to_char(ch, "You are already awake...");
	} else {
		send_to_char(ch, "You awaken, and sit up.");
		act("$n awakens.", TRUE, ch, 0, 0, TO_ROOM);
		GET_POS(ch) = POS_SITTING;
		mods::resting::remove_player_resting(player);
	}
}


ACMD(do_follow) {
	char buf[MAX_INPUT_LENGTH];
	char_data *leader;

	one_argument(argument, buf);

	if(*buf) {
		if(!(leader = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
			send_to_char(ch, "%s", NOPERSON);
			return;
		}
	} else {
		send_to_char(ch, "Whom do you wish to follow?");
		return;
	}

	if(ch->master == leader) {
		act("You are already following $M.", FALSE, ch, 0, leader, TO_CHAR);
		return;
	}

	if(AFF_FLAGGED(ch, AFF_CHARM) && (ch->master)) {
		act("But you only feel like following $N!", FALSE, ch, 0, ch->master, TO_CHAR);
	} else {			/* Not Charmed follow person */
		if(leader == ch) {
			if(!ch->master) {
				send_to_char(ch, "You are already following yourself.");
				return;
			}

			stop_follower(ch);
		} else {
			if(circle_follow(ch, leader)) {
				send_to_char(ch, "Sorry, but following in loops is not allowed.");
				return;
			}

			if(ch->master) {
				stop_follower(ch);
			}

			REMOVE_BIT(AFF_FLAGS(ch), AFF_GROUP);
			add_follower(ch, leader);
		}
	}
}
