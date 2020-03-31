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
#include "mods/object-utils.hpp"
#include "mods/player-utils.hpp"


extern void obj_to_room(obj_ptr_t object, room_rnum room);
extern void obj_from_room(obj_ptr_t object);
/**
 * Description:
 * 'cancel' stops an installation of a device
 * 
 * Arguments:
 * cancel
 * 
 */
ACMD(do_cancel) {
	MENTOC_PREAMBLE();
	if(mods::util::parse_help(argument)){
		player->pager_start();
		*player << 
			"description: the 'cancel' command is used to stop the install\r\n" <<
			"command. If you are installing a camera on the wall, it takes a\r\n" <<
			"certain amount of time before that process is done. In that time,\r\n" <<
			"you are vulnerable to attacks.\r\n" <<
			"\r\n" <<
			"For more information: see the help manual for the following keywords:\r\n" <<
			"'cancel','camera','claymore','install','uninstall'\r\n" <<
			"\r\n" << 
			"This documentation was written on 2020-03-29.\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(!mods::player_utils::is_installing(player)){
		player->sendln("Cancel what? You're not installing anything...");
		return;
	}
	player->cancel_block();
	player->sendln("You stop installing.");
}

/**
 * Description:
 * install works on claymore mines and cameras. In the future,
 * we'll be implementing this command for motion sensors as well.
 * 
 * Arguments:
 * install <claymore> <direction>
 *
 * install <camera> <face-direction>
 *
 *
 * 
 */
ACMD(do_install) {
	MENTOC_PREAMBLE();
	if(mods::util::parse_help(argument)){
		player->pager_start();
		*player << 
			"description: the 'install' command is used to install \r\n" <<
			"devices like cameras or claymore mines. To install a claymore\r\n" <<
			"mine, you would simply type 'install claymore north'. This would\r\n" <<
			"install the claymore to the north exit of the room. Any NPC that\r\n" <<
			"leaves or enters the current room through the northern exit will\r\n" <<
			"be met with an explosion.\r\n" <<
			"\r\n" <<
			"The other use case of the 'install' command is to install cameras\r\n" <<
			"onto a wall inside a room. To install a camera on the north side of\r\n" <<
			"the room, you would type 'install camera north'.\r\n" <<
			"\r\n" <<
			"To remove the device, use the 'uninstall' command\r\n" <<
			"Type 'help uninstall' for more information.\r\n" <<
			"\r\n" <<
			"To cancel the installation of a device while you are currently\r\n" <<
			"installing it, you must type 'cancel'\r\n" <<
			"\r\n" <<
			"For more information: see the help manual for the following keywords:\r\n" <<
			"'cancel','camera','claymore','install','uninstall'\r\n" <<
			"\r\n" << 
			"This documentation was written on 2020-03-29.\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	auto parsed = mods::util::parse_objdir(player,argument);
	if(!parsed.obj){
		player->sendln("You don't have anything that matches that description");
		return;
	}
	if(parsed.dir < 0){
		player->sendln("Use a valid direction");
		return;
	}
	auto & obj_name = parsed.obj->name;
	if(mods::object_utils::is_claymore(parsed.obj)) {
		player->sendln("[stub] Is claymore");
		player->send("You begin installing a %s at the foot of the %s entrance...\r\n", obj_name.c_str(), mods::globals::dir_to_str(parsed.dir, true).c_str());
		mods::object_utils::set_is_installing(parsed.obj,player,parsed.dir);
		player->block_for(30, mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, parsed.obj->uuid);
		return;
	}

	if(mods::object_utils::is_camera(parsed.obj)) {
		parsed.obj->in_room = player->room();
		player->send("You begin installing a %s onto the %s wall...\r\n", obj_name.c_str(), mods::globals::dir_to_str(parsed.dir, true).c_str());
		mods::object_utils::set_is_installing(parsed.obj,player,parsed.dir);
		player->set_camera(parsed.obj);
		player->block_for(30, mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, parsed.obj->uuid);
		return;
	}

	player->sendln("You're not holding an item that can do that.");
}


ACMD(do_view) {
	MENTOC_PREAMBLE();
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0){
		player->sendln("Usage: 'view camera'. For more information, type 'view help'");
		return;
	}
	if(mods::util::parse_help(argument)){
		player->pager_start();
		*player << 
			"description: the 'view' command is used to look through the eyes \r\n" <<
			"of a camera that has been installed.\r\n" <<
			"\r\n" <<
			"type 'view camera' and if you have a camera installed, you will see \r\n" <<
			"the contents of the room as if you were there.\r\n" <<
			"\r\n" << 
			"this documentation was written on 2020-03-25.\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	if(mods::util::icompare(vec_args[0], "camera")) {
		player->set_camera_viewing(true);
		look_at_room(ch,0);
		player->set_camera_viewing(false);
		return;
	}

}

/**
 * Description:
 * uninstall works on claymore mines and cameras.
 * 
 * Arguments:
 * uninstall <claymore> <direction>
 *
 * uninstall <camera> <face-direction>
 * 
 */
ACMD(do_uninstall) {
	MENTOC_PREAMBLE();
	if(mods::util::parse_help(argument)){
		player->pager_start();
		*player << 
			"description: the 'uninstall' command is used to uninstall \r\n" <<
			"devices like cameras or claymore mines.\r\n" <<
			"example:\r\n" <<
			"uninstall camera north\r\n" <<
			"\r\n" <<
			"For more information: see the help manual for camera\r\n" <<
			"For more information: see the help manual for claymore\r\n" <<
			"\r\n" << 
			"type 'help camera' or 'help claymore' for more information on\r\n" <<
			"those game dynamics.\r\n" <<
			"\r\n" <<
			"this documentation was written on 2020-03-26.\r\n" <<
			"\r\n";
		player->pager_end();
		player->page(0);
		return;
	}
	auto parsed = mods::util::parse_objdir(player,argument);
	if(!parsed.obj){
		player->sendln("You don't have anything that matches that description");
		return;
	}
	if(parsed.dir < 0){
		player->sendln("Use a valid direction");
		return;
	}
	auto & obj_name = parsed.obj->name;
	if(mods::object_utils::is_claymore(parsed.obj)) {
		if(!mods::object_utils::is_owner(parsed.obj,player)){
			player->sendln("You don't own that device.");
			return;
		}
		player->sendln("[stub] Is claymore");
		return;
	}
	if(mods::object_utils::is_camera(parsed.obj)) {
		if(!mods::object_utils::is_owner(parsed.obj,player)){
			player->sendln("You don't own that device.");
			return;
		}
		parsed.obj->in_room = -1;
		player->send("You begin removing a %s from the %s wall...", obj_name.c_str(), mods::globals::dir_to_str(parsed.dir, true).c_str());
		player->clear_camera();
		player->set_camera_viewing(false);
		parsed.obj->set_location_data(0);
		obj_from_room(parsed.obj);
		player->carry(parsed.obj);
		player->send("\r\nYou place a %s into your inventory.\r\n", parsed.obj->name.c_str());
		return;
	}
	player->sendln("You're not holding an item that can do that.");
}


