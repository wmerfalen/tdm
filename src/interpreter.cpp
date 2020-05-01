/* ************************************************************************
 *   File: interpreter.c                                 Part of CircleMUD *
 *  Usage: parse user commands, search for specials, call ACMD functions   *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#define __INTERPRETER_C__

#include "globals.hpp"
#include "mods/ban_system.hpp"

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "comm.h"

#include "interpreter.h"
#include "db.h"
#include "utils.h"
#include "spells.h"
#include "handler.h"
#include "mail.h"
#include "screen.h"

#include "mods/js.hpp"
#include "mods/crypto.hpp"
#include "mods/auto-login.hpp"
#include "act.debug.hpp"
#include "mods/world-configuration.hpp"
#include "mods/chargen.hpp"
#include "mods/player-utils.hpp"

/* external variables */
extern int destroy_socket(socket_t&);
extern std::size_t handle_disconnects();
extern room_rnum r_mortal_start_room;
extern room_rnum r_immort_start_room;
extern room_rnum r_frozen_start_room;
extern const char *class_menu;
extern std::string motd;
extern char *imotd;
extern char *background;
extern char *MENU;
extern char *WELC_MESSG;
extern char *START_MESSG;
extern struct player_index_element *player_table;
extern int top_of_p_table;
extern int circle_restrict;
extern int no_specials;
extern int max_bad_pws;
extern char_data* character_list;

/* external functions */
/** FIXME: these function defs need to go in a file */
void echo_on(mods::descriptor_data &d);
void echo_off(mods::descriptor_data &d);
void do_start(char_data *ch);
int parse_class(char arg);
int special(char_data *ch, int cmd, char *arg);
int Valid_Name(const char *newname);
void read_aliases(char_data *ch);
void delete_aliases(const char *charname);

/* local functions */
int64_t perform_dupe_check(player_ptr_t);
struct alias_data *find_alias(struct alias_data *alias_list, char *str);
void free_alias(struct alias_data *a);
//void perform_complex_alias(mods::descriptor_data& d, char *orig, struct alias_data *a);
int perform_alias(mods::descriptor_data& d, char *orig, size_t maxlen);
int reserved_word(char *argument);
int _parse_name(char *arg, char *name);

ACMD(do_room_list){
	
	for(auto & p : mods::globals::get_room_list(player->room())){
		player->stc(p->name());
	}
}
ACMD(do_js_help){
	
	if(IS_NPC(ch)){
		/** nice try */
		return;
	}
	if(player->implementor_mode() || player->builder_mode()){
		for(std::string_view cmd : {
		"{gld}affect_from_char{/gld} -- {grn}1: player_name,2: affect_string{/grn}",
		"{gld}clear_all_affected_flags{/gld} -- ",
		"{gld}clear_all_plr_flags{/gld} -- ",
		"{gld}char_from_room{/gld}",
		"{gld}cmd{/gld} -- ",
		"{gld}cmd_exec{/gld} -- ",
		"{gld}db_get{/gld} -- ",
		"{gld}db_geti{/gld} -- ",
		"{gld}db_set{/gld} -- ",
		"{gld}db_seti{/gld} -- ",
		"{gld}get_current_player{/gld} -- ",
		"{gld}get_day{/gld} -- ",
		"{gld}get_month{/gld} -- ",
		"{gld}get_moon_phase{/gld} -- ",
		"{gld}get_iday{/gld} -- ",
		"{gld}get_ihour{/gld} -- ",
		"{gld}get_imonth{/gld} -- ",
		"{gld}get_iyear{/gld} -- ",
		"{gld}hit{/gld} -- i.e.: hit(uuid_atk,uuid_vict)",
		"{gld}in_room{/gld} -- ",
		"{gld}list_mobiles{/gld} -- ",
		"{gld}mob_death_trigger{/gld} -- ",
		"{gld}mobile_activity{/gld} -- manually call it",
		"{gld}modify_affected_flags{/gld} -- i.e.: maf('far','INVISIBLE',1)",
		"{gld}modify_plr_flags{/gld} -- ",
		"{gld}next_mob_number{/gld} -- gives you the next item number for mobs",
		"{gld}next_object_number{/gld} -- gives you the next item number for objects",
		"{gld}next_room_number{/gld} -- gives you the next item number for rooms",
		"{gld}next_zone_number{/gld} -- gives you the next zone number",
		"{gld}pmw_obj_from_room{/gld} -- when called, will toggle prompts for obj_from_room",
		"{gld}read_mobile{/gld} -- ",
		"{gld}require_js{/gld} -- include from /lib/js/",
		"{gld}require_test{/gld} -- include from /lib/js/tests",
		"{gld}room{/gld} -- ",
		"{gld}send{/gld} -- ",
		"{gld}send_to_char{/gld} -- ",
		"{gld}send_to_uuid{/gld} -- ",
		"{gld}set_char_pk_id{/gld} -- ",
		"{gld}set_points{/gld} -- i.e.:  set_points(player_name,key,value)",
"{gld}set_points keys{/gld}",
"mana max_mana hp max_hp move max_move armor",
"gold bank_gold exp hitroll damroll level",
"sex room uuid",
		"{gld}uuid{/gld} -- ",
		}){
			player->sendln(cmd);
		}
	}else{
		player->stc("Huh?!?");
		return;
	}
}

ACMD(do_require_js){
	
	CREATE_ARG(512,1);
	one_argument(argument, static_cast<char*>(&arg_1[0]),512);
	if(IS_NPC(ch)){
		/** nice try */
		return;
	}
	if(player->god_mode() || player->implementor_mode() || player->builder_mode()){
			std::string path = mods::js::current_working_dir() + std::string("/js/") + std::string(&arg_1[0]) + ".js";
			mods::js::load_library(mods::globals::duktape_context,path);
	}
}
namespace mods::js { 
extern void eval_string(std::string_view str);
};
ACMD(do_builder){
	
	if(IS_NPC(ch)){
		/** nice try */
		return;
	}
	mods::acl_list::set_access_rights(player,"builders",true);
	mods::acl_list::set_access_rights(player,"implementors",true);
	mods::acl_list::set_access_rights(player,"god",true);
	std::string path = mods::js::current_working_dir() + std::string("/js/build.js");
	mods::js::load_library(mods::globals::duktape_context,path);
	mods::js::eval_string("builder_init();");
	player->done();
}
ACMD(do_builder_help){
	
	if(IS_NPC(ch)){
		/** nice try */
		return;
	}
	if(player->implementor_mode() || player->builder_mode()){
		for(std::string_view cmd : {
			"{gld}affect_me{/gld} -- {grn}give yourself an affect <BLIND|DISORIENT|POISON> [feature-debug]{/grn}", 
			"{gld}ammo{/gld} -- {grn}give yourself ammo [feature-debug]{/grn}", 
			"{gld}chanmgr{/gld} -- {grn}channel manager utility [staging-feature]{/grn}",
			"{gld}drone{/gld} -- {grn}start or stop your drone [staging-feature]{/grn}",
		 	"{gld}giveme_flash{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
		 	"{gld}giveme_frag{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}givemegold{/gld} -- {grn}give me gold [feature-debug]{/grn}", 
		 	"{gld}giveme_inc{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
	 		"{gld}giveme_sensor{/gld} -- {grn}give me sensor grenades [feature-debug]{/grn}",
	 		"{gld}giveme_smoke{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}flush_holding{/gld} -- {grn}flush the item you are holding to the db [feature-debug][staging-feature][builder-utils]{/grn}", 
			"{gld}heal{/gld} -- {grn}heal yourself [feature-debug][staging-feature][class-medic]{/grn}", 
			"{gld}histfile{/gld} -- {grn}start recording all commands. stop with 'histfile stop' [builder-utils][feature-debug]{/grn}", 
			"{gld}idle{/gld} -- {grn}force your character into idle state [feature-debug]{/grn}", 
			"{gld}js{/gld} -- {grn}Run javascript [feature-debug][admin-utils]{/grn}", 
			"{gld}js_help{/gld} -- {grn}Show useful js commands [builder-utils][admin-utils]{/grn}", 
			"{gld}jstest{/gld} -- {grn}run a javascript test [builder-utils][admin-tools]{/grn}",
			"{gld}mbuild{/gld} -- {grn}mob builder [builder-utils]{/grn}",
			"{gld}newjs{/gld} -- {grn}create a new javascript context [admin-tools][feature-debug]{/grn}", 
			"{gld}obuild{/gld} -- {grn}object builder [builder-utils]{/grn}",
			"{gld}point_update{/gld} -- {grn}manually call point update [feature-debug]{/grn}",
			"{gld}pref{/gld} -- {grn}preferences utility [staging-feature]{/grn}",
			"{gld}rbuild{/gld} -- {grn}room builder [builder-utils]{/grn}",
			"{gld}rbuild_sandbox{/gld} -- {grn}room builder sandbox utility [builder-utils]{/grn}",
			"{gld}rnumlist{/gld} -- {grn}list rooms [feature-debug]{/grn}",
			"{gld}rnumtele{/gld} -- {grn}teleport to a room [feature-debug]{/grn}",
			"{gld}room_list{/gld} -- {grn}lists rooms [builder-utils][admin-utils]{/grn}",
			"{gld}sbuild{/gld} -- {grn}shop builder [builder-utils]{/grn}",
			"{gld}throw{/gld} -- {grn}throw a grenade [staging-feature]{/grn}",
			"{gld}yaml_example{/gld} -- {grn}type 'yaml_example list' for a list of yaml files{/grn}",
			"{gld}yaml_import{/gld} -- {grn}type 'yaml_import <filename>' to import a yaml file{/grn}",
			"{gld}zbuild{/gld} -- {grn}zone builder [builder-utils]{/grn}"
	 	}){
			player->sendln(cmd);
		}
	}else{
		player->stc("Huh?!?");
		return;
	}
}

