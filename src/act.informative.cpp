/* ************************************************************************
*   File: act.informative.c                             Part of CircleMUD *
*  Usage: Player-level commands of an informative nature                  *
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
#include "screen.h"
#include "constants.h"
#include "globals.hpp"
#include "mods/extern.hpp"
#include "mods/quests.hpp"
#include "mods/drone.hpp"
#include "mods/util.hpp"
#include "mods/world-configuration.hpp"
#include "mods/object-utils.hpp"

extern char_data* character_list;
/* extern variables */
extern int top_of_helpt;
extern struct help_index_element *help_table;
extern char *help;
extern struct time_info_data time_info;

extern char *credits;
extern char *news;
extern char *info;
extern std::string motd;
extern char *imotd;
extern char *wizlist;
extern char *immlist;
extern char *policies;
extern char *handbook;
extern char *class_abbrevs[];

/* extern functions */
ACMD(do_action);
ACMD(do_insult);
bitvector_t find_class_bitvector(const char *arg);
int level_exp(int chclass, int level);
char *title_male(int chclass, int level);
char *title_female(int chclass, int level);
struct time_info_data *real_time_passed(time_t t2, time_t t1);
int compute_armor_class(char_data *ch);

/* local functions */
int sort_commands_helper(const void *a, const void *b);
void print_object_location(int num, struct obj_data *obj, char_data *ch, int recur);
void show_obj_to_char(struct obj_data *obj, char_data *ch, int mode);
void list_obj_to_char(struct obj_data *list, char_data *ch, int mode, int show);
void show_obj_modifiers(struct obj_data *obj, char_data *ch);
ACMD(do_js);	/*!mods*/
ACMD(do_quest);	/*!mods*/
ACMD(do_recall);	/*!mods*/
ACMD(do_givemegold); /*!mods*/
ACMD(do_drone);	/*!mods*/
ACMD(do_look);
ACMD(do_examine);
ACMD(do_gold);
ACMD(do_score);
ACMD(do_inventory);
ACMD(do_equipment);
ACMD(do_time);
ACMD(do_weather);
ACMD(do_help);
ACMD(do_who);
ACMD(do_users);
ACMD(do_gen_ps);
ACMD(do_preferences); /*!mods */
void perform_mortal_where(char_data *ch, char *arg);
void perform_immort_where(char_data *ch, char *arg);
ACMD(do_where);
ACMD(do_levels);
ACMD(do_consider);
ACMD(do_diagnose);
ACMD(do_color);
ACMD(do_toggle);
void sort_commands(void);
ACMD(do_commands);
void diag_char_to_char(char_data *i, char_data *ch);
void look_at_char(char_data *i, char_data *ch);
void list_one_char(char_data *i, char_data *ch);
void list_char_to_char(char_data *ch);
void do_auto_exits(char_data *ch);
ACMD(do_exits);
void look_in_direction(char_data *ch, int dir);
void look_in_obj(char_data *ch, char *arg);
char *find_exdesc(std::string_view word,const std::vector<extra_descr_data>& in_list);
void look_at_target(char_data *ch, char *arg);

/* local globals */
int *cmd_sort_info;

/* For show_obj_to_char 'mode'.	/-- arbitrary */
#define SHOW_OBJ_LONG		0
#define SHOW_OBJ_SHORT		1
#define SHOW_OBJ_ACTION		2

ACMD(do_drone) {
	MENTOC_PREAMBLE();
	CREATE_ARG(16,1);
	one_argument(argument, static_cast<char*>(&arg_1[0]),16);

	if(std::string(static_cast<char*>(&arg_1[0])).compare("start") == 0) {
		/* if user has a drone out already, ressurrect that drone */
		if(!mods::drone::started(ch)) {
			mods::drone::start(ch);
		}

		return;
	}

	if(std::string(static_cast<char*>(&arg_1[0])).compare("stop") == 0) {
		/* if user has a drone out already, ressurrect that drone */
		mods::drone::stop(ch);
		return;
	}

	*player << "usage: drone <start|stop>\r\n";
}

ACMD(do_givemegold) {
	MENTOC_PREAMBLE();
	ch->points.gold += 50000;
}

ACMD(do_recall) {
	MENTOC_PREAMBLE();
	if(mods::globals::boot_type == mods::globals::boot_type_t::BOOT_HELL){
		*player << "Ah Ah Ahh... You didn't say the magic word...";
		return;
	}

	//TODO: combat cooldown should prevent recall for 20 seconds
	if(time(NULL) < ch->last_fight_timestamp + 20) {
		*player << "Cannot recall this soon after combat.";
	} else {
		char_from_room(ch);
		char_to_room(ch,mods::world_conf::real_mortal_start());
		look_at_room(ch,0);
	}
}

ACMD(do_quest) {
	MENTOC_PREAMBLE();
	CREATE_ARG(16,1);
	one_argument(argument, static_cast<char*>(&arg_1[0]),16);

	if(std::string(static_cast<char*>(&arg_1[0])).compare("list") == 0) {
		auto quest_names = mods::quests::list_quests(IN_ROOM(ch));

		for(auto qn : quest_names) {
			*player << "{grn}[ QUEST ]{/grn} " << qn << "";
		}

		return;
	}

	if(std::string(static_cast<char*>(&arg_1[0])).compare("join") == 0) {
		CREATE_ARG(4,2);
		two_arguments(argument,static_cast<char*>(&arg_1[0]),static_cast<char*>(&arg_2[0]));
		std::string arg_2_str = static_cast<char*>(&arg_2[0]);

		if(arg_2_str.length() == 0) {
			*player << "usage: quest join <number>\n";
			return;
		}

		auto quest_num = mods::util::stoi(arg_2_str);

		if(quest_num.value_or(-1) == -1) {
			*player << "{red}Invalid quest number\r\n";
			return;
		}

		mods::quests::start_quest(ch,quest_num.value());
	}

	if(std::string(static_cast<char*>(&arg_1[0])).compare("leave") == 0) {
//TODO: when multiple quests are allowed, this hard-coded zero needs to change
		CREATE_ARG(4,2);
		two_arguments(argument,static_cast<char*>(&arg_1[0]),static_cast<char*>(&arg_2[0]));
		std::string arg_2_str = static_cast<char*>(&arg_2[0]);

		if(arg_2_str.length() == 0) {
			*player << "usage: quest leave <number>\n";
			return;
		}

		auto quest_num = mods::util::stoi(arg_2_str);

		if(quest_num.value_or(-1) == -1) {
			*player << "{red}Invalid quest number\r\n";
			return;
		}

		mods::quests::leave_quest(ch,quest_num.value());
		*player << "{red}You have left the quest{/red}\n";
	}
}

ACMD(do_preferences) {
	MENTOC_PREAMBLE();

}

ACMD(do_js) {
	MENTOC_PREAMBLE();
	mods::js::eval_string(std::string(argument) + ";");
}

ACMD(do_newjs) {
	MENTOC_PREAMBLE();
	mods::globals::duktape_context = mods::js::new_context();
	mods::js::load_base_functions(mods::globals::duktape_context);
	mods::js::load_c_require_functions(mods::globals::duktape_context);
}
ACMD(do_jstest) {
	MENTOC_PREAMBLE();

	if(!mods::js::run_test_suite(*player,argument)) {
		*player << "{red}Unable to load test suite{/red}\r\n";
		return;
	}

	*player << "Test suite loaded...\r\n";
	return;
}
void show_obj_to_char(struct obj_data *obj, char_data *ch, int mode) {
	MENTOC_PREAMBLE();
	if(!obj || !ch) {
		log("SYSERR: NULL pointer in show_obj_to_char(): obj=%p ch=%p", obj, ch);
		return;
	}

	if(mods::object_utils::is_camera(obj) && obj->location_data() >= 16) {
		player->send("A %s is installed on the %s wall.\r\n", obj->name.c_str(), mods::globals::dir_to_str(obj->location_data() - 16, true).c_str());
		return;
	}

	if(mods::object_utils::is_claymore(obj) && obj->location_data() >= 16) {
		player->send("A %s is installed at the foot of the %s entrance.\r\n", obj->name.c_str(), mods::globals::dir_to_str(obj->location_data() - 16, true).c_str());
		return;
	}

	switch(mode) {
		case SHOW_OBJ_LONG:
			if(obj->description.length()){
				player->sendln(obj->description);
			}else{
				player->sendln(obj->name);
			}
			break;

		case SHOW_OBJ_SHORT:
			if(obj->short_description.length()){
				player->sendln(obj->short_description);
			}else{
				player->sendln(obj->name);
			}
			break;

		case SHOW_OBJ_ACTION:
			switch(GET_OBJ_TYPE(obj)) {
				case ITEM_NOTE:
					if(obj->action_description) {
						char notebuf[MAX_NOTE_LENGTH + 64];

						snprintf(notebuf, sizeof(notebuf), "There is something written on it:\r\n\r\n%s", obj->action_description.c_str());
						page_string(*ch->desc, notebuf, TRUE);
					} else {
						player->sendln("It's blank");
					}

					return;

				case ITEM_DRINKCON:
					player->send("It looks like a drink container.");
					break;

				case ITEM_WEAPON:
					if(obj->action_description.length()) {
						player->send(obj->action_description);
						break;
					}
					/** Purposeful fall-through to display ".. nothing special.." */
				default:
					player->send("You see nothing special..");
					break;
			}

			break;

		default:
			log("SYSERR: Bad display mode (%d.in show_obj_to_char().", mode);
			return;
	}

	show_obj_modifiers(obj, ch);
}


