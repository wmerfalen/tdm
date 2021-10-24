/* ************************************************************************
*   File: fight.c                                       Part of CircleMUD *
*  Usage: Combat system                                                   *
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
#include "handler.h"
#include "interpreter.h"
#include "db.h"
#include "spells.h"
#include "screen.h"
#include "constants.h"
#include "mods/weapon.hpp"
#include "mods/behaviour_tree_impl.hpp"
#include "mods/object-utils.hpp"
#include "mods/weapons/damage-types.hpp"
#include "mods/levels.hpp"
#include "mods/rand.hpp"
#include "mods/loot.hpp"
#include "mods/classes/ghost.hpp"
#include "mods/melee/combat-order.hpp"
#include "mods/melee/main.hpp"
#include "mods/corpse.hpp"
#include "mods/combat-composer/engage-target.hpp"

#define MOD_SNIPE_SAME_ROOM_THACO 250
#define MOD_SNIPE_DISTANCE_THACO 5

/* Structures */
char_data *combat_list = NULL;	/* head of l-list of fighting chars */
char_data *next_combat_list = NULL;

/* External structures */
namespace mods::projectile {
	extern std::string fromdirstr(int direction,bool prefix, bool suffix);
};
namespace mods::js {
	extern std::string build_mob_death_trigger_string(std::string name);
};
extern struct message_list fight_messages[MAX_MESSAGES];
extern int pk_allowed;		/* see config.c */
extern int max_exp_gain;	/* see config.c */
extern int max_exp_loss;	/* see config.c */
extern int max_npc_corpse_time, max_pc_corpse_time;

/* External procedures */
char *fread_action(FILE *fl, int nr);
ACMD(do_flee);
int backstab_mult(int level);
int thaco(int ch_class, int level);
int ok_damage_shopkeeper(char_data *ch, char_data *victim);

/* local functions */
void perform_group_gain(char_data *ch, int base, char_data *victim);
void dam_message(int dam, char_data *ch, char_data *victim, int w_type);
void appear(char_data *ch);
void load_messages(void);
void free_messages(void);
void free_messages_type(struct msg_type *msg);
void check_killer(char_data *ch, char_data *vict);
void make_corpse(char_data *ch);
void change_alignment(char_data *ch, char_data *victim);
void death_cry(char_data *ch);
char *replace_string(const char *str, const char *weapon_singular, const char *weapon_plural);
void perform_violence(void);
int compute_armor_class(char_data *ch);
int compute_thaco(char_data *ch, char_data *vict);

/* Weapon attack texts */
struct attack_hit_type attack_hit_text[] = {
	{"hit", "hits"},		/* 0 */
	{"sting", "stings"},
	{"whip", "whips"},
	{"slash", "slashes"},
	{"bite", "bites"},
	{"bludgeon", "bludgeons"},	/* 5 */
	{"crush", "crushes"},
	{"pound", "pounds"},
	{"claw", "claws"},
	{"maul", "mauls"},
	{"thrash", "thrashes"},	/* 10 */
	{"pierce", "pierces"},
	{"blast", "blasts"},
	{"punch", "punches"},
	{"stab", "stabs"},
	{"snipe","snipes"},	/* 15 */
	{"blast","blasts"}, /* shotgun */
	{"shoot","shoots"}, /* smg */
	{"surpress","surpresses"}, /* LMG */
	{"explode","explodes"}, /* nades */
	{"torture","tortures"}, /* suffer */
	{"decompose","decomposes"},
	{"annihilate","annihilates"},	/* 20 */
	{"completely destroy","completely destroys"},
	{"utterly decimate","utterly decimates"},
	{"devoid","devoids"}
};

int attack_message(mw_rifle type) {
	switch(type) {
		case mw_rifle::SHOTGUN:
			return (int)TYPE_SHOTGUN;
		case mw_rifle::ASSAULT_RIFLE:
		case mw_rifle::SUB_MACHINE_GUN:
		case mw_rifle::PISTOL:
		case mw_rifle::MACHINE_PISTOL:
		case mw_rifle::HANDGUN:
			return (int)TYPE_SUB_MACHINE_GUN;
		case mw_rifle::SNIPER:
			return (int)TYPE_SNIPE;
		case mw_rifle::LIGHT_MACHINE_GUN:
			return (int)TYPE_LIGHT_MACHINE_GUN;
		default:
			return (int)TYPE_SUB_MACHINE_GUN;
	}
}