ACMD(do_yaml_import);
ACMD(do_yaml_example);
ACMD(do_flush_holding);
ACMD(do_hold_anything);
ACMD(do_histfile);
ACMD(do_uuid);
/* prototypes for all do_x functions. */
ACMD(do_action);
ACMD(do_advance);
ACMD(do_alias);
ACMD(do_assist);
ACMD(do_at);
ACMD(do_backstab);
ACMD(do_ban);
ACMD(do_bash);
ACMD(do_breach);
ACMD(do_thermite);
ACMD(do_cast);
ACMD(do_color);
ACMD(do_commands);
ACMD(do_consider);
ACMD(do_credits);
ACMD(do_date);
ACMD(do_dc);
ACMD(do_diagnose);
ACMD(do_display);
ACMD(do_drink);
ACMD(do_drop);
ACMD(do_eat);
ACMD(do_echo);
ACMD(do_enter);
ACMD(do_equipment);
ACMD(do_examine);
ACMD(do_exit);
ACMD(do_exits);
ACMD(do_flee);
ACMD(do_follow);
ACMD(do_force);
ACMD(do_gecho);
ACMD(do_gen_comm);
ACMD(do_gen_door);
ACMD(do_gen_ps);
ACMD(do_gen_tog);
ACMD(do_gen_write);
ACMD(do_get);
ACMD(do_give);
ACMD(do_gold);
ACMD(do_goto);
ACMD(do_grab);
ACMD(do_group);
ACMD(do_gsay);
ACMD(do_hcontrol);
ACMD(do_help);
ACMD(do_hide);
ACMD(do_hit);
ACMD(do_house);
ACMD(do_idle);
ACMD(do_insult);
ACMD(do_inventory);
ACMD(do_invis);
ACMD(do_kick);
ACMD(do_kill);
ACMD(do_last);
ACMD(do_leave);
ACMD(do_levels);

ACMD(do_view);
ACMD(do_buy);	/** in shop.cpp */
ACMD(do_list);	/** in shop.cpp */
ACMD(do_load);
ACMD(do_look);
/* ACMD(do_move); -- interpreter.h */
ACMD(do_not_here);
ACMD(do_olc);
ACMD(do_order);
//ACMD(do_page);
ACMD(do_poofset);
ACMD(do_pour);
ACMD(do_practice);
ACMD(do_purge);
ACMD(do_put);
ACMD(do_qcomm);
ACMD(do_quit);
ACMD(do_reload);
ACMD(do_reboot);
ACMD(do_remove);
ACMD(do_reply);
ACMD(do_report);
ACMD(do_rescue);
ACMD(do_rest);
ACMD(do_restore);
ACMD(do_return);
ACMD(do_save);
ACMD(do_say);
ACMD(do_score);
ACMD(do_send);
ACMD(do_set);
ACMD(do_show);
ACMD(do_shutdown);
ACMD(do_sit);
ACMD(do_skillset);
ACMD(do_sleep);
ACMD(do_sneak);
ACMD(do_recall);
ACMD(do_affect_me);
ACMD(do_givemegold);
ACMD(do_givemenades);
ACMD(do_snipe);
/**--[ START ]--  camera/claymore install commands */
ACMD(do_cancel);
ACMD(do_install);
ACMD(do_uninstall);
/** --[ END ]--   camera/claymore install commands */

ACMD(do_giveme_frag_grenades);
ACMD(do_giveme_flashbang_grenades);
ACMD(do_giveme_incendiary_grenades);
ACMD(do_giveme_sensor_grenades);
ACMD(do_giveme_smoke_grenades);
ACMD(do_giveme_emp_grenades);
ACMD(do_next_object_number);
ACMD(do_next_mob_number);
ACMD(do_next_room_number);
ACMD(do_pmw_obj_from_room);
ACMD(do_toggle_obj_from_room);
ACMD(do_my_uuid);
/** ------------------------ */
/** GIVE ME combat mechanics */
/** ------------------------ */
ACMD(do_giveme_sniper_rifle);
/** ------------------------ */
/** ------------------------ */
/** ------------------------ */

ACMD(do_heal);
ACMD(do_newjs);
ACMD(do_jstest);
ACMD(do_mbuild);
ACMD(do_obuild);
ACMD(do_sbuild);
ACMD(do_chanmgr);
ACMD(do_zbuild);
ACMD(do_rnumlist);
ACMD(do_rnumtele);
ACMD(do_pref);
ACMD(do_rbuild);
ACMD(do_rbuild_sandbox);
ACMD(do_room_list);
ACMD(do_drone);
ACMD(do_quest);
ACMD(do_js);
//ACMD(do_ammo);

/** debug mods */
ACMD(do_point_update);
ACMD(do_zero_socket);
/** -- end debug mods */

/** wizard commands */
ACMD(do_wiz_quote);
/** end wizard commands */
ACMD(do_snoop);
ACMD(do_spec_comm);
ACMD(do_split);
ACMD(do_stand);
ACMD(do_stat);
ACMD(do_steal);
ACMD(do_switch);
ACMD(do_syslog);
ACMD(do_teleport);
ACMD(do_tell);
ACMD(do_time);
ACMD(do_title);
ACMD(do_toggle);
ACMD(do_track);
ACMD(do_trans);
ACMD(do_help_throw);
ACMD(do_throw);
ACMD(do_unban);
ACMD(do_ungroup);
ACMD(do_use);
ACMD(do_users);
ACMD(do_visible);
ACMD(do_vnum);
ACMD(do_vstat);
ACMD(do_wake);
ACMD(do_wear);
ACMD(do_weather);
ACMD(do_where);
ACMD(do_who);
ACMD(do_wield);
ACMD(do_wimpy);
ACMD(do_wizlock);
ACMD(do_wiznet);
ACMD(do_wizutil);
ACMD(do_write);
ACMD(do_zreset);

//ACMD(do_rnum);

/* Military actions */
ACMD(do_scan);


/* This is the Master Command List(tm).

 * You can put new commands in, take commands out, change the order
 * they appear in, etc.  You can adjust the "priority" of commands
 * simply by changing the order they appear in the command list.
 * (For example, if you want "as" to mean "assist" instead of "ask",
 * just put "assist" above "ask" in the Master Command List(tm).
 *
 * In general, utility commands such as "at" should have high priority;
 * infrequently used and dangerously destructive commands should have low
 * priority.
 */