void show_obj_modifiers(struct obj_data *obj, char_data *ch) {
	MENTOC_PREAMBLE();
	if(OBJ_FLAGGED(obj, ITEM_INVISIBLE)) {
		player->send( " (invisible)");
	}

	if(OBJ_FLAGGED(obj, ITEM_BLESS) && AFF_FLAGGED(ch, AFF_DETECT_ALIGN)) {
		player->send( " ..It glows blue!");
	}

	if(OBJ_FLAGGED(obj, ITEM_MAGIC) && AFF_FLAGGED(ch, AFF_DETECT_MAGIC)) {
		player->send( " ..It glows yellow!");
	}

	if(OBJ_FLAGGED(obj, ITEM_GLOW)) {
		player->send( " ..It has a soft glowing aura!");
	}

	if(OBJ_FLAGGED(obj, ITEM_HUM)) {
		player->send( " ..It emits a faint humming sound!");
	}
}


void list_obj_to_char(struct obj_data *list, char_data *ch, int mode, int show) {
	MENTOC_PREAMBLE();
	struct obj_data *i;
	bool found = FALSE;

	for(i = list; i; i = i->next_content) {
		if(!i){
			break;
		}
		if(CAN_SEE_OBJ(ch, i)) {
			show_obj_to_char(i, ch, mode);
			found = TRUE;
		}
	}

	if(!found && show) {
		player->sendln( " Nothing.");
	}
}


void diag_char_to_char(char_data *i, char_data *ch) {
	MENTOC_PREAMBLE();
	struct {
		byte percent;
		const char *text;
	} diagnosis[] = {
		{ 100, "is in excellent condition."			},
		{  90, "has a few scratches."			},
		{  75, "has some small wounds and bruises."		},
		{  50, "has quite a few wounds."			},
		{  30, "has some big nasty wounds and scratches."	},
		{  15, "looks pretty hurt."				},
		{   0, "is in awful condition."			},
		{  -1, "is bleeding awfully from big wounds."	},
	};
	int percent, ar_index;
	mods::string pers;
	if(i == ch){
		pers = i->player.name.c_str();
	}else{
		pers = PERS(i, ch);
		if(!pers){
			pers = "Someone";
		}
	}

	if(GET_MAX_HIT(i) > 0) {
		percent = (100 * GET_HIT(i)) / GET_MAX_HIT(i);
	} else {
		percent = -1;    /* How could MAX_HIT be < 1?? */
	}

	for(ar_index = 0; diagnosis[ar_index].percent >= 0; ar_index++){
		if(percent >= diagnosis[ar_index].percent) {
			break;
		}
	}

	player->send("%s %s\r\n", pers.c_str(), diagnosis[ar_index].text);
}


void look_at_char(char_data *i, char_data *ch) {
	MENTOC_PREAMBLE();
	int j, found = false;
	struct obj_data *tmp_obj;

	if(!ch->has_desc) {
		return;
	}

	if(i->player.description) {
		player->send(i->player.description);
	} else {
		act("You see nothing special about $m.", FALSE, i, 0, ch, TO_VICT);
	}

	diag_char_to_char(i, ch);

	found = FALSE;

	auto iptr = ptr(i);
		act("\r\n$n is using:", FALSE, i, 0, ch, TO_VICT);
		for(j = 0; j < NUM_WEARS; j++){
			auto obj = iptr->equipment(j);
			if(obj){
				player->send(wear_where[j]);
				//if(!CAN_SEE_OBJ(ch, obj.get())) {
				//	player->sendln("<unknown>");
				//} else {
					show_obj_to_char(obj.get(), ch, SHOW_OBJ_SHORT);
				//}
			}
			/*else{
				player->sendln("<nothing>");
			}
			*/
		}

	if(ch != i && (IS_THIEF(ch) || GET_LEVEL(ch) >= LVL_IMMORT)) {
		found = FALSE;
		act("\r\nYou attempt to peek at $s inventory:", FALSE, i, 0, ch, TO_VICT);

		for(tmp_obj = i->carrying; tmp_obj; tmp_obj = tmp_obj->next_content) {
			if(CAN_SEE_OBJ(ch, tmp_obj) && (rand_number(0, 20) < GET_LEVEL(ch))) {
				show_obj_to_char(tmp_obj, ch, SHOW_OBJ_SHORT);
				found = TRUE;
			}
		}

		if(!found) {
			player->sendln("You can't see anything.");
		}
	}
}


void list_one_char(char_data *i, char_data *ch) {
	MENTOC_PREAMBLE();
	const char *positions[] = {
		" is lying here, dead.",
		" is lying here, mortally wounded.",
		" is lying here, incapacitated.",
		" is lying here, stunned.",
		" is sleeping here.",
		" is resting here.",
		" is sitting here.",
		"!FIGHTING!",
		" is standing here."
	};

	if(IS_NPC(i)) {
		player->stc(i->player.short_descr);
	} else {
		player->stc(i->player.name.str() + (GET_TITLE(i).length() ? " " + GET_TITLE(i).str() : ""));
	}

	if(AFF_FLAGGED(i, AFF_INVISIBLE)) {
		player->send(" (invisible)");
	}

	if(AFF_FLAGGED(i, AFF_HIDE)) {
		player->send( " (hidden)");
	}

	if(!IS_NPC(i) && !i->has_desc) {
		player->send( " (linkless)");
	}

	if(!IS_NPC(i) && PLR_FLAGGED(i, PLR_WRITING)) {
		player->send( " (writing)");
	}

	if(i->drone) {
		player->send(" is idling here.");
	} else {
		if(GET_POS(i) != POS_FIGHTING) {
			player->send(positions[(int) GET_POS(i)]);
		} else {
			if(FIGHTING(i)) {
				player->send( " is here, fighting ");

				if(FIGHTING(i) == ch) {
					player->send( "YOU!");
				} else {
					if(IN_ROOM(i) == IN_ROOM(FIGHTING(i))) {
						player->send("%s!",PERS(FIGHTING(i), ch));
					} else {
						player->send(  "someone who has already left!");
					}
				}
			} else {		/* NIL fighting pointer */
				player->send( " is here struggling with thin air.");
			}
		}
	}

	if(AFF_FLAGGED(ch, AFF_DETECT_ALIGN)) {
		if(IS_EVIL(i)) {
			player->send( " (Red Aura)");
		} else if(IS_GOOD(i)) {
			player->send( " (Blue Aura)");
		}
	}

	player->sendln("");


	if(AFF_FLAGGED(i, AFF_SANCTUARY)) {
		act("...$e glows with a bright light!", FALSE, i, 0, ch, TO_VICT);
	}
}



void list_char_to_char(char_data *ch) {
	MENTOC_PREAMBLE();
	int room = player->viewing_room();
	for(auto & player_ptr : mods::globals::get_room_list(room)){
		auto i = player_ptr->cd();
		if(player_ptr->name().compare(ch->player.name.c_str())) {
			if(CAN_SEE(ch, i)) {
				list_one_char(i, ch);
			} else if(IS_DARK(room) && !CAN_SEE_IN_DARK(ch) &&
			          AFF_FLAGGED(i, AFF_INFRAVISION)) {
				player->sendln( "You see a pair of glowing red eyes looking your way.");
			}
		}
	}
}


