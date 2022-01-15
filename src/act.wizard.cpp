/* ************************************************************************
*   File: act.wizard.c                                  Part of CircleMUD *
*  Usage: Player-level god commands and other goodies                     *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "globals.hpp"
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
#include "screen.h"
#include "constants.h"
#include "mods/acl/lambda.hpp"
#include "mods/orm/fetcher.hpp"
#include "mods/query-objects.hpp"


/*   external vars  */
extern FILE *player_fl;
extern struct attack_hit_type attack_hit_text[];
extern char *class_abbrevs[];
extern time_t boot_time;
extern int circle_shutdown, circle_reboot;
extern int circle_restrict;
extern int load_into_inventory;
extern int buf_switches, buf_largecount, buf_overflows;
extern int top_of_p_table;
extern char_data* character_list;

/* for chars */
extern const char *pc_class_types[];

/* extern functions */
void show_shops(char_data *ch, char *value);
void hcontrol_list_houses(char_data *ch);
void do_start(char_data *ch);
void appear(char_data *ch);
void reset_zone(zone_rnum zone);
void roll_real_abils(char_data *ch);
int parse_class(std::string arg);
void run_autowiz(void);

/* local functions */
int perform_set(char_data *ch, char_data *vict, int mode, char *val_arg);
void perform_immort_invis(char_data *ch, int level);
SUPERCMD(do_echo);
SUPERCMD(do_send);
room_rnum find_target_room(char_data *ch, char *rawroomstr);
SUPERCMD(do_at);
SUPERCMD(do_goto);
SUPERCMD(do_trans);
SUPERCMD(do_teleport);
SUPERCMD(do_shutdown);
SUPERCMD(do_vnum);
void do_stat_room(char_data *ch);
void do_stat_object(char_data *ch, struct obj_data *j);
void do_stat_character(char_data *ch, char_data *k);
ACMD(do_stat);
void stop_snooping(char_data *ch);
SUPERCMD(do_snoop);
SUPERCMD(do_switch);
SUPERCMD(do_return);
SUPERCMD(do_load);
SUPERCMD(do_vstat);
SUPERCMD(do_purge);
SUPERCMD(do_syslog);
SUPERCMD(do_advance);
SUPERCMD(do_restore);
void perform_immort_vis(char_data *ch);
SUPERCMD(do_invis);
SUPERCMD(do_gecho);
SUPERCMD(do_poofset);
SUPERCMD(do_dc);
SUPERCMD(do_wizlock);
SUPERCMD(do_date);
SUPERCMD(do_last);
SUPERCMD(do_force);
SUPERCMD(do_wiznet);
SUPERCMD(do_zreset);
SUPERCMD(do_wizutil);
size_t print_zone_to_buf(char *bufptr, size_t left, zone_rnum zone);
SUPERCMD(do_show);
SUPERCMD(do_set);
void snoop_check(char_data *ch);


SUPERCMD(do_echo) {

	skip_spaces(&argument);

	if(!*argument) {
		player->sendln("Yes.. but what?");
	} else {
		char buf[MAX_INPUT_LENGTH + 4];

		if(subcmd == SCMD_EMOTE) {
			snprintf(buf, sizeof(buf), "$n %s", argument);
		} else {
			strlcpy(buf, argument, sizeof(buf));
		}

		act(buf, FALSE, ch, 0, 0, TO_ROOM);

		if(PRF_FLAGGED(ch, PRF_NOREPEAT)) {
			player->sendln(OK);
		} else {
			act(buf, FALSE, ch, 0, 0, TO_CHAR);
		}
	}
}


SUPERCMD(do_send) {

	char arg[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
	char_data *vict;

	half_chop(argument, arg, buf);

	if(!*arg) {
		player->sendln("Send what to who?");
		return;
	}

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD))) {
		player->sendln(NOPERSON);
		return;
	}

	send_to_char(vict, "%s", buf);

	if(PRF_FLAGGED(ch, PRF_NOREPEAT)) {
		player->sendln("Sent.");
	} else {
		player->sendln(TOSTR("You send '") + buf + "' to " + GET_NAME(vict).str() + ".");
	}
}



/* take a string, and return an rnum.. used for goto, at, etc.  -je 4/6/93 */
room_rnum find_target_room(char_data *ch, char *rawroomstr) {
	MENTOC_PREAMBLE();
	room_rnum location = NOWHERE;
	char roomstr[MAX_INPUT_LENGTH];

	one_argument(rawroomstr, roomstr);

	if(!*roomstr) {
		player->sendln("You must supply a room number or name.");
		return (NOWHERE);
	}

	if(isdigit(*roomstr) && !strchr(roomstr, '.')) {
		if((location = real_room((room_vnum)atoi(roomstr))) == NOWHERE) {
			player->sendln("No room exists with that number.");
			return (NOWHERE);
		}
	} else {
		char_data *target_mob;
		struct obj_data *target_obj;
		char *mobobjstr = roomstr;
		int num;

		num = get_number(&mobobjstr);

		if((target_mob = get_char_vis(ch, mobobjstr, &num, FIND_CHAR_WORLD)) != NULL) {
			if((location = IN_ROOM(target_mob)) == NOWHERE) {
				player->sendln("That character is currently lost.");
				return (NOWHERE);
			}
		} else if((target_obj = get_obj_vis(ch, mobobjstr, &num)) != NULL) {
			if(IN_ROOM(target_obj) != NOWHERE) {
				location = IN_ROOM(target_obj);
			} else if(target_obj->carried_by && IN_ROOM(target_obj->carried_by) != NOWHERE) {
				location = IN_ROOM(target_obj->carried_by);
			} else if(target_obj->worn_by && IN_ROOM(target_obj->worn_by) != NOWHERE) {
				location = IN_ROOM(target_obj->worn_by);
			}

			if(location == NOWHERE) {
				player->sendln("That object is currently not in a room.");
				return (NOWHERE);
			}
		}

		if(location == NOWHERE) {
			player->sendln("Nothing exists by that name.");
			return (NOWHERE);
		}
	}

	/* a location has been found -- if you're >= GRGOD, no restrictions. */
	if(GET_LEVEL(ch) >= LVL_GRGOD) {
		return (location);
	}

	if(ROOM_FLAGGED(location, ROOM_GODROOM)) {
		player->sendln("You are not godly enough to use that room!");
	} else if(ROOM_FLAGGED(location, ROOM_PRIVATE)) {
		player->sendln("There's a private conversation going on in that room.");
	} else if(ROOM_FLAGGED(location, ROOM_HOUSE) && !House_can_enter(ch, GET_ROOM_VNUM(location))) {
		player->sendln("That's private property -- no trespassing!");
	} else {
		return (location);
	}

	return (NOWHERE);
}



