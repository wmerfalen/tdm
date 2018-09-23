#ifndef __MENTOC_DB_HEADER__
#define __MENTOC_DB_HEADER__
/* ************************************************************************
*   File: db.h                                          Part of CircleMUD *
*  Usage: header file for database handling                               *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */
#include "mods/extern.hpp"
/* arbitrary constants used by index_boot() (must be unique) */
#define DB_BOOT_WLD	0
#define DB_BOOT_MOB	1
#define DB_BOOT_OBJ	2
#define DB_BOOT_ZON	3
#define DB_BOOT_SHP	4
#define DB_BOOT_HLP	5

#if defined(CIRCLE_MACINTOSH)
#define LIB_WORLD	":world:"
#define LIB_TEXT	":text:"
#define LIB_TEXT_HELP	":text:help:"
#define LIB_MISC	":misc:"
#define LIB_ETC		":etc:"
#define LIB_PLRTEXT	":plrtext:"
#define LIB_PLROBJS	":plrobjs:"
#define LIB_PLRALIAS	":plralias:"
#define LIB_HOUSE	":house:"
#define SLASH		":"
#elif defined(CIRCLE_AMIGA) || defined(CIRCLE_UNIX) || defined(CIRCLE_WINDOWS) || defined(CIRCLE_ACORN) || defined(CIRCLE_VMS)
#define LIB_WORLD	"world/"
#define LIB_TEXT	"text/"
#define LIB_TEXT_HELP	"text/help/"
#define LIB_MISC	"misc/"
#define LIB_ETC		"etc/"
#define LIB_PLRTEXT	"plrtext/"
#define LIB_PLROBJS	"plrobjs/"
#define LIB_PLRALIAS	"plralias/"
#define LIB_HOUSE	"house/"
#define SLASH		"/"
#else
#error "Unknown path components."
#endif

#define SUF_OBJS	"objs"
#define SUF_TEXT	"text"
#define SUF_ALIAS	"alias"

#if defined(CIRCLE_AMIGA)
#define FASTBOOT_FILE   "/.fastboot"    /* autorun: boot without sleep  */
#define KILLSCRIPT_FILE "/.killscript"  /* autorun: shut mud down       */
#define PAUSE_FILE      "/pause"        /* autorun: don't restart mud   */
#elif defined(CIRCLE_MACINTOSH)
#define FASTBOOT_FILE	"::.fastboot"	/* autorun: boot without sleep	*/
#define KILLSCRIPT_FILE	"::.killscript"	/* autorun: shut mud down	*/
#define PAUSE_FILE	"::pause"	/* autorun: don't restart mud	*/
#else
#define FASTBOOT_FILE   "../.fastboot"  /* autorun: boot without sleep  */
#define KILLSCRIPT_FILE "../.killscript"/* autorun: shut mud down       */
#define PAUSE_FILE      "../pause"      /* autorun: don't restart mud   */
#endif

/* names of various files and directories */
#define INDEX_FILE	"index"		/* index of world files		*/
#define MINDEX_FILE	"index.mini"	/* ... and for mini-mud-mode	*/
#define WLD_PREFIX	"world/wld/"/* room definitions	*/
#define MOB_PREFIX  "world/mob/"/* monster prototypes	*/
#define OBJ_PREFIX	"world/obj/"/* object prototypes	*/
#define ZON_PREFIX	"world/zon/"/* zon defs & command tables */
#define SHP_PREFIX	"world/shp/"	/* shop definitions	*/
#define HLP_PREFIX	"text/help/"	/* for HELP <keyword>	*/

#define CREDITS_FILE	"text/credits" /* for the 'credits' command	*/
#define NEWS_FILE	"text/news"	/* for the 'news' command	*/
#define MOTD_FILE	"text/motd"	/* messages of the day / mortal	*/
#define IMOTD_FILE	"text/imotd"	/* messages of the day / immort	*/
#define GREETINGS_FILE	"text/greetings"	/* The opening screen.	*/
#define HELP_PAGE_FILE	"text/help/screen"	/* for HELP <CR>	*/
#define INFO_FILE	"text/info"		/* for INFO		*/
#define WIZLIST_FILE	"text/wizlist"	/* for WIZLIST		*/
#define IMMLIST_FILE	"text/immlist"	/* for IMMLIST		*/
#define BACKGROUND_FILE	"text/background"/* for the background story	*/
#define POLICIES_FILE	"text/policies"  /* player policies/rules	*/
#define HANDBOOK_FILE	"text/handbook"  /* handbook for new immorts	*/

#define IDEA_FILE	"misc/ideas"	   /* for the 'idea'-command	*/
#define TYPO_FILE	"misc/typos"	   /*         'typo'		*/
#define BUG_FILE	"misc/bugs"	   /*         'bug'		*/
#define MESS_FILE	"misc/messages" /* damage messages		*/
#define SOCMESS_FILE	"misc/socials"  /* messages for social acts	*/
#define XNAME_FILE	"misc/xnames"   /* invalid name substrings	*/

