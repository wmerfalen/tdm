/* ************************************************************************
*   File: act.offensive.c                               Part of CircleMUD *
*  Usage: player-level commands of an offensive nature                    *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "types.hpp"
#include "utils.h"
#include "comm.h"
#include "interpreter.h"
#include "handler.h"
#include "db.h"
#include "spells.h"
#include <array>
#include "mods/util.hpp"
#include "globals.hpp"
#include "mods/scan.hpp"
#include "mods/weapon.hpp"
#include "mods/weapons/sniper-rifle.hpp"
#include "globals.hpp"
#include "mods/util.hpp"
#include "mods/prefs.hpp"

/* extern variables */
extern int pk_allowed;
extern void three_arguments(char*,char*,char*,char*);
extern void die(struct char_data*,struct char_data*);
extern mods::globals::room_list_t mods::globals::room_list;
/* extern functions */
void raw_kill(struct char_data *ch);
void check_killer(struct char_data *ch, struct char_data *vict);
int compute_armor_class(struct char_data *ch);
int snipe_hit(struct char_data*,struct char_data*,int,uint16_t);

/* using directives */
using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;

/* local functions */
ACMD(do_assist);
ACMD(do_hit);
ACMD(do_kill);
ACMD(do_backstab);
ACMD(do_order);
ACMD(do_flee);
ACMD(do_bash);
ACMD(do_rescue);
ACMD(do_kick);


/* Debugging type stuff */
ACMD(do_rnum);

/* Military actions */
ACMD(do_scan);
ACMD(do_breach);

/* explosives */
ACMD(do_plant);
ACMD(do_grenade);

/* timed devices, misc, other */
ACMD(do_activate);


ACMD(do_rnum) {
	send_to_char(ch, std::to_string(ch->in_room).c_str());
}
ACMD(do_ammo) {
	MENTOC_PREAMBLE();
	if(player->weapon()){
		player->weapon()->ammo += 12;
	}
}

using vpd = mods::scan::vec_player_data;
/* Arguments:
 * throw <direction> <count>
 */
ACMD(do_throw) {
	MENTOC_PREAMBLE();

	if(!player->has_inventory_capability(mods::weapon::mask::grenade)) {
		player->stc("You must be holding a grenade to do that!");
		return;
	}

	const char* usage = "usage: throw <direction> <room_count>\r\n"
		"example: \r\n"
		" $ get frag backpack\r\n"
		" $ hold frag\r\n"
		" $ throw frag north 2\r\n"
		" This will throw a frag 2 rooms away\n"
		" NOTE:\r\n"
		"All grenades are thrown as far as they can up to a maximum amount of 4 rooms away\r\n"
		"or however many rooms before it reaches a dead-end\r\n"
		"see: help grenade\r\n\r\n";
	std::array<char,MAX_INPUT_LENGTH> item;
	std::array<char,MAX_INPUT_LENGTH> direction;
	std::array<char,MAX_INPUT_LENGTH> count;
	std::fill(item.begin(),item.end(),0);
	std::fill(direction.begin(),direction.end(),0);
	std::fill(count.begin(),count.end(),0);

		skip_spaces(&argument);

		if(!*argument) {
			player->stc("Command not recognized. see: type 'throw usage' or 'help grenade'\r\n");
			return;
		}

		three_arguments(argument, &item[0], &direction[0], &count[0]);

	int cnt = atoi(static_cast<const char*>(&count[0]));

	player->stc(std::to_string(cnt));
	if(!IS_DIRECTION(static_cast<const char*>(&direction[0])) || cnt <= 0) {
		player->stc(usage);
		return;
	}

	if(cnt > 4) {
		player->stc("But you can only throw up to 4 rooms away!\r\n");
		return;
	}

	auto dir = NORTH;
	auto str_dir = mods::projectile::todirstr(static_cast<const char*>(&direction[0]),1,0);

	auto held_object = player->equipment(WEAR_HOLD);
	if(!held_object) {
		player->stc("You're not holding anything!");
		return;
	}

	/** If the grenade is a flashbang, we have a shorter timer on it */
	//std::array<bool,>
	int ticks = 0;
	/** temporarily do this just for debugging FIXME */
	std::string object_name = "";
	switch(held_object->explosive()->type) {
		default:
		case mw_explosive::REMOTE_EXPLOSIVE:
		case mw_explosive::REMOTE_CHEMICAL:
		case mw_explosive::CLAYMORE_MINE:
		case mw_explosive::EXPLOSIVE_NONE:
			player->stc("This type of explosive is not throwable!");
			return;
			break;
		case mw_explosive::FRAG_GRENADE:
			ticks = 2;
			object_name = "frag grenade";
			break;
		case mw_explosive::INCENDIARY_GRENADE:
			object_name = "incendiary grenade";
			ticks = 2;
			break;
		case mw_explosive::EMP_GRENADE:
			object_name = "emp grenade";
			ticks = 2;
			break;
		case mw_explosive::SMOKE_GRENADE:
			object_name = "smoke grenade";
			ticks = 3;
			break;
		case mw_explosive::FLASHBANG_GRENADE:
			object_name = "flashbang grenade";
			ticks = 1;
			break;
	}
	/* Resolve cnt rooms in direction.*/
	player->unequip(WEAR_HOLD);
	if(held_object->explosive()->name.length() == 0) {
		held_object->explosive()->name = object_name;
	}
	player->stc("You lob a " + held_object->explosive()->name + str_dir);
	auto room_id = mods::projectile::cast_finite(ch,IN_ROOM(ch),dir,cnt,held_object);
	mods::globals::defer_queue->push(ticks, [room_id,&held_object]() {
		for(auto & person : mods::globals::room_list[room_id]) {
			mods::projectile::grenade_damage(person, held_object);
		}
	});

}