void do_auto_exits(char_data *ch) {
	MENTOC_PREAMBLE();
	int door, slen = 0;

	player->stc( "{gld}[ Exits: ");

	char_data* dummy = ch;
	auto cached_room = IN_ROOM(ch);
	IN_ROOM(dummy) = player->viewing_room();
	for(door = 0; door < NUM_OF_DIRS; door++) {
		if(!EXIT(dummy, door)) {
			continue;
		}

		if(EXIT(dummy, door)->to_room == NOWHERE) {
			continue;
		}

		if(EXIT_FLAGGED(EXIT(dummy, door), EX_CLOSED) &&
		        /*! mods */!EXIT_FLAGGED(EXIT(dummy,door),EX_BREACHED) &&
		        /*! mods */!IS_SET(world[EXIT(dummy,door)->to_room].dir_option[OPPOSITE_DIR(door)]->exit_info,EX_BREACHED)) {
			continue;
		}

		player->send("%c ",TOSTR(dirs[door])[0]);
		slen++;
	}

	player->send("%s]{/gld}\r\n", slen ? "" : "None!");
	IN_ROOM(ch) = cached_room;
}


ACMD(do_exits) {
	MENTOC_PREAMBLE();
	int door, len = 0;

	if(AFF_FLAGGED(ch, AFF_BLIND)) {
		player->sendln( "You can't see a damned thing, you're blind!");
		return;
	}

	player->sendln( "Obvious exits:");

	for(door = 0; door < NUM_OF_DIRS; door++) {
		if(!EXIT(ch, door) || EXIT(ch, door)->to_room == NOWHERE) {
			continue;
		}

		if(EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED)) {
			continue;
		}

		len++;

		if(GET_LEVEL(ch) >= LVL_IMMORT){
			player->send("%-5s - [%5d] %s\r\n", dirs[door], GET_ROOM_VNUM(EXIT(ch, door)->to_room),
					world[EXIT(ch, door)->to_room].name.c_str());
		}else{
			player->send("%-5s - %s\r\n", dirs[door], IS_DARK(EXIT(ch, door)->to_room) &&
					!CAN_SEE_IN_DARK(ch) ? "Too dark to tell." : world[EXIT(ch, door)->to_room].name.c_str());
		}	
	}

	if(!len) {
		player->sendln(" None.");
	}
}



void look_at_room(char_data *ch, int ignore_brief) {
	MENTOC_PREAMBLE();

	if(!ch->has_desc) {
		d("look_at_room: char doesnt have a desc! WTF");
		return;
	}

	int room = player->viewing_room();

	if(room < 0){
		player->sendln("invalid room id");
		return;
	}

	if(world.size() <= std::size_t(room)){
		d("look_at_room[world.size()<=IN_ROOM(ch)]->world.size():'" <<
				world.size() << "'|IN_ROOM(ch):'" << room << "'");
		return;
	}

	player->sendln(CCCYN(ch, C_NRM));

	if(!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_ROOMFLAGS)) {
		char buf[MAX_STRING_LENGTH];

		sprintbit(ROOM_FLAGS(room), room_bits, buf, sizeof(buf));
		player->send("[%5d] %s [ %s]", GET_ROOM_VNUM(room), world[room].name.c_str(), buf);
	} else {
		player->stc_room(room);
	}

	player->sendln(CCNRM(ch, C_NRM));

	if((!IS_NPC(ch) && !PRF_FLAGGED(ch, PRF_BRIEF)) || ignore_brief ||
			ROOM_FLAGGED(room, ROOM_DEATH)) {
		player->stc_room_desc(room);
	}

	/* autoexits */
	if(!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_AUTOEXIT)) {
		do_auto_exits(ch);
	}

	/* TODO: show map */

	/* now list characters & objects */
	player->send(CCGRN(ch, C_NRM));
	list_obj_to_char(world[room].contents, ch, SHOW_OBJ_LONG, FALSE);
	player->send(CCYEL(ch, C_NRM));
	list_char_to_char(ch);
	player->send(CCNRM(ch, C_NRM));
}



void look_in_direction(char_data *ch, int dir) {
	MENTOC_PREAMBLE();
	if(EXIT(ch, dir)) {
		if(EXIT(ch, dir)->general_description) {
			player->send(EXIT(ch, dir)->general_description);
		} else {
			player->sendln( "You see nothing special.");
		}

		if(EXIT_FLAGGED(EXIT(ch, dir), EX_CLOSED) && EXIT(ch, dir)->keyword) {
			player->send("The %s is closed.\r\n", fname(EXIT(ch, dir)->keyword));
		} else if(EXIT_FLAGGED(EXIT(ch, dir), EX_ISDOOR) && EXIT(ch, dir)->keyword) {
			player->send("The %s is open.\r\n", fname(EXIT(ch, dir)->keyword));
		}
	} else {
		player->sendln( "Nothing special there...");
	}
}



void look_in_obj(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	struct obj_data *obj = NULL;
	char_data *dummy = NULL;
	int amt, bits;

	if(!*arg) {
		player->sendln( "Look in what?");
	} else if(!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM |
					FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
		player->send("There doesn't seem to be %s %s here.\r\n", AN(arg), arg);
	} else if(obj->rifle() && obj->obj_flags.holds_ammo){
		player->sendln(std::string(TOSTR(obj->obj_flags.ammo) + " rounds.").c_str());
	} else if((GET_OBJ_TYPE(obj) != ITEM_DRINKCON) &&
			(GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN) &&
			(GET_OBJ_TYPE(obj) != ITEM_CONTAINER)) {
		player->sendln( "There's nothing inside that!");
	} else {
		if(GET_OBJ_TYPE(obj) == ITEM_CONTAINER) {
			if(OBJVAL_FLAGGED(obj, CONT_CLOSED)) {
				player->sendln( "It is closed.");
			} else {
				player->send( fname(obj->name));

				switch(bits) {
					case FIND_OBJ_INV:
						player->sendln( " (carried): ");
						break;

					case FIND_OBJ_ROOM:
						player->sendln( " (here): ");
						break;

					case FIND_OBJ_EQUIP:
						player->sendln( " (used): ");
						break;
				}

				list_obj_to_char(obj->contains, ch, SHOW_OBJ_SHORT, TRUE);
			}
		} else {		/* item must be a fountain or drink container */
			if(GET_OBJ_VAL(obj, 1) <= 0) {
				player->sendln( "It is empty.");
			} else {
				if(GET_OBJ_VAL(obj,0) <= 0 || GET_OBJ_VAL(obj,1)>GET_OBJ_VAL(obj,0)) {
					player->sendln( "Its contents seem somewhat murky."); /* BUG */
				} else {
					char buf2[MAX_STRING_LENGTH];
					amt = (GET_OBJ_VAL(obj, 1) * 3) / GET_OBJ_VAL(obj, 0);
					sprinttype(GET_OBJ_VAL(obj, 2), color_liquid, buf2, sizeof(buf2));
					player->send("It's %sfull of a %s liquid.\r\n", fullness[amt], buf2);
				}
			}
		}
	}
}

const char* find_exdesc_equipment(std::string_view word,const char_data* ch,const std::size_t& position){
	MENTOC_PREAMBLE();
	log("find_exdesc_equipment");
	auto obj = player->equipment(position);
	if(!obj){
		d("obj is null-ish. returning from findexdesc_eq");
		return nullptr;
	}
	for(const auto & i : obj->ex_description){
		d("obj exdesc (equipment) looping");
		if(i.keyword.length() == 0 || i.description.length() == 0){
			d("one of these fuckers is zero");
			continue;
		}
		if(isname(word.data(), i.keyword.c_str())) {
			d("found by is_name - findexdesc_eq");
			return (i.description.c_str());
		}
	}
	d("returning nullptr from fexequip");
	return nullptr;
}
char* find_exdesc(std::string_view word,std::vector<extra_descr_data>& in_list) {
	log("find_exdesc, word: %s (vector)",word);
	d("bruh");
	if(in_list.size() == 0){
		d("in_list.size is zero");
		return nullptr;
	}
	for(auto & i : in_list){
		d("loop auto & i in_list");
		if(i.keyword.length() == 0 || i.description.length() == 0){
			d("one of these fuckers is zero");
			continue;
		}
		if(isname(word.data(), i.keyword.c_str())) {
			return (i.description.ptr());
		}
	}

	return nullptr;
}

char * find_exdesc(char *word,room_data& r){
	log("find_exdesc, word: %s (room_data)",word);
	if(r.ex_descriptions().size() == 0){
		d("find_exdesc room_data ex desc is zero");
		return nullptr;
	}
	for(auto & i : r.ex_descriptions()){
		d("LOL");
		if(isname(word, i.keyword.c_str())) {
			d("ROFL");
			return (i.description.ptr());
		}
	}
	d("yup");
	return nullptr;
}


