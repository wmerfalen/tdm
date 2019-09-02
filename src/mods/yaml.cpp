#include "yaml.hpp"
namespace mods::yaml {

		/** TODO: write to file, instead of to cout */
	void rifle_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		//std::cerr << "write_example_file: filename: '" << file_name.c_str() << "'\n";
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			std::cerr << "can't open output yaml file, not open!\n";
			return;
		}
		if(!out_file.good()){
			std::cerr << "can't open output yaml file, not good!\n";
			return;
		}
		out_file << "# accuracy map\n" <<
			"accuracy_map: [\n";
		for(auto i=0; i < MAX_ROOM_DISTANCE;++i) { 
			out_file << "10.00";
			if(i + 1 < MAX_ROOM_DISTANCE){
				out_file << ",\n";
			}
		}
		out_file << "\n]\n" <<
			"ammo_max: 520 #integer\n" <<
			"ammo_type: 'SNIPER' #string, constant enum\n" <<
			"chance_to_injure: 4.23 #float, out of 100\n" <<
			"clip_size: 8 #integer\n"  <<
			"cooldown_between_shots: 0.413 #float, in seconds\n" <<
			"critical_chance: 0.14 #float, out of 100\n" << 
			"critical_range: 4 #integer, measured in rooms\n" <<
			"damage_map: [\n";
		for(auto i=0; i < MAX_ROOM_DISTANCE;++i) {
			out_file << "10.00";
			if(i + 1 < MAX_ROOM_DISTANCE){
				out_file << ",\n";
			}
		}
		out_file << "\n]\n" <<
		 "damage_per_second: 4.53 #float\n" <<
			"disorient_amount: 3.14 #float\n" <<
			"headshot_bonus: 3.0 #float\n" <<
			"max_range: 3 #integer,measured in rooms\n" <<
			"range_multiplier: 4.3 #float, multiplied by \n" <<
			"reload_time: 3.0 #float, measured in seconds\n" <<
			"type: 'SNIPER' #string, type of weapon\n" <<
			"name: 'A <adjectively> <horrific> PSG-1 Sniper Rifle' #string\n"
			;
		out_file.flush();
		out_file.close();
	};


	int16_t rifle_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		YAML::Node weapon = YAML::LoadFile(std::string(file.data()));
		auto type_string = weapon["type"].as<std::string>();
		if(type_string.compare("SNIPER") == 0){
			type = mods::weapon::base::rifle::SNIPER;
		}
		if(type_string.compare("ASSAULT_RIFLE") == 0){
			type = mods::weapon::base::rifle::ASSAULT_RIFLE;
		}
		if(type_string.compare("SHOTGUN") == 0){
			type = mods::weapon::base::rifle::SHOTGUN;
		}
		if(type_string.compare("SUB_MACHINE_GUN") == 0){
			type = mods::weapon::base::rifle::SUB_MACHINE_GUN;
		}
		if(type_string.compare("HANDGUN") == 0){
			type = mods::weapon::base::rifle::HANDGUN;
		}
		if(type_string.compare("MACHINE_PISTOL") == 0){
			type = mods::weapon::base::rifle::MACHINE_PISTOL;
		}
		if(type_string.compare("LIGHT_MACHINE_GUN") == 0){
			type = mods::weapon::base::rifle::LIGHT_MACHINE_GUN;
		}
		ammo_max = weapon["ammo_max"].as<int>();
		chance_to_injure = weapon["chance_to_injure"].as<float>();
		clip_size = weapon["clip_size"].as<int>();
		cooldown_between_shots = weapon["cooldown_between_shots"].as<float>();
		critical_chance = weapon["critical_chance"].as<float>();
		critical_range = weapon["critical_range"].as<float>();
		range_multiplier = weapon["range_multiplier"].as<float>();
		damage_per_second = weapon["damage_per_second"].as<float>();
		disorient_amount = weapon["disorient_amount"].as<float>();
		headshot_bonus = weapon["headshot_bonus"].as<float>();
		max_range = weapon["max_range"].as<int>();
		reload_time = weapon["reload_time"].as<float>();
		ammo_type = weapon["ammo_type"].as<std::string>();
		name = weapon["name"].as<std::string>();
		return 0;
	}

};