ACMD(do_giveme_frag_grenades) {
	MENTOC_PREAMBLE();
	auto obj = mods::weapon::new_frag_grenade_object();
	obj_to_char(obj,player);
}
ACMD(do_giveme_incendiary_grenades) {
	MENTOC_PREAMBLE();
	auto obj = mods::weapon::new_incendiary_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_emp_grenades) {
	MENTOC_PREAMBLE();
	auto obj = mods::weapon::new_emp_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_smoke_grenades) {
	MENTOC_PREAMBLE();
	auto obj = mods::weapon::new_smoke_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_flashbang_grenades) {
	MENTOC_PREAMBLE();
	auto obj = mods::weapon::new_flashbang_grenade_object();
	obj_to_char(obj,player);
}

ACMD(do_giveme_sniper_rifle) {
	MENTOC_PREAMBLE();
	if(player->cl_sniper() == nullptr) {
		player->set_class(CLASS_SNIPER);
	}
	obj_to_char(player->sniper_rifle().get(),player->cd());

}


enum weapon_status_t {
	COOLDOWN_IN_EFFECT = -1,
	OUT_OF_AMMO = -2,
	INVALID_WEAPON_TYPE = -3,
	NOT_WIELDING_WEAPON = -4,
	OKAY = 0
};
weapon_status_t weapon_preamble(
		player_ptr_t player,
		weapon_rifle_t rifle_type){
	if(!player->weapon()){
		player->sendln("You must be wielding a " + mods::weapon::to_string(rifle_type) + " to do that!");
		return NOT_WIELDING_WEAPON;
	}
	if(!player->weapon_cooldown_expired(0)){
		return COOLDOWN_IN_EFFECT;
	}
	/* Check ammo */
	if(mods::weapon::has_clip(player->weapon()) && player->weapon()->ammo <= 0) {
		*player << "{gld}*CLICK*{/gld} Your weapon is out of ammo!\r\n";
		return OUT_OF_AMMO;
	}
	if(!player->has_weapon_capability(rifle_type)){
		return INVALID_WEAPON_TYPE;
	}
	return OKAY;
}

ACMD(do_snipe) {
	MENTOC_PREAMBLE();
	if(OKAY != weapon_preamble(player,weapon_rifle_t::SNIPER)){
		return;
	}

	constexpr unsigned int max_char = 16;
	std::array<char,max_char> victim;
	std::fill(victim.begin(),victim.end(),0);
	vpd scan;
	one_argument(argument,(char*)&victim[0],max_char);

	if(!victim[0]) {
		send_to_char(ch, "Whom do you wish to snipe?\r\n");
		return;
	}

	/* HOWTO: perform line of sight scans */
	mods::scan::los_scan(ch,3,&scan);

	for(auto scanned_target : scan) {
		/* HOWTO: fuzzy match two strings */
		if(mods::util::fuzzy_match(static_cast<char*>(&victim[0]),scanned_target.ch->player.name.c_str())) {
			/* HOWTO: deal damage with a sniper rifle */
			snipe_hit(ch, scanned_target.ch, TYPE_SNIPE,scanned_target.distance);
			/* HOWTO: adjust ammo of player's current weapon */
			player->ammo_adjustment(-1);
			player->weapon_cooldown_start(3,0);

			//mods::globals::room_event(ch,mods::ai_state::AI_WITNESS_ATTACK);
			/*
			 * HOWTO: defer an action N seconds in the future
			 mods::globals::defer_queue->push_secs(3,[&](){
			 });
			 */
			return;
		} else {
			*player << "You can't find your target!\r\n";
		}
	}
}

