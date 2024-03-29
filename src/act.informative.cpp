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
#include "mods/rifle.hpp"
#include "mods/contracts.hpp"
#include "mods/drone.hpp"
#include "mods/util.hpp"
#include "mods/world-configuration.hpp"
#include "mods/object-utils.hpp"
#include "mods/player-utils.hpp"
#include "mods/rooms.hpp"
#include "mods/prefs.hpp"
#include "mods/super-users.hpp"
#include "mods/interpreter.hpp"
#include "mods/examine.hpp"
#include "mods/help.hpp"
#include "mods/levels.hpp"
#include "mods/calc-visibility.hpp"
#include "mods/overhead_map.hpp"
#include "mods/classes/ghost-overhead-map.hpp"
#include "mods/js.hpp"
#include "mods/contract-events.hpp"
#include "mods/players/messages.hpp"
#include "mods/weapons/unique-weapons.hpp"
#include "mods/classes/ghost.hpp"

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
char *title_male(int chclass, int level);
char *title_female(int chclass, int level);
struct time_info_data *real_time_passed(time_t t2, time_t t1);
int compute_armor_class(char_data *ch);

/** use this for camera feeds... not installable cameras */
void look_at_room_specific(player_ptr_t& player, int ignore_brief,int room);

/* local functions */
int sort_commands_helper(const void *a, const void *b);
void print_object_location(int num, struct obj_data *obj, char_data *ch, int recur);
void show_obj_to_char(struct obj_data *obj, char_data *ch, int mode);
void list_obj_to_char(struct obj_data *list, char_data *ch, int mode, int show);
void show_obj_modifiers(struct obj_data *obj, char_data *ch);
ACMD(do_js);
ACMD(do_recall);
ACMD(do_givemegold);
ACMD(do_drone);
ACMD(do_look);
ACMD(do_examine);
ACMD(do_inventory);
ACMD(do_equipment);
ACMD(do_time);
ACMD(do_weather);
ACMD(do_help);
ACMD(do_who);
ACMD(do_users);
ACMD(do_gen_ps);
ACMD(do_preferences);
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
	DO_HELP("drone");
	auto vec_args = PARSE_ARGS();
	std::string usage = "usage: drone <start|stop>";

	if(vec_args.size() == 0) {
		player->sendln(usage);
		return;
	}
	if(vec_args[0].compare("start") == 0) {
		/* if user has a drone out already, ressurrect that drone */
		if(!mods::drone::started(player->uuid())) {
			mods::drone::start(player->uuid());
		}

		return;
	}

	if(vec_args[0].compare("stop") == 0) {
		/* if user has a drone out already, ressurrect that drone */
		mods::drone::stop(player->uuid());
		return;
	}
	player->sendln(usage);
}

SUPERCMD(do_givemegold) {
	ADMIN_REJECT();
	ch->points.gold += 50000;
}

