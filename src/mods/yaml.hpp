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
#include "weapon-types.hpp"
using mw_rifle = mods::weapon::type::rifle;
using mw_gadget = mods::weapon::type::gadget;
using mw_explosive = mods::weapon::type::explosive;
using mw_armor = mods::weapon::type::armor;
using mw_attachment = mods::weapon::type::attachment;
using mw_drone = mods::weapon::type::drone;
using mw_consumable = mods::weapon::type::consumable;
using mw_trap = mods::weapon::type::trap;
namespace mods {
	struct player;
};
namespace mods::weapon {
	extern std::vector<cap_t> get_caps(mw_rifle);
	extern std::vector<cap_t> get_caps(mw_gadget);
	extern std::vector<cap_t> get_caps(mw_explosive);
	extern std::vector<cap_t> get_caps(mw_armor);
	extern std::vector<cap_t> get_caps(mw_attachment);
	extern std::vector<cap_t> get_caps(mw_drone);
	extern std::vector<cap_t> get_caps(mw_consumable);
	extern std::vector<cap_t> get_caps(mw_trap);
	using obj_ptr_t = std::shared_ptr<obj_data>;
	using player_ptr_t = std::shared_ptr<mods::player>;
	extern obj_ptr_t get_clip_by_name(player_ptr_t & player,std::string_view arg);
	extern void reload_primary_with(player_ptr_t& player,std::string_view arg);
	extern void reload_secondary_with(player_ptr_t& player,std::string_view arg);
	extern void reload_object_with(player_ptr_t& player,obj_ptr_t weapon,std::string_view arg);
	extern bool hits_target(player_ptr_t& player,obj_ptr_t& weapon,player_ptr_t& target, uint16_t* distance);
};
struct obj_flag_data;

#define MENTOC_BASE_MEMBERS \
		std::string description;\
		std::string short_description;\
		std::string action_description;\
		std::string manufacturer;\
		std::string name;\
		int type;\
		std::string str_type;\
		int vnum;\
		float rarity;\
		std::string feed_file;\
		uint64_t id;

#define MENTOC_BASE_MEMBERS_SET(NAME)\
			manufacturer("ACME Industries"),\
			name(NAME), type(0), str_type(NAME),\
			vnum(0),rarity(mods::rarity::DEFAULT),\
			feed_file(std::string(NAME) + ".yml"),\
			id(0)

enum alternate_explosion_t {
	ALTEX_NONE = 0,
	ALTEX_SCAN
};

namespace mods::yaml {
enum durability_profile_type_t {
	FLIMSY,
	DECENT,
	DURABLE,
	HARDENED,
	INDUSTRIAL_STRENGTH,
	GODLIKE,
	INDESTRUCTIBLE
};
static inline std::string to_string_from_durability_profile(durability_profile_type_t dp){
#define MENTOC_DP(a) case a: return #a

	switch(dp){
		MENTOC_DP(FLIMSY);
		MENTOC_DP(DECENT);
		MENTOC_DP(DURABLE);
		MENTOC_DP(HARDENED);
		MENTOC_DP(INDUSTRIAL_STRENGTH);
		MENTOC_DP(GODLIKE);
		MENTOC_DP(INDESTRUCTIBLE);
		default: return "<unknown>";
	}
#undef MENTOC_DP
}

static inline durability_profile_type_t to_durability_profile(std::string_view dp){
#define MENTOC_DP(_XDP_) if(dp.compare(#_XDP_) ==0){ return durability_profile_type_t::_XDP_; }
		MENTOC_DP(FLIMSY);
		MENTOC_DP(DECENT);
		MENTOC_DP(DURABLE);
		MENTOC_DP(HARDENED);
		MENTOC_DP(INDUSTRIAL_STRENGTH);
		MENTOC_DP(GODLIKE);
		MENTOC_DP(INDESTRUCTIBLE);
		return durability_profile_type_t::FLIMSY;
#undef MENTOC_DP
}
	using percent_t = float;
	using rooms_t = int;
	using static_amount_t = int;
	constexpr static uint8_t MAX_ROOM_DISTANCE = 4;
			static inline std::string current_working_dir(){
				/*
				char* cwd = ::get_current_dir_name();
				std::string path = cwd == nullptr ? "" : cwd;
				if(cwd){ free(cwd); }
				return path;
				*/
				return MENTOC_CURRENT_WORKING_DIR;
			}
	struct yaml_description_t {
		virtual ~yaml_description_t(){};
		virtual int16_t feed(std::string_view src_file) = 0;
		virtual int16_t write_example_file(std::string_view src_file) = 0;
	};