#define PLAYER_FILE	"etc/players"   /* the player database	*/
#define MAIL_FILE	"etc/plrmail"   /* for the mudmail system	*/
#define BAN_FILE	"etc/badsites"  /* for the siteban system	*/
#define HCONTROL_FILE	"etc/hcontrol"  /* for the house system	*/
#define TIME_FILE	"etc/time"	   /* for calendar system	*/

/* public procedures in db.c */
void  boot_hell(void);
void	boot_db(void);
void	destroy_db(void);
int	create_entry(const char *name);
void	zone_update(void);
char	*fread_string(FILE *fl, const char *error);
long	get_id_by_name(const char *name);
char	*get_name_by_id(long id);
void	save_mud_time(struct time_info_data *when);
void	free_extra_descriptions(struct extra_descr_data *edesc);
void	free_text_files(void);
void	free_player_index(void);
void	free_help(void);

zone_rnum real_zone(zone_vnum vnum);
room_rnum real_room(room_vnum vnum);
mob_rnum real_mobile(mob_vnum vnum);
obj_rnum real_object(obj_vnum vnum);

void	char_to_store(struct char_data *ch, struct char_file_u *st);
void	store_to_char(struct char_file_u *st, struct char_data *ch);
bool	load_char(const char *name, struct char_file_u *char_element);
void	save_char(struct char_data *ch);
void	init_char(std::shared_ptr<mods::player>);
struct char_data* create_char(void);
struct char_data *read_mobile(mob_vnum nr, int type);
int	vnum_mobile(char *searchname, struct char_data *ch);
void	clear_char(struct char_data *ch);
void	reset_char(struct char_data *ch);
void	free_char(struct char_data *ch);

struct obj_data *create_obj(void);
void	clear_object(struct obj_data *obj);
void	free_obj(struct obj_data *obj);
struct obj_data *read_object(obj_vnum nr, int type);
int	vnum_object(char *searchname, struct char_data *ch);

typedef enum { \
	REAL=0,\
	VIRTUAL\
} num_type_t;

/* structure for the reset commands */
struct reset_com {
	char	command;   /* current command                      */

	bool if_flag;	/* if TRUE: exe only if preceding exe'd */
	int	arg1;		/*                                      */
	int	arg2;		/* Arguments to the command             */
	int	arg3;		/*                                      */
	int line;		/* line number this command appears on  */

	/*
	*  Commands:              *
	*  'M': Read a mobile     *
	*  'O': Read an object    *
	*  'G': Give obj to mob   *
	*  'P': Put obj in obj    *
	*  'G': Obj to char       *
	*  'E': Obj to char equip *
	*  'D': Set state of door *
	*/
};



/* zone definition structure. for the 'zone-table'   */
struct zone_data {
	char	*name;		    /* name of this zone                  */
	int	lifespan;           /* how long between resets (minutes)  */
	int	age;                /* current age of this zone (minutes) */
	uint64_t bot;           /* starting room number for this zone */
	uint64_t top;           /* upper limit for rooms in this zone */

	int	reset_mode;         /* conditions for reset (see below)   */
	zone_vnum number;	    /* virtual number of this zone	  */
	std::vector<reset_com> cmd;   /* command table for reset	          */

	/*
	 * Reset mode:
	 *   0: Don't reset, and don't update age.
	 *   1: Reset if no PC's are located in zone.
	 *   2: Just reset.
	 */
};



/* for queueing zones for update   */
struct reset_q_element {
	zone_rnum zone_to_reset;            /* ref to zone_data */
	struct reset_q_element *next;
};



/* structure for the update queue     */
struct reset_q_type {
	struct reset_q_element *head;
	struct reset_q_element *tail;
};



struct player_index_element {
	mods::string name;
	long id;
};


struct help_index_element {
	char	*keyword;
	char *entry;
	int duplicate;
};


/* don't change these */
#define BAN_NOT 	0
#define BAN_NEW 	1
#define BAN_SELECT	2
#define BAN_ALL		3

#define BANNED_SITE_LENGTH    50
struct ban_list_element {
	char	site[BANNED_SITE_LENGTH+1];
	int	type;
	time_t date;
	char	name[MAX_NAME_LENGTH+1];
	struct ban_list_element *next;
};


/* global buffering system */

#ifndef __DB_C__
extern room_rnum top_of_world;

extern std::vector<zone_data> zone_table;
extern zone_rnum top_of_zone_table;

extern std::deque<mods::descriptor_data> descriptor_list;
extern struct player_special_data dummy_mob;

extern struct index_data *mob_index;
extern mob_rnum top_of_mobt;

extern struct index_data *obj_index;
extern obj_rnum top_of_objt;
#endif

#ifndef __CONFIG_C__
extern char	*OK;
extern char	*NOPERSON;
extern char	*NOEFFECT;
#endif



#endif	//header inclusion protection
