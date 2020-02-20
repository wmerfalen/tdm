#ifndef __MENTOC_MODS_YAML_HEADER__
#define __MENTOC_MODS_YAML_HEADER__

#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "rarity.hpp"
#include "item-types.hpp"


namespace mods::yaml {
	MENTOC_YAML_DESC
};
#include "../structs.h"
#include "pqxx-types.hpp"
#include "weapon.hpp"
#include "aoe.hpp"
#include <fstream>

#define MENTOC_BASE_MEMBERS \
		std::string manufacturer;\
		std::string name;\
		int type;\
		std::string str_type;\
		int vnum;\
		float rarity;\
		std::string feed_file;

#define MENTOC_BASE_MEMBERS_SET(NAME)\
			manufacturer("ACME Industries"),\
			name(NAME), type(0), str_type(NAME),\
			vnum(0),rarity(mods::rarity::DEFAULT)

enum alternate_explosion_t {
	ALTEX_NONE = 0,
	ALTEX_SCAN
};

namespace mods::yaml {
	using percent_t = float;
	using rooms_t = int;
	using static_amount_t = int;
	constexpr static uint8_t MAX_ROOM_DISTANCE = 4;
			static inline std::string current_working_dir(){
				char* cwd = ::get_current_dir_name();
				std::string path = cwd == nullptr ? "" : cwd;
				if(cwd){ free(cwd); }
				return path;
			}
	struct yaml_description_t {
		virtual ~yaml_description_t(){};
		virtual int16_t feed(std::string_view src_file) = 0;
		virtual int16_t write_example_file(std::string_view src_file) = 0;
	};

	using aoe_type_t = mods::aoe::types_t;
	constexpr static std::size_t MAX_AOE_TRIGGERS = 6;
	struct rifle_description_t : public yaml_description_t {
		virtual ~rifle_description_t() = default;
		rifle_description_t() : 
			ammo_max(0),
			ammo_type(""),
			chance_to_injure(0),
			clip_size(0),
			cooldown_between_shots(0),
			critical_chance(0),
			critical_range(0),
			damage_per_second(0),
			disorient_amount(0),
			headshot_bonus(0),
			max_range(0),
			range_multiplier(0),
			reload_time(0),
			MENTOC_BASE_MEMBERS_SET("rifle")
		{
			std::fill(accuracy_map.begin(),accuracy_map.end(),0);
			std::fill(damage_map.begin(),damage_map.end(),0);
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		uint64_t flush_to_db();
			std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
			std::array<float,MAX_ROOM_DISTANCE> damage_map;	/** Percent per room */

MENTOC_MEMBER_VARS_FOR(MENTOC_RIFLE_MEMBERS_TUPLE)

			MENTOC_BASE_MEMBERS
	};

	struct explosive_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~explosive_description_t() = default;
		explosive_description_t() :
			damage(0),
			chance_to_injure(0.0),
			critical_chance(0.0),
			critical_range(0),
			damage_per_second(0.0),
			disorient_amount(0.0),
			MENTOC_BASE_MEMBERS_SET("explosive")
		{
			std::fill(aoe_triggers.begin(),aoe_triggers.end(),0);
		}
		int damage;
		std::array<aoe_type_t,MAX_AOE_TRIGGERS> aoe_triggers;

MENTOC_MEMBER_VARS_FOR(MENTOC_EXPLOSIVE_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};

	struct drone_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~drone_description_t() = default;
		drone_description_t() :
			damage(0),
			MENTOC_BASE_MEMBERS_SET("drone")
		{
		}

MENTOC_MEMBER_VARS_FOR(MENTOC_DRONE_MEMBERS_TUPLE)

		int damage;
		MENTOC_BASE_MEMBERS
	};

	struct gadget_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~gadget_description_t() = default;
		gadget_description_t() :
			MENTOC_BASE_MEMBERS_SET("gadget")
		{
		}

MENTOC_MEMBER_VARS_FOR(MENTOC_GADGET_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};


	struct attachment_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~attachment_description_t() = default;
		attachment_description_t() :
			MENTOC_BASE_MEMBERS_SET("attachment")
		{
		}
MENTOC_MEMBER_VARS_FOR(MENTOC_ATTACHMENT_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};



	struct armor_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~armor_description_t() = default;
		armor_description_t() :
			MENTOC_BASE_MEMBERS_SET("armor")
		{
		}
MENTOC_MEMBER_VARS_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};

	struct consumable_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~consumable_description_t() = default;
		consumable_description_t() :
			MENTOC_BASE_MEMBERS_SET("consumable")
		{

		}

MENTOC_MEMBER_VARS_FOR(MENTOC_CONSUMABLE_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS

	};

	struct trap_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~trap_description_t() = default;
		trap_description_t() :
			MENTOC_BASE_MEMBERS_SET("trap"){ }

MENTOC_MEMBER_VARS_FOR(MENTOC_TRAP_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS

	};


/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 5: Add a new _description_t struct               */
/*********************************************************/
};
#endif

