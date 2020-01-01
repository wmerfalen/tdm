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
		comment += "rarity: 'COMMON'\n";
		return comment;
	}
	void base_items(std::ofstream* out_file,std::string_view name,std::string_view obj_type){
		*out_file	<< "type: '" << obj_type << "' # enum, object type\n"
			<< "manufacturer: 'ACME Industries' # string, anything \n"
			<< "name: '" << name << "' # string, anything name of object \n"
			<< "vnum: 1000 # integer, unique identifier\n"
			<< rarity_segment() 
			;
	}
	float parse_rarity(const std::string& rarity_string) {
		for(auto & rarity_pair : rarity_strings()){
			if(!rarity_pair.first.compare(rarity_string)){
				return rarity_pair.second;
			}
		}
		return mods::rarity::DEFAULT;
	}
	int16_t gadget_description_t::write_example_file(std::string_view file){
		/** TODO: */
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
		out_file << "# type: GRAPPLING_HOOK\n";
		out_file << "# type: BARBED_WIRE\n";
		out_file << "# type: CAMERA\n";
		out_file << "# type: MOTION_SENSOR\n";
		out_file << "# type: DEPLOYABLE_SHIELD\n";
		out_file << "# type: SENSOR_GRENADE\n";
		base_items(&out_file, "XUN15-SP Sensor Grenade","SENSOR_GRENADE");

		out_file.flush();
		out_file.close();
		return 0;
	};
	int16_t gadget_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node node_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = node_file["type"].as<std::string>();
#define MENTOC_MWA mw_gadget
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(SENSOR_GRENADE);
		MENTOC_FEED(GRAPPLING_HOOK);
		MENTOC_FEED(BARBED_WIRE);
		MENTOC_FEED(CAMERA);
		MENTOC_FEED(MOTION_SENSOR);
		MENTOC_FEED(DEPLOYABLE_SHIELD);
#undef MENTOC_FEED
#undef MENTOC_MWA
		manufacturer = node_file["manufacturer"].as<std::string>();
		name = node_file["name"].as<std::string>();
		vnum = node_file["vnum"].as<int>();
		rarity = parse_rarity(node_file["rarity"].as<std::string>());
		return 0;
	}
	int16_t armor_description_t::write_example_file(std::string_view file){
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
		out_file << "# type: VEST\n";
		out_file << "# type: LEGS\n";
		out_file << "# type: GAUNTLETS\n";
		out_file << "# type: SHOULDER_PADS\n";
		out_file << "# type: HELMET\n";
		out_file << "# type: HAT\n";
		out_file << "# type: EYEWEAR\n";
		out_file << "# type: GOGGLES\n";
		out_file << "# type: BACKPACK\n";
		out_file << "# type: WAIST_PACK\n";
		out_file << "# type: VEST_PACK\n";
		out_file << "# type: ELBOW_GUARDS\n";
		out_file << "# type: GLOVES\n";
		base_items(&out_file,"MK-71X Ballistic Vest","VEST");
		out_file.flush();
		out_file.close();
		return 0;
	};
	int16_t armor_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node node_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = node_file["type"].as<std::string>();
#define MENTOC_MWA mw_armor
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(VEST);
		MENTOC_FEED(LEGS);
		MENTOC_FEED(GAUNTLETS);
		MENTOC_FEED(SHOULDER_PADS);
		MENTOC_FEED(HELMET);
		MENTOC_FEED(HAT);
		MENTOC_FEED(EYEWEAR);
		MENTOC_FEED(GOGGLES);
		MENTOC_FEED(BACKPACK);
		MENTOC_FEED(WAIST_PACK);
		MENTOC_FEED(VEST_PACK);
		MENTOC_FEED(ELBOW_GUARDS);
		MENTOC_FEED(GLOVES);
#undef MENTOC_FEED
#undef MENTOC_MWA
		manufacturer = node_file["manufacturer"].as<std::string>();
		name = node_file["name"].as<std::string>();
		vnum = node_file["vnum"].as<int>();
		rarity = parse_rarity(node_file["rarity"].as<std::string>());
		return 0;
	}


	int16_t attachment_description_t::write_example_file(std::string_view file){
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
		out_file << "# type: SIGHT\n";
		out_file << "# type: MUZZLE\n";
		out_file << "# type: MAGAZINE\n";
		base_items(&out_file, "ACOG Scope","SIGHT");
		out_file.close();
		return 0;
	};
	int16_t attachment_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node node_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = node_file["type"].as<std::string>();
#define MENTOC_MWA mw_attachment
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(SIGHT);
		MENTOC_FEED(MUZZLE);
		MENTOC_FEED(MAGAZINE);
