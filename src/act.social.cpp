/* ************************************************************************
*   File: act.social.c                                  Part of CircleMUD *
*  Usage: Functions to handle socials                                     *
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
#include "spells.h"
#include "mods/positions.hpp"


#define m_debug(A) std::cerr << "[socials] " << A << "\n";
/* local globals */
static int list_top = -1;

/* local functions */
char *fread_action(FILE *fl, int nr);
int find_action(int cmd);
ACMD(do_action);
ACMD(do_insult);
void boot_social_messages(void);
void free_social_messages(void);


struct social_messg {
	using act_number_t = uint16_t;
	act_number_t act_nr;
	bool hide;
	position_t min_victim_position;	/* Position of victim */

	/* No argument was supplied */
	char *char_no_arg;
	char *others_no_arg;

	/* An argument was there, and a victim was found */
	char *char_found;		/* if NULL, read no further, ignore args */
	char *others_found;
	char *vict_found;

	/* An argument was there, but no victim was found */
	char *not_found;

	/* The victim turned out to be the character */
	char *char_auto;
	char *others_auto;
} *soc_mess_list;


int find_action(int cmd) {
	int bot, top, mid;

	bot = 0;
	top = list_top;

	if(top < 0) {
		return (-1);
	}

	for(;;) {
		mid = (bot + top) / 2;

		if(soc_mess_list[mid].act_nr == cmd) {
			return (mid);
		}

		if(bot >= top) {
			return (-1);
		}

		if(soc_mess_list[mid].act_nr > cmd) {
			top = --mid;
		} else {
			bot = ++mid;
		}
	}
}


ACMD(do_action) {
	char buf[MAX_INPUT_LENGTH];
	int act_nr;
	struct social_messg *action;
	char_data *vict;

	if((act_nr = find_action(cmd)) < 0) {
		send_to_char(ch, "That action is not supported.");
		return;
	}

	action = &soc_mess_list[act_nr];

	if(action->char_found && argument) {
		one_argument(argument, buf);
	} else {
		*buf = '\0';
	}

	if(!*buf) {
		send_to_char(ch, "%s", action->char_no_arg);
		act(action->others_no_arg, action->hide, ch, 0, 0, TO_ROOM);
		return;
	}

	if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
		send_to_char(ch, "%s", action->not_found);
	} else if(vict == ch) {
		send_to_char(ch, "%s", action->char_auto);
		act(action->others_auto, action->hide, ch, 0, 0, TO_ROOM);
	} else {
		if(GET_POS(vict) < action->min_victim_position) {
			act("$N is not in a proper position for that.", FALSE, ch, 0, vict, TO_CHAR | TO_SLEEP);
		} else {
			act(action->char_found, 0, ch, 0, vict, TO_CHAR | TO_SLEEP);
			act(action->others_found, action->hide, ch, 0, vict, TO_NOTVICT);
			act(action->vict_found, action->hide, ch, 0, vict, TO_VICT);
		}
	}
}



ACMD(do_insult) {
	char arg[MAX_INPUT_LENGTH];
	char_data *victim;

	one_argument(argument, arg);

	if(*arg) {
		if(!(victim = get_char_vis(ch, arg, NULL, FIND_CHAR_ROOM))) {
			send_to_char(ch, "Can't hear you!");
		} else {
			if(victim != ch) {
				send_to_char(ch, "You insult %s.", GET_NAME(victim).c_str());

				switch(rand_number(0, 2)) {
					case 0:
						if(GET_SEX(ch) == SEX_MALE) {
							if(GET_SEX(victim) == SEX_MALE) {
								act("$n accuses you of fighting like a woman!", FALSE, ch, 0, victim, TO_VICT);
							} else {
								act("$n says that women can't fight.", FALSE, ch, 0, victim, TO_VICT);
							}
						} else {		/* Ch == Woman */
							if(GET_SEX(victim) == SEX_MALE)
								act("$n accuses you of having the smallest... (brain?)",
								    FALSE, ch, 0, victim, TO_VICT);
							else
								act("$n tells you that you'd lose a beauty contest against a troll.",
								    FALSE, ch, 0, victim, TO_VICT);
						}

						break;

					case 1:
						act("$n calls your mother a bitch!", FALSE, ch, 0, victim, TO_VICT);
						break;

					default:
						act("$n tells you to get lost!", FALSE, ch, 0, victim, TO_VICT);
						break;
				}			/* end switch */

				act("$n insults $N.", TRUE, ch, 0, victim, TO_NOTVICT);
			} else {			/* ch == victim */
				send_to_char(ch, "You feel insulted.");
			}
		}
	} else {
		send_to_char(ch, "I'm sure you don't want to insult *everybody*...");
	}
}


