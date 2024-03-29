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
#include "mods/interpreter.hpp"

extern void point_update(void);
extern void check_idling(player_ptr_t player);
SUPERCMD(do_wait_ticks) {
	if(argshave()->size_gt(1)->nth_has_integer(0)->passed() == false) {
		player->sendln("usage: wait_ticks <ticks> <message>");
		return;
	}
	const auto uuid = player->uuid();
	const auto msg = argat(1);
	mods::globals::defer_queue->push(intat(0),[=]() {
		auto p = ptr_by_uuid(uuid);
		p->sendln(msg);
	});
}
SUPERCMD(do_get_ticks_per_minute) {
	player->sendln(CAT("[",mods::globals::defer_queue->get_ticks_per_minute(),"] ticks per minute..."));
	player->sendln(CAT("[",mods::affects::get_ticks_per_minute(),"] affects processer ticks per minute"));
}

SUPERCMD(do_feed_player) {
	auto status = mods::orm::inventory::feed_player(player);
	player->sendln(CAT("[",status,"] status"));
}
SUPERCMD(do_givemenades) {

}

SUPERCMD(do_room_dark) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		player->sendln("usage: room_dark <on|off>");
		return;
	}
	if(vec_args[0].compare("on") == 0) {
		mods::rooms::gods::force_dark(player->room());
	} else {
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
SUPERCMD(do_room_fire) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		player->sendln(
		    "usage: room_fire <on|off> [level]\r\n"
		    "level is one of:\r\n"
		    "KINDLING, COMPLETELY_ON_FIRE, SMOLDERING, SMOKING, OUT, NONE\r\n"
		    "This documentation was generated on 2020-06-08\r\n"
		);
		return;
	}
	bool on = false;
	if(vec_args[0].compare("on") == 0) {
		on = true;
	}
	std::string level = "";
	if(vec_args.size() == 2) {
		level = vec_args[1];
	}
	mods::rooms::gods::set_fire(player->room(),on,level,player);
}


SUPERCMD(do_giveme_camera) {
	auto obj = create_object(ITEM_GADGET,"camera.yml");
	obj_to_char(obj,player);
}
SUPERCMD(do_giveme_night_vision_camera) {
	auto obj = create_object(ITEM_GADGET,"night-vision-camera.yml");
	obj_to_char(obj,player);
}
SUPERCMD(do_giveme_thermal_camera) {
	auto obj = create_object(ITEM_GADGET,"thermal-camera.yml");
	obj_to_char(obj,player);
}

/**
 * @brief makes whoever you are fighting get so hurt that the next hit is fatal
 *
 * @param do_one_punch
 */
SUPERCMD(do_one_punch) {
	if(FIGHTING(ch)) {
		auto victim = FIGHTING(ch);
		damage(ch, victim, GET_HIT(victim), TYPE_HIT);
	}
}

/**
 * @brief sets ammo of weapon identified by arg1 to arg2
 *
 * @param do_set_ammo
 */
