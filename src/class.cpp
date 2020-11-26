/* ************************************************************************
*   File: class.c                                       Part of CircleMUD *
*  Usage: Source file for class-specific code                             *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

/*
 * This file attempts to concentrate most of the code which must be changed
 * in order for new classes to be added.  If you're adding a new class,
 * you should go through this entire file from beginning to end and add
 * the appropriate new special cases for your new class.
 */



#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "db.h"
#include "utils.h"
#include "spells.h"
#include "interpreter.h"
#include "constants.h"
#include "mods/util.hpp"
#include "mods/levels.hpp"

extern int siteok_everyone;

/* local functions */
void snoop_check(char_data *ch);
player_class_t parse_class(std::string arg);
bitvector_t find_class_bitvector(const char *arg);
byte saving_throws(int class_num, int type, int level);
int thaco(int class_num, int level);
void roll_real_abils(char_data *ch);
void do_start(char_data *ch);
int backstab_mult(int level);
int invalid_class(char_data *ch, struct obj_data *obj);
int level_exp(int chclass, int level);
const char *title_male(int chclass, int level);
const char *title_female(int chclass, int level);

/* Names first */

const char *class_abbrevs[] = {
	"Snp",
	"Mrn",
	"Chm",
	"Con"
	"Eng",
	"Med",
	"Mrk",
	"Psy",
	"Sup",
	"Ghs",
	"\n"
};


const char *pc_class_types[] = {
	"Sniper",
	"Marine",
	"Sentinel",
	"Contagion"
	"Engineer",
	"Medic",
	"Psyop",
	"Support",
	"\n"
};



std::string common_weaponry_skillset() {
	std::string skillset = ""
		"                   [*] --[ Weaponry Skills ]-- [*]                           \r\n"
		"                                                                             \r\n"
		" [ ] Sub Machine Guns                                                        \r\n"
		"  +-[ ] Breach and clear                                                     \r\n"
		" [ ] Assault Rifles                                                          \r\n"
		"  +-[ ] Surpressing fire                                                     \r\n"
		" [ ] Sniper Rifles                                                           \r\n"
		" [ ] Light Machine Guns                                                      \r\n"
		"  +-[ ] Surpressing fire                                                     \r\n"
		" [ ] Pistols                                                                 \r\n"
		"  +-[ ] Quickdraw                                                            \r\n"
		"  +-[ ] Dual wield                                                           \r\n"
		" [ ] Machine Pistols                                                         \r\n"
		"  +-[ ] Dual wield                                                           \r\n"
		" [ ] Shotguns                                                                \r\n"
		"  +-[ ] Limb shot                                                            \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Scope recipes ]-- [*]                             \r\n"
		" [ ] ACOG Scope                                                              \r\n"
		" [ ] Red dot sight                                                           \r\n"
		" [ ] Rifle scope 6X                                                          \r\n"
		" [ ] Rifle scope 12X                                                         \r\n"
		" [ ] Rifle scope 6X/12X                                                      \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Barrel attachments ]-- [*]                        \r\n"
		" [ ] Surpressor                                                              \r\n"
		" [ ] Compensator                                                             \r\n"
		" [ ] Muzzle brake                                                            \r\n"
		" [ ] Remote detonation                                                       \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Demolition Skills]-- [*]                          \r\n"
		" [ ] Remote detonation                                                       \r\n"
		" [ ] Claymore mines                                                          \r\n"
		" [ ] Break charge                                                            \r\n"
		" [ ] Door attached trip mine                                                 \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Gadgets ]-- [*]                                   \r\n"
		" [ ] Ground drone                                                            \r\n"
		"  +-[ ] Custom ammunition                                                    \r\n"
		"  +-[ ] Surveilance                                                          \r\n"
		"  +-[ ] Assisted scanning device                                             \r\n"
		"  +-[ ] Sedative Hypnotic                                                    \r\n"
		"  +-[ ] Tazer                                                                \r\n"
		"  +-[ ] EMP                                                                  \r\n"
		"                                                                             \r\n"
		" [ ] Aerial drone                                                            \r\n"
		"  +-[ ] Custom ammunition                                                    \r\n"
		"  +-[ ] Assisted scanning device                                             \r\n"
		"  +-[ ] EMP                                                                  \r\n"
		"                                                                             \r\n"
		;
		return skillset;
}

