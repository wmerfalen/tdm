/* ************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#ifndef __CIRCLEMUD_STRUCTS_HEADER__
#define __CIRCLEMUD_STRUCTS_HEADER__

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include "mods/string.hpp"
#include "mods/ai_state.hpp"
#include <functional>
#include <array>
#include "mods/extra_desc_data.hpp"
#include <unordered_map>
namespace mods {
	struct player;
	struct npc;
	struct descriptor_data;
	struct extra_desc_data;
};
struct char_data;
extern std::deque<mods::descriptor_data> descriptor_list;
using weapon_type_t = uint64_t;
using ai_state_map = std::map<char_data*,std::unique_ptr<mods::ai_state>>;
using memory_rec_t = std::set<char_data*>;
using ai_state_t = short;
using goal_t = short;
struct obj_data;
using uuid_t = uint64_t;
using aligned_int_t = uint64_t;
enum lense_type_t {
	FIRST,

	NORMAL_SIGHT,
	THERMAL_GOGGLES,
	NIGHT_VISION_GOGGLES,

	AERIAL_DRONE,
	AERIAL_DRONE_THERMAL,
	AERIAL_DRONE_NIGHT_VISION,

	RC_DRONE,
	RC_DRONE_THERMAL,
	RC_DRONE_NIGHT_VISION,

	LAST
};

/*
 * Intended use of this macro is to allow external packages to work with
 * a variety of CircleMUD versions without modifications.  For instance,
 * an IS_CORPSE() macro was introduced in pl13.  Any future code add-ons
 * could take into account the CircleMUD version and supply their own
 * definition for the macro if used on an older version of CircleMUD.
 * You are supposed to compare this with the macro CIRCLEMUD_VERSION()
 * in utils.h.  See there for usage.
 */
#define _CIRCLEMUD	0x030100 /* Major/Minor/Patchlevel - MMmmPP */

/*
 * If you want equipment to be automatically equipped to the same place
 * it was when players rented, set the define below to 1.  Please note
 * that this will require erasing or converting all of your rent files.
 * And of course, you have to recompile everything.  We need this feature
 * for CircleMUD to be complete but we refuse to break binary file
 * compatibility.
 */
#define USE_AUTOEQ	0	/* TRUE/FALSE aren't defined yet. */


/* preamble *************************************************************/

/*
 * As of bpl20, it should be safe to use unsigned data types for the
 * various virtual and real number data types.  There really isn't a
 * reason to use signed anymore so use the unsigned types and get
 * 65,535 objects instead of 32,768.
 *
 * NOTE: This will likely be unconditionally unsigned later.
 */
#define CIRCLE_UNSIGNED_INDEX	1	/* 0 = signed, 1 = unsigned */

#if CIRCLE_UNSIGNED_INDEX
#define IDXTYPE  ush_int
# define NOWHERE	((IDXTYPE)~0)
# define NOTHING	((IDXTYPE)~0)
# define NOBODY		((IDXTYPE)~0)
#else
#define IDXTYPE  sh_int
# define NOWHERE	(-1)	/* nil reference for rooms	*/
# define NOTHING	(-1)	/* nil reference for objects	*/
# define NOBODY		(-1)	/* nil reference for mobiles	*/
#endif

#define SPECIAL(name) \
	int (name)(struct char_data *ch, void *me, int cmd, char *argument)


/* room-related defines *************************************************/


/* The cardinal directions: used as index to room_data.dir_option[] */
#define NORTH          0
#define EAST           1
#define SOUTH          2
#define WEST           3
#define UP             4
#define DOWN           5


/* Room flags: used in room_data.room_flags */
/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */
#define ROOM_DARK		(1 << 0)   /* Dark			*/
#define ROOM_DEATH		(1 << 1)   /* Death trap		*/
#define ROOM_NOMOB		(1 << 2)   /* MOBs not allowed		*/
#define ROOM_INDOORS		(1 << 3)   /* Indoors			*/
#define ROOM_PEACEFUL		(1 << 4)   /* Violence not allowed	*/
#define ROOM_SOUNDPROOF		(1 << 5)   /* Shouts, gossip blocked	*/
#define ROOM_NOTRACK		(1 << 6)   /* Track won't go through	*/
#define ROOM_NOMAGIC		(1 << 7)   /* Magic not allowed		*/
#define ROOM_TUNNEL		(1 << 8)   /* room for only 1 pers	*/
#define ROOM_PRIVATE		(1 << 9)   /* Can't teleport in		*/
#define ROOM_GODROOM		(1 << 10)  /* LVL_GOD+ only allowed	*/
#define ROOM_HOUSE		(1 << 11)  /* (R) Room is a house	*/
#define ROOM_HOUSE_CRASH	(1 << 12)  /* (R) House needs saving	*/
#define ROOM_ATRIUM		(1 << 13)  /* (R) The door to a house	*/
#define ROOM_OLC		(1 << 14)  /* (R) Modifyable/!compress	*/
#define ROOM_BFS_MARK		(1 << 15)  /* (R) breath-first srch mrk	*/


/* Exit info: used in room_data.dir_option.exit_info */
#define EX_ISDOOR		(1 << 0)   /* Exit is a door		*/
#define EX_CLOSED		(1 << 1)   /* The door is closed	*/
#define EX_LOCKED		(1 << 2)   /* The door is locked	*/
#define EX_PICKPROOF		(1 << 3)   /* Lock can't be picked	*/
#define EX_REINFORCED		(1 << 4) /* Door is re-inforced */
#define EX_BREACHED			(1 << 5) /* Door has been breached */


/* Sector types: used in room_data.sector_type */
#define SECT_INSIDE          0		   /* Indoors			*/
#define SECT_CITY            1		   /* In a city			*/
#define SECT_FIELD           2		   /* In a field		*/
#define SECT_FOREST          3		   /* In a forest		*/
#define SECT_HILLS           4		   /* In the hills		*/
#define SECT_MOUNTAIN        5		   /* On a mountain		*/
#define SECT_WATER_SWIM      6		   /* Swimmable water		*/
#define SECT_WATER_NOSWIM    7		   /* Water - need a boat	*/
#define SECT_FLYING	     8		   /* Wheee!			*/
#define SECT_UNDERWATER	     9		   /* Underwater		*/


/* char and mob-related defines *****************************************/


/* PC classes */
#define CLASS_UNDEFINED	  (-1)
#define CLASS_MAGIC_USER  0
#define CLASS_CLERIC      1
#define CLASS_SNIPER      2
#define CLASS_WARRIOR     3

#define CLASS_ENGINEER    0
#define CLASS_MEDIC       1
#define CLASS_SNIPER      2
#define CLASS_MARINE      3
#define CLASS_SUPPORT	  4

#define NUM_CLASSES	  5  /* This must be the number of classes!! */

/* NPC classes (currently unused - feel free to implement!) */
#define CLASS_OTHER       0
#define CLASS_UNDEAD      1
#define CLASS_HUMANOID    2
#define CLASS_ANIMAL      3
#define CLASS_DRAGON      4
#define CLASS_GIANT       5


/* Sex */
#define SEX_NEUTRAL   0
#define SEX_MALE      1
#define SEX_FEMALE    2


/* Positions */
#define POS_DEAD       0	/* dead			*/
#define POS_MORTALLYW  1	/* mortally wounded	*/
#define POS_INCAP      2	/* incapacitated	*/
#define POS_STUNNED    3	/* stunned		*/
#define POS_SLEEPING   4	/* sleeping		*/
#define POS_RESTING    5	/* resting		*/
#define POS_SITTING    6	/* sitting		*/
#define POS_FIGHTING   7	/* fighting		*/
#define POS_STANDING   8	/* standing		*/


