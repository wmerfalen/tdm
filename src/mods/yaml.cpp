#include "yaml.hpp"
#include "sql.hpp"
#include "pq.hpp"
#ifndef tostr
	#define tostr(a) std::to_string(a)
#endif
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
namespace mods::yaml {

#define MENTOC_FEED_BASE_MEMBERS \
	try{ \
		std::cerr << "feeding base members...\nfeeding manufacturer..";\
		manufacturer = yaml_file["manufacturer"].as<std::string>();\
		std::cerr << "done\nfeeding name...";\
		name = yaml_file["name"].as<std::string>();\
		std::cerr << "done\nfeeding str_type...";\
		str_type = yaml_file["str_type"].as<std::string>();\
		std::cerr << "done\nfeeding vnum...";\
		vnum = yaml_file["vnum"].as<int>();\
		std::cerr << "done\nfeeding rarity...";\
		rarity = parse_rarity(yaml_file["rarity"].as<std::string>());\
		std::cerr << "done\nfeeding feed_file..";\
		feed_file = yaml_file["feed_file"].as<std::string>();\
		std::cerr << "done\nCompleted feeding base members\n";\
	}catch(std::exception &e){ std::cerr << "[yaml-cpp exception]: '" << e.what() << "'\n"; }

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
	std::string rarity_to_string(float val){
		for(auto & rarity_pair : rarity_strings()){
			if(rarity_pair.second == val){
				return rarity_pair.first;
			}
		}
		return "COMMON";
	}
	/*********************************************************/
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
	}

	uint64_t armor_description_t::flush_to_db(){
		try{
			std::map<std::string,std::string> values;
			values["armor_csv_capabilities"] = this->csv_capabilities; //VARCHAR(2048),
			values["armor_csv_attach_to"] = this->csv_attach_to; //VARCHAR(2048),
			values["armor_thac0"] = this->thac0; //INTEGER NOT NULL,
			values["armor_weight_in_lbs"] = this->weight_in_lbs; //FLOAT NOT NULL,
			values["armor_fire_resistance_percent"] = this->fire_resistance_percent; //FLOAT NOT NULL DEFAULT 0.0,
			values["armor_balistic_resistance_percent"] = this->balistic_resistance_percent; //FLOAT NOT NULL DEFAULT 0.0,
			values["armor_speed_profile"] = this->speed_profile; //speed_profile_type_t NOT NULL DEFAULT 'HINDERED',
			values["armor_offensive_damage_amount"] = this->offensive_damage_amount; //INTEGER NOT NULL DEFAULT 0,
			values["armor_durability_profile"] = this->durability_profile; //durability_profile_type_t NOT NULL DEFAULT 'DURABLE',
			auto insert_transaction = txn();
			sql_compositor comp("object_rifle",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_armor")
				.values(values)
				.returning("armor_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && record : row){
				return record["armor_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting new object_rifle record: '" << e.what() << "'\n";
			return 0;
		}
	}

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

	uint64_t rifle_description_t::db_id(){
		return this->id;
	}
	uint64_t armor_description_t::db_id(){
		return this->id;
	}
	uint64_t explosive_description_t::db_id(){
		return this->id;
	}
	uint64_t drone_description_t::db_id(){
		return this->id;
	}
	uint64_t consumable_description_t::db_id(){
		return this->id;
	}
	uint64_t trap_description_t::db_id(){
		return this->id;
	}
	uint64_t gadget_description_t::db_id(){
		return this->id;
	}
	uint64_t attachment_description_t::db_id(){
		return this->id;
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
	uint64_t rifle_description_t::flush_to_db(){
		try{
			std::cerr << "[status] rifle flush_to_db.. flushing...\n";
			std::map<std::string,std::string> values;
			std::string rifm = "rifle_accuracy_map_",
				dam = "rifle_damage_map_";
			for(unsigned i = 0; i < MAX_ROOM_DISTANCE;i++) {
				values[rifm + std::to_string(i)] = std::to_string(accuracy_map[i]);
				values[dam + std::to_string(i)] = std::to_string(damage_map[i]);
			}
			values["rifle_ammo_max"] = std::to_string(ammo_max);
			values["rifle_ammo_type"] = ammo_type;
			values["rifle_chance_to_injure"] = std::to_string(chance_to_injure);
			values["rifle_clip_size"] = std::to_string(clip_size);
			values["rifle_cooldown_between_shots"] = std::to_string(cooldown_between_shots);
			values["rifle_critical_chance"] = std::to_string(critical_chance);
			values["rifle_critical_range"] = std::to_string(critical_range);
			values["rifle_damage_per_second"] = std::to_string(damage_per_second);
			values["rifle_disorient_amount"] = std::to_string(disorient_amount);
			values["rifle_headshot_bonus"] = std::to_string(headshot_bonus);
			values["rifle_max_range"] = std::to_string(max_range);
			values["rifle_range_multiplier"] = std::to_string(range_multiplier);
			values["rifle_reload_time"] = std::to_string(reload_time);
			values["rifle_rounds_per_minute"] = std::to_string(rounds_per_minute);
			values["rifle_muzzle_velocity"] = std::to_string(muzzle_velocity);
			values["rifle_effective_firing_range"] = std::to_string(effective_firing_range);
			values["rifle_str_type"] = str_type;
			values["rifle_type"] = std::to_string(type);
			values["rifle_manufacturer"] = manufacturer;
			values["rifle_name"] = name;
			values["rifle_vnum"] = std::to_string(vnum);
			values["rifle_rarity"] = rarity_to_string(rarity);
			values["rifle_file"] = feed_file;
			mods::util::maps::dump(values);
			auto insert_transaction = txn();
			sql_compositor comp("object_rifle",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_rifle")
				.values(values)
				.returning("rifle_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && record : row){
				this->id = record["rifle_id"].as<uint64_t>();
				return this->id;
			}
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting new object_rifle record: '" << e.what() << "'\n";
			return 0;
		}
	}


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

	int16_t trap_description_t::write_example_file(std::string_view file){
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
		MENTOC_EXAMPLE_TRAP
			base_items(&out_file, "Bear Trap ","BEAR_TRAP");
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
		try {
			std::string file = current_working_dir() + "/" + in_file.data();
			feed_file = file;
			std::cerr << "[explosivedesc feed] feed_file: '" << feed_file << "'\n";
			std::cerr << "[explosivedesc file: '" << file << "'\n";
			auto yaml_file = YAML::LoadFile(file);
			std::cerr << "[explosivedesc feed(in_file)] feeding str_type...";
			auto type_string = yaml_file["str_type"].as<std::string>();
			std::cerr << "[explosivedesc feed(in_file)] type_string:'" << type_string << "' done\n";
			std::cerr << "[explosivedesc feed(in_file)] feeding all explosive members...";
			MENTOC_FEED_EXPLOSIVE
				std::cerr << "[explosivedesc feed(in_file)] done\n";
			std::cerr << "[explosivedesc feed(in_file)] feeding base members...";
			MENTOC_FEED_BASE_MEMBERS
				std::cerr << "[explosivedesc feed(in_file)] done\n";
		}catch(std::exception &e){
			std::cerr << "[exception] explosive feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}


	int16_t rifle_description_t::feed(std::string_view in_file){
		try {
			std::string file = current_working_dir() + "/" + in_file.data();
			feed_file = file;
			std::cerr << "[rifledesc feed] feed_file: '" << feed_file << "'\n";
			std::cerr << "[rifledesc file: '" << file << "'\n";
			auto yaml_file = YAML::LoadFile(file);
			std::cerr << "[rifledesc feed(in_file)] grabbing accuracy_map (rifle feed func)...\n";
			auto acmap = yaml_file["accuracy_map"].as<std::vector<float>>();
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				std::cerr << "[rifledesc feed(in_file)] [acmap]: " << acmap[i] << "\n";
				accuracy_map[i] = acmap[i];
			}
			std::cerr << "[rifledesc feed(in_file)] feeding damage_map...\n";
			auto dmap = yaml_file["damage_map"].as<std::vector<float>>();
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				std::cerr << "[rifledesc feed(in_file)] [acmap]: " << dmap[i] << "\n";
				damage_map[i] = dmap[i];
			}
			std::cerr << "[rifledesc feed(in_file)] done feeding damage_map...\n";
			std::cerr << "[rifledesc feed(in_file)] feeding str_type...";
			auto type_string = yaml_file["str_type"].as<std::string>();
			std::cerr << "[rifledesc feed(in_file)] done\n";
			std::cerr << "[rifledesc feed(in_file)] feeding all rifle members...";
			MENTOC_FEED_RIFLE
				std::cerr << "[rifledesc feed(in_file)] done\n";
			std::cerr << "[rifledesc feed(in_file)] feeding base members...";
			MENTOC_FEED_BASE_MEMBERS
				std::cerr << "[rifledesc feed(in_file)] done\n";
		}catch(std::exception &e){
			std::cerr << "[exception] rifle feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}

	int16_t rifle_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file){
		try {
			std::cerr << "[rifledesc ffpr] grabbing accuracy_map (rifle feed func)...\n";
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("accuracy_map_") + std::to_string(i)].as<float>();
				std::cerr << "[rifledesc ffpr] [acmap]: " << item << "\n";
				accuracy_map[i] = item;
			}
			std::cerr << "[rifledesc ffpr] feeding damage_map...\n";
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("damage_map_") + std::to_string(i)].as<float>();
				std::cerr << "[rifledesc ffpr] [acmap]: " << item << "\n";
				damage_map[i] = item;
			}
			std::cerr << "[rifledesc ffpr] done feeding damage_map...\n";
			std::cerr << "[rifledesc ffpr] feeding str_type...";
			auto type_string = yaml_file["str_type"].as<std::string>();
			std::cerr << "[rifledesc ffpr] done\n";
			std::cerr << "[rifledesc ffpr] feeding all rifle members...";
			MENTOC_FEED_RIFLE
				std::cerr << "[rifledesc ffpr] done\n";
			std::cerr << "[rifledesc ffpr] feeding base members...";
			MENTOC_FEED_BASE_MEMBERS
				std::cerr << "done\n";
		}catch(std::exception &e){
			std::cerr << "[exception] rifle feed: '" << e.what() << "'\n";
			return -1;
		}
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

	int16_t trap_description_t::feed(std::string_view in_file){
		std::string file = current_working_dir() + "/" + in_file.data();
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		MENTOC_FEED_TRAP
			MENTOC_FEED_BASE_MEMBERS
			return 0;
	}


};
