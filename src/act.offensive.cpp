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
#include "mods/utils.hpp"
#include "globals.hpp"
#include "mods/scan.hpp"
#include "mods/weapon.hpp"
#include "globals.hpp"

/* extern variables */
extern int pk_allowed;

/* extern functions */
void raw_kill(struct char_data *ch);
void check_killer(struct char_data *ch, struct char_data *vict);
int compute_armor_class(struct char_data *ch);
int snipe_hit(struct char_data*,struct char_data*,int);

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


ACMD(do_rnum){
	send_to_char(ch, std::to_string(ch->in_room).c_str());
}
ACMD(do_ammo){
	MENTOC_PREAMBLE();
	player->weapon()->ammo += 12;
}

using vpd = mods::scan::vec_player_data;
using vpde = mods::scan::vec_player_data_element;
ACMD(do_snipe){
	/* HOWTO: start an "Action Command" function declaration */
	MENTOC_PREAMBLE();	/* !mods */
	/*
	if(!player->weapon_cooldown_expired(0)){
		return;
	}
	*/
	
	if(!player->has_weapon_capability(mods::weapon::mask::snipe)){
		send_to_char(ch,"You must be wielding a sniper rifle to do that!");
		return;
	}
	/* Check ammo */
	if(player->weapon()->ammo <= 0){
		*player << "Out of ammo!\r\n";
		return;
	}
	constexpr unsigned int max_char = 16;
	std::array<char,max_char> victim;
	std::fill(victim.begin(),victim.end(),0);
	vpd scan;
	one_argument(argument,(char*)&victim[0],max_char);
	if(!victim[0])
		send_to_char(ch, "Whom do you wish to snipe?\r\n");
	/* HOWTO: perform line of sight scans */
	mods::scan::los_scan(ch,3,&scan);
	for(auto scanned_target : scan){
		/* HOWTO: fuzzy match two strings */
		if(mods::util::fuzzy_match(static_cast<char*>(&victim[0]),scanned_target->player.name)){
			/* HOWTO: deal damage with a sniper rifle */
      		auto damage = snipe_hit(ch, scanned_target, TYPE_SNIPE);
			/* HOWTO: adjust ammo of player's current weapon */
			player->ammo_adjustment(-1);
			player->weapon_cooldown_start(3,0);

			mods::globals::room_event(ch,mods::ai_state::AI_WITNESS_ATTACK);
			/*
			 * HOWTO: defer an action N seconds in the future
			mods::globals::defer_queue->push_secs(3,[&](){
			});
			*/
			return;
		}
	}
}

ACMD(do_thermite){
	MENTOC_PREAMBLE();
}

/* TODO: Implement weapon tags in the obj_data data structure */
ACMD(do_reload){
	MENTOC_PREAMBLE(); /* !mods */
	/* TODO get wielded equipment tag */
	if(!player->weapon()){
		*player << "Invalid weapon to reload.\r\n";
		return;
	}

	if(player->weapon()->ammo > 0){
		*player << "Weapon already loaded.\r\n";
		return;
	}
	//*player << player->weapon()->weapon_type << "\r\n";

	if(!player->carrying_ammo_of_type(player->weapon()->weapon_type)){
		*player << "{1} You don't have any ammo.\r\n";
		return;
	}

	auto ammo = player->get_ammo(player->weapon()->weapon_type);
	if(ammo->ammo -12 < 0){
		auto difference = ammo->ammo -12;
		auto rounds = difference + 12;
		if(rounds <= 0){
			*player << "Out of ammo.\r\n";
			ammo->ammo = 0;
			return;
		}
		*player << "You load " << rounds << " rounds into " << player->weapon()->name << "\r\n";
		ammo->ammo = 0;
		player->weapon()->ammo = rounds;
		return;
	}else{
		ammo->ammo -= 12;
		player->weapon()->ammo = 12;
		*player << "You load 12 rounds into " << player->weapon()->name << "\r\n";
		return;
	}
}

ACMD(do_scan){ /* !mods */
	vpd scan;
	mods::scan::los_scan_foreach(ch,3,[ch](room_rnum _room_id,int _dir,vpde _ele){
		std::string line;
		switch(_dir){
			case NORTH: line += "[north]";break;
			case SOUTH: line += "[south]";break;
			case EAST: line += "[east]";break;
			case WEST: line += "[west]";break;
			case UP: line += "[up]";break;
			case DOWN: line += "[down]";break;
		}
		line += " you see ";
		line += _ele->player.name;
		line += "\r\n";
		send_to_char(ch,line.c_str());
	});

}