std::string engineer_skillset() {

	std::string skillset = ""
		"                   [*] --[ Crafting ]-- [*]                                  \r\n"
		" [ ] Explosives                                                              \r\n"
		"  +-[ ] Incendiary grenade                                                   \r\n"
		"  +-[ ] C4                                                                   \r\n"
		"  +-[ ] Incendiary ammunition                                                \r\n"
		"  +-[ ] Explosive ammunition                                                 \r\n"
		"                                                                             \r\n"
		" [ ] Weapon attachments                                                      \r\n"
		"  +-[ ] Extended magazine                                                    \r\n"
		"  +-[ ] ACOG scope                                                           \r\n"
		"  +-[ ] Laser sight                                                          \r\n"
		"  +-[ ] Suppressor                                                           \r\n"
		"                                                                             \r\n"
		" [ ] Intel                                                                   \r\n"
		"  +-[ ] Sensor grenade                                                       \r\n"
		"  +-[ ] EMP grenade                                                          \r\n"
		"  +-[ ] Thermal scan                                                         \r\n"
		"  +-[ ] Camera                                                               \r\n"
		"  +-[ ] RC Drone                                                             \r\n"
		"  +-[ ] Aerial drone                                                         \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Offensive Tech ]-- [*]                            \r\n"
		" [ ]                                                                         \r\n"
		"                                                                             \r\n"
	;
	return skillset;
}