/*
 * Given the argument "look at <target>", figure out what object or char
 * matches the target.  First, see if there is another char in the room
 * with the name.  Then check local objs for exdescs.
 *
 * Thanks to Angus Mezick <angus@EDGIL.CCMAIL.COMPUSERVE.COM> for the
 * suggested fix to this problem.
 */
void look_at_target(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	int bits, found = FALSE, j, fnum, i = 0;
	char_data *found_char = NULL;
	obj_data *obj, *found_obj = NULL;
	char *desc;

	if(!ch->has_desc) {
		return;
	}

	if(!*arg) {
		player->sendln( "Look at what?");
		return;
	}

	bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_OBJ_EQUIP |
			FIND_CHAR_ROOM, ch, &found_char, &found_obj);

	/* Is the target a character? */
	if(found_char != NULL) {
		look_at_char(found_char, ch);

		if(ch != found_char) {
			if(CAN_SEE(found_char, ch)) {
				act("$n looks at you.", TRUE, ch, 0, found_char, TO_VICT);
			}

			act("$n looks at $N.", TRUE, ch, 0, found_char, TO_NOTVICT);
		}

		return;
	}

	/* Strip off "number." from 2.foo and friends. */
	if(!(fnum = get_number(&arg))) {
		player->sendln( "Look at what?");
		return;
	}

	/* Does the argument match an extra desc in the room? */
	d("world inroomch findex");
	if((desc = find_exdesc(arg, world[IN_ROOM(ch)])) != NULL) {
		d("page string");
		page_string(*ch->desc, desc, FALSE);
		return;
	}

	d("num wears");
	/* Does the argument match an extra desc in the char's equipment? */
	for(j = 0; j < NUM_WEARS && !found; j++){
		if(GET_EQ(ch, j) && CAN_SEE_OBJ(ch, GET_EQ(ch, j)) && GET_EQ(ch, j)->ex_description.size()){
			d("get eq ok");
			d("eq exdesc.size: " << GET_EQ(ch,j)->ex_description.size());
			//break;
			const char* ex = find_exdesc_equipment(arg, ch, j);
			d("fetched ex");
			if(!ex){d("cont"); continue; }
			d("num wears enumeration exdesc");
			//if(++i == fnum) {
				d("sending ex");
				player->send(ex);
				found = TRUE;
				break;
			//}
		}
	}
	d("carrying");

	/* Does the argument match an extra desc in the char's inventory? */
	for(auto & item : player->real_carrying()){
		if(item && CAN_SEE_OBJ(ch, item.get())){
			d("carrying enumeration exdesc (new for auto loop)");
			auto ex = find_exdesc(arg, item->ex_description);
			if(!ex){
				d("cont"); continue;
			}
			player->send(ex);
			found = true;break;
		}
	}

	d("world contents");
	/* Does the argument match an extra desc of an object in the room? */
	assert(IN_ROOM(ch) < world.size());
	//if(world[IN_ROOM(ch)].contents){

	//}
	for(obj = world[IN_ROOM(ch)].contents; obj && !found && obj->next_content; obj = obj->next_content){
		log("checking can see obj ch[%s] obj[%s]",ch->player.name.c_str(),obj->name.c_str());
		if(CAN_SEE_OBJ(ch, obj)){
			log("can see obj good. searching exdesc");
			if((desc = find_exdesc(arg, obj->ex_description)) != NULL && ++i == fnum) {
				player->sendln( desc);
				found = TRUE;
			}
		}
	}

	d("bits");
	/* If an object was found back in generic_find */
	if(bits) {
		if(!found) {
			show_obj_to_char(found_obj, ch, SHOW_OBJ_ACTION);
		} else {
			show_obj_modifiers(found_obj, ch);
			player->sendln( "");
		}
	} else if(!found) {
		player->sendln( "You do not see that here.");
	}
}


ACMD(do_look) {
	MENTOC_PREAMBLE();
	int look_type;

	if(GET_POS(ch) < POS_SLEEPING) {
		player->sendln( "You can't see anything but stars!");
	} else if(AFF_FLAGGED(ch, AFF_BLIND)) {
		player->sendln( "You can't see a damned thing, you're blind!");
	} else if(IS_DARK(IN_ROOM(ch)) && !CAN_SEE_IN_DARK(ch)) {
		player->sendln( "It is pitch black...");
		list_char_to_char(ch);	/* glowing red eyes */
	} else {
		char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

		half_chop(argument, arg, arg2);

		if(subcmd == SCMD_READ) {
			if(!*arg) {
				player->sendln( "Read what?");
			} else {
				look_at_target(ch, arg);
			}
			return;
		}

		if(!*arg) {		/* "look" alone, without an argument at all */
			look_at_room(ch, 1);
		} else if(is_abbrev(arg, "in")) {
			look_in_obj(ch, arg2);
		}
		/* did the char type 'look <direction>?' */
		else if((look_type = search_block(arg, dirs, FALSE)) >= 0) {
			look_in_direction(ch, look_type);
		} else if(is_abbrev(arg, "at")) {
			look_at_target(ch, arg2);
		} else {
			look_at_target(ch, arg);
		}
	}
}



ACMD(do_examine) {
	MENTOC_PREAMBLE();
	char_data *tmp_char;
	struct obj_data *tmp_object;
	char tempsave[MAX_INPUT_LENGTH], arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln( "Examine what?");
		return;
	}

	/* look_at_target() eats the number. */
	look_at_target(ch, strcpy(tempsave, arg));	/* strcpy: OK */

	generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_CHAR_ROOM |
			FIND_OBJ_EQUIP, ch, &tmp_char, &tmp_object);

	if(tmp_object) {
		if((GET_OBJ_TYPE(tmp_object) == ITEM_DRINKCON) ||
				(GET_OBJ_TYPE(tmp_object) == ITEM_FOUNTAIN) ||
				(GET_OBJ_TYPE(tmp_object) == ITEM_CONTAINER)) {
			player->sendln( "When you look inside, you see:");
			look_in_obj(ch, arg);
		}
	}
}



ACMD(do_gold) {
	MENTOC_PREAMBLE();
	if(GET_GOLD(ch) == 0) {
		player->sendln( "You're broke!");
	} else if(GET_GOLD(ch) == 1) {
		player->sendln( "You have one miserable little gold coin.");
	} else {
		player->sendln( TOSTR("You have ") + TOSTR(player->gold()) + " gold coins.");
	}
}