#ifndef IS_WEAPON
#define IS_WEAPON(type) (((type) >= TYPE_HIT) && ((type) < TYPE_SUFFERING))
#endif

/* The Fight related routines */

void appear(char_data *ch) {
	if(affected_by_spell(ch, SPELL_INVISIBLE)) {
		affect_from_char(ch, SPELL_INVISIBLE);
	}

	REMOVE_BIT(AFF_FLAGS(ch), AFF_INVISIBLE | AFF_HIDE);

	if(GET_LEVEL(ch) < LVL_IMMORT) {
		act("$n slowly fades into existence.", FALSE, ch, 0, 0, TO_ROOM);
	} else
		act("You feel a strange presence as $n appears, seemingly from nowhere.",
		    FALSE, ch, 0, 0, TO_ROOM);
}


int compute_armor_class(char_data *ch) {
	int armorclass = GET_AC(ch);

	if(AWAKE(ch)) {
		armorclass += dex_app[GET_DEX(ch)].defensive * 10;
	}

	return (MAX(-100, armorclass));      /* -100 is lowest */
}


void free_messages_type(struct msg_type *msg) {
	if(msg->attacker_msg)	{
		free(msg->attacker_msg);
	}

	if(msg->victim_msg) {
		free(msg->victim_msg);
	}

	if(msg->room_msg) {
		free(msg->room_msg);
	}
}


void free_messages(void) {
	int i;

	for(i = 0; i < MAX_MESSAGES; i++)
		while(fight_messages[i].msg) {
			struct message_type *former = fight_messages[i].msg;

			free_messages_type(&former->die_msg);
			free_messages_type(&former->miss_msg);
			free_messages_type(&former->hit_msg);
			free_messages_type(&former->god_msg);

			fight_messages[i].msg = fight_messages[i].msg->next;
			free(former);
		}
}


void load_messages(void) {
	MENTOC_DEPRECATED("load_messages");
	FILE *fl;
	int i, type;
	struct message_type *messages;
	char chk[129];

	chk[128] = '\0';
	if(!(fl = fopen(MESS_FILE, "r"))) {
		log("SYSERR: Error reading combat message file %s: %s", MESS_FILE, strerror(errno));
		exit(1);
	}

	for(i = 0; i < MAX_MESSAGES; i++) {
		fight_messages[i].a_type = 0;
		fight_messages[i].number_of_attacks = 0;
		fight_messages[i].msg = NULL;
	}

	fgets(chk, 128, fl);

	while(!feof(fl) && (*chk == '\n' || *chk == '*')) {
		fgets(chk, 128, fl);
	}

	while(*chk == 'M') {
		fgets(chk, 128, fl);
		sscanf(chk, " %d\n", &type);

		for(i = 0; (i < MAX_MESSAGES) && (fight_messages[i].a_type != type) &&
		        (fight_messages[i].a_type); i++);

		if(i >= MAX_MESSAGES) {
			log("SYSERR: Too many combat messages.  Increase MAX_MESSAGES and recompile.");
			exit(1);
		}

		CREATE(messages, struct message_type, 1);
		fight_messages[i].number_of_attacks++;
		fight_messages[i].a_type = type;
		messages->next = fight_messages[i].msg;
		fight_messages[i].msg = messages;

		messages->die_msg.attacker_msg = fread_action(fl, i);
		messages->die_msg.victim_msg = fread_action(fl, i);
		messages->die_msg.room_msg = fread_action(fl, i);
		messages->miss_msg.attacker_msg = fread_action(fl, i);
		messages->miss_msg.victim_msg = fread_action(fl, i);
		messages->miss_msg.room_msg = fread_action(fl, i);
		messages->hit_msg.attacker_msg = fread_action(fl, i);
		messages->hit_msg.victim_msg = fread_action(fl, i);
		messages->hit_msg.room_msg = fread_action(fl, i);
		messages->god_msg.attacker_msg = fread_action(fl, i);
		messages->god_msg.victim_msg = fread_action(fl, i);
		messages->god_msg.room_msg = fread_action(fl, i);
		fgets(chk, 128, fl);

		while(!feof(fl) && (*chk == '\n' || *chk == '*')) {
			fgets(chk, 128, fl);
		}
	}

	fclose(fl);
}

void update_pos(char_data *victim) {
	auto vict = ptr(victim);
	update_pos(vict);
}

