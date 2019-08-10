#ifndef __MENTOC_MODS_YAML_HEADER__
#define __MENTOC_MODS_YAML_HEADER__

#include <iostream>
#include <string>
#include "yaml-cpp/yaml.h"
#include "../structs.h"
#include "pqxx-types.hpp"

namespace mods::yaml {
	constexpr static uint8_t MAX_ROOM_DISTANCE = 10;
	struct rifle_description_t {
		~rifle_description_t() = default;
		rifle_description_t() : 
			type(0), ammo_max(0), chance
			ammo_max(0),
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
			ammo_type(""),
		{
			std::fill(damage_map.begin(),damage_map.end(),0);
		}
		int16_t feed(std::string_view file);

		private:
		std::array<float,MAX_ROOM_DISTANCE> accuracy_map;
		int ammo_max;
		std::string ammo_type;
		int chance_to_injure;
		int clip_size;
		float cooldown_between_shots;
		float critical_chance;
		int critical_range;
		std::array<float,MAX_ROOM_DISTANCE> damage_map;
		int damage_per_second;
		int disorient_amount;
		int headshot_bonus;
		int max_range;
		float range_multiplier;
		int reload_time;
		base::rifle type;
	};
};
#endif