ACMD(do_score) {
	MENTOC_PREAMBLE();
	struct time_info_data playing_time;

	if(IS_NPC(ch)) {
		return;
	}

	/** TODO: implement player::age() */
	player->sendln(TOSTR("You are ") + TOSTR(GET_AGE(player->cd())) + " years old.");

	if(age(ch)->month == 0 && age(ch)->day == 0) {
		player->sendln( "  It's your birthday today.");
	} else {
		player->sendln( "");
	}

	player->send("You have %d(%d) hit, %d(%d) mana and %d(%d) movement points.\r\n",
			GET_HIT(ch), GET_MAX_HIT(ch), GET_MANA(ch), GET_MAX_MANA(ch),
			GET_MOVE(ch), GET_MAX_MOVE(ch));

	player->send( "Your armor class is %d/10, and your alignment is %d.\r\n",
			compute_armor_class(ch), GET_ALIGNMENT(ch));

	player->send( "You have scored %d exp, and have %d gold coins.\r\n",
			GET_EXP(ch), GET_GOLD(ch));

	if(GET_LEVEL(ch) < LVL_IMMORT)
		player->send( "You need %d exp to reach your next level.\r\n",
				level_exp(GET_CLASS(ch), GET_LEVEL(ch) + 1) - GET_EXP(ch));

	playing_time = *real_time_passed((time(0) - ch->player.time.logon) +
			ch->player.time.played, 0);
	player->send( "You have been playing for %d day%s and %d hour%s.\r\n",
			playing_time.day, playing_time.day == 1 ? "" : "s",
			playing_time.hours, playing_time.hours == 1 ? "" : "s");

	player->send( "This ranks you as %s %s (level %d).\r\n",
			GET_NAME(ch).c_str(), GET_TITLE(ch).c_str(), GET_LEVEL(ch));

	switch(GET_POS(ch)) {
		case POS_DEAD:
			player->sendln( "You are DEAD!");
			break;

		case POS_MORTALLYW:
			player->sendln( "You are mortally wounded!  You should seek help!");
			break;

		case POS_INCAP:
			player->sendln( "You are incapacitated, slowly fading away...");
			break;

		case POS_STUNNED:
			player->sendln( "You are stunned!  You can't move!");
			break;

		case POS_SLEEPING:
			player->sendln( "You are sleeping.");
			break;

		case POS_RESTING:
			player->sendln( "You are resting.");
			break;

		case POS_SITTING:
			player->sendln( "You are sitting.");
			break;

		case POS_FIGHTING:
			player->send( "You are fighting %s.", FIGHTING(ch) ? std::string(PERS(FIGHTING(ch), ch)).c_str() : "thin air");
			break;

		case POS_STANDING:
			player->sendln( "You are standing.");
			break;

		default:
			player->sendln( "You are floating.");
			break;
	}

	if(GET_COND(ch, DRUNK) > 10) {
		player->sendln( "You are intoxicated.");
	}

	if(GET_COND(ch, FULL) == 0) {
		player->sendln( "You are hungry.");
	}

	if(GET_COND(ch, THIRST) == 0) {
		player->sendln( "You are thirsty.");
	}

	if(AFF_FLAGGED(ch, AFF_BLIND)) {
		player->sendln( "You have been blinded!");
	}

	if(AFF_FLAGGED(ch, AFF_INVISIBLE)) {
		player->sendln( "You are invisible.");
	}

	if(AFF_FLAGGED(ch, AFF_DETECT_INVIS)) {
		player->sendln( "You are sensitive to the presence of invisible things.");
	}

	if(AFF_FLAGGED(ch, AFF_SANCTUARY)) {
		player->sendln( "You are protected by Sanctuary.");
	}

	if(AFF_FLAGGED(ch, AFF_POISON)) {
		player->sendln( "You are poisoned!");
	}

	if(AFF_FLAGGED(ch, AFF_CHARM)) {
		player->sendln( "You have been charmed!");
	}

	if(affected_by_spell(ch, SPELL_ARMOR)) {
		player->sendln( "You feel protected.");
	}

	if(AFF_FLAGGED(ch, AFF_INFRAVISION)) {
		player->sendln( "Your eyes are glowing red.");
	}

	if(PRF_FLAGGED(ch, PRF_SUMMONABLE)) {
		player->sendln( "You are summonable by other players.");
	}
}


ACMD(do_inventory) {
	MENTOC_PREAMBLE();
	player->sendln( "You are carrying:");
	list_obj_to_char(ch->carrying, ch, SHOW_OBJ_SHORT, TRUE);
}


ACMD(do_equipment) {
	MENTOC_PREAMBLE();
	int i, found = 0;

	player->sendln( "You are using:");

	for(i = 0; i < NUM_WEARS; i++) {
		if(GET_EQ(ch, i)) {
			if(CAN_SEE_OBJ(ch, GET_EQ(ch, i))) {
				player->send( wear_where[i]);
				show_obj_to_char(GET_EQ(ch, i), ch, SHOW_OBJ_SHORT);
				found = TRUE;
			} else {
				player->send( wear_where[i]);
				player->sendln( "Something.");
				found = TRUE;
			}
		}
	}

	if(!found) {
		player->sendln( " Nothing.");
	}
}


ACMD(do_time) {
	MENTOC_PREAMBLE();
	const char *suf;
	int weekday, day;

	/* day in [1..35] */
	day = time_info.day + 1;

	/* 35 days in a month, 7 days a week */
	weekday = ((35 * time_info.month) + day) % 7;

	player->send( "It is %d o'clock %s, on %s.\r\n",
			(time_info.hours % 12 == 0) ? 12 : (time_info.hours % 12),
			time_info.hours >= 12 ? "pm" : "am", weekdays[weekday]);

	/*
	 * Peter Ajamian <peter@PAJAMIAN.DHS.ORG> supplied the following as a fix
	 * for a bug introduced in the ordinal display that caused 11, 12, and 13
	 * to be incorrectly displayed as 11st, 12nd, and 13rd.  Nate Winters
	 * <wintersn@HOTMAIL.COM> had already submitted a fix, but it hard-coded a
	 * limit on ordinal display which I want to avoid.	-dak
	 */

	suf = "th";

	if(((day % 100) / 10) != 1) {
		switch(day % 10) {
			case 1:
				suf = "st";
				break;

			case 2:
				suf = "nd";
				break;

			case 3:
				suf = "rd";
				break;
		}
	}

	player->send( "The %d%s Day of the %s, Year %d.\r\n",
			day, suf, month_name[time_info.month], time_info.year);
}


ACMD(do_weather) {
	MENTOC_PREAMBLE();
	const char *sky_look[] = {
		"cloudless",
		"cloudy",
		"rainy",
		"lit by flashes of lightning"
	};

	if(OUTSIDE(ch)) {
		player->send( "The sky is %s and %s.\r\n", sky_look[weather_info.sky],
				weather_info.change >= 0 ? "you feel a warm wind from south" :
				"your foot tells you bad weather is due");

		if(GET_LEVEL(ch) >= LVL_GOD)
			player->send( "Pressure: %d (change: %d), Sky: %d (%s)\r\n",
					weather_info.pressure,
					weather_info.change,
					weather_info.sky,
					sky_look[weather_info.sky]);
	} else {
		player->sendln( "You have no feeling about the weather at all.");
	}
}


ACMD(do_help) {
	MENTOC_PREAMBLE();
	int chk, bot, top, mid, minlen;

	if(!ch->has_desc) {
		return;
	}
	if(IS_NPC(ch)){ return; }

	skip_spaces(&argument);

	if(!*argument) {
		page_string(*ch->desc, help, 0);
		return;
	}

	std::array<char,50> topic;
	std::fill(topic.begin(),topic.end(),0);
	one_argument(argument,&topic[0]);
	if(!topic[0]){
		player->sendln("Please specify a topic. i.e.: help grenades");
		return;
	}

#define IS_TOPIC(a) (strncmp((char*)&topic[0],a,sizeof(topic)) == 0)
	if(IS_TOPIC("grenade") || IS_TOPIC("nade")) {
		const char* usage = "usage: throw <direction> <room_count>\r\n"
			"example: \r\n"
			" $ get frag backpack\r\n"
			" $ hold frag\r\n"
			" $ throw north 2\r\n"
			" This will throw a frag 2 rooms away\r\n"
			" NOTE:\r\n"
			"All grenades are thrown as far as they can up to a maximum amount of 4 rooms away\r\n"
			"or however many rooms before it reaches a dead-end\r\n"
			"see: help grenade";
		player->sendln(usage);
		return;
	}


	if(!help_table) {
		player->sendln( "No help available.");
		return;
	}

	bot = 0;
	top = top_of_helpt;
	minlen = strlen(argument);

	for(;;) {
		mid = (bot + top) / 2;

		if(bot > top) {
			player->sendln( "There is no help on that word.");
			return;
		} else if(!(chk = strn_cmp(argument, help_table[mid].keyword, minlen))) {
			/* trace backwards to find first matching entry. Thanks Jeff Fink! */
			while((mid > 0) &&
					(!(chk = strn_cmp(argument, help_table[mid - 1].keyword, minlen)))) {
				mid--;
			}

			page_string(*ch->desc, help_table[mid].entry, 0);
			return;
		} else {
			if(chk > 0) {
				bot = mid + 1;
			} else {
				top = mid - 1;
			}
		}
	}
}



#define WHO_FORMAT \
	"format: who [minlev[-maxlev]] [-n name] [-c classlist] [-s] [-o] [-q] [-r] [-z]"

