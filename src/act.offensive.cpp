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
#include "mods/weapons/sniper-rifle.hpp"
#include "globals.hpp"
#include "mods/util.hpp"
#include "mods/prefs.hpp"
#include "mods/affects.hpp"
#include "mods/object-utils.hpp"
#include "mods/player.hpp"
#include "mods/weapon.hpp"
#include "mods/help.hpp"
#include "mods/weapons/damage-types.hpp"
#include "mods/damage-event.hpp"
#include "mods/interpreter.hpp"
#include "mods/levels.hpp"
#include "mods/calc-visibility.hpp"
#include "mods/projectile.hpp"
#include "mods/combat-composer/includes.hpp"

using de = damage_event_t;
/* extern variables */
extern int pk_allowed;
extern void three_arguments(char*,char*,char*,char*);
namespace mods::weapons::damage_types::legacy {
	extern void die(char_data*,char_data*);
};
extern mods::globals::room_list_t mods::globals::room_list;
/* extern functions */
void check_killer(char_data *ch, char_data *vict);
int compute_armor_class(char_data *ch);

/* using directives */
using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;

using vpd = mods::scan::vec_player_data;

/* Arguments:
 * throw <object> <direction> <count>
 */
ACMD(do_throw) {
	auto vec_args = PARSE_ARGS();
	const char* usage = "Usage: throw <direction> <count>";

	if(!player->has_inventory_capability(mods::weapon::mask::grenade)) {
		player->error("You must be holding a grenade to do that!\r\n");
		return;
	}

	if(vec_args.size() < 2) {
		player->sendln(CAT(usage));
		return;
	}
	int cnt = mods::util::stoi(vec_args[1].c_str()).value_or(-1);
	if(cnt < 0) {
		player->errorln(CAT("How many rooms away do you want to throw this object?\r\n",usage));
		return;
	}
	auto dir = mods::projectile::to_direction(vec_args[0]);
	if(dir < 0) {
		player->errorln("Use a valid direction!");
		return;
	}

	auto held_object = player->equipment(WEAR_HOLD);
	if(!held_object) {
		player->errorln("You're not holding anything!");
		return;
	}
	mods::projectile::throw_object(player, dir, cnt, held_object, "lob");
}


SUPERCMD(do_affect_me) {

	constexpr unsigned int max_char = 16;
	static bool usage_set = false;
	static std::string usage;
	std::array<char,max_char> affect_type;
	std::fill(affect_type.begin(),affect_type.end(),0);
	one_argument(argument,(char*)&affect_type[0],max_char);

	if(!usage_set) {
		usage = "{gld}Usage: affect_me <";
		auto affect_names = affect_string_list();
		for(unsigned i=0; i < affect_names.size(); i++) {
			usage += affect_names[i];
			if(i+1 < affect_names.size()) {
				usage += "|";
			}
		}
		usage += ">{/gld}";
		usage_set = true;
	}
	std::string affect_type_str = (char*)&affect_type[0];
	if(!affect_type[0] || affect_type_str.compare("help") == 0) {
		player->sendln(usage);
		return;
	}
	str_queue_on_player({affect_type_str},player);
	player->sendln("It is done.");
};
ACMD(do_snipe_object) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2) {
		player->sendln("usage: snipe <object> <direction>");
		return;
	}

	int direction = mods::globals::dir_int(vec_args[1][0]);
	if(direction == -1) {
		player->sendln("Invalid direction");
		return;
	}
	mods::weapons::damage_types::rifle_attack_object_by_name(player,vec_args[0],direction);
}

ACMD(do_snipe) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 2) {
		player->sendln("usage: snipe <name> <direction>");
		return;
	}

	int direction = mods::globals::dir_int(vec_args[1][0]);
	if(direction == -1) {
		player->sendln("Invalid direction");
		return;
	}
	auto weapon = player->primary();
	if(!weapon || !weapon->has_rifle()) {
		player->damage_event(de::NO_PRIMARY_WIELDED_EVENT);
		return;
	}
	mods::combat_composer::snipe_target(player,vec_args[0], direction,0,weapon);
}

ACMD(do_spray) {
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1) {
		player->sendln("usage: spray <direction>");
		return;
	}

	int direction = mods::globals::dir_int(vec_args[0][0]);
	if(direction == -1) {
		player->sendln("Invalid direction");
		return;
	}
	auto weapon = player->primary();
	mods::combat_composer::spray_target(player,direction,weapon);
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

	//constexpr unsigned int max_char = 5;
	//std::array<char,max_char> direction;
	//one_argument(argument,&direction[0],max_char);

	/** breach and clear */
	/** frag and clear */
	/** flash and clear */
	/** smoke and clear */
	/** thermals on */
}