SUPERCMD(do_at) {
	char command[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
	room_rnum location, original_loc;

	half_chop(argument, buf, command);

	if(!*buf) {
		player->sendln("You must supply a room number or a name.");
		return;
	}

	if(!*command) {
		player->sendln("What do you want to do there?");
		return;
	}

	if((location = find_target_room(ch, buf)) == NOWHERE) {
		return;
	}

	/* a location has been found. */
	original_loc = IN_ROOM(ch);
	char_from_room(ch);
	char_to_room(ch, location);
	command_interpreter(player, std::string(command));

	/* check if the char is still there */
	if(IN_ROOM(ch) == location) {
		char_from_room(ch);
		char_to_room(ch, original_loc);
	}
}


SUPERCMD(do_goto) {
	char buf[MAX_STRING_LENGTH];
	room_rnum location;

	if((location = find_target_room(ch, argument)) == NOWHERE) {
		return;
	}

	if(ch->player_specials) {
		snprintf(buf, sizeof(buf), "$n %s", ch->player_specials->poofout.length() ? ch->player_specials->poofout.c_str() : "disappears in a puff of smoke.");
	} else {
		snprintf(buf,sizeof(buf),"$n leaves the room.");
	}
	act(buf, TRUE, ch, 0, 0, TO_ROOM);

	char_from_room(ch);
	char_to_room(ch, location);

	if(ch->player_specials) {
		snprintf(buf, sizeof(buf), "$n %s", (ch)->player_specials->poofin.length() ? ch->player_specials->poofin.c_str() : "appears with an ear-splitting bang.");
	} else {
		snprintf(buf, sizeof(buf), "$n enters the room.");
	}
	act(buf, TRUE, ch, 0, 0, TO_ROOM);

	look_at_room(ch, 0);
}



SUPERCMD(do_trans) {
	char buf[MAX_INPUT_LENGTH];
	char_data *victim;

	one_argument(argument, buf);

	if(!*buf) {
		player->sendln("Whom do you wish to transfer?");
	} else if(str_cmp("all", buf)) {
		if(!(victim = get_char_vis(ch, buf, NULL, FIND_CHAR_WORLD))) {
			player->sendln(NOPERSON);
		} else if(victim == ch) {
			player->sendln("That doesn't make much sense, does it?");
		} else {
			if((GET_LEVEL(ch) < GET_LEVEL(victim)) && !IS_NPC(victim)) {
				player->sendln("Go transfer someone your own size.");
				return;
			}

			act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
			char_from_room(victim);
			char_to_room(victim, IN_ROOM(ch));
			act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
			act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
			look_at_room(victim, 0);
		}
	} else {			/* Trans All */
		if(GET_LEVEL(ch) < LVL_GRGOD) {
			player->sendln("I think not.");
			return;
		}

		for(auto& i : descriptor_list) {
			if(STATE(i) == CON_PLAYING && i.character && i.character != ch) {
				victim = i.character;

				if(GET_LEVEL(victim) >= GET_LEVEL(ch)) {
					continue;
				}

				act("$n disappears in a mushroom cloud.", FALSE, victim, 0, 0, TO_ROOM);
				char_from_room(victim);
				char_to_room(victim, IN_ROOM(ch));
				act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
				act("$n has transferred you!", FALSE, ch, 0, victim, TO_VICT);
				look_at_room(victim, 0);
			}
		}
		player->sendln(OK);
	}
}



SUPERCMD(do_teleport) {

	char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];
	char_data *victim;
	room_rnum target;

	two_arguments(argument, buf, buf2);

	if(!*buf) {
		player->sendln("Whom do you wish to teleport?");
	} else if(!(victim = get_char_vis(ch, buf, NULL, FIND_CHAR_WORLD))) {
		player->sendln(NOPERSON);
	} else if(victim == ch) {
		player->sendln("Use 'goto' to teleport yourself.");
	} else if(GET_LEVEL(victim) >= GET_LEVEL(ch)) {
		player->sendln("Maybe you shouldn't do that.");
	} else if(!*buf2) {
		player->sendln("Where do you wish to send this person?");
	} else if((target = find_target_room(ch, buf2)) != NOWHERE) {
		player->sendln(OK);
		act("$n disappears in a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
		char_from_room(victim);
		char_to_room(victim, target);
		act("$n arrives from a puff of smoke.", FALSE, victim, 0, 0, TO_ROOM);
		act("$n has teleported you!", FALSE, ch, 0, (char *) victim, TO_VICT);
		look_at_room(victim, 0);
	}
}



SUPERCMD(do_vnum) {

	char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

	half_chop(argument, buf, buf2);

	if(!*buf || !*buf2 || (!is_abbrev(buf, "mob") && !is_abbrev(buf, "obj"))) {
		player->sendln("Usage: vnum { obj | mob } <name>");
		return;
	}

	if(is_abbrev(buf, "mob"))
		if(!vnum_mobile(buf2, ch)) {
			player->sendln("No mobiles by that name.");
		}

	if(is_abbrev(buf, "obj"))
		if(!vnum_object(buf2, ch)) {
			player->sendln("No objects by that name.");
		}
}



void do_stat_room(char_data *ch) {
	MENTOC_PREAMBLE();
	char buf2[MAX_STRING_LENGTH];
	room_data& rm = world[player->room()];
	int i,  column;
	struct obj_data *j;

	player->send("Room name: %c%s%c", CCCYN(ch, C_NRM), rm.name.c_str(), CCNRM(ch, C_NRM));

	sprinttype(rm.sector_type, sector_types, buf2, sizeof(buf2));
	player->sendln(TOSTR("Zone: [") + //%3d], VNum: [%s%5d%s], RNum: [%5d], Type: %s",
	               TOSTR(zone_table[rm.zone].number) + "], VNum: [" + CCGRN(ch, C_NRM) +
	               TOSTR(rm.number) + CCNRM(ch, C_NRM) + TOSTR(IN_ROOM(ch)) + buf2 + "]");

	sprintbit(rm.room_flags, room_bits, buf2, sizeof(buf2));
	player->sendln(TOSTR("SpecProc: ") + //%s, Flags: %s", rm.func == NULL ? "None" : "Exists", buf2);
	               (rm.func == nullptr ? TOSTR("None") : TOSTR("Exists")) + ", Flags: " + buf2);

	player->sendln(TOSTR("Description:" + rm.description.str_or(" None.")));

	if(rm.ex_descriptions().size()) {
		for(auto& k : rm.ex_descriptions()) {
			player->sendln(k.keyword);
		}

		player->sendln(CCNRM(ch, C_NRM));
	}

	player->sendln(TOSTR("Chars present:") + CCYEL(ch, C_NRM));
	column = 14;	/* ^^^ strlen ^^^ */

	unsigned counter = 0;
	for(auto& k : mods::globals::get_room_list(player->room())) {
		if(!CAN_SEE(ch, k->cd())) {
			continue;
		}
		/** FIXME what is this nonsense? */
		//column += player->sendln(
		//"%s %s(%s)", found++ ? "," : "", GET_NAME(k).c_str(),
		//                  !IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB"));

		if(column >= 62) {
			player->sendln(counter + 1 == mods::globals::get_room_list(player->room()).size() ? "," : "");
			column = 0;
		}
		++counter;
	}

	player->sendln(CCNRM(ch, C_NRM));

	if(rm.contents) {
		player->sendln(TOSTR("Contents:") + CCGRN(ch, C_NRM));
		column = 9;	/* ^^^ strlen ^^^ */

		for(j = rm.contents; j; j = j->next_content) {
			if(!CAN_SEE_OBJ(ch, j)) {
				continue;
			}

			/** FIXME nonsense */
			//column += player->stc(
			//		(found++ ? TOSTR(",") : TOSTR("") + " ") + j->short_description.str()
			//		 );

			if(column >= 62) {
				player->stc(j->next_content ? "," : "");
				column = 0;
			}
		}

		player->sendln(CCNRM(ch, C_NRM));
	}

	for(i = 0; i < NUM_OF_DIRS; i++) {
		char buf1[128];

		if(!rm.dir_option[i]) {
			continue;
		}

		if(rm.dir_option[i]->to_room == NOWHERE) {
			snprintf(buf1, sizeof(buf1), " %sNONE%s", CCCYN(ch, C_NRM), CCNRM(ch, C_NRM));
		} else {
			snprintf(buf1, sizeof(buf1), "%s%5d%s", CCCYN(ch, C_NRM), GET_ROOM_VNUM(rm.dir_option[i]->to_room), CCNRM(ch, C_NRM));
		}

		sprintbit(rm.dir_option[i]->exit_info, exit_bits, buf2, sizeof(buf2));

		player->send("Exit %s%-5s%s:  To: [%s], Key: [%5d], Keywrd: %s, Type: %s%s",
		             CCCYN(ch, C_NRM), dirs[i], CCNRM(ch, C_NRM), buf1, rm.dir_option[i]->key,
		             rm.dir_option[i]->keyword.length() ? rm.dir_option[i]->keyword.c_str() : "None", buf2,
		             rm.dir_option[i]->general_description.c_str() ? rm.dir_option[i]->general_description.c_str() : "  No exit description.");
	}
}



void do_stat_object(char_data *ch, struct obj_data *j) {
	MENTOC_PREAMBLE();
	player->sendln(j->generate_stat_page());
	return;
#if 0
	int i, found;
	obj_vnum vnum;
	struct obj_data *j2;
	char buf[MAX_STRING_LENGTH];

	vnum = GET_OBJ_VNUM(j);
	player->send("Name: '%s%s%s', Aliases: %s", CCYEL(ch, C_NRM),
	             j->short_description.length() ? j->short_description.c_str() : "<None>",
	             CCNRM(ch, C_NRM), j->name.c_str());

	sprinttype(GET_OBJ_TYPE(j), item_types, buf, sizeof(buf));
	player->send("VNum: [%s%5d%s], RNum: [%5d], Type: %s, SpecProc: %s",
	             CCGRN(ch, C_NRM), vnum, CCNRM(ch, C_NRM), GET_OBJ_RNUM(j), buf,
	             GET_OBJ_SPEC(j) ? "Exists" : "None");

	if(j->ex_description.size()) {
		player->send("Extra descs:%s", CCCYN(ch, C_NRM));

		for(auto& desc : j->ex_description) {
			player->send(" %s", desc.keyword);
		}

		player->sendln(CCNRM(ch, C_NRM));
	}

	sprintbit(GET_OBJ_WEAR(j), wear_bits, buf, sizeof(buf));
	player->send("Can be worn on: %s", buf);

	sprintbit(GET_OBJ_AFFECT(j), affected_bits, buf, sizeof(buf));
	player->send("Set char bits : %s", buf);

	sprintbit(GET_OBJ_EXTRA(j), extra_bits, buf, sizeof(buf));
	player->send("Extra flags   : %s", buf);

	player->send("Weight: %d, Value: %d, Cost/day: %d, Timer: %d",
	             GET_OBJ_WEIGHT(j), GET_OBJ_COST(j), GET_OBJ_RENT(j), GET_OBJ_TIMER(j));

	player->send("In room: %d (%s), ", GET_ROOM_VNUM(IN_ROOM(j)),
	             IN_ROOM(j) == NOWHERE ? "Nowhere" : world[IN_ROOM(j)].name.c_str());

	/*
	 * NOTE: In order to make it this far, we must already be able to see the
	 *       character holding the object. Therefore, we do not need CAN_SEE().
	 */
	player->send("In object: %s, ", j->in_obj ? j->in_obj->short_description : "None");
	player->send("Carried by: %s, ", j->carried_by ? GET_NAME(j->carried_by).c_str() : "Nobody");
	player->send("Worn by: %s", j->worn_by ? GET_NAME(j->worn_by).c_str() : "Nobody");

	switch(GET_OBJ_TYPE(j)) {
		case ITEM_LIGHT:
			if(GET_OBJ_VAL(j, 2) == -1) {
				player->sendln("Hours left: Infinite");
			} else {
				player->send("Hours left: [%d]", GET_OBJ_VAL(j, 2));
			}

			break;

		case ITEM_SCROLL:
		case ITEM_POTION:
			player->send("Spells: (Level %d) %s, %s, %s", GET_OBJ_VAL(j, 0),
			             skill_name(GET_OBJ_VAL(j, 1)), skill_name(GET_OBJ_VAL(j, 2)),
			             skill_name(GET_OBJ_VAL(j, 3)));
			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			player->send("Spell: %s at level %d, %d (of %d) charges remaining",
			             skill_name(GET_OBJ_VAL(j, 3)), GET_OBJ_VAL(j, 0),
			             GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 1));
			break;

		case ITEM_WEAPON:
			player->send("Todam: %dd%d, Message type: %d",
			             GET_OBJ_VAL(j, 1), GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 3));
			break;

		case ITEM_ARMOR:
			player->send("AC-apply: [%d]", GET_OBJ_VAL(j, 0));
			break;

		case ITEM_TRAP:
			player->send("Spell: %d, - Hitpoints: %d", GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1));
			break;

		case ITEM_CONTAINER_LEGACY:
			sprintbit(GET_OBJ_VAL(j, 1), container_bits, buf, sizeof(buf));
			player->send("Weight capacity: %d, Lock Type: %s, Key Num: %d, Corpse: %s",
			             GET_OBJ_VAL(j, 0), buf, GET_OBJ_VAL(j, 2),
			             YESNO(GET_OBJ_VAL(j, 3)));
			break;

		case ITEM_DRINKCON:
		case ITEM_FOUNTAIN:
			sprinttype(GET_OBJ_VAL(j, 2), drinks, buf, sizeof(buf));
			player->send("Capacity: %d, Contains: %d, Poisoned: %s, Liquid: %s",
			             GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1), YESNO(GET_OBJ_VAL(j, 3)), buf);
			break;

		case ITEM_NOTE:
			player->send("Tongue: %d", GET_OBJ_VAL(j, 0));
			break;

		case ITEM_KEY:
			/* Nothing */
			break;

		case ITEM_FOOD:
			player->send("Makes full: %d, Poisoned: %s", GET_OBJ_VAL(j, 0), YESNO(GET_OBJ_VAL(j, 3)));
			break;

		case ITEM_MONEY:
			player->send("Coins: %d", GET_OBJ_VAL(j, 0));
			break;

		default:
			player->send("Values 0-3: [%d] [%d] [%d] [%d]",
			             GET_OBJ_VAL(j, 0), GET_OBJ_VAL(j, 1),
			             GET_OBJ_VAL(j, 2), GET_OBJ_VAL(j, 3));
			break;
	}

	/*
	 * I deleted the "equipment status" code from here because it seemed
	 * more or less useless and just takes up valuable screen space.
	 */

	if(j->contains) {
		int column;

		player->send("Contents:%s", CCGRN(ch, C_NRM));
		column = 9;	/* ^^^ strlen ^^^ */

		for(found = 0, j2 = j->contains; j2; j2 = j2->next_content) {
			column += player->send("%s %s", found++ ? "," : "", j2->short_description);

			if(column >= 62) {
				player->sendln(j2->next_content ? "," : "");
				found = FALSE;
				column = 0;
			}
		}

		player->sendln(CCNRM(ch, C_NRM));
	}

	found = FALSE;
	player->sendln("Affections:");

	for(i = 0; i < MAX_OBJ_AFFECT; i++)
		if(j->affected[i].modifier) {
			sprinttype(j->affected[i].location, apply_types, buf, sizeof(buf));
			player->send("%s %+d to %s", found++ ? "," : "", j->affected[i].modifier, buf);
		}

	if(!found) {
		player->sendln(" None");
	}

	player->sendln("");
#endif
}