ACMD(do_who) {
	MENTOC_PREAMBLE();
	char_data *tch;
	char name_search[MAX_INPUT_LENGTH], buf[MAX_INPUT_LENGTH];
	char mode;
	int low = 0, high = LVL_IMPL, localwho = 0, questwho = 0;
	int showclass = 0, short_list = 0, outlaws = 0, num_can_see = 0;
	int who_room = 0;

	skip_spaces(&argument);
	strcpy(buf, argument);	/* strcpy: OK (sizeof: argument == buf) */
	name_search[0] = '\0';

	while(*buf) {
		char arg[MAX_INPUT_LENGTH], buf1[MAX_INPUT_LENGTH];

		half_chop(buf, arg, buf1);

		if(isdigit(*arg)) {
			sscanf(arg, "%d-%d", &low, &high);
			strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
		} else if(*arg == '-') {
			mode = *(arg + 1);       /* just in case; we destroy arg in the switch */

			switch(mode) {
				case 'o':
				case 'k':
					outlaws = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'z':
					localwho = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 's':
					short_list = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'q':
					questwho = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'l':
					half_chop(buf1, arg, buf);
					sscanf(arg, "%d-%d", &low, &high);
					break;

				case 'n':
					half_chop(buf1, name_search, buf);
					break;

				case 'r':
					who_room = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'c':
					half_chop(buf1, arg, buf);
					showclass = find_class_bitvector(arg);
					break;

				default:
					player->send( WHO_FORMAT);
					return;
			}				/* end of switch */

		} else {			/* endif */
			player->sendln( WHO_FORMAT);
			return;
		}
	}				/* end while (parser) */

	player->sendln( "Players-------");

	for(auto & player : mods::globals::player_list) {
		auto d = player->desc();
		if(STATE(d) != CON_PLAYING) {
			continue;
		}

		if(d.original) {
			tch = d.original;
		} else if(!(tch = d.character)) {
			continue;
		}

		if(*name_search && str_cmp(GET_NAME(tch), name_search) &&
				!strstr(GET_TITLE(tch).c_str(), name_search)) {
			continue;
		}

		if(!CAN_SEE(ch, tch) || GET_LEVEL(tch) < low || GET_LEVEL(tch) > high) {
			continue;
		}

		if(outlaws && !PLR_FLAGGED(tch, PLR_KILLER) &&
				!PLR_FLAGGED(tch, PLR_THIEF)) {
			continue;
		}

		if(questwho && !PRF_FLAGGED(tch, PRF_QUEST)) {
			continue;
		}

		if(localwho && world[IN_ROOM(ch)].zone != world[IN_ROOM(tch)].zone) {
			continue;
		}

		if(who_room && (IN_ROOM(tch) != IN_ROOM(ch))) {
			continue;
		}

		if(showclass && !(showclass & (1 << GET_CLASS(tch)))) {
			continue;
		}

		if(short_list) {
			player->send( "%s[%2d %s] %-12.12s%s%s",
					(GET_LEVEL(tch) >= LVL_IMMORT ? CCYEL(ch, C_SPR) : ""),
					GET_LEVEL(tch), CLASS_ABBR(tch), GET_NAME(tch).c_str(),
					(GET_LEVEL(tch) >= LVL_IMMORT ? CCNRM(ch, C_SPR) : ""),
					((!(++num_can_see % 4)) ? "\r\n" : ""));
		} else {
			num_can_see++;
			player->send( "%s[%2d %s] %s %s",
					(GET_LEVEL(tch) >= LVL_IMMORT ? CCYEL(ch, C_SPR) : ""),
					GET_LEVEL(tch), CLASS_ABBR(tch), GET_NAME(tch).c_str(),
					GET_TITLE(tch).c_str());

			if(GET_INVIS_LEV(tch)) {
				player->send( " (i%d)", GET_INVIS_LEV(tch));
			} else if(AFF_FLAGGED(tch, AFF_INVISIBLE)) {
				player->send( " (invis)");
			}

			if(PLR_FLAGGED(tch, PLR_MAILING)) {
				player->send( " (mailing)");
			} else if(PLR_FLAGGED(tch, PLR_WRITING)) {
				player->send( " (writing)");
			}

			if(PRF_FLAGGED(tch, PRF_DEAF)) {
				player->send( " (deaf)");
			}

			if(PRF_FLAGGED(tch, PRF_NOTELL)) {
				player->send( " (notell)");
			}

			if(PRF_FLAGGED(tch, PRF_QUEST)) {
				player->send( " (quest)");
			}

			if(PLR_FLAGGED(tch, PLR_THIEF)) {
				player->send( " (THIEF)");
			}

			if(PLR_FLAGGED(tch, PLR_KILLER)) {
				player->send( " (KILLER)");
			}

			if(GET_LEVEL(tch) >= LVL_IMMORT) {
				player->send( CCNRM(ch, C_SPR));
			}

			player->sendln( "");
		}				/* endif shortlist */
	}				/* end of for */

	if(short_list && (num_can_see % 4)) {
		player->sendln( "");
	}

	if(num_can_see == 0) {
		player->sendln( "\r\nNobody at all!");
	} else if(num_can_see == 1) {
		player->sendln( "\r\nOne lonely character displayed.");
	} else {
		player->send( "\r\n%d characters displayed.\r\n", num_can_see);
	}
}


#define USERS_FORMAT \
	"format: users [-l minlevel[-maxlevel]] [-n name] [-h host] [-c classlist] [-o] [-p]"

/* BIG OL' FIXME: Rewrite it all. Similar to do_who(). */
ACMD(do_users) {
	MENTOC_PREAMBLE();
	char line[200], line2[220], idletime[10], classname[20];
	char state[30], *timeptr, mode;
	char name_search[MAX_INPUT_LENGTH], host_search[MAX_INPUT_LENGTH];
	char_data *tch;
	int low = 0, high = LVL_IMPL, num_can_see = 0;
	int showclass = 0, outlaws = 0, playing = 0, deadweight = 0;
	char buf[MAX_INPUT_LENGTH], arg[MAX_INPUT_LENGTH];

	host_search[0] = name_search[0] = '\0';

	strcpy(buf, argument);	/* strcpy: OK (sizeof: argument == buf) */

	while(*buf) {
		char buf1[MAX_INPUT_LENGTH];

		half_chop(buf, arg, buf1);

		if(*arg == '-') {
			mode = *(arg + 1);  /* just in case; we destroy arg in the switch */

			switch(mode) {
				case 'o':
				case 'k':
					outlaws = 1;
					playing = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'p':
					playing = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'd':
					deadweight = 1;
					strcpy(buf, buf1);	/* strcpy: OK (sizeof: buf1 == buf) */
					break;

				case 'l':
					playing = 1;
					half_chop(buf1, arg, buf);
					sscanf(arg, "%d-%d", &low, &high);
					break;

				case 'n':
					playing = 1;
					half_chop(buf1, name_search, buf);
					break;

				case 'h':
					playing = 1;
					half_chop(buf1, host_search, buf);
					break;

				case 'c':
					playing = 1;
					half_chop(buf1, arg, buf);
					showclass = find_class_bitvector(arg);
					break;

				default:
					player->send( USERS_FORMAT);
					return;
			}				/* end of switch */

		} else {			/* endif */
			player->send( USERS_FORMAT);
			return;
		}
	}				/* end while (parser) */

	player->sendln(
			"Num Class   Name         State          Idl Login@   Site"
			"--- ------- ------------ -------------- --- -------- ------------------------");

	one_argument(argument, arg);

	for(auto & d : descriptor_list) {
		if(STATE(d) != CON_PLAYING && playing) {
			continue;
		}

		if(STATE(d) == CON_PLAYING && deadweight) {
			continue;
		}

		if(STATE(d) == CON_PLAYING) {
			if(d.original) {
				tch = d.original;
			} else if(!(tch = d.character)) {
				continue;
			}

			if(*host_search && !strstr(d.host.c_str(), host_search)) {
				continue;
			}

			if(*name_search && str_cmp(GET_NAME(tch), name_search)) {
				continue;
			}

			if(!CAN_SEE(ch, tch) || GET_LEVEL(tch) < low || GET_LEVEL(tch) > high) {
				continue;
			}

			if(outlaws && !PLR_FLAGGED(tch, PLR_KILLER) &&
					!PLR_FLAGGED(tch, PLR_THIEF)) {
				continue;
			}

			if(showclass && !(showclass & (1 << GET_CLASS(tch)))) {
				continue;
			}

			if(GET_INVIS_LEV(ch) > GET_LEVEL(ch)) {
				continue;
			}

			if(d.original)
				sprintf(classname, "[%2d %s]", GET_LEVEL(d.original),
						CLASS_ABBR(d.original));
			else
				sprintf(classname, "[%2d %s]", GET_LEVEL(d.character),
						CLASS_ABBR(d.character));
		} else {
			strcpy(classname, "   -   ");
		}

		timeptr = asctime(localtime(&d.login_time));
		timeptr += 11;
		*(timeptr + 8) = '\0';

		if(STATE(d)== CON_PLAYING && d.original) {
			strcpy(state, "Switched");
		} else {
			strcpy(state, connected_types[STATE(d)]);
		}

		if(d.character && STATE(d)== CON_PLAYING && GET_LEVEL(d.character) < LVL_GOD)
			sprintf(idletime, "%3d", d.character->char_specials.timer *
					SECS_PER_MUD_HOUR / SECS_PER_REAL_MIN);
		else {
			strcpy(idletime, "");
		}

		sprintf(line, "%3d %-7s %-12s %-14s %-3s %-8s ", d.desc_num, classname,
				d.original && d.original->player.name.c_str() ? d.original->player.name.c_str() :
				d.character && d.character->player.name.c_str() ? d.character->player.name.c_str() :
				"UNDEFINED",
				state, idletime, timeptr);

		if(d.host.length()) {
			sprintf(line + strlen(line), "[%s]\r\n", d.host.c_str());
		} else {
			strcat(line, "[Hostname unknown]\r\n");
		}

		if(STATE(d) != CON_PLAYING) {
			sprintf(line2, "%s%s%s", CCGRN(ch, C_SPR), line, CCNRM(ch, C_SPR));
			strcpy(line, line2);
		}

		if(STATE(d) != CON_PLAYING ||
				(STATE(d) == CON_PLAYING && CAN_SEE(ch, d.character))) {
			player->sendln( line);
			num_can_see++;
		}
	}

	player->send( "\r\n%d visible sockets connected.", num_can_see);
}


