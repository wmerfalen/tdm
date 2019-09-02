#ifndef __MENTOC_MODS_YAML_HEADER__
#define __MENTOC_MODS_YAML_HEADER__

#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
namespace mods::yaml {
	struct rifle_description_t;
};
#include "weapon.hpp"
#include "../structs.h"
#include "pqxx-types.hpp"
#include <fstream>

namespace mods::yaml {
	constexpr static uint8_t MAX_ROOM_DISTANCE = 4;
			static inline std::string current_working_dir(){
				char* cwd = ::get_current_dir_name();
				std::string path = cwd == nullptr ? "" : cwd;
				if(cwd){ free(cwd); }
				return path;
			}
	struct rifle_description_t {
		~rifle_description_t() = default;
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
		int16_t feed(std::string_view file);

		/** TODO: write to file, instead of to cout */
		void write_example_file(std::string_view file);

			std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
			int ammo_max;
			std::string ammo_type;
			float chance_to_injure;
			int clip_size;
			float cooldown_between_shots;
			float critical_chance;
			int critical_range;
			std::array<float,MAX_ROOM_DISTANCE> damage_map;
			float damage_per_second;
			float disorient_amount;
			float headshot_bonus;
			int max_range;
			float range_multiplier;
			float reload_time;
			int type;
			std::string name;
	};
};
#endif