ACMD(do_assist)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *helpee, *opponent;

  if (FIGHTING(ch)) {
    send_to_char(ch, "You're already fighting!  How can you assist someone else?\r\n");
    return;
  }
  one_argument(argument, arg);

  if (!*arg)
    send_to_char(ch, "Whom do you wish to assist?\r\n");
  else if (!(helpee = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM)))
    send_to_char(ch, "%s", NOPERSON);
  else if (helpee == ch)
    send_to_char(ch, "You can't help yourself any more than this!\r\n");
  else {
    /*
     * Hit the same enemy the person you're helping is.
     */
    if (FIGHTING(helpee))
      opponent = FIGHTING(helpee);
    else
      for (opponent = world[IN_ROOM(ch)].people;
	   opponent && (FIGHTING(opponent) != helpee);
	   opponent = opponent->next_in_room)
		;

    if (!opponent)
      act("But nobody is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!CAN_SEE(ch, opponent))
      act("You can't see who is fighting $M!", FALSE, ch, 0, helpee, TO_CHAR);
    else if (!pk_allowed && !IS_NPC(opponent))	/* prevent accidental pkill */
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


ACMD(do_hit)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *vict;

  one_argument(argument, arg);

  if (!*arg)
    send_to_char(ch, "Hit who?\r\n");
  else if (!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM)))
    send_to_char(ch, "They don't seem to be here.\r\n");
  else if (vict == ch) {
    send_to_char(ch, "You hit yourself...OUCH!.\r\n");
    act("$n hits $mself, and says OUCH!", FALSE, ch, 0, vict, TO_ROOM);
  } else if (AFF_FLAGGED(ch, AFF_CHARM) && (ch->master == vict))
    act("$N is just such a good friend, you simply can't hit $M.", FALSE, ch, 0, vict, TO_CHAR);
  else {
    if (!pk_allowed) {
      if (!IS_NPC(vict) && !IS_NPC(ch)) {
	if (subcmd != SCMD_MURDER) {
	  send_to_char(ch, "Use 'murder' to hit another player.\r\n");
	  return;
	} else {
	  check_killer(ch, vict);
	}
      }
      if (AFF_FLAGGED(ch, AFF_CHARM) && !IS_NPC(ch->master) && !IS_NPC(vict))
	return;			/* you can't order a charmed pet to attack a
				 * player */
    }
    if ((GET_POS(ch) == POS_STANDING) && (vict != FIGHTING(ch))) {
      hit(ch, vict, TYPE_UNDEFINED);
      WAIT_STATE(ch, PULSE_VIOLENCE + 2);
    } else
      send_to_char(ch, "You do the best you can!\r\n");
  }
}



ACMD(do_kill)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *vict;

  if (GET_LEVEL(ch) < LVL_IMPL || IS_NPC(ch)) {
    do_hit(ch, argument, cmd, subcmd);
    return;
  }
  one_argument(argument, arg);

  if (!*arg) {
    send_to_char(ch, "Kill who?\r\n");
  } else {
    if (!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM)))
      send_to_char(ch, "They aren't here.\r\n");
    else if (ch == vict)
      send_to_char(ch, "Your mother would be so sad.. :(\r\n");
    else {
      act("You chop $M to pieces!  Ah!  The blood!", FALSE, ch, 0, vict, TO_CHAR);
      act("$N chops you to pieces!", FALSE, vict, 0, ch, TO_CHAR);
      act("$n brutally slays $N!", FALSE, ch, 0, vict, TO_NOTVICT);
      raw_kill(vict);
    }
  }
}



ACMD(do_backstab)
{
  char buf[MAX_INPUT_LENGTH];
  struct char_data *vict;
  int percent, prob;

  if (IS_NPC(ch) || !GET_SKILL(ch, SKILL_BACKSTAB)) {
    send_to_char(ch, "You have no idea how to do that.\r\n");
    return;
  }

  one_argument(argument, buf);

  if (!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
    send_to_char(ch, "Backstab who?\r\n");
    return;
  }
  if (vict == ch) {
    send_to_char(ch, "How can you sneak up on yourself?\r\n");
    return;
  }
  if (!GET_EQ(ch, WEAR_WIELD)) {
    send_to_char(ch, "You need to wield a weapon to make it a success.\r\n");
    return;
  }
  if (GET_OBJ_VAL(GET_EQ(ch, WEAR_WIELD), 3) != TYPE_PIERCE - TYPE_HIT) {
    send_to_char(ch, "Only piercing weapons can be used for backstabbing.\r\n");
    return;
  }
  if (FIGHTING(vict)) {
    send_to_char(ch, "You can't backstab a fighting person -- they're too alert!\r\n");
    return;
  }

  if (MOB_FLAGGED(vict, MOB_AWARE) && AWAKE(vict)) {
    act("You notice $N lunging at you!", FALSE, vict, 0, ch, TO_CHAR);
    act("$e notices you lunging at $m!", FALSE, vict, 0, ch, TO_VICT);
    act("$n notices $N lunging at $m!", FALSE, vict, 0, ch, TO_NOTVICT);
    hit(vict, ch, TYPE_UNDEFINED);
    return;
  }

  percent = rand_number(1, 101);	/* 101% is a complete failure */
  prob = GET_SKILL(ch, SKILL_BACKSTAB);

  if (AWAKE(vict) && (percent > prob))
    damage(ch, vict, 0, SKILL_BACKSTAB);
  else
    hit(ch, vict, SKILL_BACKSTAB);

  WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
}