/* Generic page_string function for displaying text */
ACMD(do_gen_ps) {
	MENTOC_PREAMBLE();
	switch(subcmd) {
		case SCMD_CREDITS:
			page_string(*ch->desc, credits, 0);
			break;

		case SCMD_NEWS:
			page_string(*ch->desc, news, 0);
			break;

		case SCMD_INFO:
			page_string(*ch->desc, info, 0);
			break;

		case SCMD_WIZLIST:
			page_string(*ch->desc, wizlist, 0);
			break;

		case SCMD_IMMLIST:
			page_string(*ch->desc, immlist, 0);
			break;

		case SCMD_HANDBOOK:
			page_string(*ch->desc, handbook, 0);
			break;

		case SCMD_POLICIES:
			page_string(*ch->desc, policies, 0);
			break;

		case SCMD_MOTD:
			page_string(*ch->desc, motd.data(), 0);
			break;

		case SCMD_IMOTD:
			page_string(*ch->desc, imotd, 0);
			break;

		case SCMD_CLEAR:
			player->sendln( "\033[H\033[J");
			break;

		case SCMD_VERSION:
			player->send("%s\r\n", circlemud_version);
			break;

		case SCMD_WHOAMI:
			player->sendln( GET_NAME(ch).c_str());
			break;

		default:
			log("SYSERR: Unhandled case in do_gen_ps. (%d)", subcmd);
			return;
	}
}


void perform_mortal_where(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	char_data *i;

	if(!*arg) {
		player->sendln( "Players in your Zone\r\n--------------------");

		for(auto &d : descriptor_list) {
			if(STATE(d) != CON_PLAYING || d.character == ch) {
				continue;
			}

			if((i = (d.original ? d.original : d.character)) == NULL) {
				continue;
			}

			if(IN_ROOM(i) == NOWHERE || !CAN_SEE(ch, i)) {
				continue;
			}

			if(world[IN_ROOM(ch)].zone != world[IN_ROOM(i)].zone) {
				continue;
			}

			player->send( "%-20s - %s\r\n", GET_NAME(i).c_str(), world[IN_ROOM(i)].name.c_str());
		}
	} else {			/* print only FIRST char, not all. */
		for(i = character_list; i; i = i->next) {
			if(IN_ROOM(i) == NOWHERE || i == ch) {
				continue;
			}

			if(!CAN_SEE(ch, i) || world[IN_ROOM(i)].zone != world[IN_ROOM(ch)].zone) {
				continue;
			}

			if(!isname(arg, i->player.name)) {
				continue;
			}

			player->send( "%-25s - %s\r\n", GET_NAME(i).c_str(), world[IN_ROOM(i)].name.c_str());
			return;
		}

		player->sendln( "Nobody around by that name.");
	}
}


void print_object_location(int num, struct obj_data *obj, char_data *ch,
		int recur) {
	MENTOC_PREAMBLE();
	if(num > 0) {
		player->send( "O%3d. %-25s - ", num, obj->short_description);
	} else {
		player->send( "%33s", " - ");
	}

	if(IN_ROOM(obj) != NOWHERE) {
		player->send( "[%5d] %s\r\n", GET_ROOM_VNUM(IN_ROOM(obj)), world[IN_ROOM(obj)].name.c_str());
	} else if(obj->carried_by) {
		player->send( "carried by %s\r\n", std::string(PERS(obj->carried_by, ch)).c_str());
	} else if(obj->worn_by) {
		player->send( "worn by %s\r\n", std::string(PERS(obj->worn_by, ch)).c_str());
	} else if(obj->in_obj) {
		player->send( "inside %s%s\r\n", obj->in_obj->short_description, (recur ? ", which is" : " "));

		if(recur) {
			print_object_location(0, obj->in_obj, ch, recur);
		}
	} else {
		player->sendln( "in an unknown location\r\n");
	}
}



void perform_immort_where(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	char_data *i;
	struct obj_data *k;
	int num = 0, found = 0;

	if(!*arg) {
		player->send( "Players\r\n-------\r\n");

		for(auto & d : descriptor_list)
			if(STATE(d) == CON_PLAYING) {
				i = (d.original ? d.original : d.character);

				if(i && CAN_SEE(ch, i) && (IN_ROOM(i) != NOWHERE)) {
					if(d.original)
						player->send( "%-20s - [%5d] %s (in %s)\r\n",
								GET_NAME(i).c_str(), GET_ROOM_VNUM(IN_ROOM(d.character)),
								world[IN_ROOM(d.character)].name.c_str(), GET_NAME(d.character).c_str());
					else {
						player->send( "%-20s - [%5d] %s\r\n", GET_NAME(i).c_str(), GET_ROOM_VNUM(IN_ROOM(i)), world[IN_ROOM(i)].name.c_str());
					}
				}
			}
	} else {
		for(i = character_list; i; i = i->next) {
			if(CAN_SEE(ch, i) && IN_ROOM(i) != NOWHERE && isname(arg, i->player.name)) {
				found = 1;
				player->send( "M%3d. %-25s - [%5d] %s\r\n", ++num, GET_NAME(i).c_str(),
						GET_ROOM_VNUM(IN_ROOM(i)), world[IN_ROOM(i)].name.c_str());
			}
		}

		num = 0;

		for(auto& obj_reference : obj_list) {
			/* TODO !test !integrity */
			k = obj_reference.get();

			if(CAN_SEE_OBJ(ch, k) && isname(arg, k->name)) {
				found = 1;
				print_object_location(++num, k, ch, TRUE);
			}
		}

		if(!found) {
			player->sendln( "Couldn't find any such thing.");
		}
	}//End else
}



ACMD(do_where) {
	MENTOC_PREAMBLE();
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(GET_LEVEL(ch) >= LVL_IMMORT) {
		perform_immort_where(ch, arg);
	} else {
		perform_mortal_where(ch, arg);
	}
}



ACMD(do_levels) {
	MENTOC_PREAMBLE();
	char buf[MAX_STRING_LENGTH];
	unsigned i = 0, len = 0;
	int nlen;

	if(IS_NPC(ch)) {
		return;
	}

	for(i = 1; i < LVL_IMMORT; i++) {
		nlen = snprintf(buf + len, sizeof(buf) - len, "[%2d] %8d-%-8d : ", i,
				level_exp(GET_CLASS(ch), i), level_exp(GET_CLASS(ch), i + 1) - 1);

		if(len + nlen >= sizeof(buf) || nlen < 0) {
			break;
		}

		len += nlen;

		switch(GET_SEX(ch)) {
			case SEX_MALE:
			case SEX_NEUTRAL:
				nlen = snprintf(buf + len, sizeof(buf) - len, "%s\r\n", title_male(GET_CLASS(ch), i));
				break;

			case SEX_FEMALE:
				nlen = snprintf(buf + len, sizeof(buf) - len, "%s\r\n", title_female(GET_CLASS(ch), i));
				break;

			default:
				nlen = snprintf(buf + len, sizeof(buf) - len, "Oh dear.  You seem to be sexless.\r\n");
				break;
		}

		if(len + nlen >= sizeof(buf) || nlen < 0) {
			break;
		}

		len += nlen;
	}

	if(len < sizeof(buf))
		snprintf(buf + len, sizeof(buf) - len, "[%2d] %8d          : Immortality\r\n",
				LVL_IMMORT, level_exp(GET_CLASS(ch), LVL_IMMORT));

	page_string(*ch->desc, buf, TRUE);
}