ACMD(do_silencers_on) {

}

ACMD(do_silencers_off) {

}

/** Alias of silencers off */
ACMD(do_go_loud) {

}

ACMD(do_engagement_mode) {

}

ACMD(do_regroup) {

}

/** 
 *
 */
ACMD(do_command_sequence) {
	MENTOC_PREAMBLE();
	//constexpr unsigned int max_char = 5;
	//std::array<char,max_char> direction;
	//one_argument(argument,&direction[0],max_char);

	/** breach and clear */
	/** frag and clear */
	/** flash and clear */
	/** smoke and clear */
	/** thermals on */
}

ACMD(do_breach) {
	MENTOC_PREAMBLE();
	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	if(!argument || ! IS_DIRECTION((char*)&direction[0])) {
		*player << "usage: breach <direction>\r\n";
		return;
	}

	*player << "You place a breach charge on the door...\r\n";
	mods::ai_state::event_type_t dir;
	int door = NORTH;

	if(strcmp(&direction[0],"north") == 0) {
		dir = mods::ai_state::BREACHED_NORTH;
		door = NORTH;
	}

	if(strcmp(&direction[0],"south") == 0) {
		dir = mods::ai_state::BREACHED_SOUTH;
		door = SOUTH;
	}

	if(strcmp(&direction[0],"east") == 0) {
		dir = mods::ai_state::BREACHED_EAST;
		door = EAST;
	}

	if(strcmp(&direction[0],"west") == 0) {
		dir = mods::ai_state::BREACHED_WEST;
		door = WEST;
	}

	auto room = IN_ROOM(ch);

	mods::globals::defer_queue->push_secs(3,[room,dir,door]() {
			mods::globals::room_event(room,dir);
			SET_BIT(world[room].dir_option[door]->exit_info,EX_BREACHED);

			if(dir == NORTH) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[SOUTH]->exit_info,EX_BREACHED);
			}

			if(dir == SOUTH) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[NORTH]->exit_info,EX_BREACHED);
			}

			if(dir == EAST) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[WEST]->exit_info,EX_BREACHED);
			}

			if(dir == WEST) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[EAST]->exit_info,EX_BREACHED);
			}
			});

}

ACMD(do_thermite) {
	MENTOC_PREAMBLE();

	if(!player->has_thermite()) {
		*player << "You do not have a thermite charge\r\n";
		return;
	}

	constexpr unsigned int max_char = 5;
	std::array<char,max_char> direction;
	one_argument(argument,&direction[0],max_char);

	if(!argument || ! IS_DIRECTION((char*)&direction[0])) {
		*player << "usage: thermite <direction>\r\n";
		return;
	}

	*player << "You place a {red}thermite{/red} breach charge on the door...\r\n";
	mods::ai_state::event_type_t dir;
	int door = NORTH;

	if(strcmp(&direction[0],"north") == 0) {
		dir = mods::ai_state::BREACHED_NORTH;
		door = NORTH;
	}

	if(strcmp(&direction[0],"south") == 0) {
		dir = mods::ai_state::BREACHED_SOUTH;
		door = SOUTH;
	}

	if(strcmp(&direction[0],"east") == 0) {
		dir = mods::ai_state::BREACHED_EAST;
		door = EAST;
	}

	if(strcmp(&direction[0],"west") == 0) {
		dir = mods::ai_state::BREACHED_WEST;
		door = WEST;
	}

	auto room = IN_ROOM(ch);

	mods::globals::defer_queue->push_secs(3,[room,dir,door]() {
			mods::globals::room_event(room,dir);
			SET_BIT(world[room].dir_option[door]->exit_info,EX_BREACHED);

			if(dir == NORTH) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[SOUTH]->exit_info,EX_BREACHED);
			}

			if(dir == SOUTH) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[NORTH]->exit_info,EX_BREACHED);
			}

			if(dir == EAST) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[WEST]->exit_info,EX_BREACHED);
			}

			if(dir == WEST) {
			SET_BIT(world[world[room].dir_option[door]->to_room].dir_option[EAST]->exit_info,EX_BREACHED);
			}
			});

}