/* Player flags: used by char_data.char_specials.act */
#define PLR_KILLER	(1 << 0)   /* Player is a player-killer		*/
#define PLR_THIEF	(1 << 1)   /* Player is a player-thief		*/
#define PLR_FROZEN	(1 << 2)   /* Player is frozen			*/
#define PLR_DONTSET     (1 << 3)   /* Don't EVER set (ISNPC bit)	*/
#define PLR_WRITING	(1 << 4)   /* Player writing (board/mail/olc)	*/
#define PLR_MAILING	(1 << 5)   /* Player is writing mail		*/
#define PLR_CRASH	(1 << 6)   /* Player needs to be crash-saved	*/
#define PLR_SITEOK	(1 << 7)   /* Player has been site-cleared	*/
#define PLR_NOSHOUT	(1 << 8)   /* Player not allowed to shout/goss	*/
#define PLR_NOTITLE	(1 << 9)   /* Player not allowed to set title	*/
#define PLR_DELETED	(1 << 10)  /* Player deleted - space reusable	*/
#define PLR_LOADROOM	(1 << 11)  /* Player uses nonstandard loadroom	*/
#define PLR_NOWIZLIST	(1 << 12)  /* Player shouldn't be on wizlist	*/
#define PLR_NODELETE	(1 << 13)  /* Player shouldn't be deleted	*/
#define PLR_INVSTART	(1 << 14)  /* Player should enter game wizinvis	*/
#define PLR_CRYO	(1 << 15)  /* Player is cryo-saved (purge prog)	*/
#define PLR_NOTDEADYET	(1 << 16)  /* (R) Player being extracted.	*/


	/* Mobile flags: used by char_data.char_specials.act */
#define MOB_SPEC         (1 << 0)  /* Mob has a callable spec-proc	*/
#define MOB_SENTINEL     (1 << 1)  /* Mob should not move		*/
#define MOB_SCAVENGER    (1 << 2)  /* Mob picks up stuff on the ground	*/
#define MOB_ISNPC        (1 << 3)  /* (R) Automatically set on all Mobs	*/
#define MOB_AWARE	 (1 << 4)  /* Mob can't be backstabbed		*/
#define MOB_AGGRESSIVE   (1 << 5)  /* Mob auto-attacks everybody nearby	*/
#define MOB_STAY_ZONE    (1 << 6)  /* Mob shouldn't wander out of zone	*/
#define MOB_WIMPY        (1 << 7)  /* Mob flees if severely injured	*/
#define MOB_AGGR_EVIL	 (1 << 8)  /* Auto-attack any evil PC's		*/
#define MOB_AGGR_GOOD	 (1 << 9)  /* Auto-attack any good PC's		*/
#define MOB_AGGR_NEUTRAL (1 << 10) /* Auto-attack any neutral PC's	*/
#define MOB_MEMORY	 (1 << 11) /* remember attackers if attacked	*/
#define MOB_HELPER	 (1 << 12) /* attack PCs fighting other NPCs	*/
#define MOB_NOCHARM	 (1 << 13) /* Mob can't be charmed		*/
#define MOB_NOSUMMON	 (1 << 14) /* Mob can't be summoned		*/
#define MOB_NOSLEEP	 (1 << 15) /* Mob can't be slept		*/
#define MOB_NOBASH	 (1 << 16) /* Mob can't be bashed (e.g. trees)	*/
#define MOB_NOBLIND	 (1 << 17) /* Mob can't be blinded		*/
#define MOB_NOTDEADYET   (1 << 18) /* (R) Mob being extracted.		*/


	/* Preference flags: used by char_data.player_specials.pref */
#define PRF_BRIEF       (1 << 0)  /* Room descs won't normally be shown	*/
#define PRF_COMPACT     (1 << 1)  /* No extra CRLF pair before prompts	*/
#define PRF_DEAF	(1 << 2)  /* Can't hear shouts			*/
#define PRF_NOTELL	(1 << 3)  /* Can't receive tells		*/
#define PRF_DISPHP	(1 << 4)  /* Display hit points in prompt	*/
#define PRF_DISPMANA	(1 << 5)  /* Display mana points in prompt	*/
#define PRF_DISPMOVE	(1 << 6)  /* Display move points in prompt	*/
#define PRF_AUTOEXIT	(1 << 7)  /* Display exits in a room		*/
#define PRF_NOHASSLE	(1 << 8)  /* Aggr mobs won't attack		*/
#define PRF_QUEST	(1 << 9)  /* On quest				*/
#define PRF_SUMMONABLE	(1 << 10) /* Can be summoned			*/
#define PRF_NOREPEAT	(1 << 11) /* No repetition of comm commands	*/
#define PRF_HOLYLIGHT	(1 << 12) /* Can see in dark			*/
#define PRF_COLOR_1	(1 << 13) /* Color (low bit)			*/
#define PRF_COLOR_2	(1 << 14) /* Color (high bit)			*/
#define PRF_NOWIZ	(1 << 15) /* Can't hear wizline			*/
#define PRF_LOG1	(1 << 16) /* On-line System Log (low bit)	*/
#define PRF_LOG2	(1 << 17) /* On-line System Log (high bit)	*/
#define PRF_NOAUCT	(1 << 18) /* Can't hear auction channel		*/
#define PRF_NOGOSS	(1 << 19) /* Can't hear gossip channel		*/
#define PRF_NOGRATZ	(1 << 20) /* Can't hear grats channel		*/
#define PRF_ROOMFLAGS	(1 << 21) /* Can see room flags (ROOM_x)	*/
#define PRF_DISPAUTO	(1 << 22) /* Show prompt HP, MP, MV when < 30%.	*/
#define PRF_OVERHEAD_MAP (1 << 23) /* show overhead map */

	/* Affect bits: used in char_data.char_specials.saved.affected_by */
	/* WARNING: In the world files, NEVER set the bits marked "R" ("Reserved") */
#define AFF_BLIND             (1 << 0)	   /* (R) Char is blind		*/
#define AFF_INVISIBLE         (1 << 1)	   /* Char is invisible		*/
#define AFF_DETECT_ALIGN      (1 << 2)	   /* Char is sensitive to align*/
#define AFF_DETECT_INVIS      (1 << 3)	   /* Char can see invis chars  */
#define AFF_DETECT_MAGIC      (1 << 4)	   /* Char is sensitive to magic*/
#define AFF_SENSE_LIFE        (1 << 5)	   /* Char can sense hidden life*/
#define AFF_WATERWALK	      (1 << 6)	   /* Char can walk on water	*/
#define AFF_SANCTUARY         (1 << 7)	   /* Char protected by sanct.	*/
#define AFF_GROUP             (1 << 8)	   /* (R) Char is grouped	*/
#define AFF_CURSE             (1 << 9)	   /* Char is cursed		*/
#define AFF_INFRAVISION       (1 << 10)	   /* Char can see in dark	*/
#define AFF_POISON            (1 << 11)	   /* (R) Char is poisoned	*/
#define AFF_PROTECT_EVIL      (1 << 12)	   /* Char protected from evil  */
#define AFF_PROTECT_GOOD      (1 << 13)	   /* Char protected from good  */
#define AFF_SLEEP             (1 << 14)	   /* (R) Char magically asleep	*/
#define AFF_NOTRACK	      (1 << 15)	   /* Char can't be tracked	*/
#define AFF_UNUSED16	      (1 << 16)	   /* Room for future expansion	*/
#define AFF_UNUSED17	      (1 << 17)	   /* Room for future expansion	*/
#define AFF_SNEAK             (1 << 18)	   /* Char can move quietly	*/
#define AFF_HIDE              (1 << 19)	   /* Char is hidden		*/
#define AFF_UNUSED20	      (1 << 20)	   /* Room for future expansion	*/
#define AFF_CHARM             (1 << 21)	   /* Char is charmed		*/



	/* Modes of connectedness: used by descriptor_data.state */