void do_stat_character(char_data *ch, char_data *k) {
	MENTOC_PREAMBLE();
	char buf[MAX_STRING_LENGTH];
	int i, i2, column, found = FALSE;
	struct obj_data *j;
	struct follow_type *fol;
	struct affected_type *aff;

	sprinttype(GET_SEX(k), genders, buf, sizeof(buf));
	player->send("%s %s '%s'  IDNum: [%5ld], In room [%5d]",
	             buf, (!IS_NPC(k) ? "PC" : (!IS_MOB(k) ? "NPC" : "MOB")),
	             GET_NAME(k).c_str(), GET_IDNUM(k), GET_ROOM_VNUM(IN_ROOM(k)));

	if(IS_MOB(k)) {
		player->send("Alias: %s, VNum: [%5d], RNum: [%5d]", k->player.name.c_str(), GET_MOB_VNUM(k), GET_MOB_RNUM(k));
	}

	player->send("Title: %s", k->player.title.length() ? k->player.title.c_str() : "<None>");

	player->send("L-Des: %s", k->player.long_descr.length() ? k->player.long_descr.c_str() : "<None>");

	sprinttype(k->player.chclass, IS_NPC(k) ? npc_class_types : pc_class_types, buf, sizeof(buf));
	player->send("%sClass: %s, Lev: [%s%2d%s], XP: [%s%7d%s], Align: [%4d]",
	             IS_NPC(k) ? "Monster " : "", buf, CCYEL(ch, C_NRM), GET_LEVEL(k), CCNRM(ch, C_NRM),
	             CCYEL(ch, C_NRM), GET_EXP(k), CCNRM(ch, C_NRM), GET_ALIGNMENT(k));

	if(!IS_NPC(k)) {
		char buf1[64], buf2[64];

		strlcpy(buf1, asctime(localtime(&(k->player.time.birth))), sizeof(buf1));
		strlcpy(buf2, asctime(localtime(&(k->player.time.logon))), sizeof(buf2));
		buf1[10] = buf2[10] = '\0';

		player->send("Created: [%s], Last Logon: [%s], Played [%dh %dm], Age [%d]",
		             buf1, buf2, k->player.time.played / 3600,
		             ((k->player.time.played % 3600) / 60), age(k)->year);

		player->send("Hometown: [%d], Speaks: [%d/%d/%d], (STL[%d]/per[%d]/NSTL[%d])",
		             k->player.hometown, GET_TALK(k, 0), GET_TALK(k, 1), GET_TALK(k, 2),
		             0, int_app[GET_INT(k)].learn,
		             wis_app[GET_WIS(k)].bonus);
	}

	player->send("Str: [%s%d/%d%s]  Int: [%s%d%s]  Wis: [%s%d%s]  "
	             "Dex: [%s%d%s]  Con: [%s%d%s]  Cha: [%s%d%s]",
	             CCCYN(ch, C_NRM), GET_STR(k), GET_ADD(k), CCNRM(ch, C_NRM),
	             CCCYN(ch, C_NRM), GET_INT(k), CCNRM(ch, C_NRM),
	             CCCYN(ch, C_NRM), GET_WIS(k), CCNRM(ch, C_NRM),
	             CCCYN(ch, C_NRM), GET_DEX(k), CCNRM(ch, C_NRM),
	             CCCYN(ch, C_NRM), GET_CON(k), CCNRM(ch, C_NRM),
	             CCCYN(ch, C_NRM), GET_CHA(k), CCNRM(ch, C_NRM));

	player->send("Hit p.:[%s%d/%d+%d%s]  Mana p.:[%s%d/%d+%d%s]  Move p.:[%s%d/%d+%d%s]",
	             CCGRN(ch, C_NRM), GET_HIT(k), GET_MAX_HIT(k), hit_gain(k), CCNRM(ch, C_NRM),
	             CCGRN(ch, C_NRM), GET_MANA(k), GET_MAX_MANA(k), mana_gain(k), CCNRM(ch, C_NRM),
	             CCGRN(ch, C_NRM), GET_MOVE(k), GET_MAX_MOVE(k), move_gain(k), CCNRM(ch, C_NRM));

	player->send("Coins: [%9d], Bank: [%9d] (Total: %d)",
	             GET_GOLD(k), GET_BANK_GOLD(k), GET_GOLD(k) + GET_BANK_GOLD(k));

	player->send("AC: [%d%+d/10], Hitroll: [%2d], Damroll: [%2d], Saving throws: [%d/%d/%d/%d/%d]",
	             GET_AC(k), dex_app[GET_DEX(k)].defensive, k->points.hitroll,
	             k->points.damroll, GET_SAVE(k, 0), GET_SAVE(k, 1), GET_SAVE(k, 2),
	             GET_SAVE(k, 3), GET_SAVE(k, 4));

	sprinttype(GET_POS(k), position_types, buf, sizeof(buf));
	player->send("Pos: %s, Fighting: %s", buf, FIGHTING(k) ? GET_NAME(FIGHTING(k)).c_str() : "Nobody");

	if(IS_NPC(k)) {
		player->send(", Attack type: %s", attack_hit_text[(int) k->mob_specials.attack_type].singular);
	}

	if(k->has_desc) {
		sprinttype(STATE(k->desc), connected_types, buf, sizeof(buf));
		player->send(", Connected: %s", buf);
	}

	if(IS_NPC(k)) {
		sprinttype(k->mob_specials.default_pos, position_types, buf, sizeof(buf));
		player->send(", Default position: %s", buf);
		sprintbit(MOB_FLAGS(k), action_bits, buf, sizeof(buf));
		player->send("NPC flags: %s%s%s", CCCYN(ch, C_NRM), buf, CCNRM(ch, C_NRM));
	} else {
		player->send(", Idle Timer (in tics) [%d]", k->char_specials.timer);

		sprintbit(PLR_FLAGS(k), player_bits, buf, sizeof(buf));
		player->send("PLR: %s%s%s", CCCYN(ch, C_NRM), buf, CCNRM(ch, C_NRM));

		sprintbit(PRF_FLAGS(k), preference_bits, buf, sizeof(buf));
		player->send("PRF: %s%s%s", CCGRN(ch, C_NRM), buf, CCNRM(ch, C_NRM));
	}

	if(IS_MOB(k))
		player->send("Mob Spec-Proc: %s, NPC Bare Hand Dam: %dd%d",
		             (mob_index[GET_MOB_RNUM(k)].func ? "Exists" : "None"),
		             k->mob_specials.damnodice, k->mob_specials.damsizedice);

	for(i = 0, j = k->carrying; j; j = j->next_content, i++);

	player->send("Carried: weight: %d, items: %d; Items in: inventory: %d, ", IS_CARRYING_W(k), IS_CARRYING_N(k), i);

	for(i = 0, i2 = 0; i < NUM_WEARS; i++)
		if(GET_EQ(k, i)) {
			i2++;
		}

	player->send("eq: %d", i2);

	if(!IS_NPC(k)) {
		player->send("Hunger: %d, Thirst: %d, Drunk: %d", GET_COND(k, FULL), GET_COND(k, THIRST), GET_COND(k, DRUNK));
	}

	column = player->send("Master is: %s, Followers are:", k->master ? GET_NAME(k->master).c_str() : "<none>");

	if(!k->followers) {
		player->sendln(" <none>");
	} else {
		for(fol = k->followers; fol; fol = fol->next) {
			column += player->send("%s %s", found++ ? "," : "", std::string(PERS(fol->follower, ch)).c_str());

			if(column >= 62) {
				player->sendln(fol->next ? "," : "");
				found = FALSE;
				column = 0;
			}
		}

		if(column != 0) {
			player->sendln("");
		}
	}

	/* Showing the bitvector */
	sprintbit(AFF_FLAGS(k), affected_bits, buf, sizeof(buf));
	player->send("AFF: %s%s%s", CCYEL(ch, C_NRM), buf, CCNRM(ch, C_NRM));

	/* Routine to show what spells a char is affected by */
	if(k->affected) {
		for(aff = k->affected; aff; aff = aff->next) {
			player->send("SPL: (%3dhr) %s%-21s%s ", aff->duration + 1, CCCYN(ch, C_NRM), skill_name(aff->type), CCNRM(ch, C_NRM));

			if(aff->modifier) {
				player->send("%+d to %s", aff->modifier, apply_types[(int) aff->location]);
			}

			if(aff->bitvector) {
				if(aff->modifier) {
					player->sendln(", ");
				}

				sprintbit(aff->bitvector, affected_bits, buf, sizeof(buf));
				player->send("sets %s", buf);
			}

			player->sendln("");
		}
	}
}


ACMD(do_stat) {
	DO_HELP("stat,stats");
	static constexpr const char* usage = "usage: stat <object|weapon>";
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		player->errorln(usage);
		return;
	}

	{
		auto obj= mods::query_objects::query_room_for_object(player->room(), vec_args[0]);
		if(obj) {
			do_stat_object(ch,obj.get());
			return;
		}
	}
	{
		auto obj_list = mods::query_objects::query_inventory_for_object(player,vec_args[0]);
		if(obj_list.size()) {
			for(auto& uuid : obj_list) {
				auto obj = optr_by_uuid(uuid);
				if(!obj) {
					continue;
				}
				do_stat_object(ch,obj.get());
			}
			return;
		}
	}
}


SUPERCMD(do_shutdown) {

	char arg[MAX_INPUT_LENGTH];

	if(subcmd != SCMD_SHUTDOWN) {
		player->sendln("If you want to shut something down, say so!");
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		log("(GC) Shutdown by %s.", GET_NAME(ch).c_str());
		send_to_all("Shutting down.");
		circle_shutdown = 1;
	} else if(!str_cmp(arg, "reboot")) {
		log("(GC) Reboot by %s.", GET_NAME(ch).c_str());
		send_to_all("Rebooting.. come back in a minute or two.");
		touch(FASTBOOT_FILE);
		circle_shutdown = circle_reboot = 1;
	} else if(!str_cmp(arg, "die")) {
		log("(GC) Shutdown by %s.", GET_NAME(ch).c_str());
		send_to_all("Shutting down for maintenance.");
		touch(KILLSCRIPT_FILE);
		circle_shutdown = 1;
	} else if(!str_cmp(arg, "pause")) {
		log("(GC) Shutdown by %s.", GET_NAME(ch).c_str());
		send_to_all("Shutting down for maintenance.");
		touch(PAUSE_FILE);
		circle_shutdown = 1;
	} else {
		player->sendln("Unknown shutdown option.");
	}
}


void snoop_check(char_data *ch) {
	/*  This short routine is to ensure that characters that happen
	 *  to be snooping (or snooped) and get advanced/demoted will
	 *  not be snooping/snooped someone of a higher/lower level (and
	 *  thus, not entitled to be snooping.
	 */
	if(!ch || !ch->has_desc) {
		return;
	}

	if(ch->desc->snooping &&
	        (GET_LEVEL(ch->desc->snooping->character) >= GET_LEVEL(ch))) {
		ch->desc->snooping->snoop_by = NULL;
		ch->desc->snooping = NULL;
	}

	if(ch->desc->snoop_by &&
	        (GET_LEVEL(ch) >= GET_LEVEL(ch->desc->snoop_by->character))) {
		ch->desc->snoop_by->snooping = NULL;
		ch->desc->snoop_by = NULL;
	}
}

void stop_snooping(char_data *ch) {
	MENTOC_PREAMBLE();
	if(!ch->desc->snooping) {
		player->sendln("You aren't snooping anyone.");
	} else {
		player->sendln("You stop snooping.");
		ch->desc->snooping->snoop_by = NULL;
		ch->desc->snooping = NULL;
	}
}