	using aoe_type_t = mods::aoe::types_t;
	constexpr static std::size_t MAX_AOE_TRIGGERS = 6;
	struct rifle_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_rifle;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_rifle)this->type); }
		void fill_flags(obj_data* obj);
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
			rounds_per_minute(0),
			muzzle_velocity(0),
			effective_firing_range(0),
			damage_dice_count(2),
			damage_dice_sides(6),
			MENTOC_BASE_MEMBERS_SET("rifle")
		{
			std::fill(accuracy_map.begin(),accuracy_map.end(),0);
			std::fill(damage_map.begin(),damage_map.end(),0);
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		virtual int16_t write_example_file(std::string_view file);
		uint64_t db_id();
		uint64_t flush_to_db();
			std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
			std::array<float,MAX_ROOM_DISTANCE> damage_map;	/** Percent per room */

MENTOC_MEMBER_VARS_FOR(MENTOC_RIFLE_MEMBERS_TUPLE)

			MENTOC_BASE_MEMBERS
			mods::weapon::weapon_stat_list_t* base_stat_list;
	};

	struct explosive_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_explosive;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_explosive)this->type); }
		void fill_flags(obj_data*);
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
		uint64_t db_id();
		uint64_t flush_to_db();
		int damage;
		std::array<aoe_type_t,MAX_AOE_TRIGGERS> aoe_triggers;

MENTOC_MEMBER_VARS_FOR(MENTOC_EXPLOSIVE_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};

	struct drone_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_drone;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_drone)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~drone_description_t() = default;
		drone_description_t() :
			damage(0),
			MENTOC_BASE_MEMBERS_SET("drone")
		{
		}
		uint64_t db_id();

MENTOC_MEMBER_VARS_FOR(MENTOC_DRONE_MEMBERS_TUPLE)

		int damage;
		MENTOC_BASE_MEMBERS
	};

	struct gadget_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_gadget;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_gadget)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~gadget_description_t() = default;
		gadget_description_t() :
			MENTOC_BASE_MEMBERS_SET("gadget")
		{
		}
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		uint64_t db_id();
		uint64_t flush_to_db();
		durability_profile_type_t durability_profile_enum;
MENTOC_MEMBER_VARS_FOR(MENTOC_GADGET_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};


	struct attachment_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_attachment;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_attachment)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~attachment_description_t() = default;
		attachment_description_t() :
			MENTOC_BASE_MEMBERS_SET("attachment")
		{
		}
		uint64_t db_id();
MENTOC_MEMBER_VARS_FOR(MENTOC_ATTACHMENT_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
	};



	struct armor_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_armor;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_armor)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~armor_description_t() = default;
		armor_description_t() :
			MENTOC_BASE_MEMBERS_SET("armor")
		{
		}
		uint64_t db_id();
		durability_profile_type_t durability_profile_enum;
MENTOC_MEMBER_VARS_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS
		uint64_t flush_to_db();
	};

	struct consumable_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_consumable;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_consumable)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~consumable_description_t() = default;
		consumable_description_t() :
			MENTOC_BASE_MEMBERS_SET("consumable")
		{

		}
		uint64_t db_id();

MENTOC_MEMBER_VARS_FOR(MENTOC_CONSUMABLE_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS

	};

	struct trap_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_trap;
		std::vector<cap_t> get_caps(){ return mods::weapon::get_caps((mw_trap)this->type); }
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~trap_description_t() = default;
		trap_description_t() :
			MENTOC_BASE_MEMBERS_SET("trap"){ }
		uint64_t db_id();

MENTOC_MEMBER_VARS_FOR(MENTOC_TRAP_MEMBERS_TUPLE)

		MENTOC_BASE_MEMBERS

	};


/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 5: Add a new _description_t struct               */
/*********************************************************/
};
#endif