#define CON_NEEDS_AUTHENTICATION 0
#define CON_PLAYING	 1	/* Playing - Nominal state		*/
#define CON_CLOSE	 2	/* User disconnect, remove character.	*/
#define CON_GET_NAME	 3	/* By what name ..?			*/
#define CON_NAME_CNFRM	 4	/* Did I get that right, x?		*/
#define CON_PASSWORD	 5	/* Password:				*/
#define CON_NEWPASSWD	 6	/* Give me a password for x		*/
#define CON_CNFPASSWD	 7	/* Please retype password:		*/
#define CON_QSEX	 8	/* Sex?					*/
#define CON_QCLASS	 9	/* Class?				*/
#define CON_RMOTD	 10	/* PRESS RETURN after MOTD		*/
#define CON_MENU	 11	/* Your choice: (main menu)		*/
#define CON_EXDESC	 12	/* Enter a new description:		*/
#define CON_CHPWD_GETOLD 13	/* Changing passwd: get old		*/
#define CON_CHPWD_GETNEW 14	/* Changing passwd: get new		*/
#define CON_CHPWD_VRFY   15	/* Verify new password			*/
#define CON_DELCNF1	 16	/* Delete confirmation 1		*/
#define CON_DELCNF2	 17	/* Delete confirmation 2		*/
#define CON_DISCONNECT	 18	/* In-game link loss (leave character)	*/

	/* Character equipment positions: used as index for char_data.equipment[] */
	/* NOTE: Don't confuse these constants with the ITEM_ bitvectors
		 which control the valid places you can wear a piece of equipment */
#define WEAR_LIGHT      0
#define WEAR_FINGER_R   1
#define WEAR_FINGER_L   2
#define WEAR_NECK_1     3
#define WEAR_NECK_2     4
#define WEAR_BODY       5
#define WEAR_HEAD       6
#define WEAR_LEGS       7
#define WEAR_FEET       8
#define WEAR_HANDS      9
#define WEAR_ARMS      10
#define WEAR_SHIELD    11
#define WEAR_ABOUT     12
#define WEAR_WAIST     13
#define WEAR_WRIST_R   14
#define WEAR_WRIST_L   15
#define WEAR_WIELD     16
#define WEAR_HOLD      17

#define NUM_WEARS      18	/* This must be the # of eq positions!! */


	/* object-related defines ********************************************/


	/* Item types: used by obj_data.obj_flags.type_flag */
#define ITEM_LIGHT      1		/* Item is a light source	*/
#define ITEM_SCROLL     2		/* Item is a scroll		*/
#define ITEM_WAND       3		/* Item is a wand		*/
#define ITEM_STAFF      4		/* Item is a staff		*/
#define ITEM_WEAPON     5		/* Item is a weapon		*/
#define ITEM_FIREWEAPON 6		/* Unimplemented		*/
#define ITEM_MISSILE    7		/* Unimplemented		*/
#define ITEM_TREASURE   8		/* Item is a treasure, not gold	*/
#define ITEM_ARMOR      9		/* Item is armor		*/
#define ITEM_POTION    10 		/* Item is a potion		*/
#define ITEM_WORN      11		/* Unimplemented		*/
#define ITEM_OTHER     12		/* Misc object			*/
#define ITEM_TRASH     13		/* Trash - shopkeeps won't buy	*/
#define ITEM_TRAP      14		/* Unimplemented		*/
#define ITEM_CONTAINER 15		/* Item is a container		*/
#define ITEM_NOTE      16		/* Item is note 		*/
#define ITEM_DRINKCON  17		/* Item is a drink container	*/
#define ITEM_KEY       18		/* Item is a key		*/
#define ITEM_FOOD      19		/* Item is food			*/
#define ITEM_MONEY     20		/* Item is money (gold)		*/
#define ITEM_PEN       21		/* Item is a pen		*/
#define ITEM_BOAT      22		/* Item is a boat		*/
#define ITEM_FOUNTAIN  23		/* Item is a fountain		*/


	/* Take/Wear flags: used by obj_data.obj_flags.wear_flags */
#define ITEM_WEAR_TAKE		(1 << 0)  /* Item can be takes		*/
#define ITEM_WEAR_FINGER	(1 << 1)  /* Can be worn on finger	*/
#define ITEM_WEAR_NECK		(1 << 2)  /* Can be worn around neck 	*/
#define ITEM_WEAR_BODY		(1 << 3)  /* Can be worn on body 	*/
#define ITEM_WEAR_HEAD		(1 << 4)  /* Can be worn on head 	*/
#define ITEM_WEAR_LEGS		(1 << 5)  /* Can be worn on legs	*/
#define ITEM_WEAR_FEET		(1 << 6)  /* Can be worn on feet	*/
#define ITEM_WEAR_HANDS		(1 << 7)  /* Can be worn on hands	*/
#define ITEM_WEAR_ARMS		(1 << 8)  /* Can be worn on arms	*/
#define ITEM_WEAR_SHIELD	(1 << 9)  /* Can be used as a shield	*/
#define ITEM_WEAR_ABOUT		(1 << 10) /* Can be worn about body 	*/
#define ITEM_WEAR_WAIST 	(1 << 11) /* Can be worn around waist 	*/
#define ITEM_WEAR_WRIST		(1 << 12) /* Can be worn on wrist 	*/
#define ITEM_WEAR_WIELD		(1 << 13) /* Can be wielded		*/
#define ITEM_WEAR_HOLD		(1 << 14) /* Can be held		*/


	/* Extra object flags: used by obj_data.obj_flags.extra_flags */
#define ITEM_GLOW          (1 << 0)	/* Item is glowing		*/
#define ITEM_HUM           (1 << 1)	/* Item is humming		*/
#define ITEM_NORENT        (1 << 2)	/* Item cannot be rented	*/
#define ITEM_NODONATE      (1 << 3)	/* Item cannot be donated	*/
#define ITEM_NOINVIS	   (1 << 4)	/* Item cannot be made invis	*/
#define ITEM_INVISIBLE     (1 << 5)	/* Item is invisible		*/
#define ITEM_MAGIC         (1 << 6)	/* Item is magical		*/
#define ITEM_NODROP        (1 << 7)	/* Item is cursed: can't drop	*/
#define ITEM_BLESS         (1 << 8)	/* Item is blessed		*/
#define ITEM_ANTI_GOOD     (1 << 9)	/* Not usable by good people	*/
#define ITEM_ANTI_EVIL     (1 << 10)	/* Not usable by evil people	*/
#define ITEM_ANTI_NEUTRAL  (1 << 11)	/* Not usable by neutral people	*/
#define ITEM_ANTI_MAGIC_USER (1 << 12)	/* Not usable by mages		*/
#define ITEM_ANTI_CLERIC   (1 << 13)	/* Not usable by clerics	*/
#define ITEM_ANTI_THIEF	   (1 << 14)	/* Not usable by thieves	*/
#define ITEM_ANTI_WARRIOR  (1 << 15)	/* Not usable by warriors	*/
#define ITEM_NOSELL	   (1 << 16)	/* Shopkeepers won't touch it	*/


	/* Modifier constants used with obj affects ('A' fields) */
