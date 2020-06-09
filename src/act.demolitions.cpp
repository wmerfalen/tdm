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
	DO_HELP("cancel");
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
	DO_HELP("install");
	
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


extern void shop_view_item(player_ptr_t& player, int16_t item_number);
ACMD(do_view) {
	DO_HELP("view");
	/** If the user is attempting to view an item in a shop */
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() && !mods::util::icompare(vec_args[0], "camera")){
		int16_t item = mods::util::stoi(vec_args[0]).value_or(-1);
		shop_view_item(player,item);
		return;
	}
	if(vec_args.size() == 0){
		player->sendln("Usage: 'view camera'. For more information, type 'view help'");
		return;
	}
	if(mods::util::icompare(vec_args[0], "camera")) {
		if(!player->get_camera()){
			player->sendln("You haven't installed a camera");
			return;
		}
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

ACMD(do_breach) {
	
	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	auto door = mods::globals::dir_int(direction[0]);
	if(!argument || door == -1){
		*player << "usage: breach <direction>\r\n";
		return;
	}

	auto holding = player->equipment(WEAR_HOLD);
	if(!holding){
		player->sendln("You must be holding a breach charge to do that.");
		return;
	}
	auto room = player->room();
	if(mods::doors::is_open(room,door)){
		player->sendln("That door is already open.");
		return;
	}
	if(mods::doors::is_dir_electrified(room,door)){
		player->sendln("You attempt to place the breach charge but notice that the door is {blu}ELECTRIFIED{/blu}!!!");
		auto dam = mods::rand::roll(16,6); /** TODO: need to calculate resistences and what not */
		GET_HIT(player->cd()) -= dam; /** FIXME ^ */
		player->send("{red}[%d]{/red} you are {blu}ELECTROCUTED{/blu} by the door!",dam);
		return;
	}

	mods::doors::perform_breach(holding->uuid,player->uuid(),door);
}

ACMD(do_thermite) {
	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	auto door = mods::globals::dir_int(direction[0]);
	if(!argument || door == -1){
		*player << "usage: thermite <direction>\r\n";
		return;
	}

	auto holding = player->equipment(WEAR_HOLD);
	if(!holding){
		player->sendln("You must be holding a breach charge to do that.");
		return;
	}
	auto room = player->room();
	if(mods::doors::is_open(room,door)){
		player->sendln("That door is already open.");
		return;
	}
	if(mods::doors::is_dir_electrified(room,door)){
		player->sendln("You attempt to place the breach charge but notice that the door is {blu}ELECTRIFIED{/blu}!!!");
		auto dam = mods::rand::roll(16,6); /** TODO: need to calculate resistences and what not */
		GET_HIT(player->cd()) -= dam; /** FIXME ^ */
		player->send("{red}[%d]{/red} you are {blu}ELECTROCUTED{/blu} by the door!",dam);
		return;
	}

	mods::doors::perform_thermite_breach(holding->uuid,player->uuid(),door);
}
