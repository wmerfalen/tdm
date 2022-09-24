/* ************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#ifndef __MENTOC_ROOM_DIRECTION_DATA_HEADER__
#define __MENTOC_ROOM_DIRECTION_DATA_HEADER__

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <set>
#include "mods/string.hpp"
#include "mods/index-type.hpp"

struct room_direction_data {
	mods::string general_description;       /* When look DIR.			*/

	mods::string keyword;		/* for open/close			*/

	uint32_t exit_info;	/* Exit info			*/
	obj_vnum key;		/* Key's number (-1 for no key)		*/
	room_rnum to_room;		/* Where direction leads (NOWHERE)	*/
	bool contract;
	room_direction_data& operator=(room_direction_data& other);
	room_direction_data(const room_direction_data& other); // copy constructor
};

#endif