#undef MENTOC_FEED
#undef MENTOC_MWA
		manufacturer = node_file["manufacturer"].as<std::string>();
		name = node_file["name"].as<std::string>();
		vnum = node_file["vnum"].as<int>();
		rarity = parse_rarity(node_file["rarity"].as<std::string>());
		return 0;
	}


	int16_t drone_description_t::write_example_file(std::string_view file){
		/** TODO: */
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
		out_file << "# type: GROUND_DRONE\n";
		out_file << "# type: AERIAL_DRONE\n";
		out_file << "# type: AQUATIC_DRONE\n";
		out_file << "# todo: stub\n";
		base_items(&out_file, "TN Land Drone","GROUND_DRONE");

		out_file.flush();
		out_file.close();
		return 0;
	};
	int16_t drone_description_t::feed(std::string_view in_file){
		/** TODO */
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node drone = YAML::LoadFile(std::string(file.data()));
		auto type_string = drone["type"].as<std::string>();
#define MENTOC_MWA mw_drone
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(GROUND_DRONE);
		MENTOC_FEED(AERIAL_DRONE);
		MENTOC_FEED(AQUATIC_DRONE);
#undef MENTOC_FEED
#undef MENTOC_MWA
		manufacturer = drone["manufacturer"].as<std::string>();
		name = drone["name"].as<std::string>();
		vnum = drone["vnum"].as<int>();
		rarity = parse_rarity(drone["rarity"].as<std::string>());
		return 0;
	}

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
			"# type: SNIPER\n" << 
			"# type: ASSAULT_RIFLE\n" << 
			"# type: SHOTGUN\n" << 
			"# type: SUB_MACHINE_GUN\n" << 
			"# type: HANDGUN\n" << 
			"# type: MACHINE_PISTOL\n" << 
			"# type: LIGHT_MACHINE_GUN\n"
			;
		base_items(&out_file,"A <adjectively> <horrific> PSG-1 Sniper Rifle","SNIPER");
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
		out_file <<
			"chance_to_injure: 4.23 #float, out of 100\n" <<
			"critical_chance: 0.10 #float, out of 100\n" <<
			"critical_range: 4 #integer, measured in rooms\n" <<
			"damage_per_second: 4.53 #float\n" <<
			"disorient_amount: 3.14 #float\n" <<
			"model_name: 'Nade 0x10'\n"
			"# types\n" <<
			"#######\n" <<
			"# type: FRAG_GRENADE\n" << 
			"# type: INCENDIARY_GRENADE\n" << 
			"# type: REMOTE_EXPLOSIVE\n" << 
			"# type: REMOTE_CHEMICAL\n" << 
			"# type: EMP_GRENADE\n" << 
			"# type: CLAYMORE_MINE\n" << 
			"# type: SMOKE_GRENADE\n" << 
			"# type: C4\n" << 
			"# type: BREACH_CHARGE\n" << 
			"# type: FLASHBANG_GRENADE\n" << 
			"# type: IMPACT_GRENADE\n";
			base_items(&out_file, "FS12 Fragmentation grenade","FRAG_GRENADE");
		out_file.flush();
		out_file.close();
		return 0;
	};

	int16_t rifle_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node weapon = YAML::LoadFile(std::string(file.data()));
		auto type_string = weapon["type"].as<std::string>();
#define MENTOC_MWA mw_rifle
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(SNIPER);
		MENTOC_FEED(ASSAULT_RIFLE);
		MENTOC_FEED(SHOTGUN);
		MENTOC_FEED(SUB_MACHINE_GUN);
		MENTOC_FEED(HANDGUN);
		MENTOC_FEED(PISTOL);
		MENTOC_FEED(MACHINE_PISTOL);
		MENTOC_FEED(LIGHT_MACHINE_GUN);
#undef MENTOC_FEED
#undef MENTOC_MWA
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
		rounds_per_minute = weapon["rounds_per_minute"].as<int>();
		muzzle_velocity = weapon["muzzle_velocity"].as<int>();
		effective_firing_range = weapon["effective_firing_range"].as<int>();

		manufacturer = weapon["manufacturer"].as<std::string>();
		name = weapon["name"].as<std::string>();
		vnum = weapon["vnum"].as<int>();
		rarity = parse_rarity(weapon["rarity"].as<std::string>());
		return 0;
	}

	int16_t explosive_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node explosive = YAML::LoadFile(std::string(file.data()));
		auto type_string = explosive["type"].as<std::string>();
#define MENTOC_MWA mw_explosive
#define MENTOC_FEED(TYPE)\
		if(type_string.compare(#TYPE) == 0){\
			type = MENTOC_MWA::TYPE;\
		}
		MENTOC_FEED(FRAG_GRENADE);
		MENTOC_FEED(EMP_GRENADE);
		MENTOC_FEED(INCENDIARY_GRENADE);
		MENTOC_FEED(SMOKE_GRENADE);
		MENTOC_FEED(FLASHBANG_GRENADE);
#undef MENTOC_FEED
#undef MENTOC_MWA
		chance_to_injure = explosive["chance_to_injure"].as<float>();
		critical_chance = explosive["critical_chance"].as<float>();
		critical_range = explosive["critical_range"].as<int>();
		damage_per_second = explosive["damage_per_second"].as<float>();
		disorient_amount = explosive["disorient_amount"].as<float>();
		object_type = "explosive";

		manufacturer = explosive["manufacturer"].as<std::string>();
		name = explosive["name"].as<std::string>();
		vnum = explosive["vnum"].as<int>();
		rarity = parse_rarity(explosive["rarity"].as<std::string>());
		return 0;
	}

};