SUPERCMD(do_snoop) {

	char arg[MAX_INPUT_LENGTH];
	char_data *victim, *tch;

	if(!ch->has_desc) {
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		stop_snooping(ch);
	} else if(!(victim = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD))) {
		player->sendln("No such person around.");
	} else if(!victim->has_desc) {
		player->sendln("There's no link.. nothing to snoop.");
	} else if(victim == ch) {
		stop_snooping(ch);
	} else if(victim->desc->snoop_by) {
		player->sendln("Busy already. ");
	} else if(*victim->desc->snooping == *ch->desc) {
		player->sendln("Don't be stupid.");
	} else {
		if(victim->desc->original) {
			tch = victim->desc->original;
		} else {
			tch = victim;
		}

		if(GET_LEVEL(tch) >= GET_LEVEL(ch)) {
			player->sendln("You can't.");
			return;
		}

		player->sendln(OK);

		if(ch->desc->snooping) {
			ch->desc->snooping->snoop_by = NULL;
		}

		*ch->desc->snooping = *victim->desc;
		*victim->desc->snoop_by = *ch->desc;
	}
}



SUPERCMD(do_switch) {

	char arg[MAX_INPUT_LENGTH];
	char_data *victim;

	one_argument(argument, arg);

	if(ch->desc->original) {
		player->sendln("You're already switched.");
	} else if(!*arg) {
		player->sendln("Switch with who?");
	} else if(!(victim = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD))) {
		player->sendln("No such character.");
	} else if(ch == victim) {
		player->sendln("Hee hee... we are jolly funny today, eh?");
	} else if(victim->has_desc) {
		player->sendln("You can't do that, the body is already in use!");
	} else if((GET_LEVEL(ch) < LVL_IMPL) && !IS_NPC(victim)) {
		player->sendln("You aren't holy enough to use a mortal's body.");
	} else if(GET_LEVEL(ch) < LVL_GRGOD && ROOM_FLAGGED(IN_ROOM(victim), ROOM_GODROOM)) {
		player->sendln("You are not godly enough to use that room!");
	} else if(GET_LEVEL(ch) < LVL_GRGOD && ROOM_FLAGGED(IN_ROOM(victim), ROOM_HOUSE)
	          && !House_can_enter(ch, GET_ROOM_VNUM(IN_ROOM(victim)))) {
		player->sendln("That's private property -- no trespassing!");
	} else {
		player->sendln(OK);

		ch->desc->character = victim;
		ch->desc->original = ch;

		victim->desc = ch->desc;
		/** FIXME: descriptor_list.erase()? */
		ch->has_desc = false;
	}
}


SUPERCMD(do_return) {

	if(ch->has_desc && ch->desc->original) {
		player->sendln("You return to your original body.");

		/*
		 * If someone switched into your original body, disconnect them.
		 *   - JE 2/22/95
		 *
		 * Zmey: here we put someone switched in our body to disconnect state
		 * but we must also NULL his pointer to our character, otherwise
		 * close_socket() will damage our character's pointer to our descriptor
		 * (which is assigned below in this function). 12/17/99
		 */
		if(ch->desc->original->has_desc) {
			ch->desc->original->desc->character = NULL;
			STATE(ch->desc->original->desc) = CON_DISCONNECT;
		}

		/* Now our descriptor points to our original body. */
		ch->desc->character = ch->desc->original;
		ch->desc->original = NULL;

		/* And our body's pointer to descriptor now points to our descriptor. */
		ch->desc->character->desc = ch->desc;
		ch->has_desc = false;
	}
}



SUPERCMD(do_load) {

	char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

	two_arguments(argument, buf, buf2);

	if(!*buf || !*buf2 || !isdigit(*buf2)) {
		player->sendln("Usage: load { obj | mob } <number>");
		return;
	}

	if(!is_number(buf2)) {
		player->sendln("That is not a number.");
		return;
	}

	if(is_abbrev(buf, "mob")) {
		char_data *mob;
		mob_rnum r_num;

		if((r_num = real_mobile(atoi(buf2))) == NOBODY) {
			player->sendln("There is no monster with that number.");
			return;
		}

		mob = read_mobile(r_num, REAL);
		char_to_room(mob, IN_ROOM(ch));

		act("$n makes a quaint, magical gesture with one hand.", TRUE, ch,
		    0, 0, TO_ROOM);
		act("$n has created $N!", FALSE, ch, 0, mob, TO_ROOM);
		act("You create $N.", FALSE, ch, 0, mob, TO_CHAR);
	} else if(is_abbrev(buf, "obj")) {
		struct obj_data *obj;
		obj_rnum r_num;

		if((r_num = real_object(atoi(buf2))) == NOTHING) {
			player->sendln("There is no object with that number.");
			return;
		}

		obj = read_object(r_num, REAL);

		if(load_into_inventory) {
			obj_to_char(obj, ch);
		} else {
			obj_to_room(obj, IN_ROOM(ch));
		}

		act("$n makes a strange magical gesture.", TRUE, ch, 0, 0, TO_ROOM);
		act("$n has created $p!", FALSE, ch, obj, 0, TO_ROOM);
		act("You create $p.", FALSE, ch, obj, 0, TO_CHAR);
	} else {
		player->sendln("That'll have to be either 'obj' or 'mob'.");
	}
}



SUPERCMD(do_vstat) {

	char buf[MAX_INPUT_LENGTH], buf2[MAX_INPUT_LENGTH];

	two_arguments(argument, buf, buf2);

	if(!*buf || !*buf2 || !isdigit(*buf2)) {
		player->sendln("Usage: vstat { obj | mob } <number>");
		return;
	}

	if(!is_number(buf2)) {
		player->sendln("That's not a valid number.");
		return;
	}

	if(is_abbrev(buf, "mob")) {
		char_data *mob;
		mob_rnum r_num;

		if((r_num = real_mobile(atoi(buf2))) == NOBODY) {
			player->sendln("There is no monster with that number.");
			return;
		}

		mob = read_mobile(r_num, REAL);
		char_to_room(mob, 0);
		do_stat_character(ch, mob);
		extract_char(mob);
	} else if(is_abbrev(buf, "obj")) {
		struct obj_data *obj;
		obj_rnum r_num;

		if((r_num = real_object(atoi(buf2))) == NOTHING) {
			player->sendln("There is no object with that number.");
			return;
		}

		obj = read_object(r_num, REAL);
		do_stat_object(ch, obj);
		extract_obj(obj);
	} else {
		player->sendln("That'll have to be either 'obj' or 'mob'.");
	}
}




/* clean a room of all mobiles and objects */
SUPERCMD(do_purge) {

	char buf[MAX_INPUT_LENGTH];
	char_data *vict;
	struct obj_data *obj;

	one_argument(argument, buf);

	/* argument supplied. destroy single object or char */
	if(*buf) {
		if((vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM)) != NULL) {
			if(!IS_NPC(vict) && (GET_LEVEL(ch) <= GET_LEVEL(vict))) {
				player->sendln("Fuuuuuuuuu!");
				return;
			}

			act("$n disintegrates $N.", FALSE, ch, 0, vict, TO_NOTVICT);

			if(!IS_NPC(vict)) {
				mudlog(BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s has purged %s.", GET_NAME(ch).c_str(), GET_NAME(vict).c_str());

				if(vict->has_desc) {
					vict->desc->set_state(CON_CLOSE);
					vict->desc->character = NULL;
					vict->has_desc = false;
				}
			}

			extract_char(vict);
		} else if((obj = get_obj_in_list_vis(ch, buf, NULL, world[IN_ROOM(ch)].contents)) != NULL) {
			act("$n destroys $p.", FALSE, ch, obj, 0, TO_ROOM);
			extract_obj(obj);
		} else {
			player->sendln("Nothing here by that name.");
			return;
		}

		player->sendln(OK);
	} else {			/* no argument. clean out the room */
		int i;

		act("$n gestures... You are surrounded by scorching flames!",
		    FALSE, ch, 0, 0, TO_ROOM);
		send_to_room(IN_ROOM(ch), "The world seems a little cleaner.");

		//for(vict = world[IN_ROOM(ch)].people; vict; vict = vict->next_in_room) {
		for(auto& vict : mods::globals::get_room_list(player)) {
			if(!IS_NPC(vict->cd())) {
				continue;
			}

			/* Dump inventory. */
			while(vict->carrying()) {
				extract_obj(vict->carrying());
			}

			/* Dump equipment. */
			for(i = 0; i < NUM_WEARS; i++)
				if(GET_EQ(vict->cd(), i)) {
					extract_obj(GET_EQ(vict->cd(), i));
				}

			/* Dump character. */
			extract_char(vict->cd());
		}

		/* Clear the ground. */
		while(world[IN_ROOM(ch)].contents) {
			extract_obj(world[IN_ROOM(ch)].contents);
		}
	}
}



const char *logtypes[] = {
	"off", "brief", "normal", "complete", "\n"
};

SUPERCMD(do_syslog) {

	char arg[MAX_INPUT_LENGTH];
	int tp;

	one_argument(argument, arg);

	if(!*arg) {
		player->send("Your syslog is currently %s.",
		             logtypes[(PRF_FLAGGED(ch, PRF_LOG1) ? 1 : 0) + (PRF_FLAGGED(ch, PRF_LOG2) ? 2 : 0)]);
		return;
	}

	if((tp = search_block(arg, logtypes, FALSE)) == -1) {
		player->sendln("Usage: syslog { Off | Brief | Normal | Complete }");
		return;
	}

	REMOVE_BIT(PRF_FLAGS(ch), PRF_LOG1 | PRF_LOG2);
	SET_BIT(PRF_FLAGS(ch), (PRF_LOG1 * (tp & 1)) | (PRF_LOG2 * (tp & 2) >> 1));

	player->send("Your syslog is now %s.", logtypes[tp]);
}



SUPERCMD(do_advance) {

	char_data *victim;
	char name[MAX_INPUT_LENGTH], level[MAX_INPUT_LENGTH];
	int newlevel, oldlevel;

	two_arguments(argument, name, level);

	if(*name) {
		if(!(victim = get_char_vis(ch, name, NULL, FIND_CHAR_WORLD))) {
			player->sendln("That player is not here.");
			return;
		}
	} else {
		player->sendln("Advance who?");
		return;
	}

	if(GET_LEVEL(ch) <= GET_LEVEL(victim)) {
		player->sendln("Maybe that's not such a great idea.");
		return;
	}

	if(IS_NPC(victim)) {
		player->sendln("NO!  Not on NPC's.");
		return;
	}

	if(!*level || (newlevel = atoi(level)) <= 0) {
		player->sendln("That's not a level!");
		return;
	}

	if(newlevel > LVL_IMPL) {
		player->send("%d is the highest possible level.", LVL_IMPL);
		return;
	}

	if(newlevel > GET_LEVEL(ch)) {
		player->sendln("Yeah, right.");
		return;
	}

	if(newlevel == GET_LEVEL(victim)) {
		player->sendln("They are already at that level.");
		return;
	}

	oldlevel = GET_LEVEL(victim);

	if(newlevel < GET_LEVEL(victim)) {
		do_start(victim);
		GET_LEVEL(victim) = newlevel;
		send_to_char(victim, "You are momentarily enveloped by darkness!You feel somewhat diminished.");
	} else {
		act("$n makes some strange gestures."
		    "A strange feeling comes upon you,"
		    "Like a giant hand, light comes down"
		    "from above, grabbing your body, that"
		    "begins to pulse with colored lights"
		    "from inside."
		    "Your head seems to be filled with demons"
		    "from another plane as your body dissolves"
		    "to the elements of time and space itself."
		    "Suddenly a silent explosion of light"
		    "snaps you back to reality."
		    "You feel slightly different.", FALSE, ch, 0, victim, TO_VICT);
	}

	player->sendln(OK);

	if(newlevel < oldlevel)
		log("(GC) %s demoted %s from level %d to %d.",
		    GET_NAME(ch).c_str(), GET_NAME(victim).c_str(), oldlevel, newlevel);
	else
		log("(GC) %s has advanced %s to level %d (from %d)",
		    GET_NAME(ch).c_str(), GET_NAME(victim).c_str(), newlevel, oldlevel);

	if(oldlevel >= LVL_IMMORT && newlevel < LVL_IMMORT) {
		/* If they are no longer an immortal, let's remove some of the
		 * nice immortal only flags, shall we?
		 */
		REMOVE_BIT(PRF_FLAGS(victim), PRF_LOG1 | PRF_LOG2);
		REMOVE_BIT(PRF_FLAGS(victim), PRF_NOHASSLE | PRF_HOLYLIGHT);
		run_autowiz();
	}

#if 0
	mods::levels::gain_exp_regardless(victim, level_exp(GET_CLASS(victim), newlevel) - GET_EXP(victim));
#endif
	player->sendln("[stub]: need to implement gain_exp_regardless");
	mods::db::save_char(std::make_shared<mods::player>(victim));
}

