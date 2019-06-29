#ifndef __SPELL_HEADER__
#define __SPELL_HEADER__
/* ************************************************************************
*   File: spells.h                                      Part of CircleMUD *
*  Usage: header file: constants and fn prototypes for spell system       *
*                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
************************************************************************ */

#define DEFAULT_STAFF_LVL	12
#define DEFAULT_WAND_LVL	12

#define CAST_UNDEFINED	(-1)
#define CAST_SPELL	0
#define CAST_POTION	1
#define CAST_WAND	2
#define CAST_STAFF	3
#define CAST_SCROLL	4

#define MAG_DAMAGE	(1 << 0)
#define MAG_AFFECTS	(1 << 1)
#define MAG_UNAFFECTS	(1 << 2)
#define MAG_POINTS	(1 << 3)
#define MAG_ALTER_OBJS	(1 << 4)
#define MAG_GROUPS	(1 << 5)
#define MAG_MASSES	(1 << 6)
#define MAG_AREAS	(1 << 7)
#define MAG_SUMMONS	(1 << 8)
#define MAG_CREATIONS	(1 << 9)
#define MAG_MANUAL	(1 << 10)


#define TYPE_UNDEFINED               (-1)
#define SPELL_RESERVED_DBC            0  /* SKILL NUMBER ZERO -- RESERVED */

/* PLAYER SPELLS -- Numbered from 1 to MAX_SPELLS */

#define SPELL_ARMOR                   1 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_TELEPORT                2 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_BLESS                   3 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_BLINDNESS               4 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_BURNING_HANDS           5 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CALL_LIGHTNING          6 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CHARM                   7 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CHILL_TOUCH             8 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CLONE                   9 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_COLOR_SPRAY            10 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CONTROL_WEATHER        11 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CREATE_FOOD            12 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CREATE_WATER           13 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CURE_BLIND             14 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CURE_CRITIC            15 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CURE_LIGHT             16 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_CURSE                  17 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DETECT_ALIGN           18 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DETECT_INVIS           19 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DETECT_MAGIC           20 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DETECT_POISON          21 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DISPEL_EVIL            22 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_EARTHQUAKE             23 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_ENCHANT_WEAPON         24 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_ENERGY_DRAIN           25 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_FIREBALL               26 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_HARM                   27 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_HEAL                   28 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_INVISIBLE              29 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_LIGHTNING_BOLT         30 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_LOCATE_OBJECT          31 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_MAGIC_MISSILE          32 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_POISON                 33 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_PROT_FROM_EVIL         34 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_REMOVE_CURSE           35 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_SANCTUARY              36 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_SHOCKING_GRASP         37 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_SLEEP                  38 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_STRENGTH               39 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_SUMMON                 40 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_VENTRILOQUATE          41 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_WORD_OF_RECALL         42 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_REMOVE_POISON          43 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_SENSE_LIFE             44 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_ANIMATE_DEAD	     45 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_DISPEL_GOOD	     46 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_GROUP_ARMOR	     47 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_GROUP_HEAL	     48 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_GROUP_RECALL	     49 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_INFRAVISION	     50 /* Reserved Skill[] DO NOT CHANGE */
#define SPELL_WATERWALK		     51 /* Reserved Skill[] DO NOT CHANGE */
/* Insert new spells here, up to MAX_SPELLS */
#define MAX_SPELLS		    130

/* PLAYER SKILLS - Numbered from MAX_SPELLS+1 to MAX_SKILLS */
#define SKILL_BACKSTAB              131 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_BASH                  132 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_HIDE                  133 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_KICK                  134 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_PICK_LOCK             135 /* Reserved Skill[] DO NOT CHANGE */
/* Undefined                        136 */
#define SKILL_RESCUE                137 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_SNEAK                 138 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_STEAL                 139 /* Reserved Skill[] DO NOT CHANGE */
#define SKILL_TRACK		    140 /* Reserved Skill[] DO NOT CHANGE */
/* New skills may be added here up to MAX_SKILLS (200) */