/* TODO: Implement weapon tags in the obj_data data structure */
ACMD(do_reload) {
	MENTOC_PREAMBLE(); /* !mods */

	/* TODO get wielded equipment tag */
	if(!player->weapon()) {
		*player << "Invalid weapon to reload.\r\n";
		return;
	}

	if(player->weapon()->ammo > 0) {
		*player << "Weapon already loaded.\r\n";
		return;
	}

	if(!player->carrying_ammo_of_type(player->weapon()->type)) {
		*player << "{1} You don't have any ammo.\r\n";
		return;
	}

	if(player->weapon()->ammo -12 < 0) {
		auto difference = player->weapon()->ammo -12;
		auto rounds = difference + 12;

		if(rounds <= 0) {
			*player << "Out of ammo.\r\n";
			player->weapon()->ammo = 0;
			return;
		}

		*player << "You load " << rounds << " rounds into " << player->weapon_name() << "\r\n";
		player->weapon()->ammo = 0;
		player->weapon()->ammo = rounds;
		return;
	} else {
		player->weapon()->ammo -= 12;
		player->weapon()->ammo = 12;
		*player << "You load 12 rounds into " << player->weapon_name() << "\r\n";
		return;
	}
}

ACMD(do_scan) { /* !mods */
	vpd scan;
	mods::scan::los_scan_foreach(ch,3,[ch](room_rnum _room_id,int _dir,vpd _ele) -> bool {
			for(auto e : _ele) {
			std::string line;
			line += "You see {grn}";

			if(IS_NPC(e.ch)) {
			line += e.ch->player.short_descr.c_str();
			} else {
			line += e.ch->player.name.c_str();
			}

			line += "{/grn}";

			switch(e.distance) {
			case 0:
			line += " close by";
			break;

			case 1:
			break;

			case 2:
			line += " far off";
			break;

			default:
			line += "<>";
			break;
			}

			if(_dir != UP && _dir != DOWN) {
				line += " to the ";
			}

			switch(_dir) {
				case NORTH:
					line += "north";
					break;

				case SOUTH:
					line += "south";
					break;

				case EAST:
					line += "east";
					break;

				case WEST:
					line += "west";
					break;

				case UP:
					line += " above you";
					break;

				case DOWN:
					line += " below you";
					break;
			}

			line += "\r\n";
			send_to_char(ch,line.c_str());
			}
			return true;	/** true means keep iterating */
	});

}


