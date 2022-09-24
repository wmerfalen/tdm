/* ************************************************************************
 *   File: structs.h                                     Part of CircleMUD *
 *  Usage: header file for central structures and contstants               *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ************************************************************************ */

#ifndef __MENTOC_ROOM_DATA_HEADER__
#define __MENTOC_ROOM_DATA_HEADER__
#include <iostream>
#include <deque>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <set>
#include "mods/string.hpp"
#include <functional>
#include <array>
#include <forward_list>
#include "mods/extra_desc_data.hpp"
#include <unordered_map>
//#include "mods/yaml.hpp"
#include "mods/index-type.hpp"
//#include "mods/builder-data.hpp"
#include "room-direction-data.hpp"
#include "lense-type.hpp"


namespace mods {
	struct player;
};


extern void log(const char* format,...);

#ifdef __MENTOC_USE_PQXX_RESULT__
	#define mentoc_pqxx_result_t const pqxx::result::reference&
#else
	#define mentoc_pqxx_result_t pqxx::row
#endif

using direction_t = uint8_t;
using depth_t = uint8_t;

namespace mods {
	struct player;
	struct npc;
	struct descriptor_data;
	struct extra_desc_data;
};

struct char_data;
struct obj_data;
using uuid_t = uint64_t;
using aligned_int_t = uint64_t;

namespace mods::globals {
	extern uuid_t obj_uuid();
	extern void register_object_db_id(uint64_t,uuid_t);
};

//#include "mods/index-type.hpp"

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
#define ROOM_DMZ				(1 << 4)   /* Violence not allowed	*/
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
#define EX_BREACHABLE		(1 << 6)
#define EX_QUEST_LOCKED (1 << 7)
#define EX_HIDDEN (1 << 8)
#define EX_ELECTRIFIED (1 << 9)
#define EX_GLASS (1 << 10)
#define EX_AUTOMATIC (1 << 11)

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
#define SECT_ELEVATOR   10		   /* elevator */


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


#define NUM_OF_DIRS	6	/* number of directions in a room (nsewud) */

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

typedef signed char		sbyte;
typedef unsigned char		ubyte;
#if !defined(__cplusplus)	/* Anyone know a portable method? */
	typedef char			bool;
#endif

#if !defined(CIRCLE_WINDOWS) || defined(LCC_WIN32)	/* Hm, sysdep.h? */
	typedef signed char			byte;
#endif

using bitvector_t = unsigned long int;

namespace mods::orm {
	struct locker;
};

namespace mods::globals {
	void destruct_object(uuid_t);
};

namespace mods::integral_objects {
	struct generic_locker_t {
			using obj_ptr_t = std::shared_ptr<obj_data>;
			generic_locker_t();
			generic_locker_t(const generic_locker_t&) = delete;
			generic_locker_t(std::string_view in_type,const room_vnum& room);
			~generic_locker_t();
			void init();
			void load(std::string_view in_type,const room_vnum& room);
		private:
			std::vector<mods::orm::locker> orm;
			obj_ptr_t container;
			room_vnum room;
			room_rnum room_id;
			uint8_t type;
			std::vector<std::string> db_errors;
			bool good;
			std::size_t items;
	};
};

