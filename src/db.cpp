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
#include "mods/pq.hpp"
#include "mods/sql.hpp"
#include <vector>
#include <deque>
#include "mods/behaviour_tree_impl.hpp"
#include "mods/pq.hpp"
#include "mods/sql.hpp"
using behaviour_tree = mods::behaviour_tree_impl::node_wrapper;

/**************************************************************************
*  declarations of most of the 'global' variables                         *
**************************************************************************/
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
void parse_sql_rooms();
void parse_sql_zones();
int parse_sql_objects();
void parse_sql_mobiles();
std::vector<room_data> world;	/* array of rooms		 */
room_rnum top_of_world = 0;	/* ref to top element of world	 */

extern struct char_data *character_list;	/* global linked list of
						 * chars	 */
std::vector<index_data> mob_index;	/* index table for mobile file	 */
std::vector<char_data> mob_proto;	/* prototypes for mobs		 */
mob_rnum top_of_mobt = 0;	/* top of mobile index table	 */

std::deque<obj_data> object_list;	/* list of objs	 */
std::deque<char_data> mob_list;		/* list of mobs */
std::vector<index_data> obj_index;	/* index table for object file	 */
std::vector<obj_data> obj_proto;	/* prototypes for objs		 */
obj_rnum top_of_objt = 0;	/* top of object index table	 */

std::vector<zone_data> zone_table;	/* zone table			 */
zone_rnum top_of_zone_table = 0;/* top element of zone tab	 */
struct message_list fight_messages[MAX_MESSAGES];	/* fighting messages	 */

std::deque<player_index_element> player_table;	/* index to plr file	 */
FILE *player_fl = NULL;		/* file desc of player file	 */
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

char *credits = NULL;		/* game credits			 */
char *news = NULL;		/* mud news			 */
char *motd = NULL;		/* message of the day - mortals */
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
void setup_dir(FILE *fl, int room, int dir);
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
void assign_rooms(void);
void assign_the_shopkeepers(void);
void build_player_index(void);
int is_empty(zone_rnum zone_nr);
void reset_zone(zone_rnum zone);
int file_to_string(const char *name, char *buf);
int file_to_string_alloc(const char *name, char **buf);
void reboot_wizlists(void);
ACMD(do_reboot);
void boot_world(void);
int count_alias_records(FILE *fl);
int count_hash_records(FILE *fl);
bitvector_t asciiflag_conv(char *flag);
void parse_simple_mob(FILE *mob_f, int i, int nr);
void interpret_espec(const char *keyword, const char *value, int i, int nr);
void parse_espec(char *buf, int i, int nr);
void parse_enhanced_mob(FILE *mob_f, int i, int nr);
void get_one_line(FILE *fl, char *buf);
void save_etext(struct char_data *ch);
void check_start_rooms(void);
void renum_world(void);
void renum_zone_table(void);
void log_zone_error(zone_rnum zone, int cmd_no, const char *message);
void reset_time(void);
long get_ptable_by_name(const char *name);

/* external functions */
void paginate_string(char *str, struct descriptor_data *d);
struct time_info_data *mud_time_passed(time_t t2, time_t t1);
void free_alias(struct alias_data *a);
void load_messages(void);
void weather_and_time(int mode);
void mag_assign_spells(void);
void boot_social_messages(void);
void update_obj_file(void);	/* In objsave.c */
void sort_commands(void);
void sort_spells(void);
void load_banned(void);
void Read_Invalid_List(void);
void boot_the_shops(FILE *shop_f, char *filename, int rec_count);
int hsort(const void *a, const void *b);
void prune_crlf(char *txt);
void destroy_shops(void);

/* external vars */
extern int no_specials;
extern int scheck;
extern room_vnum mortal_start_room;
extern room_vnum immort_start_room;
extern room_vnum frozen_start_room;
extern std::deque<descriptor_data>  descriptor_list;
extern const char *unused_spellname;	/* spell_parser.c */

/*************************************************************************
*  routines for booting the system                                       *
*************************************************************************/

/* this is necessary for the autowiz system */
void reboot_wizlists(void) {
	file_to_string_alloc(WIZLIST_FILE, &wizlist);
	file_to_string_alloc(IMMLIST_FILE, &immlist);
}


/* Wipe out all the loaded text files, for shutting down. */
void free_text_files(void) {
	char **textfiles[] = {
		&wizlist, &immlist, &news, &credits, &motd, &imotd, &help, &info,
		&policies, &handbook, &background, NULL
	};
	int rf;

	for(rf = 0; textfiles[rf]; rf++)
		if(*textfiles[rf]) {
			free(*textfiles[rf]);
			*textfiles[rf] = NULL;
		}
}


/*
 * Too bad it doesn't check the return values to let the user
 * know about -1 values.  This will result in an 'Okay.' to a
 * 'reload' command even when the string was not replaced.
 * To fix later, if desired. -gg 6/24/99
 */
ACMD(do_reboot) {
	char arg[MAX_INPUT_LENGTH];

	one_argument(argument, arg);

	if(!str_cmp(arg, "all") || *arg == '*') {
		/*
		if(file_to_string_alloc(GREETINGS_FILE, &GREETINGS) == 0) {
			prune_crlf(GREETINGS);
		}*/

		file_to_string_alloc(WIZLIST_FILE, &wizlist);
		file_to_string_alloc(IMMLIST_FILE, &immlist);
		file_to_string_alloc(NEWS_FILE, &news);
		file_to_string_alloc(CREDITS_FILE, &credits);
		file_to_string_alloc(MOTD_FILE, &motd);
		file_to_string_alloc(IMOTD_FILE, &imotd);
		file_to_string_alloc(HELP_PAGE_FILE, &help);
		file_to_string_alloc(INFO_FILE, &info);
		file_to_string_alloc(POLICIES_FILE, &policies);
		file_to_string_alloc(HANDBOOK_FILE, &handbook);
		file_to_string_alloc(BACKGROUND_FILE, &background);
	} else if(!str_cmp(arg, "wizlist")) {
		file_to_string_alloc(WIZLIST_FILE, &wizlist);
	} else if(!str_cmp(arg, "immlist")) {
		file_to_string_alloc(IMMLIST_FILE, &immlist);
	} else if(!str_cmp(arg, "news")) {
		file_to_string_alloc(NEWS_FILE, &news);
	} else if(!str_cmp(arg, "credits")) {
		file_to_string_alloc(CREDITS_FILE, &credits);
	} else if(!str_cmp(arg, "motd")) {
		file_to_string_alloc(MOTD_FILE, &motd);
	} else if(!str_cmp(arg, "imotd")) {
		file_to_string_alloc(IMOTD_FILE, &imotd);
	} else if(!str_cmp(arg, "help")) {
		file_to_string_alloc(HELP_PAGE_FILE, &help);
	} else if(!str_cmp(arg, "info")) {
		file_to_string_alloc(INFO_FILE, &info);
	} else if(!str_cmp(arg, "policy")) {
		file_to_string_alloc(POLICIES_FILE, &policies);
	} else if(!str_cmp(arg, "handbook")) {
		file_to_string_alloc(HANDBOOK_FILE, &handbook);
	} else if(!str_cmp(arg, "background")) {
		file_to_string_alloc(BACKGROUND_FILE, &background);
	} else if(!str_cmp(arg, "greetings")) {
		/*
		if(file_to_string_alloc(GREETINGS_FILE, &GREETINGS) == 0) {
			prune_crlf(GREETINGS);
		}*/
	} else if(!str_cmp(arg, "xhelp")) {
		if(help_table) {
			free_help();
		}

		index_boot(DB_BOOT_HLP);
	} else {
		send_to_char(ch, "Unknown reload option.\r\n");
		return;
	}

	send_to_char(ch, "%s", OK);
}


void boot_world(void) {
	//log("Loading zone table.");
	//index_boot(DB_BOOT_ZON);
	log("Parsing sql zones.");
	parse_sql_zones();

	//log("Loading rooms.");
	//index_boot(DB_BOOT_WLD);


	log("Parsing sql rooms.");
	parse_sql_rooms();

	log("Checking start rooms.");
	check_start_rooms();

	log("Loading sql mobs and generating index.");
	parse_sql_mobiles();

	log("Loading sql objs and generating index.");
	//index_boot(DB_BOOT_OBJ);
	parse_sql_objects();

	log("Renumbering zone table.");
	renum_zone_table();

	if(!no_specials) {
		log("Loading shops.");
		index_boot(DB_BOOT_SHP);
	}
}


void free_extra_descriptions(struct extra_descr_data *edesc) {
	struct extra_descr_data *enext;

	for(; edesc; edesc = enext) {
		enext = edesc->next;

		free(edesc->keyword);
		free(edesc->description);
		free(edesc);
	}
}