void	update_pos(player_ptr_t& victim) {
	if((victim->hp() > 0) && (victim->position() > POS_STUNNED)) {
		return;
	} else if(victim->hp() > 0) {
		victim->position() = POS_STANDING;
	} else if(victim->hp() <= -11) {
		victim->position() = POS_DEAD;
	} else if(victim->hp() <= -6) {
		victim->position() = POS_MORTALLYW;
	} else if(victim->hp() <= -3) {
		victim->position() = POS_INCAP;
	} else {
		victim->position() = POS_STUNNED;
	}
}


void check_killer(char_data *ch, char_data *vict) {
	if(PLR_FLAGGED(vict, PLR_KILLER) || PLR_FLAGGED(vict, PLR_THIEF)) {
		return;
	}

	if(PLR_FLAGGED(ch, PLR_KILLER) || IS_NPC(ch) || IS_NPC(vict) || ch == vict) {
		return;
	}

	SET_BIT(PLR_FLAGS(ch), PLR_KILLER);
	send_to_char(ch, "If you want to be a PLAYER KILLER, so be it...");
	mudlog(BRF, LVL_IMMORT, TRUE, "PC Killer bit set on %s for initiating attack on %s at %s.",
	       GET_NAME(ch).c_str(), GET_NAME(vict).c_str(), world[IN_ROOM(vict)].name.c_str());
}


/* start one char fighting another (yes, it is horrible, I know... )  */
void set_fighting(char_data *ch, char_data *vict) {
	if(ch == vict) {
		return;
	}

	if(IN_ROOM(ch) != IN_ROOM(vict)) {
		return;
	}

	if(FIGHTING(ch)) {
		core_dump();
		return;
	}

	ch->next_fighting = combat_list;
	combat_list = ch;

	if(AFF_FLAGGED(ch, AFF_SLEEP)) {
		affect_from_char(ch, SPELL_SLEEP);
	}

	FIGHTING(ch) = vict;
	GET_POS(ch) = POS_FIGHTING;

	if(!pk_allowed) {
		check_killer(ch, vict);
	}
}



/* remove a char from the list of fighting chars */
void stop_fighting(char_data *ch) {
	char_data *temp;

	if(ch == next_combat_list) {
		next_combat_list = ch->next_fighting;
	}

	REMOVE_FROM_LIST(ch, combat_list, next_fighting);
	ch->next_fighting = NULL;
	FIGHTING(ch) = NULL;
	GET_POS(ch) = POS_STANDING;
	update_pos(ch);
}



void make_corpse(char_data *ch) {
	MENTOC_PREAMBLE();
	mods::corpse::make_corpse(player);
}


/* When ch kills victim */
void change_alignment(char_data *ch, char_data *victim) {
	/*
	 * new alignment change algorithm: if you kill a monster with alignment A,
	 * you move 1/16th of the way to having alignment -A.  Simple and fast.
	 */
	GET_ALIGNMENT(ch) += (-GET_ALIGNMENT(victim) - GET_ALIGNMENT(ch)) / 16;
}



void death_cry(char_data *ch) {
	int door;

	act("\r\nYour blood freezes as you hear $n's death cry.", FALSE, ch, 0, 0, TO_ROOM);

	for(door = 0; door < NUM_OF_DIRS; door++) {
		std::string from_dir = mods::projectile::fromdirstr(OPPOSITE_DIR(door),true,true) + "\r\n";
		if(CAN_GO(ch, door)) {
			std::string s = "Your blood freezes as you hear someone's death cry " + from_dir;
			send_to_room(world[IN_ROOM(ch)].dir_option[door]->to_room, s.c_str());
		}
	}
}



void perform_group_gain(char_data *ch, int base,
                        char_data *victim) {
	int share;

	share = MIN(max_exp_gain, MAX(1, base));

	if(share > 1) {
		send_to_char(ch, "You receive your share of experience -- %d points.", share);
	} else {
		send_to_char(ch, "You receive your share of experience -- one measly little point!");
	}

	auto p = ptr(ch);
	mods::levels::gain_exp(p, share);
	change_alignment(ch, victim);
}


