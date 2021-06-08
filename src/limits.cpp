/* ************************************************************************
*   File: limits.c                                      Part of CircleMUD *
*  Usage: limits & gain funcs for HMV, exp, hunger/thirst, idle time      *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#include "conf.h"
#include "config.hpp"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "spells.h"
#include "comm.h"
#include "db.h"
#include "handler.h"
#include "mods/loops.hpp"
#include "interpreter.h"
#include "mods/world-configuration.hpp"
#include "mods/levels.hpp"

/* external variables */
extern int max_exp_gain;
extern int max_exp_loss;
extern int idle_rent_time;
extern int idle_max_level;
extern int idle_void;
extern int immort_level_ok;
extern int use_autowiz;
extern int min_wizlist_lev;
extern int free_rent;
extern char_data* character_list;

/* local functions */
int graf(int grafage, int p0, int p1, int p2, int p3, int p4, int p5, int p6);
void run_autowiz(void);

void Crash_rentsave(char_data *ch, int cost);
char *title_male(int chclass, int level);
char *title_female(int chclass, int level);
void update_char_objects(char_data *ch);	/* handler.c */
void reboot_wizlists(void);

/* When age < 15 return the value p0 */
/* When age in 15..29 calculate the line between p1 & p2 */
/* When age in 30..44 calculate the line between p2 & p3 */
/* When age in 45..59 calculate the line between p3 & p4 */
/* When age in 60..79 calculate the line between p4 & p5 */
/* When age >= 80 return the value p6 */
int graf(int grafage, int p0, int p1, int p2, int p3, int p4, int p5, int p6) {

	if(grafage < 15) {
		return (p0);    /* < 15   */
	} else if(grafage <= 29) {
		return (p1 + (((grafage - 15) * (p2 - p1)) / 15));    /* 15..29 */
	} else if(grafage <= 44) {
		return (p2 + (((grafage - 30) * (p3 - p2)) / 15));    /* 30..44 */
	} else if(grafage <= 59) {
		return (p3 + (((grafage - 45) * (p4 - p3)) / 15));    /* 45..59 */
	} else if(grafage <= 79) {
		return (p4 + (((grafage - 60) * (p5 - p4)) / 20));    /* 60..79 */
	} else {
		return (p6);    /* >= 80 */
	}
}


/*
 * The hit_limit, mana_limit, and move_limit functions are gone.  They
 * added an unnecessary level of complexity to the internal structure,
 * weren't particularly useful, and led to some annoying bugs.  From the
 * players' point of view, the only difference the removal of these
 * functions will make is that a character's age will now only affect
 * the HMV gain per tick, and _not_ the HMV maximums.
 */

/* manapoint gain pr. game hour */
int mana_gain(char_data *ch) {
	int gain;

	if(IS_NPC(ch)) {
		/* Neat and fast */
		gain = GET_LEVEL(ch);
	} else {
		gain = graf(age(ch)->year, 4, 8, 12, 16, 12, 10, 8);

		/* Class calculations */

		/* Skill/Spell calculations */

		/* Position calculations    */
		switch(GET_POS(ch)) {
			case POS_SLEEPING:
				gain *= 2;
				break;

			case POS_RESTING:
				gain += (gain / 2);	/* Divide by 2 */
				break;

			case POS_SITTING:
				gain += (gain / 4);	/* Divide by 4 */
				break;
		}

		if(IS_MAGIC_USER(ch) || IS_CLERIC(ch)) {
			gain *= 2;
		}

		if((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0)) {
			gain /= 4;
		}
	}

	if(AFF_FLAGGED(ch, AFF_POISON)) {
		gain /= 4;
	}

	return (gain);
}


/* Hitpoint gain pr. game hour */
int hit_gain(char_data *ch) {
	int gain;

	if(IS_NPC(ch)) {
		/* Neat and fast */
		gain = GET_LEVEL(ch);
	} else {

		gain = graf(age(ch)->year, 8, 12, 20, 32, 16, 10, 4);

		/* Class/Level calculations */

		/* Skill/Spell calculations */

		/* Position calculations    */

		switch(GET_POS(ch)) {
			case POS_SLEEPING:
				gain += (gain / 2);	/* Divide by 2 */
				break;

			case POS_RESTING:
				gain += (gain / 4);	/* Divide by 4 */
				break;

			case POS_SITTING:
				gain += (gain / 8);	/* Divide by 8 */
				break;
		}

		if(IS_MAGIC_USER(ch) || IS_CLERIC(ch)) {
			gain /= 2;    /* Ouch. */
		}

		if((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0)) {
			gain /= 4;
		}
	}

	if(AFF_FLAGGED(ch, AFF_POISON)) {
		gain /= 4;
	}

	return (gain);
}



