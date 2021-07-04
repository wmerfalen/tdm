/* ************************************************************************
*   File: act.item.c                                    Part of CircleMUD *
*  Usage: object handling routines -- get/drop and container handling     *
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
#include "constants.h"
#include "mods/object-utils.hpp"
#include "mods/armor/includes.hpp"
#include "mods/yaml.hpp"
#include "mods/skills.hpp"
#include "mods/drone.hpp"
#include "mods/contract-events.hpp"

#define __MENTOC_SHOW_WEAR_DEBUG_OUTPUT__
/* extern variables */
extern room_rnum donation_room_1;
#if 0
extern room_rnum donation_room_2;  /* uncomment if needed! */
extern room_rnum donation_room_3;  /* uncomment if needed! */
#endif

/* local functions */
int can_take_obj(char_data *ch, struct obj_data *obj);
void get_check_money(char_data *ch, struct obj_data *obj);
int perform_get_from_room(char_data *ch, struct obj_data *obj);
void get_from_room(char_data *ch, char *arg, int amount);
void perform_give_gold(char_data *ch, char_data *vict, int amount);
void perform_give(char_data *ch, char_data *vict, struct obj_data *obj);
int perform_drop(char_data *ch, struct obj_data *obj, byte mode, const char *sname, room_rnum RDR);
void perform_drop_gold(char_data *ch, int amount, byte mode, room_rnum RDR);
char_data *give_find_vict(char_data *ch, char *arg);
void weight_change_object(struct obj_data *obj, int weight);
void perform_put(char_data *ch, struct obj_data *obj, struct obj_data *cont);
void name_from_drinkcon(struct obj_data *obj);
void get_from_container(char_data *ch, struct obj_data *cont, char *arg, int mode, int amount);
void name_to_drinkcon(struct obj_data *obj, int type);
void wear_message(char_data *ch, struct obj_data *obj, int where);
void perform_wear(char_data *ch, struct obj_data *obj, int where);
int find_eq_pos(char_data *ch, struct obj_data *obj, char *arg);
void perform_get_from_container(char_data *ch, struct obj_data *obj, struct obj_data *cont, int mode);
void perform_remove(player_ptr_t player,int pos);
ACMD(do_remove);
ACMD(do_put);
ACMD(do_get);
ACMD(do_drop);
ACMD(do_give);
ACMD(do_drink);
ACMD(do_eat);
ACMD(do_pour);
ACMD(do_wear);
ACMD(do_wield);
ACMD(do_grab);


void perform_put(char_data *ch, struct obj_data *obj,
                 struct obj_data *cont) {
	if(GET_OBJ_WEIGHT(cont) + GET_OBJ_WEIGHT(obj) > GET_OBJ_VAL(cont, 0)) {
		act("$p won't fit in $P.", FALSE, ch, obj, cont, TO_CHAR);
	} else if(OBJ_FLAGGED(obj, ITEM_NODROP) && IN_ROOM(cont) != NOWHERE) {
		act("You can't get $p out of your hand.", FALSE, ch, obj, NULL, TO_CHAR);
	} else {
		obj_from_char(obj);
		obj_to_obj(TO_OBJ_PTR(obj), TO_OBJ_PTR(cont));

		act("$n puts $p in $P.", TRUE, ch, obj, cont, TO_ROOM);

		/* Yes, I realize this is strange until we have auto-equip on rent. -gg */
		if(OBJ_FLAGGED(obj, ITEM_NODROP) && !OBJ_FLAGGED(cont, ITEM_NODROP)) {
			SET_BIT(GET_OBJ_EXTRA(cont), ITEM_NODROP);
			act("You get a strange feeling as you put $p in $P.", FALSE,
			    ch, obj, cont, TO_CHAR);
		} else {
			act("You put $p in $P.", FALSE, ch, obj, cont, TO_CHAR);
		}
	}
}


/* The following put modes are supported by the code below:

	1) put <object> <container>
	2) put all.<object> <container>
	3) put all <container>

	<container> must be in inventory or on ground.
	all objects to be put into container must be in inventory.
*/

ACMD(do_put) {
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	struct obj_data *obj, *next_obj, *cont;
	char_data *tmp_char;
	int obj_dotmode, cont_dotmode, found = 0, howmany = 1;
	char *theobj, *thecont;

	one_argument(two_arguments(argument, arg1, arg2), arg3);	/* three_arguments */

	if(*arg3 && is_number(arg1)) {
		howmany = atoi(arg1);
		theobj = arg2;
		thecont = arg3;
	} else {
		theobj = arg1;
		thecont = arg2;
	}

	obj_dotmode = find_all_dots(theobj);
	cont_dotmode = find_all_dots(thecont);

	if(!*theobj) {
		send_to_char(ch, "Put what in what?");
	} else if(cont_dotmode != FIND_INDIV) {
		send_to_char(ch, "You can only put things into one container at a time.");
	} else if(!*thecont) {
		send_to_char(ch, "What do you want to put %s in?", obj_dotmode == FIND_INDIV ? "it" : "them");
	} else {
		generic_find(thecont, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);

		if(!cont) {
			send_to_char(ch, "You don't see %s %s here.", AN(thecont), thecont);
		} else if(GET_OBJ_TYPE(cont) != ITEM_CONTAINER) {
			act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
		} else if(OBJVAL_FLAGGED(cont, CONT_CLOSED)) {
			send_to_char(ch, "You'd better open it first!");
		} else {
			if(obj_dotmode == FIND_INDIV) {	/* put <obj> <container> */
				if(!(obj = get_obj_in_list_vis(ch, theobj, NULL, ch->carrying))) {
					send_to_char(ch, "You aren't carrying %s %s.", AN(theobj), theobj);
				} else if(obj == cont && howmany == 1) {
					send_to_char(ch, "You attempt to fold it into itself, but fail.");
				} else {
					while(obj && howmany) {
						next_obj = obj->next_content;

						if(obj != cont) {
							howmany--;
							perform_put(ch, obj, cont);
						}

						obj = get_obj_in_list_vis(ch, theobj, NULL, next_obj);
					}
				}
			} else {
				for(obj = ch->carrying; obj; obj = next_obj) {
					next_obj = obj->next_content;

					if(obj != cont && CAN_SEE_OBJ(ch, obj) &&
					        (obj_dotmode == FIND_ALL || isname(theobj, obj->name))) {
						found = 1;
						perform_put(ch, obj, cont);
					}
				}

				if(!found) {
					if(obj_dotmode == FIND_ALL) {
						send_to_char(ch, "You don't seem to have anything to put in it.");
					} else {
						send_to_char(ch, "You don't seem to have any %ss.", theobj);
					}
				}
			}
		}
	}
}