ACMD(do_recall) {
	if(mods::globals::boot_type == mods::globals::boot_type_t::BOOT_HELL) {
		*player << "Ah Ah Ahh... You didn't say the magic word...";
		return;
	}
	if(mods::super_users::player_is(player)) {
		char_from_room(ch);
		char_to_room(ch,mods::world_conf::real_mortal_start());
		look_at_room(ch,0);
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


ACMD(do_preferences) {
	if(player->is_npc()) {
		return;
	}
	player->sendln("Not implemented");
}

SUPERCMD(do_js) {
	ADMIN_REJECT();
	player->executing_js(true);
	mods::js::eval_string(std::string(argument) + ";");
	player->executing_js(false);
}

SUPERCMD(do_newjs) {
	ADMIN_REJECT();
	mods::js::create_new_context();
	mods::js::load_base_functions();
	mods::js::load_c_require_functions();
}
SUPERCMD(do_jstest) {
	ADMIN_REJECT();

	if(!mods::js::run_test_suite(*player,argument)) {
		*player << "{red}Unable to load test suite{/red}\r\n";
		return;
	}

	*player << "Test suite loaded...\r\n";
	return;
}
bool is_camera_feed(obj_data* obj) {
	return obj->has_gadget() && obj->gadget()->attributes->vnum_list.size();
}
void look_into_camera_feed(player_ptr_t& player, obj_data* obj, int mode) {
	if(is_camera_feed(obj)) {
		auto& vlist = obj->gadget()->attributes->vnum_list;
		int index = vlist[0];
		if(index < vlist.size()) {
			auto viewing_room_vnum = vlist[vlist[0]];
			auto room = real_room(viewing_room_vnum);
			if(room == NOWHERE) {
				player->sendln("You see nothing but static...");
				return;
			}
			player->sendln("{blu}You peer into the camera feed...{/blu}");
			player->sendln(CAT("{grn}Room title:{/grn}",world[room].name.c_str()));
			player->sendln(CAT("{grn}Room description:{/grn}",world[room].description.c_str()));
			player->sendx("{grn}Room occupants:{/grn}");
			list_char_to_char(player->cd());
			player->sendln("\r\n{blu}* * * End of Camera Feed * * *{/blu}");
			player->sendx(CCNRM(ch, C_NRM));
			return;
		}
		player->sendln("You see nothing but static...");
		return;
	}
}
void show_obj_to_char(struct obj_data *obj, char_data *ch, int mode) {
	auto o = optr(obj);
	auto player = ptr(ch);
	show_obj_to_char(o,player,mode,1);
}
bool look_at_weapon(player_ptr_t& player,obj_ptr_t& obj) {
	bool has_unique = mods::weapons::has_unique_description(obj);
	if(has_unique) {
		player->sendln(mods::weapons::get_unique_weapon_description(obj));
	}
	if(mods::rifle::is_rifle_attachment(obj.get())) {
		player->sendln(mods::rifle_attachments::by_uuid(obj->uuid)->examine());
		return true;
	}
	if(has_unique) {
		return true;
	}
	if(obj->action_description.length()) {
		player->sendln(obj->action_description);
		return true;
	}
	player->sendln("You see nothing special...");
	return false;
}

void show_obj_to_char(obj_ptr_t& object, player_ptr_t& player, int mode,int count) {
	auto obj = object.get();
	auto ch = player->cd();
	if(!obj || !ch) {
		log("SYSERR: NULL pointer in show_obj_to_char(): obj=%p ch=%p", obj, ch);
		return;
	}

	if(mods::object_utils::is_camera(obj) && obj->location_data() >= 16) {
		player->sendln(
		    CAT(
		        "A ", obj->name.c_str(), " is installed on the ",
		        mods::globals::dir_to_str(obj->location_data() - 16, true).c_str(), " wall."
		    )
		);
		if(ch->contract) {
			mods::contract_events::find_item(player,object);
		}
		return;
	}

	if(mods::object_utils::is_claymore(obj) && obj->location_data() >= 16) {
		player->sendln(
		    CAT(
		        "A ", obj->name.c_str(),
		        " is installed at the foot of the ", mods::globals::dir_to_str(obj->location_data() - 16, true).c_str(),
		        " entrance."
		    )
		);
		if(ch->contract) {
			mods::contract_events::find_item(player,object);
		}
		return;
	}

	switch(mode) {
		case SHOW_OBJ_LONG:
		case SHOW_OBJ_SHORT:
		case ITEM_WEAPON:
			if(count > 1) {
				player->sendx(CAT("(",std::to_string(count),") "));
			}
			break;
		default:
			break;
	}

	switch(mode) {
		case SHOW_OBJ_LONG:
			if(obj->description.length()) {
				player->sendln(obj->description);
			} else {
				player->sendln(obj->name);
			}
			break;

		case SHOW_OBJ_SHORT:
			if(obj->short_description.length()) {
				player->sendln(obj->short_description);
			} else {
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
					player->sendx("It looks like a drink container.");
					break;

				case ITEM_WEAPON:
				default:
					break;
			}

			break;

		default:
			log("SYSERR: Bad display mode (%d.in show_obj_to_char().", mode);
			return;
	}

	show_obj_modifiers(obj, ch);
	if(ch->contract) {
		mods::contract_events::find_item(player,object);
	}
}


void show_obj_modifiers(struct obj_data *obj, char_data *ch) {
	MENTOC_PREAMBLE();
	if(OBJ_FLAGGED(obj, ITEM_INVISIBLE)) {
		player->sendx(" (invisible)");
	}

	if(OBJ_FLAGGED(obj, ITEM_BLESS) && AFF_FLAGGED(ch, AFF_DETECT_ALIGN)) {
		player->sendx(" ..It glows blue!");
	}

	if(OBJ_FLAGGED(obj, ITEM_MAGIC) && AFF_FLAGGED(ch, AFF_DETECT_MAGIC)) {
		player->sendx(" ..It glows yellow!");
	}

	if(OBJ_FLAGGED(obj, ITEM_GLOW)) {
		player->sendx(" ..It has a soft glowing aura!");
	}

	if(OBJ_FLAGGED(obj, ITEM_HUM)) {
		player->sendx(" ..It emits a faint humming sound!");
	}
}


void list_obj_to_char(struct obj_data *list, char_data *ch, int mode, int show) {
	MENTOC_PREAMBLE();
	bool found = FALSE;

	std::unordered_map<std::string,int> lines;
	std::set<std::string> sent;
	for(auto i = list; i; i = i->next_content) {
		if(!i) {
			break;
		}
		if(CAN_SEE_OBJ(ch, i)) {
			lines[i->name.c_str()]++;
		}
	}

	for(auto i = list; i; i = i->next_content) {
		if(!i) {
			break;
		}
		if(CAN_SEE_OBJ(ch, i)) {
			if(sent.find(i->name.c_str()) != sent.end()) {
				continue;
			}
			auto obj = optr(i);
			show_obj_to_char(obj, player, mode, lines[i->name.c_str()]);
			sent.insert(i->name.c_str());
			found = TRUE;
		}
	}

	if(!found && show) {
		player->sendln(" Nothing.");
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
	if(i == ch) {
		pers = i->player.name.c_str();
	} else {
		pers = PERS(i, ch);
		if(!pers) {
			pers = "Someone";
		}
	}

	if(GET_MAX_HIT(i) > 0) {
		percent = (100 * GET_HIT(i)) / GET_MAX_HIT(i);
	} else {
		percent = -1;    /* How could MAX_HIT be < 1?? */
	}

	for(ar_index = 0; diagnosis[ar_index].percent >= 0; ar_index++) {
		if(percent >= diagnosis[ar_index].percent) {
			break;
		}
	}

	player->sendln(
	    CAT(
	        " ",pers.c_str(), " ",diagnosis[ar_index].text
	    )
	);
	if(ch->contract) {
		auto iptr = ptr(i);
		mods::contract_events::find_mob(player,iptr);
	}
}


void look_at_char(char_data *i, char_data *ch) {
	MENTOC_PREAMBLE();
	int j, found = false;
	struct obj_data *tmp_obj;
	auto iptr = ptr(i);

	if(!ch->has_desc) {
		return;
	}

	if(i->player.description) {
		player->sendln(i->player.description);
	} else {
		act("You see nothing special about $m.\r\n", FALSE, i, 0, ch, TO_VICT);
	}

	diag_char_to_char(i, ch);

	found = FALSE;

	if(player->ghost()) {
		player->sendln(CAT("uuid: ",i->uuid));
	}
	act("\r\n$n is using:\r\n", FALSE, i, 0, ch, TO_VICT);
	for(j = 0; j < NUM_WEARS; j++) {
		auto obj = iptr->equipment(j);
		if(obj) {
			player->sendx(wear_where[j]);
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
		act("\r\nYou attempt to peek at $s inventory:\r\n", FALSE, i, 0, ch, TO_VICT);

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

	if(i == nullptr) {
		return;
	}
	if(ch == nullptr) {
		return;
	}

	const char *positions[] = {
		" is lying here, {red}DEAD{/red}.",
		" is lying here, {red}mortally wounded{/red}.",
		" is lying here, {yel}incapacitated{/yel}.",
		" is lying here, {yel}stunned{/yel}.",
		" is sleeping here.",
		" is resting here.",
		" is sitting here.",
		"!FIGHTING!",
		" is standing here.",
		" is prone here.",
	};
	auto iptr = ptr(i);

	if(IS_NPC(i)) {
		player->stc(i->player.short_descr);
	} else {
		player->stc(i->player.name.str());
	}

	if(AFF_FLAGGED(i, AFF_INVISIBLE)) {
		player->sendx(" (invisible)");
	}

	if(AFF_FLAGGED(i, AFF_HIDE)) {
		player->sendx(" (hidden)");
	}

	if(!IS_NPC(i) && !i->has_desc) {
		player->sendx(" (linkless)");
	}

	if(!IS_NPC(i) && PLR_FLAGGED(i, PLR_WRITING)) {
		player->sendx(" (writing)");
	}


	if(i->drone) {
		player->sendx(" is idling here.");
	} else {
		if(GET_POS(i) != POS_FIGHTING) {
			player->sendx(positions[(int) GET_POS(i)]);
		} else {
			if(FIGHTING(i)) {
				player->sendx(" is here, fighting ");

				if(FIGHTING(i) == ch) {
					player->sendx("YOU!");
				} else {
					if(IN_ROOM(i) == IN_ROOM(FIGHTING(i))) {
						player->sendx(CAT(iptr->fighting()->name(),"!"));
					} else {
						player->sendx("someone who has already left!");
					}
				}
			} else {		/* NIL fighting pointer */
				player->sendx(" is here struggling with thin air.");
			}
		}
	}

	if(AFF_FLAGGED(ch, AFF_DETECT_ALIGN)) {
		if(IS_EVIL(i)) {
			player->sendx(" (Red Aura)");
		} else if(IS_GOOD(i)) {
			player->sendx(" (Blue Aura)");
		}
	}



	if(AFF_FLAGGED(i, AFF_SANCTUARY)) {
		act(" ...$e glows with a bright light!", FALSE, i, 0, ch, TO_VICT);
	}
	if(ch == FIGHTING(i)) {
		player->sendx(" {red}[!]{/red}");
	}
	player->sendln();
	if(ch->contract) {
		mods::contract_events::find_mob(player,iptr);
	}
}



void list_char_to_char(char_data *ch) {
	//#define __MENTOC_SHOW_VISION_FLAGS_IN_INFORMATIVE_CPP__
#ifdef __MENTOC_SHOW_VISION_FLAGS_IN_INFORMATIVE_CPP__
#define dd(A_MSG) std::cerr << "break: " << A_MSG << ", line:" << __LINE__ << "\n";
#else
#define dd(A)
#endif
	MENTOC_PREAMBLE();
	int room = player->viewing_room();
	bool dark = mods::rooms::is_dark(room);
	bool smoke = mods::rooms::is_smoked(room);
	bool on_fire = mods::rooms::is_on_fire(room);
	bool viewing_camera = player->camera_viewing();
	auto camera = player->get_camera();
	bool camera_is_thermal = camera != nullptr ? mods::object_utils::is_thermal_camera(camera->object_uuid()) : false;
	bool player_has_thermal = player->has_thermal_vision();
	auto fire_status = mods::rooms::get_fire_status(room);
	bool camera_is_night_vision = camera != nullptr ? mods::object_utils::is_night_vision_camera(camera->object_uuid()) : false;

#ifdef __MENTOC_SHOW_VISION_FLAGS_IN_INFORMATIVE_CPP__
	bool can_see_through_fire = mods::rooms::can_see_through_fire(room);
	player->sendln(
	    CAT(
	        "dark: ",(int)dark,"\r\n",
	        "smoke: ",(int)smoke,"\r\n"
	        "on_fire: ",(int)on_fire,"\r\n"
	        "viewing_camera: ",(int)viewing_camera,"\r\n"
	        "camera: ",(int)camera,"\r\n"
	        "fire_status: ",(int)fire_status,"\r\n"
	        "camera_is_thermal: ",(int)camera_is_thermal,"\r\n"
	        "camera_is_night_vision: ",(int)camera_is_night_vision,"\r\n"
	        "can_see_through_fire: ",(int)can_see_through_fire,"\r\n"
	        "player_has_thermal: ",(int)player_has_thermal,"\r\n"
	    )
	);
#endif
	for(auto& target : mods::globals::get_room_list(room)) {
		dd("target");
		if(target->is(ch)) {
			dd("is target, continuing");
			continue;
		}
		dd("check if is visible");
		if(!mods::calc_visibility::is_visible(player,target,0)) {
			if(on_fire && ((viewing_camera && camera_is_thermal) || player_has_thermal) &&
			    !mods::rooms::can_see_through_fire(fire_status)) {
				player->sendln("{yel}[on fire]{/yel} You can't seem see anything but flames.");
				break;
			}
			continue;
		}
		dd("view + thermal check");
		if(on_fire && ((viewing_camera && camera_is_thermal) || player_has_thermal) &&
		    mods::rooms::can_see_through_fire(fire_status)) {
			player->sendln(
			    CAT(
			        "{yel}[thermal vision]{/yel} You see ",
			        target->name().c_str(),
			        " here."
			    )
			);
			continue;
		}
		dd("checking camera is night vision");
		if(dark && viewing_camera && camera_is_night_vision) {
			player->sendln(
			    CAT(
			        "{grn}[night-vision via camera]{/grn} You see ",
			        target->name().c_str(),
			        " here."
			    )
			);
			continue;
		}
		dd("smoke or dark check");
		if(((smoke || dark) && viewing_camera && camera_is_thermal) ||
		    ((smoke || dark) && player_has_thermal)) {
			dd("is smoked or dark");
			/** TODO: needs testing */
			if(viewing_camera) {
				player->sendln(
				    CAT(
				        "{gld}[thermal via camera]{/gld} You see ",
				        target->name().c_str(),
				        " here."
				    )
				);
			} else {
				player->sendln(
				    CAT(
				        "{gld}[thermal vision]{/gld} You see ",
				        target->name().c_str(),
				        " here."
				    )
				);
			}
			continue;
		}
		dd("checking if dark");
		if(dark) {
			dd("it is dark");
			std::string name = "someone";
			if(mods::player_utils::is_scanned(target)) {
				dd("target is scanned");
				name = target->name();
			}
			dd("check thermal vision");
			if(player->has_thermal_vision()) {
				dd("has thermal");
				player->sendln(
				    CAT(
				        "{grn}[thermal-vision]{/grn} You see ",
				        name.c_str(),
				        " here."
				    )
				);
				continue;
			}
			dd("check night vision");
			if(player->has_night_vision()) {
				dd("has night vision");
				player->sendln(
				    CAT(
				        "{grn}[night-vision]{/grn} You see ",name.c_str()," here."
				    )
				);
				continue;
			}
		}
		dd("listing one char");
		list_one_char(target->cd(),ch);
	}


	/** TODO: thermal vision distance is short. you can only see adjacent rooms and even then the sight is limited. */
	dd("viewing room number: " << player->viewing_room());
	auto room_number = player->viewing_room();
	for(auto direction : {
	        NORTH,SOUTH,EAST,WEST,UP,DOWN
	    }) {
		dd("checking dir option");
		if(world[room_number].dir_option[direction]) {
			auto dir_string = mods::globals::dir_to_str(direction,0).c_str();
			auto looped_room_id = room_number;
			for(unsigned ctr = 0; ctr < 4; ++ctr) {
				if(world.size() <= looped_room_id) {
					break;
				}
				if(!world[looped_room_id].dir_option[direction]) {
					break;
				}
				for(auto& target : mods::globals::get_room_list(world[looped_room_id].dir_option[direction]->to_room)) {
					if(!target->get_affect_dissolver().has_affect(AFF(SCANNED))) {
						continue;
					}
					auto i = target->cd();
					if(!target->is(ch)) {
						if(mods::calc_visibility::is_visible(player,target,ctr)) {
							auto pluralized = ctr + 1 == 1 ? "" : "s";
							std::string adjusted = "right here";
							if(ctr > 1) {
								adjusted = std::to_string(ctr) + " rooms away";
							}
							if(ctr == 1) {
								adjusted = "1 room away";
							}
							player->sendln(
							    CAT(
							        "\r\n[scanned] [", dir_string,"] {grn}[",adjusted,"]{/grn} [really ",ctr + 1," room",pluralized," away]-> "
							    )
							);
							list_one_char(i, ch);
						} else if(dark  && !CAN_SEE_IN_DARK(ch) &&
						    AFF_FLAGGED(i, AFF_INFRAVISION)) {
							player->sendln("You see a pair of glowing red eyes looking your way.");
						}
					}
				}
				looped_room_id = world[looped_room_id].dir_option[direction]->to_room;
			}
		}
	}
#undef dd
}


void do_auto_exits(char_data *ch) {
	MENTOC_PREAMBLE();
	int door, slen = 0;

	player->stc("{gld}[ Exits: ");

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

		std::string dir = dirs[door];
		dir[1] = ' ';
		dir[2] = '\0';
		player->sendx(dir);
		slen++;
	}

	player->sendln(
	    CAT(
	        slen ? "" : "None!",
	        "]{/gld}"
	    )
	);
	IN_ROOM(ch) = cached_room;
}


ACMD(do_exits) {
	int door, len = 0;

	if(AFF_FLAGGED(ch, AFF_BLIND)) {
		player->sendln("You can't see a damned thing, you're blind!");
		return;
	}

	player->sendln("Obvious exits:");

	for(door = 0; door < NUM_OF_DIRS; door++) {
		if(!EXIT(ch, door) || EXIT(ch, door)->to_room == NOWHERE) {
			continue;
		}

		if(EXIT_FLAGGED(EXIT(ch, door), EX_CLOSED)) {
			continue;
		}

		len++;

		if(GET_LEVEL(ch) >= LVL_IMMORT) {
			player->sendln(
			    CAT(
			        dirs[door]," - [", GET_ROOM_VNUM(EXIT(ch, door)->to_room), "] ", world[EXIT(ch, door)->to_room].name.c_str()
			    )
			);
		} else {
			player->sendln(
			    //"%-5s - %s\r\n", dirs[door], IS_DARK(EXIT(ch, door)->to_room) &&
			    CAT(
			        dirs[door],
			        " - ",  IS_DARK(EXIT(ch, door)->to_room) && !CAN_SEE_IN_DARK(ch) ? "Too dark to tell." : world[EXIT(ch, door)->to_room].name.c_str()
			    )
			);
		}
	}

	if(!len) {
		player->sendln(" None.");
	}
}


void look_at_room(player_ptr_t& player, int ignore_brief) {
	/**
	 * Conceptually, this should be something like...
	 * player->stream_attached()
	 */
	if(!player->has_desc()) {
		return;
	}

	int room = player->viewing_room();
	look_at_room_specific(player,ignore_brief,room);
}


void look_at_room(char_data *ch, int ignore_brief) {
	MENTOC_PREAMBLE();

	if(!ch->has_desc) {
		return;
	}

	int room = player->viewing_room();
	look_at_room_specific(player,ignore_brief,room);
}

void look_at_room_specific(player_ptr_t& player, int ignore_brief,int room) {
	auto ch = player->cd();
	if(IS_NPC(ch)) {
		return;
	}
	if(!ch->has_desc) {
		return;
	}

	if(room < 0 || room >= world.size()) {
		return;
	}
	bool contract = ch->contract;

	player->sendln(CCCYN(ch, C_NRM));

	char buf[MAX_STRING_LENGTH];

	sprintbit(ROOM_FLAGS(room), room_bits, buf, sizeof(buf));
	//"[zone: %d][room vnum: %5d] [rnum: %d] %s [ %s][x:%d,y:%d,z:%d]",
	player->sendln(
	    CAT(
	        "[zone: ",world[room].zone,"]",
	        "[room vnum: ",GET_ROOM_VNUM(room),"] ",
	        "[rnum: ",room,"] ",
	        world[room].name.c_str(),
	        " [ ",buf,"]",
	        "[x:",world[room].x,
	        ",y:",world[room].y,
	        ",z:",world[room].z,
	        "]"
	    )
	);
	//"\r\n{grn}%s ->{gld}", world[room].name.c_str());
	player->sendx(
	    CAT(
	        "\r\n{grn}",
	        world[room].name.c_str(),
	        " ->{gld}"
	    )
	);
	for(auto&& flag : mods::rooms::get_room_flags_from_room(player->room())) {
		player->sendx(CAT("[",flag.c_str(),"]"));
	}
	player->sendln("{/gld}");

	player->sendln(CCNRM(ch, C_NRM));
	if((!IS_NPC(ch) && !PRF_FLAGGED(ch, PRF_BRIEF)) || ignore_brief ||
	    ROOM_FLAGGED(room, ROOM_DEATH)) {
		mods::rooms::word_wrap_description(player,room);
	}
	if(((player->get_prefs()) & PRF_OVERHEAD_MAP)) {
		if(player->ghost()) {
			player->stc(mods::classes::ghost_overhead_map::generate<mods::player*>(player.get(),room,player->ghost()->overhead_lense()));
		} else {
			player->stc(mods::overhead_map::generate<mods::player*>(player.get(),room));
		}
	}

	for(auto& t : world[room].textures()) {
		auto s = mods::rooms::texture_to_string(t);
		if(s.has_value()) {
			player->sendx(CAT("[",s.value(),"]"));
		}
	}
	if(world[room].textures().size()) {
		player->sendln();
	}

	/* autoexits */
	if(PRF_FLAGGED(ch, PRF_AUTOEXIT)) {
		do_auto_exits(ch);
	}

	list_obj_to_char(world[room].contents, ch, SHOW_OBJ_LONG, FALSE);
	player->sendx(CCYEL(ch, C_NRM));
	list_char_to_char(ch);
	player->sendx(CCNRM(ch, C_NRM));
	if(contract) {
		mods::contract_events::find_room(player,room);
	}
	mods::players::messages::consume_messages_by_player(player->uuid());

}



void look_in_direction(char_data *ch, int dir) {
	if(IS_NPC(ch)) {
		return;
	}
	MENTOC_PREAMBLE();
	if(EXIT(ch, dir)) {
		if(EXIT(ch, dir)->general_description) {
			player->sendln(EXIT(ch, dir)->general_description);
		} else {
			player->sendln("You see nothing special.");
		}

		if(EXIT_FLAGGED(EXIT(ch, dir), EX_CLOSED) && EXIT(ch, dir)->keyword) {
			player->sendln(
			    CAT(
			        "The ", fname(EXIT(ch, dir)->keyword)," is closed."
			    )
			);
		} else if(EXIT_FLAGGED(EXIT(ch, dir), EX_ISDOOR) && EXIT(ch, dir)->keyword) {
			player->sendln(
			    CAT(
			        "The ", fname(EXIT(ch, dir)->keyword),
			        " is open."
			    )
			);
		}
	} else {
		player->sendln("Nothing special there...");
	}
	if(ch->contract && EXIT_FLAGGED(EXIT(ch, dir), EX_ISDOOR)) {
		mods::contract_events::find_door(player,player->room(),dir);
	}
}



void look_in_obj(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	struct obj_data *obj = NULL;
	char_data *dummy = NULL;
	int amt, bits;

	if(!*arg) {
		player->sendln("Look in what?");
	} else if(!(bits = generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM |
	                FIND_OBJ_EQUIP, ch, &dummy, &obj))) {
		player->sendln(
		    CAT(
		        "There doesn't seem to be ", AN(arg), " ", arg, " here."
		    )
		);
	} else if((GET_OBJ_TYPE(obj) != ITEM_DRINKCON) &&
	    (GET_OBJ_TYPE(obj) != ITEM_FOUNTAIN) &&
	    (GET_OBJ_TYPE(obj) != ITEM_CONTAINER)
	) {
		player->sendln("There's nothing inside that!");
	} else {
		if(GET_OBJ_TYPE(obj) == ITEM_CONTAINER) {
			if(OBJVAL_FLAGGED(obj, CONT_CLOSED)) {
				player->sendln("It is closed.");
			} else {
				player->sendx(obj->name);

				switch(bits) {
					case FIND_OBJ_INV:
						player->sendln(" (carried):");
						break;

					case FIND_OBJ_ROOM:
						player->sendln(" (here):");
						break;

					case FIND_OBJ_EQUIP:
						player->sendln(" (used):");
						break;
					default:
						player->sendln("");
						break;
				}
				auto o = optr(obj);
				mods::examine::list_obj_contents_to_char(o,player,SHOW_OBJ_SHORT,1);
			}
		} else {		/* item must be a fountain or drink container */
			if(GET_OBJ_VAL(obj, 1) <= 0) {
				player->sendln("It is empty.");
			} else {
				if(GET_OBJ_VAL(obj,0) <= 0 || GET_OBJ_VAL(obj,1)>GET_OBJ_VAL(obj,0)) {
					player->sendln("Its contents seem somewhat murky."); /* BUG */
				} else {
					char buf2[MAX_STRING_LENGTH];
					amt = (GET_OBJ_VAL(obj, 1) * 3) / GET_OBJ_VAL(obj, 0);
					sprinttype(GET_OBJ_VAL(obj, 2), color_liquid, buf2, sizeof(buf2));
					player->sendln(
					    CAT(
					        "It's ", fullness[amt],
					        "full of a ", buf2,
					        "liquid."
					    )
					);
				}
			}
		}
	}
}

const char* find_exdesc_equipment(std::string_view word,const char_data* ch,const std::size_t& position) {
	MENTOC_PREAMBLE();
	//log("find_exdesc_equipment");
	auto obj = player->equipment(position);
	if(!obj) {
		return nullptr;
	}
	for(const auto& i : obj->ex_description) {
		if(i.keyword.length() == 0 || i.description.length() == 0) {
			continue;
		}
		if(isname(word.data(), i.keyword.c_str())) {
			return (i.description.c_str());
		}
	}
	return nullptr;
}
char* find_exdesc(std::string_view word,std::vector<extra_descr_data>& in_list) {
	//log("find_exdesc, word: %s (vector)",word.data());
	if(in_list.size() == 0) {
		return nullptr;
	}
	for(auto& i : in_list) {
		if(i.keyword.length() == 0 || i.description.length() == 0) {
			continue;
		}
		if(isname(word.data(), i.keyword.c_str())) {
			return (i.description.ptr());
		}
	}

	return nullptr;
}

char * find_exdesc(char *word,room_data& r) {
	if(r.ex_descriptions().size() == 0) {
		return nullptr;
	}
	for(auto& i : r.ex_descriptions()) {
		if(isname(word, i.keyword.c_str())) {
			return (i.description.ptr());
		}
	}
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
		player->sendln("Look at what?");
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
		player->sendln("Look at what?");
		return;
	}

	/* Does the argument match an extra desc in the room? */
	if((desc = find_exdesc(arg, world[IN_ROOM(ch)])) != NULL) {
		page_string(*ch->desc, desc, FALSE);
		return;
	}

	/* Does the argument match an extra desc in the char's equipment? */
	for(j = 0; j < NUM_WEARS && !found; j++) {
		if(GET_EQ(ch, j) && CAN_SEE_OBJ(ch, GET_EQ(ch, j)) && GET_EQ(ch, j)->ex_description.size()) {
			const char* ex = find_exdesc_equipment(arg, ch, j);
			if(!ex) {
				continue;
			}
			player->sendln(ex);
			found = TRUE;
			break;
		}
	}

	/* Does the argument match an extra desc in the char's inventory? */
	for(auto item : player->vcarrying()) {
		if(item && CAN_SEE_OBJ(ch, item)) {
			auto ex = find_exdesc(arg, item->ex_description);
			if(!ex) {
				continue;
			}
			player->sendln(ex);
			found = true;
			break;
		}
	}

	/* Does the argument match an extra desc of an object in the room? */
	//assert(IN_ROOM(ch) < world.size());

	for(obj = world[IN_ROOM(ch)].contents; obj && !found && obj->next_content; obj = obj->next_content) {
		if(CAN_SEE_OBJ(ch, obj)) {
			log("can see obj good. searching exdesc");
			if((desc = find_exdesc(arg, obj->ex_description)) != NULL && ++i == fnum) {
				player->sendln(desc);
				found = TRUE;
			}
		}
	}

	/* If an object was found back in generic_find */
	if(bits) {
		if(!found) {
			show_obj_to_char(found_obj, ch, SHOW_OBJ_ACTION);
		} else {
			show_obj_modifiers(found_obj, ch);
			player->sendln("");
		}
	} else if(!found) {
		player->sendln("You do not see that here.");
	}
}


ACMD(do_look) {
	int look_type;

	if(GET_POS(ch) < POS_SLEEPING) {
		player->sendln(STOCK_STARS_MESSAGE());
	} else if(AFF_FLAGGED(ch, AFF_BLIND)) {
		player->sendln(STOCK_BLIND_MESSAGE());
	} else if(IS_DARK(IN_ROOM(ch)) && !CAN_SEE_IN_DARK(ch)) {
		player->sendln(STOCK_PITCH_BLACK_MESSAGE());
	} else {
		char arg[MAX_INPUT_LENGTH], arg2[MAX_INPUT_LENGTH];

		half_chop(argument, arg, arg2);

		if(subcmd == SCMD_READ) {
			if(!*arg) {
				player->sendln(STOCK_READ_WHAT_MESSAGE());
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
	char_data *tmp_char;
	struct obj_data *tmp_object;
	char tempsave[MAX_INPUT_LENGTH], arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln(STOCK_EXAMINE_MESSAGE());
		return;
	}

	/* look_at_target() eats the number. */
	look_at_target(ch, strcpy(tempsave, arg));	/* strcpy: OK */

	generic_find(arg, FIND_OBJ_INV | FIND_OBJ_ROOM | FIND_CHAR_ROOM |
	    FIND_OBJ_EQUIP, ch, &tmp_char, &tmp_object);

	if(tmp_object) {
		if(is_camera_feed(tmp_object)) {
			look_into_camera_feed(player,tmp_object,0);
			return;
		}
		auto object = optr(tmp_object);
		if(look_at_weapon(player,object)) {
			return;
		}
		if((GET_OBJ_TYPE(tmp_object) == ITEM_DRINKCON) ||
		    (GET_OBJ_TYPE(tmp_object) == ITEM_FOUNTAIN) ||
		    (GET_OBJ_TYPE(tmp_object) == ITEM_CONTAINER)) {
			player->sendln(STOCK_LOOK_INSIDE_MESSAGE());
			look_in_obj(ch, arg);
		}
	}
}



ACMD(do_inventory) {
	player->sendln("You are carrying:");
	list_obj_to_char(ch->carrying, ch, SHOW_OBJ_SHORT, TRUE);
}


ACMD(do_equipment) {
	int i, found = 0;

	player->sendln("You are using:");

	for(i = 0; i < NUM_WEARS; i++) {
		if(GET_EQ(ch, i)) {
			if(CAN_SEE_OBJ(ch, GET_EQ(ch, i))) {
				player->sendx(wear_where[i]);
				show_obj_to_char(GET_EQ(ch, i), ch, SHOW_OBJ_SHORT);
				found = TRUE;
			} else {
				player->sendx(wear_where[i]);
				player->sendln("Something.");
				found = TRUE;
			}
		}
	}

	if(!found) {
		player->sendln(" Nothing.");
	}
}


ACMD(do_time) {
	const char *suf;
	int weekday, day;

	/* day in [1..35] */
	day = time_info.day + 1;

	/* 35 days in a month, 7 days a week */
	weekday = ((35 * time_info.month) + day) % 7;

	player->sendln(
	    //"It is %d o'clock %s, on %s.\r\n",
	    CAT(
	        "It is ", (time_info.hours % 12 == 0) ? 12 : (time_info.hours % 12),
	        " o'clock ", time_info.hours >= 12 ? "pm" : "am", weekdays[weekday],", ",
	        " on ", weekdays[weekday],"."
	    )
	);

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

	player->sendln(
	    //"The %d%s Day of the %s, Year %d.\r\n",
	    CAT(
	        "The ", day, suf, " Day of the ",
	        month_name[time_info.month],", ",
	        "Year ", time_info.year,"."
	    )
	);
}


ACMD(do_weather) {
	const char *sky_look[] = {
		"cloudless",
		"cloudy",
		"rainy",
		"lit by flashes of lightning"
	};

	if(OUTSIDE(ch)) {
		//"The sky is %s and %s.\r\n", sky_look[weather_info.sky],
		//       weather_info.change >= 0 ? "you feel a warm wind from south" :
		//       "your foot tells you bad weather is due");
		player->sendln(
		    CAT(
		        "The sky is ", sky_look[weather_info.sky],
		        " and ", weather_info.change >= 0 ? "you feel a warm wind from south" :
		        "your foot tells you bad weather is due",
		        "."
		    )
		);

		if(GET_LEVEL(ch) >= LVL_GOD)
			//"Pressure: %d (change: %d), Sky: %d (%s)\r\n",
			player->sendln(
			    CAT(
			        "Pressure: %d ",
			        weather_info.pressure,
			        "(change: ",weather_info.change,"), ",
			        "Sky: ", weather_info.sky, "(", sky_look[weather_info.sky], ")"
			    )
			);
	} else {
		player->sendln("You have no feeling about the weather at all.");
	}
}


#define WHO_FORMAT \
	"format: who [minlev[-maxlev]] [-n name] [-c classlist] [-s] [-o] [-q] [-r] [-z]"

ACMD(do_who) {
	player->sendln("Players-------");

	int num_can_see = 0;
	for(auto& pair : mods::globals::player_name_map) {
		if(mods::super_users::player_is(player)) {
			num_can_see++;
			auto s = CAT("[",pair.second->get_class_string().str(),"] - ",pair.first.c_str());
			if(mods::super_users::invisible(pair.second)) {
				s += green_str(" (invisible to non-immortals)");
			} else {
				s += CAT("[displayed as:'",mods::super_users::who_line(pair.second),"' to non-immortals]");
			}
			s += reset_str();
			player->sendln(s);
			continue;
		}
		if(mods::super_users::invisible(pair.second)) {
			continue;
		}
		num_can_see++;
		player->sendln(CAT("[",pair.second->get_class_string().str(),"] - ",mods::super_users::who_line(pair.second)));
	}

	if(num_can_see == 0) {
		player->sendln("\r\nNobody at all!");
	} else if(num_can_see == 1) {
		player->sendln("\r\nOne lonely character displayed.");
	} else {
		player->sendln(
		    CAT(
		        "\r\n", num_can_see," characters displayed."
		    )
		);
	}
}


#define USERS_FORMAT \
	"format: users [-l minlevel[-maxlevel]] [-n name] [-h host] [-c classlist] [-o] [-p]"

/* BIG OL' FIXME: Rewrite it all. Similar to do_who(). */
ACMD(do_users) {
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
					player->sendln(USERS_FORMAT);
					return;
			}				/* end of switch */

		} else {			/* endif */
			player->sendln(USERS_FORMAT);
			return;
		}
	}				/* end while (parser) */

	player->sendln(
	    "Num Class   Name         State          Idl Login@   Site"
	    "--- ------- ------------ -------------- --- -------- ------------------------");

	one_argument(argument, arg);

	for(auto& d : descriptor_list) {
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
			player->sendln(line);
			num_can_see++;
		}
	}

	player->sendln(
	    CAT(
	        "\r\n",num_can_see," visible sockets connected."
	    )
	);
}


