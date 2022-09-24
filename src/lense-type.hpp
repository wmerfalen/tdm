/* ************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#ifndef __MENTOC_LENSE_TYPE_HEADER__
#define __MENTOC_LENSE_TYPE_HEADER__

enum lense_type_t {
	FIRST,

	NORMAL_SIGHT,
	THERMAL_GOGGLES,
	NIGHT_VISION_GOGGLES,

	AERIAL_DRONE,
	AERIAL_DRONE_THERMAL,
	AERIAL_DRONE_NIGHT_VISION,

	RC_DRONE,
	RC_DRONE_THERMAL,
	RC_DRONE_NIGHT_VISION,

	LAST
};

#endif