SUPERCMD(do_restore) {

	char buf[MAX_INPUT_LENGTH];
	char_data *vict;
	int i;

	one_argument(argument, buf);

	if(!*buf) {
		player->sendln("Whom do you wish to restore?");
	} else if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_WORLD))) {
		player->sendln(NOPERSON);
	} else if(!IS_NPC(vict) && ch != vict && GET_LEVEL(vict) >= GET_LEVEL(ch)) {
		player->sendln("They don't need your help.");
	} else {
		GET_HIT(vict) = GET_MAX_HIT(vict);
		GET_MANA(vict) = GET_MAX_MANA(vict);
		GET_MOVE(vict) = GET_MAX_MOVE(vict);

		if(!IS_NPC(vict) && GET_LEVEL(ch) >= LVL_GRGOD) {
			if(GET_LEVEL(vict) >= LVL_IMMORT)
				for(i = 1; i <= MAX_SKILLS; i++) {
					SET_SKILL(vict, i, 100);
				}

			if(GET_LEVEL(vict) >= LVL_GRGOD) {
				vict->real_abils.str_add = 100;
				vict->real_abils.intel = 25;
				vict->real_abils.wis = 25;
				vict->real_abils.dex = 25;
				vict->real_abils.str = 25;
				vict->real_abils.con = 25;
				vict->real_abils.cha = 25;
			}
		}

		update_pos(vict);
		affect_total(vict);
		player->sendln(OK);
		act("You have been fully healed by $N!", FALSE, vict, 0, ch, TO_CHAR);
	}
}


void perform_immort_vis(char_data *ch) {
	MENTOC_PREAMBLE();
	if(GET_INVIS_LEV(ch) == 0 && !AFF_FLAGGED(ch, AFF_HIDE | AFF_INVISIBLE)) {
		player->sendln("You are already fully visible.");
		return;
	}

	GET_INVIS_LEV(ch) = 0;
	appear(ch);
	player->sendln("You are now fully visible.");
}


void perform_immort_invis(char_data *ch, int level) {
	MENTOC_PREAMBLE();

	//for(tch = world[IN_ROOM(ch)].people; tch; tch = tch->next_in_room) {
	for(auto& plr : mods::globals::get_room_list(player)) {
		auto tch = plr->cd();
		if(tch == ch) {
			continue;
		}

		if(GET_LEVEL(tch) >= GET_INVIS_LEV(ch) && GET_LEVEL(tch) < level)
			act("You blink and suddenly realize that $n is gone.", FALSE, ch, 0,
			    tch, TO_VICT);

		if(GET_LEVEL(tch) < GET_INVIS_LEV(ch) && GET_LEVEL(tch) >= level)
			act("You suddenly realize that $n is standing beside you.", FALSE, ch, 0,
			    tch, TO_VICT);
	}

	GET_INVIS_LEV(ch) = level;
	player->send("Your invisibility level is %d.", level);
}


SUPERCMD(do_invis) {

	char arg[MAX_INPUT_LENGTH];
	int level;

	if(IS_NPC(ch)) {
		player->sendln("You can't do that!");
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		if(GET_INVIS_LEV(ch) > 0) {
			perform_immort_vis(ch);
		} else {
			perform_immort_invis(ch, GET_LEVEL(ch));
		}
	} else {
		level = atoi(arg);

		if(level > GET_LEVEL(ch)) {
			player->sendln("You can't go invisible above your own level.");
		} else if(level < 1) {
			perform_immort_vis(ch);
		} else {
			perform_immort_invis(ch, level);
		}
	}
}


SUPERCMD(do_gecho) {

	skip_spaces(&argument);
	delete_doubledollar(argument);

	if(!*argument) {
		player->sendln("That must be a mistake...");
	} else {
		for(auto& pt : descriptor_list) {
			if(STATE(pt) == CON_PLAYING && pt.character && pt.character != ch) {
				send_to_char(pt.character, "%s", argument);
			}
		}

		if(PRF_FLAGGED(ch, PRF_NOREPEAT)) {
			player->sendln(OK);
		} else {
			player->sendln(argument);
		}
	}
}


SUPERCMD(do_poofset) {

	skip_spaces(&argument);
	if(!*argument) {
		send_to_char(ch,"%s","That doesn't seem to be a valid string.");
		return;
	}
	switch(subcmd) {
		case SCMD_POOFIN:
			(ch)->player_specials->poofin = *argument;
			break;
		case SCMD_POOFOUT:
			(ch)->player_specials->poofout = *argument;
			break;
		default:
			return;
	}
	player->sendln(OK);
}



SUPERCMD(do_dc) {

	char arg[MAX_INPUT_LENGTH];
	int num_to_dc;

	one_argument(argument, arg);

	if(!(num_to_dc = atoi(arg))) {
		player->sendln("Usage: DC <user number> (type USERS for a list)");
		return;
	}

	auto d = descriptor_list.begin();
	bool found = false;
	for(; d != descriptor_list.end(); ++d) {
		if(d->desc_num == num_to_dc) {
			found = true;
			break;
		}
	}

	if(!found) {
		player->sendln("No such connection.");
		return;
	}

	if(d->character && GET_LEVEL(d->character) >= GET_LEVEL(ch)) {
		if(!CAN_SEE(ch, d->character)) {
			player->sendln("No such connection.");
		} else {
			player->sendln("Umm.. maybe that's not such a good idea...");
		}

		return;
	}

	/* We used to just close the socket here using close_socket(), but
	 * various people pointed out this could cause a crash if you're
	 * closing the person below you on the descriptor list.  Just setting
	 * to CON_CLOSE leaves things in a massively inconsistent state so I
	 * had to add this new flag to the descriptor. -je
	 *
	 * It is a much more logical extension for a CON_DISCONNECT to be used
	 * for in-game socket closes and CON_CLOSE for out of game closings.
	 * This will retain the stability of the close_me hack while being
	 * neater in appearance. -gg 12/1/97
	 *
	 * For those unlucky souls who actually manage to get disconnected
	 * by two different immortals in the same 1/10th of a second, we have
	 * the below 'if' check. -gg 12/17/99
	 */
	if(STATE(d) == CON_DISCONNECT || STATE(d) == CON_CLOSE) {
		player->sendln("They're already being disconnected.");
	} else {
		/*
		 * Remember that we can disconnect people not in the game and
		 * that rather confuses the code when it expected there to be
		 * a character context.
		 */
		if(STATE(d) == CON_PLAYING) {
			STATE(d) = CON_DISCONNECT;
		} else {
			STATE(d) = CON_CLOSE;
		}

		player->send("Connection #%d closed.", num_to_dc);
		log("(GC) Connection closed by %s.", GET_NAME(ch).c_str());
	}
}



SUPERCMD(do_wizlock) {

	char arg[MAX_INPUT_LENGTH];
	int value;
	const char *when;

	one_argument(argument, arg);

	if(*arg) {
		value = atoi(arg);

		if(value < 0 || value > GET_LEVEL(ch)) {
			player->sendln("Invalid wizlock value.");
			return;
		}

		circle_restrict = value;
		when = "now";
	} else {
		when = "currently";
	}

	switch(circle_restrict) {
		case 0:
			player->send("The game is %s completely open.", when);
			break;

		case 1:
			player->send("The game is %s closed to new players.", when);
			break;

		default:
			player->send("Only level %d and above may enter the game %s.", circle_restrict, when);
			break;
	}
}


SUPERCMD(do_date) {

	char *tmstr;
	time_t mytime;
	int d, h, m;

	if(subcmd == SCMD_DATE) {
		mytime = time(0);
	} else {
		mytime = boot_time;
	}

	tmstr = (char *) asctime(localtime(&mytime));
	*(tmstr + strlen(tmstr) - 1) = '\0';

	if(subcmd == SCMD_DATE) {
		player->send("Current machine time: %s", tmstr);
	} else {
		mytime = time(0) - boot_time;
		d = mytime / 86400;
		h = (mytime / 3600) % 24;
		m = (mytime / 60) % 60;

		player->send("Up since %s: %d day%s, %d:%02d", tmstr, d, d == 1 ? "" : "s", h, m);
	}
}



SUPERCMD(do_last) {

	char arg[MAX_INPUT_LENGTH];
	struct char_file_u chdata;

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln("For whom do you wish to search?");
		return;
	}

	if(char_exists(std::string(arg)) == false) {
		player->sendln("There is no such player.");
		return;
	}

}


SUPERCMD(do_force) {

	char_data *vict;
	char arg[MAX_INPUT_LENGTH], to_force[MAX_INPUT_LENGTH], buf1[MAX_INPUT_LENGTH + 32];

	half_chop(argument, arg, to_force);

	snprintf(buf1, sizeof(buf1), "$n has forced you to '%s'.", to_force);

	if(!*arg || !*to_force) {
		player->sendln("Whom do you wish to force do what?");
	} else if((GET_LEVEL(ch) < LVL_GRGOD) || (str_cmp("all", arg) && str_cmp("room", arg))) {
		if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD))) {
			player->sendln(NOPERSON);
		} else if(!IS_NPC(vict) && GET_LEVEL(ch) <= GET_LEVEL(vict)) {
			player->sendln("No, no, no!");
		} else {
			player->sendln(OK);
			act(buf1, TRUE, ch, NULL, vict, TO_VICT);
			mudlog(NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s forced %s to %s", GET_NAME(ch).c_str(), GET_NAME(vict).c_str(), to_force);
			auto v = ptr(vict);
			command_interpreter(v, std::string(to_force));
		}
	} else if(!str_cmp("room", arg)) {
		player->sendln(OK);
		mudlog(NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s forced room %d to %s",
		       GET_NAME(ch).c_str(), GET_ROOM_VNUM(IN_ROOM(ch)), to_force);

		//for(vict = world[IN_ROOM(ch)].people; vict; vict = next_force) {
		//		next_force = vict->next_in_room;
		for(auto& plr : mods::globals::get_room_list(player)) {
			auto vict = plr->cd();
			if(!IS_NPC(vict) && GET_LEVEL(vict) >= GET_LEVEL(ch)) {
				continue;
			}

			act(buf1, TRUE, ch, NULL, vict, TO_VICT);
			command_interpreter(plr, to_force);
		}
	} else { /* force all */
		player->sendln(OK);
		mudlog(NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s forced all to %s", GET_NAME(ch).c_str(), to_force);

		for(auto& i : descriptor_list) {
			if(STATE(i) != CON_PLAYING || !(vict = i.character) || (!IS_NPC(vict) && GET_LEVEL(vict) >= GET_LEVEL(ch))) {
				continue;
			}

			act(buf1, TRUE, ch, NULL, vict, TO_VICT);
			auto vict_ptr = ptr(i.character);
			command_interpreter(vict_ptr, std::string(to_force));
		}
	}
}



