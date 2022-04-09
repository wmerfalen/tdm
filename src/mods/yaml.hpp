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
using mw_container = mods::weapon::type::container;
using mw_melee = mods::weapon::type::melee;
using mw_vehicle = mods::weapon::type::vehicle;
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
	extern std::vector<cap_t> get_caps(mw_melee);
	extern std::vector<cap_t> get_caps(mw_vehicle);
	using obj_ptr_t = std::shared_ptr<obj_data>;
	using player_ptr_t = std::shared_ptr<mods::player>;
	extern obj_ptr_t get_clip_by_name(player_ptr_t& player,std::string_view arg);
	extern void reload_primary_with(player_ptr_t& player,std::string_view arg);
	extern void reload_secondary_with(player_ptr_t& player,std::string_view arg);
	extern void reload_object_with(player_ptr_t& player,obj_ptr_t weapon,std::string_view arg);
	extern bool hits_target(player_ptr_t& player,obj_ptr_t& weapon,player_ptr_t& target, uint16_t* distance);
};
struct obj_flag_data;

#define MENTOC_BASE_MEMBERS_SET(NAME) /**/

enum alternate_explosion_t {
	ALTEX_NONE = 0,
	ALTEX_SCAN = (1),
	ALTEX_SMOKE = (1 << 1),
	ALTEX_EMP = (1 << 2),
	ALTEX_INCENDIARY = (1 << 3),
	ALTEX_FLASHBANG = (1 << 4)
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
	enum armor_classification_type_t {
		NONE = 0,
		BASIC = 1,
		ADVANCED = 2,
		ELITE = 3,
		DEFAULT = armor_classification_type_t::BASIC,
	};
	static inline armor_classification_type_t to_classification(std::string_view cf) {
#define MENTOC_CF(_XDP_) if(cf.compare(#_XDP_) ==0){ return armor_classification_type_t::_XDP_; }
		MENTOC_CF(NONE);
		MENTOC_CF(BASIC);
		MENTOC_CF(ADVANCED);
		MENTOC_CF(ELITE);
#undef MENTOC_CF
		return armor_classification_type_t::BASIC;
	}
	static inline std::string to_string_from_classification(armor_classification_type_t cf) {
#define MENTOC_CF(a) case a: return #a
		switch(cf) {
				MENTOC_CF(NONE);
				MENTOC_CF(BASIC);
				MENTOC_CF(ADVANCED);
				MENTOC_CF(ELITE);
			default:
				return "BASIC";
		}
#undef MENTOC_CF
	}

	static inline std::string to_string_from_durability_profile(durability_profile_type_t dp) {
#define MENTOC_DP(a) case a: return #a

		switch(dp) {
				MENTOC_DP(FLIMSY);
				MENTOC_DP(DECENT);
				MENTOC_DP(DURABLE);
				MENTOC_DP(HARDENED);
				MENTOC_DP(INDUSTRIAL_STRENGTH);
				MENTOC_DP(GODLIKE);
				MENTOC_DP(INDESTRUCTIBLE);
			default:
				return "<unknown>";
		}
#undef MENTOC_DP
	}

