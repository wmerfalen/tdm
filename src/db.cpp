/* ************************************************************************
 *   File: db.c                                          Part of CircleMUD *
 *  Usage: Loading/saving chars, booting/resetting world, internal funcs   *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#define __DB_C__

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "db.h"
#include "comm.h"
#include "handler.h"
#include "spells.h"
#include "mail.h"
#include "interpreter.h"
#include "house.h"
#include "constants.h"
#include "globals.hpp"
#include "shop.h"
#include <vector>
#include <deque>
#include "mods/behaviour_tree_impl.hpp"
#include "mods/db.hpp"
#include "mods/filesystem.hpp"
#include "mods/lmdb.hpp"
#include "mods/hell.hpp"
#include "mods/meta_utils.hpp"
#include "mods/flags.hpp"
#include "mods/pq.hpp"
#include "mods/sql.hpp"
#include "mods/npc.hpp"
#include "mods/world-configuration.hpp"
#include "mods/classes/sentinel.hpp"
#include "mods/orm/inventory.hpp"
#include "mods/orm/shop.hpp"
#include "mods/object-utils.hpp"
#include "mods/mobs/extended-types.hpp"
#include "mods/mobs/roam-pattern.hpp"
#include "mods/util.hpp"
#include "mods/players/db-load.hpp"
#include "mods/players/friendly-reminders.hpp"
#include "mods/players/event-messages.hpp"
#include "mods/mini-games.hpp"
#include "mods/integral-objects.hpp"
#include "mods/db.hpp"
#include "mods/zone.hpp"
#include "mods/classes/ghost.hpp"
#include "mods/mob-roam.hpp"
#include "mods/util-map.hpp"
#include "mods/rooms.hpp"
#include "mods/prefs.hpp"
#include "mods/radio.hpp"
#include "mods/str.hpp"
#include "mods/orm/loot-payload.hpp"
#include "mods/orm/loot-ammo.hpp"
#include "mods/loot.hpp"

namespace mods::zone {
	extern void reset_zone(zone_rnum);
};
namespace mods::rooms {
	extern void set_sector_type(room_rnum room_id, int sector_type);
	extern void set_flag_absolute(room_rnum room_id, int);
};
using behaviour_tree = mods::behaviour_tree_impl::node_wrapper;
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
using shop_data_t = shop_data<mods::orm::shop,mods::orm::shop_rooms,mods::orm::shop_objects>;
using shop_ptr_t = std::shared_ptr<shop_data_t>;
namespace mods::skills {
	extern void load_player_levels(player_ptr_t&);
};

/**************************************************************************
 *  declarations of most of the 'global' variables                         *
 **************************************************************************/
bool db_has_been_booted = false;
std::tuple<int16_t,std::string> parse_sql_rooms();
std::tuple<int16_t,std::string> parse_sql_zones();
int parse_sql_objects();
void parse_sql_mobiles();
int parse_sql_shops();
std::deque<room_data> world;	/* array of rooms		 */
std::size_t world_size() {
	return world.size();
}
room_rnum top_of_world = 0;	/* ref to top element of world	 */

extern char_data *character_list;	/* global linked list of
																					 * chars	 */
std::vector<index_data> mob_index;	/* index table for mobile file	 */
std::deque<char_data> mob_proto;	/* prototypes for mobs		 */
mob_rnum top_of_mobt = 0;	/* top of mobile index table	 */

std::deque<std::shared_ptr<obj_data>> obj_list;
std::deque<std::shared_ptr<mods::npc>> mob_list;
std::deque<std::shared_ptr<shop_data_t>> shop_list;

extern std::deque<std::shared_ptr<shop_data_t>> shop_list;
namespace mods::globals {
	using coordinate_type_t = int32_t;
	extern void register_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room);
	extern void glue_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room);
	extern std::map<room_rnum,std::shared_ptr<shop_data_t>> room_shopmap;
};
std::vector<index_data> obj_index;	/* index table for object file	 */
std::deque<obj_data> obj_proto;	/* prototypes for objs		 */
std::deque<shop_data_t> shop_proto;	/* prototypes for objs		 */
obj_rnum top_of_objt = 0;	/* top of object index table	 */

std::deque<zone_data> zone_table;	/* zone table			 */
zone_rnum top_of_zone_table = 0;/* top element of zone tab	 */
struct message_list fight_messages[MAX_MESSAGES];	/* fighting messages	 */

std::deque<player_index_element> player_table;	/* index to plr file	 */

int top_of_p_table = 0;		/* ref to top of table		 */
long top_idnum = 0;		/* highest idnum in use		 */

int no_mail = 0;		/* mail disabled?		 */
int mini_mud = 0;		/* mini-mud mode?		 */
int no_rent_check = 0;		/* skip rent check on boot?	 */
time_t boot_time = 0;		/* time of mud boot		 */
int circle_restrict = 0;	/* level of game restriction	 */
room_rnum r_mortal_start_room;	/* rnum of mortal start room	 */
room_rnum r_immort_start_room;	/* rnum of immort start room	 */
room_rnum r_frozen_start_room;	/* rnum of frozen start room	 */
room_rnum r_idle_room;

char *credits = NULL;		/* game credits			 */
char *news = NULL;		/* mud news			 */
std::string motd = "Welcome to Tactical Denial.";		/* message of the day - mortals */
char *imotd = NULL;		/* message of the day - immorts */
std::string GREETINGS = "";		/* opening credits screen	*/
char *help = NULL;		/* help screen			 */
char *info = NULL;		/* info page			 */
char *wizlist = NULL;		/* list of higher gods		 */
char *immlist = NULL;		/* list of peon gods		 */
char *background = NULL;	/* background story		 */
char *handbook = NULL;		/* handbook for new immortals	 */
char *policies = NULL;		/* policies page		 */

struct help_index_element *help_table = 0;	/* the help table	 */
int top_of_helpt = 0;		/* top of help index table	 */

struct time_info_data time_info;/* the infomation about the time    */
struct weather_data weather_info;	/* the infomation about the weather */
struct player_special_data dummy_mob;	/* dummy spec area for mobs	*/
struct reset_q_type reset_q;	/* queue of zones to be reset	 */

/* local functions */
int check_bitvector_names(bitvector_t bits, size_t namecount, const char *whatami, const char *whatbits);
int check_object_spell_number(struct obj_data *obj, int val);
int check_object_level(struct obj_data *obj, int val);
void index_boot(int mode);
void discrete_load(FILE *fl, int mode, char *filename);
int check_object(struct obj_data *);
void parse_room(FILE *fl, int virtual_nr);
void parse_mobile(FILE *mob_f, int nr);
char *parse_object(FILE *obj_f, int nr);
void load_zones(FILE *fl, char *zonename);
void load_help(FILE *fl);
void assign_mobiles(void);
void assign_objects(void);
void assign_the_shopkeepers(void);
void build_player_index(void);
int is_empty(zone_rnum zone_nr);
void reset_zone(zone_rnum zone);
void reboot_wizlists(void);
SUPERCMD(do_reboot);
void boot_world(void);
int count_alias_records(FILE *fl);
int count_hash_records(FILE *fl);
bitvector_t asciiflag_conv(char *flag);
void parse_simple_mob(FILE *mob_f, int i, int nr);
void interpret_espec(const char *keyword, const char *value, int i, int nr);
void parse_espec(char *buf, int i, int nr);
void parse_enhanced_mob(FILE *mob_f, int i, int nr);
void get_one_line(FILE *fl, char *buf);
void save_etext(char_data *ch);
void check_start_rooms(void);
void renum_world(void);
void renum_zone_table(void);
void log_zone_error(zone_rnum zone, int cmd_no, const char *message);
void reset_time(void);
long get_ptable_by_name(const char *name);

/* external functions */
void paginate_string(char *str, mods::descriptor_data d);
struct time_info_data *mud_time_passed(time_t t2, time_t t1);
void free_alias(struct alias_data *a);
void load_messages(void);
void weather_and_time(int mode);
void mag_assign_spells(void);
void boot_social_messages(void);
void update_obj_file(void);	/* In objsave.c */
void sort_commands(void);
void sort_spells(void);
void boot_the_shops(void);
int hsort(const void *a, const void *b);
void prune_crlf(char *txt);
void destroy_shops(void);

/* external vars */
extern int no_specials;
extern int scheck;
extern room_vnum mortal_start_room;
extern room_vnum immort_start_room;
extern room_vnum frozen_start_room;
extern std::deque<mods::descriptor_data>  descriptor_list;
extern const char *unused_spellname;	/* spell_parser.c */
/*************************************************************************
 *  routines for booting the system                                       *
 *************************************************************************/

/* this is necessary for the autowiz system */
void reboot_wizlists(void) {
}


/* Wipe out all the loaded text files, for shutting down. */
void free_text_files(void) {
}


/*
 * Too bad it doesn't check the return values to let the user
 * know about -1 values.  This will result in an 'Okay.' to a
 * 'reload' command even when the string was not replaced.
 * To fix later, if desired. -gg 6/24/99
 */
SUPERCMD(do_reboot) {
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

}

void boot_hell(void) {
	log("Booting hell");
	unsigned i;

	log("Boot hell -- BEGIN.");

	log("Resetting the game time:");
	reset_time();

	log("Reading news, credits, help, bground, info & motds.");

	log("Loading spell definitions.");
	if(mods::hell::mag_assign_spells) {
		mag_assign_spells();
	}

	log("Loading help entries.");
	if(mods::hell::index_boot_hlp) {
		index_boot(DB_BOOT_HLP);
	}

	log("Generating player index.");
	/*FIXME: replace this functionality:
	 * build_player_index();
	 */

	log("Loading fight messages.");
	if(mods::hell::load_messages) {
		load_messages();
	}

	if(mods::hell::boot_social_messages) {
		log("Loading social messages.");
		boot_social_messages();
	};

	log("Assigning function pointers:");

	if(!no_specials) {
		log("   Mobiles.");
		if(mods::hell::assign_mobiles) {
			assign_mobiles();
		}
		log("   Shopkeepers.");
		if(mods::hell::assign_the_shopkeepers) {
			assign_the_shopkeepers();
		};
		log("   Objects.");
		if(mods::hell::assign_objects) {
			assign_objects();
		}
	}

	log("Assigning spell and skill levels.");
	if(mods::hell::init_spell_levels) {
		init_spell_levels();
	};

	log("Sorting command list and spells.");
	if(mods::hell::sort_commands) {
		sort_commands();
	};
	if(mods::hell::sort_spells) {
		sort_spells();
	};

	log("Booting mail system.");

	if(mods::hell::scan_file) {
		if(!scan_file()) {
			log("    Mail boot failed -- Mail system disabled");
			no_mail = 1;
		}
	}

	if(!no_rent_check) {
		log("Deleting timed-out crash and rent files:");
		if(mods::hell::update_obj_file) {
			update_obj_file();
		};
		log("   Done.");
	}

	/* Moved here so the object limit code works. -gg 6/24/98 */
	if(!mini_mud) {
		log("Booting houses.");
		if(mods::hell::House_boot) {
			House_boot();
		};
	}

	for(i = 0; i < zone_table.size(); i++) {
		log("Resetting #%d: %s (rooms %d-%d).", zone_table[i].number,
		    zone_table[i].name, zone_table[i].bot, zone_table[i].top);
		reset_zone(i);
	}

	reset_q.head = reset_q.tail = NULL;

	boot_time = time(0);

	log("Creating hell hole");

	room_data room;
	room.name = "hell hole";
	room.description = "-void-";

	world.push_back(room);
	mods::globals::register_room(world.size());

	log("Boot db -- DONE.");
	mods::globals::post_boot_db();

	db_has_been_booted = true;
}