SUPERCMD(do_wiznet) {

	char buf1[MAX_INPUT_LENGTH + MAX_NAME_LENGTH + 32],
	     buf2[MAX_INPUT_LENGTH + MAX_NAME_LENGTH + 32];
	char emote = FALSE;
	int level = LVL_IMMORT;

	skip_spaces(&argument);
	delete_doubledollar(argument);

	if(!*argument) {
		player->sendln("Usage: wiznet <text> | #<level> <text> | *<emotetext> |        wiznet @<level> *<emotetext> | wiz @");
		return;
	}

	switch(*argument) {
		case '*':
			emote = TRUE;

		case '#':
			one_argument(argument + 1, buf1);

			if(is_number(buf1)) {
				half_chop(argument+1, buf1, argument);
				level = MAX(atoi(buf1), LVL_IMMORT);

				if(level > GET_LEVEL(ch)) {
					player->sendln("You can't wizline above your own level.");
					return;
				}
			} else if(emote) {
				argument++;
			}

			break;

		case '@':
			player->sendln("God channel status:");

			for(auto& d : descriptor_list) {
				if(STATE(d) != CON_PLAYING || GET_LEVEL(d.character) < LVL_IMMORT) {
					continue;
				}

				if(!CAN_SEE(ch, d.character)) {
					continue;
				}

				player->send("  %-*s%s%s%s", MAX_NAME_LENGTH, GET_NAME(d.character).c_str(),
				             PLR_FLAGGED(d.character, PLR_WRITING) ? " (Writing)" : "",
				             PLR_FLAGGED(d.character, PLR_MAILING) ? " (Writing mail)" : "",
				             PRF_FLAGGED(d.character, PRF_NOWIZ) ? " (Offline)" : "");
			}

			return;

		case '\\':
			++argument;
			break;

		default:
			break;
	}

	if(PRF_FLAGGED(ch, PRF_NOWIZ)) {
		player->sendln("You are offline!");
		return;
	}

	skip_spaces(&argument);

	if(!*argument) {
		player->sendln("Don't bother the gods like that!");
		return;
	}

	if(level > LVL_IMMORT) {
		snprintf(buf1, sizeof(buf1), "%s: <%d> %s%s", GET_NAME(ch).c_str(), level, emote ? "<--- " : "", argument);
		snprintf(buf2, sizeof(buf1), "Someone: <%d> %s%s", level, emote ? "<--- " : "", argument);
	} else {
		snprintf(buf1, sizeof(buf1), "%s: %s%s", GET_NAME(ch).c_str(), emote ? "<--- " : "", argument);
		snprintf(buf2, sizeof(buf1), "Someone: %s%s", emote ? "<--- " : "", argument);
	}

	for(auto& d : descriptor_list) {
		if((STATE(d) == CON_PLAYING) && (GET_LEVEL(d.character) >= level) &&
		        (!PRF_FLAGGED(d.character, PRF_NOWIZ)) &&
		        (!PLR_FLAGGED(d.character, PLR_WRITING | PLR_MAILING))
		        && (!(PRF_FLAGGED(d.character, PRF_NOREPEAT)))) {
			send_to_char(d.character, "%s", CCCYN(d.character, C_NRM));

			if(CAN_SEE(d.character, ch)) {
				send_to_char(d.character, "%s", buf1);
			} else {
				send_to_char(d.character, "%s", buf2);
			}

			send_to_char(d.character, "%s", CCNRM(d.character, C_NRM));
		}
	}

	if(PRF_FLAGGED(ch, PRF_NOREPEAT)) {
		player->sendln(OK);
	}
}