std::string psyop_skillset() {

	std::string skillset = ""
		"                   [*] --[ Control and manipulation ]-- [*]                  \r\n"
		" [ ] Pyromancy                                                               \r\n"
		"  +-[ ] Ignite ammunition                                                    \r\n"
		"  +-[ ] Craft ammunition clip                                                \r\n"
		"  +-[ ] Napalm explosive ordinance                                           \r\n"
		"  +-[ ] Firewall                                                             \r\n"
		"  +-[ ] Napalm trap                                                          \r\n"
		"                                                                             \r\n"
		" [ ] Intercept Communications                                                \r\n"
		"  +-[ ] Predict enemy position                                               \r\n"
		"  +-[ ] Predict air strike location                                          \r\n"
		"  +-[ ] Predict path of least resistance                                     \r\n"
		"                                                                             \r\n"
		" [ ] Remote Viewing                                                          \r\n"
		"  +-[ ] Eye of Osiris                                                        \r\n"
		"  +-[ ] Pineal Gland Hijack                                                  \r\n"
		"                                                                             \r\n"
		" [ ] Holographic decoy                                                       \r\n"
		"  +-[ ] Call for help                                                        \r\n"
		"                                                                             \r\n"
		" [ ] Interrogate                                                             \r\n"
		"  +-[ ] Seeds of doubt                                                       \r\n"
		"  +-[ ] Disinform                                                            \r\n"
		"                                                                             \r\n"
		" [ ] Alter state of mind                                                     \r\n"
		"  +-[ ] Innebriate                                                           \r\n"
		"      +-[ ] Depressant                                                       \r\n"
		"      +-[ ] Stimulant                                                        \r\n"
		"                                                                             \r\n"
		" [ ] Detect position                                                         \r\n"
		"  +-[ ] Sense heartbeat                                                      \r\n"
		"  +-[ ] Trace gun powder                                                     \r\n"
		"  +-[ ] Trace footsteps                                                      \r\n"
		"                                                                             \r\n"
		" [ ] Lure                                                                    \r\n"
		"                   [*] -- [ Offensive PSYOPS ] -- [*]                        \r\n"
		" [ ] Cardiac Arrest                                                          \r\n"
		"                                                                             \r\n"
		" [ ] Ignite                                                                  \r\n"
		"                                                                             \r\n"
		" [ ] Poison                                                                  \r\n"
		"                                                                             \r\n"
		" [ ] Intimidate                                                              \r\n"
		"                                                                             \r\n"
		" [ ] Blind                                                                   \r\n"
		"                                                                             \r\n"
		"                                                                             \r\n"
	;
	return skillset;
}
std::string sentinel_skillset() {

	std::string skillset = ""
		"                   [*] --[ Control and manipulation ]-- [*]                  \r\n"
		" [ ] Intimidate                                                              \r\n"
		"  +-[ ] Flee                                                                 \r\n"
		"  +-[ ] Adrenal dump                                                         \r\n"
		"  +-[ ] Heart attack                                                         \r\n"
		"                                                                             \r\n"
		" [ ] Heal        [ 10hp ]                                                    \r\n"
		"  +-[ ] Light    [ 15hp ]                                                    \r\n"
		"  +-[ ] Critical [ 50hp ]                                                    \r\n"
		"  +-[ ] Advanced [ 80hp ]                                                    \r\n"
		"                                                                             \r\n"
		" [ ] Human shield                                                            \r\n"
		"  +-[ ] Absorb\r\n"
		"    +-[ ] Gather ammunition\r\n"
		"    +-[ ] Chance to gain hp\r\n"
		"  +-[ ] Ignite\r\n"
		"  +-[ ] Explosive decompression\r\n"
		"  +-[ ] Toxicity\r\n"
		"                                                                             \r\n"
		" [ ] Deny entry                                                              \r\n"
		"  +-[ ] Firewall                                                             \r\n"
		"  +-[ ] Expell                                                               \r\n"
		"  +-[ ] Barricade                                                            \r\n"
		"                                                                             \r\n"
		"                   [*] --[ Gadgets ]-- [*]                                   \r\n"
		" [ ] Shield                                                                  \r\n"
		"  +-[ ] Deflect\r\n"
		"  +-[ ] Deflect and ignite\r\n"
		"  +-[ ] Firewall\r\n"
		"                                                                             \r\n"
		"                   [*] --[ Melee ]-- [*]                                     \r\n"
		" [ ] Oblique kick                                                            \r\n"
		"  +-[ ] Heavy [ break victim's leg ]\r\n"
		"  +-[ ] Extreme [ breaks leg and victim loses 2 turns ]\r\n"
		"                                                                             \r\n"
		"                                                                             \r\n"
	;
	return skillset;
}
std::string sniper_skillset() {

	std::string skillset = ""
		"                       [*] -- [ Weaponry Skills ] -- [*]                    \r\n"
		"[ ] - Sniper Rifle Skills                                                   \r\n"
		"	+                                                                          \r\n"
		" +-[ ] Decapitate                                                           \r\n"
		" +-[ ] Disarm                                                               \r\n"
		" +-[ ] Limb Damage                                                          \r\n"
		" +-[ ] Tracking Ammunition                                                  \r\n"
		" +-[ ] Crafting Exotic Bullets                                              \r\n"
		" +-[ ] Exploding Corpse Shot                                                \r\n"
		/*
		 * Let's double back around on this in the future. Seems too overboard      
		 * for a single feature.
		"  +                                                                         \r\n"
		"  +-[ ] Chained Combos                                                      \r\n"
		"     +                                                                      \r\n"
		"     +-[ ] Acid Bath                                                        \r\n"
		"     +-[ ] Stained And Marked                                               \r\n"
		"     +-[ ] EMP                                                              \r\n"
		"     +-[ ] Concussion                                                       \r\n"
		"     +-[ ] Scan Grenade                                                     \r\n"
		"     +-[ ]                                                                  \r\n"
		*/
		"[ ] - Double Shot                                                           \r\n"
		"[ ] - Crippling Shot                                                        \r\n"
		"[ ] - Fast Draw Secondary                                                   \r\n"
		"[ ] - Interrogate                                                           \r\n"
		"[ ] - Live off the elements                                                 \r\n"
		"[ ] - Nano Bot Tracking Ammo                                                \r\n"
	;
	return skillset;
}

/*
 * The code to interpret a class letter -- used in interpreter.c when a
 * new character is selecting a class and by 'set class' in act.wizard.c.
 */