ACMD(do_order)
{
  char name[MAX_INPUT_LENGTH], message[MAX_INPUT_LENGTH];
  bool found = FALSE;
  struct char_data *vict;
  struct follow_type *k;

  half_chop(argument, name, message);

  if (!*name || !*message)
    send_to_char(ch, "Order who to do what?\r\n");
  else if (!(vict = get_char_vis(ch, name, NULL, FIND_CHAR_ROOM)) && !is_abbrev(name, "followers"))
    send_to_char(ch, "That person isn't here.\r\n");
  else if (ch == vict)
    send_to_char(ch, "You obviously suffer from skitzofrenia.\r\n");
  else {
    if (AFF_FLAGGED(ch, AFF_CHARM)) {
      send_to_char(ch, "Your superior would not aprove of you giving orders.\r\n");
      return;
    }
    if (vict) {
      char buf[MAX_STRING_LENGTH];

      snprintf(buf, sizeof(buf), "$N orders you to '%s'", message);
      act(buf, FALSE, vict, 0, ch, TO_CHAR);
      act("$n gives $N an order.", FALSE, ch, 0, vict, TO_ROOM);

      if ((vict->master != ch) || !AFF_FLAGGED(vict, AFF_CHARM))
	act("$n has an indifferent look.", FALSE, vict, 0, 0, TO_ROOM);
      else {
	send_to_char(ch, "%s", OK);
	command_interpreter(vict, message);
      }
    } else {			/* This is order "followers" */
      char buf[MAX_STRING_LENGTH];

      snprintf(buf, sizeof(buf), "$n issues the order '%s'.", message);
      act(buf, FALSE, ch, 0, 0, TO_ROOM);

      for (k = ch->followers; k; k = k->next) {
	if (IN_ROOM(ch) == IN_ROOM(k->follower))
	  if (AFF_FLAGGED(k->follower, AFF_CHARM)) {
	    found = TRUE;
	    command_interpreter(k->follower, message);
	  }
      }
      if (found)
	send_to_char(ch, "%s", OK);
      else
	send_to_char(ch, "Nobody here is a loyal subject of yours!\r\n");
    }
  }
}