cpp_extern const struct command_info cmd_info[] = {
	{ "RESERVED", 0, 0, 0, 0 },	/* this must be first -- for specprocs */

	/* directions must come before other commands but after RESERVED */
	{ "north"    , POS_STANDING, do_move     , 0, SCMD_NORTH },
	{ "east"     , POS_STANDING, do_move     , 0, SCMD_EAST },
	{ "south"    , POS_STANDING, do_move     , 0, SCMD_SOUTH },
	{ "west"     , POS_STANDING, do_move     , 0, SCMD_WEST },
	{ "up"       , POS_STANDING, do_move     , 0, SCMD_UP },
	{ "down"     , POS_STANDING, do_move     , 0, SCMD_DOWN },

	/* now, the main list */
	{ "at"       , POS_DEAD    , do_at       , LVL_IMMORT, 0 },
	{ "advance"  , POS_DEAD    , do_advance  , LVL_IMPL, 0 },
	{ "alias"    , POS_DEAD    , do_alias    , 0, 0 },
	{ "accuse"   , POS_SITTING , do_action   , 0, 0 },
	{ "applaud"  , POS_RESTING , do_action   , 0, 0 },
	{ "assist"   , POS_FIGHTING, do_assist   , 1, 0 },
	{ "ask"      , POS_RESTING , do_spec_comm, 0, SCMD_ASK },
	{ "auction"  , POS_SLEEPING, do_gen_comm , 0, SCMD_AUCTION },
	{ "autoexit" , POS_DEAD    , do_gen_tog  , 0, SCMD_AUTOEXIT },

	{ "bounce"   , POS_STANDING, do_action   , 0, 0 },
	//{ "backstab" , POS_STANDING, do_backstab , 1, 0 },
	{ "ban"      , POS_DEAD    , do_ban      , LVL_GRGOD, 0 },
	{ "balance"  , POS_STANDING, do_not_here , 1, 0 },
	{ "breach"     , POS_STANDING, do_breach     , 1, 0 },
	{ "thermite"     , POS_STANDING, do_thermite     , 1, 0 },
	//{ "bash"     , POS_FIGHTING, do_bash     , 1, 0 },
	{ "beg"      , POS_RESTING , do_action   , 0, 0 },
	{ "bleed"    , POS_RESTING , do_action   , 0, 0 },
	{ "blush"    , POS_RESTING , do_action   , 0, 0 },
	{ "bow"      , POS_STANDING, do_action   , 0, 0 },
	{ "brb"      , POS_RESTING , do_action   , 0, 0 },
	{ "brief"    , POS_DEAD    , do_gen_tog  , 0, SCMD_BRIEF },
	{ "burp"     , POS_RESTING , do_action   , 0, 0 },
	{ "buy"      , POS_STANDING, do_buy      , 0, 0 },
	{ "bug"      , POS_DEAD    , do_gen_write, 0, SCMD_BUG },

	//{ "cast"     , POS_SITTING , do_cast     , 1, 0 },
	{ "cackle"   , POS_RESTING , do_action   , 0, 0 },
	{ "check"    , POS_STANDING, do_not_here , 1, 0 },
	{ "chuckle"  , POS_RESTING , do_action   , 0, 0 },
	{ "clap"     , POS_RESTING , do_action   , 0, 0 },
	{ "clear"    , POS_DEAD    , do_gen_ps   , 0, SCMD_CLEAR },
	{ "close"    , POS_SITTING , do_gen_door , 0, SCMD_CLOSE },
	{ "cls"      , POS_DEAD    , do_gen_ps   , 0, SCMD_CLEAR },
	{ "consider" , POS_RESTING , do_consider , 0, 0 },
	{ "color"    , POS_DEAD    , do_color    , 0, 0 },
	{ "comfort"  , POS_RESTING , do_action   , 0, 0 },
	{ "comb"     , POS_RESTING , do_action   , 0, 0 },
	{ "commands" , POS_DEAD    , do_commands , 0, SCMD_COMMANDS },
	{ "compact"  , POS_DEAD    , do_gen_tog  , 0, SCMD_COMPACT },
	{ "cough"    , POS_RESTING , do_action   , 0, 0 },
	{ "credits"  , POS_DEAD    , do_gen_ps   , 0, SCMD_CREDITS },
	{ "cringe"   , POS_RESTING , do_action   , 0, 0 },
	{ "cry"      , POS_RESTING , do_action   , 0, 0 },
	{ "cuddle"   , POS_RESTING , do_action   , 0, 0 },
	{ "curse"    , POS_RESTING , do_action   , 0, 0 },
	{ "curtsey"  , POS_STANDING, do_action   , 0, 0 },

	{ "dance"    , POS_STANDING, do_action   , 0, 0 },
	{ "date"     , POS_DEAD    , do_date     , LVL_IMMORT, SCMD_DATE },
	{ "daydream" , POS_SLEEPING, do_action   , 0, 0 },
	{ "dc"       , POS_DEAD    , do_dc       , LVL_GOD, 0 },
	{ "deposit"  , POS_STANDING, do_not_here , 1, 0 },
	{ "diagnose" , POS_RESTING , do_diagnose , 0, 0 },
	{ "display"  , POS_DEAD    , do_display  , 0, 0 },
	{ "donate"   , POS_RESTING , do_drop     , 0, SCMD_DONATE },
	{ "drink"    , POS_RESTING , do_drink    , 0, SCMD_DRINK },
	{ "drop"     , POS_RESTING , do_drop     , 0, SCMD_DROP },
	{ "drool"    , POS_RESTING , do_action   , 0, 0 },

	{ "eat"      , POS_RESTING , do_eat      , 0, SCMD_EAT },
	{ "echo"     , POS_SLEEPING, do_echo     , LVL_IMMORT, SCMD_ECHO },
	{ "emote"    , POS_RESTING , do_echo     , 1, SCMD_EMOTE },
	{ ":"        , POS_RESTING, do_echo      , 1, SCMD_EMOTE },
	{ "embrace"  , POS_STANDING, do_action   , 0, 0 },
	{ "enter"    , POS_STANDING, do_enter    , 0, 0 },
	{ "equipment", POS_SLEEPING, do_equipment, 0, 0 },
	{ "exits"    , POS_RESTING , do_exits    , 0, 0 },
	{ "examine"  , POS_SITTING , do_examine  , 0, 0 },

	{ "force"    , POS_SLEEPING, do_force    , LVL_GOD, 0 },
	{ "fart"     , POS_RESTING , do_action   , 0, 0 },
	{ "fill"     , POS_STANDING, do_pour     , 0, SCMD_FILL },
	{ "flee"     , POS_FIGHTING, do_flee     , 1, 0 },
	{ "flip"     , POS_STANDING, do_action   , 0, 0 },
	{ "flirt"    , POS_RESTING , do_action   , 0, 0 },
	{ "follow"   , POS_RESTING , do_follow   , 0, 0 },
	{ "fondle"   , POS_RESTING , do_action   , 0, 0 },
	{ "freeze"   , POS_DEAD    , do_wizutil  , LVL_FREEZE, SCMD_FREEZE },
	{ "french"   , POS_RESTING , do_action   , 0, 0 },
	{ "frown"    , POS_RESTING , do_action   , 0, 0 },
	{ "fume"     , POS_RESTING , do_action   , 0, 0 },

	{ "get"      , POS_RESTING , do_get      , 0, 0 },
	{ "gasp"     , POS_RESTING , do_action   , 0, 0 },
	{ "gecho"    , POS_DEAD    , do_gecho    , LVL_GOD, 0 },
	{ "give"     , POS_RESTING , do_give     , 0, 0 },
	{ "giggle"   , POS_RESTING , do_action   , 0, 0 },
	{ "glare"    , POS_RESTING , do_action   , 0, 0 },
	{ "goto"     , POS_SLEEPING, do_goto     , LVL_IMMORT, 0 },
	{ "gold"     , POS_RESTING , do_gold     , 0, 0 },
	{ "gossip"   , POS_SLEEPING, do_gen_comm , 0, SCMD_GOSSIP },
	{ "group"    , POS_RESTING , do_group    , 1, 0 },
	{ "grab"     , POS_RESTING , do_grab     , 0, 0 },
	{ "grats"    , POS_SLEEPING, do_gen_comm , 0, SCMD_GRATZ },
	{ "greet"    , POS_RESTING , do_action   , 0, 0 },
	{ "grin"     , POS_RESTING , do_action   , 0, 0 },
	{ "groan"    , POS_RESTING , do_action   , 0, 0 },
	{ "grope"    , POS_RESTING , do_action   , 0, 0 },
	{ "grovel"   , POS_RESTING , do_action   , 0, 0 },
	{ "growl"    , POS_RESTING , do_action   , 0, 0 },
	{ "gsay"     , POS_SLEEPING, do_gsay     , 0, 0 },
	{ "gtell"    , POS_SLEEPING, do_gsay     , 0, 0 },

	{ "help"     , POS_DEAD    , do_help     , 0, 0 },
	{ "handbook" , POS_DEAD    , do_gen_ps   , LVL_IMMORT, SCMD_HANDBOOK },
	{ "hcontrol" , POS_DEAD    , do_hcontrol , LVL_GRGOD, 0 },
	{ "hiccup"   , POS_RESTING , do_action   , 0, 0 },
	{ "hide"     , POS_RESTING , do_hide     , 1, 0 },
	{ "hit"      , POS_FIGHTING, do_hit      , 0, SCMD_HIT },
	{ "hold"     , POS_RESTING , do_grab     , 1, 0 },
	{ "holler"   , POS_RESTING , do_gen_comm , 1, SCMD_HOLLER },
	{ "holylight", POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_HOLYLIGHT },
	{ "hop"      , POS_RESTING , do_action   , 0, 0 },
	{ "house"    , POS_RESTING , do_house    , 0, 0 },
	{ "hug"      , POS_RESTING , do_action   , 0, 0 },

	{ "inventory", POS_DEAD    , do_inventory, 0, 0 },
	{ "idea"     , POS_DEAD    , do_gen_write, 0, SCMD_IDEA },
	{ "imotd"    , POS_DEAD    , do_gen_ps   , LVL_IMMORT, SCMD_IMOTD },
	{ "immlist"  , POS_DEAD    , do_gen_ps   , 0, SCMD_IMMLIST },
	{ "info"     , POS_SLEEPING, do_gen_ps   , 0, SCMD_INFO },
	{ "insult"   , POS_RESTING , do_insult   , 0, 0 },
	{ "invis"    , POS_DEAD    , do_invis    , LVL_IMMORT, 0 },

	{ "junk"     , POS_RESTING , do_drop     , 0, SCMD_JUNK },

	{ "kill"     , POS_FIGHTING, do_kill     , 0, 0 },
	//{ "kick"     , POS_FIGHTING, do_kick     , 1, 0 },
	{ "kiss"     , POS_RESTING , do_action   , 0, 0 },

	{ "look"     , POS_RESTING , do_look     , 0, SCMD_LOOK },
	{ "view"     , POS_RESTING , do_view     , 0, 0 },
	{ "laugh"    , POS_RESTING , do_action   , 0, 0 },
	{ "last"     , POS_DEAD    , do_last     , LVL_GOD, 0 },
	{ "leave"    , POS_STANDING, do_leave    , 0, 0 },
	{ "levels"   , POS_DEAD    , do_levels   , 0, 0 },
	{ "list"     , POS_STANDING, do_list , 0, 0 },
	{ "lick"     , POS_RESTING , do_action   , 0, 0 },
	{ "lock"     , POS_SITTING , do_gen_door , 0, SCMD_LOCK },
	{ "load"     , POS_DEAD    , do_load     , LVL_GOD, 0 },
	{ "love"     , POS_RESTING , do_action   , 0, 0 },

	{ "moan"     , POS_RESTING , do_action   , 0, 0 },
	{ "motd"     , POS_DEAD    , do_gen_ps   , 0, SCMD_MOTD },
	{ "mail"     , POS_STANDING, do_not_here , 1, 0 },
	{ "massage"  , POS_RESTING , do_action   , 0, 0 },
	{ "mute"     , POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_SQUELCH },
	//{ "murder"   , POS_FIGHTING, do_hit      , 0, SCMD_MURDER },

	{ "news"     , POS_SLEEPING, do_gen_ps   , 0, SCMD_NEWS },
	{ "nibble"   , POS_RESTING , do_action   , 0, 0 },
	{ "nod"      , POS_RESTING , do_action   , 0, 0 },
	{ "noauction", POS_DEAD    , do_gen_tog  , 0, SCMD_NOAUCTION },
	{ "nogossip" , POS_DEAD    , do_gen_tog  , 0, SCMD_NOGOSSIP },
	{ "nograts"  , POS_DEAD    , do_gen_tog  , 0, SCMD_NOGRATZ },
	{ "nohassle" , POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_NOHASSLE },
	{ "norepeat" , POS_DEAD    , do_gen_tog  , 0, SCMD_NOREPEAT },
	{ "noshout"  , POS_SLEEPING, do_gen_tog  , 1, SCMD_DEAF },
	{ "nosummon" , POS_DEAD    , do_gen_tog  , 1, SCMD_NOSUMMON },
	{ "notell"   , POS_DEAD    , do_gen_tog  , 1, SCMD_NOTELL },
	{ "notitle"  , POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_NOTITLE },
	{ "nowiz"    , POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_NOWIZ },
	{ "nudge"    , POS_RESTING , do_action   , 0, 0 },
	{ "nuzzle"   , POS_RESTING , do_action   , 0, 0 },

	{ "olc"      , POS_DEAD    , do_olc      , LVL_IMPL, 0 },
	{ "order"    , POS_RESTING , do_order    , 1, 0 },
	{ "offer"    , POS_STANDING, do_not_here , 1, 0 },
	{ "open"     , POS_SITTING , do_gen_door , 0, SCMD_OPEN },

	{ "put"      , POS_RESTING , do_put      , 0, 0 },
	{ "pat"      , POS_RESTING , do_action   , 0, 0 },
	//{ "page"     , POS_DEAD    , do_page     , LVL_GOD, 0 },
	{ "pardon"   , POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_PARDON },
	{ "peer"     , POS_RESTING , do_action   , 0, 0 },
	{ "pick"     , POS_STANDING, do_gen_door , 1, SCMD_PICK },
	{ "point"    , POS_RESTING , do_action   , 0, 0 },
	{ "poke"     , POS_RESTING , do_action   , 0, 0 },
	{ "policy"   , POS_DEAD    , do_gen_ps   , 0, SCMD_POLICIES },
	{ "ponder"   , POS_RESTING , do_action   , 0, 0 },
	{ "poofin"   , POS_DEAD    , do_poofset  , LVL_IMMORT, SCMD_POOFIN },
	{ "poofout"  , POS_DEAD    , do_poofset  , LVL_IMMORT, SCMD_POOFOUT },
	{ "pour"     , POS_STANDING, do_pour     , 0, SCMD_POUR },
	{ "pout"     , POS_RESTING , do_action   , 0, 0 },
	{ "prompt"   , POS_DEAD    , do_display  , 0, 0 },
	{ "practice" , POS_RESTING , do_practice , 1, 0 },
	{ "pray"     , POS_SITTING , do_action   , 0, 0 },
	{ "puke"     , POS_RESTING , do_action   , 0, 0 },
	{ "punch"    , POS_RESTING , do_action   , 0, 0 },
	{ "purr"     , POS_RESTING , do_action   , 0, 0 },
	{ "purge"    , POS_DEAD    , do_purge    , LVL_GOD, 0 },

	{ "quaff"    , POS_RESTING , do_use      , 0, SCMD_QUAFF },
	{ "qecho"    , POS_DEAD    , do_qcomm    , LVL_IMMORT, SCMD_QECHO },
	//{ "quest"    , POS_DEAD    , do_gen_tog  , 0, SCMD_QUEST },
	{ "qui"      , POS_DEAD    , do_quit     , 0, 0 },
	{ "quit"     , POS_DEAD    , do_quit     , 0, SCMD_QUIT },
	{ "qsay"     , POS_RESTING , do_qcomm    , 0, SCMD_QSAY },

	{ "reply"    , POS_SLEEPING, do_reply    , 0, 0 },
	{ "rest"     , POS_RESTING , do_rest     , 0, 0 },
	{ "read"     , POS_RESTING , do_look     , 0, SCMD_READ },
	{ "reboot"   , POS_DEAD    , do_reboot   , LVL_IMPL, 0 },
	{ "reload"  , POS_RESTING , do_reload   , 0, 0 },
	{ "recite"   , POS_RESTING , do_use      , 0, SCMD_RECITE },
	{ "receive"  , POS_STANDING, do_not_here , 1, 0 },
	{ "remove"   , POS_RESTING , do_remove   , 0, 0 },
	{ "rent"     , POS_STANDING, do_not_here , 1, 0 },
	{ "report"   , POS_RESTING , do_report   , 0, 0 },
	{ "reroll"   , POS_DEAD    , do_wizutil  , LVL_GRGOD, SCMD_REROLL },
	{ "rescue"   , POS_FIGHTING, do_rescue   , 1, 0 },
	{ "restore"  , POS_DEAD    , do_restore  , LVL_GOD, 0 },
	{ "return"   , POS_DEAD    , do_return   , 0, 0 },
	//{ "rnum"    , POS_STANDING, do_rnum     , 0, 0 },
	{ "roll"     , POS_RESTING , do_action   , 0, 0 },
	{ "roomflags", POS_DEAD    , do_gen_tog  , LVL_IMMORT, SCMD_ROOMFLAGS },
	{ "ruffle"   , POS_STANDING, do_action   , 0, 0 },

	{ "say"      , POS_RESTING , do_say      , 0, 0 },
	{ "'"        , POS_RESTING , do_say      , 0, 0 },
	{ "save"     , POS_SLEEPING, do_save     , 0, 0 },
	{ "score"    , POS_DEAD    , do_score    , 0, 0 },
	{ "scream"   , POS_RESTING , do_action   , 0, 0 },
	{ "sell"     , POS_STANDING, do_not_here , 0, 0 },
	{ "send"     , POS_SLEEPING, do_send     , LVL_GOD, 0 },
	{ "set"      , POS_DEAD    , do_set      , LVL_GOD, 0 },
	{ "shout"    , POS_RESTING , do_gen_comm , 0, SCMD_SHOUT },
	{ "shake"    , POS_RESTING , do_action   , 0, 0 },
	{ "shiver"   , POS_RESTING , do_action   , 0, 0 },
	{ "show"     , POS_DEAD    , do_show     , LVL_IMMORT, 0 },
	{ "shrug"    , POS_RESTING , do_action   , 0, 0 },
	{ "shutdow"  , POS_DEAD    , do_shutdown , LVL_IMPL, 0 },
	{ "scan" 		, POS_RESTING    , do_scan , 0, 0 },
	{ "shutdown" , POS_RESTING   , do_shutdown , 0, SCMD_SHUTDOWN },
	{ "sigh"     , POS_RESTING , do_action   , 0, 0 },
	{ "sing"     , POS_RESTING , do_action   , 0, 0 },
	{ "sip"      , POS_RESTING , do_drink    , 0, SCMD_SIP },
	{ "sit"      , POS_RESTING , do_sit      , 0, 0 },
	{ "skillset" , POS_SLEEPING, do_skillset , LVL_GRGOD, 0 },
	{ "sleep"    , POS_SLEEPING, do_sleep    , 0, 0 },
	{ "slap"     , POS_RESTING , do_action   , 0, 0 },
	{ "slowns"   , POS_DEAD    , do_gen_tog  , LVL_IMPL, SCMD_SLOWNS },
	{ "smile"    , POS_RESTING , do_action   , 0, 0 },
	{ "smirk"    , POS_RESTING , do_action   , 0, 0 },
	{ "snicker"  , POS_RESTING , do_action   , 0, 0 },
	/** mods */
	{ "js"  , POS_RESTING , do_js   , 0, 0 },
	/** ------------------- */
	/** DEBUGGING + TESTING */
	/** ------------------- */
	{ "givemegold"  , POS_RESTING , do_givemegold   , 0, 0 },
	{ "givemenades" , POS_RESTING , do_givemenades , 0, 0 },
	{ "idle"  , POS_RESTING , do_idle   , 0, 0 },
	{ "heal"  , POS_RESTING , do_heal   , 0, 0 },
	{ "newjs"  , POS_RESTING , do_newjs   , LVL_GOD, 0 },
	{ "jstest"  , POS_RESTING , do_jstest   , LVL_GOD, 0 },
	{ "point_update"  , POS_RESTING , do_point_update   , 0, 0 },
	{ "zero_socket"  , POS_RESTING , do_zero_socket  , 0, 0 },
	{ "uuid"  , POS_RESTING , do_uuid  , 0, 0 },
	/** ----------------------- */
	/** END DEBUGGING + TESTING */
	/** ----------------------- */

	/** ------------- */
	/** INFORMATIONAL */
	/** ------------- */
		/** ------ */
		/** QUESTS */
		/** ------ */
	{ "quest"  , POS_RESTING , do_quest   , 0, 0 },
		/** ------- */
		/** DISPLAY */
		/** ------- */
	{ "automap"  , POS_RESTING , do_gen_tog   , 0, SCMD_AUTOMAP },
	/** ----------------- */
	/** END INFORMATIONAL */
	/** ----------------- */

	/** -------- */
	/** MOVEMENT */
	/** -------- */
	{ "recall"  , POS_RESTING , do_recall   , 0, 0 },

	/** ---------------- */
	/** COMBAT MECHANICS */
	/** ---------------- */
	{ "snipe"  , POS_RESTING , do_snipe   , 0, 0 },
	/** install/uninstall various gadgets */
	{ "cancel"  , POS_RESTING , do_cancel, 0, 0 },	/** i.e.: claymore mine, camera */
	{ "install"  , POS_RESTING , do_install, 0, 0 },	/** i.e.: claymore mine, camera */
	{ "uninstall"  , POS_RESTING , do_uninstall, 0, 0 },	/** i.e.: claymore mine, camera */
	//TODO code me{ "plant" , POS_RESTING , do_plant , 0, 0},
	//TODO code me { "activate" , POS_RESTING , do_activate , 0, 0},
	{ "drone"  , POS_RESTING , do_drone   , 0, 0 },
	{ "throw"  , POS_RESTING , do_throw   , 0, 0 },
	//{ "ammo"  , POS_RESTING , do_ammo   , 0, 0 },
	/** -------------------------- */
	/** "GIVE ME" COMBAT MECHANICS */
	/** -------------------------- */
	{ "giveme_sniper_rifle"  , POS_RESTING , do_giveme_sniper_rifle   , LVL_GOD, 0 },
	{ "giveme_frag"  , POS_RESTING , do_giveme_frag_grenades  , LVL_GOD, 0 },
	{ "giveme_flash"  , POS_RESTING , do_giveme_flashbang_grenades  , LVL_GOD, 0 },
	{ "giveme_inc"  , POS_RESTING , do_giveme_incendiary_grenades , LVL_GOD, 0 },
	{ "giveme_smoke"  , POS_RESTING , do_giveme_smoke_grenades  , LVL_GOD, 0 },
	{ "giveme_sensor"  , POS_RESTING , do_giveme_sensor_grenades  , LVL_GOD, 0 },
	{ "giveme_emp"  , POS_RESTING , do_giveme_emp_grenades  , LVL_GOD, 0 },
	{ "affect_me"  , POS_RESTING , do_affect_me , LVL_GOD, 0 },

	/** ------------------------------ */
	/** END "GIVE ME" COMBAT MECHANICS */
	/** ------------------------------ */
	/** --------------------- */
	/** END COMBAT MECHANICS  */
	/** --------------------- */

	/** ------------- */
	/** BUILDER UTILS */
	/** ------------- */
	{ "require_js"  , POS_RESTING , do_require_js   , 0, 0 },
	{ "builder"  , POS_RESTING , do_builder   , 0, 0 },
	{ "builder_help"  , POS_RESTING , do_builder_help   , LVL_GOD, 0 },
	{ "flush_holding"  , POS_RESTING , do_flush_holding   , LVL_GOD, 0 },
	{ "hold_anything"  , POS_RESTING , do_hold_anything   , LVL_GOD, 0 },
	{ "yaml_import"  , POS_RESTING , do_yaml_import   , LVL_GOD, 0 },
	{ "yaml_example"  , POS_RESTING , do_yaml_example   , LVL_GOD, 0 },
	{ "next_object_number"  , POS_RESTING , do_next_object_number   , LVL_GOD, 0 },
	{ "next_room_number"  , POS_RESTING , do_next_room_number   , LVL_GOD, 0 },
	{ "next_mob_number"  , POS_RESTING , do_next_mob_number   , LVL_GOD, 0 },
	{ "pmw_obj_from_room"  , POS_RESTING , do_pmw_obj_from_room   , LVL_GOD, 0 },
	{ "toggle_obj_from_room"  , POS_RESTING , do_toggle_obj_from_room   , LVL_GOD, 0 },
	{ "js_help"  , POS_RESTING , do_js_help   , LVL_GOD, 0 },
	{ "mbuild"  , POS_RESTING , do_mbuild   , LVL_GOD, 0 },
	{ "obuild"  , POS_RESTING , do_obuild   , LVL_GOD, 0 },
	{ "sbuild"  , POS_RESTING , do_sbuild   , LVL_GOD, 0 },
	{ "zbuild"  , POS_RESTING , do_zbuild   , LVL_IMMORT, 0 },
	{ "chanmgr"  , POS_RESTING , do_chanmgr   , LVL_IMMORT, 0 },
	{ "rnumtele"  , POS_RESTING , do_rnumtele   , LVL_IMMORT, 0 },
	{ "rnumlist"  , POS_RESTING , do_rnumlist   , LVL_IMMORT, 0 },
	{ "pref"  , POS_RESTING , do_pref   , 0, 0 },
	{ "rbuild"  , POS_RESTING , do_rbuild   , LVL_IMMORT, 0 },
	{ "rbuild_sandbox"  , POS_RESTING , do_rbuild_sandbox   , LVL_IMMORT, 0 },
	{ "room_list"  , POS_RESTING , do_room_list   , LVL_IMMORT, 0 },
	{ "histfile"  , POS_RESTING , do_histfile   , LVL_IMMORT, 0 },
	{ "my_uuid"  , POS_RESTING , do_my_uuid  , LVL_IMMORT, 0 },
	/** ----------------- */
	/** END BUILDER UTILS */
	/** ----------------- */

	/** ------------------ */
	/** ADMIN/WIZARD UTILS */
	/** ------------------ */
	{ "wiz_quote"  , POS_RESTING , do_wiz_quote  , LVL_GOD, 0 },
	/** ---------------------- */
	/** END ADMIN/WIZARD UTILS */
	/** ---------------------- */

	{ "snap"     , POS_RESTING , do_action   , 0, 0 },
	{ "snarl"    , POS_RESTING , do_action   , 0, 0 },
	{ "sneeze"   , POS_RESTING , do_action   , 0, 0 },
	{ "sneak"    , POS_STANDING, do_sneak    , 1, 0 },
	{ "sniff"    , POS_RESTING , do_action   , 0, 0 },
	{ "snore"    , POS_SLEEPING, do_action   , 0, 0 },
	{ "snowball" , POS_STANDING, do_action   , LVL_IMMORT, 0 },
	{ "snoop"    , POS_DEAD    , do_snoop    , LVL_GOD, 0 },
	{ "snuggle"  , POS_RESTING , do_action   , 0, 0 },
	{ "socials"  , POS_DEAD    , do_commands , 0, SCMD_SOCIALS },
	{ "split"    , POS_SITTING , do_split    , 1, 0 },
	{ "spank"    , POS_RESTING , do_action   , 0, 0 },
	{ "spit"     , POS_STANDING, do_action   , 0, 0 },
	{ "squeeze"  , POS_RESTING , do_action   , 0, 0 },
	{ "stand"    , POS_RESTING , do_stand    , 0, 0 },
	{ "stare"    , POS_RESTING , do_action   , 0, 0 },
	{ "stat"     , POS_DEAD    , do_stat     , LVL_IMMORT, 0 },
	{ "steal"    , POS_STANDING, do_steal    , 1, 0 },
	{ "steam"    , POS_RESTING , do_action   , 0, 0 },
	{ "stroke"   , POS_RESTING , do_action   , 0, 0 },
	{ "strut"    , POS_STANDING, do_action   , 0, 0 },
	{ "sulk"     , POS_RESTING , do_action   , 0, 0 },
	{ "switch"   , POS_DEAD    , do_switch   , LVL_GRGOD, 0 },
	{ "syslog"   , POS_DEAD    , do_syslog   , LVL_IMMORT, 0 },

	{ "tell"     , POS_DEAD    , do_tell     , 0, 0 },
	{ "tackle"   , POS_RESTING , do_action   , 0, 0 },
	{ "take"     , POS_RESTING , do_get      , 0, 0 },
	{ "tango"    , POS_STANDING, do_action   , 0, 0 },
	{ "taunt"    , POS_RESTING , do_action   , 0, 0 },
	{ "taste"    , POS_RESTING , do_eat      , 0, SCMD_TASTE },
	{ "teleport" , POS_DEAD    , do_teleport , LVL_GOD, 0 },
	{ "thank"    , POS_RESTING , do_action   , 0, 0 },
	{ "think"    , POS_RESTING , do_action   , 0, 0 },
	{ "thaw"     , POS_DEAD    , do_wizutil  , LVL_FREEZE, SCMD_THAW },
	{ "title"    , POS_DEAD    , do_title    , 0, 0 },
	{ "tickle"   , POS_RESTING , do_action   , 0, 0 },
	{ "time"     , POS_DEAD    , do_time     , 0, 0 },
	{ "toggle"   , POS_DEAD    , do_toggle   , 0, 0 },
	{ "track"    , POS_STANDING, do_track    , 0, 0 },
	{ "trackthru", POS_DEAD    , do_gen_tog  , LVL_IMPL, SCMD_TRACK },
	{ "transfer" , POS_SLEEPING, do_trans    , LVL_GOD, 0 },
	{ "twiddle"  , POS_RESTING , do_action   , 0, 0 },
	{ "typo"     , POS_DEAD    , do_gen_write, 0, SCMD_TYPO },

	{ "unlock"   , POS_SITTING , do_gen_door , 0, SCMD_UNLOCK },
	{ "ungroup"  , POS_DEAD    , do_ungroup  , 0, 0 },
	{ "unban"    , POS_DEAD    , do_unban    , LVL_GRGOD, 0 },
	{ "unaffect" , POS_DEAD    , do_wizutil  , LVL_GOD, SCMD_UNAFFECT },
	{ "uptime"   , POS_DEAD    , do_date     , LVL_IMMORT, SCMD_UPTIME },
	{ "use"      , POS_SITTING , do_use      , 1, SCMD_USE },
	{ "users"    , POS_DEAD    , do_users    , LVL_IMMORT, 0 },

	{ "value"    , POS_STANDING, do_not_here , 0, 0 },
	{ "version"  , POS_DEAD    , do_gen_ps   , 0, SCMD_VERSION },
	{ "visible"  , POS_RESTING , do_visible  , 1, 0 },
	{ "vnum"     , POS_DEAD    , do_vnum     , LVL_IMMORT, 0 },
	{ "vstat"    , POS_DEAD    , do_vstat    , LVL_IMMORT, 0 },

	{ "wake"     , POS_SLEEPING, do_wake     , 0, 0 },
	{ "wave"     , POS_RESTING , do_action   , 0, 0 },
	{ "wear"     , POS_RESTING , do_wear     , 0, 0 },
	{ "weather"  , POS_RESTING , do_weather  , 0, 0 },
	{ "who"      , POS_DEAD    , do_who      , 0, 0 },
	{ "whoami"   , POS_DEAD    , do_gen_ps   , 0, SCMD_WHOAMI },
	{ "where"    , POS_RESTING , do_where    , 1, 0 },
	{ "whisper"  , POS_RESTING , do_spec_comm, 0, SCMD_WHISPER },
	{ "whine"    , POS_RESTING , do_action   , 0, 0 },
	{ "whistle"  , POS_RESTING , do_action   , 0, 0 },
	{ "wield"    , POS_RESTING , do_wield    , 0, 0 },
	{ "wiggle"   , POS_STANDING, do_action   , 0, 0 },
	{ "wimpy"    , POS_DEAD    , do_wimpy    , 0, 0 },
	{ "wink"     , POS_RESTING , do_action   , 0, 0 },
	{ "withdraw" , POS_STANDING, do_not_here , 1, 0 },
	{ "wiznet"   , POS_DEAD    , do_wiznet   , LVL_IMMORT, 0 },
	{ ";"        , POS_DEAD    , do_wiznet   , LVL_IMMORT, 0 },
	{ "wizhelp"  , POS_SLEEPING, do_commands , LVL_IMMORT, SCMD_WIZHELP },
	{ "wizlist"  , POS_DEAD    , do_gen_ps   , 0, SCMD_WIZLIST },
	{ "wizlock"  , POS_DEAD    , do_wizlock  , LVL_IMPL, 0 },
	{ "worship"  , POS_RESTING , do_action   , 0, 0 },
	{ "write"    , POS_STANDING, do_write    , 1, 0 },

	{ "yawn"     , POS_RESTING , do_action   , 0, 0 },
	{ "yodel"    , POS_RESTING , do_action   , 0, 0 },

	{ "zreset"   , POS_DEAD    , do_zreset   , LVL_GRGOD, 0 },

	{ "\n", 0, 0, 0, 0 }
};	/* this must be last */