/*
 *  NON-PLAYER AND OBJECT SPELLS AND SKILLS
 *  The practice levels for the spells and skills below are _not_ recorded
 *  in the playerfile; therefore, the intended use is for spells and skills
 *  associated with objects (such as SPELL_IDENTIFY used with scrolls of
 *  identify) or non-players (such as NPC-only spells).
 */

#define SPELL_IDENTIFY               201
#define SPELL_FIRE_BREATH            202
#define SPELL_GAS_BREATH             203
#define SPELL_FROST_BREATH           204
#define SPELL_ACID_BREATH            205
#define SPELL_LIGHTNING_BREATH       206

#define TOP_SPELL_DEFINE	     299
/* NEW NPC/OBJECT SPELLS can be inserted here up to 299 */


/* WEAPON ATTACK TYPES */

enum weapon_attack_types_t {
	TYPE_HIT = 300,
TYPE_STING,
TYPE_WHIP,
TYPE_SLASH,
TYPE_BITE,
TYPE_BLUDGEON,
TYPE_CRUSH,
TYPE_POUND,
TYPE_CLAW,
TYPE_MAUL,
TYPE_THRASH,
TYPE_PIERCE,
TYPE_BLAST,
TYPE_PUNCH,
TYPE_STAB,
TYPE_SNIPE,
TYPE_SHOTGUN,
TYPE_SUB_MACHINE_GUN,
TYPE_SMG,
TYPE_GRENADE,
TYPE_SUFFERING
};
using weapon_attack_types = weapon_attack_types_t;
/** TODO: phase out the defined constants and use the enum instead */
#define TYPE_HIT weapon_attack_types::TYPE_HIT
#define TYPE_STING weapon_attack_types::TYPE_STING
#define TYPE_WHIP weapon_attack_types::TYPE_WHIP
#define TYPE_SLASH weapon_attack_types::TYPE_SLASH
#define TYPE_BITE weapon_attack_types::TYPE_BITE
#define TYPE_BLUDGEON weapon_attack_types::TYPE_BLUDGEON
#define TYPE_CRUSH weapon_attack_types::TYPE_CRUSH
#define TYPE_POUND weapon_attack_types::TYPE_POUND
#define TYPE_CLAW weapon_attack_types::TYPE_CLAW
#define TYPE_MAUL weapon_attack_types::TYPE_MAUL
#define TYPE_THRASH weapon_attack_types::TYPE_THRASH
#define TYPE_PIERCE weapon_attack_types::TYPE_PIERCE
#define TYPE_BLAST weapon_attack_types::TYPE_BLAST
#define TYPE_PUNCH weapon_attack_types::TYPE_PUNCH
#define TYPE_STAB weapon_attack_types::TYPE_STAB
#define TYPE_SNIPE weapon_attack_types::TYPE_SNIPE
#define TYPE_SHOTGUN weapon_attack_types::TYPE_SHOTGUN
#define TYPE_SUB_MACHINE_GUN weapon_attack_types::TYPE_SUB_MACHINE_GUN
#define TYPE_SMG weapon_attack_types::TYPE_SMG
#define TYPE_GRENADE weapon_attack_types::TYPE_GRENADE
#define TYPE_SUFFER weapon_attack_types::TYPE_SUFFERING

/* new attack types can be added here - up to TYPE_SUFFERING */



#define SAVING_PARA   0
#define SAVING_ROD    1
#define SAVING_PETRI  2
#define SAVING_BREATH 3
#define SAVING_SPELL  4