void boot_world(void) {
	log("Loading zone table.");
	//index_boot(DB_BOOT_ZON);
	log("Parsing sql zones.");
	auto zone_status = parse_sql_zones();
	log("Parsed %d zones",std::get<0>(zone_status));
	if(std::get<0>(zone_status) < 0) {
		log("SYSERR: parse_sql_zones: '%s'",std::get<1>(zone_status).c_str());
	}

	mods::orm::loot_payload_list() = mods::orm::load_all_loot_payload_list();
	mods::orm::loot_ammo_list() = mods::orm::load_all_loot_ammo_list();

	log("Parsing sql rooms.");
	auto tuple_status_rooms = parse_sql_rooms();
	log("parse_sql_rooms return: %s",(std::get<1>(tuple_status_rooms)).c_str());
	if(std::get<0>(tuple_status_rooms)  == 0) {
		log("parse_sql_rooms didn't find any rooms");
	} else {
		log("Found %d rooms",std::get<0>(tuple_status_rooms));
	}

	log("Checking start rooms.");
	check_start_rooms();

	log("Loading sql mobs and generating index.");
	parse_sql_mobiles();

	log("Loading mob roaming data");
	mods::mob_roam::boot();

	log("Loading sql objs and generating index.");
	//index_boot(DB_BOOT_OBJ);
	parse_sql_objects();

	log("Loading sql shops and generating index.");
	parse_sql_shops();


	log("Renumbering zone table.");
	renum_zone_table();

	log("Booting shops");
	boot_the_shops();

	if(!no_specials) {
		log("Loading shops.");
		index_boot(DB_BOOT_SHP);
	}

	log("Loading friendly reminders...");
	mods::players::friendly_reminders::load_from_db();

	log("Loading event messages...");
	mods::players::event_messages::load_from_db();

	mods::radio::init();
}


void free_extra_descriptions(struct extra_descr_data *edesc) {
	//	struct extra_descr_data *enext;
	//
	//	for(; edesc; edesc = enext) {
	//		enext = edesc->next;
	//
	//		free(edesc->keyword);
	//		free(edesc->description);
	//		free(edesc);
	//	}
#ifdef __MENTOC_SHOW_DEPRECATED__
	std::cerr << "[stub]: free_extra_descriptions - things will break if malloc()'d\n";
#endif
}


/* body of the booting system */
void boot_db(void) {
	unsigned i;

	log("Boot db -- BEGIN.");

	log("Resetting the game time:");
	reset_time();

	log("Reading news, credits, help, bground, info & motds.");
	log("Loading spell definitions.");
	mag_assign_spells();

	boot_world();

	log("Loading help entries.");
	index_boot(DB_BOOT_HLP);

	log("Generating player index.");
	/*FIXME: replace this functionality:
	 * build_player_index();
	 */

	log("Loading fight messages.");
	load_messages();

	log("Loading social messages.");
	boot_social_messages();

	log("Assigning function pointers:");

	if(!no_specials) {
		log("   Mobiles.");
		assign_mobiles();
		log("   Shopkeepers.");
		assign_the_shopkeepers();
		log("   Objects.");
		assign_objects();
	}

	log("Assigning spell and skill levels.");
	init_spell_levels();

	log("Sorting command list and spells.");
	sort_commands();
	sort_spells();

	log("Booting mail system.");

	if(!scan_file()) {
		log("    Mail boot failed -- Mail system disabled");
		no_mail = 1;
	}

	if(!no_rent_check) {
		log("Deleting timed-out crash and rent files:");
		update_obj_file();
		log("   Done.");
	}

	/* Moved here so the object limit code works. -gg 6/24/98 */
	if(!mini_mud) {
		log("Booting houses.");
		House_boot();
	}

	for(i = 0; i < zone_table.size(); i++) {
		log("Resetting #%d: %s (rooms %d-%d).", zone_table[i].number,
		    zone_table[i].name, zone_table[i].bot, zone_table[i].top);
		reset_zone(i);
	}

	mods::mini_games::init();

	reset_q.head = reset_q.tail = NULL;

	boot_time = time(0);

	log("Boot db -- DONE.");
	mods::globals::post_boot_db();
	db_has_been_booted = true;
}


/* reset the time in the game from file */
void reset_time(void) {
	time_t beginning_of_time = 0;

	if(beginning_of_time == 0) {
		beginning_of_time = 650336715;
	}

	time_info = *mud_time_passed(time(0), beginning_of_time);

	if(time_info.hours <= 4) {
		weather_info.sunlight = SUN_DARK;
	} else if(time_info.hours == 5) {
		weather_info.sunlight = SUN_RISE;
	} else if(time_info.hours <= 20) {
		weather_info.sunlight = SUN_LIGHT;
	} else if(time_info.hours == 21) {
		weather_info.sunlight = SUN_SET;
	} else {
		weather_info.sunlight = SUN_DARK;
	}

	log("   Current Gametime: %dH %dD %dM %dY.", time_info.hours,
	    time_info.day, time_info.month, time_info.year);

	weather_info.pressure = 960;

	if((time_info.month >= 7) && (time_info.month <= 12)) {
		weather_info.pressure += dice(1, 50);
	} else {
		weather_info.pressure += dice(1, 80);
	}

	weather_info.change = 0;

	if(weather_info.pressure <= 980) {
		weather_info.sky = SKY_LIGHTNING;
	} else if(weather_info.pressure <= 1000) {
		weather_info.sky = SKY_RAINING;
	} else if(weather_info.pressure <= 1020) {
		weather_info.sky = SKY_CLOUDY;
	} else {
		weather_info.sky = SKY_CLOUDLESS;
	}
}


/* Write the time in 'when' to the MUD-time file. */
void save_mud_time(struct time_info_data *when) {
#if 0
	FILE *bgtime;

	if((bgtime = fopen(TIME_FILE, "w")) == NULL) {
		log("SYSERR: Can't write to '%s' time file.", TIME_FILE);
	} else {
		fprintf(bgtime, "%ld\n", mud_time_to_secs(when));
		fclose(bgtime);
	}
#endif
}


void free_player_index(void) {
	log("[deprecated] free_player_index");
	return;
}


/* generate index table for the player file */
void build_player_index(void) {
	log("[deprecated] build_player_index");
	return;
}

/*
 * Thanks to Andrey (andrey@alex-ua.com) for this bit of code, although I
 * did add the 'goto' and changed some "while()" into "do { } while()".
 *	-gg 6/24/98 (technically 6/25/98, but I care not.)
 */
int count_alias_records(FILE *fl) {
	char key[READ_SIZE], next_key[READ_SIZE];
	char line[READ_SIZE], *scan;
	int total_keywords = 0;

	/* get the first keyword line */
	get_one_line(fl, key);

	while(*key != '$') {
		/* skip the text */
		do {
			get_one_line(fl, line);

			if(feof(fl)) {
				goto ackeof;
			}
		} while(*line != '#');

		/* now count keywords */
		scan = key;

		do {
			scan = one_word(scan, next_key);

			if(*next_key) {
				++total_keywords;
			}
		} while(*next_key);

		/* get next keyword line (or $) */
		get_one_line(fl, key);

		if(feof(fl)) {
			goto ackeof;
		}
	}

	return (total_keywords);

	/* No, they are not evil. -gg 6/24/98 */
ackeof:
	log("SYSERR: Unexpected end of help file.");
	exit(1);	/* Some day we hope to handle these things better... */
}

/* function to count how many hash-mark delimited records exist in a file */
int count_hash_records(FILE *fl) {
	char buf[128];
	int count = 0;

	while(fgets(buf, 128, fl))
		if(*buf == '#') {
			count++;
		}

	return (count);
}



void index_boot(int mode) {
	switch(mode) {
		case DB_BOOT_WLD:
			world.clear();
			break;

		case DB_BOOT_MOB:
			break;

		case DB_BOOT_OBJ:
			break;

		case DB_BOOT_ZON:
			break;

		case DB_BOOT_HLP:
			break;
	}

	switch(mode) {
		case DB_BOOT_WLD:
		case DB_BOOT_OBJ:
		case DB_BOOT_MOB:
			//discrete_load(db_file, mode, buf2);
			break;

		case DB_BOOT_ZON:
			//load_zones(db_file, buf2);
			break;

		case DB_BOOT_HLP:
			/*
			 * If you think about it, we have a race here.  Although, this is the
			 * "point-the-gun-at-your-own-foot" type of race.
			 */
			//load_help(db_file);
			break;

		case DB_BOOT_SHP:
			//boot_the_shops();
			break;
	}

	/* sort the help index */
	if(mode == DB_BOOT_HLP) {
		//qsort(help_table, top_of_helpt, sizeof(struct help_index_element), hsort);
		//top_of_helpt--;
	}
}


void discrete_load(FILE *fl, int mode, char *filename) {
	int nr = -1, last;
	char line[READ_SIZE];

	const char *modes[] = {"world", "mob", "obj"};

	for(;;) {
		/*
		 * we have to do special processing with the obj files because they have
		 * no end-of-record marker :(
		 */
		if(mode != DB_BOOT_OBJ || nr < 0)
			if(!get_line(fl, line)) {
				if(nr == -1) {
					log("SYSERR: %s file %s is empty!", modes[mode], filename);
				} else {
					log("SYSERR: Format error in %s after %s #%d\n"
					    "...expecting a new %s, but file ended!\n"
					    "(maybe the file is not terminated with '$'?)", filename,
					    modes[mode], nr, modes[mode]);
				}

				exit(1);
			}

		if(*line == '$') {
			return;
		}

		if(*line == '#') {
			last = nr;

			if(sscanf(line, "#%d", &nr) != 1) {
				log("SYSERR: Format error after %s #%d", modes[mode], last);
				exit(1);
			}
		} else {
			log("SYSERR: Format error in %s file %s near %s #%d", modes[mode],
			    filename, modes[mode], nr);
			log("SYSERR: ... offending line: '%s'", line);
			exit(1);
		}
	}
}

bitvector_t asciiflag_conv(char *flag) {
	bitvector_t flags = 0;
	int is_num = TRUE;
	char *p;

	for(p = flag; *p; p++) {
		if(islower(*p)) {
			flags |= 1 << (*p - 'a');
		} else if(isupper(*p)) {
			flags |= 1 << (26 + (*p - 'A'));
		}

		if(!isdigit(*p)) {
			is_num = FALSE;
		}
	}

	if(is_num) {
		flags = atol(flag);
	}

	return (flags);
}