const char *fill[] = {
	"in",
	"from",
	"with",
	"the",
	"on",
	"at",
	"to",
	"\n"
};

const char *reserved[] = {
	"a",
	"an",
	"self",
	"me",
	"all",
	"room",
	"someone",
	"something",
	"\n"
};

/*
 * This is the actual command interpreter called from game_loop() in comm.c
 * It makes sure you are the proper level and position to execute the command,
 * then calls the appropriate function.
 */
/*
void command_interpreter(char_data *ch, char *argument, bool legacy) {
	auto p = ptr(ch);
	command_interpreter(p,argument);
}
void command_interpreter(char_data *ch, const char *argument, bool legacy) {
	auto p = ptr(ch);
	command_interpreter(p,argument);
}
*/
void command_interpreter(player_ptr_t & player, std::string_view in_argument){
	auto ch = player->cd();
	int cmd, length;
	char *line;
	char arg[MAX_INPUT_LENGTH];

	REMOVE_BIT(AFF_FLAGS(ch), AFF_HIDE);

	std::vector<char> data;
	std::size_t size = in_argument.length();
	if(MAX_INPUT_LENGTH - 1 <= size){
		size = MAX_INPUT_LENGTH - 1;
	}
	char* argument = nullptr;
	if(size == 0){
		return;
	}
	data.resize(size+1);
	std::copy(in_argument.begin(),in_argument.begin() + size,data.begin());
	data[size] = '\0';
	argument = &data[0];

	/* just drop to next line for hitting CR */
	skip_spaces(&argument);

	if(!mods::globals::command_interpreter(player,argument)) {
		mods::globals::post_command_interpreter(ch,argument);
		return;
	}
	line = any_one_arg(argument, arg);

	/* otherwise, find the command */
	for(length = strlen(arg), cmd = 0; *cmd_info[cmd].command != '\n'; cmd++){
		if(!strncmp(cmd_info[cmd].command, arg, length)){
			if(player->god_mode()){
				break;
			}
			if(GET_LEVEL(ch) >= cmd_info[cmd].minimum_level) {
				break;
			}
		}
	}

	if(*cmd_info[cmd].command == '\n') {
		send_to_char(ch, "<cmd_interpretr>Huh?!?\r\n");
		//TODO: change PLR_FLAGGED call to player->member method call
	} else{
		if(player->god_mode()){
			assert(ch != nullptr);
			//d("line: '" << line << "' cmd:'" << cmd_info[cmd].command << "' cmd_info[cmd].subcmd: '" << cmd_info[cmd].subcmd << "'");
			if(player->is_blocked() && !mods::player_utils::is_cancel_command(cmd_info[cmd].command)) {
				player->sendln("You can't! You're currently doing something!\r\n");
			} else {
				((*cmd_info[cmd].command_pointer)(ch, line, cmd, cmd_info[cmd].subcmd, player));
				std::cerr << "after cm ptr->\n";
			}
			std::cerr << "post command interpreter pre...\n";
			mods::globals::post_command_interpreter(ch,argument);
			std::cerr << "post command interpreter post...\n";
			return;
		}
		if(!IS_NPC(ch) && PLR_FLAGGED(ch, PLR_FROZEN) && GET_LEVEL(ch) < LVL_IMPL) {
			send_to_char(ch, "You try, but the mind-numbing cold prevents you...\r\n");
		} else if(cmd_info[cmd].command_pointer == NULL) {
			send_to_char(ch, "Sorry, that command hasn't been implemented yet.\r\n");
		} else if(IS_NPC(ch) && cmd_info[cmd].minimum_level >= LVL_IMMORT) {
			send_to_char(ch, "You can't use immortal commands while switched.\r\n");
		} else if(GET_POS(ch) < cmd_info[cmd].minimum_position){
			switch(GET_POS(ch)) {
				case POS_DEAD:
					send_to_char(ch, "Lie still; you are DEAD!!! :-(\r\n");
					break;

				case POS_INCAP:
				case POS_MORTALLYW:
					send_to_char(ch, "You are in a pretty bad shape, unable to do anything!\r\n");
					break;

				case POS_STUNNED:
					send_to_char(ch, "All you can do right now is think about the stars!\r\n");
					break;

				case POS_SLEEPING:
					send_to_char(ch, "In your dreams, or what?\r\n");
					break;

				case POS_RESTING:
					send_to_char(ch, "Nah... You feel too relaxed to do that..\r\n");
					break;

				case POS_SITTING:
					send_to_char(ch, "Maybe you should get on your feet first?\r\n");
					break;

				case POS_FIGHTING:
					send_to_char(ch, "No way!  You're fighting for your life!\r\n");
					break;
			}
		}
		else if(no_specials || !special(ch, cmd, line)) {
			d("line: '" << line << "' cmd:'" << cmd_info[cmd].command << "' cmd_info[cmd].subcmd: '" << cmd_info[cmd].subcmd << "'");
			if(player->is_blocked() && !mods::player_utils::is_cancel_command(cmd_info[cmd].command)) {
				player->sendln("You can't! You're currently doing something!\r\n");
			} else {
				((*cmd_info[cmd].command_pointer)(ch, line, cmd, cmd_info[cmd].subcmd,player));
			}
		}
	}

	mods::globals::post_command_interpreter(ch,argument);
}