ACMD(do_assist) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *helpee, *opponent;

	if(FIGHTING(ch)) {
		send_to_char(ch, "You're already fighting!  How can you assist someone else?\r\n");
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Whom do you wish to assist?\r\n");
	} else if(!(helpee = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "%s", NOPERSON);
	} else if(helpee == ch) {
		send_to_char(ch, "You can't help yourself any more than this!\r\n");
	} else {
		/*
		 * Hit the same enemy the person you're helping is.
		 */
		if(FIGHTING(helpee)) {
			opponent = FIGHTING(helpee);
		} else
			for(opponent = world[IN_ROOM(ch)].people;
					opponent && (FIGHTING(opponent) != helpee);
					opponent = opponent->next_in_room)
				;

		if(!opponent) {
			act("But nobody is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
		} else if(!CAN_SEE(ch, opponent)) {
			act("You can't see who is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
		} else if(!pk_allowed && !IS_NPC(opponent))	/* prevent accidental pkill */
			act("Use 'murder' if you really want to attack $N.", FALSE,
					ch, 0, opponent, TO_CHAR);
		else {
			send_to_char(ch, "You join the fight!\r\n");
			act("$N assists you!", 0, helpee, 0, ch, TO_CHAR);
			act("$n assists $N.", FALSE, ch, 0, helpee, TO_NOTVICT);
			hit(ch, opponent, TYPE_UNDEFINED);
		}
	}
}


ACMD(do_hit) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *vict;

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Hit who?\r\n");
	} else if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "They don't seem to be here.\r\n");
	} else if(vict == ch) {
		send_to_char(ch, "You hit yourself...OUCH!.\r\n");
		act("$n hits $mself, and says OUCH!", FALSE, ch, 0, vict, TO_ROOM);
	} else if(AFF_FLAGGED(ch, AFF_CHARM) && (ch->master == vict)) {
		act("$N is just such a good friend, you simply can't hit $M.", FALSE, ch, 0, vict, TO_CHAR);
	} else {
		if(!pk_allowed) {
			if(!IS_NPC(vict) && !IS_NPC(ch)) {
				if(subcmd != SCMD_MURDER) {
					send_to_char(ch, "Use 'murder' to hit another player.\r\n");
					return;
				} else {
					check_killer(ch, vict);
				}
			}

			if(AFF_FLAGGED(ch, AFF_CHARM) && !IS_NPC(ch->master) && !IS_NPC(vict)) {
				return;
			}			/* you can't order a charmed pet to attack a

						 * player */
		}

		if((GET_POS(ch) == POS_STANDING) && (vict != FIGHTING(ch))) {
			hit(ch, vict, TYPE_UNDEFINED);
			WAIT_STATE(ch, PULSE_VIOLENCE + 2);
		} else {
			send_to_char(ch, "You do the best you can!\r\n");
		}
	}
}



ACMD(do_kill) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *vict;

	if(GET_LEVEL(ch) < LVL_IMPL || IS_NPC(ch)) {
		do_hit(ch, argument, cmd, subcmd);
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Kill who?\r\n");
	} else {
		if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
			send_to_char(ch, "They aren't here.\r\n");
		} else if(ch == vict) {
			send_to_char(ch, "Your mother would be so sad.. :(\r\n");
		} else {
			act("You chop $M to pieces!  Ah!  The blood!", FALSE, ch, 0, vict, TO_CHAR);
			act("$N chops you to pieces!", FALSE, vict, 0, ch, TO_CHAR);
			act("$n brutally slays $N!", FALSE, ch, 0, vict, TO_NOTVICT);
			die(ch,vict);
			//raw_kill(vict);
		}
	}
}



ACMD(do_backstab) {
	char buf[MAX_INPUT_LENGTH];
	struct char_data *vict;
	int percent, prob;

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_BACKSTAB)) {
		send_to_char(ch, "You have no idea how to do that.\r\n");
		return;
	}

	one_argument(argument, buf);

	if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "Backstab who?\r\n");
		return;
	}

	if(vict == ch) {
		send_to_char(ch, "How can you sneak up on yourself?\r\n");
		return;
	}

	if(!GET_EQ(ch, WEAR_WIELD)) {
		send_to_char(ch, "You need to wield a weapon to make it a success.\r\n");
		return;
	}

	if(GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD), 3) != TYPE_PIERCE - TYPE_HIT) {
		send_to_char(ch, "Only piercing weapons can be used for backstabbing.\r\n");
		return;
	}

	if(FIGHTING(vict)) {
		send_to_char(ch, "You can't backstab a fighting person -- they're too alert!\r\n");
		return;
	}

	if(MOB_FLAGGED(vict, MOB_AWARE) && AWAKE(vict)) {
		act("You notice $N lunging at you!", FALSE, vict, 0, ch, TO_CHAR);
		act("$e notices you lunging at $m!", FALSE, vict, 0, ch, TO_VICT);
		act("$n notices $N lunging at $m!", FALSE, vict, 0, ch, TO_NOTVICT);
		hit(vict, ch, TYPE_UNDEFINED);
		return;
	}

	percent = rand_number(1, 101);	/* 101% is a complete failure */
	prob = GET_SKILL(ch, SKILL_BACKSTAB);

	if(AWAKE(vict) && (percent > prob)) {
		damage(ch, vict, 0, SKILL_BACKSTAB);
	} else {
		hit(ch, vict, SKILL_BACKSTAB);
	}

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
}