void parse_sql_mobiles() {
	/** Function status
	 * This function will load a mobile prototype from
	 * the database. A prototype is a template for what
	 * a mob is supposed to be. It has the base values
	 * that are more like suggestions for the inheriting
	 * mobile. Cool idea: have values that are set in stone
	 *
	 * !needs-testing: I have not verified that all of the columns
	 * loaded from postgres are in fact loading into each prototype correctly.
	 * I can assume that they are since the code doesn't crash when a prototype
	 * is loaded, but that's a very superficial assumption. Edge cases here
	 * will likely cause weird behaviour down the road if not caught now. [2019-02-15]
	 */
	top_of_mobt = 0;

#ifdef __MENTOC_PARSE_MOBILES_DEBUG_OUTPUT__
#define psm_debug(a){ std::cerr << "[parse_sql_mobiles]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
#else
#define psm_debug(a) /**/
#endif
	psm_debug("db_get_all mobiles... -> parse_sql_mobiles [start]");
	for(auto&& row : db_get_all("mobile")) {
		char_data proto;
		psm_debug("name");
		proto.player.name.assign(row["mob_name"]);
		psm_debug("DEBUG: mob proto name: '" << row["mob_name"].c_str());
		psm_debug("proto.player.name.c_str(): '" << proto.player.name.c_str() << "'");
		proto.player.short_descr.assign(row["mob_short_description"]);
		proto.player.long_descr.assign(row["mob_long_description"]);
		psm_debug("proto.player.short_descr: '" << proto.player.short_descr.c_str() << "'");
		psm_debug("proto.player.long_descr: '" << proto.player.long_descr.c_str() << "'");

		proto.player.description.assign(row["mob_description"]);
		psm_debug("proto.player.description: '" << proto.player.description.c_str() << "'");

		proto.char_specials.saved.act = mods::util::stoi<int>(row["mob_action_bitvector"]);
		SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
		REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);
		proto.char_specials.saved.affected_by = mods::util::stoi<int>(row["mob_affection_bitvector"]);
		proto.char_specials.saved.alignment = mods::util::stoi<int>(row["mob_alignment"]);

		/* AGGR_TO_ALIGN is ignored if the mob is AGGRESSIVE. */
		if(MOB_FLAGGED(&proto, MOB_AGGRESSIVE) && MOB_FLAGGED(&proto, MOB_AGGR_GOOD | MOB_AGGR_EVIL | MOB_AGGR_NEUTRAL)) {
			log("SYSERR: Mob both Aggressive and Aggressive_to_Alignment.");
		}

#define MENTOC_ABIL_SET(struct_name,sql_name) proto.real_abils.struct_name = mods::util::stoi<int>(row[#sql_name]);
		MENTOC_ABIL_SET(str,mob_ability_strength);
		MENTOC_ABIL_SET(intel,mob_ability_intelligence);
		MENTOC_ABIL_SET(wis,mob_ability_wisdom);
		MENTOC_ABIL_SET(dex,mob_ability_dexterity);
		MENTOC_ABIL_SET(con,mob_ability_constitution);
		MENTOC_ABIL_SET(cha,mob_ability_charisma);

		MENTOC_ABIL_SET(electronics,mob_ability_electronics);
		MENTOC_ABIL_SET(armor,mob_ability_armor);
		MENTOC_ABIL_SET(marksmanship,mob_ability_marksmanship);
		MENTOC_ABIL_SET(sniping,mob_ability_sniping);
		MENTOC_ABIL_SET(demolitions,mob_ability_demolitions);
		MENTOC_ABIL_SET(chemistry,mob_ability_chemistry);
		MENTOC_ABIL_SET(weapon_handling,mob_ability_weapon_handling);
		MENTOC_ABIL_SET(strategy,mob_ability_strategy);
		MENTOC_ABIL_SET(medical,mob_ability_medical);

		proto.player.level = mods::util::stoi<int>(row["mob_level"]);
		proto.points.hitroll = mods::util::stoi<int>(row["mob_hitroll"]);
		proto.points.armor = mods::util::stoi<int>(row["mob_armor"]);
		proto.points.damroll = mods::util::stoi<int>(row["mob_damroll"]);

		proto.mob_specials.init();

		/* max hit = 0 is a flag that H, M, V is xdy+z */
		GET_MAX_HIT(&proto) = mods::util::stoi<int>(row["mob_max_hitpoints"]);
		GET_HIT(&proto) = mods::util::stoi<int>(row["mob_hitpoints"]);
		GET_MANA(&proto) = mods::util::stoi<int>(row["mob_mana"]);
		GET_MOVE(&proto) = mods::util::stoi<int>(row["mob_move"]);
		GET_MAX_MANA(&proto) = mods::util::stoi<int>(row["mob_max_mana"]);
		GET_MAX_MOVE(&proto) = mods::util::stoi<int>(row["mob_max_move"]);
		proto.mob_specials.damnodice = mods::util::stoi<int>(row["mob_damnodice"]);
		proto.mob_specials.damsizedice = mods::util::stoi<int>(row["mob_damsizedice"]);
		proto.mob_specials.behaviour_tree = behaviour_tree::NONE;
		proto.mob_specials.experience = mods::util::stoi<int>(row["mob_exp"]);
		proto.mob_specials.mp = mods::util::stoi<int>(row["mob_gold"]);
		proto.mob_specials.raid_id = mods::util::stoi<int>(row["mob_raid_id"]);
		proto.mob_specials.scalable = !!mods::util::stoi<int>(row["mob_scalable"]);
		//GET_EXP(&proto) = mods::util::stoi<int>(row["mob_exp"]);
		GET_POS(&proto) = mods::util::stoi<int>(row["mob_load_position"]);
		GET_DEFAULT_POS(&proto) = mods::util::stoi<int>(row["mob_load_position"]);
		GET_SEX(&proto) = mods::util::stoi<int>(row["mob_sex"]);
		GET_CLASS(&proto) = mods::util::stoi<int>(row["mob_class"]);
		GET_WEIGHT(&proto) = mods::util::stoi<int>(row["mob_weight"]);
		GET_HEIGHT(&proto) = mods::util::stoi<int>(row["mob_height"]);
		/*
		 * these are now save applies; base save numbers for MOBs are now from
		 * the warrior save table.
		 */
		unsigned j = 0;

		for(; j < 5; j++) {
			GET_SAVE(&proto, j) = 0;
		}

		proto.aff_abils = proto.real_abils;

		for(; j < NUM_WEARS; j++) {
			proto.equipment[j] = nullptr;
		}

		proto.nr = mods::util::stoi<decltype(proto.nr)>(row["mob_virtual_number"]);
		proto.mob_specials.vnum = mods::util::stoi<decltype(proto.mob_specials.vnum)>(row["mob_virtual_number"]);
#ifdef __MENTOC_SHOW_PARSE_MOB_OUTPUT__
		log("proto.nr: %d", proto.nr);
#endif
		proto.uuid = mods::globals::mob_uuid();
		proto.mob_specials.extended_mob_type = static_cast<mods::mobs::extended_types_t>(row["mob_special_extended_type"].as<uint16_t>());
		mob_proto.push_back(proto);
		mob_proto.back().nr = proto.nr;
		if(row["mob_roam_pattern"].is_null() == false) {
			mods::mobs::roam_pattern::register_roam_pattern(proto.nr, row["mob_roam_pattern"].c_str());
		}
		if(row["mob_targets"].is_null() == false) {
			mods::mobs::extended_types::register_targets(proto.nr, row["mob_targets"].c_str());
		}
#ifdef __MENTOC_SHOW_PARSE_MOB_OUTPUT__
		log("mob_proto.size(): %d",mob_proto.size());
		log("mob_proto.back().nr: %d", mob_proto.back().nr);
#endif

		top_of_mobt = mob_proto.size();
		index_data m_index;
		m_index.vnum = mods::util::stoi<int>(row["mob_virtual_number"]);
		m_index.number = 0;
		m_index.func = nullptr;
		//assert(real_mobile(m_index.vnum) == mob_proto.size() -1);
		mob_index.push_back(m_index);
	}
	psm_debug("db_get_all mobiles... -> parse_sql_mobiles [end]");
}