ACMD(do_scan) { /* !mods */
	vpd scan;
	mods::scan::los_scan(ch,mods::weapon::MAX_RANGE,&scan);
	for(auto e : scan) {
		auto found_player = ptr(e.ch);
		if(!found_player) {
			continue;
		}
		if(!mods::calc_visibility::is_visible(player,found_player,e.distance)) {
			continue;
		}
		std::string line;
		line += "You see {grn}";

		line += found_player->name().c_str();

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
			case 3:
			case 4:
				line += " quite a ways off";
				break;
			default:
				line += " way off";
				break;
		}

		if(e.direction != UP && e.direction != DOWN) {
			line += " to the ";
		}

		switch(e.direction) {
			default:
				line += "?";
				break;
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
		player->sendln(line);
	}

}


ACMD(do_assist) {

	char arg[MAX_INPUT_LENGTH];
	player_ptr_t helpee,opponent;

	if(player->fighting()) {
		player->psendln("You're already fighting!  How can you assist someone else?");
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		player->psendln("Whom do you wish to assist?");
	} else if(!(helpee = get_char_vis(player, arg, NULL, FIND_CHAR_ROOM))) {
		player->sendln(NOPERSON);
	} else if(helpee->is(ch)) {
		player->psendln("You can't help yourself any more than this!");
	} else {
		/*
		 * Hit the same enemy the person you're helping is.
		 */
		opponent = nullptr;
		if(helpee->fighting()) {
			opponent = helpee->fighting();
		} else {
			for(auto& plr : mods::globals::get_room_list(player->room())) {
				if(plr->fighting() == helpee) {
					opponent = plr;
					break;
				}
			}
		}

		if(!opponent) {
			act("But nobody is fighting $M!", FALSE, *player, 0, *helpee, TO_CHAR);
		} else if(!CAN_SEE(player->cd(), opponent->cd())) {
			act("You can't see who is fighting $M!", FALSE, *player, 0, *helpee, TO_CHAR);
#ifdef MENTOC_PK_ALLOWED
		} else if(!pk_allowed && !opponent->is_npc())	/* prevent accidental pkill */
			act("Use 'murder' if you really want to attack $N.", FALSE,
			    ch, 0, *opponent, TO_CHAR);
#endif
	} else {
		player->psendln("You join the fight!");
		act("$N assists you!", 0, *helpee, 0, *player, TO_CHAR);
		act("$n assists $N.", FALSE, ch, 0, *helpee, TO_NOTVICT);
		hit(*player, *opponent, TYPE_UNDEFINED);
	}
}
}


ACMD(do_hit) {

	char arg[MAX_INPUT_LENGTH];
	char_data *vict;

	one_argument(argument, arg);

	auto weapon = player->primary();
	if(!*arg) {
		send_to_char(ch, "Hit who?");
	} else if(weapon != nullptr && mods::combat_composer::engage_target(player,std::string(arg),weapon)) {
		/** Handled. We can return; */
		return;
	} else if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "They don't seem to be here.");
	} else if(vict == ch) {
		send_to_char(ch, "You hit yourself...OUCH!.");
		act("$n hits $mself, and says OUCH!", FALSE, ch, 0, vict, TO_ROOM);
	} else if(AFF_FLAGGED(ch, AFF_CHARM) && (ch->master == vict)) {
		act("$N is just such a good friend, you simply can't hit $M.", FALSE, ch, 0, vict, TO_CHAR);
	} else {
#ifdef MENTOC_PK_ALLOWED
		if(!pk_allowed) {
			if(!IS_NPC(vict) && !IS_NPC(ch)) {
				if(subcmd != SCMD_MURDER) {
					send_to_char(ch, "Use 'murder' to hit another player.");
					return;
				} else {
					check_killer(ch, vict);
				}
			}
			if(AFF_FLAGGED(ch, AFF_CHARM) && !IS_NPC(ch->master) && !IS_NPC(vict)) {
				return;
			}
		}
#endif

		if((GET_POS(ch) == POS_STANDING) && (vict != FIGHTING(ch))) {
			hit(ch, vict, TYPE_UNDEFINED);
			WAIT_STATE(ch, PULSE_VIOLENCE + 2);
		} else {
			send_to_char(ch, "You do the best you can!");
		}
	}
}