int can_take_obj(char_data *ch, struct obj_data *obj) {
	if(mods::object_utils::is_installed(obj)) {
		act("$p: you must use the uninstall command to remove this device.", FALSE, ch, obj, 0, TO_CHAR);
		return 0;
	} else if(mods::object_utils::is_installing(obj)) {
		act("$p: you can't! You're currently installing it", FALSE, ch, obj, 0, TO_CHAR);
		return 0;
	} else if(IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
		act("$p: you can't carry that many items.", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	} else if((IS_CARRYING_W(ch) + GET_OBJ_WEIGHT(obj)) > CAN_CARRY_W(ch)) {
		act("$p: you can't carry that much weight.", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	} else if(!(CAN_WEAR(obj, ITEM_WEAR_TAKE))) {
		act("$p: you can't take that!", FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}

	return (1);
}


void get_check_money(char_data *ch, struct obj_data *obj) {
	int value = GET_OBJ_VAL(obj, 0);

	if(GET_OBJ_TYPE(obj) != ITEM_MONEY || value <= 0) {
		return;
	}

	extract_obj(obj);

	GET_GOLD(ch) += value;

	if(value == 1) {
		send_to_char(ch, "There was 1 coin.");
	} else {
		send_to_char(ch, "There were %d coins.", value);
	}
}


void perform_get_from_container(char_data *ch, struct obj_data *obj,
                                struct obj_data *cont, int mode) {
	if(mode == FIND_OBJ_INV || can_take_obj(ch, obj)) {
		if(IS_CARRYING_N(ch) >= CAN_CARRY_N(ch)) {
			act("$p: you can't hold any more items.", FALSE, ch, obj, 0, TO_CHAR);
		} else {
			obj_from_obj(obj);
			obj_to_char(obj, ch);
			act("You get $p from $P.", FALSE, ch, obj, cont, TO_CHAR);
			act("$n gets $p from $P.", TRUE, ch, obj, cont, TO_ROOM);
			get_check_money(ch, obj);
			if(ch->contract) {
				mods::contract_events::perform_get(ch,obj);
			}
		}
	}
}


void get_from_container(char_data *ch, struct obj_data *cont,
                        char *arg, int mode, int howmany) {
	struct obj_data *obj, *next_obj;
	int obj_dotmode, found = 0;

	obj_dotmode = find_all_dots(arg);

	if(OBJVAL_FLAGGED(cont, CONT_CLOSED)) {
		act("$p is closed.", FALSE, ch, cont, 0, TO_CHAR);
	} else if(obj_dotmode == FIND_INDIV) {
		if(!(obj = get_obj_in_list_vis(ch, arg, NULL, cont->contains))) {
			char buf[MAX_STRING_LENGTH];

			snprintf(buf, sizeof(buf), "There doesn't seem to be %s %s in $p.", AN(arg), arg);
			act(buf, FALSE, ch, cont, 0, TO_CHAR);
		} else {
			struct obj_data *obj_next;

			while(obj && howmany--) {
				obj_next = obj->next_content;
				perform_get_from_container(ch, obj, cont, mode);
				obj = get_obj_in_list_vis(ch, arg, NULL, obj_next);
			}
		}
	} else {
		if(obj_dotmode == FIND_ALLDOT && !*arg) {
			send_to_char(ch, "Get all of what?");
			return;
		}

		for(obj = cont->contains; obj; obj = next_obj) {
			next_obj = obj->next_content;

			if(CAN_SEE_OBJ(ch, obj) &&
			        (obj_dotmode == FIND_ALL || isname(arg, obj->name))) {
				found = 1;
				perform_get_from_container(ch, obj, cont, mode);
			}
		}

		if(!found) {
			if(obj_dotmode == FIND_ALL) {
				act("$p seems to be empty.", FALSE, ch, cont, 0, TO_CHAR);
			} else {
				char buf[MAX_STRING_LENGTH];

				snprintf(buf, sizeof(buf), "You can't seem to find any %ss in $p.", arg);
				act(buf, FALSE, ch, cont, 0, TO_CHAR);
			}
		}
	}
}


int perform_get_from_room(char_data *ch, struct obj_data *obj) {
	if(can_take_obj(ch, obj)) {
		obj_from_room(obj);
		obj_to_char(obj, ch);
		act("You get $p.", FALSE, ch, obj, 0, TO_CHAR);
		act("$n gets $p.", TRUE, ch, obj, 0, TO_ROOM);
		get_check_money(ch, obj);
		if(ch->contract) {
			mods::contract_events::perform_get(ch,obj);
		}
		return (1);
	}

	return (0);
}


void get_from_room(char_data *ch, char *arg, int howmany) {
	struct obj_data *obj, *next_obj;
	int dotmode, found = 0;

	dotmode = find_all_dots(arg);

	if(dotmode == FIND_INDIV) {
		if(!(obj = get_obj_in_list_vis(ch, arg, NULL, world[IN_ROOM(ch)].contents))) {
			send_to_char(ch, "You don't see %s %s here.", AN(arg), arg);
		} else {
			struct obj_data *obj_next;

			while(obj && howmany--) {
				obj_next = obj->next_content;
				perform_get_from_room(ch, obj);
				obj = get_obj_in_list_vis(ch, arg, NULL, obj_next);
			}
		}
	} else {
		if(dotmode == FIND_ALLDOT && !*arg) {
			send_to_char(ch, "Get all of what?");
			return;
		}

		for(obj = world[IN_ROOM(ch)].contents; obj; obj = next_obj) {
			next_obj = obj->next_content;

			if(CAN_SEE_OBJ(ch, obj) &&
			        (dotmode == FIND_ALL || isname(arg, obj->name))) {
				found = 1;
				perform_get_from_room(ch, obj);
			}
		}

		if(!found) {
			if(dotmode == FIND_ALL) {
				send_to_char(ch, "There doesn't seem to be anything here.");
			} else {
				send_to_char(ch, "You don't see any %ss here.", arg);
			}
		}
	}
}



ACMD(do_get) {
	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];

	int cont_dotmode, found = 0, mode;
	struct obj_data *cont;
	char_data *tmp_char;

	one_argument(two_arguments(argument, arg1, arg2), arg3);	/* three_arguments */

	if(!*arg1) {
		send_to_char(ch, "Get what?");
	} else if(!*arg2) {
		if(std::string(arg1).compare("drone") == 0) {
			mods::drone::get_drone(player->uuid());
		} else {
			get_from_room(ch, arg1, 1);
		}
	} else if(is_number(arg1) && !*arg3) {
		get_from_room(ch, arg2, atoi(arg1));
	} else {
		int amount = 1;

		if(is_number(arg1)) {
			amount = atoi(arg1);
			strcpy(arg1, arg2);	/* strcpy: OK (sizeof: arg1 == arg2) */
			strcpy(arg2, arg3);	/* strcpy: OK (sizeof: arg2 == arg3) */
		}

		cont_dotmode = find_all_dots(arg2);

		if(cont_dotmode == FIND_INDIV) {
			mode = generic_find(arg2, FIND_OBJ_INV | FIND_OBJ_ROOM, ch, &tmp_char, &cont);

			if(!cont) {
				send_to_char(ch, "You don't have %s %s.", AN(arg2), arg2);
			} else if(GET_OBJ_TYPE(cont) != ITEM_CONTAINER) {
				act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
			} else {
				get_from_container(ch, cont, arg1, mode, amount);
			}
		} else {
			if(cont_dotmode == FIND_ALLDOT && !*arg2) {
				send_to_char(ch, "Get from all of what?");
				return;
			}

			for(cont = ch->carrying; cont; cont = cont->next_content)
				if(CAN_SEE_OBJ(ch, cont) &&
				        (cont_dotmode == FIND_ALL || isname(arg2, cont->name))) {
					if(GET_OBJ_TYPE(cont) == ITEM_CONTAINER) {
						found = 1;
						get_from_container(ch, cont, arg1, FIND_OBJ_INV, amount);
					} else if(cont_dotmode == FIND_ALLDOT) {
						found = 1;
						act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
					}
				}

			for(cont = world[IN_ROOM(ch)].contents; cont; cont = cont->next_content)
				if(CAN_SEE_OBJ(ch, cont) &&
				        (cont_dotmode == FIND_ALL || isname(arg2, cont->name))) {
					if(GET_OBJ_TYPE(cont) == ITEM_CONTAINER) {
						get_from_container(ch, cont, arg1, FIND_OBJ_ROOM, amount);
						found = 1;
					} else if(cont_dotmode == FIND_ALLDOT) {
						act("$p is not a container.", FALSE, ch, cont, 0, TO_CHAR);
						found = 1;
					}
				}

			if(!found) {
				if(cont_dotmode == FIND_ALL) {
					send_to_char(ch, "You can't seem to find any containers.");
				} else {
					send_to_char(ch, "You can't seem to find any %ss here.", arg2);
				}
			}
		}
	}
}


#define VANISH(mode) ((mode == SCMD_DONATE || mode == SCMD_JUNK) ? \
		      "  It vanishes in a puff of smoke!" : "")

int perform_drop(char_data *ch, struct obj_data *obj,
                 byte mode, const char *sname, room_rnum RDR) {
	MENTOC_PREAMBLE();
	auto o = optr(obj);
	player->uncarry(o);

	char buf[MAX_STRING_LENGTH];
	int value;

	if(OBJ_FLAGGED(obj, ITEM_NODROP)) {
		snprintf(buf, sizeof(buf), "You can't %s $p, it must be CURSED!", sname);
		act(buf, FALSE, ch, obj, 0, TO_CHAR);
		return (0);
	}

	snprintf(buf, sizeof(buf), "You %s $p.%s", sname, VANISH(mode));
	act(buf, FALSE, ch, obj, 0, TO_CHAR);

	snprintf(buf, sizeof(buf), "$n %ss $p.%s", sname, VANISH(mode));
	act(buf, TRUE, ch, obj, 0, TO_ROOM);

	if((mode == SCMD_DONATE) && OBJ_FLAGGED(obj, ITEM_NODONATE)) {
		mode = SCMD_JUNK;
	}

	switch(mode) {
		case SCMD_DROP:
			obj_to_room(obj, IN_ROOM(ch));
			return (0);

		case SCMD_DONATE:
			obj_to_room(obj, RDR);
			act("$p suddenly appears in a puff a smoke!", FALSE, 0, obj, 0, TO_ROOM);
			return (0);

		case SCMD_JUNK:
			value = MAX(1, MIN(200, GET_OBJ_COST(obj) / 16));
			extract_obj(obj);
			return (value);

		default:
			log("SYSERR: Incorrect argument %d passed to perform_drop.", mode);
			break;
	}

	return (0);
}



ACMD(do_drop) {
	char arg[MAX_INPUT_LENGTH];
	struct obj_data *obj, *next_obj;
	room_rnum RDR = 0;
	byte mode = SCMD_DROP;
	int dotmode, amount = 0, multi;
	const char *sname;

	switch(subcmd) {
		case SCMD_JUNK:
			sname = "junk";
			mode = SCMD_JUNK;
			break;

		case SCMD_DONATE:
			sname = "donate";
			mode = SCMD_DONATE;

			switch(rand_number(0, 2)) {
				case 0:
					mode = SCMD_JUNK;
					break;

				case 1:
				case 2:
					RDR = real_room(donation_room_1);
					break;
					/*    case 3: RDR = real_room(donation_room_2); break;
					      case 4: RDR = real_room(donation_room_3); break;
					*/
			}

			if(RDR == NOWHERE) {
				send_to_char(ch, "Sorry, you can't donate anything right now.");
				return;
			}

			break;

		default:
			sname = "drop";
			break;
	}

	argument = one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "What do you want to %s?", sname);
		return;
	} else if(is_number(arg)) {
		multi = atoi(arg);
		one_argument(argument, arg);

		if(multi <= 0) {
			send_to_char(ch, "Yeah, that makes sense.");
		} else if(!*arg) {
			send_to_char(ch, "What do you want to %s %d of?", sname, multi);
		} else if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
			send_to_char(ch, "You don't seem to have any %ss.", arg);
		} else {
			do {
				next_obj = get_obj_in_list_vis(ch, arg, NULL, obj->next_content);
				amount += perform_drop(ch, obj, mode, sname, RDR);
				obj = next_obj;
			} while(obj && --multi);
		}
	} else {
		dotmode = find_all_dots(arg);

		/* Can't junk or donate all */
		if((dotmode == FIND_ALL) && (subcmd == SCMD_JUNK || subcmd == SCMD_DONATE)) {
			if(subcmd == SCMD_JUNK) {
				send_to_char(ch, "Go to the dump if you want to junk EVERYTHING!");
			} else {
				send_to_char(ch, "Go do the donation room if you want to donate EVERYTHING!");
			}

			return;
		}

		if(dotmode == FIND_ALL) {
			if(!ch->carrying) {
				send_to_char(ch, "You don't seem to be carrying anything.");
			} else
				for(obj = ch->carrying; obj; obj = next_obj) {
					next_obj = obj->next_content;
					amount += perform_drop(ch, obj, mode, sname, RDR);
				}
		} else if(dotmode == FIND_ALLDOT) {
			if(!*arg) {
				send_to_char(ch, "What do you want to %s all of?", sname);
				return;
			}

			if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
				send_to_char(ch, "You don't seem to have any %ss.", arg);
			}

			while(obj) {
				next_obj = get_obj_in_list_vis(ch, arg, NULL, obj->next_content);
				amount += perform_drop(ch, obj, mode, sname, RDR);
				obj = next_obj;
			}
		} else {
			if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
				send_to_char(ch, "You don't seem to have %s %s.", AN(arg), arg);
			} else {
				amount += perform_drop(ch, obj, mode, sname, RDR);
			}
		}
	}

	if(amount && (subcmd == SCMD_JUNK)) {
		send_to_char(ch, "You have been rewarded by the gods!");
		act("$n has been rewarded by the gods!", TRUE, ch, 0, 0, TO_ROOM);
		GET_GOLD(ch) += amount;
	}
}


