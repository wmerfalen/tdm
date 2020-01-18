/* ************************************************************************
 *   File: act.offensive.c                               Part of CircleMUD *
 *  Usage: player-level commands of an offensive nature                    *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#include "globals.hpp"
#include "mods/scan.hpp"
#include "globals.hpp"
#include "mods/util.hpp"
#include "mods/item.hpp"
#include "mods/sensor-grenade.hpp"


/**
 * Arguments:
 * install <claymore> <direction>
 */
ACMD(do_install) {
	MENTOC_PREAMBLE();
	auto parsed = mods::util::parse_objdir(player,argument);
	if(!parsed.obj){
		player->sendln("You don't have anything that matches that description");
		return;
	}
	if(parsed.dir < 0){
		player->sendln("Use a valid direction");
		return;
	}
	//if(parsed.obj->ca mods::item::capability_t::INSTALL);
}