ACMD(do_kill) {
	char arg[MAX_INPUT_LENGTH];
	char_data *vict;

	if(GET_LEVEL(ch) < LVL_IMPL || IS_NPC(ch)) {
		do_hit(ch, argument, cmd, subcmd,player);
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		send_to_char(ch, "Kill who?");
	} else {
		if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
			send_to_char(ch, "They aren't here.");
		} else if(ch == vict) {
			send_to_char(ch, "Your mother would be so sad.. :(");
		} else {
			act("You chop $M to pieces!  Ah!  The blood!", FALSE, ch, 0, vict, TO_CHAR);
			act("$N chops you to pieces!", FALSE, vict, 0, ch, TO_CHAR);
			act("$n brutally slays $N!", FALSE, ch, 0, vict, TO_NOTVICT);
			mods::weapons::damage_types::legacy::die(ch,vict);
		}
	}
}



ACMD(do_backstab) {
	char buf[MAX_INPUT_LENGTH];
	char_data *vict;
	int percent, prob;

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_BACKSTAB)) {
		send_to_char(ch, "You have no idea how to do that.");
		return;
	}

	one_argument(argument, buf);

	if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "Backstab who?");
		return;
	}

	if(vict == ch) {
		send_to_char(ch, "How can you sneak up on yourself?");
		return;
	}

	if(!GET_EQ(ch, WEAR_WIELD)) {
		send_to_char(ch, "You need to wield a weapon to make it a success.");
		return;
	}

	if(GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD), 3) != TYPE_PIERCE - TYPE_HIT) {
		send_to_char(ch, "Only piercing weapons can be used for backstabbing.");
		return;
	}

	if(FIGHTING(vict)) {
		send_to_char(ch, "You can't backstab a fighting person -- they're too alert!");
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
	char_data *vict;
	struct follow_type *k;

	half_chop(argument, name, message);

	if(!*name || !*message) {
		send_to_char(ch, "Order who to do what?");
	} else if(!(vict = get_char_vis(ch, name, NULL, FIND_CHAR_ROOM)) && !is_abbrev(name, "followers")) {
		send_to_char(ch, "That person isn't here.");
	} else if(ch == vict) {
		send_to_char(ch, "You obviously suffer from skitzofrenia.");
	} else {
		if(AFF_FLAGGED(ch, AFF_CHARM)) {
			send_to_char(ch, "Your superior would not aprove of you giving orders.");
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
				auto v = ptr(vict);
				//FIXME command_interpreter(v, std::string(message));
			}
		} else {			/* This is order "followers" */
			char buf[MAX_STRING_LENGTH];

			snprintf(buf, sizeof(buf), "$n issues the order '%s'.", message);
			act(buf, FALSE, ch, 0, 0, TO_ROOM);

			for(k = ch->followers; k; k = k->next) {
				if(IN_ROOM(ch) == IN_ROOM(k->follower))
					if(AFF_FLAGGED(k->follower, AFF_CHARM)) {
						found = TRUE;
						auto kptr = ptr(k->follower);
						//FIXME command_interpreter(kptr, message);
					}
			}

			if(found) {
				send_to_char(ch, "%s", OK);
			} else {
				send_to_char(ch, "Nobody here is a loyal subject of yours!");
			}
		}
	}
}


