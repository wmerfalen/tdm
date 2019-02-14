/* ************************************************************************
*   File: spec_assign.c                                 Part of CircleMUD *
*  Usage: Functions to assign function pointers to objs/mobs/rooms        *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "interpreter.h"
#include "utils.h"


/* external globals */
extern int dts_are_dumps;
extern int mini_mud;

/* external functions */
SPECIAL(dump);
SPECIAL(pet_shops);
SPECIAL(postmaster);
SPECIAL(cityguard);
SPECIAL(receptionist);
SPECIAL(cryogenicist);
SPECIAL(guild_guard);
SPECIAL(guild);
SPECIAL(puff);
SPECIAL(fido);
SPECIAL(janitor);
SPECIAL(mayor);
SPECIAL(snake);
SPECIAL(thief);
SPECIAL(magic_user);
SPECIAL(bank);
SPECIAL(gen_board);
void assign_kings_castle(void);

/* local functions */
void assign_mobiles();
void assign_objects();
void assign_rooms();
void ASSIGNROOM(room_vnum room, SPECIAL(fname));
void ASSIGNMOB(mob_vnum mob, SPECIAL(fname));
void ASSIGNOBJ(obj_vnum obj, SPECIAL(fname));

/* functions to perform assignments */

void ASSIGNMOB(mob_vnum mob, SPECIAL(fname)) {
	mob_rnum rnum;

	if((rnum = real_mobile(mob)) != NOBODY) {
		mob_index[rnum].func = fname;
	} else if(!mini_mud) {
		log("SYSERR: Attempt to assign spec to non-existant mob #%d", mob);
	}
}

void ASSIGNOBJ(obj_vnum obj, SPECIAL(fname)) {
	obj_rnum rnum;

	if((rnum = real_object(obj)) != NOTHING) {
		obj_index[rnum].func = fname;
	} else if(!mini_mud) {
		log("SYSERR: Attempt to assign spec to non-existant obj #%d", obj);
	}
}

void ASSIGNROOM(room_vnum room, SPECIAL(fname)) {
	room_rnum rnum;

	if((rnum = real_room(room)) != NOWHERE) {
		world[rnum].func = fname;
	} else if(!mini_mud) {
		log("SYSERR: Attempt to assign spec to non-existant room #%d", room);
	}
}


/* ********************************************************************
*  Assignments                                                        *
******************************************************************** */

/* assign special procedures to mobiles */
void assign_mobiles() {
}



/* assign special procedures to objects */
void assign_objects() {
}



/* assign special procedures to rooms */
void assign_rooms(void) {
	if(top_of_world == 0){
		log("INFO: assign_rooms called, but top_of_world is zero");
		return;
	}
	room_rnum i;

	if(dts_are_dumps)
		for(i = 0; i <= top_of_world; i++)
			if(ROOM_FLAGGED(i, ROOM_DEATH)) {
				world[i].func = dump;
			}
}