ACMD(do_consider) {
	MENTOC_PREAMBLE();
	char buf[MAX_INPUT_LENGTH];
	char_data *victim;
	int diff;

	one_argument(argument, buf);

	if(!(victim = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
		player->sendln( "Consider killing who?");
		return;
	}

	if(victim == ch) {
		player->sendln( "Easy!  Very easy indeed!");
		return;
	}

	if(!IS_NPC(victim)) {
		player->sendln( "Would you like to borrow a cross and a shovel?");
		return;
	}

	diff = (GET_LEVEL(victim) - GET_LEVEL(ch));

	if(diff <= -10) {
		player->sendln( "Now where did that chicken go?");
	} else if(diff <= -5) {
		player->sendln( "You could do it with a needle!");
	} else if(diff <= -2) {
		player->sendln( "Easy.");
	} else if(diff <= -1) {
		player->sendln( "Fairly easy.");
	} else if(diff == 0) {
		player->sendln( "The perfect match!");
	} else if(diff <= 1) {
		player->sendln( "You would need some luck!");
	} else if(diff <= 2) {
		player->sendln( "You would need a lot of luck!");
	} else if(diff <= 3) {
		player->sendln( "You would need a lot of luck and great equipment!");
	} else if(diff <= 5) {
		player->sendln( "Do you feel lucky, punk?");
	} else if(diff <= 10) {
		player->sendln( "Are you mad!?");
	} else if(diff <= 100) {
		player->sendln( "You ARE mad!");
	}
}



ACMD(do_diagnose) {
	MENTOC_PREAMBLE();
	char buf[MAX_INPUT_LENGTH];
	char_data *vict;

	one_argument(argument, buf);

	if(*buf) {
		if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
			player->sendln( NOPERSON);
		} else {
			diag_char_to_char(vict, ch);
		}
	} else {
		if(FIGHTING(ch)) {
			diag_char_to_char(FIGHTING(ch), ch);
		} else {
			player->sendln( "Diagnose who?");
		}
	}
}


const char *ctypes[] = {
	"off", "sparse", "normal", "complete", "\n"
};

ACMD(do_color) {
	MENTOC_PREAMBLE();
	char arg[MAX_INPUT_LENGTH];
	int tp;

	if(IS_NPC(ch)) {
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		player->send( "Your current color level is %s.\r\n", ctypes[COLOR_LEV(ch)]);
		return;
	}

	if((tp = search_block(arg, ctypes, FALSE)) == -1) {
		player->sendln( "Usage: color { Off | Sparse | Normal | Complete }");
		return;
	}

	REMOVE_BIT(PRF_FLAGS(ch), PRF_COLOR_1 | PRF_COLOR_2);
	SET_BIT(PRF_FLAGS(ch), (PRF_COLOR_1 * (tp & 1)) | (PRF_COLOR_2 * (tp & 2) >> 1));

	player->send( "Your %scolor%s is now %s.\r\n", CCRED(ch, C_SPR), CCNRM(ch, C_OFF), ctypes[tp]);
}


ACMD(do_toggle) {
	MENTOC_PREAMBLE();
	char buf2[4];

	if(IS_NPC(ch)) {
		return;
	}

	if(GET_WIMP_LEV(ch) == 0) {
		strcpy(buf2, "OFF");    /* strcpy: OK */
	} else {
		sprintf(buf2, "%-3.3d", GET_WIMP_LEV(ch));    /* sprintf: OK */
	}

	if(GET_LEVEL(ch) >= LVL_IMMORT) {
		player->send(
				"      No Hassle: %-3s    "
				"      Holylight: %-3s    "
				"     Room Flags: %-3s\r\n",
				ONOFF(PRF_FLAGGED(ch, PRF_NOHASSLE)),
				ONOFF(PRF_FLAGGED(ch, PRF_HOLYLIGHT)),
				ONOFF(PRF_FLAGGED(ch, PRF_ROOMFLAGS))
				);
	}

	player->send(
			"Hit Pnt Display: %-3s    "
			"     Brief Mode: %-3s    "
			" Summon Protect: %-3s\r\n"

			"   Move Display: %-3s    "
			"   Compact Mode: %-3s    "
			"       On Quest: %-3s\r\n"

			"   Mana Display: %-3s    "
			"         NoTell: %-3s    "
			"   Repeat Comm.: %-3s\r\n"

			" Auto Show Exit: %-3s    "
			"           Deaf: %-3s    "
			"     Wimp Level: %-3s\r\n"

			" Gossip Channel: %-3s    "
			"Auction Channel: %-3s    "
			"  Grats Channel: %-3s\r\n"

			"    Color Level: %s\r\n",

		ONOFF(PRF_FLAGGED(ch, PRF_DISPHP)),
		ONOFF(PRF_FLAGGED(ch, PRF_BRIEF)),
		ONOFF(!PRF_FLAGGED(ch, PRF_SUMMONABLE)),

		ONOFF(PRF_FLAGGED(ch, PRF_DISPMOVE)),
		ONOFF(PRF_FLAGGED(ch, PRF_COMPACT)),
		YESNO(PRF_FLAGGED(ch, PRF_QUEST)),

		ONOFF(PRF_FLAGGED(ch, PRF_DISPMANA)),
		ONOFF(PRF_FLAGGED(ch, PRF_NOTELL)),
		YESNO(!PRF_FLAGGED(ch, PRF_NOREPEAT)),

		ONOFF(PRF_FLAGGED(ch, PRF_AUTOEXIT)),
		YESNO(PRF_FLAGGED(ch, PRF_DEAF)),
		buf2,

		ONOFF(!PRF_FLAGGED(ch, PRF_NOGOSS)),
		ONOFF(!PRF_FLAGGED(ch, PRF_NOAUCT)),
		ONOFF(!PRF_FLAGGED(ch, PRF_NOGRATZ)),

		ctypes[COLOR_LEV(ch)]);
}


int sort_commands_helper(const void *a, const void *b) {
	return strcmp(cmd_info[*(const int *)a].command, cmd_info[*(const int *)b].command);
}


void sort_commands(void) {
	int a, num_of_cmds = 0;

	while(cmd_info[num_of_cmds].command[0] != '\n') {
		num_of_cmds++;
	}

	num_of_cmds++;	/* \n */

	CREATE(cmd_sort_info, int, num_of_cmds);

	for(a = 0; a < num_of_cmds; a++) {
		cmd_sort_info[a] = a;
	}

	/* Don't sort the RESERVED or \n entries. */
	qsort(cmd_sort_info + 1, num_of_cmds - 2, sizeof(int), sort_commands_helper);
}


ACMD(do_commands) {
	MENTOC_PREAMBLE();
	int no, i, cmd_num;
	int wizhelp = 0, socials = 0;
	char_data *vict;
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(*arg) {
		if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD)) || IS_NPC(vict)) {
			player->sendln( "Who is that?");
			return;
		}

		if(GET_LEVEL(ch) < GET_LEVEL(vict)) {
			player->sendln( "You can't see the commands of people above your level.");
			return;
		}
	} else {
		vict = ch;
	}

	if(subcmd == SCMD_SOCIALS) {
		socials = 1;
	} else if(subcmd == SCMD_WIZHELP) {
		wizhelp = 1;
	}

	player->send( "The following %s%s are available to %s:\r\n",
			wizhelp ? "privileged " : "",
			socials ? "socials" : "commands",
			vict == ch ? "you" : GET_NAME(vict).c_str());

	/* cmd_num starts at 1, not 0, to remove 'RESERVED' */
	for(no = 1, cmd_num = 1; cmd_info[cmd_sort_info[cmd_num]].command[0] != '\n'; cmd_num++) {
		i = cmd_sort_info[cmd_num];

		if(cmd_info[i].minimum_level < 0 || GET_LEVEL(vict) < cmd_info[i].minimum_level) {
			continue;
		}

		if((cmd_info[i].minimum_level >= LVL_IMMORT) != wizhelp) {
			continue;
		}

		if(!wizhelp && socials != (cmd_info[i].command_pointer == do_action || cmd_info[i].command_pointer == do_insult)) {
			continue;
		}

		send_to_char(ch, "%-11s%s", cmd_info[i].command, no++ % 7 == 0 ? "\r\n" : "");
	}

	if(no % 7 != 1) {
		player->sendln( "\r\n");
	}
}
