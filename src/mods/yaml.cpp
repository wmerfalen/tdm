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
		d("feeding base members");\
		manufacturer = yaml_file["manufacturer"].as<std::string>();\
		name = yaml_file["name"].as<std::string>();\
		str_type = yaml_file["str_type"].as<std::string>();\
		vnum = yaml_file["vnum"].as<int>();\
		rarity = parse_rarity(yaml_file["rarity"].as<std::string>());\
		feed_file = yaml_file["feed_file"].as<std::string>();\
		description = yaml_file["description"].as<std::string>();\
		short_description = yaml_file["short_description"].as<std::string>();\
		action_description = yaml_file["action_description"].as<std::string>();\
		d("done\nCompleted feeding base members\n");\
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
			<< "type: '" << obj_type << "' # string enum \n"
			<< "manufacturer: 'ACME Industries' # string, anything \n"
			<< "name: '" << name << "' # string, anything name of object \n"
			<< "vnum: 1000 # integer, unique identifier\n"
			<< "description: 'Description'\n" 
			<< "short_description: 'short description'\n"
			<< "action_description: 'action description'\n"
			<< "feed_file: '" << obj_type.data() << ".yml'\n"
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
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
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
			values["armor_durability_profile"] = this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum); //durability_profile_type_t NOT NULL DEFAULT 'DURABLE',
			values["armor_str_type"] = str_type;
			values["armor_type"] = std::to_string(type);
			values["armor_manufacturer"] = manufacturer;
			values["armor_name"] = name;
			values["armor_vnum"] = std::to_string(vnum);
			values["armor_rarity"] = rarity_to_string(rarity);
			values["armor_file"] = feed_file;
			mods::util::maps::dump(values);
			auto insert_transaction = txn();
			sql_compositor comp("object_armor",&insert_transaction);
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
			std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting new object_armor record: '" << e.what() << "'\n";
			return 0;
		}
	}
	uint64_t gadget_description_t::flush_to_db(){
		try{
			d("[status] gadget flush_to_db.. flushing...\n");
			std::map<std::string,std::string> values;
			values["gadget_csv_capabilities"] = csv_capabilities;
			values["gadget_csv_attach_to"] = csv_attach_to;
			values["gadget_health_points"] = std::to_string(health_points);
			values["gadget_electronic"] = std::to_string(electronic);
			values["gadget_shield_points"] = std::to_string(shield_points);
			values["gadget_durability_profile"] = this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
			values["gadget_move_points"] = std::to_string(move_points);
			values["gadget_damage_points"] = std::to_string(damage_points);
			values["gadget_str_type"] = str_type;
			values["gadget_type"] = std::to_string(type);
			values["gadget_manufacturer"] = manufacturer;
			values["gadget_name"] = name;
			values["gadget_vnum"] = std::to_string(vnum);
			values["gadget_rarity"] = rarity_to_string(rarity);
			values["gadget_file"] = feed_file;
			mods::util::maps::dump(values);
			auto insert_transaction = txn();
			sql_compositor comp("object_gadget",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_gadget")
				.values(values)
				.returning("gadget_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && record : row){
				this->id = record["gadget_id"].as<uint64_t>();
				return this->id;
			}
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting new object_gadget record: '" << e.what() << "'\n";
			return 0;
		}
	}

	int16_t gadget_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file){
		try {
			d("[gadget-desc ffpr] feeding str_type...");
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_GADGET
			d("[gadget-desc ffpr] feeding base members...");
			MENTOC_FEED_BASE_MEMBERS
			d("done\n");
		}catch(std::exception &e){
			std::cerr << "[exception] rifle feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}


	int16_t gadget_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			return -1;
		}
		if(!out_file.good()){
			return -2;
		}
		MENTOC_EXAMPLE_GADGETS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_GADGET_MEMBERS_TUPLE)
		base_items(&out_file,"XMC Trip Wire","TRIPWIRE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	void gadget_description_t::fill_flags(obj_flag_data* o){
		d("[gadget_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_GADGET;
		switch((mw_gadget)this->type){
			case mw_gadget::GRAPPLING_HOOK:
			case mw_gadget::BARBED_WIRE:
			case mw_gadget::CAMERA:
			case mw_gadget::THERMAL_CAMERA:
			case mw_gadget::NIGHT_VISION_CAMERA:
			case mw_gadget::MOTION_SENSOR:
			case mw_gadget::DEPLOYABLE_SHIELD:
			case mw_gadget::TRIPWIRE:
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				break;
		}
		d("[gadget_description_t]::[fill_flags]*****\n");
	}


	int16_t attachment_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
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
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
			return -2;
		}
		MENTOC_EXAMPLE_DRONES
			MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_DRONE_MEMBERS_TUPLE)
			base_items(&out_file, "TN Land Drone","GROUND_DRONE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	void rifle_description_t::fill_flags(obj_flag_data* o){
		d("[rifle_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		o->ammo = this->ammo_max;
		o->ammo_max = this->ammo_max;
		o->weapon_flags = this->type;
		o->clip_size =this->clip_size;
		(*tf) = ITEM_RIFLE;
		switch((mw_rifle)this->type){
			case mw_rifle::PISTOL:
			case mw_rifle::HANDGUN:
			case mw_rifle::MACHINE_PISTOL:
				(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD | ITEM_WEAR_SECONDARY;
				break;
			case mw_rifle::SHOTGUN:
			case mw_rifle::ASSAULT_RIFLE:
			case mw_rifle::SUB_MACHINE_GUN:
			case mw_rifle::SNIPER:
			case mw_rifle::LIGHT_MACHINE_GUN:
				(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD;
				break;
			default: 
				d("[fill_wear_flags][rifle][WARNING] defaulted..\n");
				break;
		}
		d("[rifle_description_t]::[fill_flags]*****\n");
	}
	/** vim sorcery:

		s/(\([A-Z_]\+\)) .*$/\t\tcase mw_gadget::\1:\r\t\t(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;\r\t\t\treturn;\r/g

*/
	void armor_description_t::fill_flags(obj_flag_data* o){
		d("[armor_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_ARMOR;
		switch((mw_armor)this->type){
			default:
				d("[fill_wear_flags][armor][WARNING] defaulted..\n");
				break;
			case mw_armor::VEST:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_BODY | ITEM_WEAR_HOLD;
				break;

			case mw_armor::LEGS:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_LEGS;
				break;

			case mw_armor::GAUNTLETS:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_ARMS;
				break;

			case mw_armor::SHOULDER_PADS:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_TAKE;
				break;

			case mw_armor::HELMET:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_HEAD;
				break;

			case mw_armor::HAT:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_HEAD;
				break;

			case mw_armor::EYEWEAR:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_GOGGLES;
				break;

			case mw_armor::GOGGLES:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_GOGGLES;
				break;

			case mw_armor::BACKPACK:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_BACKPACK;
				break;

			case mw_armor::WAIST_PACK:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_WAIST;
				break;

			case mw_armor::VEST_PACK:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_VEST_PACK;
				break;

			case mw_armor::ELBOW_GUARDS:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_ELBOW;
				break;

			case mw_armor::GLOVES:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_HANDS;
				break;

			case mw_armor::SHIELD:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_SHIELD;
				break;
		}
		d("[armor_description_t]::[fill_flags]*****\n");
	}
	void drone_description_t::fill_flags(obj_flag_data* o){
		d("[drone_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_DRONE;
		switch((mw_drone)this->type){
			default: 
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				d("[fill_wear_flags][armor][WARNING] defaulted..\n");
				break;
		}
		d("[drone_description_t]::[fill_flags]*****\n");
	}
	void consumable_description_t::fill_flags(obj_flag_data* o){
		d("[consumable_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_CONSUMABLE;
		switch((mw_consumable)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				d("[fill_wear_flags][consumable][WARNING] defaulted..\n");
				break;
		}
		d("[consumable_description_t]::[fill_flags]*****\n");
	}
	void trap_description_t::fill_flags(obj_flag_data* o){
		d("[trap_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_TRAP;
		switch((mw_trap)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				d("[fill_wear_flags][trap][WARNING] defaulted..\n");
				break;
		}
		d("[trap_description_t]::[fill_flags]*****\n");
	}
	void attachment_description_t::fill_flags(obj_flag_data* o){
		d("[attachment_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_ATTACHMENT;
		switch((mw_attachment)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				d("[fill_wear_flags][attachment][WARNING] defaulted..\n");
				break;
		}
		d("[attachment_description_t]::[fill_flags]*****\n");
	}

	uint64_t rifle_description_t::db_id(){
		return this->id;
	}
	uint64_t armor_description_t::db_id(){
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
		//d("write_example_file: filename: '" << file_name.c_str() << "'\n");
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
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
			base_items(&out_file,"A PSG-1 Sniper Rifle","SNIPER");
		out_file.flush();
		out_file.close();
		return 0;
	};
	uint64_t rifle_description_t::flush_to_db(){
		try{
			d("[status] rifle flush_to_db.. flushing...\n");
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


	/** explosives */
	/** explosives */
	/** explosives */
	void explosive_description_t::fill_flags(obj_flag_data* o){
		d("[explosive_description_t]::[fill_flags]*****\n");
		auto * w = &o->wear_flags;
		auto * tf = &o->type_flag;
		(*tf) = ITEM_EXPLOSIVE;
		switch((mw_explosive)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				d("[fill_wear_flags][explosive][WARNING] defaulted..\n");
				break;
		}
		d("[explosive_description_t]::[fill_flags]*****\n");
	}
	uint64_t explosive_description_t::db_id(){
		return this->id;
	}
	int16_t explosive_description_t::feed(std::string_view in_file){
		try {
			std::string file = current_working_dir() + "/" + in_file.data();
			feed_file = file;
			d("[explosivedesc feed] feed_file: '" << feed_file << "'\n");
			d("[explosivedesc file: '" << file << "'\n");
			auto yaml_file = YAML::LoadFile(file);
			d("[explosivedesc feed(in_file)] feeding str_type...");
			auto type_string = yaml_file["str_type"].as<std::string>();
			d("[explosivedesc feed(in_file)] type_string:'" << type_string << "' done\n");
			d("[explosivedesc feed(in_file)] feeding all explosive members...");
			MENTOC_FEED_EXPLOSIVE
				d("[explosivedesc feed(in_file)] done\n");
			d("[explosivedesc feed(in_file)] feeding base members...");
			MENTOC_FEED_BASE_MEMBERS
				d("[explosivedesc feed(in_file)] done\n");
		}catch(std::exception &e){
			std::cerr << "[exception] explosive feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}
	int16_t explosive_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
			return -2;
		}
		MENTOC_EXAMPLE_EXPLOSIVES
			base_items(&out_file, "FS12 Fragmentation grenade","FRAG_GRENADE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	//
	// vim-sorcery: :555,569s/values\["\([^ ]\+\).*/\t\tvalues["\1"] = std::to_string(this->\1);/
	//
	uint64_t explosive_description_t::flush_to_db(){
		try{
			std::map<std::string,std::string> values;
			values["explosive_alternate_explosion_type"] = this->alternate_explosion_type;
			values["explosive_chance_to_injure"] = std::to_string(this->chance_to_injure);
			values["explosive_critical_chance"] = std::to_string(this->critical_chance);
			values["explosive_critical_range"] = std::to_string(this->critical_range);
			values["explosive_blast_radius"] = std::to_string(this->blast_radius);
			values["explosive_damage_per_second"] = std::to_string(this->damage_per_second);
			values["explosive_disorient_amount"] = std::to_string(this->disorient_amount);
			values["explosive_loudness_type"] = this->loudness_type;
			values["explosive_str_type"] = this->str_type;
			values["explosive_type"] = std::to_string(this->type);
			values["explosive_manufacturer"] = this->manufacturer;
			values["explosive_name"] = this->name;
			values["explosive_vnum"] = std::to_string(this->vnum);
			values["explosive_rarity"] = rarity_to_string(this->rarity);
			values["explosive_file"] = this->feed_file;
			auto insert_transaction = txn();
			sql_compositor comp("object_explosive",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_explosive")
				.values(values)
				.returning("explosive_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			for(auto && record : row){
				return record["explosive_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			std::cerr << __FILE__ << ": " << __LINE__ << ": error inserting new object_explosive record: '" << e.what() << "'\n";
			return 0;
		}
	}
	/** explosives */
	/** explosives */
	/** explosives */

	int16_t consumable_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
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
			d("can't open output yaml file, not open!\n");
			return -1;
		}
		if(!out_file.good()){
			d("can't open output yaml file, not good!\n");
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
		try {
			std::string file = current_working_dir() + "/" + in_file.data();
			d("gadget-desc absolute path to feed file: '" << file << "'\n");
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			this->str_type = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_GADGET
			MENTOC_FEED_BASE_MEMBERS
			this->durability_profile_enum = mods::yaml::to_durability_profile(yaml_file["durability_profile"].as<std::string>());
			this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
		}catch(std::exception &e){
			std::cerr << "[exception] gadget feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}


	int16_t rifle_description_t::feed(std::string_view in_file){
		try {
			std::string file = current_working_dir() + "/" + in_file.data();
			feed_file = file;
			d("[rifledesc feed] feed_file: '" << feed_file << "'\n");
			d("[rifledesc file: '" << file << "'\n");
			auto yaml_file = YAML::LoadFile(file);
			d("[rifledesc feed(in_file)] grabbing accuracy_map (rifle feed func)...\n");
			auto acmap = yaml_file["accuracy_map"].as<std::vector<float>>();
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				d("[rifledesc feed(in_file)] [acmap]: " << acmap[i] << "\n");
				accuracy_map[i] = acmap[i];
			}
			d("[rifledesc feed(in_file)] feeding damage_map...\n");
			auto dmap = yaml_file["damage_map"].as<std::vector<float>>();
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				d("[rifledesc feed(in_file)] [acmap]: " << dmap[i] << "\n");
				damage_map[i] = dmap[i];
			}
			d("[rifledesc feed(in_file)] done feeding damage_map...\n");
			d("[rifledesc feed(in_file)] feeding str_type...");
			auto type_string = yaml_file["str_type"].as<std::string>();
			d("[rifledesc feed(in_file)] done\n");
			d("[rifledesc feed(in_file)] feeding all rifle members...");
			MENTOC_FEED_RIFLE
				d("[rifledesc feed(in_file)] done\n");
			d("[rifledesc feed(in_file)] feeding base members...");
			MENTOC_FEED_BASE_MEMBERS
				d("[rifledesc feed(in_file)] done\n");
		}catch(std::exception &e){
			std::cerr << "[exception] rifle feed: '" << e.what() << "'\n";
			return -1;
		}
		return 0;
	}

	int16_t rifle_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file){
		try {
			d("[rifledesc ffpr] grabbing accuracy_map (rifle feed func)...\n");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("accuracy_map_") + std::to_string(i)].as<float>();
				accuracy_map[i] = item;
			}
			d("[rifledesc ffpr] feeding damage_map...\n");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("damage_map_") + std::to_string(i)].as<float>();
				damage_map[i] = item;
			}
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_RIFLE
			MENTOC_FEED_BASE_MEMBERS
			d("done\n");
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
			this->durability_profile_enum = mods::yaml::to_durability_profile(yaml_file["durability_profile"].as<std::string>());
		this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
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