#define APPLY_NONE              0	/* No effect			*/
#define APPLY_STR               1	/* Apply to strength		*/
#define APPLY_DEX               2	/* Apply to dexterity		*/
#define APPLY_INT               3	/* Apply to intelligence	*/
#define APPLY_WIS               4	/* Apply to wisdom		*/
#define APPLY_CON               5	/* Apply to constitution	*/
#define APPLY_CHA		6	/* Apply to charisma		*/
#define APPLY_CLASS             7	/* Reserved			*/
#define APPLY_LEVEL             8	/* Reserved			*/
#define APPLY_AGE               9	/* Apply to age			*/
#define APPLY_CHAR_WEIGHT      10	/* Apply to weight		*/
#define APPLY_CHAR_HEIGHT      11	/* Apply to height		*/
#define APPLY_MANA             12	/* Apply to max mana		*/
#define APPLY_HIT              13	/* Apply to max hit points	*/
#define APPLY_MOVE             14	/* Apply to max move points	*/
#define APPLY_GOLD             15	/* Reserved			*/
#define APPLY_EXP              16	/* Reserved			*/
#define APPLY_AC               17	/* Apply to Armor Class		*/
#define APPLY_HITROLL          18	/* Apply to hitroll		*/
#define APPLY_DAMROLL          19	/* Apply to damage roll		*/
#define APPLY_SAVING_PARA      20	/* Apply to save throw: paralz	*/
#define APPLY_SAVING_ROD       21	/* Apply to save throw: rods	*/
#define APPLY_SAVING_PETRI     22	/* Apply to save throw: petrif	*/
#define APPLY_SAVING_BREATH    23	/* Apply to save throw: breath	*/
#define APPLY_SAVING_SPELL     24	/* Apply to save throw: spells	*/


	/* Container flags - value[1] */
#define CONT_CLOSEABLE      (1 << 0)	/* Container can be closed	*/
#define CONT_PICKPROOF      (1 << 1)	/* Container is pickproof	*/
#define CONT_CLOSED         (1 << 2)	/* Container is closed		*/
#define CONT_LOCKED         (1 << 3)	/* Container is locked		*/


	/* Some different kind of liquids for use in values of drink containers */
#define LIQ_WATER      0
#define LIQ_BEER       1
#define LIQ_WINE       2
#define LIQ_ALE        3
#define LIQ_DARKALE    4
#define LIQ_WHISKY     5
#define LIQ_LEMONADE   6
#define LIQ_FIREBRT    7
#define LIQ_LOCALSPC   8
#define LIQ_SLIME      9
#define LIQ_MILK       10
#define LIQ_TEA        11
#define LIQ_COFFE      12
#define LIQ_BLOOD      13
#define LIQ_SALTWATER  14
#define LIQ_CLEARWATER 15


	/* other miscellaneous defines *******************************************/


	/* Player conditions */
#define DRUNK        0
#define FULL         1
#define THIRST       2


	/* Sun state for weather_data */
#define SUN_DARK	0
#define SUN_RISE	1
#define SUN_LIGHT	2
#define SUN_SET		3


	/* Sky conditions for weather_data */
#define SKY_CLOUDLESS	0
#define SKY_CLOUDY	1
#define SKY_RAINING	2
#define SKY_LIGHTNING	3


	/* Rent codes */
#define RENT_UNDEF      0
#define RENT_CRASH      1
#define RENT_RENTED     2
#define RENT_CRYO       3
#define RENT_FORCED     4
#define RENT_TIMEDOUT   5


	/* other #defined constants **********************************************/

	/*
	 * **DO**NOT** blindly change the number of levels in your MUD merely by
	 * changing these numbers and without changing the rest of the code to match.
	 * Other changes throughout the code are required.  See coding.doc for
	 * details.
	 *
	 * LVL_IMPL should always be the HIGHEST possible immortal level, and
	 * LVL_IMMORT should always be the LOWEST immortal level.  The number of
	 * mortal levels will always be LVL_IMMORT - 1.
	 */
#define LVL_IMPL	34
#define LVL_GRGOD	33
#define LVL_GOD		32
#define LVL_IMMORT	31

	/* Level of the 'freeze' command */
#define LVL_FREEZE	LVL_GRGOD

#define NUM_OF_DIRS	6	/* number of directions in a room (nsewud) */
#define MAGIC_NUMBER	(0x06)	/* Arbitrary number that won't be in a string */

	/*
	 * OPT_USEC determines how many commands will be processed by the MUD per
	 * second and how frequently it does socket I/O.  A low setting will cause
	 * actions to be executed more frequently but will increase overhead due to
	 * more cycling to check.  A high setting (e.g. 1 Hz) may upset your players
	 * as actions (such as large speedwalking chains) take longer to be executed.
	 * You shouldn't need to adjust this.
	 */
#define OPT_USEC	100000		/* 10 passes per second */
#define PASSES_PER_SEC	(1000000 / OPT_USEC)
#define RL_SEC		* PASSES_PER_SEC

#define PULSE_BTREE		(1 RL_SEC)
#define PULSE_ZONE      (10 RL_SEC)
#define PULSE_MOBILE    (10 RL_SEC)
#define PULSE_VIOLENCE  ( 2 RL_SEC)
#define PULSE_AUTOSAVE	(60 RL_SEC)
#define PULSE_IDLEPWD	(15 RL_SEC)
#define PULSE_SANITY	(30 RL_SEC)
#define PULSE_USAGE	(5 * 60 RL_SEC)	/* 5 mins */
#define PULSE_TIMESAVE	(30 * 60 RL_SEC) /* should be >= SECS_PER_MUD_HOUR */

	/* Variables for the output buffering system */
#define MAX_SOCK_BUF            (12 * 1024) /* Size of kernel's sock buf   */
#define MAX_PROMPT_LENGTH       96          /* Max length of prompt        */
#define GARBAGE_SPACE		32          /* Space for **OVERFLOW** etc  */
#define SMALL_BUFSIZE		1024        /* Static output buffer size   */
	/* Max amount of output that can be buffered */
#define LARGE_BUFSIZE	   (MAX_SOCK_BUF - GARBAGE_SPACE - MAX_PROMPT_LENGTH)

#define HISTORY_SIZE		5	/* Keep last 5 commands. */
#define MAX_STRING_LENGTH	2*8192
#define MAX_INPUT_LENGTH	5096	/* Max length per *line* of input */
#define MAX_RAW_INPUT_LENGTH	5596	/* Max size of *raw* input */
#define MAX_MESSAGES		60
#define MAX_NAME_LENGTH		20  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_PWD_LENGTH		10  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_TITLE_LENGTH	80  /* Used in char_file_u *DO*NOT*CHANGE* */
#define HOST_LENGTH		30  /* Used in char_file_u *DO*NOT*CHANGE* */
#define EXDSCR_LENGTH		240 /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_TONGUE		3   /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_SKILLS		200 /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_AFFECT		32  /* Used in char_file_u *DO*NOT*CHANGE* */
#define MAX_OBJ_AFFECT		6 /* Used in obj_file_elem *DO*NOT*CHANGE* */
#define MAX_NOTE_LENGTH		1000	/* arbitrary */

	/*
	 * A MAX_PWD_LENGTH of 10 will cause BSD-derived systems with MD5 passwords
	 * and GNU libc 2 passwords to be truncated.  On BSD this will enable anyone
	 * with a name longer than 5 character to log in with any password.  If you
	 * have such a system, it is suggested you change the limit to 20.
	 *
	 * Please note that this will erase your player files.  If you are not
	 * prepared to do so, simply erase these lines but heed the above warning.
	 */