SUPERCMD(do_zreset) {

	char arg[MAX_INPUT_LENGTH];
	zone_rnum i;
	zone_vnum j;

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln("You must specify a zone.");
		return;
	}

	if(*arg == '*') {
		for(i = 0; i <= top_of_zone_table; i++) {
			reset_zone(i);
		}

		player->sendln("Reset world.");
		mudlog(NRM, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s reset entire world.", GET_NAME(ch).c_str());
		return;
	} else if(*arg == '.') {
		i = world[IN_ROOM(ch)].zone;
	} else {
		j = atoi(arg);

		for(i = 0; i <= top_of_zone_table; i++)
			if(zone_table[i].number == j) {
				break;
			}
	}

	if(i <= top_of_zone_table) {
		reset_zone(i);
		player->send("Reset zone %d (#%d): %s.", i, zone_table[i].number, zone_table[i].name.c_str());
		mudlog(NRM, MAX(LVL_GRGOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s reset zone %d (%s)", GET_NAME(ch).c_str(), i, zone_table[i].name.c_str());
	} else {
		player->sendln("Invalid zone number.");
	}
}

/**
 * Command for adding/removing quotes from the database
 *
 * example: wiz_quote list
 * example: wiz_quote add "title goes here" @@@description goes here @@@
 * example: wiz_quote delete <id>
 * example: wiz_quote ammend <title|content> <id> <title|@@@content@@@>
 */
SUPERCMD(do_wiz_quote) {
}

/*
 *  General fn for wizcommands of the sort: cmd <player>
 */
SUPERCMD(do_wizutil) {

	char arg[MAX_INPUT_LENGTH];
	char_data *vict;
	long result;

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln("Yes, but for whom?!?");
	} else if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD))) {
		player->sendln("There is no such player.");
	} else if(IS_NPC(vict)) {
		player->sendln("You can't do that to a mob!");
	} else if(GET_LEVEL(vict) > GET_LEVEL(ch)) {
		player->sendln("Hmmm...you'd better not.");
	} else {
		switch(subcmd) {
			case SCMD_REROLL:
				player->sendln("Rerolled...");
				roll_real_abils(vict);
				log("(GC) %s has rerolled %s.", GET_NAME(ch).c_str(), GET_NAME(vict).c_str());
				player->send("New stats: Str %d/%d, Int %d, Wis %d, Dex %d, Con %d, Cha %d",
				             GET_STR(vict), GET_ADD(vict), GET_INT(vict), GET_WIS(vict),
				             GET_DEX(vict), GET_CON(vict), GET_CHA(vict));
				break;

			case SCMD_PARDON:
				if(!PLR_FLAGGED(vict, PLR_THIEF | PLR_KILLER)) {
					player->sendln("Your victim is not flagged.");
					return;
				}

				REMOVE_BIT(PLR_FLAGS(vict), PLR_THIEF | PLR_KILLER);
				player->sendln("Pardoned.");
				send_to_char(vict, "You have been pardoned by the Gods!");
				mudlog(BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s pardoned by %s", GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				break;

			case SCMD_NOTITLE:
				result = PLR_TOG_CHK(vict, PLR_NOTITLE);
				mudlog(NRM, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) Notitle %s for %s by %s.",
				       ONOFF(result), GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				player->send("(GC) Notitle %s for %s by %s.", ONOFF(result), GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				break;

			case SCMD_SQUELCH:
				result = PLR_TOG_CHK(vict, PLR_NOSHOUT);
				mudlog(BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) Squelch %s for %s by %s.",
				       ONOFF(result), GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				player->send("(GC) Squelch %s for %s by %s.", ONOFF(result), GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				break;

			case SCMD_FREEZE:
				if(ch == vict) {
					player->sendln("Oh, yeah, THAT'S real smart...");
					return;
				}

				if(PLR_FLAGGED(vict, PLR_FROZEN)) {
					player->sendln("Your victim is already pretty cold.");
					return;
				}

				SET_BIT(PLR_FLAGS(vict), PLR_FROZEN);
				GET_FREEZE_LEV(vict) = GET_LEVEL(ch);
				send_to_char(vict, "A bitter wind suddenly rises and drains every erg of heat from your body!You feel frozen!");
				player->sendln("Frozen.");
				act("A sudden cold wind conjured from nowhere freezes $n!", FALSE, vict, 0, 0, TO_ROOM);
				mudlog(BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s frozen by %s.", GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				break;

			case SCMD_THAW:
				if(!PLR_FLAGGED(vict, PLR_FROZEN)) {
					player->sendln("Sorry, your victim is not morbidly encased in ice at the moment.");
					return;
				}

				if(GET_FREEZE_LEV(vict) > GET_LEVEL(ch)) {
					player->send("Sorry, a level %d God froze %s... you can't unfreeze %s.",
					             GET_FREEZE_LEV(vict), GET_NAME(vict).c_str(), HMHR(vict));
					return;
				}

				mudlog(BRF, MAX(LVL_GOD, GET_INVIS_LEV(ch)), TRUE, "(GC) %s un-frozen by %s.", GET_NAME(vict).c_str(), GET_NAME(ch).c_str());
				REMOVE_BIT(PLR_FLAGS(vict), PLR_FROZEN);
				send_to_char(vict, "A fireball suddenly explodes in front of you, melting the ice!You feel thawed.");
				player->sendln("Thawed.");
				act("A sudden fireball conjured from nowhere thaws $n!", FALSE, vict, 0, 0, TO_ROOM);
				break;

			case SCMD_UNAFFECT:
				if(vict->affected) {
					while(vict->affected) {
						affect_remove(vict, vict->affected);
					}

					send_to_char(vict, "There is a brief flash of light!You feel slightly different.");
					player->sendln("All spells removed.");
				} else {
					player->sendln("Your victim does not have any affections!");
					return;
				}

				break;

			default:
				log("SYSERR: Unknown subcmd %d passed to do_wizutil (%s)", subcmd, __FILE__);
				break;
		}

		mods::db::save_char(std::make_shared<mods::player>(vict));
	}
}


/* single zone printing fn used by "show zone" so it's not repeated in the
   code 3 times ... -je, 4/6/93 */

/* FIXME: overflow possible */
size_t print_zone_to_buf(char *bufptr, size_t left, zone_rnum zone) {
	return 0;
}


SUPERCMD(do_show) {

	struct char_file_u vbuf;
	unsigned i, j, l, con, nlen;		/* i, j, k to specifics? */
	size_t len;
	zone_rnum zrn;
	zone_vnum zvn;
	byte self = FALSE;
	char_data *vict;
	char field[MAX_INPUT_LENGTH], value[MAX_INPUT_LENGTH],
	     arg[MAX_INPUT_LENGTH], buf[MAX_STRING_LENGTH];

	struct show_struct {
		const char *cmd;
		const player_level_t level;
	} fields[] = {
		{ "nothing",	LVL_MORTAL  },				/* 0 */
		{ "zones",		LVL_IMMORT },			/* 1 */
		{ "player",		LVL_GOD },
		{ "rent",		LVL_GOD },
		{ "stats",		LVL_IMMORT },
		{ "errors",		LVL_IMPL },			/* 5 */
		{ "death",		LVL_GOD },
		{ "godrooms",	LVL_GOD },
		{ "shops",		LVL_IMMORT },
		{ "houses",		LVL_GOD },
		{ "snoop",		LVL_GRGOD },			/* 10 */
		{ "\n", LVL_MORTAL }
	};

	skip_spaces(&argument);

	if(!*argument) {
		player->sendln("Show options:");

		for(j = 0, i = 1; fields[i].level; i++)
			if(fields[i].level <= GET_LEVEL(ch)) {
				player->send("%-15s%s", fields[i].cmd, (!(++j % 5) ? "" : ""));
			}

		player->sendln("");
		return;
	}

	strcpy(arg, two_arguments(argument, field, value));	/* strcpy: OK (argument <= MAX_INPUT_LENGTH == arg) */

	for(l = 0; *(fields[l].cmd) != '\n'; l++)
		if(!strncmp(field, fields[l].cmd, strlen(field))) {
			break;
		}

	if(GET_LEVEL(ch) < fields[l].level) {
		player->sendln("You are not godly enough for that!");
		return;
	}

	if(!strcmp(value, ".")) {
		self = TRUE;
	}

	buf[0] = '\0';

	switch(l) {
		/* show zone */
		case 1:

			/* tightened up by JE 4/6/93 */
			if(self) {
				print_zone_to_buf(buf, sizeof(buf), world[IN_ROOM(ch)].zone);
			} else if(*value && is_number(value)) {
				for(zvn = atoi(value), zrn = 0; zone_table[zrn].number != zvn && zrn <= top_of_zone_table; zrn++);

				if(zrn <= top_of_zone_table) {
					print_zone_to_buf(buf, sizeof(buf), zrn);
				} else {
					player->sendln("That is not a valid zone.");
					return;
				}
			} else
				for(len = zrn = 0; zrn <= top_of_zone_table; zrn++) {
					nlen = print_zone_to_buf(buf + len, sizeof(buf) - len, zrn);

					if(len + nlen >= sizeof(buf) || nlen < 0) {
						break;
					}

					len += nlen;
				}

			page_string(*ch->desc, buf, TRUE);
			break;

		/* show player */
		case 2:
			if(!*value) {
				player->sendln("A name would help.");
				return;
			}

			if(char_exists(std::string(value)) == false) {
				player->sendln("There is no such player.");
				return;
			}

			player->send("Player: %-12s (%s) [%2d %s]", vbuf.name.c_str(),
			             genders[(int) vbuf.sex], 0, class_abbrevs[(int) vbuf.chclass]);
			player->send("Au: %-8d  Bal: %-8d  Exp: %-8d  Align: %-5d",
			             vbuf.points.gold, vbuf.points.bank_gold, vbuf.points.exp,
			             vbuf.char_specials_saved.alignment);
			/* ctime() uses static buffer: do not combine. */
			player->send("Started: %-20.16s  ", ctime(&vbuf.birth));
			player->send("Last: %-20.16s  Played: %3dh %2dm", ctime(&vbuf.last_logon), vbuf.played / 3600, vbuf.played / 60 % 60);
			break;

		/* show rent */
		case 3:
			if(!*value) {
				player->sendln("A name would help.");
				return;
			}

			Crash_listrent(ch, value);
			break;

		/* show stats */
		case 4:
			i = 0;
			j = 0;
			con = 0;

			for(vict = character_list; vict; vict = vict->next) {
				if(IS_NPC(vict)) {
					j++;
				} else if(CAN_SEE(ch, vict)) {
					i++;

					if(vict->has_desc) {
						con++;
					}
				}
			}

			send_to_char(ch,
			             "Current stats:"
			             "  %5d players in game  %5d connected"
			             "  %5d registered"
			             "  %5d mobiles          %5d prototypes"
			             "  %5lu objects          %5d prototypes"
			             "  %5d rooms            %5d zones"
			             "  %5d large bufs"
			             "  %5d buf switches     %5d overflows",
			             i, con,
			             top_of_p_table + 1,
			             j, top_of_mobt + 1,
			             obj_list.size(), top_of_objt + 1,
			             top_of_world + 1, top_of_zone_table + 1,
			             buf_largecount,
			             buf_switches, buf_overflows
			            );
			break;

		/* show errors */
		case 5:
			len = strlcpy(buf, "Errant Rooms------------", sizeof(buf));

			for(unsigned i = 0, k = 0; i <= top_of_world; i++)
				for(j = 0; j < NUM_OF_DIRS; j++)
					if(world[i].dir_option[j] && world[i].dir_option[j]->to_room == 0) {
						nlen = snprintf(buf + len, sizeof(buf) - len, "%2d: [%5d] %s", ++k, GET_ROOM_VNUM(i), world[i].name.c_str());

						if(len + nlen >= sizeof(buf) || nlen < 0) {
							break;
						}

						len += nlen;
					}

			page_string(*ch->desc, buf, TRUE);
			break;

		/* show death */
		case 6:
			len = strlcpy(buf, "Death Traps-----------", sizeof(buf));

			for(i = 0, j = 0; i <= top_of_world; i++)
				if(ROOM_FLAGGED(i, ROOM_DEATH)) {
					nlen = snprintf(buf + len, sizeof(buf) - len, "%2d: [%5d] %s", ++j, GET_ROOM_VNUM(i), world[i].name.c_str());

					if(len + nlen >= sizeof(buf) || nlen < 0) {
						break;
					}

					len += nlen;
				}

			page_string(*ch->desc, buf, TRUE);
			break;

		/* show godrooms */
		case 7:
			len = strlcpy(buf, "Godrooms--------------------------", sizeof(buf));

			for(i = 0, j = 0; i <= top_of_world; i++)
				if(ROOM_FLAGGED(i, ROOM_GODROOM)) {
					nlen = snprintf(buf + len, sizeof(buf) - len, "%2d: [%5d] %s", ++j, GET_ROOM_VNUM(i), world[i].name.c_str());

					if(len + nlen >= sizeof(buf) || nlen < 0) {
						break;
					}

					len += nlen;
				}

			page_string(*ch->desc, buf, TRUE);
			break;

		/* show shops */
		case 8:
			show_shops(ch, value);
			break;

		/* show houses */
		case 9:
			hcontrol_list_houses(ch);
			break;

		/* show snoop */
		case 10:
			i = 0;
			player->sendln("People currently snooping:--------------------------");

			for(auto& d : descriptor_list) {
				if(d.snooping == NULL || d.character == NULL) {
					continue;
				}

				if(STATE(d) != CON_PLAYING || GET_LEVEL(ch) < GET_LEVEL(d.character)) {
					continue;
				}

				if(!CAN_SEE(ch, d.character) || IN_ROOM(d.character) == NOWHERE) {
					continue;
				}

				i++;
				player->send("%-10s - snooped by %s.", GET_NAME(d.snooping->character).c_str(), GET_NAME(d.character).c_str());
			}

			if(i == 0) {
				player->sendln("No one is currently snooping.");
			}

			break;

		/* show what? */
		default:
			player->sendln("Sorry, I don't understand that.");
			break;
	}//end switch
}


/***************** The do_set function ***********************************/

#define PC   1
#define NPC  2
#define BOTH 3

#define MISC	0
#define BINARY	1
#define NUMBER	2

#define SET_OR_REMOVE(flagset, flags) { \
	if (on) SET_BIT(flagset, flags); \
	else if (off) REMOVE_BIT(flagset, flags); }

#define RANGE(low, high) (value = MAX((low), MIN((high), (value))))


/* The set options available */
struct set_struct {
	const char *cmd;
	const player_level_t level;
	const char pcnpc;
	const char type;
} set_fields[] = {
	{ "brief",		LVL_GOD, 	PC, 	BINARY },  /* 0 */
	{ "invstart", 	LVL_GOD, 	PC, 	BINARY },  /* 1 */
	{ "title",		LVL_GOD, 	PC, 	MISC },
	{ "nosummon", 	LVL_GRGOD, 	PC, 	BINARY },
	{ "maxhit",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "maxmana", 	LVL_GRGOD, 	BOTH, 	NUMBER },  /* 5 */
	{ "maxmove", 	LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "hit", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "mana",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "move",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "align",		LVL_GOD, 	BOTH, 	NUMBER },  /* 10 */
	{ "str",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "stradd",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "int", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "wis", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "dex", 		LVL_GRGOD, 	BOTH, 	NUMBER },  /* 15 */
	{ "con", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "cha",		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "ac", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "gold",		LVL_GOD, 	BOTH, 	NUMBER },
	{ "bank",		LVL_GOD, 	PC, 	NUMBER },  /* 20 */
	{ "exp", 		LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "hitroll", 	LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "damroll", 	LVL_GRGOD, 	BOTH, 	NUMBER },
	{ "invis",		LVL_IMPL, 	PC, 	NUMBER },
	{ "nohassle", 	LVL_GRGOD, 	PC, 	BINARY },  /* 25 */
	{ "frozen",		LVL_FREEZE, 	PC, 	BINARY },
	{ "practices", 	LVL_GRGOD, 	PC, 	NUMBER },
	{ "lessons", 	LVL_GRGOD, 	PC, 	NUMBER },
	{ "drunk",		LVL_GRGOD, 	BOTH, 	MISC },
	{ "hunger",		LVL_GRGOD, 	BOTH, 	MISC },    /* 30 */
	{ "thirst",		LVL_GRGOD, 	BOTH, 	MISC },
	{ "killer",		LVL_GOD, 	PC, 	BINARY },
	{ "thief",		LVL_GOD, 	PC, 	BINARY },
	{ "level",		LVL_IMPL, 	BOTH, 	NUMBER },
	{ "room",		LVL_IMPL, 	BOTH, 	NUMBER },  /* 35 */
	{ "roomflag", 	LVL_GRGOD, 	PC, 	BINARY },
	{ "siteok",		LVL_GRGOD, 	PC, 	BINARY },
	{ "deleted", 	LVL_IMPL, 	PC, 	BINARY },
	{ "class",		LVL_GRGOD, 	BOTH, 	MISC },
	{ "nowizlist", 	LVL_GOD, 	PC, 	BINARY },  /* 40 */
	{ "quest",		LVL_GOD, 	PC, 	BINARY },
	{ "loadroom", 	LVL_GRGOD, 	PC, 	MISC },
	{ "color",		LVL_GOD, 	PC, 	BINARY },
	{ "idnum",		LVL_IMPL, 	PC, 	NUMBER },
	{ "passwd",		LVL_IMPL, 	PC, 	MISC },    /* 45 */
	{ "nodelete", 	LVL_GOD, 	PC, 	BINARY },
	{ "sex", 		LVL_GRGOD, 	BOTH, 	MISC },
	{ "age",		LVL_GRGOD,	BOTH,	NUMBER },
	{ "height",		LVL_GOD,	BOTH,	NUMBER },
	{ "weight",		LVL_GOD,	BOTH,	NUMBER },  /* 50 */
	{ "\n", LVL_MORTAL, BOTH, MISC }
};


int perform_set(char_data *ch, char_data *vict, int mode,
                char *val_arg) {
	MENTOC_PREAMBLE();
	int i, on = 0, off = 0, value = 0;
	room_rnum rnum;
	room_vnum rvnum;

	/* Check to make sure all the levels are correct */
	if(GET_LEVEL(ch) != LVL_IMPL) {
		if(!IS_NPC(vict) && GET_LEVEL(ch) <= GET_LEVEL(vict) && vict != ch) {
			player->sendln("Maybe that's not such a great idea...");
			return (0);
		}
	}

	if(GET_LEVEL(ch) < set_fields[mode].level) {
		player->sendln("You are not godly enough for that!");
		return (0);
	}

	/* Make sure the PC/NPC is correct */
	if(IS_NPC(vict) && !(set_fields[mode].pcnpc & NPC)) {
		player->sendln("You can't do that to a beast!");
		return (0);
	} else if(!IS_NPC(vict) && !(set_fields[mode].pcnpc & PC)) {
		player->sendln("That can only be done to a beast!");
		return (0);
	}

	/* Find the value of the argument */
	if(set_fields[mode].type == BINARY) {
		if(!strcmp(val_arg, "on") || !strcmp(val_arg, "yes")) {
			on = 1;
		} else if(!strcmp(val_arg, "off") || !strcmp(val_arg, "no")) {
			off = 1;
		}

		if(!(on || off)) {
			player->sendln("Value must be 'on' or 'off'.");
			return (0);
		}

		player->send("%s %s for %s.", set_fields[mode].cmd, ONOFF(on), GET_NAME(vict).c_str());
	} else if(set_fields[mode].type == NUMBER) {
		value = atoi(val_arg);
		player->send("%s's %s set to %d.", GET_NAME(vict).c_str(), set_fields[mode].cmd, value);
	} else {
		player->sendln(OK);
	}

	switch(mode) {
		case 0:
			SET_OR_REMOVE(PRF_FLAGS(vict), PRF_BRIEF);
			break;

		case 1:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_INVSTART);
			break;

		case 2:
			set_title(vict, val_arg);
			player->send("%s's title is now: %s", GET_NAME(vict).c_str(), GET_TITLE(vict).c_str());
			break;

		case 3:
			SET_OR_REMOVE(PRF_FLAGS(vict), PRF_SUMMONABLE);
			player->send("Nosummon %s for %s.", ONOFF(!on), GET_NAME(vict).c_str());
			break;

		case 4:
			vict->points.max_hit = RANGE(1, 5000);
			affect_total(vict);
			break;

		case 5:
			vict->points.max_mana = RANGE(1, 5000);
			affect_total(vict);
			break;

		case 6:
			vict->points.max_move = RANGE(1, 5000);
			affect_total(vict);
			break;

		case 7:
			vict->points.hit = RANGE(-9, vict->points.max_hit);
			affect_total(vict);
			break;

		case 8:
			vict->points.mana = RANGE(0, vict->points.max_mana);
			affect_total(vict);
			break;

		case 9:
			vict->points.move = RANGE(0, vict->points.max_move);
			affect_total(vict);
			break;

		case 10:
			GET_ALIGNMENT(vict) = RANGE(-1000, 1000);
			affect_total(vict);
			break;

		case 11:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.str = value;
			vict->real_abils.str_add = 0;
			affect_total(vict);
			break;

		case 12:
			vict->real_abils.str_add = RANGE(0, 100);

			if(value > 0) {
				vict->real_abils.str = 18;
			}

			affect_total(vict);
			break;

		case 13:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.intel = value;
			affect_total(vict);
			break;

		case 14:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.wis = value;
			affect_total(vict);
			break;

		case 15:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.dex = value;
			affect_total(vict);
			break;

		case 16:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.con = value;
			affect_total(vict);
			break;

		case 17:
			if(IS_NPC(vict) || GET_LEVEL(vict) >= LVL_GRGOD) {
				RANGE(3, 25);
			} else {
				RANGE(3, 18);
			}

			vict->real_abils.cha = value;
			affect_total(vict);
			break;

		case 18:
			vict->points.armor = RANGE(-100, 100);
			affect_total(vict);
			break;

		case 19:
			GET_GOLD(vict) = RANGE(0, 100000000);
			break;

		case 20:
			GET_BANK_GOLD(vict) = RANGE(0, 100000000);
			break;

		case 21:
			vict->points.exp = RANGE(0, 50000000);
			break;

		case 22:
			vict->points.hitroll = RANGE(-20, 20);
			affect_total(vict);
			break;

		case 23:
			vict->points.damroll = RANGE(-20, 20);
			affect_total(vict);
			break;

		case 24:
			if(GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
				player->sendln("You aren't godly enough for that!");
				return (0);
			}

			GET_INVIS_LEV(vict) = RANGE(0, GET_LEVEL(vict));
			break;

		case 25:
			if(GET_LEVEL(ch) < LVL_IMPL && ch != vict) {
				player->sendln("You aren't godly enough for that!");
				return (0);
			}

			SET_OR_REMOVE(PRF_FLAGS(vict), PRF_NOHASSLE);
			break;

		case 26:
			if(ch == vict && on) {
				player->sendln("Better not -- could be a long winter!");
				return (0);
			}

			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_FROZEN);
			break;

		case 27:
		case 28:
			break;

		case 29:
		case 30:
		case 31:
			if(!str_cmp(val_arg, "off")) {
				GET_COND(vict, (mode - 29)) = -1; /* warning: magic number here */
				player->send("%s's %s now off.", GET_NAME(vict).c_str(), set_fields[mode].cmd);
			} else if(is_number(val_arg)) {
				value = atoi(val_arg);
				RANGE(0, 24);
				GET_COND(vict, (mode - 29)) = value; /* and here too */
				player->send("%s's %s set to %d.", GET_NAME(vict).c_str(), set_fields[mode].cmd, value);
			} else {
				player->sendln("Must be 'off' or a value from 0 to 24.");
				return (0);
			}

			break;

		case 32:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_KILLER);
			break;

		case 33:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_THIEF);
			break;

		case 34:
			if(value > GET_LEVEL(ch) || value > LVL_IMPL) {
				player->sendln("You can't do that.");
				return (0);
			}

			RANGE(0, LVL_IMPL);
			vict->player.level = value;
			break;

		case 35:
			if((rnum = real_room(value)) == NOWHERE) {
				player->sendln("No room exists with that number.");
				return (0);
			}

			if(IN_ROOM(vict) != NOWHERE) {	/* Another Eric Green special. */
				char_from_room(vict);
			}

			char_to_room(vict, rnum);
			break;

		case 36:
			SET_OR_REMOVE(PRF_FLAGS(vict), PRF_ROOMFLAGS);
			break;

		case 37:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_SITEOK);
			break;

		case 38:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_DELETED);
			break;

		case 39:
			if((i = parse_class(val_arg)) == CLASS_UNDEFINED) {
				player->sendln("That is not a class.");
				return (0);
			}

			GET_CLASS(vict) = i;
			break;

		case 40:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_NOWIZLIST);
			break;

		case 41:
			SET_OR_REMOVE(PRF_FLAGS(vict), PRF_QUEST);
			break;

		case 42:
			if(!str_cmp(val_arg, "off")) {
				REMOVE_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
			} else if(is_number(val_arg)) {
				rvnum = atoi(val_arg);

				if(real_room(rvnum) != NOWHERE) {
					SET_BIT(PLR_FLAGS(vict), PLR_LOADROOM);
					GET_LOADROOM(vict) = rvnum;
					player->send("%s will enter at room #%d.", GET_NAME(vict).c_str(), GET_LOADROOM(vict));
				} else {
					player->sendln("That room does not exist!");
					return (0);
				}
			} else {
				player->sendln("Must be 'off' or a room's virtual number.");
				return (0);
			}

			break;

		case 43:
			SET_OR_REMOVE(PRF_FLAGS(vict), (PRF_COLOR_1 | PRF_COLOR_2));
			break;

		case 44:
			if(GET_IDNUM(ch) != 1 || !IS_NPC(vict)) {
				return (0);
			}

			GET_IDNUM(vict) = value;
			break;

		case 45:
			if(GET_IDNUM(ch) > 1) {
				player->sendln("Please don't use this command, yet.");
				return (0);
			}

			if(GET_LEVEL(vict) >= LVL_GRGOD) {
				player->sendln("You cannot change that.");
				return (0);
			}

			GET_PASSWD(vict).assign(CRYPT(val_arg, GET_NAME(vict)));
			player->send("Password changed to '%s'.", val_arg);
			break;

		case 46:
			SET_OR_REMOVE(PLR_FLAGS(vict), PLR_NODELETE);
			break;

		case 47:
			if((i = search_block(val_arg, genders, FALSE)) < 0) {
				player->sendln("Must be 'male', 'female', or 'neutral'.");
				return (0);
			}

			GET_SEX(vict) = i;
			break;

		case 48:	/* set age */
			if(value < 2 || value > 200) {	/* Arbitrary limits. */
				player->sendln("Ages 2 to 200 accepted.");
				return (0);
			}

			/*
			 * NOTE: May not display the exact age specified due to the integer
			 * division used elsewhere in the code.  Seems to only happen for
			 * some values below the starting age (17) anyway. -gg 5/27/98
			 */
			vict->player.time.birth = time(0) - ((value - 17) * SECS_PER_MUD_YEAR);
			break;

		case 49:	/* Blame/Thank Rick Glover. :) */
			GET_HEIGHT(vict) = value;
			affect_total(vict);
			break;

		case 50:
			GET_WEIGHT(vict) = value;
			affect_total(vict);
			break;

		default:
			player->sendln("Can't set that!");
			return (0);
	}

	return (1);
}