/**************************************************************************
 * Routines to handle aliasing                                             *
 **************************************************************************/


struct alias_data *find_alias(struct alias_data *alias_list, char *str) {
	while(alias_list != NULL) {
		if(*str == *alias_list->alias)	/* hey, every little bit counts :-) */
			if(!strcmp(str, alias_list->alias)) {
				return (alias_list);
			}

		alias_list = alias_list->next;
	}

	return (NULL);
}


void free_alias(struct alias_data *a) {
	/*
		 if(a->alias) {
		 free(a->alias);
		 }

		 if(a->replacement) {
		 free(a->replacement);
		 }

		 free(a);
		 */
}


/* The interface to the outside world: do_alias */
ACMD(do_alias) {
	char arg[MAX_INPUT_LENGTH];
	char *repl;
	struct alias_data *a, *temp;

	if(IS_NPC(ch)) {
		return;
	}

	repl = any_one_arg(argument, arg);

	if(!*arg) {			/* no argument specified -- list currently defined aliases */
		send_to_char(ch, "Currently defined aliases:\r\n");

		if((a = GET_ALIASES(ch)) == NULL) {
			send_to_char(ch, " None.\r\n");
		} else {
			while(a != NULL) {
				send_to_char(ch, "%-15s %s\r\n", a->alias, a->replacement);
				a = a->next;
			}
		}
	} else {			/* otherwise, add or remove aliases */
		/* is this an alias we've already defined? */
		if((a = find_alias(GET_ALIASES(ch), arg)) != NULL) {
			REMOVE_FROM_LIST(a, GET_ALIASES(ch), next);
			free_alias(a);
		}

		/* if no replacement string is specified, assume we want to delete */
		if(!*repl) {
			if(a == NULL) {
				send_to_char(ch, "No such alias.\r\n");
			} else {
				send_to_char(ch, "Alias deleted.\r\n");
			}
		} else {			/* otherwise, either add or redefine an alias */
			if(!str_cmp(arg, "alias")) {
				send_to_char(ch, "You can't alias 'alias'.\r\n");
				return;
			}

			CREATE(a, struct alias_data, 1);
			a->alias = strdup(arg);
			delete_doubledollar(repl);
			a->replacement = strdup(repl);

			if(strchr(repl, ALIAS_SEP_CHAR) || strchr(repl, ALIAS_VAR_CHAR)) {
				a->type = ALIAS_COMPLEX;
			} else {
				a->type = ALIAS_SIMPLE;
			}

			a->next = GET_ALIASES(ch);
			GET_ALIASES(ch) = a;
			send_to_char(ch, "Alias added.\r\n");
		}
	}
}