int parse_sql_objects() {
	auto result = db_get_all("object");

	mods::object_utils::set_yaml_initiator("parse_sql_objects","","");
	mods::object_utils::set_yaml_transaction_id(0);

	if(result.size()) {

		for(auto row : result) {
			if(strlen(row["obj_file"].c_str()) == 0) {
				REPORT_DB_ISSUE(CAT("SYSERR: obj_file is zero!-> '",row["id"].c_str(),"'").c_str(),"");
				continue;
			}
			if(row["obj_type"].as<int>() == 0 || !row["obj_file"].c_str()) {
				REPORT_DB_ISSUE("parse_sql_objects encountered broken row ","");
				continue;
			}
			mods::object_utils::set_yaml_initiator("parse_sql_objects","",row["obj_file"].c_str());
			index_data index;
			index.vnum = (row["obj_item_number"]).as<int>();
			index.number = 0;
			index.func = nullptr;
			obj_index.push_back(index);
			auto index_ref = &obj_index.back();
			std::string obj_file = "g36c.yml";
			if(strlen(row["obj_file"].c_str())) {
				obj_file = row["obj_file"].c_str();
			}
			obj_proto.push_back(obj_data(row["obj_type"].as<int16_t>(),obj_file));
			auto& proto = obj_proto.back();
			index_ref->vnum = proto.item_number = row["obj_item_number"].as<obj_vnum>();
			//!proposed lmdb code:
			auto aff_rows = db_get_by_meta("affected_type","aff_fk_id",row["obj_item_number"]);
			for(unsigned i = 0; i < MAX_OBJ_AFFECT; i++) {
				proto.affected[i].location = 0;
				proto.affected[i].modifier = 0;
			}

			proto.carried_by = proto.worn_by = nullptr;
			proto.next_content = nullptr;
			proto.contains = nullptr;
			proto.in_obj = nullptr;
			proto.worn_by = nullptr;
			proto.carried_by = nullptr;
			mods::globals::obj_stat_pages[
			index_ref->vnum
			] = proto.generate_stat_page();
		}
	} else {
		log("[notice] no objects from sql");
	}
	return 0;
}
/* load the zones */
std::tuple<int16_t,std::string> parse_sql_zones() {
	top_of_zone_table = 0;
	zone_table.clear();
	log("[status] Loading sql zones");

	for(const auto& row: db_get_all("zone")) {
#ifdef __MENTOC_SHOW_ZONE_DB_CPP__
		std::cerr << green_str("parsing zone:") << row["zone_name"].c_str() << "'\n";
#endif
		zone_data z;
		//struct zone_data {
		//  1    char *name;          /* name of this zone                  */
		//  2    int  lifespan;           /* how long between resets (minutes)  */
		//  3    int  age;                /* current age of this zone (minutes) */
		//  4    room_vnum bot;           /* starting room number for this zone */
		//  5    room_vnum top;           /* upper limit for rooms in this zone */
		//  6
		//  7    int  reset_mode;         /* conditions for reset (see below)   */
		//  8    zone_vnum number;        /* virtual number of this zone    */
		//  9    struct reset_com *cmd;   /* command table for reset            */
		// 10
		// 11    /*
		// 12     * Reset mode:
		// 13     *   0: Don't reset, and don't update age.
		// 14     *   1: Reset if no PC's are located in zone.
		// 15     *   2: Just reset.
		// 16     */
		// 17 };
		// 18
		//siege=# \d zone
		// id         | integer               |           | not null | nextval('zone_id_seq'::regclass)
		// zone_start | integer               |           | not null |
		// zone_end   | integer               |           | not null |
		// zone_name  | character varying(64) |           | not null |
		// lifespan   | integer               |           | not null |
		// reset_mode | integer               |           | not null |
		//
		//siege=#
		z.name.assign(row["zone_name"].c_str());
		z.lifespan = mods::util::stoi<int>(row["lifespan"]);
		z.age = 0;
		z.bot =mods::util::stoi<int>(row["zone_start"]);
		z.top =mods::util::stoi<int>(row["zone_end"]);
		z.reset_mode =mods::util::stoi<int>(row["reset_mode"]);
		/** WRONG */
		z.number =mods::util::stoi<int>(row["zone_virtual_number"]);
		z.set_id(mods::util::stoi<int>(row["id"]));
		//struct reset_com {
		//  1    char command;   /* current command                      */
		//  2
		//  3    bool if_flag;    /* if TRUE: exe only if preceding exe'd */
		//  4    int  arg1;       /*                                      */
		//  5    int  arg2;       /* Arguments to the command             */
		//  6    int  arg3;       /*                                      */
		//  7    int line;        /* line number this command appears on  */
		//  8
		//  9    /*
		// 10     *  Commands:              *
		// 11     *  'M': Read a mobile     *
		// 12     *  'O': Read an object    *
		// 13     *  'G': Give obj to mob   *
		// 14     *  'P': Put obj in obj    *
		// 15     *  'G': Obj to char       *
		// 16     *  'E': Obj to char equip *
		// 17     *  'D': Set state of door *
		// 18    */
		// 19 };
		// 20

		//siege=# \d zone_data
		// id           | integer              |           | not null | nextval('zone_data_id_seq'::regclass)
		// zone_id      | integer              |           | not null |
		// zone_command | character varying(1) |           | not null |
		// zone_if_flag | character varying(1) |           | not null |
		// zone_arg1    | integer              |           | not null |
		// zone_arg2    | integer              |           | not null |
		// zone_arg3    | integer              |           | not null |
		//
		//siege=#

		for(auto&& zone_data_row : db_get_by_meta("zone_data","zone_id",std::to_string(z.number))) {
#ifdef __MENTOC_SHOW_ZONE_DB_CPP__
			std::cerr << green_str("parsing zone_data from db") << "\n";
#endif
			reset_com res;
			res.command = std::string(zone_data_row["zone_command"].c_str())[0];
			res.if_flag =mods::util::stoi<int>(zone_data_row["zone_if_flag"]);
			res.arg1 =mods::util::stoi<int>(zone_data_row["zone_arg1"]);
			res.arg2 =mods::util::stoi<int>(zone_data_row["zone_arg2"]);
			res.arg3 =mods::util::stoi<int>(zone_data_row["zone_arg3"]);
			res.yaml = zone_data_row["zone_yaml"].is_null() ? "" : zone_data_row["zone_yaml"].c_str();
			res.line = 0; //TODO: mods::util::stoi<int>(zone_data_row["line"]);
			res.count = 0;
			z.cmd.push_back(res);
		}
		zone_table.emplace_back(z);
		top_of_zone_table = zone_table.size();
		std::cerr << "zone_table.back().get_id: " << zone_table.back().get_id() << "\n";
		std::cerr << "zone_table.size() " << zone_table.size() << "\n";

		log("DEBUG: parse_sql_zones: '%s' loaded",z.name.c_str());
	}

	return {zone_table.size(),"ok"};
}
void crawl_block(room_data* in_room) {
	static std::vector<room_vnum> crawled;
	if(!in_room) {
		return;
	}
	if(std::find(crawled.begin(),crawled.end(),in_room->number) != crawled.end()) {
		return;
	} else {
		crawled.emplace_back(in_room->number);
	}
	for(unsigned direction = 0; direction < NUM_OF_DIRS; direction++) {
		if(!in_room->dir_option[direction]) {
			continue;
		}
		if(world.size() <= in_room->dir_option[direction]->to_room) {
			continue;
		}
		auto& room = world[in_room->dir_option[direction]->to_room];
		switch(direction) {
			case NORTH:
				room.y = in_room->y + 1;
				room.x = in_room->x;
				room.z = in_room->z;
				break;
			case SOUTH:
				room.y = in_room->y - 1;
				room.x = in_room->x;
				room.z = in_room->z;
				break;
			case EAST:
				room.y = in_room->y;
				room.x = in_room->x + 1;
				room.z = in_room->z;
				break;
			case WEST:
				room.y = in_room->y;
				room.x = in_room->x - 1;
				room.z = in_room->z;
				break;
			case UP:
				room.y = in_room->y;
				room.x = in_room->x;
				room.z = in_room->z + 1;
				break;
			case DOWN:
				room.y = in_room->y;
				room.x = in_room->x;
				room.z = in_room->z - 1;
				break;
			default:
				break;
		}
		mods::globals::register_room_at_coordinates(room.x, room.y, room.z, room.number);
		crawl_block(&world[in_room->dir_option[direction]->to_room]);
	}
}
void set_coordinates() {
	auto start = real_room(mods::world_conf::get_mortal_start_room_vnum());
	auto& room = world[start];
	room.x = 0;
	room.y = 0;
	room.z = 0;
	mods::globals::register_room_at_coordinates(room.x, room.y, room.z, room.number);
	crawl_block(&room);
}
/* load the rooms */
std::tuple<int16_t,std::string> parse_sql_rooms() {
	top_of_world = 0;
	const auto& frozen_room_vnum = mods::world_conf::get_frozen_room_vnum();
	mods::rooms::create_room({
		{"name","Frozen"},
		{"description","It's cold. You can't do anything."},
		{"room_number",std::to_string(frozen_room_vnum)},
		/** these are the default values..
		{"zone","0"},
		{"sector_type","0"},
		{"light","0"},
		*/
	});

	mods::pq::result room_records,room_description_data_records;
	try {
		//siege=# \d room
		// id             | integer                |           | not null | nextval('room_id_seq'::regclass)
		// room_number    | integer                |           | not null |
		// zone           | integer                |           | not null |
		// sector_type    | integer                |           | not null |
		// name           | character varying(256) |           | not null |
		// description    | text                   |           | not null |
		// ex_keyword     | character varying(256) |           |          |
		// ex_description | text                   |           |          |
		// light          | integer                |           |          |
		// room_flag      | integer                |           | not null |
		//
		/**
		 * TODO: db_get_all() is a perfect candidate for coroutines (generators)
		 */
		auto room_records = db_get_all("room");
		log("parse_sql_rooms[result.size()]->%d",room_records.size());
		if(room_records.size() == 0) {
			return {0,"warning: no room_records fetched from postgres"};
		}

		if(room_records.size() == 0) {
			return {0,"warning: no room_records fetched from postgres"};
		}
		for(auto&& room_records_row: room_records) {
			try {
				room_data room;
				/**
				 * TODO: someday, we can have indexed strings with no duplicates
				 *
				room.name.allow_duplicates(false);
				*/
				room.name.assign(room_records_row["name"]);
				room.description.assign(room_records_row["description"]);
#ifdef __MENTOC_SHOW_PARSE_SQL_ROOMS_DEBUG__
				log("DEBUG: room: %d name: (%s), description: (%s)",mods::util::stoi<int>(room_records_row["id"].c_str()),room.name.c_str(),room.description.c_str());
#endif
				room.number = room_records_row["room_number"].as<int>();
#ifdef __MENTOC_SHOW_PARSE_SQL_ROOMS_DEBUG__
				log("parse_sql_rooms: room.number (%d)",room.number);
#endif
				room.zone = real_zone(room_records_row["zone"].as<int>());
				room.sector_type = room_records_row["sector_type"].as<int>();
				room.light = (room_records_row["light"]).as<int>();

				world.emplace_back(room);
				mods::globals::register_room(world.size());
				mods::rooms::set_sector_type(world.size()-1,room_records_row["sector_type"].as<int>());
				if(!room_records_row["textures"].is_null()) {
					mods::rooms::set_room_textures(world.size()-1,room_records_row["textures"].c_str());
				}
				mods::rooms::set_flag_absolute(world.size()-1,room_records_row["room_flag"].as<int>(0));
				top_of_world = world.size();
				if(std::string(room_records_row["nickname"].c_str()).length()) {
					mods::rooms::register_nickname(top_of_world,room_records_row["nickname"].c_str());
					world[top_of_world-1].nickname.assign(room_records_row["nickname"].c_str());
				}
				mods::zone::new_room(&room);
				mods::loot::new_room(&room);
			} catch(std::exception& e) {
				REPORT_DB_ISSUE("SYSERR: exception select from rooms db: ",e.what());
			}
		}
		log("parse_sql_rooms: world.size(): %d",world.size());

		for(auto&& row2: db_get_all("room_direction_data")) {
			//siege=# \d room_direction_data
			// id                  | integer                |           | not null | nextval('room_direction_data_id_seq'::regclass)
			// room_number         | integer                |           | not null |
			// exit_direction      | integer                |           | not null |
			// general_description | character varying(256) |           | not null |
			// keyword             | character varying(16)  |           |          |
			// exit_info           | integer                |           |          |
			// exit_key            | integer                |           |          |
			// to_room             | integer                |           | not null |
			//
			//siege=#
			auto direction = row2["exit_direction"].as<int>();
			if(direction >= NUM_OF_DIRS) {
				log("SYSERR: Invalid direction: (%d) Max number of Directions: (%d)",direction,NUM_OF_DIRS);
				continue;
			}
			room_rnum real_room_number = real_room((row2["room_number"]).as<int>());
			if(real_room_number == NOWHERE) {
				log("Invalid real_room_number: %d",(row2["room_number"]).as<int>());
				continue;
			}
			std::string gen_desc = row2["general_description"].c_str();
			std::string keyword = row2["keyword"].c_str();
			room_rnum to_room = real_room(row2["to_room"].as<int>());
			world[real_room_number].set_dir_option(direction,gen_desc,keyword,row2["exit_info"].as<int>(),row2["exit_key"].as<int>(),to_room);
		}
		for(auto&& row: db_get_all("room_extra_descriptions")) {
			try {
				room_rnum room_id = real_room(row["red_room_vnum"].as<int>());
				world[room_id].ex_descriptions().emplace_back(row["red_keyword"].c_str(),row["red_description"].c_str());
			} catch(std::exception& red_except) {
				REPORT_DB_ISSUE("error fetching room_extra_descriptions row",red_except.what());
				continue;
			}
		}
	} catch(std::exception& e) {
		REPORT_DB_ISSUE("error selecting room from dd",e.what());
		return {-1,std::string("An exception occured: ") + e.what()};
	}
	set_coordinates();
	return {world.size(),"okay"};
}

int16_t install_shop(shop_ptr_t& shop) {
	for(const auto& s_room_vnum : shop->room_info.rooms) {
		auto room = real_room(s_room_vnum);
		if(room == NOWHERE) {
			log("install_shop skipping room vnum: (%d)", s_room_vnum);
			continue;
		}
#ifdef __MENTOC_SHOW_INSTALL_SHOP_DEBUG__
		log("Installing shop title: %s: %s", shop->title.c_str(), shop->description.c_str());
#endif
		world[room].name.assign(shop->title.c_str());
		world[room].description.assign(shop->description.c_str());
		mods::globals::room_shopmap[s_room_vnum] = shop;
		world[room].shop_vnum = shop->vnum;
	}
	return 1;
}

int parse_sql_shops() {
	auto result = db_get_all("shops");

	if(result.size()) {
		shop_proto.resize(result.size());
		for(const auto& current_row : result) {
			shop_proto.emplace_back();
			shop_proto.back().feed(current_row);
			auto shop_ptr = std::make_shared<shop_data_t>();
			shop_ptr->feed(current_row);
			install_shop(shop_ptr);
			shop_list.emplace_back(std::move(shop_ptr));
		}
	} else {
		log("[notice] no shops from sql");
	}

	return 0;
}