/* Generic page_string function for displaying text */
ACMD(do_gen_ps) {
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
			player->sendln("\033[H\033[J");
			break;

		case SCMD_VERSION:
			player->sendln(circlemud_version);
			break;

		case SCMD_WHOAMI:
			player->sendln(GET_NAME(ch).c_str());
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
		player->sendln("Players in your Zone\r\n--------------------");

		for(auto& d : descriptor_list) {
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

			player->sendln(
			    CAT(
			        GET_NAME(i).c_str(), " - ",  world[IN_ROOM(i)].name.c_str()
			    )
			);
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

			player->sendln(
			    CAT(
			        GET_NAME(i).c_str(), " - ",  world[IN_ROOM(i)].name.c_str()
			    )
			);
			return;
		}

		player->sendln("Nobody around by that name.");
	}
}


void print_object_location(int num, struct obj_data *obj, char_data *ch,
    int recur) {
	MENTOC_PREAMBLE();
	if(num > 0) {
		//"O%3d. %-25s - ", num, obj->short_description);
		player->sendx(
		    CAT(
		        "O",num,". ",
		        obj->short_description,
		        " - "
		    )
		);
	} else {
		player->sendx(CAT(std::string(" ",33)).c_str());//"%33s", " - ");
	}

	if(IN_ROOM(obj) != NOWHERE) {
		//"[%5d] %s\r\n", GET_ROOM_VNUM(IN_ROOM(obj)), world[IN_ROOM(obj)].name.c_str());
		player->sendln(
		    CAT(
		        "[", GET_ROOM_VNUM(IN_ROOM(obj)), "] ", world[IN_ROOM(obj)].name.c_str()
		    )
		);
	} else if(obj->carried_by) {
		player->sendln(
		    CAT(
		        "carried by ", std::string(PERS(obj->carried_by, ch)).c_str()
		    )
		);
	} else if(obj->worn_by) {
		player->sendln(
		    CAT(
		        "worn by ", std::string(PERS(obj->worn_by, ch)).c_str()
		    )
		);
	} else if(obj->in_obj) {
		player->sendln(
		    CAT(
		        "inside ", obj->in_obj->short_description, (recur ? ", which is" : " ")
		    )
		);

		if(recur) {
			print_object_location(0, obj->in_obj, ch, recur);
		}
	} else {
		player->sendln("in an unknown location\r\n");
	}
}