void perform_give(char_data *ch, char_data *vict,
                  struct obj_data *obj) {
	log("DEPRECATED: perform_give ch, vict, obj_data*");
	if(OBJ_FLAGGED(obj, ITEM_NODROP)) {
		act("You can't let go of $p!!  Yeech!", FALSE, ch, obj, 0, TO_CHAR);
		return;
	}

	if(IS_CARRYING_N(vict) >= CAN_CARRY_N(vict)) {
		act("$N seems to have $S hands full.", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}

	if(GET_OBJ_WEIGHT(obj) + IS_CARRYING_W(vict) > CAN_CARRY_W(vict)) {
		act("$E can't carry that much weight.", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}

	obj_from_char(obj);
	obj_to_char(obj, vict);
	act("You give $p to $N.", FALSE, ch, obj, vict, TO_CHAR);
	act("$n gives you $p.", FALSE, ch, obj, vict, TO_VICT);
	act("$n gives $p to $N.", TRUE, ch, obj, vict, TO_NOTVICT);
	if(ch->contract) {
		mods::contract_events::perform_give(ch,vict,obj);
	}
	if(vict->contract) {
		mods::contract_events::perform_get(vict,obj);
	}
}

/* utility function for give */
char_data *give_find_vict(char_data *ch, char *arg) {
	char_data *vict;

	skip_spaces(&arg);

	if(!*arg) {
		send_to_char(ch, "To who?");
	} else if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "%s", NOPERSON);
	} else if(vict == ch) {
		send_to_char(ch, "What's the point of that?");
	} else {
		return (vict);
	}

	return (NULL);
}