char *fread_action(FILE *fl, int nr) {
	char buf[MAX_STRING_LENGTH+1];

	fgets(buf, MAX_STRING_LENGTH, fl);
	buf[MAX_STRING_LENGTH] = '\0';

	if(feof(fl)) {
		log("SYSERR: fread_action: unexpected EOF near action #%d", nr);
		exit(1);
	}

	if(*buf == '#') {
		return (NULL);
	}

	buf[strlen(buf) - 1] = '\0';
	return (strdup(buf));
}


void free_social_messages(void) {
	int ac;
	struct social_messg *soc;

	for(ac = 0; ac <= list_top; ac++) {
		soc = &soc_mess_list[ac];

		if(soc->char_no_arg)	{
			free(soc->char_no_arg);
		}

		if(soc->others_no_arg)	{
			free(soc->others_no_arg);
		}

		if(soc->char_found)	{
			free(soc->char_found);
		}

		if(soc->others_found)	{
			free(soc->others_found);
		}

		if(soc->vict_found)	{
			free(soc->vict_found);
		}

		if(soc->not_found) {
			free(soc->not_found);
		}

		if(soc->char_auto) {
			free(soc->char_auto);
		}

		if(soc->others_auto)	{
			free(soc->others_auto);
		}
	}

	free(soc_mess_list);
}

bool expect_space(FILE* fl) {
	auto space = (char)fgetc(fl);
	bool is_space = space == ' ';
	ungetc(space,fl);
	return is_space;
}

/**
 * Just reads a char and discards it
 */
void fread_advance(FILE* fl) {
	fgetc(fl);
}

void fread_preview(FILE* fl,std::size_t count) {
	std::string str;
	for(auto f = 0; f < count; f++) {
		str += (char)fgetc(fl);
	}
	m_debug("preview(" << count << "): '" << str << "'");
	for(auto f = 0; f < count; f++) {
		ungetc(str[count-f-1],fl);
	}
}



bool expect_digit(FILE* fl) {
	auto ch = (char)fgetc(fl);
	bool is_digit = isdigit(ch);
	ungetc(ch,fl);
	return is_digit;
}

std::optional<int> fread_digit(FILE* fl) {

	auto hide_char = (char)fgetc(fl);
	if(!isdigit(hide_char)) {
		return std::nullopt;
	}

	std::array<char,2> temp_array;
	temp_array[0] = hide_char;
	temp_array[1] = '\0';

	return atoi(&temp_array[0]);
}