#define TAR_IGNORE      (1 << 0)
#define TAR_CHAR_ROOM   (1 << 1)
#define TAR_CHAR_WORLD  (1 << 2)
#define TAR_FIGHT_SELF  (1 << 3)
#define TAR_FIGHT_VICT  (1 << 4)
#define TAR_SELF_ONLY   (1 << 5) /* Only a check, use with i.e. TAR_CHAR_ROOM */
#define TAR_NOT_SELF   	(1 << 6) /* Only a check, use with i.e. TAR_CHAR_ROOM */
#define TAR_OBJ_INV     (1 << 7)
#define TAR_OBJ_ROOM    (1 << 8)
#define TAR_OBJ_WORLD   (1 << 9)
#define TAR_OBJ_EQUIP	(1 << 10)

struct spell_info_type {
	byte min_position;	/* Position for caster	 */
	int mana_min;	/* Min amount of mana used by a spell (highest lev) */
	int mana_max;	/* Max amount of mana used by a spell (lowest lev) */
	int mana_change;	/* Change in mana used by spell from lev to lev */

	int min_level[NUM_CLASSES];
	int routines;
	byte violent;
	int targets;         /* See below for use with TAR_XXX  */
	const char *name;	/* Input size not limited. Originates from string constants. */
	const char *wear_off_msg;	/* Input size not limited. Originates from string constants. */
};

/* Possible Targets:

   bit 0 : IGNORE TARGET
   bit 1 : PC/NPC in room
   bit 2 : PC/NPC in world
   bit 3 : Object held
   bit 4 : Object in inventory
   bit 5 : Object in room
   bit 6 : Object in world
   bit 7 : If fighting, and no argument, select tar_char as self
   bit 8 : If fighting, and no argument, select tar_char as victim (fighting)
   bit 9 : If no argument, select self, if argument check that it IS self.

*/

#define SPELL_TYPE_SPELL   0
#define SPELL_TYPE_POTION  1
#define SPELL_TYPE_WAND    2
#define SPELL_TYPE_STAFF   3
#define SPELL_TYPE_SCROLL  4


/* Attacktypes with grammar */

struct attack_hit_type {
	const char	*singular;
	const char	*plural;
};


#define ASPELL(spellname) \
void	spellname(int level, struct char_data *ch, \
		  struct char_data *victim, struct obj_data *obj)

#define MANUAL_SPELL(spellname)	spellname(level, caster, cvict, ovict);

ASPELL(spell_create_water);
ASPELL(spell_recall);
ASPELL(spell_teleport);
ASPELL(spell_summon);
ASPELL(spell_locate_object);
ASPELL(spell_charm);
ASPELL(spell_information);
ASPELL(spell_identify);
ASPELL(spell_enchant_weapon);
ASPELL(spell_detect_poison);

/* basic magic calling functions */

int find_skill_num(char *name);

int mag_damage(int level, struct char_data *ch, struct char_data *victim,
               int spellnum, int savetype);

void mag_affects(int level, struct char_data *ch, struct char_data *victim,
                 int spellnum, int savetype);

void mag_groups(int level, struct char_data *ch, int spellnum, int savetype);

void mag_masses(int level, struct char_data *ch, int spellnum, int savetype);

void mag_areas(int level, struct char_data *ch, int spellnum, int savetype);

void mag_summons(int level, struct char_data *ch, struct obj_data *obj,
                 int spellnum, int savetype);

void mag_points(int level, struct char_data *ch, struct char_data *victim,
                int spellnum, int savetype);

void mag_unaffects(int level, struct char_data *ch, struct char_data *victim,
                   int spellnum, int type);

void mag_alter_objs(int level, struct char_data *ch, struct obj_data *obj,
                    int spellnum, int type);

void mag_creations(int level, struct char_data *ch, int spellnum);

int	call_magic(struct char_data *caster, struct char_data *cvict,
               struct obj_data *ovict, int spellnum, int level, int casttype);

void	mag_objectmagic(struct char_data *ch, struct obj_data *obj,
                        char *argument);

int	cast_spell(struct char_data *ch, struct char_data *tch,
               struct obj_data *tobj, int spellnum);


/* other prototypes */
void spell_level(int spell, int chclass, int level);
void init_spell_levels(void);
const char *skill_name(int num);
#endif