/* make sure the start rooms exist & resolve their vnums to rnums */
void check_start_rooms(void) {
	auto real_rooms = mods::world_conf::load_all();

	if((r_idle_room = real_rooms[1]) == NOWHERE) {
		log("SYSERR:  Warning: IDLE room does not exist.  Change in config.c.");
		if(world.size() > 0) {
			log("defaulting to 0");
			r_frozen_start_room = 0;
		} else {
			log("defaulting to r_mortal_start_room");
			r_frozen_start_room = r_mortal_start_room;
		}
	}
	if((r_mortal_start_room = real_rooms[3]) == NOWHERE) {
		log("SYSERR:  Mortal start room does not exist.  Change in config.c.");
		if(world.size() > 0) {
			log("defaulting to 0");
			r_mortal_start_room = 0;
		}
	}

	if((r_immort_start_room = real_rooms[2]) == NOWHERE) {
		log("SYSERR:  Warning: Immort start room does not exist.  Change in config.c.");
		if(world.size() > 0) {
			log("defaulting to 0");
			r_immort_start_room = 0;
		} else {
			log("defaulting to r_mortal_start_room");
			r_immort_start_room = r_mortal_start_room;
		}
	}

	if((r_frozen_start_room = real_rooms[0]) == NOWHERE) {
		log("SYSERR:  Warning: Frozen start room does not exist.  Change in config.c.");
		if(world.size() > 0) {
			log("defaulting to 0");
			r_frozen_start_room = 0;
		} else {
			log("defaulting to r_mortal_start_room");
			r_frozen_start_room = r_mortal_start_room;
		}
	}
}


/* resolve all vnums into rnums in the world */
void renum_world(void) {
	unsigned room, door;

	for(room = 0; room < top_of_world; room++)
		for(door = 0; door < NUM_OF_DIRS; door++)
			if(world[room].dir_option[door])
				if(world[room].dir_option[door]->to_room != NOWHERE)
					world[room].dir_option[door]->to_room =
					    real_room(world[room].dir_option[door]->to_room);
}



/*
 * "resulve vnums into rnums in the zone reset tables"
 *
 * Or in English: Once all of the zone reset tables have been loaded, we
 * resolve the virtual numbers into real numbers all at once so we don't have
 * to do it repeatedly while the game is running.  This does make adding any
 * room, mobile, or object a little more difficult while the game is running.
 *
 * NOTE 1: Assumes NOWHERE == NOBODY == NOTHING.
 * NOTE 2: Assumes sizeof(room_rnum) >= (sizeof(mob_rnum) and sizeof(obj_rnum))
 */
namespace mods::zone {
	extern void renum_zone_table();
};
void renum_zone_table() {
	mods::zone::renum_zone_table();
}



void parse_simple_mob(FILE *mob_f, int i, int nr) {
#if 0
	int j, t[10];
	char line[READ_SIZE];

	mob_proto[i].real_abils.str = 11;
	mob_proto[i].real_abils.intel = 11;
	mob_proto[i].real_abils.wis = 11;
	mob_proto[i].real_abils.dex = 11;
	mob_proto[i].real_abils.con = 11;
	mob_proto[i].real_abils.cha = 11;

	if(!get_line(mob_f, line)) {
		log("SYSERR: Format error in mob #%d, file ended after S flag!", nr);
		exit(1);
	}

	if(sscanf(line, " %d %d %d %dd%d+%d %dd%d+%d ",
	          t, t + 1, t + 2, t + 3, t + 4, t + 5, t + 6, t + 7, t + 8) != 9) {
		log("SYSERR: Format error in mob #%d, first line after S flag\n"
		    "...expecting line of form '# # # #d#+# #d#+#'", nr);
		exit(1);
	}

	GET_LEVEL(mob_proto + i) = t[0];
	GET_HITROLL(mob_proto + i) = 20 - t[1];
	GET_AC(mob_proto + i) = 10 * t[2];

	/* max hit = 0 is a flag that H, M, V is xdy+z */
	GET_MAX_HIT(mob_proto + i) = 0;
	GET_HIT(mob_proto + i) = t[3];
	GET_MANA(mob_proto + i) = t[4];
	GET_MOVE(mob_proto + i) = t[5];

	GET_MAX_MANA(mob_proto + i) = 10;
	GET_MAX_MOVE(mob_proto + i) = 50;

	mob_proto[i].mob_specials.damnodice = t[6];
	mob_proto[i].mob_specials.damsizedice = t[7];
	GET_DAMROLL(mob_proto + i) = t[8];

	if(!get_line(mob_f, line)) {
		log("SYSERR: Format error in mob #%d, second line after S flag\n"
		    "...expecting line of form '# #', but file ended!", nr);
		exit(1);
	}

	if(sscanf(line, " %d %d ", t, t + 1) != 2) {
		log("SYSERR: Format error in mob #%d, second line after S flag\n"
		    "...expecting line of form '# #'", nr);
		exit(1);
	}

	GET_GOLD(mob_proto + i) = t[0];
	GET_EXP(mob_proto + i) = t[1];

	if(!get_line(mob_f, line)) {
		log("SYSERR: Format error in last line of mob #%d\n"
		    "...expecting line of form '# # #', but file ended!", nr);
		exit(1);
	}

	if(sscanf(line, " %d %d %d ", t, t + 1, t + 2) != 3) {
		log("SYSERR: Format error in last line of mob #%d\n"
		    "...expecting line of form '# # #'", nr);
		exit(1);
	}

	GET_POS(mob_proto + i) = t[0];
	GET_DEFAULT_POS(mob_proto + i) = t[1];
	GET_SEX(mob_proto + i) = t[2];

	GET_CLASS(mob_proto + i) = 0;
	GET_WEIGHT(mob_proto + i) = 200;
	GET_HEIGHT(mob_proto + i) = 198;

	/*
	 * these are now save applies; base save numbers for MOBs are now from
	 * the warrior save table.
	 */
	for(j = 0; j < 5; j++) {
		GET_SAVE(mob_proto + i, j) = 0;
	}

#endif
}


/*
 * interpret_espec is the function that takes espec keywords and values
 * and assigns the correct value to the mob as appropriate.  Adding new
 * e-specs is absurdly easy -- just add a new CASE statement to this
 * function!  No other changes need to be made anywhere in the code.
 *
 * CASE		: Requires a parameter through 'value'.
 * BOOL_CASE	: Being specified at all is its value.
 */

#define CASE(test)	\
	if (value && !matched && !str_cmp(keyword, test) && (matched = TRUE))

#define BOOL_CASE(test)	\
	if (!value && !matched && !str_cmp(keyword, test) && (matched = TRUE))

#define RANGE(low, high)	\
	(num_arg = MAX((low), MIN((high), (num_arg))))

void interpret_espec(const char *keyword, const char *value, int i, int nr) {
	int num_arg = 0, matched = FALSE;

	/*
	 * If there isn't a colon, there is no value.  While Boolean options are
	 * possible, we don't actually have any.  Feel free to make some.
	 */
	if(value) {
		num_arg = atoi(value);
	}

	CASE("BareHandAttack") {
		RANGE(0, 99);
		mob_proto[i].mob_specials.attack_type = num_arg;
	}

	CASE("Str") {
		RANGE(3, 25);
		mob_proto[i].real_abils.str = num_arg;
	}

	CASE("StrAdd") {
		RANGE(0, 100);
		mob_proto[i].real_abils.str_add = num_arg;
	}

	CASE("Int") {
		RANGE(3, 25);
		mob_proto[i].real_abils.intel = num_arg;
	}

	CASE("Wis") {
		RANGE(3, 25);
		mob_proto[i].real_abils.wis = num_arg;
	}

	CASE("Dex") {
		RANGE(3, 25);
		mob_proto[i].real_abils.dex = num_arg;
	}

	CASE("Con") {
		RANGE(3, 25);
		mob_proto[i].real_abils.con = num_arg;
	}

	CASE("Cha") {
		RANGE(3, 25);
		mob_proto[i].real_abils.cha = num_arg;
	}

	if(!matched) {
		log("SYSERR: Warning: unrecognized espec keyword %s in mob #%d",
		    keyword, nr);
	}
}

#undef CASE
#undef BOOL_CASE
#undef RANGE

void parse_espec(char *buf, int i, int nr) {
	char *ptr;

	if((ptr = strchr(buf, ':')) != NULL) {
		*(ptr++) = '\0';

		while(isspace(*ptr)) {
			ptr++;
		}
	}

	interpret_espec(buf, ptr, i, nr);
}


void parse_enhanced_mob(FILE *mob_f, int i, int nr) {
	char line[READ_SIZE];

	parse_simple_mob(mob_f, i, nr);

	while(get_line(mob_f, line)) {
		if(!strcmp(line, "E")) {	/* end of the enhanced section */
			return;
		} else if(*line == '#') {	/* we've hit the next mob, maybe? */
			log("SYSERR: Unterminated E section in mob #%d", nr);
			exit(1);
		} else {
			parse_espec(line, i, nr);
		}
	}

	log("SYSERR: Unexpected end of file reached after mob #%d", nr);
	exit(1);
}


void parse_mobile(FILE *mob_f, int nr) {
	log("[DEPRECATED] parse_mobile");
	return;
}




/* read all objects from obj file; generate index and prototypes */
char *parse_object(FILE *obj_f, int nr) {
	log("[DEPRECATED] parse_object");
	return nullptr;
}


#define Z	zone_table[zone]

/* load the zone table and command tables */
void load_zones(FILE *fl, char *zonename) {
	log("!DEPRECATED!: Call to load_zones");
	return;
}

#undef Z


void get_one_line(FILE *fl, char *buf) {
	if(fgets(buf, READ_SIZE, fl) == NULL) {
		log("SYSERR: error reading help file: not terminated with $?");
		exit(1);
	}

	buf[strlen(buf) - 1] = '\0'; /* take off the trailing \n */
}


void free_help(void) {
	int hp;

	if(!help_table) {
		return;
	}

	for(hp = 0; hp <= top_of_helpt; hp++) {
		if(help_table[hp].keyword) {
			free(help_table[hp].keyword);
		}

		if(help_table[hp].entry && !help_table[hp].duplicate) {
			free(help_table[hp].entry);
		}
	}

	free(help_table);
	help_table = NULL;
	top_of_helpt = 0;
}


void load_help(FILE *fl) {
#if defined(CIRCLE_MACINTOSH)
	static char key[READ_SIZE + 1], next_key[READ_SIZE + 1], entry[32384]; /* too big for stack? */
#else
	char key[READ_SIZE + 1], next_key[READ_SIZE + 1], entry[32384];
#endif
	size_t entrylen;
	char line[READ_SIZE + 1], *scan;
	struct help_index_element el;

	/* get the first keyword line */
	get_one_line(fl, key);

	while(*key != '$') {
		strcat(key, "\r\n");	/* strcat: OK (READ_SIZE - "\n" + "\r\n" == READ_SIZE + 1) */
		entrylen = strlcpy(entry, key, sizeof(entry));

		/* read in the corresponding help entry */
		get_one_line(fl, line);

		while(*line != '#' && entrylen < sizeof(entry) - 1) {
			entrylen += strlcpy(entry + entrylen, line, sizeof(entry) - entrylen);

			if(entrylen + 2 < sizeof(entry) - 1) {
				strcpy(entry + entrylen, "\r\n");	/* strcpy: OK (size checked above) */
				entrylen += 2;
			}

			get_one_line(fl, line);
		}

		if(entrylen >= sizeof(entry) - 1) {
			int keysize;
			const char *truncmsg = "\r\n*TRUNCATED*\r\n";

			strcpy(entry + sizeof(entry) - strlen(truncmsg) - 1, truncmsg);	/* strcpy: OK (assuming sane 'entry' size) */

			keysize = strlen(key) - 2;
			log("SYSERR: Help entry exceeded buffer space: %.*s", keysize, key);

			/* If we ran out of buffer space, eat the rest of the entry. */
			while(*line != '#') {
				get_one_line(fl, line);
			}
		}

		/* now, add the entry to the index with each keyword on the keyword line */
		el.duplicate = 0;
		el.entry = strdup(entry);
		scan = one_word(key, next_key);

		while(*next_key) {
			el.keyword = strdup(next_key);
			help_table[top_of_helpt++] = el;
			el.duplicate++;
			scan = one_word(scan, next_key);
		}

		/* get next keyword line (or $) */
		get_one_line(fl, key);
	}
}