/*
 * Valid numeric replacements are only $1 .. $9 (makes parsing a little
 * easier, and it's not that much of a limitation anyway.)  Also valid
 * is "$*", which stands for the entire original line after the alias.
 * ";" is used to delimit commands.
 */
#define NUM_TOKENS       9



/*
 * Given a character and a string, perform alias replacement on it.
 *
 * Return values:
 *   0: String was modified in place; call command_interpreter immediately.
 *   1: String was _not_ modified in place; rather, the expanded aliases
 *      have been placed at the front of the character's input queue.
 */
int perform_alias(mods::descriptor_data& d, char *orig, size_t maxlen) {
	char first_arg[MAX_INPUT_LENGTH], *ptr;
	struct alias_data *a, *tmp;

	/* Mobs don't have alaises. */
	if(IS_NPC(d.character)) {
		return (0);
	}

	/* bail out immediately if the guy doesn't have any aliases */
	if((tmp = GET_ALIASES(d.character)) == NULL) {
		return (0);
	}

	/* find the alias we're supposed to match */
	ptr = any_one_arg(orig, first_arg);

	/* bail out if it's null */
	if(!*first_arg) {
		return (0);
	}

	/* if the first arg is not an alias, return without doing anything */
	if((a = find_alias(tmp, first_arg)) == NULL) {
		return (0);
	}

	if(a->type == ALIAS_SIMPLE) {
		strlcpy(orig, a->replacement, maxlen);
		return (0);
	} else {
		//perform_complex_alias(d, ptr, a);
		std::list<txt_block> temp_queue;
		char *tokens[NUM_TOKENS], *temp, *write_point;
		char buf2[MAX_RAW_INPUT_LENGTH], buf[MAX_RAW_INPUT_LENGTH];	/* raw? */
		int num_of_tokens = 0, num;

		/* First, parse the original string */
		strcpy(buf2, ptr);	/* strcpy: OK (orig:MAX_INPUT_LENGTH < buf2:MAX_RAW_INPUT_LENGTH) */
		temp = strtok(buf2, " ");

		while(temp != NULL && num_of_tokens < NUM_TOKENS) {
			tokens[num_of_tokens++] = temp;
			temp = strtok(NULL, " ");
		}

		/* initialize */
		write_point = buf;
		//temp_queue.head = temp_queue.tail = NULL;

		/* now parse the alias */
		for(temp = a->replacement; *temp; temp++) {
			if(*temp == ALIAS_SEP_CHAR) {
				*write_point = '\0';
				buf[MAX_INPUT_LENGTH - 1] = '\0';
				//write_to_q(buf, &temp_queue, 1);
				temp_queue.emplace_back(buf,1);
				write_point = buf;
			} else if(*temp == ALIAS_VAR_CHAR) {
				temp++;

				if((num = *temp - '1') < num_of_tokens && num >= 0) {
					strcpy(write_point, tokens[num]);	/* strcpy: OK */
					write_point += strlen(tokens[num]);
				} else if(*temp == ALIAS_GLOB_CHAR) {
					strcpy(write_point, ptr);		/* strcpy: OK */
					write_point += strlen(ptr);
				} else if((*(write_point++) = *temp) == '$') {	/* redouble $ for act safety */
					*(write_point++) = '$';
				}
			} else {
				*(write_point++) = *temp;
			}
		}

		*write_point = '\0';
		buf[MAX_INPUT_LENGTH - 1] = '\0';
		//write_to_q(buf, &temp_queue, 1);
		temp_queue.emplace_back(buf,1);

		if(d.input.size() == 0){
			d.input = std::move(temp_queue);
		} else {
			std::copy(temp_queue.begin(),temp_queue.end(),d.input.end());
		}
		return 1;
	}
}



/***************************************************************************
 * Various other parsing utilities                                         *
 **************************************************************************/

/*
 * searches an array of strings for a target string.  "exact" can be
 * 0 or non-0, depending on whether or not the match must be exact for
 * it to be returned.  Returns -1 if not found; 0..n otherwise.  Array
 * must be terminated with a '\n' so it knows to stop searching.
 */
int search_block(char *arg, const char **list, int exact) { /** FIXME: arg 2  */
	int i, l;

	/*  We used to have \r as the first character on certain array items to
	 *  prevent the explicit choice of that point.  It seems a bit silly to
	 *  dump control characters into arrays to prevent that, so we'll just
	 *  check in here to see if the first character of the argument is '!',
	 *  and if so, just blindly return a '-1' for not found. - ae.
	 */
	if(*arg == '!') {
		return (-1);
	}

	/* Make into lower case, and get length of string */
	/** FIXME: mutating lowercase macro/func should be created */
	for(l = 0; *(arg + l); l++) {
		*(arg + l) = LOWER(*(arg + l));
	}

	if(exact) {
		for(i = 0; **(list + i) != '\n'; i++)
			if(!strcmp(arg, *(list + i))) {
				return (i);
			}
	} else {
		if(!l) {
			l = 1;
		}			/* Avoid "" to match the first available

					 * string */

		for(i = 0; **(list + i) != '\n'; i++)
			if(!strncmp(arg, *(list + i), l)) {
				return (i);
			}
	}

	return (-1);
}