SUPERCMD(do_set) {

	char_data *vict = NULL, *cbuf = NULL;
	struct char_file_u tmp_store;
	char field[MAX_INPUT_LENGTH], name[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
	int mode = 0, len = 0, retval =0;
	char is_file = 0, is_player = 0;

	half_chop(argument, name, buf);

	if(!strcmp(name, "file")) {
		is_file = 1;
		half_chop(buf, name, buf);
	} else if(!str_cmp(name, "player")) {
		is_player = 1;
		half_chop(buf, name, buf);
	} else if(!str_cmp(name, "mob")) {
		half_chop(buf, name, buf);
	}

	half_chop(buf, field, buf);

	if(!*name || !*field) {
		player->sendln("Usage: set <victim> <field> <value>");
		return;
	}

	/* find the target */
	if(!is_file) {
		if(is_player) {
			if(!(vict = get_player_vis(ch, name, NULL, FIND_CHAR_WORLD))) {
				player->sendln("There is no such player.");
				return;
			}
		} else { /* is_mob */
			if(!(vict = get_char_vis(ch, name, NULL, FIND_CHAR_WORLD))) {
				player->sendln("There is no such creature.");
				return;
			}
		}
	} else if(is_file) {
		return;

	}

	/* find the command in the list */
	len = strlen(field);

	for(mode = 0; *(set_fields[mode].cmd) != '\n'; mode++)
		if(!strncmp(field, set_fields[mode].cmd, len)) {
			break;
		}

	/* perform the set */
	retval = perform_set(ch, vict, mode, buf);

	/* save the character if a change was made */
	if(retval) {
		if(!is_file && !IS_NPC(vict)) {
			mods::db::save_char(std::make_shared<mods::player>(vict));
		}

		if(is_file) {
			//char_to_store(vict, &tmp_store);
			player->sendln("Saved in file.");
		}
	}

	/* free the memory if we allocated it earlier */
	if(is_file) {
		free_char(cbuf);
	}
}