	static inline durability_profile_type_t to_durability_profile(std::string_view dp) {
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
	using static_amount_t = int16_t;
	constexpr static uint8_t MAX_ROOM_DISTANCE = 4;
	static inline std::string current_working_dir() {
		/*
		char* cwd = ::get_current_dir_name();
		std::string path = cwd == nullptr ? "" : cwd;
		if(cwd){ free(cwd); }
		return path;
		*/
		return MENTOC_CURRENT_WORKING_DIR;
	}
	struct yaml_description_t {
		virtual ~yaml_description_t() {};
		yaml_description_t() :
			description("desc"), short_description("short desc"),
			action_description("action desc"), manufacturer("manufacturer"),
			name("name"), type(0), str_type("rifle"),
			vnum(0), rarity(0), feed_file("feed_file"),id(0) {

		}
		//virtual int16_t feed(std::string_view src_file) = 0;
		//virtual int16_t write_example_file(std::string_view src_file) = 0;
		uint64_t db_id() {
			return id;
		}
		std::string description;
		std::string short_description;
		std::string action_description;
		std::string manufacturer;
		std::string name;
		int type;
		std::string str_type;
		int vnum;
		float rarity;
		std::string feed_file;
		uint64_t id;
	};

	using aoe_type_t = mods::aoe::types_t;
	constexpr static std::size_t MAX_AOE_TRIGGERS = 6;
	struct rifle_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_rifle;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_rifle)this->type);
		}
		void fill_flags(obj_data* obj);
		virtual ~rifle_description_t() = default;
		rifle_description_t() {
			std::fill(accuracy_map.begin(),accuracy_map.end(),0);
			std::fill(damage_map.begin(),damage_map.end(),0);
			MENTOC_INITIALIZE(MENTOC_RIFLE_MEMBERS_TUPLE);
			feed_status = 2;
			is_rifle_attachment = 0;
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		virtual int16_t write_example_file(std::string_view file);
		uint64_t flush_to_db();
		std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
		std::array<float,MAX_ROOM_DISTANCE> damage_map;	/** Percent per room */

		MENTOC_MEMBER_VARS_FOR(MENTOC_RIFLE_MEMBERS_TUPLE)

		mods::weapon::weapon_stat_list_t* base_stat_list;
		int16_t feed_status;
		bool is_rifle_attachment;
		bool is_sniper() const;
		uint16_t unique_weapon_id;
	};

	struct explosive_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_explosive;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_explosive)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~explosive_description_t() = default;
		explosive_description_t() {
			std::fill(aoe_triggers.begin(),aoe_triggers.end(),0);
			MENTOC_INITIALIZE(MENTOC_EXPLOSIVE_MEMBERS_TUPLE);
			feed_status = 2;
		}
		uint64_t flush_to_db();
		int damage;
		std::array<aoe_type_t,MAX_AOE_TRIGGERS> aoe_triggers;

		MENTOC_MEMBER_VARS_FOR(MENTOC_EXPLOSIVE_MEMBERS_TUPLE)

	};

	struct drone_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_drone;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_drone)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~drone_description_t() = default;
		drone_description_t() {
			MENTOC_INITIALIZE(MENTOC_DRONE_MEMBERS_TUPLE);
			feed_status = 2;
		}

		MENTOC_MEMBER_VARS_FOR(MENTOC_DRONE_MEMBERS_TUPLE)

		int damage;
	};

	struct gadget_description_t : public yaml_description_t {
		std::vector<int> vnum_list;
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_gadget;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_gadget)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~gadget_description_t() = default;
		gadget_description_t() {
			MENTOC_INITIALIZE(MENTOC_GADGET_MEMBERS_TUPLE);
			feed_status = 2;
		}
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		uint64_t flush_to_db();
		durability_profile_type_t durability_profile_enum;
		MENTOC_MEMBER_VARS_FOR(MENTOC_GADGET_MEMBERS_TUPLE)

	};


	struct attachment_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_attachment;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_attachment)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~attachment_description_t() = default;
		attachment_description_t() {
			MENTOC_INITIALIZE(MENTOC_ATTACHMENT_MEMBERS_TUPLE);
			feed_status = 2;
		}
		MENTOC_MEMBER_VARS_FOR(MENTOC_ATTACHMENT_MEMBERS_TUPLE)

	};



	struct armor_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_armor;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_armor)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~armor_description_t() = default;
		armor_description_t() {
			MENTOC_INITIALIZE(MENTOC_ARMOR_MEMBERS_TUPLE);
			feed_status = 2;
		}
		durability_profile_type_t durability_profile_enum;
		MENTOC_MEMBER_VARS_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)

		armor_classification_type_t classification_enum;
		uint64_t flush_to_db();
		uint16_t unique_armor_id;
	};

	struct consumable_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_consumable;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_consumable)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~consumable_description_t() = default;
		consumable_description_t() {
			MENTOC_INITIALIZE(MENTOC_CONSUMABLE_MEMBERS_TUPLE);
			feed_status = 2;

		}

		MENTOC_MEMBER_VARS_FOR(MENTOC_CONSUMABLE_MEMBERS_TUPLE)


	};

	struct trap_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_trap;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_trap)this->type);
		}
		void fill_flags(obj_data*);
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~trap_description_t() = default;
		trap_description_t() {
			MENTOC_INITIALIZE(MENTOC_TRAP_MEMBERS_TUPLE);
			feed_status = 2;
		}

		MENTOC_MEMBER_VARS_FOR(MENTOC_TRAP_MEMBERS_TUPLE)


	};

	struct container_description_t : public yaml_description_t {
		int16_t feed_status;
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_container;

		std::vector<cap_t> get_caps() {
			return {};
		}
		void fill_flags(obj_data* c);
		uint64_t flush_to_db();
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~container_description_t() = default;
		container_description_t() {
			MENTOC_INITIALIZE(MENTOC_CONTAINER_MEMBERS_TUPLE);
			feed_status = 2;
		}

		MENTOC_MEMBER_VARS_FOR(MENTOC_CONTAINER_MEMBERS_TUPLE)


	};
	struct melee_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_melee;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_melee)this->type);
		}
		void fill_flags(obj_data* obj);
		virtual ~melee_description_t() = default;
		melee_description_t()  {
			std::fill(accuracy_map.begin(),accuracy_map.end(),0);
			std::fill(damage_map.begin(),damage_map.end(),0);
			MENTOC_INITIALIZE(MENTOC_MELEE_MEMBERS_TUPLE);
			feed_status = 2;
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		virtual int16_t write_example_file(std::string_view file);
		uint64_t flush_to_db();
		std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
		std::array<float,MAX_ROOM_DISTANCE> damage_map;	/** Percent per room */

		MENTOC_MEMBER_VARS_FOR(MENTOC_MELEE_MEMBERS_TUPLE)

		mods::weapon::weapon_stat_list_t* base_stat_list;
		int16_t feed_status;

	};

	struct vehicle_description_t : public yaml_description_t {
		std::map<std::string,std::string> exported;
		void generate_map();
		using mw_type = mw_vehicle;
		std::vector<cap_t> get_caps() {
			return mods::weapon::get_caps((mw_vehicle)this->type);
		}
		void fill_flags(obj_data* obj);
		virtual ~vehicle_description_t() = default;
		vehicle_description_t()  {
			MENTOC_INITIALIZE(MENTOC_VEHICLE_MEMBERS_TUPLE);
			feed_status = 2;
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t feed_from_po_record(mentoc_pqxx_result_t);
		virtual int16_t write_example_file(std::string_view file);
		uint64_t flush_to_db();

		MENTOC_MEMBER_VARS_FOR(MENTOC_VEHICLE_MEMBERS_TUPLE)

		int16_t feed_status;

	};



	/*********************************************************/
	/** HOWTO: Add new item and subcategories                */
	/* Step 5: Add a new _description_t struct               */
	/*********************************************************/
};
#endif