/* Free the world, in a memory allocation sense. */
void destroy_db(void) {
	ssize_t cnt, itr;
	struct char_data *chtmp;

	/* Active Mobiles & Players */
	while(character_list) {
		chtmp = character_list;
		character_list = character_list->next;
		free_char(chtmp);
	}

	/* Active Objects */
	/* object_list frees itself thanks to destructors !mods */

	/* Rooms */
	for(cnt = 0; cnt <= top_of_world; cnt++) {
		free_extra_descriptions(world[cnt].ex_description);

		for(itr = 0; itr < NUM_OF_DIRS; itr++) {
			if(!world[cnt].dir_option[itr]) {
				continue;
			}

			/** TODO: make dir_option elements not crappy malloc'd :) */
			if(world[cnt].dir_option[itr]->general_description) {
			}

			free(world[cnt].dir_option[itr]);
		}
	}

	/* We don't need to free the world since it's a std::vector now !mods */
	//free(world);

	/* Objects */
	for(cnt = 0; cnt <= top_of_objt; cnt++) {
		if(obj_proto[cnt].name) {
			free(obj_proto[cnt].name);
		}

		if(obj_proto[cnt].description) {
			free(obj_proto[cnt].description);
		}

		if(obj_proto[cnt].short_description) {
			free(obj_proto[cnt].short_description);
		}

		if(obj_proto[cnt].action_description) {
			free(obj_proto[cnt].action_description);
		}

		free_extra_descriptions(obj_proto[cnt].ex_description);
	}

	/* Mobiles */
	for(cnt = 0; cnt <= top_of_mobt; cnt++) {
		while(mob_proto[cnt].affected) {
			affect_remove(&mob_proto[cnt], mob_proto[cnt].affected);
		}
	}

	/* Shops */
	destroy_shops();
}


/* body of the booting system */
void boot_db(void) {
	unsigned i;

	log("Boot db -- BEGIN.");

	log("Resetting the game time:");
	reset_time();

	log("Reading news, credits, help, bground, info & motds.");
	/*
	file_to_string_alloc(NEWS_FILE, &news);
	file_to_string_alloc(CREDITS_FILE, &credits);
	file_to_string_alloc(MOTD_FILE, &motd);
	file_to_string_alloc(IMOTD_FILE, &imotd);
	file_to_string_alloc(HELP_PAGE_FILE, &help);
	file_to_string_alloc(INFO_FILE, &info);
	file_to_string_alloc(WIZLIST_FILE, &wizlist);
	file_to_string_alloc(IMMLIST_FILE, &immlist);
	file_to_string_alloc(POLICIES_FILE, &policies);
	file_to_string_alloc(HANDBOOK_FILE, &handbook);
	file_to_string_alloc(BACKGROUND_FILE, &background);

	if(file_to_string_alloc(GREETINGS_FILE, &GREETINGS) == 0) {
		prune_crlf(GREETINGS);
	}
	*/

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
		log("   Rooms.");
		assign_rooms();
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

	log("Reading banned site and invalid-name list.");
	load_banned();
	Read_Invalid_List();

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

	reset_q.head = reset_q.tail = NULL;

	boot_time = time(0);

	log("Boot db -- DONE.");
	mods::globals::post_boot_db();
}


/* reset the time in the game from file */
void reset_time(void) {
	time_t beginning_of_time = 0;
	FILE *bgtime;

	if((bgtime = fopen(TIME_FILE, "r")) == NULL) {
		log("SYSERR: Can't read from '%s' time file.", TIME_FILE);
	} else {
		fscanf(bgtime, "%ld\n", &beginning_of_time);
		fclose(bgtime);
	}

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
	FILE *bgtime;

	if((bgtime = fopen(TIME_FILE, "w")) == NULL) {
		log("SYSERR: Can't write to '%s' time file.", TIME_FILE);
	} else {
		fprintf(bgtime, "%ld\n", mud_time_to_secs(when));
		fclose(bgtime);
	}
}


void free_player_index(void) {
	log("[deprecated] free_player_index");
	return;
}