void group_gain(char_data *ch, char_data *victim) {
	int tot_members, base, tot_gain;
	char_data *k;
	struct follow_type *f;

	if(!(k = ch->master)) {
		k = ch;
	}

	if(AFF_FLAGGED(k, AFF_GROUP) && (IN_ROOM(k) == IN_ROOM(ch))) {
		tot_members = 1;
	} else {
		tot_members = 0;
	}

	for(f = k->followers; f; f = f->next)
		if(AFF_FLAGGED(f->follower, AFF_GROUP) && IN_ROOM(f->follower) == IN_ROOM(ch)) {
			tot_members++;
		}

	/* round up to the next highest tot_members */
	tot_gain = (GET_EXP(victim) / 3) + tot_members - 1;

	/* prevent illegal xp creation when killing players */
	if(!IS_NPC(victim)) {
		tot_gain = MIN(max_exp_loss * 2 / 3, tot_gain);
	}

	if(tot_members >= 1) {
		base = MAX(1, tot_gain / tot_members);
	} else {
		base = 0;
	}

	if(AFF_FLAGGED(k, AFF_GROUP) && IN_ROOM(k) == IN_ROOM(ch)) {
		perform_group_gain(k, base, victim);
	}

	for(f = k->followers; f; f = f->next)
		if(AFF_FLAGGED(f->follower, AFF_GROUP) && IN_ROOM(f->follower) == IN_ROOM(ch)) {
			perform_group_gain(f->follower, base, victim);
		}
}



char *replace_string(const char *str, const char *weapon_singular, const char *weapon_plural) {
	static char buf[256];
	char *cp = buf;

	for(; *str; str++) {
		if(*str == '#') {
			switch(*(++str)) {
				case 'W':
					for(; *weapon_plural; *(cp++) = *(weapon_plural++));

					break;

				case 'w':
					for(; *weapon_singular; *(cp++) = *(weapon_singular++));

					break;

				default:
					*(cp++) = '#';
					break;
			}
		} else {
			*(cp++) = *str;
		}

		*cp = 0;
	}				/* For */

	return (buf);
}