int hsort(const void *a, const void *b) {
	const struct help_index_element *a1, *b1;

	a1 = (const struct help_index_element *) a;
	b1 = (const struct help_index_element *) b;

	return (str_cmp(a1->keyword, b1->keyword));
}


/*************************************************************************
 *  procedures for resetting, both play-time and boot-time	 	 *
 *************************************************************************/


int vnum_mobile(char *searchname, char_data *ch) {
	unsigned int nr, found = 0;

	for(nr = 0; nr <= top_of_mobt; nr++)
		if(isname(searchname, mob_proto[nr].player.name)) {
			send_to_char(ch, "%3d. [%5d] %s", ++found, mob_index[nr].vnum, mob_proto[nr].player.short_descr.c_str());
		}

	return (found);
}



int vnum_object(char *searchname, char_data *ch) {
	MENTOC_PREAMBLE();
	unsigned int nr, found = 0;

	for(nr = 0; nr <= top_of_objt; nr++)
		if(isname(searchname, obj_proto[nr].name)) {
			player->sendln(
			    //"%3d. [%5d] %s\r\n", ++found, obj_index[nr].vnum, obj_proto[nr].short_description.c_str());
			    CAT(
			        ++found, ". [",obj_index[nr].vnum, "] ",obj_proto[nr].short_description.c_str())
			);
		}

	return (found);
}


/* create a new mobile from a prototype */
char_data *read_mobile(mob_vnum nr, int type) { /* and mob_rnum */
	return mods::globals::read_mobile(nr,type);
	/*
		 mob_rnum i;
		 char_data *mob;

		 if(type == VIRTUAL) {
		 if((i = real_mobile(nr)) == NOBODY) {
		 log("WARNING: Mobile vnum %d does not exist in database.", nr);
		 return (NULL);
		 }
		 } else {
		 i = nr;
		 }

		 CREATE(mob, char_data, 1);
		 clear_char(mob);	//commented out
	 *mob = mob_proto[i];
	 mob->next = character_list;
	 character_list = mob;

	 if(!mob->points.max_hit) {
	 mob->points.max_hit = dice(mob->points.hit, mob->points.mana) +
	 mob->points.move;
	 } else {
	 mob->points.max_hit = rand_number(mob->points.hit, mob->points.mana);
	 }

	 mob->points.hit = mob->points.max_hit;
	 mob->points.mana = mob->points.max_mana;
	 mob->points.move = mob->points.max_move;

	 mob->player.time.birth = time(0);
	 mob->player.time.played = 0;
	 mob->player.time.logon = time(0);

	 mob_index[i].number++;

	 return (mob);
	 */
}


obj_ptr_t blank_object() {
	obj_list.push_back(std::make_shared<obj_data>(ITEM_RIFLE,"g36c.yml"));
	mods::globals::register_object(obj_list.back());
	return obj_list.back();
}
obj_ptr_t create_object(std::string_view yaml) {
	int type = -1;
	std::string yaml_file = yaml.data();
	if(str::starts_with("rifle/",yaml_file)) {
		type = ITEM_RIFLE;
	}
	if(str::starts_with("explosive/",yaml_file)) {
		type = ITEM_EXPLOSIVE;
	}
	if(str::starts_with("armor/",yaml_file)) {
		type = ITEM_ARMOR;
	}
	if(str::starts_with("attachment/",yaml_file)) {
		type = ITEM_ATTACHMENT;
	}
	if(str::starts_with("consumable/",yaml_file)) {
		type = ITEM_CONSUMABLE;
	}
	if(str::starts_with("container/",yaml_file)) {
		type = ITEM_CONTAINER;
	}
	if(str::starts_with("drone/",yaml_file)) {
		type = ITEM_DRONE;
	}
	if(str::starts_with("gadget/",yaml_file)) {
		type = ITEM_GADGET;
	}
	if(str::starts_with("melee/",yaml_file)) {
		type = ITEM_MELEE;
	}
	if(str::starts_with("vehicle/",yaml_file)) {
		type = ITEM_VEHICLE;
	}
	if(type == -1) {
		log("Unable to figure out yaml type enum from file: '%s'",yaml_file.c_str());
		return nullptr;
	}
	std::vector<std::string> parts = mods::util::explode('/',yaml_file);
	if(parts.size() < 2) {
		log("Unrecognized format for yaml file (in create_object(str)): '%s'",yaml_file.c_str());
		return nullptr;
	}
	return create_object(type,parts[1]);
}
obj_ptr_t create_object_into(std::string_view file,obj_ptr_t& target_object) {
	auto created_object = create_object(file);
	if(created_object) {
		obj_to_obj(created_object,target_object);
	}
	return created_object;
}
/** check for rifle|pkid:N match */
obj_ptr_t create_object(int type,std::string yaml_file) {
#ifdef __MENTOC_SHOW_CREATE_OBJECT_OUTPUT__
	std::cerr << "create_object:'" << yaml_file << "'\n";
#endif
	if(yaml_file.find_first_of('|') != std::string::npos) {
		return mods::util::create_pkid_object(yaml_file);
	}
	std::string path = mods::util::compile_yaml_path_from_type_and_file(type,yaml_file);
	if(path.compare(mods::util::UNKNOWN_YAML_FILE) == 0) {
		auto error = CAT("SYSERR: create_object:: warning: unknown yaml file type+yaml_file:'",yaml_file,"', with type:'", type,"'");
		log(error.c_str());
		mods::object_utils::report_yaml_message(error);
		auto obj = blank_object();
		obj_list.push_back(std::move(obj));
		mods::globals::register_object(obj_list.back());
		return obj_list.back();
	}
	bool exists = mods::util::yaml_file_exists(path);
	if(!exists) {
		auto error = CAT("SYSERR: create_object:: warning: yaml file DOESNT EXIST:'",yaml_file,"', with type:'", type,"'");
		mods::object_utils::report_yaml_message(error);
		log(error.c_str());
		auto obj = blank_object();
		obj_list.push_back(std::move(obj));
		mods::globals::register_object(obj_list.back());
		return obj_list.back();
	}
	obj_list.push_back(std::make_shared<obj_data>(type,yaml_file));
	mods::globals::register_object(obj_list.back());
	auto item = obj_list.back();
	return obj_list.back();
}
std::tuple<bool,obj_ptr_t> create_object_opt(int type,std::string yaml_file) {
	auto obj = std::make_shared<obj_data>(type,yaml_file);
	if(!obj) {
		return {false,nullptr};
	}
	obj_list.push_back(std::move(obj));
	mods::globals::register_object(obj_list.back());
	return {true,obj_list.back()};
}

shop_ptr_t create_shop_from_index(std::size_t proto_index) {
	if(proto_index >= shop_proto.size()) {
		log("SYSERR: requesting to read shop number(%d) out of shop_proto.size(): (%d)",
		    proto_index, shop_proto.size());
		return nullptr;
	}
	shop_list.push_back(std::make_shared<shop_data_t>(shop_proto[proto_index]));
	mods::globals::register_shop<shop_ptr_t>(shop_list.back());
	//shop_index[proto_index].number++;
	return shop_list.back();
}


/* create a new object from a prototype */
obj_ptr_t create_object_from_index(std::size_t proto_index) {
	if(proto_index >= obj_proto.size()) {
		exit(4);
		log("SYSERR: requesting to read object number(%d) out of obj_proto.size(): (%d)",
		    proto_index, obj_proto.size());
		return nullptr;
	}
	/*
	for(auto & p : obj_proto){
		std::cerr << "[obj_proto dump]: type: '" << p.type << "' feed_file: '" << p.feed_file() << "'\n";
	}
	*/
	obj_list.push_back(std::make_shared<obj_data>(obj_proto[proto_index].type,obj_proto[proto_index].feed_file()));
	mods::globals::register_object(obj_list.back());
	obj_index[proto_index].number++;
	return obj_list.back();
}

/* create a new object from a prototype */
struct obj_data *read_object(obj_vnum nr, int type) { /* and obj_rnum */
	obj_rnum i = type == VIRTUAL ? real_object(nr) : nr;

	std::size_t ii = i;

	if(i == NOTHING || ii >= obj_proto.size()) {
		log("Object (%c) %d does not exist in database.", type == VIRTUAL ? 'V' : 'R', nr);
		return (NULL);
	}
	return create_object_from_index(i).get();
}

/* create a new object from a prototype */
obj_ptr_t read_object_ptr(obj_vnum nr, int type) { /* and obj_rnum */
	obj_rnum i = type == VIRTUAL ? real_object(nr) : nr;

	std::size_t ii = i;

	if(i == NOTHING || ii >= obj_proto.size()) {
		log("Object (%c) %d does not exist in database.", type == VIRTUAL ? 'V' : 'R', nr);
		return (NULL);
	}
	return create_object_from_index(i);
}



/* update zone ages, queue for reset if necessary, and dequeue when possible */
void zone_update() {
#ifdef __MENTOC_SHOW_DEPRECATED__
	std::cerr << red_str("DEPRECATED: legacy zone_update(). Returning...") << "\n";
#endif
	return;
}

void reset_zone(zone_rnum zone) {
	mods::zone::reset_zone(zone);
	return;
}



/* for use in reset_zone; return TRUE if zone 'nr' is free of PC's  */
int is_empty(zone_rnum zone_nr) {
	for(auto& i : descriptor_list) {
		if(STATE(i) != CON_PLAYING) {
			continue;
		}

		if(IN_ROOM(i.character) == NOWHERE) {
			continue;
		}

		if(GET_LEVEL(i.character) >= LVL_IMMORT) {
			continue;
		}

		if(world[IN_ROOM(i.character)].zone != zone_nr) {
			continue;
		}

		return (0);
	}

	return (1);
}





/*************************************************************************
 *  stuff related to the save/load player system				 *
 *************************************************************************/


long get_ptable_by_name(const char *name) {
	int i;

	for(i = 0; i <= top_of_p_table; i++)
		if(static_cast<std::string>(
		            player_table[i].name).compare(name) == 0) {
			return (i);
		}

	return (-1);
}


long get_id_by_name(const char *name) {
	int i;

	for(i = 0; i <= top_of_p_table; i++)
		if(!str_cmp(player_table[i].name, name)) {
			return (player_table[i].id);
		}

	return (-1);
}


char *get_name_by_id(long id) {
	int i;

	for(i = 0; i <= top_of_p_table; i++)
		if(player_table[i].id == id) {
			return (player_table[i].name.ptr());
		}

	return (NULL);
}

bool char_exists(const std::string& name, aligned_int_t& meta_int_id) {
	std::cout << "debug: char_exists param: '" << name << "'\n";
	meta_int_id = 0;
	mutable_map_t where,row;
	where["player_name"] = name;
	mods::db::load_record_by_meta("player",&where,row);
	mods::util::maps::dump<std::string,std::string>(row);
	if(row.size()) {
		std::cout << "debug: found record for player\n";
		return true;
	}
	return false;
}

bool char_exists(player_ptr_t player_ptr) {
	return char_exists(player_ptr->name().c_str());
}
bool char_exists(const std::string& name) {
	aligned_int_t i;
	return char_exists(name,i);
}

/*
 * write the vital data of a player to sql
 */
