#ifndef __MENTOC_MODS_YAML_HEADER__
#define __MENTOC_MODS_YAML_HEADER__

#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
namespace mods::yaml {
	struct rifle_description_t;
	struct explosive_description_t;
};
#include "../structs.h"
#include "pqxx-types.hpp"
#include "weapon.hpp"
#include "aoe.hpp"
#include <fstream>

namespace mods::yaml {
	constexpr static uint8_t MAX_ROOM_DISTANCE = 10;
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
			reload_time(0)
		{
			std::fill(accuracy_map.begin(),accuracy_map.end(),0);
			std::fill(damage_map.begin(),damage_map.end(),0);
			name = "";
		}
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);

			std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
			int ammo_max;
			std::string ammo_type;
			float chance_to_injure;	/** Percent */
			int clip_size;	/** Flat int */
			float cooldown_between_shots;	/** Seconds */
			float critical_chance;	/** Percent */
			int critical_range;	/** Rooms */
			std::array<float,MAX_ROOM_DISTANCE> damage_map;	/** Percent per room */
			float damage_per_second;
			float disorient_amount;
			float headshot_bonus;
			int max_range;
			float range_multiplier;
			float reload_time;
			int type;
			std::string name;
	};
	struct explosive_description_t : public yaml_description_t {
		virtual int16_t feed(std::string_view file);
		virtual int16_t write_example_file(std::string_view file);
		virtual ~explosive_description_t() = default;
		explosive_description_t() :
			damage(0),
			chance_to_injure(0.0),	 /** Percent */
			critical_chance(0.0), /** Percent */
			critical_range(0), /** Rooms */
			blast_radius(1),	/** 1 will only affect room it detonates in */
			damage_per_second(0.0), /** static amount of damage done per second after detonation */
			disorient_amount(0.0), /** percent */
			manufacturer("ACME Industries"), /** Fictional entity that creates this */
			name("Generic Explosive"), /** Name of item */
			type(0)	/** Explosive enum */
		{}
		int damage;
		std::array<aoe_type_t,MAX_AOE_TRIGGERS> aoe_triggers;
		float chance_to_injure;
		float critical_chance;
		int critical_range;
		int blast_radius;	/** In rooms */
		float damage_per_second;
		float disorient_amount;
		std::string manufacturer;
		std::string name;
		int type;
	};
};
#endif