/* generate index table for the player file */
void build_player_index(void) {
	int nr = -1;
	long size, recs;
	struct char_file_u dummy;

	if(!(player_fl = fopen(PLAYER_FILE, "r+b"))) {
		if(errno != ENOENT) {
			perror("SYSERR: fatal error opening playerfile");
			exit(1);
		} else {
			log("No playerfile.  Creating a new one.");
			touch(PLAYER_FILE);

			if(!(player_fl = fopen(PLAYER_FILE, "r+b"))) {
				perror("SYSERR: fatal error opening playerfile");
				exit(1);
			}
		}
	}

	fseek(player_fl, 0L, SEEK_END);
	size = ftell(player_fl);
	rewind(player_fl);

	if(size % sizeof(struct char_file_u)) {
		log("\aWARNING:  PLAYERFILE IS PROBABLY CORRUPT!");
	}

	recs = size / sizeof(struct char_file_u);

	if(recs) {
		log("   %ld players in database.", recs);
	} else {
		top_of_p_table = -1;
		return;
	}

	for(;;) {
		fread(&dummy, sizeof(struct char_file_u), 1, player_fl);

		if(feof(player_fl)) {
			break;
		}

		/* new record */
		nr++;
		player_table[nr].name.assign(dummy.name.c_str());

		player_table[nr].id = dummy.char_specials_saved.idnum;
		top_idnum = MAX(top_idnum, dummy.char_specials_saved.idnum);
	}

	top_of_p_table = nr;
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
	const char *index_filename, *prefix = NULL;	/* NULL or egcs 1.1 complains */
	FILE *db_index, *db_file;
	int rec_count = 0, size[2];
	char buf2[PATH_MAX], buf1[MAX_STRING_LENGTH];

	switch(mode) {
		case DB_BOOT_WLD:
			prefix = WLD_PREFIX;
			break;

		case DB_BOOT_MOB:
			prefix = MOB_PREFIX;
			break;

		case DB_BOOT_OBJ:
			prefix = OBJ_PREFIX;
			break;

		case DB_BOOT_ZON:
			prefix = ZON_PREFIX;
			break;

		case DB_BOOT_SHP:
			prefix = SHP_PREFIX;
			break;

		case DB_BOOT_HLP:
			prefix = HLP_PREFIX;
			break;

		default:
			log("SYSERR: Unknown subcommand %d to index_boot!", mode);
			exit(1);
	}

	if(mini_mud) {
		index_filename = MINDEX_FILE;
	} else {
		index_filename = INDEX_FILE;
	}

	snprintf(buf2, sizeof(buf2), "%s%s", prefix, index_filename);

	if(!(db_index = fopen(buf2, "r"))) {
		log("SYSERR: opening index file '%s': %s", buf2, strerror(errno));
		exit(1);
	}

	/* first, count the number of records in the file so we can malloc */
	fscanf(db_index, "%s\n", buf1);

	while(*buf1 != '$') {
		snprintf(buf2, sizeof(buf2), "%s%s", prefix, buf1);

		if(!(db_file = fopen(buf2, "r"))) {
			log("SYSERR: File '%s' listed in '%s/%s': %s", buf2, prefix,
			    index_filename, strerror(errno));
			fscanf(db_index, "%s\n", buf1);
			continue;
		} else {
			if(mode == DB_BOOT_ZON) {
				rec_count++;
			} else if(mode == DB_BOOT_HLP) {
				rec_count += count_alias_records(db_file);
			} else {
				rec_count += count_hash_records(db_file);
			}
		}

		fclose(db_file);
		fscanf(db_index, "%s\n", buf1);
	}

	/* Exit if 0 records, unless this is shops */
	if(!rec_count) {
		if(mode == DB_BOOT_SHP) {
			return;
		}

		log("SYSERR: boot error - 0 records counted in %s/%s.", prefix,
		    index_filename);
		exit(1);
	}

	/*
	 * NOTE: "bytes" does _not_ include strings or other later malloc'd things.
	 */
	switch(mode) {
		case DB_BOOT_WLD:
			//CREATE(world, struct room_data, rec_count);
			world.reserve(rec_count);
			size[0] = sizeof(struct room_data) * rec_count;
			log("   %d rooms, %d bytes.", rec_count, size[0]);
			break;

		case DB_BOOT_MOB:
			size[0] = sizeof(struct index_data) * rec_count;
			size[1] = sizeof(struct char_data) * rec_count;
			log("   %d mobs, %d bytes in index, %d bytes in prototypes.", rec_count, size[0], size[1]);
			break;

		case DB_BOOT_OBJ:
			//CREATE(obj_proto, struct obj_data, rec_count);
			//CREATE(obj_index, struct index_data, rec_count);
			//obj_index.reserve(rec_count);
			size[0] = sizeof(struct index_data) * rec_count;
			size[1] = sizeof(struct obj_data) * rec_count;
			log("   %d objs, %d bytes in index, %d bytes in prototypes.", rec_count, size[0], size[1]);
			break;

		case DB_BOOT_ZON:
			log("!DEPRECATED! DB_BOOT_ZON");
			break;

		case DB_BOOT_HLP:
			CREATE(help_table, struct help_index_element, rec_count);
			size[0] = sizeof(struct help_index_element) * rec_count;
			log("   %d entries, %d bytes.", rec_count, size[0]);
			break;
	}

	rewind(db_index);
	fscanf(db_index, "%s\n", buf1);

	while(*buf1 != '$') {
		snprintf(buf2, sizeof(buf2), "%s%s", prefix, buf1);

		if(!(db_file = fopen(buf2, "r"))) {
			log("SYSERR: %s: %s", buf2, strerror(errno));
			exit(1);
		}

		switch(mode) {
			case DB_BOOT_WLD:
			case DB_BOOT_OBJ:
			case DB_BOOT_MOB:
				discrete_load(db_file, mode, buf2);
				break;

			case DB_BOOT_ZON:
				load_zones(db_file, buf2);
				break;

			case DB_BOOT_HLP:
				/*
				 * If you think about it, we have a race here.  Although, this is the
				 * "point-the-gun-at-your-own-foot" type of race.
				 */
				load_help(db_file);
				break;

			case DB_BOOT_SHP:
				boot_the_shops(db_file, buf2, rec_count);
				break;
		}

		fclose(db_file);
		fscanf(db_index, "%s\n", buf1);
	}

	fclose(db_index);

	/* sort the help index */
	if(mode == DB_BOOT_HLP) {
		qsort(help_table, top_of_helpt, sizeof(struct help_index_element), hsort);
		top_of_helpt--;
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

			if(nr >= 99999) {
				return;
			} else
				switch(mode) {
					case DB_BOOT_WLD:
						parse_room(fl, nr);
						break;

					case DB_BOOT_MOB:
						break;

					case DB_BOOT_OBJ:
						//strlcpy(line, parse_object(fl, nr), sizeof(line));
						break;
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
	char_data proto;
	clear_char(&proto);
	auto txn = txn();
	sql_compositor comp("mobile",&txn);
	auto sql = comp
	           .select("COUNT(*)")
	           .from("mobile")
	           .where("1","=","1")
	           .sql();
	auto result = mods::pq::exec(txn,sql);
	mods::pq::commit(txn);

	if(result.size()) {
		auto txn2 = txn();
		std::cerr << "foobar\n";
		auto result2 =
		    mods::pq::exec(txn2,sql_compositor("mobile",&txn2)
					.select("*")
		                   .from("mobile")
		                   .where("1","=","1")
		                   .sql()
		                  );

		for(auto row : result2) {
			std::cerr << "foo";
			proto.player.name.assign(row["mob_name"].c_str());
			proto.player.short_descr.assign(row["mob_short_description"].c_str());
			proto.player.long_descr.assign(row["mob_long_description"].c_str());

			if(!row["mob_description"].is_null()) {
				proto.player.description.assign(row["mob_description"].c_str());
			}

			proto.char_specials.saved.act = row["mob_action_bitvector"].as<int>();
			SET_BIT(proto.char_specials.saved.act, MOB_ISNPC);
			REMOVE_BIT(proto.char_specials.saved.act, MOB_NOTDEADYET);
			proto.char_specials.saved.affected_by = 0;
			proto.char_specials.saved.alignment = row["mob_alignment"].as<int>();

			/* AGGR_TO_ALIGN is ignored if the mob is AGGRESSIVE. */
			if(MOB_FLAGGED(&proto, MOB_AGGRESSIVE) && MOB_FLAGGED(&proto, MOB_AGGR_GOOD | MOB_AGGR_EVIL | MOB_AGGR_NEUTRAL)) {
				log("SYSERR: Mob both Aggressive and Aggressive_to_Alignment.");
			}

#define MENTOC_ABIL_SET(struct_name,sql_name) proto.real_abils.struct_name = row[#sql_name].as<int>();
			MENTOC_ABIL_SET(str,mob_ability_strength);
			MENTOC_ABIL_SET(intel,mob_ability_intelligence);
			MENTOC_ABIL_SET(wis,mob_ability_wisdom);
			MENTOC_ABIL_SET(dex,mob_ability_dexterity);
			MENTOC_ABIL_SET(con,mob_ability_constitution);
			MENTOC_ABIL_SET(cha,mob_ability_charisma);

			GET_LEVEL(&proto) = row["mob_level"].as<int>();
			GET_HITROLL(&proto) = 20 - row["mob_hitroll"].as<int>();
			GET_AC(&proto) = 10 * row["mob_armor"].as<int>();

			/* max hit = 0 is a flag that H, M, V is xdy+z */
			GET_MAX_HIT(&proto) = row["mob_max_hitpoints"].as<int>();
			GET_HIT(&proto) = row["mob_hitpoints"].as<int>();
			GET_MANA(&proto) = row["mob_mana"].as<int>();
			GET_MOVE(&proto) = row["mob_move"].as<int>();
			GET_MAX_MANA(&proto) = row["mob_max_mana"].as<int>();
			GET_MAX_MOVE(&proto) = row["mob_max_move"].as<int>();
			proto.mob_specials.damnodice = row["mob_damnodice"].as<int>();
			proto.mob_specials.damsizedice = row["mob_damsizedice"].as<int>();
			proto.mob_specials.behaviour_tree = behaviour_tree::NONE;
			GET_DAMROLL(&proto) = row["mob_damroll"].as<int>();
			GET_GOLD(&proto) = row["mob_gold"].as<int>();
			GET_EXP(&proto) = row["mob_exp"].as<int>();
			GET_POS(&proto) = row["mob_load_position"].as<int>();
			GET_DEFAULT_POS(&proto) = row["mob_load_position"].as<int>();
			GET_SEX(&proto) = row["mob_sex"].as<int>();
			GET_CLASS(&proto) = row["mob_class"].as<int>();
			GET_WEIGHT(&proto) = row["mob_weight"].as<int>();
			GET_HEIGHT(&proto) = row["mob_height"].as<int>();
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

			proto.nr = 0;
			proto.desc = nullptr;
			proto.uuid = mods::globals::get_uuid();
			mob_proto.push_back(proto);
			top_of_mobt = mob_proto.size();
			index_data m_index;
			m_index.vnum = row["mob_virtual_number"].as<int>();
			m_index.number = 0;
			m_index.func = nullptr;
			mob_index.push_back(m_index);
		}
	}
}

int parse_sql_objects() {
	auto txn = txn();
	sql_compositor comp("object",&txn);
	auto sql = comp
	           .select("COUNT(*)")
	           .from("object")
	           .where("1","=","1")
	           .sql();
	auto result = mods::pq::exec(txn,sql);
	mods::pq::commit(txn);

	if(result.size()) {
		auto count = mods::pq::as_int(result,0,0);
		log((std::to_string(count) + " objects found in postgres").c_str());

		if(count > 0) {
			obj_index.reserve(count);
			obj_proto.reserve(count);
			auto txn2 = txn();
			auto rows = mods::pq::exec(txn2,sql_compositor("object",&txn2)
			                           .select("*")
			                           .from("object")
			                           .left_join("object_flags")
			                           .on("object_flags.obj_fk_id","=","object.id")
			                           .left_join("object_weapon")
			                           .on("object_weapon.obj_fk_id","=","object.id")
			                           .where("1","=","1")
			                           .sql()
			                          );
			unsigned item = 0;
			log((std::string("Objects ") + std::to_string(rows.size())).c_str());
			mods::pq::commit(txn2);

			for(auto  row : rows) {
				struct index_data index;
				index.vnum = row["obj_item_number"].as<int>();
				index.number = 0;
				index.func = nullptr;
				obj_index.push_back(index);
				struct obj_data proto;
				auto txn3 = txn();
				auto aff_rows = mods::pq::exec(txn3,sql_compositor("affected_type",&txn3)
				                               .select("aff_location,aff_modifier")
				                               .from("affected_type")
				                               .where("aff_fk_id","=",row["obj_item_number"].c_str())
				                               .sql()
				                              );
				mods::pq::commit(txn3);

				for(unsigned i = 0; i < MAX_OBJ_AFFECT; i++) {
					proto.affected[i].location = 0;
					proto.affected[i].modifier = 0;
				}

				unsigned aff_index = 0;

				for(auto aff_row : aff_rows) {
					if(aff_index >= MAX_OBJ_AFFECT) {
						log(
						    (std::string(
						         "WARNING: sql has more affected rows than allowed on object #")
						     + std::to_string(row["obj_item_number"].as<int>())
						    ).c_str()
						);
						break;
					}

					proto.affected[aff_index].location = aff_row["aff_location"].as<int>();
					proto.affected[aff_index].modifier = aff_row["aff_modifier"].as<int>();
					++aff_index;
				}

				proto.item_number = row["obj_item_number"].as<int>();
				proto.name = strdup(row["obj_name"].c_str());
				proto.description = strdup(row["obj_description"].c_str());
#define MENTOC_STR(sql_name,obj_name) \
				if(std::string(row[#sql_name].c_str()).length()){\
					proto.obj_name = \
						strdup(row[#sql_name].c_str());\
				}else{\
					proto.obj_name = strdup("<default>");\
				}
				MENTOC_STR(obj_short_description,short_description);
				MENTOC_STR(obj_action_description,action_description);
				auto txn5 = txn();
				auto ed_rows = mods::pq::exec(txn5,sql_compositor("extra_description",&txn5)
				                              .select("*")
				                              .from("extra_description")
				                              .where("obj_fk_id","=",row["id"].c_str())
				                              .sql()
				                             );
				proto.ex_description = (extra_descr_data*) calloc(1,sizeof(extra_descr_data));
				proto.ex_description->next = nullptr;
				proto.ex_description->keyword = proto.ex_description->description = nullptr;

				if(ed_rows.size()) {
					auto ex_desc = proto.ex_description;
					auto previous = ex_desc;
					int ctr = 0;

					for(auto ed_row : ed_rows) {
						if(!ex_desc) {
							ex_desc = (extra_descr_data*)
							          calloc(1,sizeof(extra_descr_data));
						}

						ex_desc->keyword = strdup(ed_row["extra_keyword"].c_str());
						ex_desc->description = strdup(ed_row["extra_description"].c_str());
						ex_desc->next = nullptr;

						if(ctr) {
							previous->next = ex_desc;
						}

						previous = ex_desc;
						ex_desc = ex_desc->next;
					}
				}

				mods::pq::commit(txn5);
				proto.ex_description->next = nullptr;
				proto.worn_on = row["obj_worn_on"].as<int>();
				proto.type = row["obj_type"].as<int>();
				proto.ammo = 0;
				memset(&proto.obj_flags,0,sizeof(obj_flag_data));
				//TODO: !small do obj->flags fetching from db
				auto txn4 = txn();
				auto flag_rows = mods::pq::exec(txn4,sql_compositor("affected_type",&txn4)
				                                .select("*")
				                                .from("object_flags")
				                                .where("obj_fk_id","=",row["id"].c_str())
				                                .sql()
				                               );
				mods::pq::commit(txn4);

				if(flag_rows.size()) {
					auto flag_row = flag_rows[0];

					if(!flag_row["value_0"].is_null()) {
						proto.obj_flags.value[0] = flag_row["value_0"].as<int>();
					}

					if(!flag_row["value_1"].is_null()) {
						proto.obj_flags.value[1] = flag_row["value_1"].as<int>();
					}

					if(!flag_row["value_2"].is_null()) {
						proto.obj_flags.value[2] = flag_row["value_2"].as<int>();
					}

					if(!flag_row["value_3"].is_null()) {
						proto.obj_flags.value[3] = flag_row["value_3"].as<int>();
					}

					proto.obj_flags.type_flag = flag_row["type_flag"].as<int>();
					proto.obj_flags.wear_flags = flag_row["wear_flags"].as<int>();
					proto.obj_flags.extra_flags = flag_row["extra_flags"].as<int>();
					proto.obj_flags.weight = flag_row["weight"].as<int>();
					proto.obj_flags.cost = flag_row["cost"].as<int>();
					proto.obj_flags.cost_per_day = flag_row["cost_per_day"].as<int>();
					proto.obj_flags.timer = flag_row["timer"].as<int>();
					proto.obj_flags.bitvector = flag_row["bitvector"].as<int>();
				}

				if(!row["obj_ammo_max"].is_null()) {
					proto.ammo_max = row["obj_ammo_max"].as<int>();
				} else {
					proto.ammo_max = 0;
				}

				proto.holds_ammo = 0;

				if(!row["obj_type_data"].is_null()) {
					proto.weapon_type = std::hash<std::string> {}
					                    (
					                        row["obj_type_data"].c_str()
					                    );
				} else {
					proto.weapon_type = 0;
				}

				proto.carried_by = proto.worn_by = nullptr;
				proto.next_content = nullptr;
				proto.contains = nullptr;
				proto.in_obj = nullptr;
				proto.worn_by = nullptr;
				proto.carried_by = nullptr;
				obj_proto.push_back(proto);
				++item;
			}
		}
	} else {
		log("[notice] no objects from sql");
	}

	return 0;
}
/* load the zones */
void parse_sql_zones() {
	zone_table.clear();
	log("[status] Loading sql zones");
	auto trans2 = mods::pq::transaction(*mods::globals::pq_con);
	auto r = mods::pq::exec(trans2,"SELECT * FROM zone");
	mods::pq::commit(trans2);

	for(auto row: r) {
		struct zone_data z;
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
//		select * from zone;
//		 id | zone_start | zone_end | zone_name | lifespan | reset_mode
//		 ----+------------+----------+-----------+----------+------------
//		 (0 rows)
		z.name = strdup(row["zone_name"].c_str());
		z.lifespan = row[4].as<int>();
		z.age = 0;
		z.bot = row[1].as<int>();
		z.top = row[2].as<int>();
		z.reset_mode = row[5].as<int>();
		z.number = row[0].as<int>();
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
		//TODO: SELECT COUNT(*) FROM zone_data where zone_id = z.number
		auto trans3 = mods::pq::transaction(*mods::globals::pq_con);
		auto zone_data_result = mods::pq::exec(trans3,
		                                       std::string(
		                                           "SELECT * FROM zone_data where zone_id="
		                                       ) +
		                                       trans3.quote(z.number)
		                                      );
		mods::pq::commit(trans3);

		for(auto row : zone_data_result) {
			struct reset_com res;
			std::string command = row[2].c_str();
			res.command = command[0];
			res.if_flag = row[3].as<int>();
			res.arg1 = row[4].as<int>();
			res.arg2 = row[5].as<int>();
			res.arg3 = row[6].as<int>();
			res.line = row[0].as<int>();
			z.cmd.push_back(res);
		}

		zone_table.emplace_back(z);
		log("zone loaded");
	}

	top_of_zone_table = zone_table.size();
}
/* load the rooms */
void parse_sql_rooms() {
	auto trans = mods::pq::transaction(*mods::globals::pq_con);
	auto result = mods::pq::exec(trans,"SELECT COUNT(*) FROM room");
	mods::pq::commit(trans);
	world.reserve(mods::pq::as_int(result,0,0));
	auto trans2 = mods::pq::transaction(*mods::globals::pq_con);
	auto r = mods::pq::exec(trans2,"SELECT * FROM room");
	mods::pq::commit(trans2);

	for(auto row: r) {
		struct room_data room;
		room.number = row[1].as<int>();
		room.zone = row[2].as<int>();
		room.name = row["name"].c_str();
		room.description = row["description"].c_str();
		room.room_flags = row[9].as<int>();
		room.sector_type = row[3].as<int>();
		room.ex_description = nullptr;
		room.func = nullptr;
		room.contents = nullptr;
		room.people = nullptr;
		room.light = row[8].as<int>();

		//TODO: setup directions to work properly here (dir_option)
		for(unsigned i = 0; i < NUM_OF_DIRS; i++) {
			room.dir_option[i] = nullptr;
		}

		world.push_back(room);
		mods::globals::register_room(world.size());
		top_of_world = world.size();
	}

	auto trans4 = mods::pq::transaction(*mods::globals::pq_con);
	r = mods::pq::exec(trans4,"SELECT * FROM room");
	mods::pq::commit(trans4);

	for(auto row: r) {
		auto trans3 = mods::pq::transaction(*mods::globals::pq_con);
		auto r2 = mods::pq::exec(trans3,
		                         std::string(
		                             "SELECT * FROM room_direction_data WHERE room_number="
		                         )
		                         +
		                         trans3.quote(
		                             row[1].as<int>()
		                         )
		                        );

		for(auto row2: r2) {
			//id | room_number | exit_direction | general_description | keyword | exit_info | exit_key | to_room
			auto direction = row2[2].as<int>();
			auto real_room_number = real_room(row2[1].as<int>());

			if(real_room_number == NOWHERE) {
				log("Invalid real_room_number: %d",row2[1].as<int>());
				continue;
			}

			world[real_room_number].dir_option[direction] = (room_direction_data*) calloc(sizeof(room_direction_data),1);
			world[real_room_number].dir_option[direction]->general_description = strdup(row2["general_description"].c_str());
			world[real_room_number].dir_option[direction]->keyword = strdup(row2["keyword"].c_str());
			auto exit_info = row2[5].as<int>();

			switch(exit_info) {
				case 1:
				default:
					world[real_room_number].dir_option[direction]->exit_info = EX_ISDOOR;
					REMOVE_BIT(world[real_room_number].dir_option[direction]->exit_info,EX_CLOSED);
					break;

				case 2:
					world[real_room_number].dir_option[direction]->exit_info = EX_ISDOOR | EX_PICKPROOF;
					SET_BIT(world[real_room_number].dir_option[direction]->exit_info,EX_CLOSED);
					break;

				case 3:
					world[real_room_number].dir_option[direction]->exit_info = EX_ISDOOR | EX_REINFORCED;
					SET_BIT(world[real_room_number].dir_option[direction]->exit_info,EX_CLOSED);
					break;
			}

			world[real_room_number].dir_option[direction]->key = row2[6].as<int>();
			world[real_room_number].dir_option[direction]->to_room = real_room(row2[7].as<int>());
		}
	}

	log("Number of rooms in postgres: %d",mods::pq::as_int(result,0,0));
	top_of_world = world.size();
	return;
}
void parse_room(FILE *fl, int virtual_nr) {
	auto str = "[DEPRECATED] parse_room";
	log(str);
}


/* read direction data */
void setup_dir(FILE *fl, int room, int dir) {
	int t[5];
	char line[READ_SIZE], buf2[128];

	snprintf(buf2, sizeof(buf2), "room #%d, direction D%d", GET_ROOM_VNUM(room), dir);

	CREATE(world[room].dir_option[dir], struct room_direction_data, 1);
	world[room].dir_option[dir]->general_description = fread_string(fl, buf2);
	world[room].dir_option[dir]->keyword = fread_string(fl, buf2);

	if(!get_line(fl, line)) {
		log("SYSERR: Format error, %s", buf2);
		exit(1);
	}

	if(sscanf(line, " %d %d %d ", t, t + 1, t + 2) != 3) {
		log("SYSERR: Format error, %s", buf2);
		exit(1);
	}

	if(t[0] == 1) {
		world[room].dir_option[dir]->exit_info = EX_ISDOOR;
		SET_BIT(world[room].dir_option[dir]->exit_info,EX_CLOSED);
	} else if(t[0] == 2) {
		world[room].dir_option[dir]->exit_info = EX_ISDOOR | EX_PICKPROOF;
		SET_BIT(world[room].dir_option[dir]->exit_info,EX_CLOSED);
		/* !mods */
	} else if(t[0] == 3) {
		world[room].dir_option[dir]->exit_info = EX_ISDOOR | EX_REINFORCED;
		SET_BIT(world[room].dir_option[dir]->exit_info,EX_CLOSED);
	} else {
		world[room].dir_option[dir]->exit_info = 0;
	}

	world[room].dir_option[dir]->key = t[1];
	world[room].dir_option[dir]->to_room = t[2];
}


/* make sure the start rooms exist & resolve their vnums to rnums */
void check_start_rooms(void) {
	if((r_mortal_start_room = real_room(mortal_start_room)) == NOWHERE) {
		log("SYSERR:  Mortal start room does not exist.  Change in config.c.");
		exit(1);
	}

	if((r_immort_start_room = real_room(immort_start_room)) == NOWHERE) {
		if(!mini_mud) {
			log("SYSERR:  Warning: Immort start room does not exist.  Change in config.c.");
		}

		r_immort_start_room = r_mortal_start_room;
	}

	if((r_frozen_start_room = real_room(frozen_start_room)) == NOWHERE) {
		if(!mini_mud) {
			log("SYSERR:  Warning: Frozen start room does not exist.  Change in config.c.");
		}

		r_frozen_start_room = r_mortal_start_room;
	}
}


/* resolve all vnums into rnums in the world */
void renum_world(void) {
	int room, door;

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
void renum_zone_table(void) {
	room_rnum a, b, c, olda, oldb, oldc;
	char buf[128];

	//for (zone = 0; zone < top_of_zone_table; zone++)
	for(unsigned zone = 0; zone < zone_table.size(); zone++) {
		for(auto ZCMD : zone_table[zone].cmd) {
			a = b = c = 0;
			olda = ZCMD.arg1;
			oldb = ZCMD.arg2;
			oldc = ZCMD.arg3;

			switch(ZCMD.command) {
				case 'M':
					a = ZCMD.arg1 = real_mobile(ZCMD.arg1);
					c = ZCMD.arg3 = real_room(ZCMD.arg3);
					break;

				case 'O':
					a = ZCMD.arg1 = real_object(ZCMD.arg1);

					if(ZCMD.arg3 != NOWHERE) {
						c = ZCMD.arg3 = real_room(ZCMD.arg3);
					}

					break;

				case 'G':
					a = ZCMD.arg1 = real_object(ZCMD.arg1);
					break;

				case 'E':
					a = ZCMD.arg1 = real_object(ZCMD.arg1);
					break;

				case 'P':
					a = ZCMD.arg1 = real_object(ZCMD.arg1);
					c = ZCMD.arg3 = real_object(ZCMD.arg3);
					break;

				case 'D':
					a = ZCMD.arg1 = real_room(ZCMD.arg1);
					break;

				case 'R': /* rem obj from room */
					a = ZCMD.arg1 = real_room(ZCMD.arg1);
					b = ZCMD.arg2 = real_object(ZCMD.arg2);
					break;
			}

			if(a == NOWHERE || b == NOWHERE || c == NOWHERE) {
				if(!mini_mud) {
					snprintf(buf, sizeof(buf), "Invalid vnum %d, cmd disabled",
					         a == NOWHERE ? olda : b == NOWHERE ? oldb : oldc);
					log_zone_error(zone, 0, buf);
				}

				ZCMD.command = '*';
			}
		}
	}
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


int vnum_mobile(char *searchname, struct char_data *ch) {
	int nr, found = 0;

	for(nr = 0; nr <= top_of_mobt; nr++)
		if(isname(searchname, mob_proto[nr].player.name)) {
			send_to_char(ch, "%3d. [%5d] %s\r\n", ++found, mob_index[nr].vnum, mob_proto[nr].player.short_descr.c_str());
		}

	return (found);
}



int vnum_object(char *searchname, struct char_data *ch) {
	int nr, found = 0;

	for(nr = 0; nr <= top_of_objt; nr++)
		if(isname(searchname, obj_proto[nr].name)) {
			send_to_char(ch, "%3d. [%5d] %s\r\n", ++found, obj_index[nr].vnum, obj_proto[nr].short_description);
		}

	return (found);
}


/* create a character, and add it to the char list */
struct char_data *create_char(void) {
	struct char_data *ch;

	CREATE(ch, struct char_data, 1);
	clear_char(ch);
	ch->next = character_list;
	character_list = ch;

	return (ch);
}


/* create a new mobile from a prototype */
struct char_data *read_mobile(mob_vnum nr, int type) { /* and mob_rnum */
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

	CREATE(mob, struct char_data, 1);
	clear_char(mob);
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


/* create an object, and add it to the object list */
struct obj_data *create_obj(void) {
	struct obj_data *obj;

	/*
	CREATE(obj, struct obj_data, 1);
	clear_object(obj);
	obj->next = object_list;
	object_list = obj;
	*/
	struct obj_data tmp;
	clear_object(&tmp);
	object_list.push_back(tmp);
	obj = &(object_list.at(object_list.size()-1));
	return obj;
}


/* create a new object from a prototype */
struct obj_data *read_object(obj_vnum nr, int type) { /* and obj_rnum */
	struct obj_data obj;
	obj_rnum i = type == VIRTUAL ? real_object(nr) : nr;

	std::size_t ii = i;

	if(i == NOTHING || ii > obj_proto.size()) {
		log("Object (%c) %d does not exist in database.", type == VIRTUAL ? 'V' : 'R', nr);
		return (NULL);
	}

#if 0
	CREATE(obj, struct obj_data, 1);
	clear_object(obj);
	*obj = obj_proto[i];
	obj->next = object_list;
	object_list = obj;
#endif
	clear_object(&obj);
	obj = obj_proto[i];
	object_list.push_back(obj);
	obj_index[i].number++;
	return &object_list.at(object_list.size()-1);
}



#define ZO_DEAD  999

/* update zone ages, queue for reset if necessary, and dequeue when possible */
void zone_update(void) {
	unsigned int i;
	struct reset_q_element *update_u, *temp;
	static int timer = 0;

	/* jelson 10/22/92 */
	if(((++timer * PULSE_ZONE) / PASSES_PER_SEC) >= 60) {
		/* one minute has passed */
		/*
		 * NOT accurate unless PULSE_ZONE is a multiple of PASSES_PER_SEC or a
		 * factor of 60
		 */

		timer = 0;

		/* since one minute has passed, increment zone ages */
		for(i = 0; i < zone_table.size(); i++) {
			if(zone_table[i].age < zone_table[i].lifespan &&
			        zone_table[i].reset_mode) {
				(zone_table[i].age)++;
			}

			if(zone_table[i].age >= zone_table[i].lifespan &&
			        zone_table[i].age < ZO_DEAD && zone_table[i].reset_mode) {
				/* enqueue zone */

				CREATE(update_u, struct reset_q_element, 1);

				update_u->zone_to_reset = i;
				update_u->next = 0;

				if(!reset_q.head) {
					reset_q.head = reset_q.tail = update_u;
				} else {
					reset_q.tail->next = update_u;
					reset_q.tail = update_u;
				}

				zone_table[i].age = ZO_DEAD;
			}
		}
	}	/* end - one minute has passed */


	/* dequeue zones (if possible) and reset */
	/* this code is executed every 10 seconds (i.e. PULSE_ZONE) */
	for(update_u = reset_q.head; update_u; update_u = update_u->next)
		if(zone_table[update_u->zone_to_reset].reset_mode == 2 ||
		        is_empty(update_u->zone_to_reset)) {
			reset_zone(update_u->zone_to_reset);
			mudlog(CMP, LVL_GOD, FALSE, "Auto zone reset: %s", zone_table[update_u->zone_to_reset].name);

			/* dequeue */
			if(update_u == reset_q.head) {
				reset_q.head = reset_q.head->next;
			} else {
				for(temp = reset_q.head; temp->next != update_u;
				        temp = temp->next);

				if(!update_u->next) {
					reset_q.tail = temp;
				}

				temp->next = update_u->next;
			}

			free(update_u);
			break;
		}
}

void log_zone_error(zone_rnum zone, int cmd_no, const char *message) {
	mudlog(NRM, LVL_GOD, TRUE, "SYSERR: zone file: %s", message);
	mudlog(NRM, LVL_GOD, TRUE, "SYSERR: ...offending cmd: '%c' cmd in zone #%d, line %d",
	       '0', zone_table[zone].number, 0);
}

#define ZONE_ERROR(message) \
	{ log_zone_error(zone, cmd_no, message); last_cmd = 0; }

/* execute the reset command table of a given zone */
void reset_zone(zone_rnum zone) {
	int cmd_no = 0, last_cmd = 0;
	struct char_data *mob = NULL;
	struct obj_data *obj, *obj_to;

	log("Resetting zone: %d",zone);

	for(auto ZCMD : zone_table[zone].cmd) {

		if(ZCMD.if_flag && !last_cmd) {
			continue;
		}

		/*  This is the list of actual zone commands.  If any new
		 *  zone commands are added to the game, be certain to update
		 *  the list of commands in load_zone() so that the counting
		 *  will still be correct. - ae.
		 */
		switch(ZCMD.command) {
			case '*':			/* ignore command */
				last_cmd = 0;
				break;

			case 'M':			/* read a mobile */
				if(mob_index[ZCMD.arg1].number < ZCMD.arg2) {
					mob = read_mobile(ZCMD.arg1, REAL);
					char_to_room(mob, real_room(ZCMD.arg3));
					last_cmd = 1;
				} else {
					last_cmd = 0;
				}

				break;

			case 'O':			/* read an object */
				if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
					if(ZCMD.arg3 != NOWHERE) {
						obj = read_object(ZCMD.arg1, REAL);
						obj_to_room(obj, ZCMD.arg3);
						last_cmd = 1;
					} else {
						obj = read_object(ZCMD.arg1, REAL);
						IN_ROOM(obj) = NOWHERE;
						last_cmd = 1;
					}
				} else {
					last_cmd = 0;
				}

				break;

			case 'P':			/* object to object */
				if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
					obj = read_object(ZCMD.arg1, REAL);

					if(!(obj_to = get_obj_num(ZCMD.arg3))) {
						ZONE_ERROR("target obj not found, command disabled");
						ZCMD.command = '*';
						break;
					}

					obj_to_obj(obj, obj_to);
					last_cmd = 1;
				} else {
					last_cmd = 0;
				}

				break;

			case 'G':			/* obj_to_char */
				if(!mob) {
					ZONE_ERROR("attempt to give obj to non-existant mob, command disabled");
					ZCMD.command = '*';
					break;
				}

				if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
					obj = read_object(ZCMD.arg1, REAL);
					obj_to_char(obj, mob);
					last_cmd = 1;
				} else {
					last_cmd = 0;
				}

				break;

			case 'E':			/* object to equipment list */
				if(!mob) {
					ZONE_ERROR("trying to equip non-existant mob, command disabled");
					ZCMD.command = '*';
					break;
				}

				if(obj_index[ZCMD.arg1].number < ZCMD.arg2) {
					if(ZCMD.arg3 < 0 || ZCMD.arg3 >= NUM_WEARS) {
						ZONE_ERROR("invalid equipment pos number");
					} else {
						obj = read_object(ZCMD.arg1, REAL);
						equip_char(mob, obj, ZCMD.arg3);
						last_cmd = 1;
					}
				} else {
					last_cmd = 0;
				}

				break;

			case 'R': /* rem obj from room */
				if((obj = get_obj_in_list_num(ZCMD.arg2, world[ZCMD.arg1].contents)) != NULL) {
					extract_obj(obj);
				}

				last_cmd = 1;
				break;


			case 'D':			/* set state of door */
				if(ZCMD.arg2 < 0 || ZCMD.arg2 >= NUM_OF_DIRS ||
				        (world[ZCMD.arg1].dir_option[ZCMD.arg2] == NULL)) {
					ZONE_ERROR("door does not exist, command disabled");
					ZCMD.command = '*';
				} else
					switch(ZCMD.arg3) {
						case 0:
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_LOCKED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_CLOSED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_BREACHED);
							break;

						case 1:
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_CLOSED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_LOCKED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_BREACHED);
							break;

						case 2:
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_LOCKED);
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_CLOSED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_BREACHED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_REINFORCED);
							break;

						/*!mods*/
						case 3:
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_REINFORCED);
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_LOCKED);
							SET_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							        EX_CLOSED);
							REMOVE_BIT(world[ZCMD.arg1].dir_option[ZCMD.arg2]->exit_info,
							           EX_BREACHED);
							break;
					}

				last_cmd = 1;
				break;

			default:
				ZONE_ERROR("unknown cmd in reset table; cmd disabled");
				ZCMD.command = '*';
				break;
		}
	}

	zone_table[zone].age = 0;
}