player_class_t parse_class(std::string a) {
	static std::map<std::string,player_class_t> class_map;
	static bool initialized = false;
	if(!initialized){
		class_map["1G"] = player_class_t::CLASS_GHOST;
		class_map["1M"] = player_class_t::CLASS_MARKSMAN;
		class_map["1B"] = player_class_t::CLASS_BANDIT;
		class_map["2B"] = player_class_t::CLASS_BUTCHER;
		class_map["2S"] = player_class_t::CLASS_STRIKER;
		class_map["2M"] = player_class_t::CLASS_OBSTRUCTOR;
		class_map["3M"] = player_class_t::CLASS_MALADY;
		class_map["3P"] = player_class_t::CLASS_PYREXIA;
		class_map["3D"] = player_class_t::CLASS_DEALER;
		class_map["4F"] = player_class_t::CLASS_FORGE;
		class_map["4S"] = player_class_t::CLASS_SYNDROME;
		class_map["4M"] = player_class_t::CLASS_MACHINIST;
		initialized = true;
	}
	std::string trimmed = "";
	for(auto ch : a){
		if(isspace(ch)){ continue; }
		if(isalpha(ch) && islower(ch)){
			trimmed += toupper(ch);
			break;
		}
		if(isalpha(ch) && isupper(ch)){
			trimmed += ch;
			break;
		}
		if(isdigit(ch)){
			trimmed += ch;
		}
		if(trimmed.length() >= 2){
			break;
		}
	}
	std::cerr << "[trimmed]: '" << trimmed << "'\n";
	if(trimmed.length() < 2){
		return player_class_t::CLASS_UNDEFINED;
	}
	auto it = class_map.find(trimmed);
	if(it != class_map.end()){
		return it->second;
	}
	return player_class_t::CLASS_UNDEFINED;
}

/*
 * bitvectors (i.e., powers of two) for each class, mainly for use in
 * do_who and do_users.  Add new classes at the end so that all classes
 * use sequential powers of two (1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4,
 * 1 << 5, etc.) up to the limit of your bitvector_t, typically 0-31.
 */
bitvector_t find_class_bitvector(const char *arg) {
	size_t rpos, ret = 0;

		std::string s = "";
	for(rpos = 0; rpos < strlen(arg); rpos++) {
		std::cerr << "[FIXME] find_class_bitvector may be broken...\n";
		s = "";
		s += arg[rpos];
		ret |= (1 << parse_class(s));
	}

	return (ret);
}


/*
 * These are definitions which control the guildmasters for each class.
 *
 * The first field (top line) controls the highest percentage skill level
 * a character of the class is allowed to attain in any skill.  (After
 * this level, attempts to practice will say "You are already learned in
 * this area."
 *
 * The second line controls the maximum percent gain in learnedness a
 * character is allowed per practice -- in other words, if the random
 * die throw comes out higher than this number, the gain will only be
 * this number instead.
 *
 * The third line controls the minimu percent gain in learnedness a
 * character is allowed per practice -- in other words, if the random
 * die throw comes out below this number, the gain will be set up to
 * this number.
 *
 * The fourth line simply sets whether the character knows 'spells'
 * or 'skills'.  This does not affect anything except the message given
 * to the character when trying to practice (i.e. "You know of the
 * following spells" vs. "You know of the following skills"
 */

#define SPELL	0
#define SKILL	1

/* #define LEARNED_LEVEL	0  % known which is considered "learned" */
/* #define MAX_PER_PRAC		1  max percent gain in skill per practice */
/* #define MIN_PER_PRAC		2  min percent gain in skill per practice */
/* #define PRAC_TYPE		3  should it say 'spell' or 'skill'?	*/

int prac_params[4][NUM_CLASSES] = {
	/* MAG	CLE	THE	WAR */
	{ 95,		95,	85,	80	},	/* learned level */
	{ 100,	100,	12,	12	},	/* max per practice */
	{ 25,		25,	0,	0	},	/* min per practice */
	{ SPELL,	SPELL,	SKILL,	SKILL	},	/* prac name */
};


/*
 * ...And the appropriate rooms for each guildmaster/guildguard; controls
 * which types of people the various guildguards let through.  i.e., the
 * first line shows that from room 3017, only MAGIC_USERS are allowed
 * to go south.
 *
 * Don't forget to visit spec_assign.c if you create any new mobiles that
 * should be a guild master or guard so they can act appropriately. If you
 * "recycle" the existing mobs that are used in other guilds for your new
 * guild, then you don't have to change that file, only here.
 */
struct guild_info_type guild_info[] = {

	/* Midgaard */
	/** FIXME */
	{ CLASS_ENGINEER,	3017,	SCMD_SOUTH	},
	{ CLASS_MEDIC,	3004,	SCMD_NORTH	},
	{ CLASS_SNIPER,	3027,	SCMD_EAST	},
	{ CLASS_PSYOP,	3027,	SCMD_DOWN	},
	{ CLASS_SUPPORT,	3021,	SCMD_UP	},

	/* Brass Dragon */
	{ -999 /* all */ ,	5065,	SCMD_WEST	},