/* move gain pr. game hour */
int move_gain(char_data *ch) {
	int gain;

	if(IS_NPC(ch)) {
		/* Neat and fast */
		gain = GET_LEVEL(ch);
	} else {
		gain = graf(age(ch)->year, 16, 20, 24, 20, 16, 12, 10);

		/* Class/Level calculations */

		/* Skill/Spell calculations */


		/* Position calculations    */
		switch(GET_POS(ch)) {
			case POS_SLEEPING:
				gain += (gain / 2);	/* Divide by 2 */
				break;

			case POS_RESTING:
				gain += (gain / 4);	/* Divide by 4 */
				break;

			case POS_SITTING:
				gain += (gain / 8);	/* Divide by 8 */
				break;
		}

		if((GET_COND(ch, FULL) == 0) || (GET_COND(ch, THIRST) == 0)) {
			gain /= 4;
		}
	}

	if(AFF_FLAGGED(ch, AFF_POISON)) {
		gain /= 4;
	}

	return (gain);
}



void set_title(char_data* ch, const char* title) {
	MENTOC_PREAMBLE();
	set_title(player,title);
}
void set_title(player_ptr_t player, const char* title) {
	if(!title) {
		auto ch = player->cd();
		std::string final_title;
		if(GET_SEX(ch) == SEX_FEMALE) {
			final_title = title_female(player->chclass(), player->level());
		} else {
			final_title = title_male(player->chclass(), player->level());
		}
		player->title().assign(final_title);
		return;
	} else {
		player->title().assign(title);
	}
}


void run_autowiz(void) {
	log("Calling run_autowiz is deprecated. Returning immediately. %s:%d",__FILE__,__LINE__);
	return;
#if 0
#if defined(CIRCLE_UNIX) || defined(CIRCLE_WINDOWS)

	if(use_autowiz) {
		size_t res;
		char buf[256];

#if defined(CIRCLE_UNIX)
		res = snprintf(buf, sizeof(buf), "nice ../bin/autowiz %d %s %d %s %d &",
		               min_wizlist_lev, WIZLIST_FILE, LVL_IMMORT, IMMLIST_FILE, (int) getpid());
#elif defined(CIRCLE_WINDOWS)
		res = snprintf(buf, sizeof(buf), "autowiz %d %s %d %s",
		               min_wizlist_lev, WIZLIST_FILE, LVL_IMMORT, IMMLIST_FILE);
#endif /* CIRCLE_WINDOWS */

		/* Abusing signed -> unsigned conversion to avoid '-1' check. */
		if(res < sizeof(buf)) {
			mudlog(CMP, LVL_IMMORT, FALSE, "Initiating autowiz.");
			reboot_wizlists();
		} else {
			log("Cannot run autowiz: command-line doesn't fit in buffer.");
		}
	}

#endif /* CIRCLE_UNIX || CIRCLE_WINDOWS */
#endif
}