ACMD(do_give) {
	char arg[MAX_STRING_LENGTH];
	int amount, dotmode;
	char_data *vict;
	struct obj_data *obj, *next_obj;

	argument = one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Give what to who?");
	} else if(is_number(arg)) {
		amount = atoi(arg);
		argument = one_argument(argument, arg);

		if(!*arg) {	/* Give multiple code. */
			send_to_char(ch, "What do you want to give %d of?", amount);
		} else if(!(vict = give_find_vict(ch, argument))) {
			return;
		} else if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
			send_to_char(ch, "You don't seem to have any %ss.", arg);
		} else {
			while(obj && amount--) {
				next_obj = get_obj_in_list_vis(ch, arg, NULL, obj->next_content);
				perform_give(ch, vict, obj);
				obj = next_obj;
			}
		}
	} else {
		char buf1[MAX_INPUT_LENGTH];

		one_argument(argument, buf1);

		if(!(vict = give_find_vict(ch, buf1))) {
			return;
		}

		dotmode = find_all_dots(arg);

		if(dotmode == FIND_INDIV) {
			if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
				send_to_char(ch, "You don't seem to have %s %s.", AN(arg), arg);
			} else {
				perform_give(ch, vict, obj);
			}
		} else {
			if(dotmode == FIND_ALLDOT && !*arg) {
				send_to_char(ch, "All of what?");
				return;
			}

			if(!ch->carrying) {
				send_to_char(ch, "You don't seem to be holding anything.");
			} else
				for(obj = ch->carrying; obj; obj = next_obj) {
					next_obj = obj->next_content;

					if(CAN_SEE_OBJ(ch, obj) &&
					        ((dotmode == FIND_ALL || isname(arg, obj->name)))) {
						perform_give(ch, vict, obj);
					}
				}
		}
	}
}



void weight_change_object(struct obj_data *obj, int weight) {
	struct obj_data *tmp_obj;
	char_data *tmp_ch;

	if(IN_ROOM(obj) != NOWHERE) {
		GET_OBJ_WEIGHT(obj) += weight;
	} else if((tmp_ch = obj->carried_by)) {
		obj_from_char(obj);
		GET_OBJ_WEIGHT(obj) += weight;
		obj_to_char(obj, tmp_ch);
	} else if((tmp_obj = obj->in_obj)) {
		obj_from_obj(obj);
		GET_OBJ_WEIGHT(obj) += weight;
		obj_to_obj(TO_OBJ_PTR(obj), TO_OBJ_PTR(tmp_obj));
	} else {
		log("SYSERR: Unknown attempt to subtract weight from an object.");
	}
}



void name_from_drinkcon(struct obj_data *obj) {
	log("[disabled]: name_from_drinkcon disabled due to possible buffer overflow");
	return;
//	char *new_name, *cur_name, *next;
//	const char *liqname;
//	int liqlen, cpylen;
//
//	if(!obj || (GET_OBJ_TYPE(obj) != ITEM_DRINKCON && GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN)) {
//		return;
//	}
//
//	liqname = drinknames[GET_OBJ_VAL(obj, 2)];
//
//	if(!isname(liqname, obj->name)) {
//		log("SYSERR: Can't remove liquid '%s' from '%s' (%d) item.", liqname, obj->name, obj->item_number);
//		return;
//	}
//
//	liqlen = strlen(liqname);
//	CREATE(new_name, char, strlen(obj->name) - strlen(liqname)); /* +1 for NUL, -1 for space */
//
//	for(cur_name = obj->name; cur_name; cur_name = next) {
//		if(*cur_name == ' ') {
//			cur_name++;
//		}
//
//		if((next = strchr(cur_name, ' '))) {
//			cpylen = next - cur_name;
//		} else {
//			cpylen = strlen(cur_name);
//		}
//
//		if(!strn_cmp(cur_name, liqname, liqlen)) {
//			continue;
//		}
//
//		if(*new_name) {
//			strcat(new_name, " ");    /* strcat: OK (size precalculated) */
//		}
//
//		strncat(new_name, cur_name, cpylen);	/* strncat: OK (size precalculated) */
//	}
//
//	if(GET_OBJ_RNUM(obj) == NOTHING || obj->name != obj_proto[GET_OBJ_RNUM(obj)].name) {
//		free(obj->name);
//	}
//
//	obj->name = new_name;
}



void name_to_drinkcon(struct obj_data *obj, int type) {
	if(!obj || (GET_OBJ_TYPE(obj) != ITEM_DRINKCON && GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN)) {
		return;
	}
	std::string new_name = obj->name.str() + " " + drinknames[type];
	obj->name.assign(new_name);
}