	/* this must go last -- add new guards above! */
	{ -1, NOWHERE, -1}
};



/* THAC0 for classes and levels.  (To Hit Armor Class 0) */
int thaco(int class_num, int level) {
	switch(class_num) {
		case CLASS_ENGINEER:
			switch(level) {
				case  0:
					return 100;

				case  1:
					return  20;

				case  2:
					return  20;

				case  3:
					return  20;

				case  4:
					return  19;

				case  5:
					return  19;

				case  6:
					return  19;

				case  7:
					return  18;

				case  8:
					return  18;

				case  9:
					return  18;

				case 10:
					return  17;

				case 11:
					return  17;

				case 12:
					return  17;

				case 13:
					return  16;

				case 14:
					return  16;

				case 15:
					return  16;

				case 16:
					return  15;

				case 17:
					return  15;

				case 18:
					return  15;

				case 19:
					return  14;

				case 20:
					return  14;

				case 21:
					return  14;

				case 22:
					return  13;

				case 23:
					return  13;

				case 24:
					return  13;

				case 25:
					return  12;

				case 26:
					return  12;

				case 27:
					return  12;

				case 28:
					return  11;

				case 29:
					return  11;

				case 30:
					return  11;

				case 31:
					return  10;

				case 32:
					return  10;

				case 33:
					return  10;

				case 34:
					return   9;

				default:
					log("SYSERR: Missing level for mage thac0.");
			}

		case CLASS_MEDIC:
			switch(level) {
				case  0:
					return 100;

				case  1:
					return  20;

				case  2:
					return  20;

				case  3:
					return  20;

				case  4:
					return  18;

				case  5:
					return  18;

				case  6:
					return  18;

				case  7:
					return  16;

				case  8:
					return  16;

				case  9:
					return  16;

				case 10:
					return  14;

				case 11:
					return  14;

				case 12:
					return  14;

				case 13:
					return  12;

				case 14:
					return  12;

				case 15:
					return  12;

				case 16:
					return  10;

				case 17:
					return  10;

				case 18:
					return  10;

				case 19:
					return   8;

				case 20:
					return   8;

				case 21:
					return   8;

				case 22:
					return   6;

				case 23:
					return   6;

				case 24:
					return   6;

				case 25:
					return   4;

				case 26:
					return   4;

				case 27:
					return   4;

				case 28:
					return   2;

				case 29:
					return   2;

				case 30:
					return   2;

				case 31:
					return   1;

				case 32:
					return   1;

				case 33:
					return   1;

				case 34:
					return   1;

				default:
					log("SYSERR: Missing level for cleric thac0.");
			}

		case CLASS_SNIPER:
			switch(level) {
				case  0:
					return 100;

				case  1:
					return  20;

				case  2:
					return  20;

				case  3:
					return  19;

				case  4:
					return  19;

				case  5:
					return  18;

				case  6:
					return  18;

				case  7:
					return  17;

				case  8:
					return  17;

				case  9:
					return  16;

				case 10:
					return  16;

				case 11:
					return  15;

				case 12:
					return  15;

				case 13:
					return  14;

				case 14:
					return  14;

				case 15:
					return  13;

				case 16:
					return  13;

				case 17:
					return  12;

				case 18:
					return  12;

				case 19:
					return  11;

				case 20:
					return  11;

				case 21:
					return  10;

				case 22:
					return  10;

				case 23:
					return   9;

				case 24:
					return   9;

				case 25:
					return   8;

				case 26:
					return   8;

				case 27:
					return   7;

				case 28:
					return   7;

				case 29:
					return   6;

				case 30:
					return   6;

				case 31:
					return   5;

				case 32:
					return   5;

				case 33:
					return   4;

				case 34:
					return   4;

				default:
					log("SYSERR: Missing level for thief thac0.");
			}

		case CLASS_MARINE:
			switch(level) {
				case  0:
					return 100;

				case  1:
					return  20;

				case  2:
					return  19;

				case  3:
					return  18;

				case  4:
					return  17;

				case  5:
					return  16;

				case  6:
					return  15;

				case  7:
					return  14;

				case  8:
					return  14;

				case  9:
					return  13;

				case 10:
					return  12;

				case 11:
					return  11;

				case 12:
					return  10;

				case 13:
					return   9;

				case 14:
					return   8;

				case 15:
					return   7;

				case 16:
					return   6;

				case 17:
					return   5;

				case 18:
					return   4;

				case 19:
					return   3;

				case 20:
					return   2;

				case 21:
					return   1;

				case 22:
					return   1;

				case 23:
					return   1;

				case 24:
					return   1;

				case 25:
					return   1;

				case 26:
					return   1;

				case 27:
					return   1;

				case 28:
					return   1;

				case 29:
					return   1;

				case 30:
					return   1;

				case 31:
					return   1;

				case 32:
					return   1;

				case 33:
					return   1;

				case 34:
					return   1;

				default:
					log("SYSERR: Missing level for warrior thac0.");
			}

		default:
			log("SYSERR: Unknown class in thac0 chart.");
	}

	/* Will not get there unless something is wrong. */
	return 100;
}