ACMD(do_order) {
	char name[MAX_INPUT_LENGTH], message[MAX_INPUT_LENGTH];
	bool found = FALSE;
	struct char_data *vict;
	struct follow_type *k;

	half_chop(argument, name, message);

	if(!*name || !*message) {
		send_to_char(ch, "Order who to do what?\r\n");
	} else if(!(vict = get_char_vis(ch, name, NULL, FIND_CHAR_ROOM)) && !is_abbrev(name, "followers")) {
		send_to_char(ch, "That person isn't here.\r\n");
	} else if(ch == vict) {
		send_to_char(ch, "You obviously suffer from skitzofrenia.\r\n");
	} else {
		if(AFF_FLAGGED(ch, AFF_CHARM)) {
			send_to_char(ch, "Your superior would not aprove of you giving orders.\r\n");
			return;
		}

		if(vict) {
			char buf[MAX_STRING_LENGTH];

			snprintf(buf, sizeof(buf), "$N orders you to '%s'", message);
			act(buf, FALSE, vict, 0, ch, TO_CHAR);
			act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

			if((vict->master != ch) || !AFF_FLAGGED(vict, AFF_CHARM)) {
				act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
			} else {
				send_to_char(ch, "%s", OK);
				command_interpreter(vict, message);
			}
		} else {			/* This is order "followers" */
			char buf[MAX_STRING_LENGTH];

			snprintf(buf, sizeof(buf), "$n issues the order '%s'.", message);
			act(buf, FALSE, ch, 0, 0, TO_ROOM);

			for(k = ch->followers; k; k = k->next) {
				if(IN_ROOM(ch) == IN_ROOM(k->follower))
					if(AFF_FLAGGED(k->follower, AFF_CHARM)) {
						found = TRUE;
						command_interpreter(k->follower, message);
					}
			}

			if(found) {
				send_to_char(ch, "%s", OK);
			} else {
				send_to_char(ch, "Nobody here is a loyal subject of yours!\r\n");
			}
		}
	}
}



ACMD(do_flee) {
	int i, attempt, loss;
	struct char_data *was_fighting;

	if(GET_POS(ch) < POS_FIGHTING) {
		send_to_char(ch, "You are in pretty bad shape, unable to flee!\r\n");
		return;
	}

	for(i = 0; i < 6; i++) {
		attempt = rand_number(0, NUM_OF_DIRS - 1);	/* Select a random direction */

		if(CAN_GO(ch, attempt) &&
				!ROOM_FLAGGED(EXIT(ch, attempt)->to_room, ROOM_DEATH)) {
			act("$n panics, and attempts to flee!", TRUE, ch, 0, 0, TO_ROOM);
			was_fighting = FIGHTING(ch);

			if(do_simple_move(ch, attempt, TRUE)) {
				send_to_char(ch, "You flee head over heels.\r\n");

				if(was_fighting && !IS_NPC(ch)) {
					loss = GET_MAX_HIT(was_fighting) - GET_HIT(was_fighting);
					loss *= GET_LEVEL(was_fighting);
					gain_exp(ch, -loss);
				}
			} else {
				act("$n tries to flee, but can't!", TRUE, ch, 0, 0, TO_ROOM);
			}

			return;
		}
	}

	send_to_char(ch, "PANIC!  You couldn't escape!\r\n");
}


ACMD(do_bash) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *vict;
	int percent, prob;

	one_argument(argument, arg);

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_BASH)) {
		send_to_char(ch, "You have no idea how.\r\n");
		return;
	}

	if(ROOM_FLAGGED(IN_ROOM(ch), ROOM_PEACEFUL)) {
		send_to_char(ch, "This room just has such a peaceful, easy feeling...\r\n");
		return;
	}

	if(!GET_EQ(ch, WEAR_WIELD)) {
		send_to_char(ch, "You need to wield a weapon to make it a success.\r\n");
		return;
	}

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		if(FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
			vict = FIGHTING(ch);
		} else {
			send_to_char(ch, "Bash who?\r\n");
			return;
		}
	}

	if(vict == ch) {
		send_to_char(ch, "Aren't we funny today...\r\n");
		return;
	}

	percent = rand_number(1, 101);	/* 101% is a complete failure */
	prob = GET_SKILL(ch, SKILL_BASH);

	if(MOB_FLAGGED(vict, MOB_NOBASH)) {
		percent = 101;
	}

	if(percent > prob) {
		damage(ch, vict, 0, SKILL_BASH);
		GET_POS(ch) = POS_SITTING;
	} else {
		/*
		 * If we bash a player and they wimp out, they will move to the previous
		 * room before we set them sitting.  If we try to set the victim sitting
		 * first to make sure they don't flee, then we can't bash them!  So now
		 * we only set them sitting if they didn't flee. -gg 9/21/98
		 */
		if(damage(ch, vict, 1, SKILL_BASH) > 0) {	/* -1 = dead, 0 = miss */
			WAIT_STATE(vict, PULSE_VIOLENCE);

			if(IN_ROOM(ch) == IN_ROOM(vict)) {
				GET_POS(vict) = POS_SITTING;
			}
		}
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}