ACMD(do_drink) {
	char arg[MAX_INPUT_LENGTH];
	struct obj_data *temp;
	struct affected_type af;
	int amount, weight;
	int on_ground = 0;

	one_argument(argument, arg);

	if(IS_NPC(ch)) {	/* Cannot use GET_COND() on mobs. */
		return;
	}

	if(!*arg) {
		send_to_char(ch, "Drink from what?");
		return;
	}

	if(!(temp = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
		if(!(temp = get_obj_in_list_vis(ch, arg, NULL, world[IN_ROOM(ch)].contents))) {
			send_to_char(ch, "You can't find it!");
			return;
		} else {
			on_ground = 1;
		}
	}

	if((GET_OBJ_TYPE(temp) != ITEM_DRINKCON) &&
	        (GET_OBJ_TYPE(temp) != ITEM_FOUNTAIN)) {
		send_to_char(ch, "You can't drink from that!");
		return;
	}

	if(on_ground && (GET_OBJ_TYPE(temp) == ITEM_DRINKCON)) {
		send_to_char(ch, "You have to be holding that to drink from it.");
		return;
	}

	if((GET_COND(ch, DRUNK) > 10) && (GET_COND(ch, THIRST) > 0)) {
		/* The pig is drunk */
		send_to_char(ch, "You can't seem to get close enough to your mouth.");
		act("$n tries to drink but misses $s mouth!", TRUE, ch, 0, 0, TO_ROOM);
		return;
	}

	if((GET_COND(ch, FULL) > 20) && (GET_COND(ch, THIRST) > 0)) {
		send_to_char(ch, "Your stomach can't contain anymore!");
		return;
	}

	if(!GET_OBJ_VAL(temp, 1)) {
		send_to_char(ch, "It's empty.");
		return;
	}

	if(subcmd == SCMD_DRINK) {
		char buf[MAX_STRING_LENGTH];

		snprintf(buf, sizeof(buf), "$n drinks %s from $p.", drinks[GET_OBJ_VAL(temp, 2)]);
		act(buf, TRUE, ch, temp, 0, TO_ROOM);

		send_to_char(ch, "You drink the %s.", drinks[GET_OBJ_VAL(temp, 2)]);

		if(drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK] > 0) {
			amount = (25 - GET_COND(ch, THIRST)) / drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK];
		} else {
			amount = rand_number(3, 10);
		}

	} else {
		act("$n sips from $p.", TRUE, ch, temp, 0, TO_ROOM);
		send_to_char(ch, "It tastes like %s.", drinks[GET_OBJ_VAL(temp, 2)]);
		amount = 1;
	}

	amount = MIN(amount, GET_OBJ_VAL(temp, 1));

	/* You can't subtract more than the object weighs */
	weight = MIN(amount, GET_OBJ_WEIGHT(temp));

	weight_change_object(temp, -weight);	/* Subtract amount */

	gain_condition(ch, DRUNK,  drink_aff[GET_OBJ_VAL(temp, 2)][DRUNK]  * amount / 4);
	gain_condition(ch, FULL,   drink_aff[GET_OBJ_VAL(temp, 2)][FULL]   * amount / 4);
	gain_condition(ch, THIRST, drink_aff[GET_OBJ_VAL(temp, 2)][THIRST] * amount / 4);

	if(GET_COND(ch, DRUNK) > 10) {
		send_to_char(ch, "You feel drunk.");
	}

	if(GET_COND(ch, THIRST) > 20) {
		send_to_char(ch, "You don't feel thirsty any more.");
	}

	if(GET_COND(ch, FULL) > 20) {
		send_to_char(ch, "You are full.");
	}

	if(GET_OBJ_VAL(temp, 3)) {	/* The crap was poisoned ! */
		send_to_char(ch, "Oops, it tasted rather strange!");
		act("$n chokes and utters some strange sounds.", TRUE, ch, 0, 0, TO_ROOM);

		af.type = SPELL_POISON;
		af.duration = amount * 3;
		af.modifier = 0;
		af.location = APPLY_NONE;
		af.bitvector = AFF_POISON;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
	}

	/* empty the container, and no longer poison. */
	GET_OBJ_VAL(temp, 1) -= amount;

	if(!GET_OBJ_VAL(temp, 1)) {	/* The last bit */
		name_from_drinkcon(temp);
		GET_OBJ_VAL(temp, 2) = 0;
		GET_OBJ_VAL(temp, 3) = 0;
	}

	return;
}