#if defined(HAVE_UNSAFE_CRYPT) && MAX_PWD_LENGTH == 10
#error You need to increase MAX_PWD_LENGTH to at least 20.
#error See the comment near these errors for more explanation.
#endif

	/**********************************************************************
	 * Structures                                                          *
	 **********************************************************************/


	typedef signed char		sbyte;
	typedef unsigned char		ubyte;
	//typedef signed short int	sh_int;
	//typedef unsigned short int	ush_int;
	using sh_int = int32_t;
	using ush_int = uint32_t;
#if !defined(__cplusplus)	/* Anyone know a portable method? */
	typedef char			bool;
#endif

#if !defined(CIRCLE_WINDOWS) || defined(LCC_WIN32)	/* Hm, sysdep.h? */
	typedef signed char			byte;
#endif

	/* Various virtual (human-reference) number types. */
	typedef IDXTYPE room_vnum;
	typedef IDXTYPE obj_vnum;
	typedef IDXTYPE mob_vnum;
	typedef IDXTYPE zone_vnum;
	typedef IDXTYPE shop_vnum;

	/* Various real (array-reference) number types. */
	typedef IDXTYPE room_rnum;
	typedef IDXTYPE obj_rnum;
	typedef IDXTYPE mob_rnum;
	typedef IDXTYPE zone_rnum;
	typedef IDXTYPE shop_rnum;


	/*
	 * Bitvector type for 32 bit unsigned long bitvectors.
	 * 'unsigned long long' will give you at least 64 bits if you have GCC.
	 *
	 * Since we don't want to break the pfiles, you'll have to search throughout
	 * the code for "bitvector_t" and change them yourself if you'd like this
	 * extra flexibility.
	 */
	typedef unsigned long int	bitvector_t;

	/* Extra description: used in objects, mobiles, and rooms */
	struct extra_descr_data {
		char	*keyword;                 /* Keyword in look/examine          */
		char	*description;             /* What to see                      */
		struct extra_descr_data *next; /* Next in list                     */
	};


	/* object-related structures ******************************************/


	/* object flags; used in obj_data */
	struct obj_flag_data {
		obj_flag_data() : type_flag(0),weapon_flags(0),
			ammo_max(0),ammo(0),clip_size(0),wear_flags(0),extra_flags(0),
			weight(0), cost(0), cost_per_day(0),timer(0),bitvector(0){
				memset(value,0,sizeof(value));
			}
		void feed(pqxx::row);
		~obj_flag_data() = default;
		int	value[4];	/* Values of the item (see list)    */
		byte type_flag;	/* Type of item			    */
		uint64_t weapon_flags;
		uint16_t ammo_max;
		uint16_t ammo;
		uint16_t clip_size;
		int /*bitvector_t*/	wear_flags;	/* Where you can wear it	    */
		int /*bitvector_t*/	extra_flags;	/* If it hums, glows, etc.	    */
		int	weight;		/* Weigt what else                  */
		int	cost;		/* Value when sold (gp.)            */
		int	cost_per_day;	/* Cost to keep pr. real day        */
		int	timer;		/* Timer for object                 */
		long /*bitvector_t*/	bitvector;	/* To set chars bits                */
	};


/** TODO: I'd like to uncomment his at some point and utilize it. For now,
 * we'll place weapon specific fields into either obj_flag_data or obj_data
 */
//	struct obj_weapon_data {
//		obj_weapon_data() : weapon_flags(0),
//			ammo_max(0),ammo(0),clip_size(0){
//		}
//		void feed(pqxx::row);
//		~obj_weapon_data() = default;
//		uint64_t weapon_flags;
//		uint16_t ammo_max;
//		uint16_t ammo;
//		uint16_t clip_size;
//	};



	/* Used in obj_file_elem *DO*NOT*CHANGE* */
	struct obj_affected_type {
		byte location;      /* Which ability to change (APPLY_XXX) */
		sbyte modifier;     /* How much it changes by              */
	};


	/* ================== Memory Structure for Objects ================== */
	struct obj_data {
		/**TODO: create constructor and destructor */
		obj_vnum item_number;	/* Where in data-base			*/
		room_rnum in_room;		/* In what room -1 when conta/carr	*/

		obj_flag_data obj_flags;/* Object information               */
		obj_affected_type affected[MAX_OBJ_AFFECT];  /* affects */

		char	*name;                    /* Title of object :get etc.        */
		char	*description;		  /* When in room                     */
		char	*short_description;       /* when worn/carry/in cont.         */
		char	*action_description;      /* What to write when used          */
		extra_descr_data *ex_description; /* extra descriptions     */
		char_data *carried_by;  /* Carried by :NULL in room/conta   */
		char_data *worn_by;	  /* Worn by?			      */
		sh_int worn_on;		  /* Worn where?		      */

		obj_data *in_obj;       /* In what object NULL when none    */
		obj_data *contains;     /* Contains objects                 */

		obj_data *next_content; /* For 'contains' lists             */
		obj_data *next;         /* For the object list              */
		uint8_t ai_state;

		uuid_t uuid;
		int16_t ammo;
		int16_t ammo_max;
		short loaded;
		short holds_ammo;
		weapon_type_t weapon_type;
		int16_t type;
	};