bool player_exists(player_ptr_t player_ptr) {
	return db_get_by_meta("player","player_name",player_ptr->name().c_str()).size();
}
bool login(std::string_view user_name,std::string_view password) {
	try {
		auto select_transaction = txn();
		sql_compositor comp("player",&select_transaction);
		auto room_sql = comp.select("id")
		                .from("player")
		                .where_crypt("player_password",password.data())
		                .op_and("player_name","=",user_name.data())
		                .sql();
		auto row = mods::pq::exec(select_transaction,room_sql.data());
		return row.size();
	} catch(std::exception& e) {
		REPORT_DB_ISSUE("login exception",e.what());
		return false;
	}
}
bool parse_sql_player(player_ptr_t player_ptr) {
	/** TODO: make sure sql injection is not possible here */
	mods::players::db_load::set_reporter_lambda([&player_ptr](int64_t code,std::string_view msg) {
		log("SYSERR: failed loading player's class [player name:'%s'] error code: %d, message: '%s'", player_ptr->name().c_str(), code, msg.data());
		REPORT_DB_ISSUE("failed loading player's class",CAT("failed loading player's class [player name:'",player_ptr->name(),"'] error code: ",code,", message: '",msg.data(),"'"));
	});
	for(auto&& row: db_get_by_meta("player","player_name",player_ptr->name().c_str())) {
		player_ptr->set_db_id(row["id"].as<int>());
		player_ptr->clear_all_affected();
		player_ptr->clear_all_affected_plr();
		player_ptr->set_name(row["player_name"].c_str());
		player_ptr->short_descr().assign((row["player_short_description"]));
		player_ptr->long_descr().assign((row["player_long_description"]));
		player_ptr->saved().act = mods::util::stoi<long>(row["player_action_bitvector"]);
		player_ptr->real_abils().str = mods::util::stoi<uint16_t>(row["player_ability_strength"]);
		player_ptr->real_abils().str_add = mods::util::stoi<uint16_t>(row["player_ability_strength_add"]);
		player_ptr->real_abils().intel = mods::util::stoi<uint16_t>(row["player_ability_intelligence"]);
		player_ptr->real_abils().wis = mods::util::stoi<uint16_t>(row["player_ability_wisdom"]);
		player_ptr->real_abils().dex = mods::util::stoi<uint16_t>(row["player_ability_dexterity"]);
		player_ptr->real_abils().con = mods::util::stoi<uint16_t>(row["player_ability_constitution"]);
		player_ptr->real_abils().cha = mods::util::stoi<uint16_t>(row["player_ability_charisma"]);
		player_ptr->saved().alignment = mods::util::stoi<int>(row["player_ability_alignment"]);

		/** hp, mana, move */
		player_ptr->max_hp() = mods::util::stoi<sh_int>(row["player_max_hitpoints"]);
		player_ptr->max_mana() = mods::util::stoi<sh_int>(row["player_max_mana"]);
		player_ptr->max_move() = mods::util::stoi<sh_int>(row["player_max_move"]);
		player_ptr->hp() = mods::util::stoi<sh_int>(row["player_hitpoints"]);
		player_ptr->mana() = mods::util::stoi<sh_int>(row["player_mana"]);
		player_ptr->move() = mods::util::stoi<sh_int>(row["player_move"]);

		/** gold, exp, sex */
		player_ptr->gold() = mods::util::stoi<int>(row["player_gold"]);
		player_ptr->exp() = mods::util::stoi<uint32_t>(row["player_exp"]);
		player_ptr->sex() = std::string(row["player_sex"].c_str()).compare("M") == 0 ? SEX_MALE : SEX_FEMALE;

		player_ptr->level() = mods::util::stoi<uint16_t>(row["player_level"]);


		/** damroll, hitroll, weight, height */
		player_ptr->damroll() = mods::util::stoi<sbyte>(row["player_damroll"]);
		player_ptr->hitroll() = mods::util::stoi<sbyte>(row["player_hitroll"]);
		player_ptr->weight() = mods::util::stoi<ubyte>(row["player_weight"]);
		player_ptr->height() = mods::util::stoi<ubyte>(row["player_height"]);
		player_ptr->title().assign((row["player_title"]));
		player_ptr->hometown() = mods::util::stoi<uint8_t>(row["player_hometown"]);
		player_ptr->clear_all_affected();
		player_ptr->clear_all_affected_plr();

#ifdef __MENTOC_PLR_DEBUG_OUTPUT__
#define __MENTOC_PLR(a) case a: std::cerr << "flag: " << #a << " is set\n"; break;
#else
#define __MENTOC_PLR
#endif

		//player_ptr->level() = level;
		if(strlen(row["player_affection_bitvector"].c_str()) > 0) {
			uint64_t aff = row["player_affection_bitvector"].as<uint64_t>(0);
			uint64_t shift = 1;
			for(unsigned i=0; i < 64; i++) {
				if(aff & shift) {
					switch(shift) {
							__MENTOC_PLR(AFF_BLIND);
							__MENTOC_PLR(AFF_INVISIBLE);
							__MENTOC_PLR(AFF_DETECT_ALIGN);
							__MENTOC_PLR(AFF_DETECT_INVIS);
							__MENTOC_PLR(AFF_DETECT_MAGIC);
							__MENTOC_PLR(AFF_SENSE_LIFE);
							__MENTOC_PLR(AFF_INTIMIDATED);
							__MENTOC_PLR(AFF_SANCTUARY);
							__MENTOC_PLR(AFF_GROUP);
							__MENTOC_PLR(AFF_CURSE);
							__MENTOC_PLR(AFF_INFRAVISION);
							__MENTOC_PLR(AFF_POISON);
							__MENTOC_PLR(AFF_PROTECT_EVIL);
							__MENTOC_PLR(AFF_PROTECT_GOOD);
							__MENTOC_PLR(AFF_SLEEP);
							__MENTOC_PLR(AFF_NOTRACK);
							__MENTOC_PLR(AFF_SNEAK);
							__MENTOC_PLR(AFF_HIDE);
							__MENTOC_PLR(AFF_CHARM);
						default:
#ifdef __MENTOC_SHOW_UNKNOWN_AFF__
							std::cerr << "unknown affected flag: " << shift << "\n";
#endif
							break;
					}
					player_ptr->affect(shift);
				}
				shift <<= 1;
			}
		}
		if(strlen(row["player_affection_plr_bitvector"].c_str()) > 0) {
			uint64_t aff = row["player_affection_plr_bitvector"].as<uint64_t>(0);
			uint64_t shift = 1;
			for(unsigned i=0; i < 64; i++) {
				if(aff & shift) {
					switch(shift) {
							__MENTOC_PLR(PLR_KILLER);
							__MENTOC_PLR(PLR_THIEF);
							__MENTOC_PLR(PLR_FROZEN);
							__MENTOC_PLR(PLR_DONTSET);
							__MENTOC_PLR(PLR_WRITING);
							__MENTOC_PLR(PLR_MAILING);
							__MENTOC_PLR(PLR_CRASH);
							__MENTOC_PLR(PLR_SITEOK);
							__MENTOC_PLR(PLR_NOSHOUT);
							__MENTOC_PLR(PLR_NOTITLE);
							__MENTOC_PLR(PLR_DELETED);
							__MENTOC_PLR(PLR_LOADROOM);
							__MENTOC_PLR(PLR_NOWIZLIST);
							__MENTOC_PLR(PLR_NODELETE);
							__MENTOC_PLR(PLR_INVSTART);
							__MENTOC_PLR(PLR_CRYO);
							__MENTOC_PLR(PLR_NOTDEADYET);
						default:
#ifdef __MENTOC_SHOW_UNKNOWN_AFF__
							std::cerr << "unknown affected_plr flag: " << shift << "\n";
#endif
							break;
					}
					player_ptr->affect(shift);
				}
				shift <<= 1;
			}
		}

		player_ptr->set_time_birth(mods::util::stoi<int>(row["player_birth"]));
		player_ptr->set_time_played(mods::util::stoi<int>(row["player_time_played"]));
		player_ptr->set_time_logon(time(0));
		player_ptr->set_prefs(mods::util::stoi<long>(row["player_preferences"]));
		mods::prefs::events::prefs_loaded_from_db(player_ptr);
		player_ptr->practice_sessions() = row["player_practice_sessions"].as<uint16_t>();
		str_map_t values;
		get_player_map(player_ptr->name(),"mute-channels", values);
		auto default_prefs = EXPLODE(EXTENDED_PREFERENCES_DEFAULTS(),'|');
		unsigned ctr = 0;
		for(auto& preference : EXPLODE(EXTENDED_PREFERENCES_DEFAULTS(),'|')) {
			if(default_prefs.size() <= ctr) {
				values[preference] = "0";
				++ctr;
				continue;
			}
			if(values.find(preference) == values.end()) {
				values[preference] = default_prefs[ctr];
			} else {
				values[preference] = "0";
			}
			++ctr;
		}
		mods::players::db_load::set_class(player_ptr, static_cast<player_class_t>(row["player_class"].as<int>()));
		mods::players::db_load::load_base_abilities(player_ptr);
		mods::players::db_load::sync_player_with_class_skills(player_ptr->db_id(), player_ptr->get_class_string().c_str());
		mods::players::db_load::load_aliases(player_ptr);
		return true;
	}
	return false;
}


/* release memory allocated for a char struct */
void free_char(char_data *ch) {
	log("DEPRECATED: free_char");

	while(ch->affected) {
		affect_remove(ch, ch->affected);
	}

	if(ch->has_desc) {
		ch->desc->character = nullptr;
	}
}


/* release memory allocated for an obj struct */
void free_obj(struct obj_data *obj) {
	unsigned int nr;

	if((nr = GET_OBJ_RNUM(obj)) == NOTHING) {
		if(obj->name) {
			obj->name.assign("");
		}

		if(obj->description) {
			obj->description.assign("");
		}

		if(obj->short_description) {
			obj->short_description.assign("");
		}

		if(obj->action_description) {
			obj->action_description.assign("");
		}

		if(obj->ex_description.size()) {
			obj->ex_description.clear();
		}
	} else {
		if(obj->name.length() && strcmp(obj->name.c_str(),obj_proto[nr].name.c_str()) != 0) {
			obj->name.assign("");
		}

		if(obj->description.length() && strcmp(obj->description.c_str(),obj_proto[nr].description.c_str()) != 0) {
			obj->description.assign("");
		}

		if(obj->short_description.length() && strcmp(obj->short_description.c_str(),obj_proto[nr].short_description.c_str()) != 0) {
			obj->short_description.assign("");
		}

		if(obj->action_description.length() && strcmp(obj->action_description.c_str(),obj_proto[nr].action_description.c_str()) != 0) {
			obj->action_description.assign("");
		}

		/** FIXME decipher and refactor */
		/**
			if(obj->ex_description  && strcmp(obj->ex_descriptionc_str(),obj_proto[nr].ex_description.c_str()) != 0){
			free_extra_descriptions(obj->ex_description);
			}
			*/
	}
}



/* clear some of the the working variables of a char */
void reset_char(char_data *ch) {
	int i;

	for(i = 0; i < NUM_WEARS; i++) {
		GET_EQ(ch, i) = nullptr;
	}

	ch->followers = nullptr;
	ch->master = nullptr;
	IN_ROOM(ch) = NOWHERE;
	ch->carrying = nullptr;
	ch->next = nullptr;
	ch->next_fighting = nullptr;
	FIGHTING(ch) = nullptr;
	ch->char_specials.position = POS_STANDING;
	ch->mob_specials.default_pos = POS_STANDING;
	ch->mob_specials.behaviour_tree = behaviour_tree::NONE;
	ch->char_specials.carry_weight = 0;
	ch->char_specials.carry_items = 0;

	if(GET_HIT(ch) <= 0) {
		GET_HIT(ch) = 1;
	}

	if(GET_MOVE(ch) <= 0) {
		GET_MOVE(ch) = 1;
	}

	if(GET_MANA(ch) <= 0) {
		GET_MANA(ch) = 1;
	}

	GET_LAST_TELL(ch) = NOBODY;
}



