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
#include "mods/debug.hpp"
#include "spells.h" // for TYPE_HIT
#include "mods/object-utils.hpp"
#include "mods/rooms.hpp"
#include "mods/orm/inventory.hpp"
extern void point_update(void);

ACMD(do_get_ticks_per_minute){
	player->send("[%d] ticks per minute\r\n",mods::globals::defer_queue->get_ticks_per_minute());
	player->send("[%d] affects processer ticks per minute\r\n",mods::affects::get_ticks_per_minute());
}

ACMD(do_flush_player){
	auto status = mods::orm::inventory::flush_player(player);
	player->send("[%d] status\r\n",status);
}
ACMD(do_feed_player){
	auto status = mods::orm::inventory::feed_player(player);
	player->send("[%d] status\r\n",status);
}
ACMD(do_givemenades){

}

ACMD(do_room_dark){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0){
		player->sendln("usage: room_dark <on|off>");
		return;
	}
	if(vec_args[0].compare("on") == 0){
		mods::rooms::gods::force_dark(player->room());
	}else{
		mods::rooms::gods::remove_dark(player->room());
	}
}

/**
 * @brief if arg1 is on, sets fire to room. optionally arg2 is used to set to specific fire_status_t string representation
 *
 * @param do_room_fire
 * @param on|off
 * @param level optional fire_status_t
 */
ACMD(do_room_fire){
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0){
		player->sendln(
				"usage: room_fire <on|off> [level]\r\n"
				"level is one of:\r\n"
				"KINDLING, COMPLETELY_ON_FIRE, SMOLDERING, SMOKING, OUT, NONE\r\n"
				"This documentation was generated on 2020-06-08\r\n"
		);
		return;
	}
	bool on = false;
	if(vec_args[0].compare("on") == 0){
		on = true;
	}
	std::string level = "";
	if(vec_args.size() == 2){
		level = vec_args[1];
	}
	mods::rooms::gods::set_fire(player->room(),on,level,player);
}


ACMD(do_giveme_camera) {
	auto obj = mods::object_utils::yaml_import("gadget","camera.yml");
	obj_to_char(obj,player);
}
ACMD(do_giveme_night_vision_camera) {
	auto obj = mods::object_utils::yaml_import("gadget","night-vision-camera.yml");
	obj_to_char(obj,player);
}
ACMD(do_giveme_thermal_camera) {
	auto obj = mods::object_utils::yaml_import("gadget","thermal-camera.yml");
	obj_to_char(obj,player);
}

/**
 * @brief makes whoever you are fighting get so hurt that the next hit is fatal
 *
 * @param do_one_punch
 */
ACMD(do_one_punch){
	if(FIGHTING(ch)){
		auto victim = FIGHTING(ch);
		damage(ch, victim, GET_HIT(victim), TYPE_HIT);
	}
}

/**
 * @brief sets ammo of weapon identified by arg1 to arg2
 *
 * @param do_set_ammo
 */
ACMD(do_set_ammo) {
	obj_data* obj = nullptr;
	char_data* dummy = nullptr;
	int bits = 0;
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2){
		player->sendln("usage: set_ammo <weapon> <number>");
		return;
	}
	if(!(bits = generic_find(argument, FIND_OBJ_INV | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
		player->send("There doesn't seem to be %s %s here.\r\n", AN(argument), argument);
		return;
	}
	int ammo = mods::util::stoi(vec_args[1]).value_or(-1);
	if(ammo < 0){
		player->sendln("invalid number");
		return;
	}
	obj->obj_flags.ammo = ammo;
	player->send("Set %s ammo count to %d\r\n",obj->name.c_str(),ammo);
}

ACMD(do_giveme_frag_grenades) {
	
	auto obj = mods::weapon::new_frag_grenade_object();
	obj_to_char(obj,player);
}
ACMD(do_giveme_incendiary_grenades) {
	
	auto obj = mods::weapon::new_incendiary_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_emp_grenades) {
	
	auto obj = mods::weapon::new_emp_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_smoke_grenades) {
	
	auto obj = mods::weapon::new_smoke_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_sensor_grenades) {
	auto obj = mods::object_utils::yaml_import("explosive","sensor_grenade.yml");
	obj_to_char(obj,player);
}

ACMD(do_giveme_flashbang_grenades) {
	
	auto obj = mods::weapon::new_flashbang_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_sniper_rifle) {
	
	if(player->cl_sniper() == nullptr) {
		player->set_class(CLASS_SNIPER);
	}
	obj_to_char(player->sniper_rifle(),player);
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


ACMD(do_show_tics) {
	auto state = mods::debug::debug_state->show_tics();
	player->send("Toggling %s\r\n", state ? "{red}off{/red}" : "{grn}on{/grn}");
	mods::debug::debug_state->show_tics(!state);
}