/* message for doing damage with a weapon */
void dam_message(int dam, char_data *ch, char_data *victim,
                 int w_type) {
	MENTOC_PREAMBLE();
	char *buf;
	int msgnum;

	static struct dam_weapon_type {
		const char *to_room;
		const char *to_char;
		const char *to_victim;
	} dam_weapons[] = {

		/* use #w for singular (i.e. "slash") and #W for plural (i.e. "slashes") */

		{
			"$n tries to #w $N, but misses.",	/* 0: 0     */
			"You try to #w $N, but miss.",
			"$n tries to #w you, but misses."
		},

		{
			"$n tickles $N as $e #W $M.",	/* 1: 1..2  */
			"You tickle $N as you #w $M.",
			"$n tickles you as $e #W you."
		},

		{
			"$n barely #W $N.",		/* 2: 3..4  */
			"You barely #w $N.",
			"$n barely #W you."
		},

		{
			"$n #W $N.",			/* 3: 5..6  */
			"You #w $N.",
			"$n #W you."
		},

		{
			"$n #W $N hard.",			/* 4: 7..10  */
			"You #w $N hard.",
			"$n #W you hard."
		},

		{
			"$n #W $N very hard.",		/* 5: 11..14  */
			"You #w $N very hard.",
			"$n #W you very hard."
		},

		{
			"$n #W $N extremely hard.",	/* 6: 15..19  */
			"You #w $N extremely hard.",
			"$n #W you extremely hard."
		},

		{
			"$n massacres $N to small fragments with $s #w.",	/* 7: 19..23 */
			"You massacre $N to small fragments with your #w.",
			"$n massacres you to small fragments with $s #w."
		},

		{
			"$n OBLITERATES $N with $s deadly #w!!",	/* 8: > 23   */
			"You OBLITERATE $N with your deadly #w!!",
			"$n OBLITERATES you with $s deadly #w!!"
		}
	};

	static dam_weapon_type msg_SNIPER = {
		"$n snipes $N!",	/* 8: > 23   */
		"You aim and hit $N with your shot!!",
		"$n snipes you!!"
	};
	static dam_weapon_type msg_SUB_MACHINE_GUN = {
		"$n fires off some shots at $N!",	/* 8: > 23   */
		"You aim and hit $N with a burst of bullets!!",
		"$n hits you with a burst of bullets!!"
	};
	static dam_weapon_type msg_SHOTGUN = {
		"$n unloads a shotgun blast at $N!",	/* 8: > 23   */
		"You unload a shotgun blast at $N!!",
		"$n unloads a shotgun blast directly at you!!"
	};
	static dam_weapon_type msg_ASSAULT_RIFLE = {
		"$n release an unrelenting burst of shots at $N!",
		"You unload an unrelenting burst of shots at $N!!",
		"$n unloads an unrelenting burst of shots at you!!"
	};
	static dam_weapon_type msg_PISTOL = {
		"$n fires off a shot at $N!",
		"You fire off a shot at $N!!",
		"$n fires off a shot at you!!"
	};
	static dam_weapon_type msg_MACHINE_PISTOL = {
		"$n unleashes fast machine pistol fire at $N!",
		"You unleash fast machine pistol fire at $N!!",
		"$n unleashes fast machine pistol fire at you!!"
	};
	static dam_weapon_type msg_LIGHT_MACHINE_GUN = {
		"$n unleashes suppressive fire at $N!",
		"You suppress $N with your LMG burst!!",
		"$n suppresses you with LMG fire!!"
	};

	const char* to_room = nullptr;
	const char* to_char = nullptr;
	const char* to_victim = nullptr;
	if(player->attacking_with()) {
		if(player->attacking_with()->has_rifle()) {
			switch(player->attacking_with_type()) {
				default:
					break;
				case mw_rifle::SHOTGUN:
					to_room = msg_SHOTGUN.to_room;
					to_char = msg_SHOTGUN.to_char;
					to_victim = msg_SHOTGUN.to_victim;
					break;

				case mw_rifle::ASSAULT_RIFLE:
					to_room = msg_ASSAULT_RIFLE.to_room;
					to_char = msg_ASSAULT_RIFLE.to_char;
					to_victim = msg_ASSAULT_RIFLE.to_victim;
					break;

				case mw_rifle::SUB_MACHINE_GUN:
					to_room = msg_SUB_MACHINE_GUN.to_room;
					to_char = msg_SUB_MACHINE_GUN.to_char;
					to_victim = msg_SUB_MACHINE_GUN.to_victim;
					break;

				case mw_rifle::SNIPER:
					to_room = msg_SNIPER.to_room;
					to_char = msg_SNIPER.to_char;
					to_victim = msg_SNIPER.to_victim;
					break;

				case mw_rifle::HANDGUN:
				case mw_rifle::PISTOL:
					to_room = msg_PISTOL.to_room;
					to_char = msg_PISTOL.to_char;
					to_victim = msg_PISTOL.to_victim;
					break;

				case mw_rifle::MACHINE_PISTOL:
					to_room = msg_MACHINE_PISTOL.to_room;
					to_char = msg_MACHINE_PISTOL.to_char;
					to_victim = msg_MACHINE_PISTOL.to_victim;
					break;

				case mw_rifle::LIGHT_MACHINE_GUN:
					to_room = msg_LIGHT_MACHINE_GUN.to_room;
					to_char = msg_LIGHT_MACHINE_GUN.to_char;
					to_victim = msg_LIGHT_MACHINE_GUN.to_victim;
					break;
			}
		}
		if(player->attacking_with()->has_melee()) {
			/** TODO */
#if 0
			switch(player->attacking_with_type()) {
				default:
					to_room = "heh";
					break;
			}
#endif
		}
	}

	if(to_room && to_char && to_victim) {
		act(to_room, FALSE, ch, NULL, victim, TO_NOTVICT);
		send_to_char(ch, CCYEL(ch, C_CMP));
		act(to_char, FALSE, ch, NULL, victim, TO_CHAR);
		send_to_char(ch, CCNRM(ch, C_CMP));
		send_to_char(victim, CCRED(victim, C_CMP));
		act(to_victim, FALSE, ch, NULL, victim, TO_VICT | TO_SLEEP);
		send_to_char(victim, CCNRM(victim, C_CMP));
		return;
	}



	w_type -= TYPE_HIT;		/* Change to base of table with text */

	if(dam == 0) {
		msgnum = 0;
	} else if(dam <= 2) {
		msgnum = 1;
	} else if(dam <= 4) {
		msgnum = 2;
	} else if(dam <= 6) {
		msgnum = 3;
	} else if(dam <= 10) {
		msgnum = 4;
	} else if(dam <= 14) {
		msgnum = 5;
	} else if(dam <= 19) {
		msgnum = 6;
	} else if(dam <= 23) {
		msgnum = 7;
	} else {
		msgnum = 8;
	}



	/* damage message to onlookers */
	buf = replace_string(dam_weapons[msgnum].to_room,
	                     attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_NOTVICT);

	/* damage message to damager */
	send_to_char(ch, CCYEL(ch, C_CMP));
	buf = replace_string(dam_weapons[msgnum].to_char,
	                     attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_CHAR);
	send_to_char(ch, CCNRM(ch, C_CMP));

	/* damage message to damagee */
	send_to_char(victim, CCRED(victim, C_CMP));
	buf = replace_string(dam_weapons[msgnum].to_victim,
	                     attack_hit_text[w_type].singular, attack_hit_text[w_type].plural);
	act(buf, FALSE, ch, NULL, victim, TO_VICT | TO_SLEEP);
	send_to_char(victim, CCNRM(victim, C_CMP));
}