ACMD(do_bash) {
	char arg[MAX_INPUT_LENGTH];
	char_data *vict;
	int percent, prob;

	one_argument(argument, arg);

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_BASH)) {
		send_to_char(ch, "You have no idea how.");
		return;
	}

	if(ROOM_FLAGGED(IN_ROOM(ch), ROOM_PEACEFUL)) {
		send_to_char(ch, "This room just has such a peaceful, easy feeling...");
		return;
	}

	if(!GET_EQ(ch, WEAR_WIELD)) {
		send_to_char(ch, "You need to wield a weapon to make it a success.");
		return;
	}

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		if(FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
			vict = FIGHTING(ch);
		} else {
			send_to_char(ch, "Bash who?");
			return;
		}
	}

	if(vict == ch) {
		send_to_char(ch, "Aren't we funny today...");
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
	//char_data *vict, *tmp_ch;
	int percent, prob;
	player_ptr_t vict,tmp_ch;

	if(player->is_npc() || !GET_SKILL(ch, SKILL_RESCUE)) {
		player->psendln("You have no idea how to do that.");
		return;
	}

	one_argument(argument, arg);

	if(!(vict = get_char_vis(player, arg, NULL, FIND_CHAR_ROOM))) {
		player->psendln("Whom do you want to rescue?");
		return;
	}

	if(vict->is(player)) {
		player->psendln("What about fleeing instead?");
		return;
	}

	if(player->fighting() == vict) {
		player->psendln("How can you rescue someone you are trying to kill?");
		return;
	}

	tmp_ch = nullptr;
	for(auto& plr : mods::globals::get_room_list(player->room())) {
		auto f = plr->fighting();
		if(f && f->is(vict)) {
			tmp_ch = f;
			break;
		}
	}

	if(!tmp_ch) {
		act("But nobody is fighting $M!", FALSE, ch, 0, *vict, TO_CHAR);
		return;
	}

	percent = rand_number(1, 101);	/* 101% is a complete failure */
	prob = GET_SKILL(ch, SKILL_RESCUE);

	if(percent > prob) {
		player->psendln("You fail the rescue!");
		return;
	}

	player->psendln("Banzai!  To the rescue...");
	act("You are rescued by $N, you are confused!", FALSE, *vict, 0, ch, TO_CHAR);
	act("$n heroically rescues $N!", FALSE, ch, 0, *vict, TO_NOTVICT);

	if(vict->fighting() == tmp_ch) {
		stop_fighting(*vict);
	}

	if(tmp_ch->fighting()) {
		stop_fighting(*tmp_ch);
	}

	if(player->fighting()) {
		stop_fighting(*player);
	}

	set_fighting(*player, *tmp_ch);
	set_fighting(*tmp_ch, *player);

	WAIT_STATE(vict->cd(), 2 * PULSE_VIOLENCE);
}



ACMD(do_kick) {
	char arg[MAX_INPUT_LENGTH];
	char_data *vict;
	int percent, prob;

	if(IS_NPC(ch) || !GET_SKILL(ch, SKILL_KICK)) {
		send_to_char(ch, "You have no idea how.");
		return;
	}

	one_argument(argument, arg);

	if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
		if(FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
			vict = FIGHTING(ch);
		} else {
			send_to_char(ch, "Kick who?");
			return;
		}
	}

	if(vict == ch) {
		send_to_char(ch, "Aren't we funny today...");
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


}

namespace offensive {
	void init() {
		mods::interpreter::add_command("affect_me",POS_RESTING,do_affect_me,0,0);
		mods::interpreter::add_command("assist",POS_RESTING,do_assist,0,0);
		mods::interpreter::add_command("backstab",POS_RESTING,do_backstab,0,0);
		mods::interpreter::add_command("bash",POS_RESTING,do_bash,0,0);
		mods::interpreter::add_command("c4_brain",POS_RESTING,c4_brain,0,0);
		mods::interpreter::add_command("command_sequence",POS_RESTING,do_command_sequence,0,0);
		mods::interpreter::add_command("engagement_mode",POS_RESTING,do_engagement_mode,0,0);
		mods::interpreter::add_command("go_loud",POS_RESTING,do_go_loud,0,0);
		mods::interpreter::add_command("hit",POS_RESTING,do_hit,0,0);
		mods::interpreter::add_command("kick",POS_RESTING,do_kick,0,0);
		mods::interpreter::add_command("kill",POS_RESTING,do_kill,0,0);
		mods::interpreter::add_command("order",POS_RESTING,do_order,0,0);
		mods::interpreter::add_command("regroup",POS_RESTING,do_regroup,0,0);
		mods::interpreter::add_command("rescue",POS_RESTING,do_rescue,0,0);
		mods::interpreter::add_command("scan",POS_RESTING,do_scan,0,0); /* !mods */
		mods::interpreter::add_command("silencers_off",POS_RESTING,do_silencers_off,0,0);
		mods::interpreter::add_command("silencers_on",POS_RESTING,do_silencers_on,0,0);
		mods::interpreter::add_command("snipe",POS_RESTING,do_snipe,0,0);
		mods::interpreter::add_command("snipe_object",POS_RESTING,do_snipe_object,0,0);
		mods::interpreter::add_command("spray",POS_RESTING,do_spray,0,0);
		mods::interpreter::add_command("throw",POS_RESTING,do_throw,0,0);
	}
};