SUPERCMD(do_set_ammo) {
	obj_data* obj = nullptr;
	char_data* dummy = nullptr;
	int bits = 0;
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2) {
		player->sendln("usage: set_ammo <weapon> <number>");
		return;
	}
	if(!(bits = generic_find(argument, FIND_OBJ_INV | FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
		player->sendln(CAT("There doesn't seem to be ",AN(argument), " ",argument," here."));
		return;
	}
	int ammo = mods::util::stoi(vec_args[1]).value_or(-1);
	if(ammo < 0) {
		player->sendln("invalid number");
		return;
	}
	obj->obj_flags.ammo = ammo;
	player->sendln(CAT("Set ",obj->name.c_str()," ammo count to ",ammo));
}

SUPERCMD(do_giveme_frag_grenades) {

	auto obj = mods::weapon::new_frag_grenade_object();
	obj_to_char(obj,player);
}
SUPERCMD(do_giveme_incendiary_grenades) {

	auto obj = mods::weapon::new_incendiary_grenade_object();
	obj_to_char(obj,player);
}

SUPERCMD(do_giveme_emp_grenades) {

	auto obj = mods::weapon::new_emp_grenade_object();
	obj_to_char(obj,player);
}

SUPERCMD(do_giveme_smoke_grenades) {

	auto obj = mods::weapon::new_smoke_grenade_object();
	obj_to_char(obj,player);
}

SUPERCMD(do_giveme_sensor_grenades) {
	auto obj = create_object(ITEM_EXPLOSIVE,"sensor-grenade.yml");
	obj_to_char(obj,player);
}

SUPERCMD(do_giveme_flashbang_grenades) {

	auto obj = mods::weapon::new_flashbang_grenade_object();
	obj_to_char(obj,player);
}

SUPERCMD(do_giveme_sniper_rifle) {
	auto obj = create_object(ITEM_RIFLE,"psg1.yml");
	obj_to_char(obj,player);
}

SUPERCMD(do_point_update) {

	if(IS_NPC(ch)) {
		return;
	}
	player->stc("[debug]->Running point_update");
	point_update();
	player->stc("[debug]->done");
}
SUPERCMD(do_room_list_uuid) {
	player->sendln("Beginning room list...");
	for(auto& p : room_list(player->room())) {
		player->sendln(CAT("uuid: [", p->uuid()," ] name: '",p->name(),"'"));
	}
	player->sendln("Done listing.");
}
SUPERCMD(do_send_report) {
	player->sendln("Beginning report...");
	for(auto& p : room_list(player->room())) {
		p->report();
	}
	player->sendln("Done reporting.");
}
SUPERCMD(do_set_npc_position) {
	DO_HELP("set_npc_position");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2) {
		player->sendln("not enough arguments");
		return;
	}
	auto uuid = mods::util::stoi_optional<uuid_t>(vec_args[0]);
	if(uuid.value_or(-1) < 0) {
		player->sendln("first argument needs to be a uuid. unknown value encountered");
		return;
	}
	auto target = ptr_opt(uuid.value());
	if(!target.has_value()) {
		player->sendln("unable to find anyone with that uuid");
		return;
	}
	player->sendln(CAT("player with that uuid: '", target.value()->name().c_str(),"'"));
#define MENTOC_LAZY_POS(a,b)\
	if(mods::util::is_lower_match(vec_args[1],#a)){\
		target.value()->position() = b;\
		player->sendln(CAT("{grn}Set position to: ",#a," on ",target.value()->name().c_str(),"{/grn}"));\
		return;\
	}
	MENTOC_LAZY_POS(DEAD,POS_DEAD);
	MENTOC_LAZY_POS(MORTALLY,POS_MORTALLYW);
	MENTOC_LAZY_POS(INCAP,POS_INCAP);
	MENTOC_LAZY_POS(STUNNED,POS_STUNNED);
	MENTOC_LAZY_POS(SLEEPING,POS_SLEEPING);
	MENTOC_LAZY_POS(RESTING,POS_RESTING);
	MENTOC_LAZY_POS(SITTING,POS_SITTING);
	MENTOC_LAZY_POS(FIGHTING,POS_FIGHTING);
	MENTOC_LAZY_POS(STANDING,POS_STANDING);
#undef MENTOC_LAZY_POS
	player->sendln("Didn't set position. Make sure you used the correct string.");

}
extern int idle_void;
SUPERCMD(do_set_position) {
	if(IS_NPC(ch)) {
		return;
	}
	DO_HELP("set_position");
	auto vec_args = PARSE_ARGS();
#define MENTOC_LAZY_POS(a,b)\
	if(mods::util::is_lower_match(vec_args[0],#a)){\
		player->position() = b;\
		player->sendln(CAT("{grn}Set your position to: ",#a,"{/grn}"));\
		return;\
	}
	MENTOC_LAZY_POS(DEAD,POS_DEAD);
	MENTOC_LAZY_POS(MORTALLY,POS_MORTALLYW);
	MENTOC_LAZY_POS(INCAP,POS_INCAP);
	MENTOC_LAZY_POS(STUNNED,POS_STUNNED);
	MENTOC_LAZY_POS(SLEEPING,POS_SLEEPING);
	MENTOC_LAZY_POS(RESTING,POS_RESTING);
	MENTOC_LAZY_POS(SITTING,POS_SITTING);
	MENTOC_LAZY_POS(FIGHTING,POS_FIGHTING);
	if(mods::util::is_lower_match(vec_args[0],"IDLE") || mods::util::is_lower_match(vec_args[0],"CON_IDLE")) {
		player->char_specials().timer = idle_void + 1;
		check_idling(player);
		player->set_state(CON_IDLE);
		player->cd()->desc->set_state(CON_IDLE);
		player->sendln("{grn}Set you to idle{/grn}.");
		return;
	}
#undef MENTOC_LAZY_POS
	player->sendln("Didn't set your position. Make sure you used the correct string.");

}


SUPERCMD(do_my_uuid) {

	player->stc(std::to_string(player->uuid()));
}
SUPERCMD(do_zero_socket) {

	player->stc("[debug]->Zeroing out your socket (goodbye).");
	player->desc().descriptor = 0;
}


SUPERCMD(do_show_tics) {
	auto state = mods::debug::debug_state->show_tics();
	player->sendln(CAT("Toggling ", state ? "{red}off{/red}" : "{grn}on{/grn}"));
	mods::debug::debug_state->show_tics(!state);
}

namespace src::act::debug {
	void init() {
		ADD_BUILDER_COMMAND("wait_ticks",do_wait_ticks);
	}
};
