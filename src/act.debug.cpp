/* ************************************************************************
*   File: act.comm.c                                    Part of CircleMUD *
*  Usage: Player-level communication commands                             *
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
#include "screen.h"
#include "globals.hpp"

extern void point_update(void);

ACMD(do_givemenades){
	


}

ACMD(do_point_update) {
	
	if(IS_NPC(ch)){
		return;
	}
	player->stc("[debug]->Running point_update");
	point_update();
	player->stc("[debug]->done");
}

ACMD(do_my_uuid) {
	
	player->stc(std::to_string(player->uuid()));
}
ACMD(do_zero_socket) {
	
	player->stc("[debug]->Zeroing out your socket (goodbye).");
	player->desc().descriptor = 0;
}