/*
 *  message for doing damage with a spell or skill
 *  C3.0: Also used for weapon damage on miss and death blows
 */
int skill_message(int dam, char_data *ch, char_data *vict,
                  int attacktype) {
	int i, j, nr;
	struct message_type *msg;

	struct obj_data *weap = GET_EQ(ch, WEAR_WIELD);

	for(i = 0; i < MAX_MESSAGES; i++) {
		if(fight_messages[i].a_type == attacktype) {
			nr = dice(1, fight_messages[i].number_of_attacks);

			for(j = 1, msg = fight_messages[i].msg; (j < nr) && msg; j++) {
				msg = msg->next;
			}

			if(!IS_NPC(vict) && (GET_LEVEL(vict) >= LVL_IMMORT)) {
				act(msg->god_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
				act(msg->god_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT);
				act(msg->god_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
			} else if(dam != 0) {
				/*
				 * Don't send redundant color codes for TYPE_SUFFERING & other types
				 * of damage without attacker_msg.
				 */
				if(GET_POS(vict) == POS_DEAD) {
					if(msg->die_msg.attacker_msg) {
						send_to_char(ch, CCYEL(ch, C_CMP));
						act(msg->die_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
						send_to_char(ch, CCNRM(ch, C_CMP));
					}

					send_to_char(vict, CCRED(vict, C_CMP));
					act(msg->die_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
					send_to_char(vict, CCNRM(vict, C_CMP));

					act(msg->die_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
				} else {
					if(msg->hit_msg.attacker_msg) {
						send_to_char(ch, CCYEL(ch, C_CMP));
						act(msg->hit_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
						send_to_char(ch, CCNRM(ch, C_CMP));
					}

					send_to_char(vict, CCRED(vict, C_CMP));
					act(msg->hit_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
					send_to_char(vict, CCNRM(vict, C_CMP));

					act(msg->hit_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
				}
			} else if(ch != vict) {	/* Dam == 0 */
				if(msg->miss_msg.attacker_msg) {
					send_to_char(ch, CCYEL(ch, C_CMP));
					act(msg->miss_msg.attacker_msg, FALSE, ch, weap, vict, TO_CHAR);
					send_to_char(ch, CCNRM(ch, C_CMP));
				}

				send_to_char(vict, CCRED(vict, C_CMP));
				act(msg->miss_msg.victim_msg, FALSE, ch, weap, vict, TO_VICT | TO_SLEEP);
				send_to_char(vict, CCNRM(vict, C_CMP));

				act(msg->miss_msg.room_msg, FALSE, ch, weap, vict, TO_NOTVICT);
			}

			return (1);
		}
	}

	return (0);
}


int damage(char_data *ch, char_data *victim, int dam, int attacktype) {
	return mods::weapons::damage_types::legacy::damage(ch,victim,dam,attacktype);
}

/*
 * Calculate the THAC0 of the attacker.
 *
 * 'victim' currently isn't used but you could use it for special cases like
 * weapons that hit evil creatures easier or a weapon that always misses
 * attacking an animal.
 */
int compute_thaco(char_data *ch, char_data *victim) {
	int calc_thaco;

	if(!IS_NPC(ch)) {
		calc_thaco = thaco(GET_CLASS(ch), GET_LEVEL(ch));
	} else {	/* THAC0 for monsters is set in the HitRoll */
		calc_thaco = 20;
	}

	calc_thaco -= str_app[STRENGTH_APPLY_INDEX(ch)].tohit;
	calc_thaco -= GET_HITROLL(ch);
	calc_thaco -= (int)((GET_INT(ch) - 13) / 1.5);	/* Intelligence helps! */
	calc_thaco -= (int)((GET_WIS(ch) - 13) / 1.5);	/* So does wisdom */

	return calc_thaco;
}

using vpd = mods::scan::vec_player_data;

void hit(char_data *ch, char_data *victim, int type) {
	MENTOC_PREAMBLE();
	using namespace mods::combat;
	if(!victim) {
		log("SYSERR: hit passed a null victim pointer");
		return;
	}
	auto victim_ptr_opt = ptr_opt(victim);
	if(!victim_ptr_opt.has_value()) {
		log("SYSERR: found null victim ptr... returning prematurely");
		return;
	}
	auto victim_ptr = victim_ptr_opt.value();
	int w_type, victim_ac, calc_thaco, dam, diceroll;
	auto primary = player->primary();
	auto secondary = player->secondary();
	obj_ptr_t wielded_weapon = nullptr;

	bool same_room = IN_ROOM(ch) == IN_ROOM(victim);
	bool primary_can_attack_same_room = primary && mods::object_utils::can_attack_same_room(primary);
	bool secondary_can_attack_same_room = secondary && mods::object_utils::can_attack_same_room(secondary);
	obj_ptr_t ub = (
	                   player->ghost() && player->ghost()->underbarrel() &&
	                   std::string(player->ghost()->underbarrel()->attachment()->attributes->underbarrel_launcher_type).compare("SHOTGUN") == 0
	               ) ? player->ghost()->underbarrel() : nullptr;
	bool ub_has_ammo = ub && player->ghost() && player->ghost()->get_shotgun_underbarrel_wrapper().ammo() > 0;

	bool do_ballistics = player->rules_of_engagement() == ROM_BALLISTIC;
	bool do_cqc = player->rules_of_engagement() == ROM_CQC;
	if(!same_room) {
		player->sendln("You must be in the same room to hit someone");
		return;
	}
	if(player->rules_of_engagement() == ROM_AUXILIARY) {
		if(ub && ub_has_ammo) {
			/**
			 * This just indirectly calls underbarrel_shotgun_attack_with_feedback()
			 * so, for now, this is fine, but we'll want to have engage_target handle
			 * this.
			 */
			mods::weapons::damage_types::rifle_attack_with_feedback(
			    player,
			    player->primary(),
			    victim_ptr,
			    0,
			    NORTH);
			return;
		}
		if(player->primary() == nullptr) {
			do_cqc = true;
		} else {
			do_ballistics = true;
		}
	}
	if(do_cqc || (do_ballistics && player->primary() == nullptr && player->secondary() == nullptr)) {
		auto hands_ptr = player->equipment(WEAR_HANDS);
		if(hands_ptr && hands_ptr->has_melee() && hands_ptr->melee()->attributes->type == mw_melee::BRASS_KNUCKLES && !wielded_weapon) {
			wielded_weapon = hands_ptr;
		}
		player->set_attacking_with(wielded_weapon);
		auto& combat_order = player->get_combat_order();
		if(combat_order.size()) {
			set_fighting(ch,victim);
			mods::melee::main::dispatch_player(player,victim_ptr);
			return;
		}
	}
	if(player->rules_of_engagement() == ROM_SECONDARY) {
		wielded_weapon = nullptr;
		if(secondary_can_attack_same_room) {
			wielded_weapon = secondary;
			player->set_attacking_with(wielded_weapon);
			if(wielded_weapon) {
				mods::combat_composer::engage_target(player,victim_ptr,wielded_weapon);
				return;
			}
		} else {
			do_ballistics = true;
		}
	}
	if(do_ballistics) {
		wielded_weapon = nullptr;
		if(primary_can_attack_same_room) {
			wielded_weapon = primary;
			player->set_attacking_with(wielded_weapon);
		} else if(secondary_can_attack_same_room) {
			wielded_weapon = secondary;
			player->set_attacking_with(wielded_weapon);
		}
		if(wielded_weapon) {
			mods::combat_composer::engage_target(player,victim_ptr,wielded_weapon);
			return;
		}
	}
	ch->last_fight_timestamp = time(NULL);
	/* Do some sanity checking, in case someone flees, etc. */
	if(IN_ROOM(ch) != IN_ROOM(victim)) {
		if(FIGHTING(ch) && FIGHTING(ch) == victim) {
			stop_fighting(ch);
		}

		return;
	}

	if(wielded_weapon && wielded_weapon->has_melee()) {
		mods::weapons::damage_types::melee_damage_with_feedback(
		    player,
		    wielded_weapon,
		    victim_ptr);
		return;
	}


	/* Find the weapon type (for display purposes only) */
	if(wielded_weapon && GET_OBJ_TYPE(wielded_weapon) == ITEM_WEAPON) {
		w_type = GET_OBJ_VAL(wielded_weapon, 3) + TYPE_HIT;
	} else {
		if(IS_NPC(ch) && ch->mob_specials.attack_type != 0) {
			w_type = ch->mob_specials.attack_type + TYPE_HIT;
		} else {
			w_type = TYPE_HIT;
		}
	}

	/* Calculate chance of hit. Lower THAC0 is better for attacker. */
	calc_thaco = compute_thaco(ch, victim);

	send_to_char(ch,(std::to_string(calc_thaco) + "\r\n").c_str());

	/* Calculate the raw armor including magic armor.  Lower AC is better for defender. */
	victim_ac = compute_armor_class(victim) / 10;

	/* roll the die and take your chances... */

	diceroll = 0;
	if(wielded_weapon) {
		if(wielded_weapon->has_rifle()) {
			diceroll = mods::rand::roll(wielded_weapon->rifle()->attributes->damage_dice_count,wielded_weapon->rifle()->attributes->damage_dice_sides);
		} else {
			diceroll = mods::rand::roll(1, 20);
		}
	} else {
		diceroll = rand_number(1, 20);
	}

	/*
	 * Decide whether this is a hit or a miss.
	 *
	 *  Victim asleep = hit, otherwise:
	 *     1   = Automatic miss.
	 *   2..19 = Checked vs. AC.
	 *    20   = Automatic hit.
	 */
	if(diceroll == 20 || !AWAKE(victim)) {
		dam = TRUE;
	} else if(diceroll == 1) {
		dam = FALSE;
	} else {
		dam = (calc_thaco - diceroll <= victim_ac);
	}

	if(!dam)
		/* the attacker missed the victim */
	{
		damage(ch, victim, 0, type == SKILL_BACKSTAB ? SKILL_BACKSTAB : w_type);
	} else {
		/* okay, we know the guy has been hit.  now calculate damage. */

		/* Start with the damage bonuses: the damroll and strength apply */

		/* Maybe holding arrow? */

		/*
		 * Include a damage multiplier if victim isn't ready to fight:
		 *
		 * Position sitting  1.33 x normal
		 * Position resting  1.66 x normal
		 * Position sleeping 2.00 x normal
		 * Position stunned  2.33 x normal
		 * Position incap    2.66 x normal
		 * Position mortally 3.00 x normal
		 *
		 * Note, this is a hack because it depends on the particular
		 * values of the POSITION_XXX constants.
		 */
		if(GET_POS(victim) < POS_FIGHTING) {
			dam *= 1 + (POS_FIGHTING - GET_POS(victim)) / 3;
		}

		/* at least 1 hp damage min per hit */
		dam = MAX(1, dam);

		if(type == SKILL_BACKSTAB) {
			damage(ch, victim, dam * backstab_mult(GET_LEVEL(ch)), SKILL_BACKSTAB);
		} else {
			damage(ch, victim, dam, w_type);
		}
	}
}



/* control the fights going on.  Called every 2 seconds from comm.c. */
void perform_violence() {
	char_data *ch;

	for(ch = combat_list; ch; ch = next_combat_list) {
		auto mob_ptr_opt = ptr_opt(FIGHTING(ch));
		if(!mob_ptr_opt.has_value()) {
			char_data* temp;
			log("Invalid pointer. not performing violence...");
			REMOVE_FROM_LIST(ch, combat_list, next_fighting);
			ch->next_fighting = NULL;
			FIGHTING(ch) = NULL;
			log("removed invalid ptr from combat_list...");
			continue;
		}
		next_combat_list = ch->next_fighting;

		if(FIGHTING(ch) == NULL || IN_ROOM(ch) != IN_ROOM(FIGHTING(ch))) {
			stop_fighting(ch);
			continue;
		}

		if(IS_NPC(ch)) {
			if(GET_MOB_WAIT(ch) > 0) {
				GET_MOB_WAIT(ch) -= PULSE_VIOLENCE;
				continue;
			}

			GET_MOB_WAIT(ch) = 0;

			if(GET_POS(ch) < POS_FIGHTING) {
				GET_POS(ch) = POS_FIGHTING;
				act("$n scrambles to $s feet!", TRUE, ch, 0, 0, TO_ROOM);
			}
		}

		if(GET_POS(ch) < POS_FIGHTING) {
			send_to_char(ch, "You can't fight while sitting!!");
			continue;
		}

		hit(ch, FIGHTING(ch), TYPE_UNDEFINED);

		if(MOB_FLAGGED(ch, MOB_SPEC) && GET_MOB_SPEC(ch) && !MOB_FLAGGED(ch, MOB_NOTDEADYET)) {
			char actbuf[MAX_INPUT_LENGTH] = "";
			MENTOC_PREAMBLE();
			(GET_MOB_SPEC(ch))(ch, ch, 0, actbuf,player);
		}
	}
}