struct obj_data_weapon : public obj_data {
	/**TODO: call parent constructor/destructor */
	/**TODO: eventually, these fields will be specific to this struct and not
	 * the parent structure
	 */
		//int16_t ammo;
		//int16_t ammo_max;
		//short loaded;
		//short holds_ammo;
		//weapon_type_t weapon_type;
};
	/* ======================================================================= */


	/* ====================== File Element for Objects ======================= */
	/*                 BEWARE: Changing it will ruin rent files		   */
	struct obj_file_elem {
		obj_vnum item_number;

#if USE_AUTOEQ
		sh_int location;
#endif
		int	value[4];
		int /*bitvector_t*/	extra_flags;
		int	weight;
		int	timer;
		long /*bitvector_t*/	bitvector;
		struct obj_affected_type affected[MAX_OBJ_AFFECT];
	};


	/* header block for rent files.  BEWARE: Changing it will ruin rent files  */
	struct rent_info {
		int	time;
		int	rentcode;
		int	net_cost_per_diem;
		int	gold;
		int	account;
		int	nitems;
		int	spare0;
		int	spare1;
		int	spare2;
		int	spare3;
		int	spare4;
		int	spare5;
		int	spare6;
		int	spare7;
	};
	/* ======================================================================= */


	/* room-related structures ************************************************/


	struct room_direction_data {
		mods::string general_description;       /* When look DIR.			*/

		mods::string keyword;		/* for open/close			*/

		int exit_info;	/* Exit info			*/
		obj_vnum key;		/* Key's number (-1 for no key)		*/
		room_rnum to_room;		/* Where direction leads (NOWHERE)	*/
	};


	/* ================== Memory Structure for room ======================= */
	struct room_data {
		enum texture_type_t { 
			FIRST,
			GRASS,			/** typically grass that isn't flammable (i.e. not dried out) */
			CEMENT,			/** asphault, nearly indestructible */
			OUTSIDE,		/** Outside where anyone can see you */
			INSIDE,			/** Example: inside a home */
			SEWER,			/** Underground sewer tunnel */
			RADIOACTIVE, /** actively emitting radioactivity */
			VOLATILE,		/** Volatile means any slight spark will ignite an explosion */
			RUBBLE,			/** decimation of buildings resulting in lots of ruble */
			DIRT,				/** think about outside+cement but except no cement */
			SHATTERED_GLASS, /** results of breaking glass objects */
			LOW_ATMOSPHERE,	/** atmosphere too thin for helicopter to fly */
			ON_FIRE,		/** actively burning */
			NON_HAZARDOUS_SMOKE, /** think: burning car */
			HAZARDOUS_SMOKE, /** think: gas attacks */
			LAST
		};
		
		void init();

		room_data();
		room_data(const room_data& r);
		~room_data();
		
		void set_dir_option(byte i,
				const std::string& gen_desc,
				const std::string& keyword,
				const int & ex_info,
				const int & key,
				const room_rnum to_room);
		room_vnum number;		/* Rooms number	(vnum)		      */
		zone_rnum zone;              /* Room zone (for resetting)          */
		int	sector_type;            /* sector type (move/hide)            */
		mods::string	name;                  /* Rooms name 'You are ...'           */
		mods::string	description;           /* Shown when entered                 */
		//extra_descr_data *ex_description; /* for examine/look       */
		room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
		int room_flags;		/* DEATH,DARK ... etc */

		byte light;                  /* Number of lightsources in room     */
		SPECIAL(*func);

		std::vector<mods::extra_desc_data>& ex_descriptions();
		obj_data *contents;   /* List of items in room              */
		char_data *people;    /* List of NPC / PC in room           */
		std::string_view overhead(const lense_type_t& );
		const std::vector<texture_type_t>& textures() const;

		protected:
			std::vector<mods::extra_desc_data> m_ex_descriptions;
			std::vector<texture_type_t> m_textures;
	};
	/* ====================================================================== */


	/* char-related structures ************************************************/

	/* memory structure for characters */
	struct memory_rec_struct {
		/* TODO: replace this with std::vector<char_data*> */
		long	id;
		struct memory_rec_struct *next;
	};

	typedef struct memory_rec_struct memory_rec;


	/* This structure is purely intended to be an easy way to transfer */
	/* and return information about time (real or mudwise).            */
	struct time_info_data {
		int hours, day, month;
		sh_int year;
	};


	/* These data contain information about a players time data */
	struct time_data {
		time_t birth;    /* This represents the characters age                */
		time_t logon;    /* Time of the last logon (used to calculate played) */
		int	played;     /* This is the total accumulated time played in secs */
	};


	/* general player-related info, usually PC's and NPC's */
	/** TODO place this is the db */
	struct char_player_data {
		constexpr static unsigned max_pwd_length = MAX_PWD_LENGTH+1;
		mods::string name;	       /* PC / NPC s name (kill ...  )         */
		mods::string short_descr;  /* for NPC 'actions'                    */
		mods::string long_descr;   /* for 'look'			       */
		mods::string description;  /* Extra descriptions                   */
		mods::string title;        /* PC / NPC's title                     */
		byte sex;           /* PC / NPC's sex                       */
		byte chclass;       /* PC / NPC's class		       */
		byte level;         /* PC / NPC's level                     */
		sh_int hometown;    /* PC s Hometown (zone)                 */
		struct time_data time;  /* PC's AGE in days                 */
		ubyte weight;       /* PC / NPC's weight                    */
		ubyte height;       /* PC / NPC's height                    */
		/** TODO: phase this out */
		mods::string passwd;
		char_player_data();
		~char_player_data() = default;
		private:
		std::array<char,max_pwd_length>	m_passwd; /* character's password      */
	};


	/* Char's abilities.  Used in char_file_u *DO*NOT*CHANGE* */
	/** TODO place this in the db */
	struct char_ability_data {
		sbyte str;
		sbyte str_add;      /* 000 - 100 if strength 18             */
		sbyte intel;
		sbyte wis;
		sbyte dex;
		sbyte con;
		sbyte cha;
		char_ability_data() : str(0),str_add(0),intel(0),
		wis(0),dex(0),con(0),cha(0){}
		~char_ability_data() = default;
	};


	/* Char's points.  Used in char_file_u *DO*NOT*CHANGE* */
	/** TODO: construct this using a "player template" identifier
	 * In essence, it would be an enum that is passed to the constructor.
	 * The enum would be a class type or a specifier to how much each field
	 * should default to. 
	 */
	struct char_point_data {
		sh_int mana;
		sh_int max_mana;     /* Max mana for PC/NPC			   */
		sh_int hit;
		sh_int max_hit;      /* Max hit for PC/NPC                      */
		sh_int move;
		sh_int max_move;     /* Max move for PC/NPC                     */

		sh_int armor;        /* Internal -100..100, external -10..10 AC */
		int	gold;           /* Money carried                           */
		int	bank_gold;	/* Gold the char has in a bank account	   */
		int	exp;            /* The experience of the player            */

		sbyte hitroll;       /* Any bonus or penalty to the hit roll    */
		sbyte damroll;       /* Any bonus or penalty to the damage roll */
		char_point_data() : mana(0),max_mana(0),hit(0),max_hit(0),
		move(0),max_move(0),armor(0),gold(0),bank_gold(0),exp(0),
		hitroll(0),damroll(0){}
		~char_point_data() = default;
	};


	/*
	 * char_special_data_saved: specials which both a PC and an NPC have in
	 * common, but which must be saved to the playerfile for PC's.
	 *
	 * WARNING:  Do not change this structure.  Doing so will ruin the
	 * playerfile.  If you want to add to the playerfile, use the spares
	 * in player_special_data.
	 */
	struct char_special_data_saved {
		char_special_data_saved() : 
			alignment(0), idnum(0), act(0),
			affected_by(0){ 
				memset((void*)&apply_saving_throw[0],0,sizeof(sh_int) * 5);
				
			}
		~char_special_data_saved() = default;
		int	alignment;		/* +-1000 for alignments                */
		long	idnum;			/* player's idnum; -1 for mobiles	*/
		long /*bitvector_t*/ act;	/* act flag for NPC's; player flag for PC's */

		long /*bitvector_t*/	affected_by;
		/* Bitvector for spells/skills affected by */
		sh_int apply_saving_throw[5]; /* Saving throw (Bonuses)		*/
	};


	/* Special playing constants shared by PCs and NPCs which aren't in pfile */
	struct char_special_data {
		char_data *fighting;	/* Opponent				*/
		char_data *hunting;	/* Char hunted by this char		*/

		byte position;		/* Standing, fighting, sleeping, etc.	*/

		int	carry_weight;		/* Carried weight			*/
		byte carry_items;		/* Number of items carried		*/
		int	timer;			/* Timer for update			*/

		char_special_data_saved saved; /* constants saved in plrfile	*/
		char_special_data() : fighting(nullptr), hunting(nullptr), position(POS_STANDING),
		carry_weight(0), carry_items(0),timer(time(0)){

		}
		~char_special_data() = default;
	};


	struct player_special_data_saved {
		constexpr static unsigned conditions_max = 3;
		constexpr static unsigned max_skills = MAX_SKILLS;
		constexpr static unsigned max_tongue = MAX_TONGUE;
		std::array<byte,max_skills> skills;	/* array of skills plus skill 0		*/
		byte PADDING0;		/* used to be spells_to_learn		*/
		std::array<bool,max_tongue> talks;	/* PC s Tongues 0 for NPC		*/
		int	wimp_level;		/* Below this # of hit points, flee!	*/
		byte freeze_level;		/* Level of god who froze char, if any	*/
		sh_int invis_level;		/* level of invisibility		*/
		room_vnum load_room;		/* Which room to place char in		*/
		long /*bitvector_t*/	pref;	/* preference flags for PC's.		*/
		ubyte bad_pws;		/* number of bad password attemps	*/
		std::array<sbyte,conditions_max> conditions;         /* Drunk, full, thirsty			*/

		player_special_data_saved() : PADDING0(0),
		wimp_level(0),freeze_level(0),invis_level(0),
		load_room(0),pref(0),bad_pws(0) {
			std::fill(skills.begin(),skills.end(),0);
			std::fill(talks.begin(),talks.end(),0);
			std::fill(conditions.begin(),conditions.end(),0);
		}
	};

	/*
	 * Specials needed only by PCs, not NPCs.  Space for this structure is
	 * not allocated in memory for NPCs, but it is for PCs and the portion
	 * of it labelled 'saved' is saved in the playerfile.  This structure can
	 * be changed freely; beware, though, that changing the contents of
	 * player_special_data_saved will corrupt the playerfile.
	 */
	struct player_special_data {
		player_special_data_saved saved;
		std::string	poofin;		/* Description on arrival of a god.     */
		std::string poofout;		/* Description upon a god's exit.       */
		struct alias_data *aliases;	/* Character's aliases			*/
		long last_tell;		/* idnum of last tell from		*/
		void *last_olc_targ;		/* olc control				*/
		int last_olc_mode;		/* olc control				*/
		bool js_profile_initialized;
		player_special_data() :  poofin(""),
			poofout(""),aliases(nullptr),last_tell(0),last_olc_targ(nullptr),
			last_olc_mode(-1),js_profile_initialized(false){
		}
		~player_special_data() = default;
	};


	/* Specials used by NPCs, not PCs */
	struct mob_special_data {
		memory_rec_t memory;
		byte	attack_type;        /* The Attack Type Bitvector for NPC's     */
		byte default_pos;        /* Default position for NPC                */
		byte damnodice;          /* The number of damage dice's	       */
		byte damsizedice;        /* The size of the damage dice's           */
		char_data* snipe_tracking;
		uint16_t behaviour_tree;
		uint64_t behaviour_tree_flags;
		mob_special_data() : memory({}),
		attack_type(0),default_pos(POS_STANDING),
		damnodice(0),damsizedice(0),snipe_tracking(nullptr),
		behaviour_tree(0),behaviour_tree_flags(0)
		{

		}
		~mob_special_data() = default;
	};


	/* An affect structure.  Used in char_file_u *DO*NOT*CHANGE* */
	struct affected_type {
		std::size_t index;
		sh_int type;          /* The type of spell that caused this      */
		sh_int duration;      /* For how long its effects will last      */
		sbyte modifier;       /* This is added to apropriate ability     */
		byte location;        /* Tells which ability to change(APPLY_XXX)*/
		long /*bitvector_t*/	bitvector; /* Tells which bits to set (AFF_XXX) */
		affected_type() : index(0), type(0),duration(0),
		modifier(0),location(0),bitvector(0),next(0){
		}
		affected_type(const affected_type& t) : 
		 index(t.index), type(t.type),duration(t.duration),
		modifier(t.modifier),location(t.location),bitvector(t.bitvector),next(0){
		}
		~affected_type() = default;

		struct affected_type *next;
	};


	/* Structure used for chars following other chars */
	struct follow_type {
		struct char_data *follower;
		struct follow_type *next;
	};

	/* Pave mode structure for builders */
	struct room_pavement_t {
		room_vnum start_room;
		int transact_id;
		room_vnum current_room_number;
		int zone_id;
		std::vector<room_rnum> rooms;
		room_pavement_t() : start_room(0), transact_id(-1),
		current_room_number(0), zone_id(-1){}
		room_pavement_t(room_vnum start,int z_id) :  start_room(start), transact_id(0),
		current_room_number(0),zone_id(z_id) {}
		~room_pavement_t() = default;
	};

	struct zone_pavement_t {
		typedef std::vector<mob_vnum> mob_pavements_t;
		typedef std::vector<obj_vnum> obj_pavements_t;
		mob_pavements_t mob;
		obj_pavements_t obj;
		zone_pavement_t() = default;
		~zone_pavement_t() = default;
	};

	struct builder_data_t {
		bool room_pave_mode;
		bool zone_pave_mode;
		room_pavement_t room_pavements;
		zone_pavement_t zone_pavements;
		builder_data_t() : room_pave_mode(false),zone_pave_mode(false){ }
		~builder_data_t() = default;
	};


	/* ================== Structure for player/non-player ===================== */
	struct txt_block {
		char	*text;
		int aliased;
		struct txt_block *next;
	};


	struct txt_q {
		struct txt_block *head;
		struct txt_block *tail;
	};
	/** FIXME This use of namespace mods is random and out of place */
	namespace mods {
		struct descriptor_data {
			using buffer_type_t = std::string;
			using history_type_t = std::array<mods::string,HISTORY_SIZE>;
			using history_pos_type_t = std::size_t;
			enum queue_behaviour_enum_t {
				NORMAL = 0,
				IGNORE_ALL,
				REDIRECT_TO_PLAYER,
				REDIRECT_TO_FILESYSTEM,
				REDIRECT_TO_DB
			};
			constexpr static size_t OUTPUT_SIZE = LARGE_BUFSIZE+1;
			descriptor_data(){ clear(); }
			descriptor_data(queue_behaviour_enum_t queue_behaviour){ 
				clear();
				set_queue_behaviour(queue_behaviour);
			}
			~descriptor_data() = default;
			operator bool() const {
				return !!connected;
			}
			void clear(){ 
				/* Clear out the entire struct */
				descriptor = 0; bad_pws = 0; idle_tics = 0;
				bufptr = 0;bufspace = 0;large_outbuf = nullptr; input = {};
				character = nullptr; original = nullptr;
				connected = 0; desc_num = 0;login_time = 0;showstr_head = 0;
				showstr_vector = 0;showstr_count = 0;showstr_page = 0;str = 0;
				max_str = 0;mail_to = 0;has_prompt = 0;
				history_pos = 0;
				has_output = false;
				m_queue_behaviour = queue_behaviour_enum_t::NORMAL;
				inbuf.clear();
				last_input.clear();
				small_outbuf.clear();
				output.clear();
				host.clear();
			}
			void set_queue_behaviour(queue_behaviour_enum_t queue_behaviour){ 
				m_queue_behaviour = queue_behaviour;
			}
			queue_behaviour_enum_t get_queue_behaviour() const {
				return m_queue_behaviour;
			}
			void set_state(int c) { connected = c; }
			socket_t	descriptor;	/* file descriptor for socket		*/
			mods::string host;		/* hostname				*/
			byte	bad_pws;		/* number of bad pw attemps this login	*/
			byte idle_tics;		/* tics idle at password prompt		*/
			int bufptr;
			int bufspace;
			struct txt_block *large_outbuf; /* ptr to large buffer, if we need it */
			struct txt_q input;             /* q of unprocessed input               */
			char_data *character;    /** FIXME: turn to mods::player */
			char_data *original;    /** FIXME: turn to mods::player */
			int	connected;		/* mode of 'connectedness'		*/
			int	desc_num;		/* unique num assigned to desc		*/
			time_t login_time;		/* when the person connected		*/
			char *showstr_head;		/* for keeping track of an internal str	*/
			char **showstr_vector;	/* for paging through texts		*/
			int  showstr_count;		/* number of pages to page through	*/
			int  showstr_page;		/* which page are we currently showing?	*/
			char	**str;			/* for the modify-str system		*/
			size_t max_str;	        /*		-			*/
			long	mail_to;		/* name for mail system			*/
			int	has_prompt;		/* is the user at a prompt?             */
			buffer_type_t inbuf;  /* buffer for raw input		*/
			buffer_type_t last_input; /* the last input			*/
			buffer_type_t small_outbuf;  /* standard output buffer		*/
			history_type_t history;		/* History of commands, for ! mostly.	*/
			history_pos_type_t history_pos;
			std::shared_ptr<mods::descriptor_data> snooping; /* Who is this char snooping	*/
			std::shared_ptr<mods::descriptor_data> snoop_by; /* And who is snooping this char	*/
			bool has_output;
			size_t queue_output(const std::string &s);
			size_t flush_output();
			private:
			queue_behaviour_enum_t m_queue_behaviour;
			std::string output;		/* ptr to the current output buffer	*/
		};
	};
	struct char_data {
		char_data(){
			init();	
		};
		~char_data() = default;
		char_data(char_data* o);
		void init();
		int pfilepos;			 /* playerfile pos		  */
		uuid_t uuid;
		time_t last_fight_timestamp;			/* timestamp of the last time the user fought */
		mob_rnum nr;                          /* Mob's rnum			  */
		room_rnum in_room;                    /* Location (real room number)	  */
		room_rnum was_in_room;		 /* location for linkdead people  */
		int wait;				 /* wait for how many loops	  */
		bool drone;
		uuid_t drone_owner;
		bool drone_simulate;
		uuid_t drone_uuid;
		char_player_data player;       /* Normal data                   */
		char_ability_data real_abils;	 /* Abilities without modifiers   */
		char_ability_data aff_abils;	 /* Abils with spells/stones/etc  */
		char_point_data points;        /* Points                        */
		char_special_data char_specials;	/* PC/NPC specials	  */
		std::shared_ptr<player_special_data> player_specials; /* PC specials		  */
		mob_special_data mob_specials;	/* NPC specials		  */

		/** Design philosophy:
		 * It's going to take a very long time to remove all linked list
		 * implementation code to loop through affected. Instead, we will 
		 * build a vector and have our own affection implementation surrounded
		 * by that. 
		 */
		affected_type *affected;       /* affected by what spells       */
		/** TODO: convert to std::vector */
		obj_data *equipment[NUM_WEARS];/* Equipment array               */

		/** TODO: convert to forward list or std::vector */
		obj_data *carrying;            /* Head of list                  */
		std::deque<std::shared_ptr<obj_data>> m_carrying;
		bool has_desc;
		std::shared_ptr<mods::descriptor_data> desc;         /* NULL for mobiles              */

		/** TODO: our ultimate goal is to completely get rid of these linked list members */
		char_data *next_in_room;     /* For room->people - list         */
		char_data *next;             /* For either monster or ppl-list  */
		char_data *next_fighting;    /* For fighting list               */

		follow_type *followers;        /* List of chars followers       */
		char_data *master;             /* Who is char following?        */

		std::shared_ptr<mods::player> player_ptr;
		/**
		 * The following will be deprecated
		 */
		goal_t goal;
		short disorient;
		/** TODO: this needs to go into a different structure. Preferably mob_specials. */
		ai_state_t state;

		/** TODO: this needs to go elsewhere. There is no reason why this should
		 * be in this structure, especially since NPC characters and non-builder
		 * players will share this structure.
		 */
		std::shared_ptr<builder_data_t> builder_data;
	};
	/* ====================================================================== */


	/* ==================== File Structure for Player ======================= */
	/*             BEWARE: Changing it will ruin the playerfile		  */
	struct char_file_u {
		/* char_player_data */
		mods::string name;
		mods::string description;
		mods::string title;
		byte sex;
		byte chclass;
		byte level;
		sh_int hometown;
		time_t birth;   /* Time of birth of character     */
		int	played;    /* Number of secs played in total */
		ubyte weight;
		ubyte height;

		char	pwd[MAX_PWD_LENGTH+1];    /* character's password */

		struct char_special_data_saved char_specials_saved;
		struct player_special_data_saved player_specials_saved;
		struct char_ability_data abilities;
		struct char_point_data points;
		struct affected_type affected[MAX_AFFECT];

		time_t last_logon;		/* Time (in secs) of last logon */
		mods::string host;	/* host of last logon */
	};
	/* ====================================================================== */


	/* descriptor-related structures ******************************************/






	/* other miscellaneous structures ***************************************/


	struct msg_type {
		char	*attacker_msg;  /* message to attacker */
		char	*victim_msg;    /* message to victim   */
		char	*room_msg;      /* message to room     */
	};


	struct message_type {
		struct msg_type die_msg;	/* messages when death			*/
		struct msg_type miss_msg;	/* messages when miss			*/
		struct msg_type hit_msg;	/* messages when hit			*/
		struct msg_type god_msg;	/* messages when hit on god		*/
		struct message_type *next;	/* to next messages of this kind.	*/
	};


	struct message_list {
		int	a_type;			/* Attack type				*/
		int	number_of_attacks;	/* How many attack messages to chose from. */
		struct message_type *msg;	/* List of messages.			*/
	};


	struct dex_skill_type {
		sh_int p_pocket;
		sh_int p_locks;
		sh_int traps;
		sh_int sneak;
		sh_int hide;
	};


	struct dex_app_type {
		sh_int reaction;
		sh_int miss_att;
		sh_int defensive;
	};


	struct str_app_type {
		sh_int tohit;    /* To Hit (THAC0) Bonus/Penalty        */
		sh_int todam;    /* Damage Bonus/Penalty                */
		sh_int carry_w;  /* Maximum weight that can be carrried */
		sh_int wield_w;  /* Maximum weight that can be wielded  */
	};


	struct wis_app_type {
		byte bonus;       /* how many practices player gains per lev */
	};


	struct int_app_type {
		byte learn;       /* how many % a player learns a spell/skill */
	};


	struct con_app_type {
		sh_int hitp;
		sh_int shock;
	};


	struct weather_data {
		int	pressure;	/* How is the pressure ( Mb ) */
		int	change;	/* How fast and what way does it change. */
		int	sky;	/* How is the sky. */
		int	sunlight;	/* And how much sun. */
	};


	/*
	 * Element in monster and object index-tables.
	 *
	 * NOTE: Assumes sizeof(mob_vnum) >= sizeof(obj_vnum)
	 */
	struct index_data {
		mob_vnum	vnum;	/* virtual number of this mob/obj		*/
		int		number;	/* number of existing units of this mob/obj	*/
		SPECIAL(*func);
	};

	struct guild_info_type {
		int pc_class;
		room_vnum guild_room;
		int direction;
	};

	typedef std::map<obj_vnum,obj_data*> map_object_list;

#endif