ACMD(do_eat) {
	char arg[MAX_INPUT_LENGTH];
	struct obj_data *food;
	struct affected_type af;
	int amount;

	one_argument(argument, arg);

	if(IS_NPC(ch)) {	/* Cannot use GET_COND() on mobs. */
		return;
	}

	if(!*arg) {
		send_to_char(ch, "Eat what?");
		return;
	}

	if(!(food = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
		send_to_char(ch, "You don't seem to have %s %s.", AN(arg), arg);
		return;
	}

	if(subcmd == SCMD_TASTE && ((GET_OBJ_TYPE(food) == ITEM_DRINKCON) ||
	                            (GET_OBJ_TYPE(food) == ITEM_FOUNTAIN))) {
		do_drink(ch, argument, 0, SCMD_SIP, player);
		return;
	}

	if((GET_OBJ_TYPE(food) != ITEM_FOOD) && (GET_LEVEL(ch) < LVL_GOD)) {
		send_to_char(ch, "You can't eat THAT!");
		return;
	}

	if(GET_COND(ch, FULL) > 20) { /* Stomach full */
		send_to_char(ch, "You are too full to eat more!");
		return;
	}

	if(subcmd == SCMD_EAT) {
		act("You eat $p.", FALSE, ch, food, 0, TO_CHAR);
		act("$n eats $p.", TRUE, ch, food, 0, TO_ROOM);
	} else {
		act("You nibble a little bit of $p.", FALSE, ch, food, 0, TO_CHAR);
		act("$n tastes a little bit of $p.", TRUE, ch, food, 0, TO_ROOM);
	}

	amount = (subcmd == SCMD_EAT ? GET_OBJ_VAL(food, 0) : 1);

	gain_condition(ch, FULL, amount);

	if(GET_COND(ch, FULL) > 20) {
		send_to_char(ch, "You are full.");
	}

	if(GET_OBJ_VAL(food, 3) && (GET_LEVEL(ch) < LVL_IMMORT)) {
		/* The crap was poisoned ! */
		send_to_char(ch, "Oops, that tasted rather strange!");
		act("$n coughs and utters some strange sounds.", FALSE, ch, 0, 0, TO_ROOM);

		af.type = SPELL_POISON;
		af.duration = amount * 2;
		af.modifier = 0;
		af.location = APPLY_NONE;
		af.bitvector = AFF_POISON;
		affect_join(ch, &af, FALSE, FALSE, FALSE, FALSE);
	}

	if(subcmd == SCMD_EAT) {
		extract_obj(food);
	} else {
		if(!(--GET_OBJ_VAL(food, 0))) {
			send_to_char(ch, "There's nothing left now.");
			extract_obj(food);
		}
	}
}


ACMD(do_pour) {
	char arg1[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];
	struct obj_data *from_obj = NULL, *to_obj = NULL;
	int amount;

	two_arguments(argument, arg1, arg2);

	if(subcmd == SCMD_POUR) {
		if(!*arg1) {		/* No arguments */
			send_to_char(ch, "From what do you want to pour?");
			return;
		}

		if(!(from_obj = get_obj_in_list_vis(ch, arg1, NULL, ch->carrying))) {
			send_to_char(ch, "You can't find it!");
			return;
		}

		if(GET_OBJ_TYPE(from_obj) != ITEM_DRINKCON) {
			send_to_char(ch, "You can't pour from that!");
			return;
		}
	}

	if(subcmd == SCMD_FILL) {
		if(!*arg1) {		/* no arguments */
			send_to_char(ch, "What do you want to fill?  And what are you filling it from?");
			return;
		}

		if(!(to_obj = get_obj_in_list_vis(ch, arg1, NULL, ch->carrying))) {
			send_to_char(ch, "You can't find it!");
			return;
		}

		if(GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) {
			act("You can't fill $p!", FALSE, ch, to_obj, 0, TO_CHAR);
			return;
		}

		if(!*arg2) {		/* no 2nd argument */
			act("What do you want to fill $p from?", FALSE, ch, to_obj, 0, TO_CHAR);
			return;
		}

		if(!(from_obj = get_obj_in_list_vis(ch, arg2, NULL, world[IN_ROOM(ch)].contents))) {
			send_to_char(ch, "There doesn't seem to be %s %s here.", AN(arg2), arg2);
			return;
		}

		if(GET_OBJ_TYPE(from_obj) != ITEM_FOUNTAIN) {
			act("You can't fill something from $p.", FALSE, ch, from_obj, 0, TO_CHAR);
			return;
		}
	}

	if(GET_OBJ_VAL(from_obj, 1) == 0) {
		act("The $p is empty.", FALSE, ch, from_obj, 0, TO_CHAR);
		return;
	}

	if(subcmd == SCMD_POUR) {	/* pour */
		if(!*arg2) {
			send_to_char(ch, "Where do you want it?  Out or in what?");
			return;
		}

		if(!str_cmp(arg2, "out")) {
			act("$n empties $p.", TRUE, ch, from_obj, 0, TO_ROOM);
			act("You empty $p.", FALSE, ch, from_obj, 0, TO_CHAR);

			weight_change_object(from_obj, -GET_OBJ_VAL(from_obj, 1)); /* Empty */

			name_from_drinkcon(from_obj);
			GET_OBJ_VAL(from_obj, 1) = 0;
			GET_OBJ_VAL(from_obj, 2) = 0;
			GET_OBJ_VAL(from_obj, 3) = 0;

			return;
		}

		if(!(to_obj = get_obj_in_list_vis(ch, arg2, NULL, ch->carrying))) {
			send_to_char(ch, "You can't find it!");
			return;
		}

		if((GET_OBJ_TYPE(to_obj) != ITEM_DRINKCON) &&
		        (GET_OBJ_TYPE(to_obj) != ITEM_FOUNTAIN)) {
			send_to_char(ch, "You can't pour anything into that.");
			return;
		}
	}

	if(to_obj == from_obj) {
		send_to_char(ch, "A most unproductive effort.");
		return;
	}

	if((GET_OBJ_VAL(to_obj, 1) != 0) &&
	        (GET_OBJ_VAL(to_obj, 2) != GET_OBJ_VAL(from_obj, 2))) {
		send_to_char(ch, "There is already another liquid in it!");
		return;
	}

	if(!(GET_OBJ_VAL(to_obj, 1) < GET_OBJ_VAL(to_obj, 0))) {
		send_to_char(ch, "There is no room for more.");
		return;
	}

	if(subcmd == SCMD_POUR) {
		send_to_char(ch, "You pour the %s into the %s.", drinks[GET_OBJ_VAL(from_obj, 2)], arg2);
	}

	if(subcmd == SCMD_FILL) {
		act("You gently fill $p from $P.", FALSE, ch, to_obj, from_obj, TO_CHAR);
		act("$n gently fills $p from $P.", TRUE, ch, to_obj, from_obj, TO_ROOM);
	}

	/* New alias */
	if(GET_OBJ_VAL(to_obj, 1) == 0) {
		name_to_drinkcon(to_obj, GET_OBJ_VAL(from_obj, 2));
	}

	/* First same type liq. */
	GET_OBJ_VAL(to_obj, 2) = GET_OBJ_VAL(from_obj, 2);

	/* Then how much to pour */
	GET_OBJ_VAL(from_obj, 1) -= (amount =
	                                 (GET_OBJ_VAL(to_obj, 0) - GET_OBJ_VAL(to_obj, 1)));

	GET_OBJ_VAL(to_obj, 1) = GET_OBJ_VAL(to_obj, 0);

	if(GET_OBJ_VAL(from_obj, 1) < 0) {	/* There was too little */
		GET_OBJ_VAL(to_obj, 1) += GET_OBJ_VAL(from_obj, 1);
		amount += GET_OBJ_VAL(from_obj, 1);
		name_from_drinkcon(from_obj);
		GET_OBJ_VAL(from_obj, 1) = 0;
		GET_OBJ_VAL(from_obj, 2) = 0;
		GET_OBJ_VAL(from_obj, 3) = 0;
	}

	/* Then the poison boogie */
	GET_OBJ_VAL(to_obj, 3) =
	    (GET_OBJ_VAL(to_obj, 3) || GET_OBJ_VAL(from_obj, 3));

	/* And the weight boogie */
	weight_change_object(from_obj, -amount);
	weight_change_object(to_obj, amount);	/* Add weight */
}



void wear_message(char_data *ch, struct obj_data *obj, int where) {
	static const char *wear_messages[][2] = {
		{
			"$n lights $p and holds it.",
			"You light $p and hold it."
		},

		{
			"$n slides $p on to $s right ring finger.",
			"You slide $p on to your right ring finger."
		},

		{
			"$n slides $p on to $s left ring finger.",
			"You slide $p on to your left ring finger."
		},

		{
			"$n wears $p around $s neck.",
			"You wear $p around your neck."
		},

		{
			"$n wears $p around $s neck.",
			"You wear $p around your neck."
		},

		{
			"$n wears $p on $s body.",
			"You wear $p on your body."
		},

		{
			"$n wears $p on $s head.",
			"You wear $p on your head."
		},

		{
			"$n puts $p on $s legs.",
			"You put $p on your legs."
		},

		{
			"$n wears $p on $s feet.",
			"You wear $p on your feet."
		},

		{
			"$n puts $p on $s hands.",
			"You put $p on your hands."
		},

		{
			"$n wears $p on $s arms.",
			"You wear $p on your arms."
		},

		{
			"$n straps $p around $s arm as a shield.",
			"You start to use $p as a shield."
		},

		{
			"$n wears $p about $s body.",
			"You wear $p around your body."
		},

		{
			"$n wears $p around $s waist.",
			"You wear $p around your waist."
		},

		{
			"$n puts $p on around $s right wrist.",
			"You put $p on around your right wrist."
		},

		{
			"$n puts $p on around $s left wrist.",
			"You put $p on around your left wrist."
		},

		{
			"$n wields $p.",
			"You wield $p."
		},

		{
			"$n grabs $p.",
			"You grab $p."
		}
	};

	act(wear_messages[where][0], TRUE, ch, obj, 0, TO_ROOM);
	act(wear_messages[where][1], FALSE, ch, obj, 0, TO_CHAR);
}



bool perform_wear_with_confirmation(player_ptr_t& player, obj_ptr_t& in_obj, int where) {
	/*
	 * ITEM_WEAR_TAKE is used for objects that do not require special bits
	 * to be put into that position (e.g. you can hold any object, not just
	 * an object with a HOLD bit.)
	 */

	static constexpr int wear_bitvectors[] = {
		ITEM_WEAR_LIGHT,
		ITEM_WEAR_FINGER,
		ITEM_WEAR_FINGER,
		ITEM_WEAR_NECK,
		ITEM_WEAR_NECK,
		ITEM_WEAR_BODY,
		ITEM_WEAR_HEAD,
		ITEM_WEAR_LEGS,
		ITEM_WEAR_FEET,
		ITEM_WEAR_HANDS,
		ITEM_WEAR_ARMS,
		ITEM_WEAR_SHIELD,
		ITEM_WEAR_ABOUT,
		ITEM_WEAR_WAIST,
		ITEM_WEAR_WRIST,
		ITEM_WEAR_WRIST,
		ITEM_WEAR_WIELD,
		ITEM_WEAR_HOLD,
		ITEM_WEAR_SECONDARY,
		ITEM_WEAR_WEAPON_ATTACHMENT,
		ITEM_WEAR_SHOULDERS,
		ITEM_WEAR_SHOULDERS,
		ITEM_WEAR_BACKPACK,
		ITEM_WEAR_GOGGLES,
		ITEM_WEAR_VEST_PACK,
		ITEM_WEAR_ELBOW,
		ITEM_WEAR_ELBOW
	};

	static const std::string already_wearing[] = {
		"You're already using a light.",
		"YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.",
		"You're already wearing something on both of your ring fingers.",
		"YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.",
		"You can't wear anything else around your neck.",
		"You're already wearing something on your body.",
		"You're already wearing something on your head.",
		"You're already wearing something on your legs.",
		"You're already wearing something on your feet.",
		"You're already wearing something on your hands.",
		"You're already wearing something on your arms.",
		"You're already using a shield.",
		"You're already wearing something about your body.",
		"You already have something around your waist.",
		"YOU SHOULD NEVER SEE THIS MESSAGE.  PLEASE REPORT.",
		"You're already wearing something around both of your wrists.",
		"You're already wielding a weapon.",
		"You're already holding something.",
		"Your secondary weapon spot is already taken.",
		"You're already wearing something on your shoulders.",
		"Your vest pack is already occupied.",
		"You are already wearing something on your elbows.",
		"You are already wearing something as a backpack.",
		"You are already wearing something over your eyes."
	};
	char_data * ch = player->cd();
	obj_data * obj = in_obj.get();

	/* first, make sure that the wear position is valid. */
	if(!CAN_WEAR(obj, wear_bitvectors[where])) {
		act("You can't wear $p there.", FALSE, ch, obj, 0, TO_CHAR);
		return 0;
	}

	/* for neck, finger, and wrist, try pos 2 if pos 1 is already full */
	if((where == WEAR_FINGER_R) || (where == WEAR_NECK_1) || (where == WEAR_WRIST_R))
		if(GET_EQ(ch, where)) {
			where++;
		}

	if(GET_EQ(ch, where)) {
		player->sendln(already_wearing[where]);
		return 0;
	}
	if(obj->has_armor()) {
		auto classification = obj->armor()->attributes->classification_enum;
		if(classification == mods::yaml::armor_classification_type_t::BASIC &&
		        !mods::skills::player_can(player,"basic-armor")) {
			act("You can't wear $p until you master the {yel}BASIC_ARMOR{/yel} skill first.", FALSE, ch, obj, 0, TO_CHAR);
			return 0;
		}
		if(classification == mods::yaml::armor_classification_type_t::ADVANCED &&
		        !mods::skills::player_can(player,"advanced-armor")) {
			act("You can't wear $p until you master the {yel}ADVANCED_ARMOR{/yel} skill first.", FALSE, ch, obj, 0, TO_CHAR);
			return 0;
		}
		if(classification == mods::yaml::armor_classification_type_t::ELITE &&
		        !mods::skills::player_can(player,"elite-armor")) {
			act("You can't wear $p until you master the {yel}ELITE_ARMOR{/yel} skill first.", FALSE, ch, obj, 0, TO_CHAR);
			return 0;
		}
	}
	wear_message(ch, obj, where);
	player->equip(obj->uuid,where);
	player->uncarry(optr_by_uuid(obj->uuid));
	return 1;
}
void perform_wear(char_data *ch, struct obj_data *obj, int where) {
	MENTOC_PREAMBLE();
	auto o = optr(obj);
	perform_wear_with_confirmation(player,o,where);
}



int find_eq_pos(char_data *ch, struct obj_data *obj, char *arg) {
	int where = -1;

	const char *keywords[] = {
		"light",
		"finger",
		"!RESERVED!",
		"neck",
		"!RESERVED!",
		"body",
		"head",
		"legs",
		"feet",
		"hands",
		"arms",
		"shield",
		"about",
		"waist",
		"wrist",
		"!RESERVED!", //wrist L
		"primary",		//wield
		"hold",	//hold
		"secondary",
		"attachment",
		"shoulder",	//shoulder L
		"!RESERVED!", //shoulder R
		"backpack",
		"goggles",
		"vestpack",
		"elbow", //elbow L
		"!RESERVED!" //elbow R
		"\n"
	};

	if(!arg || !*arg) {
		if(CAN_WEAR(obj, ITEM_WEAR_FINGER)) {
			where = WEAR_FINGER_R;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_NECK)) {
			where = WEAR_NECK_1;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_BODY)) {
			where = WEAR_BODY;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_HEAD)) {
			where = WEAR_HEAD;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_LEGS)) {
			where = WEAR_LEGS;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_FEET)) {
			where = WEAR_FEET;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_HANDS)) {
			where = WEAR_HANDS;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_ARMS)) {
			where = WEAR_ARMS;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_SHIELD)) {
			where = WEAR_SHIELD;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_ABOUT)) {
			where = WEAR_ABOUT;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_WAIST)) {
			where = WEAR_WAIST;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_WRIST)) {
			where = WEAR_WRIST_R;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_SECONDARY)) {
			where = WEAR_SECONDARY_WEAPON;
		}

		if(CAN_WEAR(obj, ITEM_WEAR_SECONDARY)) {
			where = WEAR_SECONDARY_WEAPON;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_WEAPON_ATTACHMENT)) {
			where = WEAR_WEAPON_ATTACHMENT;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_SHOULDERS)) {
			where = WEAR_SHOULDERS_L;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_SHOULDERS)) {
			where = WEAR_SHOULDERS_R;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_BACKPACK)) {
			where = WEAR_BACKPACK;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_GOGGLES)) {
			where = WEAR_GOGGLES;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_VEST_PACK)) {
			where = WEAR_VEST_PACK;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_ELBOW)) {
			where = WEAR_ELBOW_L;
		}
		if(CAN_WEAR(obj, ITEM_WEAR_ELBOW)) {
			where = WEAR_ELBOW_R;
		}
	} else if((where = search_block(arg, keywords, FALSE)) < 0) {
		send_to_char(ch, "'%s'?  What part of your body is THAT?", arg);
	}

	return (where);
}



