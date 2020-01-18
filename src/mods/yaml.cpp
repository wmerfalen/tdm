#include "yaml.hpp"
namespace mods::yaml {

#define MENTOC_FEED_BASE_MEMBERS \
		manufacturer = yaml_file["manufacturer"].as<std::string>();\
		name = yaml_file["name"].as<std::string>();\
		str_type = yaml_file["str_type"].as<std::string>();\
		vnum = yaml_file["vnum"].as<int>();\
		rarity = parse_rarity(yaml_file["rarity"].as<std::string>());\
		feed_file = yaml_file["feed_file"].as<std::string>();

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
		*out_file	<< "str_type: '" << obj_type << "' # enum, object type\n"
			<< "type: 0 # integer enum \n"
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
/*********************************************************/
/** HOWTO: Add new item and subcategories                */
/* Step 6: Define the write_example_file function:       */
/*********************************************************/
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
		MENTOC_EXAMPLE_ARMORS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)
		base_items(&out_file,"MK-71X Ballistic Vest","VEST");
		out_file.flush();
		out_file.close();
		return 0;
	};

	int16_t gadget_description_t::write_example_file(std::string_view file){
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
		MENTOC_EXAMPLE_GADGETS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_GADGET_MEMBERS_TUPLE)
		base_items(&out_file,"XMC Trip Wire","TRIPWIRE");
		out_file.flush();
		out_file.close();
		return 0;
	};


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
		MENTOC_EXAMPLE_ATTACHMENTS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_ATTACHMENT_MEMBERS_TUPLE)
		base_items(&out_file, "ACOG Scope","SIGHT");
		out_file.flush();
		out_file.close();
		return 0;
	};


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
		MENTOC_EXAMPLE_DRONES
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_DRONE_MEMBERS_TUPLE)
		base_items(&out_file, "TN Land Drone","GROUND_DRONE");
		out_file.flush();
		out_file.close();
		return 0;
	};

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
			"damage_map: [\n";
		for(auto i=0; i < MAX_ROOM_DISTANCE;++i) {
			out_file << "10.00";
			if(i + 1 < MAX_ROOM_DISTANCE){
				out_file << ",\n";
			}
		}
		out_file << "\n]\n";
		MENTOC_EXAMPLE_RIFLES
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_RIFLE_MEMBERS_TUPLE)
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
		MENTOC_EXAMPLE_EXPLOSIVES
		base_items(&out_file, "FS12 Fragmentation grenade","FRAG_GRENADE");
		out_file.flush();
		out_file.close();
		return 0;
	};



	int16_t consumable_description_t::write_example_file(std::string_view file){
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
		MENTOC_EXAMPLE_CONSUMABLE
		base_items(&out_file, "N7RCX Human Growth Hormone ","PED");
		out_file.flush();
		out_file.close();
		return 0;
	};

/*******************************************/
/** HOWTO: Add new item and subcategories  */
/* Step 7: Define the feed function:       */
/*******************************************/
	int16_t gadget_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_GADGET
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}
	int16_t explosive_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_EXPLOSIVE
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}
	int16_t rifle_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_RIFLE
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}
	int16_t drone_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_DRONE
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}
	int16_t attachment_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_ATTACHMENT
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}
	int16_t armor_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_ARMOR
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}

	int16_t consumable_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_CONSUMABLE
		MENTOC_FEED_BASE_MEMBERS
		return 0;
	}


};