/* for use in reset_zone; return TRUE if zone 'nr' is free of PC's  */
int is_empty(zone_rnum zone_nr) {
	for(auto & i : descriptor_list) {
		if(STATE(&i) != CON_PLAYING) {
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


/* Load a char, TRUE if loaded, FALSE if not */
bool load_char(const char *name, struct char_file_u *char_element) {
	try{
		auto txn = txn();
		sql_compositor comp("player",&txn);
		auto result = mods::pq::exec(txn,comp
				.select("*")
				.from("player")
				.where("name","=",name)
				.sql());
			return true;
    } catch(std::exception& e) {
			mudlog(CMP,LVL_GOD,FALSE,(std::string("Error `load_char`: ") + e.what()).c_str());
			return false;
		}
	return false;
}	




/*
 * write the vital data of a player to sql
 */
void save_char(struct char_data *ch) {
	struct char_file_u st;

	if(IS_NPC(ch) || !ch->desc || GET_PFILEPOS(ch) < 0) {
		return;
	}

	st.host = ch->desc->host;
	try {
      auto txn = txn();
			sql_compositor comp("player",&txn);
			auto result = mods::pq::exec(txn,comp
				.select("*")
				.from("player")
				.where("name","=",ch->player.name.c_str())
				.sql());
      mods::pq::commit(txn);
			sql_compositor::value_map values;
			values["player_name"] = ch->player.name.c_str();
			values["player_short_description"] = ch->player.short_descr;
			values["player_long_description"] = ch->player.long_descr;
			values["player_action_bitvector"] = std::to_string(ch->char_specials.saved.act);
			values["player_ability_strength"] = std::to_string(ch->real_abils.str);
			values["player_ability_strength_add"] = std::to_string(ch->real_abils.str_add);
			values["player_ability_intelligence"] = std::to_string(ch->real_abils.intel);
			values["player_ability_wisdom"] = std::to_string(ch->real_abils.wis);
			values["player_ability_dexterity"] = std::to_string(ch->real_abils.dex);
			values["player_ability_constitution"] = std::to_string(ch->real_abils.con);
			values["player_ability_charisma"] = std::to_string(ch->real_abils.cha);
			values["player_ability_alignment"] = std::to_string(ch->char_specials.saved.alignment);
			values["player_attack_type"] = std::to_string(ch->real_abils.con);
			values["player_ability_constitution"] = std::to_string(ch->real_abils.con);
			values["player_attack_type"] = "0";
			values["player_type"] = "PC";
			values["player_alignment"] = "0";
			values["player_level"] = "0";
			values["player_hitroll"] = "0";
			values["player_armor"] = "0";
			values["player_max_hitpoints"] = std::to_string(ch->points.max_hit);
			values["player_max_mana"] = std::to_string(ch->points.max_mana);
			values["player_max_move"] = std::to_string(ch->points.max_move);
			values["player_gold"] = std::to_string(ch->points.gold);
			values["player_exp"] = std::to_string(ch->points.exp);
			values["player_sex"] = ch->player.sex == SEX_MALE ? "M" : "F";
			values["player_hitpoints"] = std::to_string(ch->points.max_hit);
			values["player_mana"] = std::to_string(ch->points.mana);
			values["player_move"] = std::to_string(ch->points.move);
			values["player_damnodice"] = std::to_string(0);
			values["player_damsizedice"] = "0";
			values["player_damroll"] = std::to_string(ch->points.damroll);
			values["player_weight"] = std::to_string(ch->player.weight);
			values["player_height"] = std::to_string(ch->player.height);
			values["player_class"] = std::to_string(ch->player.chclass);
			values["player_title"] = ch->player.title.c_str();
			values["player_hometown"] = std::to_string(ch->player.hometown);
			if(result.size()){
				auto update_txn = txn();
				mods::pq::exec(update_txn,comp
						.update("player")
						.set(values)
						.where("player_name","=",ch->player.name.c_str())
						.sql());
			}else{
				auto insert_txn = txn();
				mods::pq::exec(insert_txn,comp
						.insert()
						.into("player")
						.values(values)
						.sql());
			}
    } catch(std::exception& e) {
			mudlog(CMP,LVL_GOD,FALSE,(std::string("Error `save_char`: ") + e.what()).c_str());
			return;
		}
}





bool parse_sql_player(const char* name,char_data* ch){
	try{
		auto txn = txn();
		sql_compositor comp("player",&txn);
		auto result = mods::pq::exec(txn,comp
				.select("*")
				.from("player")
				.where("name","=",name)
				.sql()
				);
		if(result.size()){
			for(auto row : result){
				ch->player.name.assign(name);
				ch->player.short_descr.assign(row["player_short_description"].c_str());
				ch->player.long_descr.assign(row["player_long_description"].c_str());
				ch->char_specials.saved.act = row["player_action_bitvector"].as<int>();
				ch->real_abils.str = row["player_ability_strength"].as<int>();
				ch->real_abils.str_add = row["player_ability_strength_add"].as<int>();
				ch->real_abils.intel = row["player_ability_intelligence"].as<int>();
				ch->real_abils.wis = row["player_ability_wisdom"].as<int>();
				ch->real_abils.dex = row["player_ability_dexterity"].as<int>();
				ch->real_abils.con = row["player_ability_constitution"].as<int>();
				ch->real_abils.cha = row["player_ability_charisma"].as<int>();
				ch->char_specials.saved.alignment = row["player_ability_alignment"].as<int>();
				ch->points.max_hit = row["player_max_hitpoints"].as<int>();
				ch->points.max_mana = row["player_max_mana"].as<int>();
				ch->points.max_move = row["player_max_move"].as<int>();
				ch->points.gold = row["player_gold"].as<int>();
				ch->points.exp = row["player_exp"].as<int>();
				ch->player.sex = std::string(row["player_sex"].c_str()).compare("M") == 0 ? SEX_MALE : SEX_FEMALE;
				ch->points.max_hit = row["player_hitpoints"].as<int>();
				ch->points.mana = row["player_mana"].as<int>();
				ch->points.move = row["player_move"].as<int>();
				ch->points.damroll = row["player_damroll"].as<int>();
				ch->player.weight = row["player_weight"].as<int>();
				ch->player.height = row["player_height"].as<int>();
				ch->player.chclass = row["player_class"].as<int>();
				ch->player.title.assign(row["player_title"].c_str());
				ch->player.hometown = row["player_hometown"].as<int>();
				GET_PASSWD(ch).assign(row["player_password"].c_str());
				break;
			}
		}
	}catch(std::exception& e){
		mudlog(CMP,LVL_GOD,FALSE,(std::string("Error `parse_sql_player`: ") + e.what()).c_str());
		return false;
	}
	/* to save memory, only PC's -- not MOB's -- have player_specials */
	if(!ch->player_specials) {
		ch->player_specials = std::make_shared<player_special_data>();
	}

	ch->player.time.birth = 0;
	ch->player.time.played = 0;
	ch->player.time.logon = time(0);

	GET_LAST_TELL(ch) = NOBODY;

	if(ch->points.max_mana < 100) {
		ch->points.max_mana = 100;
	}

	ch->char_specials.carry_weight = 0;
	ch->char_specials.carry_items = 0;
	ch->points.armor = 100;
	ch->points.hitroll = 0;
	ch->points.damroll = 0;

	/*
	 * If you're not poisioned and you've been away for more than an hour of
	 * real time, we'll set your HMV back to full
	 */

	if(!AFF_FLAGGED(ch, AFF_POISON)){
		GET_HIT(ch) = GET_MAX_HIT(ch);
		GET_MOVE(ch) = GET_MAX_MOVE(ch);
		GET_MANA(ch) = GET_MAX_MANA(ch);
	}
	return true;
}
/* copy data from the file structure to a char struct */
void store_to_char(struct char_file_u *st, struct char_data *ch) {
	parse_sql_player(st->name,ch);
}				/* store_to_char */




/* copy vital data from a players char-structure to the file structure */
void char_to_store(struct char_data *ch, struct char_file_u *st) {
	int i;
	struct affected_type *af;
	struct obj_data *char_eq[NUM_WEARS];

	/* Unaffect everything a character can be affected by */

	for(i = 0; i < NUM_WEARS; i++) {
		if(GET_EQ(ch, i)) {
			char_eq[i] = unequip_char(ch, i);
		} else {
			char_eq[i] = NULL;
		}
	}

	for(af = ch->affected, i = 0; i < MAX_AFFECT; i++) {
		if(af) {
			st->affected[i] = *af;
			st->affected[i].next = 0;
			af = af->next;
		} else {
			st->affected[i].type = 0;	/* Zero signifies not used */
			st->affected[i].duration = 0;
			st->affected[i].modifier = 0;
			st->affected[i].location = 0;
			st->affected[i].bitvector = 0;
			st->affected[i].next = 0;
		}
	}


	/*
	 * remove the affections so that the raw values are stored; otherwise the
	 * effects are doubled when the char logs back in.
	 */

	while(ch->affected) {
		affect_remove(ch, ch->affected);
	}

	if((i >= MAX_AFFECT) && af && af->next) {
		log("SYSERR: WARNING: OUT OF STORE ROOM FOR AFFECTED TYPES!!!");
	}

	ch->aff_abils = ch->real_abils;

	st->birth = ch->player.time.birth;
	st->played = ch->player.time.played;
	st->played += time(0) - ch->player.time.logon;
	st->last_logon = time(0);

	ch->player.time.played = st->played;
	ch->player.time.logon = time(0);

	st->hometown = ch->player.hometown;
	st->weight = GET_WEIGHT(ch);
	st->height = GET_HEIGHT(ch);
	st->sex = GET_SEX(ch);
	st->chclass = GET_CLASS(ch);
	st->level = GET_LEVEL(ch);
	st->abilities = ch->real_abils;
	st->points = ch->points;
	st->char_specials_saved = ch->char_specials.saved;
	st->player_specials_saved = ch->player_specials->saved;

	st->points.armor = 100;
	st->points.hitroll = 0;
	st->points.damroll = 0;

	if(GET_TITLE(ch)) {
		st->title = GET_TITLE(ch);
	} else {
		st->title.clear();
	}

	if(ch->player.description) {
		if(strlen(ch->player.description) >= sizeof(st->description)) {
			log("SYSERR: char_to_store: %s's description length: %d, max: %d! "
			    "Truncated.", GET_PC_NAME(ch), strlen(ch->player.description),
			    (st->description.length()));
			ch->player.description.concat("\r\n");
		}

		st->description = ch->player.description.c_str();	/* strcpy: OK (checked above) */
	} else {
		st->description.clear();
	}

	st->name = GET_NAME(ch);

	/* add spell and eq affections back in now */
	for(i = 0; i < MAX_AFFECT; i++) {
		if(st->affected[i].type) {
			affect_to_char(ch, &st->affected[i]);
		}
	}

	for(i = 0; i < NUM_WEARS; i++) {
		if(char_eq[i]) {
			equip_char(ch, char_eq[i], i);
		}
	}

	/*   affect_total(ch); unnecessary, I think !?! */
}				/* Char to store */



void save_etext(struct char_data *ch) {
	/* this will be really cool soon */
}


/*
 * Create a new entry in the in-memory index table for the player file.
 * If the name already exists, by overwriting a deleted character, then
 * we re-use the old position.
 */
int create_entry(const char *name) {
	int i, pos;

	if(top_of_p_table == -1) {	/* no table */
		player_table.emplace_back();
		pos = top_of_p_table = 0;
	} else if((pos = get_ptable_by_name(name)) == -1) {	/* new name */
		i = ++top_of_p_table + 1;
		pos = top_of_p_table;
	}

	/* copy lowercase equivalent of name to table field */
	for(i = 0; LOWER(name[i]); i++){
		player_table[pos].name.concat(LOWER(name[i]));
	}

	return (pos);
}



/************************************************************************
*  funcs of a (more or less) general utility nature			*
************************************************************************/


/* read and allocate space for a '~'-terminated string from a given file */
char *fread_string(FILE *fl, const char *error) {
	char buf[MAX_STRING_LENGTH], tmp[513];
	char *point;
	int done = 0, length = 0, templength;

	*buf = '\0';

	do {
		if(!fgets(tmp, 512, fl)) {
			log("SYSERR: fread_string: format error at or near %s", error);
			exit(1);
		}

		/* If there is a '~', end the string; else put an "\r\n" over the '\n'. */
		if((point = strchr(tmp, '~')) != NULL) {
			*point = '\0';
			done = 1;
		} else {
			point = tmp + strlen(tmp) - 1;
			*(point++) = '\r';
			*(point++) = '\n';
			*point = '\0';
		}

		templength = strlen(tmp);

		if(length + templength >= MAX_STRING_LENGTH) {
			log("SYSERR: fread_string: string too large (db.c)");
			log("%s", error);
			exit(1);
		} else {
			strcat(buf + length, tmp);	/* strcat: OK (size checked above) */
			length += templength;
		}
	} while(!done);

	/* allocate space for the new string and copy it */
	return (strlen(buf) ? strdup(buf) : NULL);
}


/* release memory allocated for a char struct */
void free_char(struct char_data *ch) {
	struct alias_data *a;

	if(ch->player_specials) {
		while((a = GET_ALIASES(ch)) != NULL) {
			GET_ALIASES(ch) = (GET_ALIASES(ch))->next;
			free_alias(a);
		}
	}

	while(ch->affected) {
		affect_remove(ch, ch->affected);
	}

	if(ch->desc) {
		ch->desc->character = NULL;
	}

}




/* release memory allocated for an obj struct */
void free_obj(struct obj_data *obj) {
	int nr;

	if((nr = GET_OBJ_RNUM(obj)) == NOTHING) {
		if(obj->name) {
			free(obj->name);
		}

		if(obj->description) {
			free(obj->description);
		}

		if(obj->short_description) {
			free(obj->short_description);
		}

		if(obj->action_description) {
			free(obj->action_description);
		}

		if(obj->ex_description) {
			free_extra_descriptions(obj->ex_description);
		}
	} else {
		if(obj->name && obj->name != obj_proto[nr].name) {
			free(obj->name);
		}

		if(obj->description && obj->description != obj_proto[nr].description) {
			free(obj->description);
		}

		if(obj->short_description && obj->short_description != obj_proto[nr].short_description) {
			free(obj->short_description);
		}

		if(obj->action_description && obj->action_description != obj_proto[nr].action_description) {
			free(obj->action_description);
		}

		if(obj->ex_description && obj->ex_description != obj_proto[nr].ex_description) {
			free_extra_descriptions(obj->ex_description);
		}
	}

	free(obj);
}


/*
 * Steps:
 *   1: Read contents of a text file.
 *   2: Make sure no one is using the pointer in paging.
 *   3: Allocate space.
 *   4: Point 'buf' to it.
 *
 * We don't want to free() the string that someone may be
 * viewing in the pager.  page_string() keeps the internal
 * strdup()'d copy on ->showstr_head and it won't care
 * if we delete the original.  Otherwise, strings are kept
 * on ->showstr_vector but we'll only match if the pointer
 * is to the string we're interested in and not a copy.
 *
 * If someone is reading a global copy we're trying to
 * replace, give everybody using it a different copy so
 * as to avoid special cases.
 */
int file_to_string_alloc(const char *name, char **buf) {
	log("[deprecated]: file_to_string_alloc");
	return (0);
}


/* read contents of a text file, and place in buf */
int file_to_string(const char *name, char *buf) {
	FILE *fl;
	char tmp[READ_SIZE + 3];
	int len;

	*buf = '\0';

	if(!(fl = fopen(name, "r"))) {
		log("SYSERR: reading %s: %s", name, strerror(errno));
		return (-1);
	}

	for(;;) {
		if(!fgets(tmp, READ_SIZE, fl)) {	/* EOF check */
			break;
		}

		if((len = strlen(tmp)) > 0) {
			tmp[len - 1] = '\0';    /* take off the trailing \n */
		}

		strcat(tmp, "\r\n");	/* strcat: OK (tmp:READ_SIZE+3) */

		if(strlen(buf) + strlen(tmp) + 1 > MAX_STRING_LENGTH) {
			log("SYSERR: %s: string too big (%d max)", name, MAX_STRING_LENGTH);
			*buf = '\0';
			fclose(fl);
			return (-1);
		}

		strcat(buf, tmp);	/* strcat: OK (size checked above) */
	}

	fclose(fl);

	return (0);
}



/* clear some of the the working variables of a char */
void reset_char(struct char_data *ch) {
	int i;

	for(i = 0; i < NUM_WEARS; i++) {
		GET_EQ(ch, i) = NULL;
	}

	ch->followers = NULL;
	ch->master = NULL;
	IN_ROOM(ch) = NOWHERE;
	ch->carrying = NULL;
	ch->next = NULL;
	ch->next_fighting = NULL;
	ch->next_in_room = NULL;
	FIGHTING(ch) = NULL;
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



/* clear ALL the working variables of a char; do NOT free any space alloc'ed */
void clear_char(struct char_data *ch) {
	memset((char *) ch, 0, sizeof(struct char_data));

	IN_ROOM(ch) = NOWHERE;
	GET_PFILEPOS(ch) = -1;
	GET_MOB_RNUM(ch) = NOBODY;
	GET_WAS_IN(ch) = NOWHERE;
	GET_POS(ch) = POS_STANDING;
	ch->mob_specials.default_pos = POS_STANDING;
	ch->mob_specials.behaviour_tree = behaviour_tree::NONE;
	ch->drone_simulate = false;
	ch->drone = false;
	ch->drone_owner = 0;
	ch->drone_uuid = 0;
	GET_AC(ch) = 100;		/* Basic Armor */

	if(ch->points.max_mana < 100) {
		ch->points.max_mana = 100;
	}

	ch->carrying = nullptr;
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
void init_char(struct char_data *ch) {
	int i;

	/* create a player_special structure */
	if(!ch->player_specials) {
		ch->player_specials = std::make_shared<player_special_data>();
	}

	/* *** if this is our first player --- he be God *** */
	if(top_of_p_table == 0) {
		GET_LEVEL(ch) = LVL_IMPL;
		GET_EXP(ch) = 7000000;

		/* The implementor never goes through do_start(). */
		GET_MAX_HIT(ch) = 500;
		GET_MAX_MANA(ch) = 100;
		GET_MAX_MOVE(ch) = 82;
		GET_HIT(ch) = GET_MAX_HIT(ch);
		GET_MANA(ch) = GET_MAX_MANA(ch);
		GET_MOVE(ch) = GET_MAX_MOVE(ch);
	}

	set_title(ch, NULL);
	ch->player.short_descr = "";
	ch->player.long_descr = "";
	ch->player.description = "";

	ch->player.time.birth = time(0);
	ch->player.time.logon = time(0);
	ch->player.time.played = 0;

	GET_HOME(ch) = 1;
	GET_AC(ch) = 100;

	for(i = 0; i < MAX_TONGUE; i++) {
		GET_TALK(ch, i) = 0;
	}

	/*
	 * make favors for sex -- or in English, we bias the height and weight of the
	 * character depending on what gender they've chosen for themselves. While it
	 * is possible to have a tall, heavy female it's not as likely as a male.
	 *
	 * Height is in centimeters. Weight is in pounds.  The only place they're
	 * ever printed (in stock code) is SPELL_IDENTIFY.
	 */
	if(GET_SEX(ch) == SEX_MALE) {
		GET_WEIGHT(ch) = rand_number(120, 180);
		GET_HEIGHT(ch) = rand_number(160, 200); /* 5'4" - 6'8" */
	} else {
		GET_WEIGHT(ch) = rand_number(100, 160);
		GET_HEIGHT(ch) = rand_number(150, 180); /* 5'0" - 6'0" */
	}

	if((i = get_ptable_by_name(GET_NAME(ch).c_str())) != -1) {
		player_table[i].id = GET_IDNUM(ch) = ++top_idnum;
	} else {
		log("SYSERR: init_char: Character '%s' not found in player table.", GET_NAME(ch).c_str());
	}

	for(i = 1; i <= MAX_SKILLS; i++) {
		if(GET_LEVEL(ch) < LVL_IMPL) {
			SET_SKILL(ch, i, 0);
		} else {
			SET_SKILL(ch, i, 100);
		}
	}

	AFF_FLAGS(ch) = 0;

	for(i = 0; i < 5; i++) {
		GET_SAVE(ch, i) = 0;
	}

	ch->real_abils.intel = 25;
	ch->real_abils.wis = 25;
	ch->real_abils.dex = 25;
	ch->real_abils.str = 25;
	ch->real_abils.str_add = 100;
	ch->real_abils.con = 25;
	ch->real_abils.cha = 25;

	for(i = 0; i < 3; i++) {
		GET_COND(ch, i) = (GET_LEVEL(ch) == LVL_IMPL ? -1 : 24);
	}

	GET_LOADROOM(ch) = NOWHERE;
	ch->drone = false;
}



/* returns the real number of the room with given virtual number */
room_rnum real_room(room_vnum vnum) {
	room_rnum bot, top, mid;

	bot = 0;
	top = top_of_world;

	for(unsigned i =0; i < world.size(); i++) {
		if(world[i].number == vnum) {
			return i;
		}
	}

	return NOWHERE;
	/* perform binary search on world-table */

	for(;;) {
		mid = (bot + top) / 2;

		if((world[mid]).number == vnum) {
			return (mid);
		}

		if(bot >= top) {
			return (NOWHERE);
		}

		if((world[mid]).number > vnum) {
			top = mid - 1;
		} else {
			bot = mid + 1;
		}
	}
}



/* returns the real number of the monster with given virtual number */
mob_rnum real_mobile(mob_vnum vnum) {
	mob_rnum bot, top, mid;

	bot = 0;
	top = top_of_mobt;

	if(top == 0) {
		return NOBODY;
	}

	/* perform binary search on mob-table */
	for(;;) {
		mid = (bot + top) / 2;

		if((mob_index[mid]).vnum == vnum) {
			return (mid);
		}

		if(bot >= top) {
			return (NOBODY);
		}

		if((mob_index[mid]).vnum > vnum) {
			top = mid - 1;
		} else {
			bot = mid + 1;
		}
	}
}


/* returns the real number of the object with given virtual number */
obj_rnum real_object(obj_vnum vnum) {
	obj_rnum bot, top, mid;

	bot = 0;
	top = obj_index.size();

	/* perform binary search on obj-table */
	for(;;) {
		mid = (bot + top) / 2;

		if((obj_index[mid]).vnum == vnum) {
			return (mid);
		}

		if(bot >= top) {
			return (NOTHING);
		}

		if((obj_index[mid]).vnum > vnum) {
			top = mid - 1;
		} else {
			bot = mid + 1;
		}
	}
}


/* returns the real number of the zone with given virtual number */
room_rnum real_zone(room_vnum vnum) {
	room_rnum real_zone_number = 0;

	for(auto& zone : zone_table) {
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

	snprintf(objname, sizeof(objname), "Object #%d (%s)", GET_OBJ_VNUM(obj), obj->short_description);
	error |= check_bitvector_names(GET_OBJ_WEAR(obj), wear_bits_count, objname, "object wear");
	error |= check_bitvector_names(GET_OBJ_EXTRA(obj), extra_bits_count, objname, "object extra");
	error |= check_bitvector_names(GET_OBJ_AFFECT(obj), affected_bits_count, objname, "object affect");

	switch(GET_OBJ_TYPE(obj)) {
		case ITEM_DRINKCON: {
				char onealias[MAX_INPUT_LENGTH], *space = strrchr(obj->name, ' ');

				strlcpy(onealias, space ? space + 1 : obj->name, sizeof(onealias));

				if(search_block(onealias, drinknames, TRUE) < 0 && (error = TRUE))
					log("SYSERR: Object #%d (%s) doesn't have drink type as last alias. (%s)",
					    GET_OBJ_VNUM(obj), obj->short_description, obj->name);
			}

		/* Fall through. */
		case ITEM_FOUNTAIN:
			if(GET_OBJ_VAL(obj, 1) > GET_OBJ_VAL(obj, 0) && (error = TRUE))
				log("SYSERR: Object #%d (%s) contains (%d) more than maximum (%d).",
				    GET_OBJ_VNUM(obj), obj->short_description,
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