ACMD(do_flee)
{
  int i, attempt, loss;
  struct char_data *was_fighting;

  if (GET_POS(ch) < POS_FIGHTING) {
    send_to_char(ch, "You are in pretty bad shape, unable to flee!\r\n");
    return;
  }

  for (i = 0; i < 6; i++) {
    attempt = rand_number(0, NUM_OF_DIRS - 1);	/* Select a random direction */
    if (CAN_GO(ch, attempt) &&
	!ROOM_FLAGGED(EXIT(ch, attempt)->to_room, ROOM_DEATH)) {
      act("$n panics, and attempts to flee!", TRUE, ch, 0, 0, TO_ROOM);
      was_fighting = FIGHTING(ch);
      if (do_simple_move(ch, attempt, TRUE)) {
	send_to_char(ch, "You flee head over heels.\r\n");
	if (was_fighting && !IS_NPC(ch)) {
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


ACMD(do_bash)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *vict;
  int percent, prob;

  one_argument(argument, arg);

  if (IS_NPC(ch) || !GET_SKILL(ch, SKILL_BASH)) {
    send_to_char(ch, "You have no idea how.\r\n");
    return;
  }
  if (ROOM_FLAGGED(IN_ROOM(ch), ROOM_PEACEFUL)) {
    send_to_char(ch, "This room just has such a peaceful, easy feeling...\r\n");
    return;
  }
  if (!GET_EQ(ch, WEAR_WIELD)) {
    send_to_char(ch, "You need to wield a weapon to make it a success.\r\n");
    return;
  }
  if (!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
    if (FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
      vict = FIGHTING(ch);
    } else {
      send_to_char(ch, "Bash who?\r\n");
      return;
    }
  }
  if (vict == ch) {
    send_to_char(ch, "Aren't we funny today...\r\n");
    return;
  }
  percent = rand_number(1, 101);	/* 101% is a complete failure */
  prob = GET_SKILL(ch, SKILL_BASH);

  if (MOB_FLAGGED(vict, MOB_NOBASH))
    percent = 101;

  if (percent > prob) {
    damage(ch, vict, 0, SKILL_BASH);
    GET_POS(ch) = POS_SITTING;
  } else {
    /*
     * If we bash a player and they wimp out, they will move to the previous
     * room before we set them sitting.  If we try to set the victim sitting
     * first to make sure they don't flee, then we can't bash them!  So now
     * we only set them sitting if they didn't flee. -gg 9/21/98
     */
    if (damage(ch, vict, 1, SKILL_BASH) > 0) {	/* -1 = dead, 0 = miss */
      WAIT_STATE(vict, PULSE_VIOLENCE);
      if (IN_ROOM(ch) == IN_ROOM(vict))
        GET_POS(vict) = POS_SITTING;
    }
  }
  WAIT_STATE(ch, PULSE_VIOLENCE * 2);
}


ACMD(do_rescue)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *vict, *tmp_ch;
  int percent, prob;

  if (IS_NPC(ch) || !GET_SKILL(ch, SKILL_RESCUE)) {
    send_to_char(ch, "You have no idea how to do that.\r\n");
    return;
  }

  one_argument(argument, arg);

  if (!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
    send_to_char(ch, "Whom do you want to rescue?\r\n");
    return;
  }
  if (vict == ch) {
    send_to_char(ch, "What about fleeing instead?\r\n");
    return;
  }
  if (FIGHTING(ch) == vict) {
    send_to_char(ch, "How can you rescue someone you are trying to kill?\r\n");
    return;
  }
  for (tmp_ch = world[IN_ROOM(ch)].people; tmp_ch &&
       (FIGHTING(tmp_ch) != vict); tmp_ch = tmp_ch->next_in_room);

  if (!tmp_ch) {
    act("But nobody is fighting $M!", FALSE, ch, 0, vict, TO_CHAR);
    return;
  }
  percent = rand_number(1, 101);	/* 101% is a complete failure */
  prob = GET_SKILL(ch, SKILL_RESCUE);

  if (percent > prob) {
    send_to_char(ch, "You fail the rescue!\r\n");
    return;
  }
  send_to_char(ch, "Banzai!  To the rescue...\r\n");
  act("You are rescued by $N, you are confused!", FALSE, vict, 0, ch, TO_CHAR);
  act("$n heroically rescues $N!", FALSE, ch, 0, vict, TO_NOTVICT);

  if (FIGHTING(vict) == tmp_ch)
    stop_fighting(vict);
  if (FIGHTING(tmp_ch))
    stop_fighting(tmp_ch);
  if (FIGHTING(ch))
    stop_fighting(ch);

  set_fighting(ch, tmp_ch);
  set_fighting(tmp_ch, ch);

  WAIT_STATE(vict, 2 * PULSE_VIOLENCE);
}



ACMD(do_kick)
{
  char arg[MAX_INPUT_LENGTH];
  struct char_data *vict;
  int percent, prob;

  if (IS_NPC(ch) || !GET_SKILL(ch, SKILL_KICK)) {
    send_to_char(ch, "You have no idea how.\r\n");
    return;
  }
  one_argument(argument, arg);

  if (!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
    if (FIGHTING(ch) && IN_ROOM(ch) == IN_ROOM(FIGHTING(ch))) {
      vict = FIGHTING(ch);
    } else {
      send_to_char(ch, "Kick who?\r\n");
      return;
    }
  }
  if (vict == ch) {
    send_to_char(ch, "Aren't we funny today...\r\n");
    return;
  }
  /* 101% is a complete failure */
  percent = ((10 - (compute_armor_class(vict) / 10)) * 2) + rand_number(1, 101);
  prob = GET_SKILL(ch, SKILL_KICK);

  if (percent > prob) {
    damage(ch, vict, 0, SKILL_KICK);
  } else
    damage(ch, vict, GET_LEVEL(ch) / 2, SKILL_KICK);

  WAIT_STATE(ch, PULSE_VIOLENCE * 3);
}