void clear_object(struct obj_data *obj) {
	memset((char *) obj, 0, sizeof(struct obj_data));

	obj->item_number = NOTHING;
	IN_ROOM(obj) = NOWHERE;
	obj->worn_on = NOWHERE;
}




/*
 * Called during character creation after picking character class
 * (and then never again for that character).
 */
void init_char(player_ptr_t player) {
	int i;

	/* *** if this is our first player --- he be God *** */
	if(top_of_p_table == 0) {
		d("first char. he is a god");
		player->set_imp_mode(true);
		player->set_god_mode(true);
		player->set_bui_mode(true);
		GET_EXP(player->cd()) = 7000000;

		/* The implementor never goes through do_start(). */
		GET_MAX_HIT(player->cd()) = 500;
		GET_MAX_MANA(player->cd()) = 100;
		GET_MAX_MOVE(player->cd()) = 82;
		GET_HIT(player->cd()) = GET_MAX_HIT(player->cd());
		GET_MANA(player->cd()) = GET_MAX_MANA(player->cd());
		GET_MOVE(player->cd()) = GET_MAX_MOVE(player->cd());
	}

	set_title(player, "");
	player->short_descr().clear();
	player->long_descr().clear();
	player->description().clear();

	player->set_time_birth(time(0));
	player->set_time_logon(time(0));
	player->set_time_played(0);

	player->hometown() = 1;
	player->armor() = 100;

	for(i = 0; i < MAX_TONGUE; i++) {
		GET_TALK(player->cd(), i) = 0;
	}

	/*
	 * make favors for sex -- or in English, we bias the height and weight of the
	 * character depending on what gender they've chosen for themselves. While it
	 * is possible to have a tall, heavy female it's not as likely as a male.
	 *
	 * Height is in centimeters. Weight is in pounds.  The only place they're
	 * ever printed (in stock code) is SPELL_IDENTIFY.
	 */
	if(player->sex() == SEX_MALE) {
		player->weight() = rand_number(120, 180);
		player->height() = rand_number(160, 200); /* 5'4" - 6'8" */
	} else {
		player->weight() = rand_number(100, 160);
		player->height() = rand_number(150, 180); /* 5'0" - 6'0" */
	}

	/*
		 if((i = get_ptable_by_name(GET_NAME(player->cd()).c_str())) != -1) {
		 player_table[i].id = GET_IDNUM(player->cd()) = ++top_idnum;
		 } else {
		 log("SYSERR: init_char: Character '%s' not found in player table.", GET_NAME(player->cd()).c_str());
		 }*/

	for(i = 1; i <= MAX_SKILLS; i++) {
		if(player->level() < LVL_IMPL) {
			SET_SKILL(player->cd(), i, 0);
		} else {
			SET_SKILL(player->cd(), i, 100);
		}
	}

	player->clear_all_affected();

	for(i = 0; i < 5; i++) {
		player->saved().apply_saving_throw[i] = 0;
	}

	player->real_abils().intel = 25;
	player->real_abils().wis = 25;
	player->real_abils().dex = 25;
	player->real_abils().str = 25;
	player->real_abils().str_add = 100;
	player->real_abils().con = 25;
	player->real_abils().cha = 25;

	for(i = 0; i < 3; i++) {
		GET_COND(player->cd(), i) = (player->level() == LVL_IMPL ? -1 : 24);
	}

	GET_LOADROOM(player->cd()) = NOWHERE;
	player->cd()->drone = false;
	d("init_char [done]");
}



/* returns the real number of the room with given virtual number */
room_rnum real_room(room_vnum vnum) {
	for(unsigned i =0; i < world.size(); i++) {
		if(world[i].number == vnum) {
			return i;
		}
	}

	return NOWHERE;
}



/* returns the real number of the monster with given virtual number */
mob_rnum real_mobile(mob_vnum vnum) {
	static std::map<mob_vnum,mob_rnum> real_mobile_static_map;
	if(real_mobile_static_map.find(vnum) == real_mobile_static_map.end()) {
#ifdef __MENTOC_SHOW_PARSE_MOB_OUTPUT__
		log("real_mobile first lookup for vnum: %d", vnum);
#endif
		for(unsigned i=0; i < mob_proto.size(); i++) {
			if(mob_proto[i].nr == vnum) {
				real_mobile_static_map[vnum] = i;
				return i;
			}
		}
		real_mobile_static_map[vnum] = NOBODY;
	}
	return real_mobile_static_map[vnum];
}


/* returns the real number of the object with given virtual number */
obj_rnum real_object(obj_vnum vnum) {
	obj_rnum real_object_vnum_id = NOTHING;
	for(const auto& obj : obj_index) {
		if(obj.vnum == vnum) {
			return real_object_vnum_id;
		}
		++real_object_vnum_id;
	}
	return NOTHING;
}


/* returns the real number of the zone with given virtual number */
room_rnum real_zone(room_vnum vnum) {
	room_rnum real_zone_number = 0;

	for(const auto& zone : zone_table) {
		if(zone.number == vnum) {
			return real_zone_number;
		}

		real_zone_number++;
	}

	return NOWHERE;
}


/*
 * Extend later to include more checks.
 *
 * TODO: Add checks for unknown bitvectors.
 */
int check_object(struct obj_data *obj) {
	char objname[MAX_INPUT_LENGTH + 32];
	int error = FALSE;

	if(GET_OBJ_WEIGHT(obj) < 0 && (error = TRUE))
		log("SYSERR: Object #%d (%s) has negative weight (%d).",
		    GET_OBJ_VNUM(obj), obj->short_description, GET_OBJ_WEIGHT(obj));

	if(GET_OBJ_RENT(obj) < 0 && (error = TRUE))
		log("SYSERR: Object #%d (%s) has negative cost/day (%d).",
		    GET_OBJ_VNUM(obj), obj->short_description, GET_OBJ_RENT(obj));

	snprintf(objname, sizeof(objname), "Object #%d (%s)", GET_OBJ_VNUM(obj), obj->short_description.c_str());
	error |= check_bitvector_names(GET_OBJ_WEAR(obj), wear_bits_count, objname, "object wear");
	error |= check_bitvector_names(GET_OBJ_EXTRA(obj), extra_bits_count, objname, "object extra");
	error |= check_bitvector_names(GET_OBJ_AFFECT(obj), affected_bits_count, objname, "object affect");

	switch(GET_OBJ_TYPE(obj)) {
		case ITEM_DRINKCON: {
				char onealias[MAX_INPUT_LENGTH], *space = strrchr(obj->name, ' ');

				strlcpy(onealias, space ? space + 1 : obj->name.c_str(), sizeof(onealias));

				if(search_block(onealias, drinknames, TRUE) < 0 && (error = TRUE))
					log("SYSERR: Object #%d (%s) doesn't have drink type as last alias. (%s)",
					    GET_OBJ_VNUM(obj), obj->short_description.c_str(), obj->name.c_str());
			}

		/* Fall through. */
		case ITEM_FOUNTAIN:
			if(GET_OBJ_VAL(obj, 1) > GET_OBJ_VAL(obj, 0) && (error = TRUE))
				log("SYSERR: Object #%d (%s) contains (%d) more than maximum (%d).",
				    GET_OBJ_VNUM(obj), obj->short_description.c_str(),
				    GET_OBJ_VAL(obj, 1), GET_OBJ_VAL(obj, 0));

			break;

		case ITEM_SCROLL:
		case ITEM_POTION:
			error |= check_object_level(obj, 0);
			error |= check_object_spell_number(obj, 1);
			error |= check_object_spell_number(obj, 2);
			error |= check_object_spell_number(obj, 3);
			break;

		case ITEM_WAND:
		case ITEM_STAFF:
			error |= check_object_level(obj, 0);
			error |= check_object_spell_number(obj, 3);

			if(GET_OBJ_VAL(obj, 2) > GET_OBJ_VAL(obj, 1) && (error = TRUE))
				log("SYSERR: Object #%d (%s) has more charges (%d) than maximum (%d).",
				    GET_OBJ_VNUM(obj), obj->short_description,
				    GET_OBJ_VAL(obj, 2), GET_OBJ_VAL(obj, 1));

			break;
	}

	return (error);
}

int check_object_spell_number(struct obj_data *obj, int val) {
	int error = FALSE;
	const char *spellname;

	if(GET_OBJ_VAL(obj, val) == -1) {	/* i.e.: no spell */
		return (error);
	}

	/*
	 * Check for negative spells, spells beyond the top define, and any
	 * spell which is actually a skill.
	 */
	if(GET_OBJ_VAL(obj, val) < 0) {
		error = TRUE;
	}

	if(GET_OBJ_VAL(obj, val) > TOP_SPELL_DEFINE) {
		error = TRUE;
	}

	if(GET_OBJ_VAL(obj, val) > MAX_SPELLS && GET_OBJ_VAL(obj, val) <= MAX_SKILLS) {
		error = TRUE;
	}

	if(error)
		log("SYSERR: Object #%d (%s) has out of range spell #%d.",
		    GET_OBJ_VNUM(obj), obj->short_description, GET_OBJ_VAL(obj, val));

	/*
	 * This bug has been fixed, but if you don't like the special behavior...
	 */
#if 0

	if(GET_OBJ_TYPE(obj) == ITEM_STAFF &&
	        HAS_SPELL_ROUTINE(GET_OBJ_VAL(obj, val), MAG_AREAS | MAG_MASSES))
		log("... '%s' (#%d) uses %s spell '%s'.",
		    obj->short_description,	GET_OBJ_VNUM(obj),
		    HAS_SPELL_ROUTINE(GET_OBJ_VAL(obj, val), MAG_AREAS) ? "area" : "mass",
		    skill_name(GET_OBJ_VAL(obj, val)));

#endif

	if(scheck) {	/* Spell names don't exist in syntax check mode. */
		return (error);
	}

	/* Now check for unnamed spells. */
	spellname = skill_name(GET_OBJ_VAL(obj, val));

	if((spellname == unused_spellname || !str_cmp("UNDEFINED", spellname)) && (error = TRUE))
		log("SYSERR: Object #%d (%s) uses '%s' spell #%d.",
		    GET_OBJ_VNUM(obj), obj->short_description, spellname,
		    GET_OBJ_VAL(obj, val));

	return (error);
}

int check_object_level(struct obj_data *obj, int val) {
	int error = FALSE;

	if((GET_OBJ_VAL(obj, val) < 0 || GET_OBJ_VAL(obj, val) > LVL_IMPL) && (error = TRUE))
		log("SYSERR: Object #%d (%s) has out of range level #%d.",
		    GET_OBJ_VNUM(obj), obj->short_description, GET_OBJ_VAL(obj, val));

	return (error);
}

int check_bitvector_names(bitvector_t bits, size_t namecount, const char *whatami, const char *whatbits) {
	unsigned int flagnum;
	bool error = FALSE;

	/* See if any bits are set above the ones we know about. */
	if(bits <= (~(bitvector_t)0 >> (sizeof(bitvector_t) * 8 - namecount))) {
		return (FALSE);
	}

	for(flagnum = namecount; flagnum < sizeof(bitvector_t) * 8; flagnum++)
		if((1 << flagnum) & bits) {
			log("SYSERR: %s has unknown %s flag, bit %d (0 through %d known).", whatami, whatbits, flagnum, namecount - 1);
			error = TRUE;
		}

	return (error);
}