/*
 * Roll the 6 stats for a character... each stat is made of the sum of
 * the best 3 out of 4 rolls of a 6-sided die.  Each class then decides
 * which priority will be given for the best to worst stats.
 */
void roll_real_abils(char_data *ch) {
	int i, j, k, temp;
	ubyte table[6];
	ubyte rolls[4];

	for(i = 0; i < 6; i++) {
		table[i] = 0;
	}

	for(i = 0; i < 6; i++) {

		for(j = 0; j < 4; j++) {
			rolls[j] = rand_number(1, 6);
		}

		temp = rolls[0] + rolls[1] + rolls[2] + rolls[3] -
		       MIN(rolls[0], MIN(rolls[1], MIN(rolls[2], rolls[3])));

		for(k = 0; k < 6; k++)
			if(table[k] < temp) {
				temp ^= table[k];
				table[k] ^= temp;
				temp ^= table[k];
			}
	}

	ch->real_abils.str_add = 0;


	switch(GET_CLASS(ch)) {
		case CLASS_ENGINEER:
			ch->real_abils.intel = table[0];
			ch->real_abils.wis = table[1];
			ch->real_abils.dex = table[2];
			ch->real_abils.str = table[3];
			ch->real_abils.con = table[4];
			ch->real_abils.cha = table[5];

		ch->real_abils.electronics = table[0];
		ch->real_abils.armor = table[3];
		ch->real_abils.marksmanship = table[2];
		ch->real_abils.sniping = table[2];
		ch->real_abils.demolitions = table[0];
		ch->real_abils.chemistry = table[0];
		ch->real_abils.weapon_handling = table[3];
		ch->real_abils.strategy = table[0];
		ch->real_abils.medical = table[5];
			break;

		case CLASS_MEDIC:
			ch->real_abils.wis = table[0];
			ch->real_abils.intel = table[1];
			ch->real_abils.str = table[2];
			ch->real_abils.dex = table[3];
			ch->real_abils.con = table[4];
			ch->real_abils.cha = table[5];
			break;

		case CLASS_SNIPER:
			ch->real_abils.dex = table[0];
			ch->real_abils.str = table[1];
			ch->real_abils.con = table[2];
			ch->real_abils.intel = table[3];
			ch->real_abils.wis = table[4];
			ch->real_abils.cha = table[5];
			break;

		case CLASS_MARINE:
			ch->real_abils.str = table[0];
			ch->real_abils.dex = table[1];
			ch->real_abils.con = table[2];
			ch->real_abils.wis = table[3];
			ch->real_abils.intel = table[4];
			ch->real_abils.cha = table[5];

			if(ch->real_abils.str == 18) {
				ch->real_abils.str_add = rand_number(0, 100);
			}

			break;
	}

	ch->aff_abils = ch->real_abils;
}


/* Some initializations for characters, including initial skills */
void do_start(char_data *ch) {
	MENTOC_PREAMBLE();
	player->sendln("do_start");
	player->level() = 0;
	mods::levels::gain_exp(player,0);

	set_title(player,"");
	roll_real_abils(ch);

	GET_MAX_HIT(ch)  = 10;
	GET_MAX_MANA(ch) = 100;
	GET_MAX_MOVE(ch) = 82;

	//advance_level(ch);
	//mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "%s advanced to level %d", GET_NAME(ch).c_str(), GET_LEVEL(ch));

	GET_HIT(ch) = GET_MAX_HIT(ch);
	GET_MANA(ch) = GET_MAX_MANA(ch);
	GET_MOVE(ch) = GET_MAX_MOVE(ch);

	GET_COND(ch, THIRST) = 24;
	GET_COND(ch, FULL) = 24;
	GET_COND(ch, DRUNK) = 0;

	if(siteok_everyone) {
		SET_BIT(PLR_FLAGS(ch), PLR_SITEOK);
	}
}