/* ================== Memory Structure for room ======================= */
struct room_data {
		using player_ptr_t = std::shared_ptr<mods::player>;
		bool contract;
		std::set<player_ptr_t> watchers;
		enum texture_type_t : uint8_t {
			AIR,
			CARPET,
			CEMENT,			/** asphault, nearly indestructible */
			DAMP,
			DESERT,
			DIRT,				/** think about outside+cement but except no cement */
			DRY,
			ELEVATOR,
			EMP, /** chaff or emp has been detonated */
			FOREST,
			FROZEN,
			GLASS_WINDOWS,
			GRASS,			/** typically grass that isn't flammable (i.e. not dried out) */
			HAZARDOUS_SMOKE, /** think: gas attacks */
			INSIDE,			/** Example: inside a home */
			LADDER, /** a ladder leading up or down */
			LOW_ATMOSPHERE,	/** atmosphere too thin for helicopter to fly */
			METAL_HATCH,
			METAL_WALL,
			NON_HAZARDOUS_SMOKE, /** think: burning car */
			ON_FIRE,		/** actively burning */
			OUTSIDE,		/** Outside where anyone can see you */
			RADIOACTIVE, /** actively emitting radioactivity */
			ROOFTOP,
			RUBBLE,			/** decimation of buildings resulting in lots of ruble */
			SCANNED,
			SERVER_ROOM,
			SEWER,			/** Underground sewer tunnel */
			SHATTERED_GLASS, /** results of breaking glass objects */
			TREE,
			TRASHY,
			TUNNEL, /** a tunnel made of any material */
			UNDERWATER,
			VOLATILE,		/** Volatile means any slight spark will ignite an explosion */
			WATER,
			WOODEN_WALLS,
			SHALLOW_WATER,	/** less than 5 inches of water */
			FENCED, /** surrounded by fencing */
			STREET,
			GHETTO,
			GANG_INFESTED,
			ONRAMP,
			NARROW_EAST_WEST,
			NARROW_NORTH_SOUTH,
			ALLEY,
			PARKING_LOT,
			PARKING_STALL,
			SIDEWALK,
			ATRIUM,
			TILE,
			GLASS_CONTENTS,
			FREEZING,
			REINFORCED_WALLS,
			METAL_FLOORS,
			WOODEN_BRIDGE,
			TENT,
		};
		constexpr static std::array<texture_type_t,5> textures_that_have_levels = {
			texture_type_t::RADIOACTIVE,
			texture_type_t::LOW_ATMOSPHERE,
			texture_type_t::ON_FIRE,
			texture_type_t::NON_HAZARDOUS_SMOKE,
			texture_type_t::HAZARDOUS_SMOKE
		};
		using texture_level_t = uint8_t;
		enum fire_status_t : uint8_t {
			NONE = 0,
			KINDLING = 1,
			COMPLETELY_ON_FIRE = 2,
			SMOLDERING = 3,
			SMOKING = 4,
			OUT = 5
		};

		void init();

		room_data();
		room_data(const room_data& r);
		~room_data();

		void set_dir_option(byte i,
		    const std::string& gen_desc,
		    const std::string& keyword,
		    const int& ex_info,
		    const int& key,
		    const room_rnum to_room);
		room_vnum number;		/* Rooms number	(vnum)		      */
		zone_rnum zone;              /* Room zone (for resetting)          */
		uint16_t	sector_type;            /* sector type (move/hide)            */
		mods::string	name;                  /* Rooms name 'You are ...'           */
		mods::string	description;           /* Shown when entered                 */
		room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
		int room_flags;		/* DEATH,DARK ... etc */

		byte light;                  /* Number of lightsources in room     */
#define SPECIAL(name) \
	int (name)(char_data *ch, void *me, int cmd, char *argument,player_ptr_t& player)
		SPECIAL(*func);

		std::vector<mods::extra_desc_data>& ex_descriptions();
		obj_data *contents;   /* List of items in room              */
		std::deque<std::shared_ptr<obj_data>>& contents_container() {
			return this->m_contents;
		}
		char_data *people;    /* List of NPC / PC in room           */
		std::string_view overhead(const lense_type_t&);
		std::set<texture_type_t>& textures();
		void add_texture(texture_type_t);
		void add_textures(std::set<texture_type_t> m);
		void remove_texture(texture_type_t);
		bool has_texture(texture_type_t);

		const std::vector<uint8_t>& directions() const;


		/** fire status */
		/** fire status */
		/** fire status */
		fire_status_t& fire_status();
		void fire_status_start();
		/** END fire status */
		texture_level_t& texture_level(texture_type_t type);
		bool watching;
		room_vnum shop_vnum;
		bool starting_point;
		int x;
		int y;
		int z;
		mods::string nickname;
		std::forward_list<mods::integral_objects::generic_locker_t> containers;
	protected:
		std::map<texture_type_t,texture_level_t> m_texture_levels;
		std::vector<uint8_t> m_directions;
		std::vector<mods::extra_desc_data> m_ex_descriptions;
		std::set<texture_type_t> m_textures;
		std::deque<std::shared_ptr<obj_data>> m_contents;
};

#endif