ACMD(do_rescue) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *vict, *tmp_ch;
	int percent, prob;

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_RESCUE)) {
		send_to_char(ch, "You have no idea how to do that.\r\n");
		return;
	}

	one_argument(argument, arg);

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "Whom do you want to rescue?\r\n");
		return;
	}

	if(vict == ch) {
		send_to_char(ch, "What about fleeing instead?\r\n");
		return;
	}

	if(FIGHTING(ch) == vict) {
		send_to_char(ch, "How can you rescue someone you are trying to kill?\r\n");
		return;
	}

	for(tmp_ch = world[IN_ROOM(ch)].people; tmp_ch &&
			(FIGHTING(tmp_ch) != vict); tmp_ch = tmp_ch->next_in_room);

	if(!tmp_ch) {
		act("But nobody is fighting $M!", FALSE, ch, 0, vict, TO_CHAR);
		return;
	}

	percent = rand_number(1, 101);	/* 101% is a complete failure */
	prob = GET_SKILL(ch, SKILL_RESCUE);

	if(percent > prob) {
		send_to_char(ch, "You fail the rescue!\r\n");
		return;
	}

	send_to_char(ch, "Banzai!  To the rescue...\r\n");
	act("You are rescued by $N, you are confused!", FALSE, vict, 0, ch, TO_CHAR);
	act("$n heroically rescues $N!", FALSE, ch, 0, vict, TO_NOTVICT);

	if(FIGHTING(vict) == tmp_ch) {
		stop_fighting(vict);
	}

	if(FIGHTING(tmp_ch)) {
		stop_fighting(tmp_ch);
	}

	if(FIGHTING(ch)) {
		stop_fighting(ch);
	}

	set_fighting(ch, tmp_ch);
	set_fighting(tmp_ch, ch);

	WAIT_STATE(vict, 2 * PULSE_VIOLENCE);
}



ACMD(do_kick) {
	char arg[MAX_INPUT_LENGTH];
	struct char_data *vict;
	int percent, prob;

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_KICK)) {
		send_to_char(ch, "You have no idea how.\r\n");
		return;
	}

	one_argument(argument, arg);

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		if(FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
			vict = FIGHTING(ch);
		} else {
			send_to_char(ch, "Kick who?\r\n");
			return;
		}
	}

	if(vict == ch) {
		send_to_char(ch, "Aren't we funny today...\r\n");
		return;
	}

	/* 101% is a complete failure */
	percent = ((10 - (compute_armor_class(vict) / 10)) * 2) + rand_number(1, 101);
	prob = GET_SKILL(ch, SKILL_KICK);

	if(percent > prob) {
		damage(ch, vict, 0, SKILL_KICK);
	} else {
		damage(ch, vict, GET_LEVEL(ch) / 2, SKILL_KICK);
	}

	WAIT_STATE(ch, PULSE_VIOLENCE * 3);
}

/**
 * +----------+
 * C-4 Brain  |  
 * +----------+
 *
 * Triggers an explosion the size of one room
 *
 * Levels: 
 * 	[ 10 ] -> One room away
 * 	[ 25 ] -> Two rooms away
 * 		|
 * 		+----> [ decision point ]
 * 		               |
 * 		               |
 * 		  -------------+----------   [ A ] -> Extrasensory instinct 
 * 		  |         |            |   [ B ] -> Chance to injure
 * 		  |         |            |   [ C ] -> Siphon life force from victim
 * 		[ A ]     [ B ]        [ C ]
 *
 * 	[ 45 ]--> [ decision point ]
 *    |+-- [ A2 ] -> Custom shrapnel
 * 	  |+-- [ B2 ] -> Chance to disarm victim
 * 	  |+-- [ C2 ] -> 
 * 	    
 */
ACMD(c4_brain) {
	MENTOC_PREAMBLE();

}

