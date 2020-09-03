#include "db-report.hpp"
#include "yaml.hpp"
#include "sql.hpp"
#include "object-utils.hpp"
#include "pq.hpp"
#include "filesystem.hpp"
#ifndef tostr
#define tostr(a) std::to_string(a)
#endif
using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
namespace mods::yaml {
#ifdef __MENOC_FEED_BASE_MEMBERS_DEBUG__
#define debug_echo(a) std::cerr << "[MENTOC_FEED_BASE_MEMBERS][DEBUG]:'" << a << "'\n"; 
#else
#define debug_echo(a) /**/
#endif
#define MENTOC_FILE_EXISTS_PREAMBLE(type)\
	std::string file = true_path(#type,in_file);\
		if(!mods::filesystem::file_exists(file)){\
			mods::object_utils::yaml_file_doesnt_exist(#type);\
			return -1;\
		}

#define MENTOC_FEED_BASE_MEMBERS \
	try{ \
		debug_echo("manufacturer");\
		manufacturer = yaml_file["manufacturer"].as<std::string>();\
		fed_items.push_back("manufacturer");\
		debug_echo("name");\
		name = yaml_file["name"].as<std::string>();\
		fed_items.push_back("name");\
		debug_echo("str_type");\
		str_type = yaml_file["str_type"].as<std::string>();\
		fed_items.push_back("str_type");\
		debug_echo("vnum");\
		vnum = yaml_file["vnum"].as<int>();\
		fed_items.push_back("vnum");\
		debug_echo("rarity");\
		rarity = parse_rarity(yaml_file["rarity"].as<std::string>());\
		fed_items.push_back("rarity");\
		debug_echo("feed_file");\
		feed_file = yaml_file["feed_file"].as<std::string>();\
		fed_items.push_back("feed_file");\
		debug_echo("description");\
		description = yaml_file["description"].as<std::string>();\
		fed_items.push_back("description");\
		debug_echo("short_description");\
		short_description = yaml_file["short_description"].as<std::string>();\
		fed_items.push_back("short_description");\
		debug_echo("action_desc");\
		action_description = yaml_file["action_description"].as<std::string>();\
		fed_items.push_back("action_description");\
	}catch(YAML::Exception &e){\
		REPORT_DB_ISSUE("yaml error",e.what());\
		this->feed_status = -3; mods::object_utils::report_yaml_exception(e,fed_items); }

	std::string true_path(std::string_view type,std::string_view in_file){
		std::string cwd = current_working_dir();
		std::string prefixed = CAT(cwd,"/objects/",type.data(),"/",in_file.data());
		std::string standalone = CAT(cwd,"/",in_file.data());
		if(mods::filesystem::file_exists(prefixed)){
			return prefixed;
		}
		if(mods::filesystem::file_exists(standalone)){
			return standalone;
		}
		return "";
	}

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
			return -1;
		}
		if(!out_file.good()){
			return -1;
		}
		MENTOC_EXAMPLE_ARMORS
			MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)
			base_items(&out_file,"MK-71X Ballistic Vest","VEST");
		out_file.flush();
		out_file.close();
		return 0;
	}

	void armor_description_t::generate_map(){
			this->exported["armor_csv_capabilities"] = this->csv_capabilities; //VARCHAR(2048),
			this->exported["armor_csv_attach_to"] = this->csv_attach_to; //VARCHAR(2048),
			this->exported["armor_thac0"] = this->thac0; //INTEGER NOT NULL,
			this->exported["armor_weight_in_lbs"] = this->weight_in_lbs; //FLOAT NOT NULL,
			this->exported["armor_fire_resistance_percent"] = this->fire_resistance_percent; //FLOAT NOT NULL DEFAULT 0.0,
			this->exported["armor_balistic_resistance_percent"] = this->balistic_resistance_percent; //FLOAT NOT NULL DEFAULT 0.0,
			this->exported["armor_speed_profile"] = this->speed_profile; //speed_profile_type_t NOT NULL DEFAULT 'HINDERED',
			this->exported["armor_offensive_damage_amount"] = this->offensive_damage_amount; //INTEGER NOT NULL DEFAULT 0,
			this->exported["armor_durability_profile"] = this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum); //durability_profile_type_t NOT NULL DEFAULT 'DURABLE',
			this->exported["armor_str_type"] = str_type;
			this->exported["armor_type"] = std::to_string(type);
			this->exported["armor_manufacturer"] = manufacturer;
			this->exported["armor_name"] = name;
			this->exported["armor_vnum"] = std::to_string(vnum);
			this->exported["armor_rarity"] = rarity_to_string(rarity);
			this->exported["armor_file"] = feed_file;
	}
	uint64_t armor_description_t::flush_to_db(){
		try{
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("object_armor",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_armor")
				.values(this->exported)
				.returning("armor_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()){
				return row[0]["armor_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error inserting new object_armor record: '",e.what());
			return 0;
		}
	}
	void gadget_description_t::generate_map(){
			this->exported["gadget_csv_capabilities"] = csv_capabilities;
			this->exported["gadget_csv_attach_to"] = csv_attach_to;
			this->exported["gadget_health_points"] = std::to_string(health_points);
			this->exported["gadget_electronic"] = std::to_string(electronic);
			this->exported["gadget_shield_points"] = std::to_string(shield_points);
			this->exported["gadget_durability_profile"] = this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
			this->exported["gadget_move_points"] = std::to_string(move_points);
			this->exported["gadget_damage_points"] = std::to_string(damage_points);
			this->exported["gadget_str_type"] = str_type;
			this->exported["gadget_type"] = std::to_string(type);
			this->exported["gadget_manufacturer"] = manufacturer;
			this->exported["gadget_name"] = name;
			this->exported["gadget_vnum"] = std::to_string(vnum);
			this->exported["gadget_rarity"] = rarity_to_string(rarity);
			this->exported["gadget_file"] = feed_file;
	}
	uint64_t gadget_description_t::flush_to_db(){
		try{
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("object_gadget",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_gadget")
				.values(this->exported)
				.returning("gadget_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()){
				return this->id = row[0]["gadget_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error inserting new object_gadget record: '",e.what());
			return 0;
		}
	}

	int16_t gadget_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file){
		try {
			auto type_string = yaml_file["str_type"].as<std::string>();
			std::vector<std::string> fed_items;
			MENTOC_FEED_GADGET
			MENTOC_FEED_BASE_MEMBERS
		}catch(std::exception &e){
			REPORT_DB_ISSUE("rifle feed error",e.what());
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
	void gadget_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_GADGET;
		o->gadget()->type = (mw_gadget)this->type;
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
	}


	int16_t attachment_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			return -1;
		}
		if(!out_file.good()){
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
			return -1;
		}
		if(!out_file.good()){
			return -2;
		}
		MENTOC_EXAMPLE_DRONES
			MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_DRONE_MEMBERS_TUPLE)
			base_items(&out_file, "TN Land Drone","GROUND_DRONE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	void rifle_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		o->obj_flags.ammo = this->ammo_max;
		o->obj_flags.ammo_max = this->ammo_max;
		o->obj_flags.clip_size =this->clip_size;
		(*tf) = ITEM_RIFLE;

		bool recognized = 0;
		o->rifle()->type = (mw_rifle)this->type;
		switch((mw_rifle)this->type){
			case mw_rifle::PISTOL:
			case mw_rifle::HANDGUN:
			case mw_rifle::MACHINE_PISTOL:
				(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD | ITEM_WEAR_SECONDARY;
				recognized = 1;
				break;
			case mw_rifle::SHOTGUN:
			case mw_rifle::ASSAULT_RIFLE:
			case mw_rifle::SUB_MACHINE_GUN:
			case mw_rifle::SNIPER:
			case mw_rifle::LIGHT_MACHINE_GUN:
				(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD;
				recognized = 1;
				break;
			default: 
				(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD | ITEM_WEAR_SECONDARY;
				log("[rifle_description_t][WARNING] fill_flags used the default flags");
				break;
		}
		o->rifle_instance = std::make_unique<rifle_instance_data>();
		if(recognized){
			o->rifle_instance->ammo = this->clip_size;
		}
		o->obj_flags.weapon_flags = this->type;
	}
	/** vim sorcery:

		s/(\([A-Z_]\+\)) .*$/\t\tcase mw_gadget::\1:\r\t\t(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;\r\t\t\treturn;\r/g

*/
	void armor_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_ARMOR;
		o->armor()->type = (mw_armor)this->type;
		switch((mw_armor)this->type){
			default:
				log("[armor_description_t][WARNING] fill_flags used the default flags");
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
	}
	void drone_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_DRONE;
		o->drone()->type = (mw_drone)this->type;
		switch((mw_drone)o->extended_type){
			default: 
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				log("[drone_description_t][WARNING] fill_flags used the default flags");
				break;
		}
	}
	void consumable_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_CONSUMABLE;
		o->consumable()->type = (mw_consumable)this->type;
		switch((mw_consumable)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				log("[consumable_description_t][WARNING] fill_flags used the default flags");
				break;
		}
	}
	void trap_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_TRAP;
		o->trap()->type = (mw_trap)this->type;
		switch((mw_trap)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				log("[trap_description_t][WARNING] fill_flags used the default flags");
				break;
		}
	}
	void attachment_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_ATTACHMENT;
		o->attachment()->type = (mw_attachment)this->type;
		switch((mw_attachment)this->type){
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				log("[attachment_description_t][WARNING] fill_flags used the default flags");
				break;
		}
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
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			return -1;
		}
		if(!out_file.good()){
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
			base_items(&out_file,"A PSG-1 Sniper Rifle",std::to_string(ITEM_RIFLE));
		out_file.flush();
		out_file.close();
		return 0;
	};
	void rifle_description_t::generate_map(){
			std::string rifm = "rifle_accuracy_map_",
				dam = "rifle_damage_map_";
			for(unsigned i = 0; i < MAX_ROOM_DISTANCE;i++) {
				this->exported[rifm + std::to_string(i)] = std::to_string(accuracy_map[i]);
				this->exported[dam + std::to_string(i)] = std::to_string(damage_map[i]);
			}
			this->exported["rifle_ammo_max"] = std::to_string(ammo_max);
			this->exported["rifle_ammo_type"] = ammo_type;
			this->exported["rifle_chance_to_injure"] = std::to_string(chance_to_injure);
			this->exported["rifle_clip_size"] = std::to_string(clip_size);
			this->exported["rifle_cooldown_between_shots"] = std::to_string(cooldown_between_shots);
			this->exported["rifle_critical_chance"] = std::to_string(critical_chance);
			this->exported["rifle_critical_range"] = std::to_string(critical_range);
			this->exported["rifle_damage_per_second"] = std::to_string(damage_per_second);
			this->exported["rifle_disorient_amount"] = std::to_string(disorient_amount);
			this->exported["rifle_headshot_bonus"] = std::to_string(headshot_bonus);
			this->exported["rifle_max_range"] = std::to_string(max_range);
			this->exported["rifle_range_multiplier"] = std::to_string(range_multiplier);
			this->exported["rifle_reload_time"] = std::to_string(reload_time);
			this->exported["rifle_rounds_per_minute"] = std::to_string(rounds_per_minute);
			this->exported["rifle_muzzle_velocity"] = std::to_string(muzzle_velocity);
			this->exported["rifle_effective_firing_range"] = std::to_string(effective_firing_range);
			this->exported["rifle_str_type"] = str_type;
			this->exported["rifle_type"] = std::to_string(type);
			this->exported["rifle_manufacturer"] = manufacturer;
			this->exported["rifle_name"] = name;
			this->exported["rifle_vnum"] = std::to_string(vnum);
			this->exported["rifle_file"] = feed_file;
			for(auto & rarity_pair : rarity_strings()){
				if(rarity == rarity_pair.second){
					this->exported["rifle_rarity"] = rarity_pair.first;
					break;
				}
			}
	}

	uint64_t rifle_description_t::flush_to_db(){
		try{
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("object_rifle",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_rifle")
				.values(this->exported)
				.returning("rifle_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()){
				return this->id = row[0]["rifle_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error inserting new object_rifle record: '",e.what());
			return 0;
		}
	}


	/** explosives */
	/** explosives */
	/** explosives */
	void explosive_description_t::fill_flags(obj_data* o){
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_EXPLOSIVE;
		(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
		o->explosive()->type = (mw_explosive)this->type;
		switch((mw_explosive)this->type){
			case mw_explosive::SENSOR_GRENADE:
				o->explosive()->attributes->alternate_explosion_type = ALTEX_SCAN;
				break;
			case mw_explosive::SMOKE_GRENADE:
				o->explosive()->attributes->alternate_explosion_type = ALTEX_SMOKE;
				break;
			case mw_explosive::EMP_GRENADE:
				o->explosive()->attributes->alternate_explosion_type = ALTEX_EMP;
				break;
			case mw_explosive::INCENDIARY_GRENADE:
				o->explosive()->attributes->alternate_explosion_type = ALTEX_INCENDIARY;
				break;
			case mw_explosive::FLASHBANG_GRENADE:
				o->explosive()->attributes->alternate_explosion_type = ALTEX_FLASHBANG;
				break;
			default:
				break;
		}
	}
	uint64_t explosive_description_t::db_id(){
		return this->id;
	}
	int16_t explosive_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(explosive);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_EXPLOSIVE
		}catch(YAML::Exception &e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
			return -2;
		}
		return 0;
	}
	int16_t explosive_description_t::write_example_file(std::string_view file){
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()){
			return -1;
		}
		if(!out_file.good()){
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
	void explosive_description_t::generate_map(){
			this->exported["explosive_alternate_explosion_type"] = this->alternate_explosion_type;
			this->exported["explosive_chance_to_injure"] = std::to_string(this->chance_to_injure);
			this->exported["explosive_critical_chance"] = std::to_string(this->critical_chance);
			this->exported["explosive_critical_range"] = std::to_string(this->critical_range);
			this->exported["explosive_blast_radius"] = std::to_string(this->blast_radius);
			this->exported["explosive_damage_per_second"] = std::to_string(this->damage_per_second);
			this->exported["explosive_disorient_amount"] = std::to_string(this->disorient_amount);
			this->exported["explosive_loudness_type"] = this->loudness_type;
			this->exported["explosive_str_type"] = this->str_type;
			this->exported["explosive_type"] = std::to_string(this->type);
			this->exported["explosive_manufacturer"] = this->manufacturer;
			this->exported["explosive_name"] = this->name;
			this->exported["explosive_vnum"] = std::to_string(this->vnum);
			this->exported["explosive_rarity"] = rarity_to_string(this->rarity);
			this->exported["explosive_file"] = this->feed_file;
		}
	uint64_t explosive_description_t::flush_to_db(){
		try{
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("object_explosive",&insert_transaction);
			auto up_sql = comp
				.insert()
				.into("object_explosive")
				.values(this->exported)
				.returning("explosive_id")
				.sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()){
				return this->id = row[0]["explosive_id"].as<uint64_t>();
			}
			return 0;
		}catch(std::exception& e){
			REPORT_DB_ISSUE(": error inserting new object_explosive record: '",e.what());
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
			return -1;
		}
		if(!out_file.good()){
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
			return -1;
		}
		if(!out_file.good()){
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
		MENTOC_FILE_EXISTS_PREAMBLE(gadget);
			std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			this->str_type = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_GADGET
			MENTOC_FEED_BASE_MEMBERS
			this->durability_profile_enum = mods::yaml::to_durability_profile(yaml_file["durability_profile"].as<std::string>());
			this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception &e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}


	int16_t rifle_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(rifle);
			std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto acmap = yaml_file["accuracy_map"].as<std::vector<float>>();
			fed_items.push_back("accuracy_map");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				accuracy_map[i] = acmap[i];
				fed_items.push_back(std::string("accuracy_map[") + std::to_string(i) + "]");
			}
			auto dmap = yaml_file["damage_map"].as<std::vector<float>>();
			fed_items.push_back("damage_map");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				damage_map[i] = dmap[i];
				fed_items.push_back(std::string("damage_map[") + std::to_string(i) + "]");
			}
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_RIFLE
			this->base_stat_list = mods::weapon::weapon_stats(this->type);
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception &e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}

	int16_t rifle_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file){
		exit(200);
		try {
			std::vector<std::string> fed_items;
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("accuracy_map_") + std::to_string(i)].as<float>();
				accuracy_map[i] = item;
			}
			for(unsigned i=0; i < MAX_ROOM_DISTANCE;i++){
				float item = yaml_file[std::string("damage_map_") + std::to_string(i)].as<float>();
				damage_map[i] = item;
			}
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_RIFLE
		}catch(std::exception &e){
			REPORT_DB_ISSUE("rifle error",e.what());
			return -1;
		}
		return 0;
	}


	int16_t drone_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(drone);
		std::vector<std::string> fed_items;
		try {
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			std::vector<std::string> fed_items;
		auto type_string = yaml_file["str_type"].as<std::string>();
		fed_items.push_back("str_type");
		MENTOC_FEED_DRONE
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception& e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}
	int16_t attachment_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(attachment);
		std::vector<std::string> fed_items;
		try{
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		fed_items.push_back("str_type");
		MENTOC_FEED_ATTACHMENT
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception& e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}
	int16_t armor_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(armor);
		std::vector<std::string> fed_items;
		try{
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		fed_items.push_back("str_type");
		MENTOC_FEED_ARMOR
			MENTOC_FEED_BASE_MEMBERS
			this->durability_profile_enum = mods::yaml::to_durability_profile(yaml_file["durability_profile"].as<std::string>());
		this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
			this->feed_status = 0;
		return 0;
		}catch(YAML::Exception& e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}

	int16_t consumable_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(consumable);
			std::vector<std::string> fed_items;
		try{
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		fed_items.push_back("str_type");
		MENTOC_FEED_CONSUMABLE
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception& e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}

	int16_t trap_description_t::feed(std::string_view in_file){
		MENTOC_FILE_EXISTS_PREAMBLE(trap);
			std::vector<std::string> fed_items;
		try{
		feed_file = file;
		YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
		auto type_string = yaml_file["str_type"].as<std::string>();
		fed_items.push_back("str_type");
		MENTOC_FEED_TRAP
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		}catch(YAML::Exception& e){
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
		this->feed_status = -2;
			return -2;
		}
	}


};
