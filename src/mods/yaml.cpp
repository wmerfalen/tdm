#include "yaml.hpp"
namespace mods::yaml {

	std::vector<std::pair<std::string,float>> rarity_strings() {
			return {
				{"COMMON",mods::rarity::COMMON},
				{"UNCOMMON",mods::rarity::UNCOMMON},
				{"RARE",mods::rarity::RARE},
				{"LEGENDARY",mods::rarity::LEGENDARY},
				{"GOD_TIER",mods::rarity::GOD_TIER},
				{"DEFAULT",mods::rarity::DEFAULT}
			};
	}
	std::string rarity_segment(){
		std::string comment = "#rarity types\n";
		for(auto & rarity_pair : rarity_strings()){
			comment += "# " + rarity_pair.first + "\n";
		}
		comment += "rarity: COMMON\n";
		return comment;
	}
	std::string base_class_items(std::string obj_type){
		std::string items = rarity_segment();
		items += "vnum: 101 #virtual number\n";
		items += "object_type: " + obj_type + "\n";
		return items;
	}
	float parse_rarity(const std::string& rarity_string) {
		for(auto & rarity_pair : rarity_strings()){
			if(!rarity_pair.first.compare(rarity_string)){
				return rarity_pair.second;
			}
		}
		return mods::rarity::DEFAULT;
	}
		/** TODO: write to file, instead of to cout */
	int16_t rifle_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		//std::cerr << "write_example_file: filename: '" << file_name.c_str() << "'\n";
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			std::cerr << "can't open output yaml file, not open!\n";
			return -1;
		}
		if(!out_file.good()){
			std::cerr << "can't open output yaml file, not good!\n";
			return -2;
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
			"rounds_per_minute: 750\n" <<
			"muzzle_velocity: 3018 # Feet per second\n" <<
			"effective_firing_range: 1500 # feet\n" << 
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
		"# types\n" <<
		"#######\n" <<
		"# SNIPER\n" << 
		"# ASSAULT_RIFLE\n" << 
		"# SHOTGUN\n" << 
		"# SUB_MACHINE_GUN\n" << 
		"# HANDGUN\n" << 
		"# MACHINE_PISTOL\n" << 
		"# LIGHT_MACHINE_GUN\n" << 
		"type: 'SNIPER' #string, type of weapon\n" <<
		"name: 'A <adjectively> <horrific> PSG-1 Sniper Rifle' #string\n" <<
			base_class_items("rifle")
			;
		out_file.flush();
		out_file.close();
		return 0;
	};


	int16_t explosive_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			std::cerr << "can't open output yaml file, not open!\n";
			return -1;
		}
		if(!out_file.good()){
			std::cerr << "can't open output yaml file, not good!\n";
			return -2;
		}
		out_file << "\n]\n" <<
			"type: 'FRAG_GRENADE' #string, constant enum\n" <<
			"chance_to_injure: 4.23 #float, out of 100\n" <<
			"critical_chance: 0.10 #float, out of 100\n" <<
			"critical_range: 4 #integer, measured in rooms\n" <<
		 	"damage_per_second: 4.53 #float\n" <<
			"disorient_amount: 3.14 #float\n" <<
			"manufacturer: 'DX Standard' #string. company that made item\n" <<
			"model_name: 'Nade 0x10'\n" <<
			"name: 'A fragmentation grenade' #string\n" <<
			base_class_items("explosive")
			;
		out_file.flush();
		out_file.close();
		return 0;
	};

	int16_t rifle_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		YAML::Node weapon = YAML::LoadFile(std::string(file.data()));
		auto type_string = weapon["type"].as<std::string>();
		if(type_string.compare("SNIPER") == 0){
			type = mw_rifle::SNIPER;
		}
		if(type_string.compare("ASSAULT_RIFLE") == 0){
			type = mods::weapon::type::rifle::ASSAULT_RIFLE;
		}
		if(type_string.compare("SHOTGUN") == 0){
			type = mods::weapon::type::rifle::SHOTGUN;
		}
		if(type_string.compare("SUB_MACHINE_GUN") == 0){
			type = mods::weapon::type::rifle::SUB_MACHINE_GUN;
		}
		if(type_string.compare("HANDGUN") == 0){
			type = mods::weapon::type::rifle::HANDGUN;
		}
		if(type_string.compare("PISTOL") == 0){
			type = mods::weapon::type::rifle::PISTOL;
		}
		if(type_string.compare("MACHINE_PISTOL") == 0){
			type = mods::weapon::type::rifle::MACHINE_PISTOL;
		}
		if(type_string.compare("LIGHT_MACHINE_GUN") == 0){
			type = mods::weapon::type::rifle::LIGHT_MACHINE_GUN;
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
		vnum = weapon["vnum"].as<int>();
		rarity = parse_rarity(weapon["rarity"].as<std::string>());
		rounds_per_minute = weapon["rounds_per_minute"].as<int>();
		muzzle_velocity = weapon["muzzle_velocity"].as<int>();
		effective_firing_range = weapon["effective_firing_range"].as<int>();
		return 0;
	}

	int16_t explosive_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		YAML::Node explosive = YAML::LoadFile(std::string(file.data()));
		auto type_string = explosive["type"].as<std::string>();
		if(type_string.compare("FRAG_GRENADE") == 0){
			type = mw_explosive::FRAG_GRENADE;
		}
		if(type_string.compare("EMP_GRENADE") == 0){
			type = mw_explosive::EMP_GRENADE;
		}
		if(type_string.compare("INCENDIARY_GRENADE") == 0){
			type = mw_explosive::INCENDIARY_GRENADE;
		}
		if(type_string.compare("SMOKE_GRENADE") == 0){
			type = mw_explosive::SMOKE_GRENADE;
		}
		if(type_string.compare("FLASHBANG_GRENADE") == 0){
			type = mw_explosive::FLASHBANG_GRENADE;
		}
		chance_to_injure = explosive["chance_to_injure"].as<float>();
		critical_chance = explosive["critical_chance"].as<float>();
		critical_range = explosive["critical_range"].as<int>();
		damage_per_second = explosive["damage_per_second"].as<float>();
		disorient_amount = explosive["disorient_amount"].as<float>();
		name = explosive["name"].as<std::string>();
		vnum = explosive["vnum"].as<int>();
		object_type = "explosive";
		rarity = parse_rarity(explosive["rarity"].as<std::string>());
		return 0;
	}

};