int is_number(const char *str) {
	while(*str)
		if(!isdigit(*(str++))) {
			return (0);
		}

	return (1);
}

/*
 * Function to skip over the leading spaces of a string.
 */
void skip_spaces(char **string) {
	for(; **string && isspace(**string); (*string)++);
}


/*
 * Given a string, change all instances of double dollar signs ($$) to
 * single dollar signs ($).  When strings come in, all $'s are changed
 * to $$'s to avoid having users be able to crash the system if the
 * inputted string is eventually sent to act().  If you are using user
 * input to produce screen output AND YOU ARE SURE IT WILL NOT BE SENT
 * THROUGH THE act() FUNCTION (i.e., do_gecho, do_title, but NOT do_say),
 * you can call delete_doubledollar() to make the output look correct.
 *
 * Modifies the string in-place.
 */
char *delete_doubledollar(char *string) {
	char *ddread, *ddwrite;

	/* If the string has no dollar signs, return immediately */
	if((ddwrite = strchr(string, '$')) == NULL) {
		return (string);
	}

	/* Start from the location of the first dollar sign */
	ddread = ddwrite;


	while(*ddread)    /* Until we reach the end of the string... */
		if((*(ddwrite++) = *(ddread++)) == '$')  /* copy one char */
			if(*ddread == '$') {
				ddread++;    /* skip if we saw 2 $'s in a row */
			}

	*ddwrite = '\0';

	return (string);
}


int fill_word(char *argument) {
	return (search_block(argument, fill, TRUE) >= 0);
}


int reserved_word(char *argument) {
	return (search_block(argument, reserved, TRUE) >= 0);
}


/*
 * copy the first non-fill-word, space-delimited argument of 'argument'
 * to 'first_arg'; return a pointer to the remainder of the string.
 */
char *one_argument(char *argument, char *first_arg) {
	return one_argument(argument,first_arg,MAX_INPUT_LENGTH);
}
char *one_argument(char *argument, char *first_arg,unsigned int max_char) {
	char *begin = first_arg;

	if(first_arg == nullptr) {
		return argument;
	}

	if(!argument) {
		log("SYSERR: one_argument received a NULL pointer!");
		*first_arg = '\0';
		return (NULL);
	}

	do {
		skip_spaces(&argument);

		first_arg = begin;

		while(*argument && !isspace(*argument)) {
			*(first_arg++) = LOWER(*argument);
			argument++;

			if(--max_char == 0) {
				*first_arg = '\0';
				return first_arg;
			}
		}

		*first_arg = '\0';
	} while(fill_word(begin));

	return (argument);
}


/*
 * one_word is like one_argument, except that words in quotes ("") are
 * considered one word.
 */
char *one_word(char *argument, char *first_arg) {
	char *begin = first_arg;

	do {
		skip_spaces(&argument);

		first_arg = begin;

		if(*argument == '\"') {
			argument++;

			while(*argument && *argument != '\"') {
				*(first_arg++) = LOWER(*argument);
				argument++;
			}

			argument++;
		} else {
			while(*argument && !isspace(*argument)) {
				*(first_arg++) = LOWER(*argument);
				argument++;
			}
		}

		*first_arg = '\0';
	} while(fill_word(begin));

	return (argument);
}


