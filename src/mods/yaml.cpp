#include "yaml.hpp"
namespace mods::yaml {
	int16_t rifle_description_t::feed(std::string_view file){
		YAML::Node weapon = YAML::LoadFile(file.c_str());
		type = weapon["type"].as<int>();
		ammo_max = weapon["ammo_max"].as<int>();
		chance_to_injure = weapon["chance_to_injure"].as<int>();
		clip_size = weapon["clip_size"].as<int>();
		cooldown_between_shots = weapon["cooldown_between_shots"].as<float>();
		critical_chance = weapon["critical_chance"].as<float>();
		critical_range = weapon["critical_range"].as<float>();
		range_multiplier = weapon["range_multiplier"].as<int>();
		damage_per_second = weapon["damage_per_second"].as<int>();
		disorient_amount = weapon["disorient_amount"].as<int>();
		headshot_bonus = weapon["headshot_bonus"].as<int>();
		max_range = weapon["max_range"].as<int>();
		reload_time = weapon["reload_time"].as<int>();
		ammo_type = weapon["ammo_type"].as<std::string>();
		return 0;
	}

		base::rifle type;
		int ammo_max;
		int chance_to_injure;
		int clip_size;
		float cooldown_between_shots;
		float critical_chance;
		int critical_range;
		float range_multiplier;
		std::array<float,MAX_ROOM_DISTANCE> damage_map;
		int damage_per_second;
		int disorient_amount;
		int headshot_bonus;
		int max_range;
		int reload_time;
		std::string ammo_type;

		const std::string username = weapon["username"].as<std::string>();
		const std::string password = weapon["password"].as<std::string>();
		login(username, password);
		weapon["lastLogin"] = getCurrentDateTime();

		std::ofstream fout("weapon.yaml");
		fout << weapon;

	}
};