void perform_immort_where(char_data *ch, char *arg) {
	MENTOC_PREAMBLE();
	char_data *i;
	struct obj_data *k;
	int num = 0, found = 0;

	if(!*arg) {
		player->sendln("Players\r\n-------");

		for(auto& d : descriptor_list)
			if(STATE(d) == CON_PLAYING) {
				i = (d.original ? d.original : d.character);

				if(i && CAN_SEE(ch, i) && (IN_ROOM(i) != NOWHERE)) {
					if(d.original)
						//"%-20s - [%5d] %s (in %s)\r\n",
						player->sendln(
						    CAT(
						        GET_NAME(i).c_str(), " - [",GET_ROOM_VNUM(IN_ROOM(d.character)),"] ",
						        world[IN_ROOM(d.character)].name.c_str(),
						        " (in ", GET_NAME(d.character).c_str(),")"
						    )
						);
					else {
						//"%-20s - [%5d] %s\r\n", GET_NAME(i).c_str(), GET_ROOM_VNUM(IN_ROOM(i)), world[IN_ROOM(i)].name.c_str());
						player->sendln(
						    CAT(
						        GET_NAME(i).c_str()," - [",
						        GET_ROOM_VNUM(IN_ROOM(i)), "] ",
						        world[IN_ROOM(i)].name.c_str()
						    )
						);
					}
				}
			}
	} else {
		for(i = character_list; i; i = i->next) {
			if(CAN_SEE(ch, i) && IN_ROOM(i) != NOWHERE && isname(arg, i->player.name)) {
				found = 1;
				player->sendln(
				    CAT(
				        ++num,". ", GET_NAME(i).c_str()," - [", GET_ROOM_VNUM(IN_ROOM(i)), "] ",
				        world[IN_ROOM(i)].name.c_str()
				    )
				);
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
			player->sendln("Couldn't find any such thing.");
		}
	}//End else
}



ACMD(do_where) {
	/*
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(GET_LEVEL(ch) >= LVL_IMMORT) {
		perform_immort_where(ch, arg);
	} else {
		perform_mortal_where(ch, arg);
	}*/
}



ACMD(do_levels) {
	char buf[MAX_STRING_LENGTH];
	unsigned i = 0, len = 0;
	int nlen;

	if(IS_NPC(ch)) {
		return;
	}

	for(i = 1; i < LVL_IMMORT; i++) {
		nlen = snprintf(buf + len, sizeof(buf) - len, "[%2d] %8d-%-8d : ", i,
		        mods::levels::level_exp(i), mods::levels::level_exp(i + 1) - 1);

		if(len + nlen >= sizeof(buf) || nlen < 0) {
			break;
		}

		len += nlen;

		switch(GET_SEX(ch)) {
			case SEX_MALE:
			case SEX_NEUTRAL:
			default:
				nlen = snprintf(buf + len, sizeof(buf) - len, "%s\r\n", title_male(GET_CLASS(ch), i));
				break;

			case SEX_FEMALE:
				nlen = snprintf(buf + len, sizeof(buf) - len, "%s\r\n", title_female(GET_CLASS(ch), i));
				break;
		}

		if(len + nlen >= sizeof(buf) || nlen < 0) {
			break;
		}

		len += nlen;
	}

	if(len < sizeof(buf))
		snprintf(buf + len, sizeof(buf) - len, "[%2d] %8d          : Immortality\r\n",
		    LVL_IMMORT, mods::levels::level_exp(LVL_IMMORT));

	page_string(*ch->desc, buf, TRUE);
}



ACMD(do_consider) {
	char buf[MAX_INPUT_LENGTH];
	char_data *victim;
	int diff;

	one_argument(argument, buf);

	if(!(victim = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
		player->sendln("Consider killing who?");
		return;
	}

	if(victim == ch) {
		player->sendln("Easy!  Very easy indeed!");
		return;
	}

	if(!IS_NPC(victim)) {
		player->sendln("Would you like to borrow a cross and a shovel?");
		return;
	}

	diff = (GET_LEVEL(victim) - GET_LEVEL(ch));

	if(diff <= -10) {
		player->sendln("Now where did that chicken go?");
	} else if(diff <= -5) {
		player->sendln("You could do it with a needle!");
	} else if(diff <= -2) {
		player->sendln("Easy.");
	} else if(diff <= -1) {
		player->sendln("Fairly easy.");
	} else if(diff == 0) {
		player->sendln("The perfect match!");
	} else if(diff <= 1) {
		player->sendln("You would need some luck!");
	} else if(diff <= 2) {
		player->sendln("You would need a lot of luck!");
	} else if(diff <= 3) {
		player->sendln("You would need a lot of luck and great equipment!");
	} else if(diff <= 5) {
		player->sendln("Do you feel lucky, punk?");
	} else if(diff <= 10) {
		player->sendln("Are you mad!?");
	} else if(diff <= 100) {
		player->sendln("You ARE mad!");
	}
}



ACMD(do_diagnose) {
	char buf[MAX_INPUT_LENGTH];
	char_data *vict;

	one_argument(argument, buf);

	if(*buf) {
		if(!(vict = get_char_vis(ch, buf, NULL, FIND_CHAR_ROOM))) {
			player->sendln(NOPERSON);
		} else {
			diag_char_to_char(vict, ch);
		}
	} else {
		if(FIGHTING(ch)) {
			diag_char_to_char(FIGHTING(ch), ch);
		} else {
			player->sendln("Diagnose who?");
		}
	}
}


const char *ctypes[] = {
	"off", "sparse", "normal", "complete", "\n"
};

ACMD(do_color) {
	char arg[MAX_INPUT_LENGTH];
	int tp;

	if(IS_NPC(ch)) {
		return;
	}

	one_argument(argument, arg);

	if(!*arg) {
		player->sendln(
		    CAT(
		        "Your current color level is ", ctypes[COLOR_LEV(ch)]
		    )
		);
		return;
	}

	if((tp = search_block(arg, ctypes, FALSE)) == -1) {
		player->sendln("Usage: color { Off | Sparse | Normal | Complete }");
		return;
	}

	REMOVE_BIT(PRF_FLAGS(ch), PRF_COLOR_1 | PRF_COLOR_2);
	SET_BIT(PRF_FLAGS(ch), (PRF_COLOR_1 * (tp & 1)) | (PRF_COLOR_2 * (tp & 2) >> 1));

	player->sendln(
	    CAT(
	        "Your color is now ", ctypes[tp]
	    )
	);
}


ACMD(do_toggle) {
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
		player->sendln(
		    CAT(
		        "      No Hassle: ", ONOFF(PRF_FLAGGED(ch, PRF_NOHASSLE)), "    ",
		        "      Holylight: ", ONOFF(PRF_FLAGGED(ch, PRF_HOLYLIGHT)), "    ",
		        "     Room Flags: ", ONOFF(PRF_FLAGGED(ch, PRF_ROOMFLAGS)), "    "
		    )
		);
	}

	//TODO uncomment
#if 0
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
#endif
}


int sort_commands_helper(const void *a, const void *b) {
	return strcmp(cmd_info[*(const int *)a].command, cmd_info[*(const int *)b].command);
}


void sort_commands(void) {
	int a, num_of_cmds = 0;

	while(*cmd_info[num_of_cmds].command != '\n') {
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
	int no, i, cmd_num;
	int wizhelp = 0, socials = 0;
	char_data *vict;
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(*arg) {
		if(!(vict = get_char_vis(ch, arg, NULL, FIND_CHAR_WORLD)) || IS_NPC(vict)) {
			player->sendln("Who is that?");
			return;
		}

		if(GET_LEVEL(ch) < GET_LEVEL(vict)) {
			player->sendln("You can't see the commands of people above your level.");
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

	player->sendln(
	    CAT("The following ",
	        wizhelp ? "privileged " : "",
	        socials ? "socials" : "commands",
	        " are available to '",
	        vict == ch ? "you" : GET_NAME(vict),
	        "':"
	    )
	);

	/* cmd_num starts at 1, not 0, to remove 'RESERVED' */
	for(no = 1, cmd_num = 1; *cmd_info[cmd_sort_info[cmd_num]].command != '\n'; cmd_num++) {
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

		send_to_char(ch, "%-11s%s", cmd_info[i].str_command.c_str(), no++ % 7 == 0 ? "" : "");
	}

	if(no % 7 != 1) {
		player->sendln("\r\n");
	}
}

extern void shop_view_item(player_ptr_t& player, int16_t item_number);
ACMD(do_view) {
	DO_HELP("view");
	/** If the user is attempting to view an item in a shop */
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() && !mods::util::is_lower_match(vec_args[0], "camera")) {
		int16_t item = mods::util::stoi(vec_args[0]).value_or(-1);
		shop_view_item(player,item);
		return;
	}
	if(vec_args.size() == 0) {
		player->sendln("Usage: 'view camera'. For more information, type 'view help'");
		return;
	}
	if(mods::util::is_lower_match(vec_args[0], "camera")) {
		if(!player->get_camera()) {
			player->sendln("You haven't installed a camera");
			return;
		}
		player->set_camera_viewing(true);
		look_at_room(ch,0);
		player->set_camera_viewing(false);
		return;
	}

}

namespace informative {
	void init() {
		mods::interpreter::add_command("action",POS_RESTING,do_action,0,0);
		mods::interpreter::add_command("color",POS_RESTING,do_color,0,0);
		mods::interpreter::add_command("commands",POS_RESTING,do_commands,0,0);
		mods::interpreter::add_command("consider",POS_RESTING,do_consider,0,0);
		mods::interpreter::add_command("diagnose",POS_RESTING,do_diagnose,0,0);
		mods::interpreter::add_command("drone",POS_RESTING,do_drone,0,0);
		mods::interpreter::add_command("equipment",POS_RESTING,do_equipment,0,0);
		mods::interpreter::add_command("examine",POS_RESTING,do_examine,0,0);
		mods::interpreter::add_command("exits",POS_RESTING,do_exits,0,0);
		mods::interpreter::add_command("gen_ps",POS_RESTING,do_gen_ps,0,0);
		mods::interpreter::add_command("givemegold",POS_RESTING,do_givemegold,0,0);
		mods::interpreter::add_command("insult",POS_RESTING,do_insult,0,0);
		mods::interpreter::add_command("inventory",POS_RESTING,do_inventory,0,0);
		mods::interpreter::add_command("js",POS_RESTING,do_js,0,0);
		mods::interpreter::add_command("jstest",POS_RESTING,do_jstest,0,0);
		mods::interpreter::add_command("levels",POS_RESTING,do_levels,0,0);
		mods::interpreter::add_command("look",POS_RESTING,do_look,0,0);
		mods::interpreter::add_command("newjs",POS_RESTING,do_newjs,0,0);
		mods::interpreter::add_command("preferences",POS_RESTING,do_preferences,0,0);
		mods::interpreter::add_command("recall",POS_RESTING,do_recall,0,0);
		mods::interpreter::add_command("time",POS_RESTING,do_time,0,0);
		mods::interpreter::add_command("toggle",POS_RESTING,do_toggle,0,0);
		mods::interpreter::add_command("users",POS_RESTING,do_users,0,0);
		mods::interpreter::add_command("view",POS_RESTING,do_view,0,0);
		mods::interpreter::add_command("weather",POS_RESTING,do_weather,0,0);
		mods::interpreter::add_command("where",POS_RESTING,do_where,0,0);
		mods::interpreter::add_command("who",POS_RESTING,do_who,0,0);
	}
};