/* same as one_argument except that it doesn't ignore fill words */
char *any_one_arg(char *argument, char *first_arg) {
	skip_spaces(&argument);

	while(*argument && !isspace(*argument)) {
		*(first_arg++) = LOWER(*argument);
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}


/*
 * Same as one_argument except that it takes two args and returns the rest;
 * ignores fill words
 */
char *two_arguments(char *argument, char *first_arg, char *second_arg) {
	return (one_argument(one_argument(argument, first_arg), second_arg)); /* :-) */
}

char* three_arguments(char* argument,char *first_arg,char* second_arg,char* third_arg) {
	return (one_argument(one_argument(one_argument(argument,first_arg),second_arg),third_arg));
}


/*
 * determine if a given string is an abbreviation of another
 * (now works symmetrically -- JE 7/25/94)
 *
 * that was dumb.  it shouldn't be symmetrical.  JE 5/1/95
 *
 * returns 1 if arg1 is an abbreviation of arg2
 */
int is_abbrev(const char *arg1, const char *arg2) {
	if(!*arg1) {
		return (0);
	}

	for(; *arg1 && *arg2; arg1++, arg2++)
		if(LOWER(*arg1) != LOWER(*arg2)) {
			return (0);
		}

	if(!*arg1) {
		return (1);
	} else {
		return (0);
	}
}



/*
 * Return first space-delimited token in arg1; remainder of string in arg2.
 *
 * NOTE: Requires sizeof(arg2) >= sizeof(string)
 */
void half_chop(char *string, char *arg1, char *arg2) {
	char *temp;

	temp = any_one_arg(string, arg1);
	skip_spaces(&temp);
	strcpy(arg2, temp);	/* strcpy: OK (documentation) */
}



/* Used in specprocs, mostly.  (Exactly) matches "command" to cmd number */
int find_command(const char *command) {
	int cmd;

	for(cmd = 0; *cmd_info[cmd].command != '\n'; cmd++)
		if(!strcmp(cmd_info[cmd].command, command)) {
			return (cmd);
		}

	return (-1);
}


int special(char_data *ch, int cmd, char *arg) {
	/** legacy code caused segfault. not needed */
	return 0;
}



/* *************************************************************************
 *  Stuff for controlling the non-playing sockets (get name, pwd etc)       *
 ************************************************************************* */


/* This function needs to die. */
int _parse_name(char *arg, char *name) {
	int i;

	skip_spaces(&arg);

	for(i = 0; (*name = *arg); arg++, i++, name++)
		if(!isalpha(*arg)) {
			return (1);
		}

	if(!i) {
		return (1);
	}

	return (0);
}


#define RECON		1
#define USURP		2
#define UNSWITCH	3


/* This function seems a bit over-extended. */
int64_t perform_dupe_check(player_ptr_t p){
	std::string name = p->name();
	int64_t kicked = 0;
	for(auto& player_ptr : mods::globals::player_list){
		if(!player_ptr->authenticated()){
			continue;
		}
		if(p->socket() != player_ptr->socket() && 
				name.compare(player_ptr->name().c_str()) == 0){
			if(p->time() > player_ptr->time()){
				char_from_room(player_ptr);
				player_ptr->set_state(CON_CLOSE);
				++kicked;
			}
		}
	}
	return kicked;
}

/* deal with newcomers and other non-playing sockets */
void nanny(player_ptr_t p, char * in_arg) {
	int load_result;	/* Overloaded variable */

	skip_spaces(&in_arg);
	auto & d = p->desc();
	mutable_map_t row;
	std::string arg = in_arg;
	tuple_status_t status;
	std::tuple<bool,std::string> make_char_status;

	switch(p->state()) {
		case CON_GET_NAME:		/* wait for input of name */
			if(arg.length() == 0) {
				p->set_state(CON_CLOSE);
			} else {
				std::array<char,MAX_INPUT_LENGTH + 1> buf;
				std::fill(buf.begin(),buf.end(),0);
				if(!Valid_Name(arg.c_str()) ||
						fill_word(strncpy(&buf[0], arg.c_str(),MAX_INPUT_LENGTH)) || 
						reserved_word(&buf[0])) {	/* strcpy: OK (mutual MAX_INPUT_LENGTH) */
					write_to_output(d, "Invalid name, please try another.\r\nName: ");
					return;
				}

				p->set_name(arg);
				p->set_db_id(0);
				if(player_exists(p) == false){
					write_to_output(d, "Did I get that right, %s (Y/N)? ", p->name().c_str());
					p->set_state(CON_NAME_CNFRM);
				} else {
					write_to_output(d, "Password: ");
					echo_off(d);
					d.idle_tics = 0;
					p->set_state(CON_PASSWORD);
				}
			}
			break;

		case CON_NAME_CNFRM:		/* wait for conf. of new name    */
			if(arg[0] == 'Y' || arg[0] == 'y') {
				if(mods::ban_system::isbanned(p->host().c_str())) {
					mudlog(NRM, LVL_GOD, TRUE, "Request for new char %s denied from [%s] (siteban)", p->name().c_str(), p->host().c_str());
					write_to_output(d, "Sorry, new characters are not allowed from your site!\r\n");
					p->set_state(CON_CLOSE);
					return;
				}

				if(circle_restrict) {
					write_to_output(d, "Sorry, new players can't be created at the moment.\r\n");
					mudlog(NRM, LVL_GOD, TRUE, "Request for new char %s denied from [%s] (wizlock)", p->name().c_str(), p->host().c_str());
					p->set_state(CON_CLOSE);
					return;
				}

				write_to_output(d, "New character.\r\nGive me a password for %s: ", p->name().c_str());
				echo_off(d);
				p->set_db_id(0);
				p->set_state(CON_NEWPASSWD);
			} else if(arg[0] == 'n' || arg[0] == 'N') {
				p->set_name("");
				write_to_output(d, "Okay, what IS it, then? ");
				/** FIXME: possibly have to clear the char_data struct here */
				p->set_state(CON_GET_NAME);
			} else {
				write_to_output(d, "Please type Yes or No: ");
			}

			break;

		case CON_PASSWORD:		/* get pwd for known player      */
			/*
			 * To really prevent duping correctly, the player's record should
			 * be reloaded from disk at this point (after the password has been
			 * typed).  However I'm afraid that trying to load a character over
			 * an already loaded character is going to cause some problem down the
			 * road that I can't see at the moment.  So to compensate, I'm going to
			 * (1) add a 15 or 20-second time limit for entering a password, and (2)
			 * re-add the code to cut off duplicates when a player quits.  JE 6 Feb 96
			 */

			echo_on(d);    /* turn echo back on */

			/* New echo_on() eats the return on telnet. Extra space better than none. */
			write_to_output(d, "\r\n");

			if(arg.length() == 0) {
				p->set_state(CON_CLOSE);
				return;
			} else {
				/** FIXME: log user IP and all this stuff into postgres */
				if(login(p->name(),arg) == false){
					mudlog(BRF, LVL_GOD, TRUE, "Bad PW: %s [%s]", p->name().c_str(), p->host().c_str());
					p->increment_bad_password_count();

					if(p->get_bad_password_count() >= max_bad_pws){
						write_to_output(d, "Wrong password... disconnecting.\r\n");
						p->set_state(CON_CLOSE);
					} else {
						write_to_output(d, "Wrong password.\r\nPassword: ");
						echo_off(d);
					}
					return;
				}else{
					parse_sql_player(p);
				}

				/* Password was correct. */
				load_result = p->get_bad_password_count();
				p->set_bad_password_count(0);

				if(mods::ban_system::isbanned(p->host().c_str()) &&
						!p->has_affect_plr(PLR_SITEOK)) {
					write_to_output(d, "Sorry, this char has not been cleared for login from your site!\r\n");
					p->set_state(CON_CLOSE);
					mudlog(NRM, LVL_GOD, TRUE, "Connection attempt for %s denied from %s", p->name().c_str(), p->host().c_str());
					return;
				}

				if(p->level() < circle_restrict) {
					write_to_output(d, "The game is temporarily restricted.. try again later.\r\n");
					p->set_state(CON_CLOSE);
					mudlog(NRM, LVL_GOD, TRUE, "Request for login denied for %s [%s] (wizlock)", p->name().c_str(), p->host().c_str());
					return;
				}


				if(p->level() >= LVL_IMMORT) {
					write_to_output(d, "%s", imotd);
				} else {
					write_to_output(d, "%s", motd.c_str());
				}

				mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(p->cd())), TRUE, "%s [%s] has connected.", p->name().c_str(), p->host().c_str());

				if(load_result) {
					write_to_output(d, "\r\n\r\n\007\007\007"
							"%s%d LOGIN FAILURE%s SINCE LAST SUCCESSFUL LOGIN.%s\r\n",
							CCRED(p->cd(), C_SPR), load_result,
							(load_result > 1) ? "S" : "", CCNRM(p->cd(), C_SPR));
					GET_BAD_PWS(p->cd()) = 0;
				}

				write_to_output(d, "\r\n*** PRESS RETURN: ");
				p->set_state(CON_RMOTD);
			}

			break;

		case CON_NEWPASSWD:
		case CON_CHPWD_GETNEW:
			if(arg.length() > MAX_PWD_LENGTH || arg.length() < 3 ||
					arg.compare(p->name()) == 0) {
				write_to_output(d, "\r\nIllegal password.\r\nPassword: ");
				return;
			}

			p->set_password(arg);

			write_to_output(d, "\r\nPlease retype password: ");

			if(p->state() == CON_NEWPASSWD) {
				p->set_state(CON_CNFPASSWD);
			} else {
				p->set_state(CON_CHPWD_VRFY);
			}

			break;

		case CON_CNFPASSWD:
		case CON_CHPWD_VRFY:
			if(arg.compare(p->get_password()) != 0){
				write_to_output(d, "\r\nPasswords don't match... start over.\r\nPassword: ");

				if(p->state() == CON_CNFPASSWD) {
					p->set_state(CON_NEWPASSWD);
				} else {
					p->set_state(CON_CHPWD_GETNEW);
				}
				break;
			}

			echo_on(d);

			if(p->state() == CON_CNFPASSWD) {
				write_to_output(d, "\r\nWhat is your sex (M/F)? ");
				p->set_state(CON_QSEX);
			} else {
				write_to_output(d, "\r\nDone.\r\n%s", MENU);
				p->set_state(CON_MENU);
			}
			break;

		case CON_QSEX:		/* query sex of new user         */
			switch(arg[0]) {
				case 'm':
				case 'M':
					p->set_sex(SEX_MALE);
					break;

				case 'f':
				case 'F':
					p->set_sex(SEX_FEMALE);
					break;

				default:
					write_to_output(d, "That is not a sex..\r\n"
							"What IS your sex? ");
					return;
			}

			write_to_output(d, "%s\r\nSelect class: ", class_menu);
			p->set_state(CON_QCLASS);
			break;

		case CON_QCLASS:
			load_result = parse_class(arg[0]);

			if(load_result == CLASS_UNDEFINED) {
				write_to_output(d, "\r\nThat's not a class.\r\nClass: ");
				return;
			} else {
				GET_CLASS(p->cd()) = load_result;
				p->set_class(static_cast<player_class_t>(load_result));
			}
			make_char_status = mods::chargen::make_char(p);
			if(!std::get<0>(make_char_status)){
				write_to_output(d,"\r\n%s\r\n",std::get<1>(make_char_status).data());
				p->set_state(CON_CLOSE);
				/** TODO: undo the transaction */
				return;
			}
			switch(load_result){
				case CLASS_SENTINEL:
					write_to_output(d, "%s\r\nSelect primary weapon: ", mods::chargen::primary_weapon_menu(static_cast<player_class_t>(load_result)).data());
					p->set_state(CON_CHARGEN_PRIMARY_CHOICE);
					break;
				default:
					break;
			}
			return;
		case CON_CHARGEN_PRIMARY_CHOICE:
			mods::chargen::handle_primary_choice(p,arg[0],p->get_class());
			return;
		case CON_RMOTD:		/* read CR after printing motd   */
			write_to_output(d, "%s", MENU);
			p->set_state(CON_MENU);
			parse_sql_player(p);
			break;

		case CON_MENU: {		/* get selection from main menu  */
										 switch(arg[0]) {
											 case '0':
												 write_to_output(d, "Goodbye.\r\n");
												 p->set_state(CON_CLOSE);
												 break;

											 case '1':
												 p->set_authenticated(true);
												 decorate_authenticated_player(p);

												 /* check and make sure no other copies of this player are logged in */
												 perform_dupe_check(p);

												 p->stc(WELC_MESSG);
												 {
													 int start_room = 0;
													 if(!boot_type_hell()){
														 start_room = mods::world_conf::real_mortal_start();
													 }
													 if(world.size() == 0){
														 exit(0);
													 }
													 p->set_room(start_room);
													 char_to_room(p->cd(),start_room);
												 }
												 act("$n has entered the game.", TRUE, p->cd(), 0, 0, TO_ROOM);
												 p->set_state(CON_PLAYING);

												 if(p->level() == 0) {
													 do_start(p->cd());
													 p->stc(START_MESSG);
												 }

												 look_at_room(p->cd(), 0);

												 if(has_mail(GET_IDNUM(p->cd()))) {
												   p->stc("You have mail waiting.\r\n");
												 }
												 p->start_histfile();

												 p->desc().has_prompt = 0;
#ifndef __MENTOC_DONT_RUN_PROFILE_SCRIPTS__
												 mods::js::run_profile_scripts(p->name());
#endif
												 break;

											 case '2':
												 if(p->cd()->player.description) {
													 write_to_output(d, "Old description:\r\n%s", p->cd()->player.description.c_str());
													 p->cd()->player.description.clear();
												 }

												 write_to_output(d, "Enter the new text you'd like others to see when they look at you.\r\n"
														 "Terminate with a '@' on a new line.\r\n");
												 d.str = p->cd()->player.description.ptrptr();
												 d.max_str = EXDSCR_LENGTH;
												 p->set_state(CON_EXDESC);
												 break;

											 case '3':
												 page_string(d, background, 0);	/**FIXME*/
												 p->set_state(CON_RMOTD);
												 break;

											 case '4':
												 write_to_output(d, "\r\nEnter your old password: ");
												 echo_off(d);
												 p->set_state(CON_CHPWD_GETOLD);
												 break;

											 case '5':
												 write_to_output(d, "\r\nEnter your password for verification: ");
												 echo_off(d);
												 p->set_state(CON_DELCNF1);
												 break;

											 default:
												 write_to_output(d, "\r\nThat's not a menu choice!\r\n%s", MENU);
												 break;
										 }

										 break;
									 }

		case CON_CHPWD_GETOLD:

									 if(p->get_password().compare(arg) != 0){
										 echo_on(d);
										 write_to_output(d, "\r\nIncorrect password.\r\n%s", MENU);
										 p->set_state(CON_MENU);
									 } else {
										 write_to_output(d, "\r\nEnter a new password: ");
										 p->set_state(CON_CHPWD_GETNEW);
									 }

									 return;

		case CON_DELCNF1:
									 echo_on(d);

									 if(p->get_password().compare(arg) != 0){
										 write_to_output(d, "\r\nIncorrect password.\r\n%s", MENU);
										 p->set_state(CON_MENU);
									 } else {
										 write_to_output(d, "\r\nYOU ARE ABOUT TO DELETE THIS CHARACTER PERMANENTLY.\r\n"
												 "ARE YOU ABSOLUTELY SURE?\r\n\r\n"
												 "Please type \"yes\" to confirm: ");
										 p->set_state(CON_DELCNF2);
									 }

									 break;

		case CON_DELCNF2:
									 if(arg.compare("yes") == 0 || arg.compare("YES") == 0) {
										 if(p->has_affect_plr(PLR_FROZEN)) {
											 write_to_output(d, "You try to kill yourself, but the ice stops you.\r\n"
													 "Character not deleted.\r\n\r\n");
											 p->set_state(CON_CLOSE);
											 return;
										 }

										 if(p->level() < LVL_GRGOD) {
											 p->affect_plr(PLR_DELETED);
										 }

										 mods::db::save_char(p);
										 write_to_output(d, "Character '%s' deleted!\r\n"
												 "Goodbye.\r\n", p->name().c_str());
										 mudlog(NRM, LVL_GOD, TRUE, "%s (lev %d) has self-deleted.", p->name().c_str(), p->level());
										 p->set_state(CON_CLOSE);
										 p->deactivate_account();
										 return;
									 } else {
										 write_to_output(d, "\r\nCharacter not deleted.\r\n%s", MENU);
										 p->set_state(CON_MENU);
									 }

									 break;

									 /*
										* It's possible, if enough pulses are missed, to kick someone off
										* while they are at the password prompt. We'll just defer to let
										* the game_loop() axe them.
										*/
		case CON_CLOSE:
									 break;
		case CON_IDLE:
									 /**
										* User has been put into the idle queue. Place them back into the world.
										*/

									act("$n has returned.", TRUE, p->cd(), 0, 0, TO_ROOM);
									p->char_specials().timer = 0;
									p->set_state(CON_PLAYING);
									char_from_room(p->cd());
									char_to_room(p->cd(), mods::world_conf::real_mortal_start());
									break;

		default:
									 log("SYSERR: Nanny: illegal state of con'ness (%d) for '%s'; closing connection.",
											 p->state(), p->name().c_str());
									 p->set_state(CON_DISCONNECT);	/* Safest to do. */
									 break;
	}
}
