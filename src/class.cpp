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


const char* class_menu =
    "  [ Sniper               [          ROGUE ARCHETYPE ]\r\n"
		"     -> [1G] GHOST       [         stealth operator ]\r\n" 
		"     -> [1M] Marksman    [           weapons expert ]\r\n" 
		"     -> [1B] Bandit      [                    thief ]\r\n" 
		"  ===================================================\r\n"
		"  [ Marine               [        WARRIOR ARCHETYPE ]\r\n"
		"     -> [2B] Butcher     [ bladed weapon specialist ]\r\n"
		"     -> [2S] Striker     [      martial arts expert ]\r\n"
		"     -> [2M] Obstructor  [       submissions expert ]\r\n"
		"  ===================================================\r\n"
		"  [ Chemist              [      ALCHEMIST ARCHETYPE ]\r\n"
		"     -> [3M] Malady      [ poison weapon specialist ]\r\n"
		"     -> [3P] Pyrexia     [    incendiary specialist ]\r\n"
		"     -> [3D] Dealer      [   enhancement specialist ]\r\n"
		"  ===================================================\r\n"
		"  [ Engineer             [     BLACKSMITH ARCHETYPE ]\r\n"
		"     -> [4F] Forge       [          weapons crafter ]\r\n"
		"     -> [4S] Syndrome    [  autonomous drone expert ]\r\n"
		"     -> [4M] Machinist   [        technology expert ]\r\n"
		"  ===================================================\r\n"
		"\r\n"
		"NOTE: For a description of a class, type a question mark then the number.\r\n"
		"For example: ?1 will give you a description of the Sniper class\r\n"
		"\r\n"
		;

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
	player->level() = 1;
	player->exp() = 1;

	set_title(player,"");
	roll_real_abils(ch);

	GET_MAX_HIT(ch)  = 10;
	GET_MAX_MANA(ch) = 100;
	GET_MAX_MOVE(ch) = 82;

	/*
	switch(GET_CLASS(ch)) {

		case CLASS_ENGINEER:
			break;

		case CLASS_MEDIC:
			break;

		case CLASS_SNIPER:
			SET_SKILL(ch, SKILL_SNEAK, 10);
			SET_SKILL(ch, SKILL_HIDE, 5);
			SET_SKILL(ch, SKILL_STEAL, 15);
			SET_SKILL(ch, SKILL_BACKSTAB, 10);
			SET_SKILL(ch, SKILL_PICK_LOCK, 10);
			SET_SKILL(ch, SKILL_TRACK, 10);
			break;

		case CLASS_MARINE:
			break;
	}
	*/

	advance_level(ch);
	mudlog(BRF, MAX(LVL_IMMORT, GET_INVIS_LEV(ch)), TRUE, "%s advanced to level %d", GET_NAME(ch).c_str(), GET_LEVEL(ch));

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
	std::cerr << "[stub][FIXME] advance_level is currently broken [2020-03-03]\n";
	return;
	/*
	MENTOC_PREAMBLE();
	int add_hp, add_mana = 0, add_move = 0, i;

	add_hp = con_app[GET_CON(ch)].hitp;

	switch(GET_CLASS(ch)) {

		case CLASS_ENGINEER:
			add_hp += rand_number(3, 8);
			add_mana = rand_number(GET_LEVEL(ch), (int)(1.5 * GET_LEVEL(ch)));
			add_mana = MIN(add_mana, 10);
			add_move = rand_number(0, 2);
			break;

		case CLASS_MEDIC:
			add_hp += rand_number(5, 10);
			add_mana = rand_number(GET_LEVEL(ch), (int)(1.5 * GET_LEVEL(ch)));
			add_mana = MIN(add_mana, 10);
			add_move = rand_number(0, 2);
			break;

		case CLASS_SNIPER:
			add_hp += rand_number(7, 13);
			add_mana = 0;
			add_move = rand_number(1, 3);
			break;

		case CLASS_MARINE:
			add_hp += rand_number(10, 15);
			add_mana = 0;
			add_move = rand_number(1, 3);
			break;
	}

	ch->points.max_hit += MAX(1, add_hp);
	ch->points.max_move += MAX(1, add_move);

	if(GET_LEVEL(ch) > 1) {
		ch->points.max_mana += add_mana;
	}

	if(GET_LEVEL(ch) >= LVL_IMMORT) {
		for(i = 0; i < 3; i++) {
			GET_COND(ch, i) = (char) -1;
		}

		SET_BIT(PRF_FLAGS(ch), PRF_HOLYLIGHT);
	}

	snoop_check(ch);
	mods::db::save_char(player);
	*/
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
int level_exp(int chclass, int level) {
	if(level > LVL_IMPL || level < 0) {
		log("SYSERR: Requesting exp for invalid level %d!", level);
		return 0;
	}

	/*
	 * Gods have exp close to EXP_MAX.  This statement should never have to
	 * changed, regardless of how many mortal or immortal levels exist.
	 */
	if(level > LVL_IMMORT) {
		return EXP_MAX - ((LVL_IMPL - level) * 1000);
	}

	/* Exp required for normal mortals is below */

	switch(chclass) {

		case CLASS_ENGINEER:
			switch(level) {
				case  0:
					return 0;

				case  1:
					return 1;

				case  2:
					return 2500;

				case  3:
					return 5000;

				case  4:
					return 10000;

				case  5:
					return 20000;

				case  6:
					return 40000;

				case  7:
					return 60000;

				case  8:
					return 90000;

				case  9:
					return 135000;

				case 10:
					return 250000;

				case 11:
					return 375000;

				case 12:
					return 750000;

				case 13:
					return 1125000;

				case 14:
					return 1500000;

				case 15:
					return 1875000;

				case 16:
					return 2250000;

				case 17:
					return 2625000;

				case 18:
					return 3000000;

				case 19:
					return 3375000;

				case 20:
					return 3750000;

				case 21:
					return 4000000;

				case 22:
					return 4300000;

				case 23:
					return 4600000;

				case 24:
					return 4900000;

				case 25:
					return 5200000;

				case 26:
					return 5500000;

				case 27:
					return 5950000;

				case 28:
					return 6400000;

				case 29:
					return 6850000;

				case 30:
					return 7400000;

				/* add new levels here */
				case LVL_IMMORT:
					return 8000000;
			}

			break;

		case CLASS_MEDIC:
			switch(level) {
				case  0:
					return 0;

				case  1:
					return 1;

				case  2:
					return 1500;

				case  3:
					return 3000;

				case  4:
					return 6000;

				case  5:
					return 13000;

				case  6:
					return 27500;

				case  7:
					return 55000;

				case  8:
					return 110000;

				case  9:
					return 225000;

				case 10:
					return 450000;

				case 11:
					return 675000;

				case 12:
					return 900000;

				case 13:
					return 1125000;

				case 14:
					return 1350000;

				case 15:
					return 1575000;

				case 16:
					return 1800000;

				case 17:
					return 2100000;

				case 18:
					return 2400000;

				case 19:
					return 2700000;

				case 20:
					return 3000000;

				case 21:
					return 3250000;

				case 22:
					return 3500000;

				case 23:
					return 3800000;

				case 24:
					return 4100000;

				case 25:
					return 4400000;

				case 26:
					return 4800000;

				case 27:
					return 5200000;

				case 28:
					return 5600000;

				case 29:
					return 6000000;

				case 30:
					return 6400000;

				/* add new levels here */
				case LVL_IMMORT:
					return 7000000;
			}

			break;

		case CLASS_SNIPER:
			switch(level) {
				case  0:
					return 0;

				case  1:
					return 1;

				case  2:
					return 1250;

				case  3:
					return 2500;

				case  4:
					return 5000;

				case  5:
					return 10000;

				case  6:
					return 20000;

				case  7:
					return 40000;

				case  8:
					return 70000;

				case  9:
					return 110000;

				case 10:
					return 160000;

				case 11:
					return 220000;

				case 12:
					return 440000;

				case 13:
					return 660000;

				case 14:
					return 880000;

				case 15:
					return 1100000;

				case 16:
					return 1500000;

				case 17:
					return 2000000;

				case 18:
					return 2500000;

				case 19:
					return 3000000;

				case 20:
					return 3500000;

				case 21:
					return 3650000;

				case 22:
					return 3800000;

				case 23:
					return 4100000;

				case 24:
					return 4400000;

				case 25:
					return 4700000;

				case 26:
					return 5100000;

				case 27:
					return 5500000;

				case 28:
					return 5900000;

				case 29:
					return 6300000;

				case 30:
					return 6650000;

				/* add new levels here */
				case LVL_IMMORT:
					return 7000000;
			}

			break;

		case CLASS_MARINE:
			switch(level) {
				case  0:
					return 0;

				case  1:
					return 1;

				case  2:
					return 2000;

				case  3:
					return 4000;

				case  4:
					return 8000;

				case  5:
					return 16000;

				case  6:
					return 32000;

				case  7:
					return 64000;

				case  8:
					return 125000;

				case  9:
					return 250000;

				case 10:
					return 500000;

				case 11:
					return 750000;

				case 12:
					return 1000000;

				case 13:
					return 1250000;

				case 14:
					return 1500000;

				case 15:
					return 1850000;

				case 16:
					return 2200000;

				case 17:
					return 2550000;

				case 18:
					return 2900000;

				case 19:
					return 3250000;

				case 20:
					return 3600000;

				case 21:
					return 3900000;

				case 22:
					return 4200000;

				case 23:
					return 4500000;

				case 24:
					return 4800000;

				case 25:
					return 5150000;

				case 26:
					return 5500000;

				case 27:
					return 5950000;

				case 28:
					return 6400000;

				case 29:
					return 6850000;

				case 30:
					return 7400000;

				/* add new levels here */
				case LVL_IMMORT:
					return 8000000;
			}

			break;
	}

	/*
	 * This statement should never be reached if the exp tables in this function
	 * are set up properly.  If you see exp of 123456 then the tables above are
	 * incomplete -- so, complete them!
	 */
	log("SYSERR: XP tables not set up correctly in class.c!");
	return 123456;
}


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

	switch(chclass) {

		case CLASS_ENGINEER:
			switch(level) {
				case  1:
					return "the Apprentice Engineer";

				case  2:
					return "the Apprentice Engineer";

				case  3:
					return "the Aspiring Engineer";

				case  4:
					return "the Aspiring Engineer";

				case  5:
					return "the Engineer";

				case  6:
					return "the Engineer";

				case  7:
					return "the Engineer";

				case  8:
					return "the Engineer";

				case  9:
					return "the Gifted Engineer";

				case 10:
					return "the Gifted Engineer";

				case 11:
					return "the Blackbox Hacker";

				case 12:
					return "the Blackbox Hacker";

				case 13:
					return "the Gifted Hacker";

				case 14:
					return "the Gifted Hacker";

				case 15:
					return "the Master Engineer";

				case 16:
					return "the Savant Engineer";

				case 17:
					return "the Crafty Veteran Engineer";

				case 18:
					return "the Engineering Wizard";

				case 19:
					return "the Elite Engineer";

				case 20:
					return "the Elite Engineer";

				case 21:
					return "the Elite Engineer";

				case 22:
					return "the Legendary Engineer";

				case 23:
					return "the Legendary Engineer";

				case 24:
					return "the Disciple of Engineering";

				case 25:
					return "the Demi-god of Engineering";

				case 26:
					return "the Greater Demi-god of Engineering";

				case 27:
					return "the Master Craftsman";

				case 28:
					return "Godlike Engineer";

				case 29:
					return "Weaver of Engineering feats";

				case 30:
					return "Big Boss of Engineering";

				case LVL_IMMORT:
					return "the Immortal Engineer";

				case LVL_GOD:
					return "the Godlike Engineer";

				case LVL_GRGOD:
					return "the God of Engineering";

				default:
					return "the Engineer";
			}

			break;

		case CLASS_MEDIC:
			switch(level) {
				case  1:
					return "the Apprentice Medic";

				case  2:
					return "the Apprentice Medic";

				case  3:
					return "the Aspiring Medic";

				case  4:
					return "the Aspiring Medic";

				case LVL_IMMORT:
					return "the Immortal Medic";

				case LVL_GOD:
					return "the Doctor of Medicine";

				case LVL_GRGOD:
					return "the God of Medicine";

				default:
					return "the Medic";
			}

		case CLASS_SNIPER:
			switch(level) {
				case  1:
					return "the Apprentice Sniper";

				case  2:
					return "the Apprentice Sniper";

				case  3:
					return "the Aspiring Sniper";

				case  4:
					return "the Aspiring Sniper";

				case  5:
					return "the Marksman";

				case  6:
					return "the Marksman";

				case  7:
					return "the Marksman";

				case  8:
					return "the Marksman";

				case  9:
					return "the Sharp Shooter";

				case 10:
					return "the Sharp Shooter";

				case 11:
					return "the Rogue Sharp Shooter";

				case 12:
					return "the Rogue Sharp Shooter";

				case 13:
					return "the Elite Sharp Shooter";

				case 14:
					return "the Gifted Sharp Shooter";

				case 15:
					return "the Master Sharp Shooter";

				case 16:
					return "the Master Marksman";

				case 17:
					return "the Elite Sniper";

				case 18:
					return "the Elite Sniper";

				case 19:
					return "the Elite Sniper";

				case 20:
					return "the Legendary Sniper";

				/* no one ever thought up these titles 21-30 */
				case LVL_IMMORT:
					return "the Immortal Sniper";

				case LVL_GOD:
					return "the Godlike Sniper";

				case LVL_GRGOD:
					return "the God of Sniping";

				default:
					return "the Sniper";
			}

			break;

		case CLASS_MARINE:
			switch(level) {
				case  1:
				case  2:
					return "the Recruit";

				case LVL_IMMORT:
					return "the Immortal Warlord";

				case LVL_GOD:
					return "the Extirpator";

				case LVL_GRGOD:
					return "the God of war";

				default:
					return "the Marine";
			}

			break;
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

	switch(chclass) {

		case CLASS_ENGINEER:
			switch(level) {
				case  1:
					return "the Apprentice Engineer";

				case  2:
					return "the Apprentice Engineer";

				case  3:
					return "the Aspiring Engineer";

				case  4:
					return "the Aspiring Engineer";

				case  5:
					return "the Engineer";

				case  6:
					return "the Engineer";

				case  7:
					return "the Engineer";

				case  8:
					return "the Engineer";

				case  9:
					return "the Gifted Engineer";

				case 10:
					return "the Gifted Engineer";

				case 11:
					return "the Blackbox Hacker";

				case 12:
					return "the Blackbox Hacker";

				case 13:
					return "the Gifted Hacker";

				case 14:
					return "the Gifted Hacker";

				case 15:
					return "the Master Engineer";

				case 16:
					return "the Savant Engineer";

				case 17:
					return "the Crafty Veteran Engineer";

				case 18:
					return "the Engineering Wizard";

				case 19:
					return "the Elite Engineer";

				case 20:
					return "the Elite Engineer";

				case 21:
					return "the Elite Engineer";

				case 22:
					return "the Legendary Engineer";

				case 23:
					return "the Legendary Engineer";

				case 24:
					return "the Disciple of Engineering";

				case 25:
					return "the Demi-goddess of Engineering";

				case 26:
					return "the Greater Demi-goddess of Engineering";

				case 27:
					return "the Master Craftswoman";

				case 28:
					return "Godlike Engineer";

				case 29:
					return "Weaver of Engineering feats";

				case 30:
					return "Big Boss of Engineering";

				case LVL_IMMORT:
					return "the Immortal Engineer";

				case LVL_GOD:
					return "the Empress of Engineering";

				case LVL_GRGOD:
					return "the Goddess of Engineering";

				default:
					return "the Engineer";
			}

			break;

		case CLASS_MEDIC:
			switch(level) {
				case  1:
					return "the Apprentice Medic";

				case  2:
					return "the Apprentice Medic";

				case  3:
					return "the Aspiring Medic";

				case  4:
					return "the Aspiring Medic";

				/* no one ever thought up these titles 21-30 */
				case LVL_IMMORT:
					return "the Immortal Medic";

				case LVL_GOD:
					return "the Empress of Medicine";

				case LVL_GRGOD:
					return "the Goddess of Medicine";

				default:
					return "the Medic";
			}

			break;

		case CLASS_SNIPER:
			switch(level) {
				case  1:
					return "the Apprentice Sniper";

				case  2:
					return "the Apprentice Sniper";

				case  3:
					return "the Aspiring Sniper";

				case  4:
					return "the Aspiring Sniper";

				case  5:
					return "the Marksman";

				case  6:
					return "the Marksman";

				case  7:
					return "the Marksman";

				case  8:
					return "the Marksman";

				case  9:
					return "the Sharp Shooter";

				case 10:
					return "the Sharp Shooter";

				case 11:
					return "the Rogue Sharp Shooter";

				case 12:
					return "the Rogue Sharp Shooter";

				case 13:
					return "the Elite Sharp Shooter";

				case 14:
					return "the Gifted Sharp Shooter";

				case 15:
					return "the Master Sharp Shooter";

				case 16:
					return "the Master Marksman";

				case 17:
					return "the Elite Sniper";

				case 18:
					return "the Elite Sniper";

				case 19:
					return "the Elite Sniper";

				case 20:
					return "the Legendary Sniper";

				/* no one ever thought up these titles 21-30 */
				case LVL_IMMORT:
					return "the Immortal Sniper";

				case LVL_GOD:
					return "the Godlike Sniper";

				case LVL_GRGOD:
					return "the God of Sniping";

				default:
					return "the Sniper";
			}

			break;

		case CLASS_MARINE:
			switch(level) {
				case  1:
					return "the Recruit";

				case  2:
					return "the Recruit";

				case LVL_IMMORT:
					return "the Immortal Lady of War";

				case LVL_GOD:
					return "the Queen of Destruction";

				case LVL_GRGOD:
					return "the Goddess of war";

				default:
					return "the Marine";
			}

			break;
	}

	/* Default title for classes which do not have titles defined */
	return "the Classless";
}

