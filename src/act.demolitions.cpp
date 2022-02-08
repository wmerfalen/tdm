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
#include "mods/util.hpp"
#include "mods/item.hpp"
#include "mods/sensor-grenade.hpp"
#include "mods/object-utils.hpp"
#include "mods/player-utils.hpp"
#include "mods/doors.hpp"
#include "mods/rand.hpp"
#include "mods/demolitions.hpp"
#include "interpreter.h"
#include "mods/interpreter.hpp"


extern void obj_to_room(obj_ptr_t object, room_rnum room);
extern void obj_from_room(obj_ptr_t object);
/**
 * Description:
 * 'cancel' stops an installation of a device
 * 'cancel' stops
 *
 * Arguments:
 * cancel
 *
 */
/**
 * @brief stops a player from performing his/her blocking action
 *
 * @param do_cancel
 */
ACMD(do_cancel) {
	DO_HELP("cancel");
	if(!mods::player_utils::is_installing(player)) {
		player->sendln("Cancel what? You're not installing anything...");
		return;
	}

	std::string message = "You stop installing.";
	switch(player->current_block()) {
		case mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION:
			message = "You stop installing.";
			break;
		case mods::deferred::EVENT_PLAYER_UNBLOCK_BREACH:
			message = "You stop installing your breach charge.";
			break;
		case mods::deferred::EVENT_PLAYER_REVIVE_SUCCESSFUL:
			message = "You stop reviving...";
			break;
		case mods::deferred::EVENT_GET_ATTACKED:
		/** Purposely using fall-through behaviour */
		default:
			message = "You stop what you're doing.";
			break;
	}
	player->cancel_block();
	player->sendln(message);
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
	DO_HELP_WITH_ZERO("install");

	auto parsed = mods::util::parse_objdir(player,argument);
	if(!parsed.obj) {
		player->sendln("You don't have anything that matches that description");
		return;
	}
	if(parsed.dir < 0) {
		player->sendln("Use a valid direction");
		return;
	}
	auto& obj_name = parsed.obj->name;
	if(mods::object_utils::is_claymore(parsed.obj)) {
		mods::demolitions::plant_claymore(player,parsed.dir,parsed.obj);
		return;
	}

	if(mods::object_utils::is_camera(parsed.obj)) {
		parsed.obj->in_room = player->room();
		player->sendln(CAT("You begin installing a ", obj_name.c_str(), " onto the ", mods::globals::dir_to_str(parsed.dir, true).c_str(), " wall..."));
		mods::object_utils::set_is_installing(parsed.obj,player,parsed.dir);
		player->set_camera(parsed.obj);
		player->block_for(CAMERA_INSTALLATION_TICS(), mods::deferred::EVENT_PLAYER_UNBLOCK_INSTALLATION, parsed.obj->uuid);
		return;
	}

	player->sendln("You're not holding an item that can do that.");
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

	if(mods::util::parse_help(argument)) {
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
	auto room_parsed = mods::util::parse_owned_room_objdir(player,argument);
	if(!parsed.obj) {
		player->sendln("You don't have anything that matches that description");
		return;
	}
	if(parsed.dir < 0) {
		player->sendln("Use a valid direction");
		return;
	}
	auto& obj_name = parsed.obj->name;
	if(mods::object_utils::is_claymore(parsed.obj)) {
		if(!mods::object_utils::is_owner(parsed.obj,player)) {
			player->sendln("You don't own that device.");
			return;
		}
		/** TODO uninstall claymore and put back in inventory */
		player->sendln("[stub] remove claymore");
		return;
	}
	if(mods::object_utils::is_camera(parsed.obj)) {
		if(!mods::object_utils::is_owner(parsed.obj,player)) {
			player->sendln("You don't own that device.");
			return;
		}
		player->sendln(CAT("You begin removing a ", obj_name.c_str()," from the ", mods::globals::dir_to_str(parsed.dir, true), " wall..."));
		player->clear_camera();
		player->set_camera_viewing(false);
		parsed.obj->set_location_data(0);
		obj_from_room(parsed.obj);
		player->carry(parsed.obj);
		player->sendln(CAT("\r\nYou place a ", parsed.obj->name.c_str(), " into your inventory."));
		return;
	}
	player->sendln("You're not holding an item that can do that.");
}

ACMD(do_breach) {

	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	auto door = mods::globals::dir_int(direction[0]);
	if(!argument || door == -1) {
		*player << "usage: breach <direction>\r\n";
		return;
	}

	auto holding = player->equipment(WEAR_HOLD);
	if(!holding) {
		player->sendln("You must be holding a breach charge to do that.");
		return;
	}
	auto room = player->room();
	if(mods::doors::is_open(room,door)) {
		player->sendln("That door is already open.");
		return;
	}
	if(mods::doors::is_dir_electrified(room,door)) {
		player->sendln("You attempt to place the breach charge but notice that the door is {blu}ELECTRIFIED{/blu}!!!");
		auto dam = dice(16,6); /** TODO: need to calculate resistences and what not */
		GET_HIT(player->cd()) -= dam; /** FIXME ^ */
		player->sendln(CAT("{red}[", dam, "]{/red} you are {blu}ELECTROCUTED{/blu} by the door!"));
		return;
	}

	/** TODO move this to demolitions namespace */
	mods::doors::perform_breach(holding->uuid,player->uuid(),door);
}

ACMD(do_thermite) {
	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	auto door = mods::globals::dir_int(direction[0]);
	if(!argument || door == -1) {
		*player << "usage: thermite <direction>\r\n";
		return;
	}

	auto holding = player->equipment(WEAR_HOLD);
	if(!holding) {
		player->sendln("You must be holding a breach charge to do that.");
		return;
	}
	auto room = player->room();
	if(mods::doors::is_open(room,door)) {
		player->sendln("That door is already open.");
		return;
	}
	if(mods::doors::is_dir_electrified(room,door)) {
		player->sendln("You attempt to place the breach charge but notice that the door is {blu}ELECTRIFIED{/blu}!!!");
		auto dam = mods::rand::roll(16,6); /** TODO: need to calculate resistences and what not */
		GET_HIT(player->cd()) -= dam; /** FIXME ^ */
		player->sendln(CAT("{red}[",dam,"]{/red} you are {blu}ELECTROCUTED{/blu} by the door!"));
		return;
	}

	/** TODO move this to demolitions namespace */
	mods::doors::perform_thermite_breach(holding->uuid,player->uuid(),door);
}

namespace demolitions {
	void init() {
		mods::interpreter::add_command("plant", POS_RESTING, do_install, 0,0);
		mods::interpreter::add_command("install", POS_RESTING, do_install, 0,0);
		mods::interpreter::add_command("uninstall", POS_RESTING, do_uninstall, 0,0);
		mods::interpreter::add_command("cancel", POS_RESTING, do_cancel, 0,0);
	}
};