namespace legacy {
	void gain_exp(char_data *ch, int gain) {
		int is_altered = FALSE;
		int num_levels = 0;

		if(!IS_NPC(ch) && ((GET_LEVEL(ch) < 1 || GET_LEVEL(ch) >= LVL_IMMORT))) {
			return;
		}

		if(IS_NPC(ch)) {
			GET_EXP(ch) += gain;
			return;
		}

		if(gain > 0) {
			gain = MIN(max_exp_gain, gain);	/* put a cap on the max gain per kill */
			GET_EXP(ch) += gain;

			while(GET_LEVEL(ch) < LVL_IMMORT - immort_level_ok &&
			        GET_EXP(ch) >= mods::levels::level_exp(GET_LEVEL(ch) + 1)) {
				GET_LEVEL(ch) += 1;
				num_levels++;
				advance_level(ch);
				is_altered = TRUE;
			}

			if(is_altered) {
				mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "%s advanced %d level%s to level %d.",
				       GET_NAME(ch).c_str(), num_levels, num_levels == 1 ? "" : "s", GET_LEVEL(ch));

				if(num_levels == 1) {
					send_to_char(ch, "You rise a level!");
				} else {
					send_to_char(ch, "You rise %d levels!", num_levels);
				}

				set_title(ch, NULL);

				if(GET_LEVEL(ch) >= LVL_IMMORT) {
					run_autowiz();
				}
			}
		} else if(gain < 0) {
			gain = MAX(-max_exp_loss, gain);	/* Cap max exp lost per death */
			GET_EXP(ch) += gain;

			if(GET_EXP(ch) < 0) {
				GET_EXP(ch) = 0;
			}
		}
	}


	void gain_exp_regardless(char_data *ch, int gain) {
		int is_altered = FALSE;
		int num_levels = 0;

		GET_EXP(ch) += gain;

		if(GET_EXP(ch) < 0) {
			GET_EXP(ch) = 0;
		}

		if(!IS_NPC(ch)) {
			while(GET_LEVEL(ch) < LVL_IMPL &&
			        GET_EXP(ch) >= mods::levels::level_exp(GET_LEVEL(ch) + 1)) {
				GET_LEVEL(ch) += 1;
				num_levels++;
				advance_level(ch);
				is_altered = TRUE;
			}

			if(is_altered) {
				mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "%s advanced %d level%s to level %d.",
				       GET_NAME(ch).c_str(), num_levels, num_levels == 1 ? "" : "s", GET_LEVEL(ch));

				if(num_levels == 1) {
					send_to_char(ch, "You rise a level!");
				} else {
					send_to_char(ch, "You rise %d levels!", num_levels);
				}

				set_title(ch, NULL);

				if(GET_LEVEL(ch) >= LVL_IMMORT) {
					run_autowiz();
				}
			}
		}
	}
};//end legacy


void gain_condition(char_data *ch, int condition, int value) {
	bool intoxicated;

	if(IS_NPC(ch) || GET_COND(ch, condition) == -1) {	/* No change */
		return;
	}

	intoxicated = (GET_COND(ch, DRUNK) > 0);

	GET_COND(ch, condition) += value;

	GET_COND(ch, condition) = MAX(0, GET_COND(ch, condition));
	GET_COND(ch, condition) = MIN(24, GET_COND(ch, condition));

	if(GET_COND(ch, condition) || PLR_FLAGGED(ch, PLR_WRITING)) {
		return;
	}

	switch(condition) {
		case FULL:
			send_to_char(ch, "You are hungry.");
			break;

		case THIRST:
			send_to_char(ch, "You are thirsty.");
			break;

		case DRUNK:
			if(intoxicated) {
				send_to_char(ch, "You are now sober.");
			}

			break;

		default:
			break;
	}

}



void check_idling(player_ptr_t player) {
	auto ch = player->cd();
	if(++(player->char_specials().timer) > idle_void) {
		if(GET_WAS_IN(ch) == NOWHERE && IN_ROOM(ch) != NOWHERE) {
			GET_WAS_IN(ch) = IN_ROOM(ch);

			if(FIGHTING(ch)) {
				stop_fighting(FIGHTING(ch));
				stop_fighting(ch);
			}

			/*!mods*/
			if(!ch->drone) {
				act("$n disappears into the void.", TRUE, ch, 0, 0, TO_ROOM);
				player->stc("You have been idle, and are pulled into a void.\r\n");
				mods::db::save_char(player);
				char_from_room(ch);
				char_to_room(ch, mods::world_conf::real_idle());
			}
		} else if(ch->char_specials.timer > idle_rent_time) {
			if(IN_ROOM(ch) != NOWHERE) {
				char_from_room(ch);
			}

			char_to_room(ch, mods::world_conf::real_idle());

			if(ch->has_desc) {
				ch->desc->set_state(CON_IDLE);
				/*
				 * For the 'if (d.character)' test in close_socket().
				 * -gg 3/1/98 (Happy anniversary.)
				 */
			}

#ifdef __MENTOC_RENT_DYNAMICS__
			if(free_rent) {
				Crash_rentsave(ch, 0);
			} else {
				Crash_idlesave(ch);
			}
			mudlog(CMP, LVL_GOD, TRUE, "%s force-rented and extracted (idle).", GET_NAME(ch).c_str());
			extract_char(ch);
#else
			auto save_status = mods::db::save_char(player);
			if(std::get<0>(save_status) == false) {
				std::cerr << "check_idling[db::save_char]->error saving character\n";
			}
#endif
		}
	}
}