void boot_social_messages(void) {
	FILE *fl;
	int nr, i, hide = -1, curr_soc = -1;
	position_t min_pos = position_t::INVALID;
	char next_soc[1024];
	struct social_messg temp;

	/* open social file */
	if(!(fl = fopen(SOCMESS_FILE, "r"))) {
		log("SYSERR: can't open socials file '%s': %s", SOCMESS_FILE, strerror(errno));
		exit(1);
	}

	/* count socials & allocate space */
	for(nr = 0; *cmd_info[nr].command != '\n'; nr++) {
		if(cmd_info[nr].command_pointer == do_action) {
			list_top++;
		}
	}

	m_debug("list_top is: " << list_top);

	CREATE(soc_mess_list, struct social_messg, list_top + 1);

	/**
	 * Format of socials file
	 * <command> <hide> <min_pos>
	 *
	 * i.e.:
	 * accuse 0 5
	 *
	 */
	/* now read 'em */
	for(;;) {
		std::string temp_buffer;
		temp_buffer.clear();

		/**
		 * This parses the initial line
		 * i.e.: "accuse 0 5"
		 */
		for(auto i=0; i < 64; i++) {
			auto ch = (char)fgetc(fl);
			m_debug("ch: '" << ch << "'");
			if(isspace(ch)) {
				ungetc(ch,fl);
				break;
			}
			temp_buffer += ch;
		}
		m_debug("temp_buffer: '" << temp_buffer << "'");
		memset(next_soc,0,sizeof(next_soc));
		bcopy(temp_buffer.c_str(),next_soc,std::min(sizeof(next_soc) - 3,temp_buffer.length()));

		if(next_soc[0] == '$' && temp_buffer.length() < 2) {
			m_debug("breaking cuz next_soc[0] == '$'");
			break;
		}

		//fread_preview(fl,5);
		if(!expect_space(fl)) {
			log("SYSERR: [SOCIALS:1] expected SPACE CHARACTER after SOCIALS NAME column: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}
		fread_advance(fl);

		if(!expect_digit(fl)) {
			log("SYSERR: [SOCIALS:2] expected DIGIT after SPACE separating SOCIALS NAME and HIDE columns: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}

		hide = fread_digit(fl).value_or(-1);

		if(!expect_space(fl)) {
			log("SYSERR: [SOCIALS:3] expected SPACE CHARACTER after HIDE column: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}
		fread_advance(fl);

		if(!expect_digit(fl)) {
			log("SYSERR: [SOCIALS:4] expected DIGIT after SPACE identifying the MIN_POS column: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}

		min_pos = static_cast<position_t>(fread_digit(fl).value_or(position_t::INVALID));

		if(hide < 0) {
			log("SYSERR: [SOCIALS:5] invalid HIDE value for command: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}
		if(min_pos < 0) {
			log("SYSERR: [SOCIALS:6] invalid MIN_POS value for command: '%s' while parsing socials file... aborting",next_soc);
			sleep(-1);
		}

		if(feof(fl)) {
			m_debug("Breaking because feof");
			break;
		}
		auto newline = (char)fgetc(fl);
		if(newline != '\n') {
			ungetc(newline,fl);
			//m_debug("Breaking because newline != '\\n': " << (char)newline);
			//break;
		}

		if(++curr_soc > list_top) {
			log("SYSERR: Ran out of slots in social array. (%d > %d)", curr_soc, list_top);
			break;
		}

		m_debug("next_soc: '" << next_soc << "'");
		/* read the stuff */
		soc_mess_list[curr_soc].act_nr = nr = find_command(next_soc);
		soc_mess_list[curr_soc].hide = hide;
		soc_mess_list[curr_soc].min_victim_position = min_pos;

		/**
		 * char_no_arg - what is sent to the player who executes the command when you type no argument
		 * i.e.: "Accuse who?"
		 */
		soc_mess_list[curr_soc].char_no_arg = fread_action(fl, nr);

		/**
		 * others_no_arg - what others see when player executes the command with no argument
		 * i.e.: "$n bounces around!"
		 */
		soc_mess_list[curr_soc].others_no_arg = fread_action(fl, nr);
		/**
		 * char_found - if a victim was found
		 * i.e.: "You desperately try to squeeze a few coins from $M."
		 */
		soc_mess_list[curr_soc].char_found = fread_action(fl, nr);

		/* if no char_found, the rest is to be ignored */
		if(!soc_mess_list[curr_soc].char_found) {
			/** advance so we can consume the newline */
			fread_advance(fl);
			continue;
		}

		/** i.e.: "$n begs you for money.  You gratiously let $m peep at your fortune." */
		soc_mess_list[curr_soc].others_found = fread_action(fl, nr);
		/** i.e.: "$n begs $N for a dime or two -- or twenty!" */
		soc_mess_list[curr_soc].vict_found = fread_action(fl, nr);
		/** i.e.: "Your money-lender seems to be out for the moment." */
		soc_mess_list[curr_soc].not_found = fread_action(fl, nr);
		/** i.e.: "How? - begging yourself for money doesn't help." */
		soc_mess_list[curr_soc].char_auto = fread_action(fl, nr);
		/** i.e.: "$n tries to comb $s tangled hair." */
		soc_mess_list[curr_soc].others_auto = fread_action(fl, nr);

		/* If social not found, re-use this slot.  'curr_soc' will be reincremented. */
		if(nr < 0) {
			log("SYSERR: Unknown social '%s' in social file.", next_soc);
			memset(&soc_mess_list[curr_soc--], 0, sizeof(struct social_messg));
			continue;
		}

		/* If the command we found isn't do_action, we didn't count it for the CREATE(). */
		if(cmd_info[nr].command_pointer != do_action) {
			log("SYSERR: Social '%s' already assigned to a command.", next_soc);
			memset(&soc_mess_list[curr_soc--], 0, sizeof(struct social_messg));
		}
		auto last = (char)fgetc(fl);
		if(last != '\n') {
			ungetc(last,fl);
		}
	}
	m_debug("outside");

	/* close file & set top */
	fclose(fl);
	list_top = curr_soc;

	/* now, sort 'em */
	for(curr_soc = 0; curr_soc < list_top; curr_soc++) {
		min_pos = static_cast<position_t>(curr_soc);

		for(i = curr_soc + 1; i <= list_top; i++)
			if(soc_mess_list[i].act_nr < soc_mess_list[min_pos].act_nr) {
				min_pos = static_cast<position_t>(i);
			}

		if(curr_soc != min_pos) {
			temp = soc_mess_list[curr_soc];
			soc_mess_list[curr_soc] = soc_mess_list[min_pos];
			soc_mess_list[min_pos] = temp;
		}
	}
}
#undef m_debug