ACMD(do_wear) {

	char arg1[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	struct obj_data *obj, *next_obj;
	int where, dotmode, items_worn = 0;

	two_arguments(argument, arg1, arg2);

	if(!*arg1) {
		send_to_char(ch, "Wear what?");
		return;
	}

	dotmode = find_all_dots(arg1);

	if(*arg2 && (dotmode != FIND_INDIV)) {
		send_to_char(ch, "You can't specify the same body location for more than one item!");
		return;
	}

	if(dotmode == FIND_ALL) {
		for(obj = ch->carrying; obj; obj = next_obj) {
			next_obj = obj->next_content;

			if(CAN_SEE_OBJ(ch, obj) && (where = find_eq_pos(ch, obj, 0)) >= 0) {
				auto obj_ptr = optr(obj);
				if(perform_wear_with_confirmation(player,obj_ptr,where)) {
					items_worn++;
				}
			}
		}

		if(!items_worn) {
			send_to_char(ch, "You don't seem to have anything wearable.");
		}
	} else if(dotmode == FIND_ALLDOT) {
		if(!*arg1) {
			send_to_char(ch, "Wear all of what?");
			return;
		}

		if(!(obj = get_obj_in_list_vis(ch, arg1, NULL, ch->carrying))) {
			send_to_char(ch, "You don't seem to have any %ss.", arg1);
		} else
			while(obj) {
				next_obj = get_obj_in_list_vis(ch, arg1, NULL, obj->next_content);

				if((where = find_eq_pos(ch, obj, 0)) >= 0) {
					perform_wear(ch, obj, where);
				} else {
#ifdef __MENTOC_SHOW_WEAR_DEBUG_OUTPUT__
					act("dotmode == FIND_ALLDOT || You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
#endif
					act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
				}

				obj = next_obj;
			}
	} else {
		if(!(obj = get_obj_in_list_vis(ch, arg1, NULL, ch->carrying))) {
			send_to_char(ch, "You don't seem to have %s %s.", AN(arg1), arg1);
		} else {
			if((where = find_eq_pos(ch, obj, arg2)) >= 0) {
				perform_wear(ch, obj, where);
			} else if(!*arg2) {
#ifdef __MENTOC_SHOW_WEAR_DEBUG_OUTPUT__
				act("Final ELSE || You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
#endif
				act("You can't wear $p.", FALSE, ch, obj, 0, TO_CHAR);
			}
		}
	}

}



ACMD(do_wield) {

	char arg[MAX_INPUT_LENGTH];
	struct obj_data *obj;

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Wield what?");
	} else if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
		send_to_char(ch, "You don't seem to have %s %s.", AN(arg), arg);
	} else {
		if(!CAN_WEAR(obj, ITEM_WEAR_WIELD)) {
			send_to_char(ch, "You can't wield that.");
		} else if(GET_OBJ_WEIGHT(obj) > str_app[STRENGTH_APPLY_INDEX(ch)].wield_w) {
			send_to_char(ch, "It's too heavy for you to use.");
		} else {
			perform_wear(ch, obj, WEAR_WIELD);
		}
	}
}



ACMD(do_grab) {

	auto vec_args = PARSE_ARGS();
	struct obj_data *obj;
	auto obj_ptr = mods::util::parse_object_vec(player,vec_args);
	if(!obj_ptr) {
		player->sendln("Hold what?");
		return;
	}
	obj = obj_ptr.get();
	char* arg = vec_args[0].data();
	if(player->get_imp_mode() && player->get_misc_pref(player->mpref::HOLD_ANYTHING)) {
		perform_wear(ch, obj, WEAR_HOLD);
	} else if(!(obj = get_obj_in_list_vis(ch, arg, NULL, ch->carrying))) {
		send_to_char(ch, "You don't seem to have %s %s.", AN(arg), arg);
	} else {
		if(GET_OBJ_TYPE(obj) == ITEM_LIGHT) {
			perform_wear(ch, obj, WEAR_LIGHT);
		} else {
			if(!CAN_WEAR(obj, ITEM_WEAR_HOLD) && GET_OBJ_TYPE(obj) != ITEM_WAND &&
			        GET_OBJ_TYPE(obj) != ITEM_STAFF && GET_OBJ_TYPE(obj) != ITEM_SCROLL &&
			        GET_OBJ_TYPE(obj) != ITEM_POTION) {
				send_to_char(ch, "You can't hold that.");
			} else {
				perform_wear(ch, obj, WEAR_HOLD);
			}
		}
	}
}



int can_carry_n(player_ptr_t player) {
	return (5 + (player->aff_abils().dex >> 1) + (player->level() >> 1));
}

void perform_remove(player_ptr_t player, int pos) {
	auto equipment = player->equipment(pos);
	player->unequip(pos);
	if(!equipment) {
		log("SYSERR: perform_remove: bad pos %d passed.", pos);
		return;
	} else if(equipment->flagged(ITEM_NODROP)) {
		act("You can't remove $p, it must be CURSED!", FALSE, player->cd(), equipment.get(), 0, TO_CHAR);
	} else if(player->carry_items() >= can_carry_n(player)) {
		act("$p: you can't carry that many items!", FALSE, player->cd(), equipment.get(), 0, TO_CHAR);
	} else {
		player->carry(equipment);
		act("You stop using $p.", FALSE, player->cd(), equipment.get(), 0, TO_CHAR);
		act("$n stops using $p.", TRUE, player->cd(), equipment.get(), 0, TO_ROOM);
	}
}



ACMD(do_remove) {

	char arg[MAX_INPUT_LENGTH];
	int i, dotmode, found;

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Remove what?");
		return;
	}

	dotmode = find_all_dots(arg);

	if(dotmode == FIND_ALL) {
		found = 0;

		for(i = 0; i < NUM_WEARS; i++)
			if(GET_EQ(ch, i)) {
				perform_remove(player, i);
				found = 1;
			}

		if(!found) {
			send_to_char(ch, "You're not using anything.");
		}
	} else if(dotmode == FIND_ALLDOT) {
		if(!*arg) {
			send_to_char(ch, "Remove all of what?");
		} else {
			found = 0;

			for(i = 0; i < NUM_WEARS; i++)
				if(GET_EQ(ch, i) && CAN_SEE_OBJ(ch, GET_EQ(ch, i)) &&
				        isname(arg, GET_EQ(ch, i)->name)) {
					perform_remove(player, i);
					found = 1;
				}

			if(!found) {
				send_to_char(ch, "You don't seem to be using any %ss.", arg);
			}
		}
	} else {
		if((i = get_obj_pos_in_equip_vis(ch, arg, NULL, ch->equipment)) < 0) {
			send_to_char(ch, "You don't seem to be using %s %s.", AN(arg), arg);
		} else {
			perform_remove(player, i);
		}
	}
}