/*
 * This function controls the change to maxmove, maxmana, and maxhp for
 * each class every time they gain a level.
 */
void advance_level(char_data *ch) {
	MENTOC_PREAMBLE();
	mods::levels::advance_level(player);
	
	snoop_check(ch);
	mods::levels::reward_player_for_advancing_levels(player);
	mods::db::save_char(player);
}


/*
 * This simply calculates the backstab multiplier based on a character's
 * level.  This used to be an array, but was changed to be a function so
 * that it would be easier to add more levels to your MUD.  This doesn't
 * really create a big performance hit because it's not used very often.
 */
int backstab_mult(int level) {
	if(level <= 0) {
		return 1;    /* level 0 */
	} else if(level <= 7) {
		return 2;    /* level 1 - 7 */
	} else if(level <= 13) {
		return 3;    /* level 8 - 13 */
	} else if(level <= 20) {
		return 4;    /* level 14 - 20 */
	} else if(level <= 28) {
		return 5;    /* level 21 - 28 */
	} else if(level < LVL_IMMORT) {
		return 6;    /* all remaining mortal levels */
	} else {
		return 20;    /* immortals */
	}
}


/*
 * invalid_class is used by handler.c to determine if a piece of equipment is
 * usable by a particular class, based on the ITEM_ANTI_{class} bitvectors.
 */
int invalid_class(char_data *ch, struct obj_data *obj) {
	if(OBJ_FLAGGED(obj, ITEM_ANTI_MAGIC_USER) && IS_MAGIC_USER(ch)) {
		return TRUE;
	}

	if(OBJ_FLAGGED(obj, ITEM_ANTI_CLERIC) && IS_CLERIC(ch)) {
		return TRUE;
	}

	if(OBJ_FLAGGED(obj, ITEM_ANTI_WARRIOR) && IS_MARINE(ch)) {
		return TRUE;
	}

	if(OBJ_FLAGGED(obj, ITEM_ANTI_THIEF) && IS_THIEF(ch)) {
		return TRUE;
	}

	return FALSE;
}


/*
 * SPELLS AND SKILLS.  This area defines which spells are assigned to
 * which classes, and the minimum level the character must be to use
 * the spell or skill.
 */
void init_spell_levels(void) {
}


/*
 * This is the exp given to implementors -- it must always be greater
 * than the exp required for immortality, plus at least 20,000 or so.
 */
#define EXP_MAX  10000000

/* Function to return the exp required for each class/level */


/*
 * Default titles of male characters.
 */
const char *title_male(int chclass, int level) {
	if(level <= 0 || level > LVL_IMPL) {
		return "the Man";
	}

	if(level == LVL_IMPL) {
		return "the Implementor";
	}
	//player_class_t chclass = (player_class_t)in_chclass;

	if(chclass == GHOST) {
		return "the Ghost";
	}
	if(chclass == MARKSMAN){
		return "the Marksman";
	}
	if(chclass == BANDIT){
		return "the Bandit";
	}
	if(chclass == BUTCHER){
		return "the Butcher";
	}
	if(chclass == STRIKER){
		return "the Striker";
	}
	if(chclass == OBSTRUCTOR){
		return "the Obstructor";
	}
	if(chclass == MALADY){
		return "the Malady";
	}
	if(chclass == PYREXIA){
		return "the Pyrexia";
	}
	if(chclass == DEALER){
		return "the Dealer";
	}
	if(chclass == FORGE){
		return "the Forge";
	}
	if(chclass == SYNDROME){
		return "the Syndrome";
	}
	if(chclass == MACHINIST){
		return "the Machinist";
	}

	/* Default title for classes which do not have titles defined */
	return "the Classless";
}


/*
 * Default titles of female characters.
 */
const char *title_female(int chclass, int level) {
	if(level <= 0 || level > LVL_IMPL) {
		return "the Woman";
	}

	if(level == LVL_IMPL) {
		return "the Implementress";
	}
	return title_male(chclass,level);
}