ACMD(do_idle) {

	player->char_specials().timer = idle_void + 1;
	check_idling(player);
}

/* Update PCs, NPCs, and objects */
void point_update(void) {
	struct obj_data *jj, *next_thing2;

	/* characters */
	mods::loops::foreach_all([&](player_ptr_t player) -> bool {
		auto ch = player->cd();
		if(!ch) {
			log("foreach_all recieved null char_data ptr");
			return true;
		}
		auto i = ch;
#ifdef ENABLE_MENTOC_HUNGRY__
		gain_condition(i, FULL, -1);
#endif
		gain_condition(i, DRUNK, -1);
#ifdef ENABLE_MENTOC_THIRSTY__
		gain_condition(i, THIRST, -1);
#endif

		if(GET_POS(i) >= POS_STUNNED) {
			GET_HIT(i) = MIN(GET_HIT(i) + hit_gain(i), GET_MAX_HIT(i));
			GET_MANA(i) = MIN(GET_MANA(i) + mana_gain(i), GET_MAX_MANA(i));
			GET_MOVE(i) = MIN(GET_MOVE(i) + move_gain(i), GET_MAX_MOVE(i));
			if(AFF_FLAGGED(i, AFF_POISON)) {
				if(damage(i, i, 2, SPELL_POISON) == -1) {
					//log("player died of poison");
					return true;    /* Oops, they died. -gg 6/24/98 */
				}
			}

			if(GET_POS(i) <= POS_STUNNED) {
				update_pos(i);
			}
		} else if(GET_POS(i) == POS_INCAP) {
			if(damage(i, i, 1, TYPE_SUFFERING) == -1) {
				return true;
			}
		} else if(GET_POS(i) == POS_MORTALLYW) {
			if(damage(i, i, 2, TYPE_SUFFERING) == -1) {
				return true;
			}
		}

		if(!IS_NPC(i)) {
			update_char_objects(i);

			if(GET_LEVEL(i) < idle_max_level) {
				check_idling(player);
			}
		}
		return true;
	});

	/* objects */
	for(auto it = obj_list.begin(); it != obj_list.end(); ++it) {
		auto j = it->get();
		if(!j) {
			log("SYSERR: obj_list has null object.. attempting to remove...");
			obj_list.erase(it);
			continue;
		}

#ifdef __MENTOC_SHOW_IS_CORPSE_ON_ALL_OBJECTS__
		std::cerr << "IS_CORPSE: " << j->uuid << ", :'" << j->name.c_str() << "'\n";
#endif
		/* If this is a corpse */
		if(IS_CORPSE(j)) {
			/* timer count down */
			if(GET_OBJ_TIMER(j) > 0) {
				GET_OBJ_TIMER(j)--;
			}

			if(!GET_OBJ_TIMER(j)) {

				if(j->carried_by) {
					act("$p decays in your hands.", FALSE, j->carried_by, j, 0, TO_CHAR);
				} else if((IN_ROOM(j) != NOWHERE) && (world[IN_ROOM(j)].people)) {
					act("A quivering horde of maggots consumes $p.",
					    TRUE, world[IN_ROOM(j)].people, j, 0, TO_ROOM);
					act("A quivering horde of maggots consumes $p.",
					    TRUE, world[IN_ROOM(j)].people, j, 0, TO_CHAR);
				}

				for(jj = j->contains; jj; jj = next_thing2) {
					next_thing2 = jj->next_content;	/* Next in inventory */
					obj_from_obj(jj);

					if(j->in_obj) {
						obj_to_obj(optr(jj), optr(j->in_obj));
					} else if(j->carried_by) {
						obj_to_room(jj, IN_ROOM(j->carried_by));
					} else if(IN_ROOM(j) != NOWHERE) {
						obj_to_room(jj, IN_ROOM(j));
					} else {
						core_dump();
					}
				}

				extract_obj(j);
			}
		}
	}//end for object_list
}
