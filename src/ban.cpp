/* ************************************************************************
*   File: ban.c                                         Part of CircleMUD *
*  Usage: banning/unbanning/checking sites and player names               *
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

/* local globals */
struct ban_list_element *ban_list = NULL;

/* local functions */
void load_banned(void);
int isbanned(char *hostname);
void _write_one_node(FILE *fp, struct ban_list_element *node);
void write_ban_list(void);
SUPERCMD(do_ban);
SUPERCMD(do_unban);
int Valid_Name(char *newname);
void Read_Invalid_List(void);
void Free_Invalid_List(void);


const char *ban_types[] = {
	"no",
	"new",
	"select",
	"all",
	"ERROR"
};


void load_banned(void) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}


int isbanned(const char *hostname) {
	log("[deprecated] banning by hostname (isbanned)");
	return 0;
}


void _write_one_node(FILE *fp, struct ban_list_element *node) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}



void write_ban_list(void) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}


#define BAN_LIST_FORMAT "%-25.25s  %-8.8s  %-10.10s  %-16.16s\r\n"
SUPERCMD(do_ban) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}
#undef BAN_LIST_FORMAT


SUPERCMD(do_unban) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}


/**************************************************************************
 *  Code to check for invalid names (i.e., profanity, etc.)		  *
 *  Written by Sharon P. Goza						  *
 **************************************************************************/

#define MAX_INVALID_NAMES	200

char *invalid_list[MAX_INVALID_NAMES];
int num_invalid = 0;

int Valid_Name(const char *newname) {
	/*
	 * Make sure someone isn't trying to create this same name.  We want to
	 * do a 'str_cmp' so people can't do 'Bob' and 'BoB'.  The creating login
	 * will not have a character name yet and other people sitting at the
	 * prompt won't have characters yet.
	 */
	for(auto& p : mods::globals::player_list) {
		if(mods::util::is_lower_match(p->name().c_str(), newname)) {
			if((p->state() == CON_PLAYING || p->state() == CON_IDLE) && p->authenticated()) {
				return 0;
			}
		}
	}

	return (1);
}


/* What's with the wacky capitalization in here? */
void Free_Invalid_List(void) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}


void Read_Invalid_List(void) {
	log("DEPRECATED: load_banned uses flat files. Replace this with postgresql.");
	return;
}
