#include "db-report.hpp"
#include "yaml.hpp"
#include "sql.hpp"
#include "object-utils.hpp"
#include "pq.hpp"
#include "filesystem.hpp"
#ifndef tostr
#define tostr(a) std::to_string(a)
#endif

#ifndef MELEE_STUB
#define MELEE_STUB(A) std::cerr << red_str("TODO stub: MELEE ") << "\n";
#endif

#ifndef VEHICLE_STUB
#define VEHICLE_STUB(A) std::cerr << red_str("TODO stub: VEHICLE ") << "\n";
#endif

using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
namespace mods::yaml {
	template <typename T>
	static inline std::string to_string(T& item) {
		return std::to_string(item);
	}
	template <>
	inline std::string to_string<std::string>(std::string& item) {
		return item;
	}

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
	}catch(YAML::Exception &e){this->feed_status = -3; mods::object_utils::report_yaml_exception(e,fed_items); REPORT_DB_ISSUE("yaml error",e.what()); }

	std::string true_path(std::string_view type,std::string_view in_file) {
		std::string cwd = current_working_dir();
		std::string prefixed = CAT(cwd,"/objects/",type.data(),"/",in_file.data());
		std::string standalone = CAT(cwd,"/",in_file.data());
		if(mods::filesystem::file_exists(prefixed)) {
			return prefixed;
		}
		if(mods::filesystem::file_exists(standalone)) {
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
	std::string rarity_segment() {
		std::string comment = "#rarity types\n";
		for(auto& rarity_pair : rarity_strings()) {
			comment += "# " + rarity_pair.first + "\n";
		}
		comment += "rarity: 'COMMON'\n";
		return comment;
	}
	void base_items(std::ofstream* out_file,std::string_view name,std::string_view obj_type) {
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
		for(auto& rarity_pair : rarity_strings()) {
			if(!rarity_pair.first.compare(rarity_string)) {
				return rarity_pair.second;
			}
		}
		return mods::rarity::DEFAULT;
	}
	std::string rarity_to_string(float val) {
		for(auto& rarity_pair : rarity_strings()) {
			if(rarity_pair.second == val) {
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
	int16_t armor_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -1;
		}
		MENTOC_EXAMPLE_ARMORS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_ARMOR_MEMBERS_TUPLE)
		base_items(&out_file,"Heavy Metal Helmet","HELMET");
		out_file.flush();
		out_file.close();
		return 0;
	}

	int16_t container_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -1;
		}
		MENTOC_EXAMPLE_CONTAINERS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_CONTAINER_MEMBERS_TUPLE)
		base_items(&out_file,"Weapons Locker","WEAPON_LOCKER");
		out_file.flush();
		out_file.close();
		return 0;
	}
	void consumable_description_t::generate_map() {
		exported.clear();
		exported["consumable_csv_capabilities"] = (this->csv_capabilities);
		exported["consumable_csv_attach_to"] = (this->csv_attach_to);
		exported["consumable_weight_in_lbs"] = std::to_string(this->weight_in_lbs);
		exported["consumable_consumed_by"] = (this->consumed_by);
		exported["consumable_adds_hp"] = std::to_string(this->adds_hp);
		exported["consumable_adds_movement"] = std::to_string(this->adds_movement);
		exported["consumable_adds_room_range"] = std::to_string(this->adds_room_range);
		exported["consumable_adds_strength"] = std::to_string(this->adds_strength);
		exported["consumable_adds_intelligence"] = std::to_string(this->adds_intelligence);
		exported["consumable_adds_charisma"] = std::to_string(this->adds_charisma);
		exported["consumable_adds_dexterity"] = std::to_string(this->adds_dexterity);
		exported["consumable_adds_constitution"] = std::to_string(this->adds_constitution);
		exported["consumable_adds_wisdom"] = std::to_string(this->adds_wisdom);
		exported["consumable_adds_weight"] = std::to_string(this->adds_weight);
		exported["consumable_adds_experience"] = std::to_string(this->adds_experience);
		exported["consumable_adds_armor_class"] = std::to_string(this->adds_armor_class);
		exported["consumable_adds_hitroll"] = std::to_string(this->adds_hitroll);
		exported["consumable_adds_damage_roll"] = std::to_string(this->adds_damage_roll);
		exported["consumable_ticks_until_zero"] = std::to_string(this->ticks_until_zero);
		exported["consumable_ammo_type"] = (this->ammo_type);
		exported["consumable_capacity"] = std::to_string(this->capacity);
		exported["consumable_adds_fire_damage"] = std::to_string(this->adds_fire_damage);
		exported["consumable_adds_critical_chance"] = std::to_string(this->adds_critical_chance);
		exported["consumable_adds_ammo_max"] = std::to_string(this->adds_ammo_max);
		exported["consumable_adds_chance_to_injure"] = std::to_string(this->adds_chance_to_injure);
		exported["consumable_adds_clip_size"] = std::to_string(this->adds_clip_size);
		exported["consumable_adds_cooldown_between_shots"] = std::to_string(this->adds_cooldown_between_shots);
		exported["consumable_adds_critical_range"] = std::to_string(this->adds_critical_range);
		exported["consumable_adds_disorient_amount"] = std::to_string(this->adds_disorient_amount);
		exported["consumable_adds_headshot_bonus"] = std::to_string(this->adds_headshot_bonus);
		exported["consumable_adds_max_range"] = std::to_string(this->adds_max_range);
		exported["consumable_adds_reload_time"] = std::to_string(this->adds_reload_time);
		exported["consumable_adds_muzzle_velocity"] = std::to_string(this->adds_muzzle_velocity);
		exported["consumable_adds_damage_dice_count"] = std::to_string(this->adds_damage_dice_count);
	}
	void attachment_description_t::generate_map() {
		exported.clear();
	}
	void trap_description_t::generate_map() {
		exported.clear();
	}
	void vehicle_description_t::generate_map() {
		exported.clear();
	}
	void drone_description_t::generate_map() {
		exported.clear();
	}
	void container_description_t::generate_map() {
		exported["container_detailed_description"] = this->detailed_description;
	}
	void armor_description_t::generate_map() {
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
		this->exported["armor_hp"] = std::to_string(hp);
		this->exported["armor_classification"] = classification;
		this->exported["armor_worth"] = std::to_string(worth);
		this->exported["armor_incendiary_resistance_percent"] = std::to_string(this->incendiary_resistance_percent);
		this->exported["armor_explosive_resistance_percent"] = std::to_string(this->explosive_resistance_percent);
		this->exported["armor_shrapnel_resistance_percent"] = std::to_string(this->shrapnel_resistance_percent);
		this->exported["armor_corrosive_resistance_percent"] = std::to_string(this->corrosive_resistance_percent);
		this->exported["armor_cryogenic_resistance_percent"] = std::to_string(this->cryogenic_resistance_percent);
		this->exported["armor_radiation_resistance_percent"] = std::to_string(this->radiation_resistance_percent);
		this->exported["armor_emp_resistance_percent"] = std::to_string(this->emp_resistance_percent);
		this->exported["armor_shock_resistance_percent"] = std::to_string(this->shock_resistance_percent);
		this->exported["armor_anti_matter_resistance_percent"] = std::to_string(this->anti_matter_resistance_percent);
		this->exported["armor_stat_strength"] = std::to_string(stat_strength);
		this->exported["armor_stat_intelligence"] = std::to_string(stat_intelligence);
		this->exported["armor_stat_wisdom"] = std::to_string(stat_wisdom);
		this->exported["armor_stat_dexterity"] = std::to_string(stat_dexterity);
		this->exported["armor_stat_constitution"] = std::to_string(stat_constitution);
		this->exported["armor_stat_electronics"] = std::to_string(stat_electronics);
		this->exported["armor_stat_armor"] = std::to_string(stat_armor);
		this->exported["armor_stat_marksmanship"] = std::to_string(stat_marksmanship);
		this->exported["armor_stat_sniping"] = std::to_string(stat_sniping);
		this->exported["armor_stat_demolitions"] = std::to_string(stat_demolitions);
		this->exported["armor_stat_chemistry"] = std::to_string(stat_chemistry);
		this->exported["armor_stat_weapon_handling"] = std::to_string(stat_weapon_handling);
		this->exported["armor_stat_strategy"] = std::to_string(stat_strategy);
		this->exported["armor_stat_medical"] = std::to_string(stat_medical);
	}
	uint64_t container_description_t::flush_to_db() {
		try {
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("object_container",&insert_transaction);
			auto up_sql = comp
			              .insert()
			              .into("object_container")
			              .values(this->exported)
			              .returning("container_id")
			              .sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()) {
				return row[0]["container_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_armor record: '",e.what());
			return 0;
		}
	}
	uint64_t armor_description_t::flush_to_db() {
		try {
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
			if(row.size()) {
				return row[0]["armor_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_armor record: '",e.what());
			return 0;
		}
	}
	void gadget_description_t::generate_map() {
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
		this->exported["gadget_exit_key"] = std::to_string(exit_key);
		this->exported["gadget_min_level"] = std::to_string(min_level);
		this->exported["gadget_worth"] = std::to_string(worth);
	}
	uint64_t gadget_description_t::flush_to_db() {
		try {
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
			if(row.size()) {
				return this->id = row[0]["gadget_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_gadget record: '",e.what());
			return 0;
		}
	}

	int16_t gadget_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file) {
		try {
			auto type_string = yaml_file["str_type"].as<std::string>();
			std::vector<std::string> fed_items;
			MENTOC_FEED_GADGET
			MENTOC_FEED_BASE_MEMBERS
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("rifle feed error",e.what());
			return -1;
		}
		return 0;
	}


	int16_t gadget_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_GADGETS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_GADGET_MEMBERS_TUPLE)
		base_items(&out_file,"XMC Trip Wire","TRIPWIRE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	void container_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_CONTAINER;
		o->container()->type = (mw_container)this->type;
		switch((mw_container)this->type) {
			case mw_container::LOCKER:
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
				break;
		}
	}

	void gadget_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_GADGET;
		o->gadget()->type = (mw_gadget)this->type;
		switch((mw_gadget)this->type) {
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


	int16_t attachment_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_ATTACHMENTS
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_ATTACHMENT_MEMBERS_TUPLE)
		base_items(&out_file, "ACOG Scope","SIGHT");
		out_file.flush();
		out_file.close();
		return 0;
	};


	int16_t drone_description_t::write_example_file(std::string_view file) {
		/** TODO: */
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_DRONES
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_DRONE_MEMBERS_TUPLE)
		base_items(&out_file, "TN Land Drone","GROUND_DRONE");
		out_file.flush();
		out_file.close();
		return 0;
	};
	void rifle_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		o->obj_flags.ammo = this->ammo_max;
		o->obj_flags.ammo_max = this->ammo_max;
		o->obj_flags.clip_size =this->clip_size;
		(*tf) = ITEM_RIFLE;

		bool recognized = 0;
		o->rifle()->type = (mw_rifle)this->type;
		switch((mw_rifle)this->type) {
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
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log(CAT("[rifle_description_t][WARNING] fill_flags used the default flags for type: ",this->str_type,", and this file: '",this->feed_file,"'").c_str());
#endif
				break;
		}
		o->rifle_instance = std::make_unique<rifle_instance_data<attachment_data_t,obj_ptr_t,uuid_t>>();
		if(recognized) {
			o->rifle_instance->ammo = this->clip_size;
		}
		o->obj_flags.weapon_flags = this->type;
	}

	void vehicle_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		//bool recognized = 0;
		(*tf) = ITEM_MELEE;
		o->vehicle()->type = (mw_vehicle)this->type;
		(*w) |= ITEM_VEHICLE;
		o->obj_flags.weapon_flags = 0;
	}

	void melee_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_MELEE;
		o->melee()->type = (mw_melee)this->type;
		(*w) |= ITEM_WEAPON | ITEM_WEAR_TAKE | ITEM_WEAR_WIELD | ITEM_WEAR_PRIMARY | ITEM_WEAR_HOLD | ITEM_WEAR_SECONDARY;
		switch((mw_melee)this->type) {
			case mw_type::MACHETE:
			case mw_type::KNIFE:
			case mw_type::CROWBAR:
			case mw_type::CHAIN:
			case mw_type::WOODEN_BASEBALL_BAT:
			case mw_type::ALUMINUM_BASEBALL_BAT:
			case mw_type::HAMMER:
			case mw_type::SLEDGEHAMMER:
			case mw_type::BATON:
			case mw_type::HOCKEY_STICK:
			case mw_type::AXE:
			case mw_type::SHOVEL:
			case mw_type::PICKAXE:
			case mw_type::KATANA:
			case mw_type::MACE:
			case mw_type::LONGSWORD:
			case mw_type::NUNCHAKU:
			case mw_type::NAGINATA:
			case mw_type::DAGGER:
			case mw_type::SCYTHE:
			case mw_type::SHURIKEN:
			case mw_type::SPEAR:
			case mw_type::PIKE:
			case mw_type::HALBERD:
			case mw_type::LANCE:
			case mw_type::TRIDENT:
			case mw_type::QUARTERSTAFF:
			case mw_type::REBAR:
			case mw_type::SICKLE:
			case mw_type::ICE_PICK:
			case mw_type::CHAINSAW:
			case mw_type::BRASS_KNUCKLES:
				break;
			default:
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log(CAT("[melee_description_t][WARNING] fill_flags used the default flags for type: ",this->str_type,", and this file: '",this->feed_file,"'").c_str());
#endif
				break;
		}
		o->obj_flags.weapon_flags = this->type;
	}

	/** vim sorcery:

		s/(\([A-Z_]\+\)) .*$/\t\tcase mw_gadget::\1:\r\t\t(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;\r\t\t\treturn;\r/g

	*/
	void armor_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_ARMOR;
		o->armor()->type = (mw_armor)this->type;
#ifdef __MENTOC_FILL_FLAGS_OUTPUT__
		std::cerr << "armor_description_t::fill_flags() o->armor()->type: '" << green_str(std::to_string(o->armor()->type)) << "'\n";
#endif

		switch((mw_armor)this->type) {
			default:
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log(CAT("[melee_description_t][WARNING] fill_flags used the default flags for type: ",this->str_type,", and this file: '",this->feed_file,"'").c_str());
#endif
				break;
			case mw_armor::VEST:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_BODY | ITEM_WEAR_HOLD;
				break;

			case mw_armor::LEGS:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_LEGS;
				break;

			case mw_armor::FEET:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_FEET;
				break;

			case mw_armor::WAIST:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_WAIST;
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
			case mw_armor::WRIST:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_WRIST;
				break;


			case mw_armor::SHIELD:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD | ITEM_WEAR_SHIELD;
				break;
		}
		classification_enum = to_classification(classification);
	}
	void drone_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_DRONE;
		o->drone()->type = (mw_drone)this->type;
		switch((mw_drone)o->extended_type) {
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log("[drone_description_t][WARNING] fill_flags used the default flags");
#endif
				break;
		}
	}
	void consumable_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_CONSUMABLE;
		o->consumable()->type = (mw_consumable)this->type;
		switch((mw_consumable)this->type) {
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log("[consumable_description_t][WARNING] fill_flags used the default flags");
#endif
				break;
		}
	}
	void trap_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_TRAP;
		o->trap()->type = (mw_trap)this->type;
		switch((mw_trap)this->type) {
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log("[trap_description_t][WARNING] fill_flags used the default flags");
#endif
				break;
		}
	}
	void attachment_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_ATTACHMENT;
		o->attachment()->type = (mw_attachment)this->type;
		switch((mw_attachment)this->type) {
			default:
				(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
#ifdef __MENTOC_SHOW_DEFAULT_FILL_FLAGS_LOG__
				log("[attachment_description_t][WARNING] fill_flags used the default flags");
#endif
				break;
		}
	}

	int16_t vehicle_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_VEHICLE
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_VEHICLE_MEMBERS_TUPLE)
		base_items(&out_file,"Response team SUV",std::to_string(ITEM_VEHICLE));
		out_file.flush();
		out_file.close();
		return 0;
	};
	int16_t melee_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_MELEE
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_MELEE_MEMBERS_TUPLE)
		base_items(&out_file,"Brutal Machete",std::to_string(ITEM_MELEE));
		out_file.flush();
		out_file.close();
		return 0;
	};
	int16_t rifle_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		out_file << "# accuracy map\n" <<
		         "accuracy_map: [\n";
		for(auto i=0; i < MAX_ROOM_DISTANCE; ++i) {
			out_file << "10.00";
			if(i + 1 < MAX_ROOM_DISTANCE) {
				out_file << ",\n";
			}
		}
		out_file << "\n]\n" <<
		         "damage_map: [\n";
		for(auto i=0; i < MAX_ROOM_DISTANCE; ++i) {
			out_file << "10.00";
			if(i + 1 < MAX_ROOM_DISTANCE) {
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
	void melee_description_t::generate_map() {
		this->exported["melee_chance_to_injure"] = std::to_string(chance_to_injure);
		this->exported["melee_critical_chance"] = std::to_string(critical_chance);
		this->exported["melee_damage_per_second"] = std::to_string(damage_per_second);
		this->exported["melee_disorient_amount"] = std::to_string(disorient_amount);
		this->exported["melee_str_type"] = str_type;
		this->exported["melee_type"] = std::to_string(type);
		this->exported["melee_name"] = name;
		this->exported["melee_vnum"] = std::to_string(vnum);
		this->exported["melee_file"] = feed_file;
		this->exported["melee_incendiary_damage"] = std::to_string(incendiary_damage);
		this->exported["melee_explosive_damage"] = std::to_string(explosive_damage);
		this->exported["melee_shrapnel_damage"] = std::to_string(shrapnel_damage);
		this->exported["melee_corrosive_damage"] = std::to_string(corrosive_damage);
		this->exported["melee_cryogenic_damage"] = std::to_string(cryogenic_damage);
		this->exported["melee_radioactive_damage"] = std::to_string(radioactive_damage);
		this->exported["melee_emp_damage"] = std::to_string(emp_damage);
		this->exported["melee_shock_damage"] = std::to_string(shock_damage);
		this->exported["melee_anti_matter_damage"] = std::to_string(anti_matter_damage);
		this->exported["melee_stat_strength"] = std::to_string(stat_strength);
		this->exported["melee_stat_intelligence"] = std::to_string(stat_intelligence);
		this->exported["melee_stat_wisdom"] = std::to_string(stat_wisdom);
		this->exported["melee_stat_dexterity"] = std::to_string(stat_dexterity);
		this->exported["melee_stat_constitution"] = std::to_string(stat_constitution);
		this->exported["melee_stat_electronics"] = std::to_string(stat_electronics);
		this->exported["melee_stat_armor"] = std::to_string(stat_armor);
		this->exported["melee_stat_marksmanship"] = std::to_string(stat_marksmanship);
		this->exported["melee_stat_sniping"] = std::to_string(stat_sniping);
		this->exported["melee_stat_demolitions"] = std::to_string(stat_demolitions);
		this->exported["melee_stat_chemistry"] = std::to_string(stat_chemistry);
		this->exported["melee_stat_weapon_handling"] = std::to_string(stat_weapon_handling);
		this->exported["melee_stat_strategy"] = std::to_string(stat_strategy);
		this->exported["melee_stat_medical"] = std::to_string(stat_medical);
		for(auto& rarity_pair : rarity_strings()) {
			if(rarity == rarity_pair.second) {
				this->exported["melee_rarity"] = rarity_pair.first;
				break;
			}
		}
	}
	void rifle_description_t::generate_map() {
		std::string rifm = "rifle_accuracy_map_",
		            dam = "rifle_damage_map_";
		for(unsigned i = 0; i < MAX_ROOM_DISTANCE; i++) {
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
		this->exported["rifle_incendiary_damage"] = std::to_string(incendiary_damage);
		this->exported["rifle_explosive_damage"] = std::to_string(explosive_damage);
		this->exported["rifle_shrapnel_damage"] = std::to_string(shrapnel_damage);
		this->exported["rifle_corrosive_damage"] = std::to_string(corrosive_damage);
		this->exported["rifle_cryogenic_damage"] = std::to_string(cryogenic_damage);
		this->exported["rifle_radioactive_damage"] = std::to_string(radioactive_damage);
		this->exported["rifle_emp_damage"] = std::to_string(emp_damage);
		this->exported["rifle_shock_damage"] = std::to_string(shock_damage);
		this->exported["rifle_anti_matter_damage"] = std::to_string(anti_matter_damage);
		this->exported["rifle_stat_strength"] = std::to_string(stat_strength);
		this->exported["rifle_stat_intelligence"] = std::to_string(stat_intelligence);
		this->exported["rifle_stat_wisdom"] = std::to_string(stat_wisdom);
		this->exported["rifle_stat_dexterity"] = std::to_string(stat_dexterity);
		this->exported["rifle_stat_constitution"] = std::to_string(stat_constitution);
		this->exported["rifle_stat_electronics"] = std::to_string(stat_electronics);
		this->exported["rifle_stat_armor"] = std::to_string(stat_armor);
		this->exported["rifle_stat_marksmanship"] = std::to_string(stat_marksmanship);
		this->exported["rifle_stat_sniping"] = std::to_string(stat_sniping);
		this->exported["rifle_stat_demolitions"] = std::to_string(stat_demolitions);
		this->exported["rifle_stat_chemistry"] = std::to_string(stat_chemistry);
		this->exported["rifle_stat_weapon_handling"] = std::to_string(stat_weapon_handling);
		this->exported["rifle_stat_strategy"] = std::to_string(stat_strategy);
		this->exported["rifle_stat_medical"] = std::to_string(stat_medical);
		for(auto& rarity_pair : rarity_strings()) {
			if(rarity == rarity_pair.second) {
				this->exported["rifle_rarity"] = rarity_pair.first;
				break;
			}
		}
	}
	uint64_t vehicle_description_t::flush_to_db() {
		VEHICLE_STUB(__LINE__);
		return 0;
#if 0
		try {
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("melee_instance",&insert_transaction);
			auto up_sql = comp
			              .insert()
			              .into("melee_instance")
			              .values(this->exported)
			              .returning("melee_id")
			              .sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()) {
				return this->id = row[0]["melee_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_melee record: '",e.what());
			return 0;
		}
#endif
	}

	uint64_t melee_description_t::flush_to_db() {
		MELEE_STUB(__LINE__);
		return 0;
#if 0
		try {
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("melee_instance",&insert_transaction);
			auto up_sql = comp
			              .insert()
			              .into("melee_instance")
			              .values(this->exported)
			              .returning("melee_id")
			              .sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()) {
				return this->id = row[0]["melee_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_melee record: '",e.what());
			return 0;
		}
#endif
	}

	uint64_t rifle_description_t::flush_to_db() {
		try {
			this->generate_map();
			auto insert_transaction = txn();
			sql_compositor comp("rifle_instance",&insert_transaction);
			auto up_sql = comp
			              .insert()
			              .into("rifle_instance")
			              .values(this->exported)
			              .returning("rifle_id")
			              .sql();
			auto row = mods::pq::exec(insert_transaction,up_sql);
			mods::pq::commit(insert_transaction);
			if(row.size()) {
				return this->id = row[0]["rifle_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_rifle record: '",e.what());
			return 0;
		}
	}


	/** explosives */
	/** explosives */
	/** explosives */
	void explosive_description_t::fill_flags(obj_data* o) {
		auto * w = &(o->obj_flags.wear_flags);
		auto * tf = &(o->obj_flags.type_flag);
		(*tf) = ITEM_EXPLOSIVE;
		(*w) |= ITEM_WEAR_TAKE | ITEM_WEAR_HOLD;
		o->explosive()->type = (mw_explosive)this->type;
		switch((mw_explosive)this->type) {
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
	int16_t vehicle_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(vehicle);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_VEHICLE
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			return -2;
		}
		return 0;
	}

	int16_t melee_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(melee);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_MELEE
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			return -2;
		}
		return 0;
	}
	int16_t explosive_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(explosive);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_EXPLOSIVE
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			return -2;
		}
		return 0;
	}
	int16_t explosive_description_t::write_example_file(std::string_view file) {
		this->generate_map();
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
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
	void explosive_description_t::generate_map() {
		MENTOC_GENERATE_MAP(MENTOC_EXPLOSIVE_MEMBERS_TUPLE,"explosive");
	}
	uint64_t explosive_description_t::flush_to_db() {
		try {
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
			if(row.size()) {
				return this->id = row[0]["explosive_id"].as<uint64_t>();
			}
			return 0;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE(": error inserting new object_explosive record: '",e.what());
			return 0;
		}
	}
	/** explosives */
	/** explosives */
	/** explosives */

	int16_t consumable_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_CONSUMABLE
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_CONSUMABLE_MEMBERS_TUPLE)
		base_items(&out_file, "N7RCX Human Growth Hormone ","PED");
		out_file.flush();
		out_file.close();
		return 0;
	};

	int16_t trap_description_t::write_example_file(std::string_view file) {
		std::string file_name = current_working_dir() + "/" + file.data();
		std::ofstream out_file(file_name);
		if(!out_file.is_open()) {
			return -1;
		}
		if(!out_file.good()) {
			return -2;
		}
		MENTOC_EXAMPLE_TRAP
		MENTOC_MEMBER_VARS_EXAMPLE_FOR(MENTOC_TRAP_MEMBERS_TUPLE)
		base_items(&out_file, "Bear Trap ","BEAR_TRAP");
		out_file.flush();
		out_file.close();
		return 0;
	};


	/*******************************************/
	/** HOWTO: Add new item and subcategories  */
	/* Step 7: Define the feed function:       */
	/*******************************************/
	int16_t gadget_description_t::feed(std::string_view in_file) {
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
		} catch(YAML::Exception& e) {
			this->feed_status = -2;
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			return -2;
		}
	}


	int16_t rifle_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(rifle);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			auto yaml_file = YAML::LoadFile(file);
			auto acmap = yaml_file["accuracy_map"].as<std::vector<float>>();
			fed_items.push_back("accuracy_map");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE; i++) {
				accuracy_map[i] = acmap[i];
				fed_items.push_back(std::string("accuracy_map[") + std::to_string(i) + "]");
			}
			auto dmap = yaml_file["damage_map"].as<std::vector<float>>();
			fed_items.push_back("damage_map");
			for(unsigned i=0; i < MAX_ROOM_DISTANCE; i++) {
				damage_map[i] = dmap[i];
				fed_items.push_back(std::string("damage_map[") + std::to_string(i) + "]");
			}
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_RIFLE
			this->base_stat_list = mods::weapon::weapon_stats(this->type);
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}

	int16_t vehicle_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file) {
		try {
			std::vector<std::string> fed_items;
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_VEHICLE
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("vehicle error",e.what());
			return -1;
		}
		return 0;
	}

	int16_t melee_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file) {
		MELEE_STUB(__LINE__);
		return 0;
	}
	int16_t rifle_description_t::feed_from_po_record(mentoc_pqxx_result_t yaml_file) {
		exit(200);
		try {
			std::vector<std::string> fed_items;
			for(unsigned i=0; i < MAX_ROOM_DISTANCE; i++) {
				float item = yaml_file[std::string("accuracy_map_") + std::to_string(i)].as<float>();
				accuracy_map[i] = item;
			}
			for(unsigned i=0; i < MAX_ROOM_DISTANCE; i++) {
				float item = yaml_file[std::string("damage_map_") + std::to_string(i)].as<float>();
				damage_map[i] = item;
			}
			auto type_string = yaml_file["str_type"].as<std::string>();
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_RIFLE
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("rifle error",e.what());
			return -1;
		}
		return 0;
	}


	int16_t drone_description_t::feed(std::string_view in_file) {
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
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}
	int16_t attachment_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(attachment);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			auto type_string = yaml_file["str_type"].as<std::string>();
			fed_items.push_back("str_type");
			MENTOC_FEED_ATTACHMENT
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			REPORT_DB_ISSUE("error",e.what());
			mods::object_utils::report_yaml_exception(e,fed_items);
			this->feed_status = -2;
			return -2;
		}
	}
	int16_t armor_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(armor);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			auto type_string = yaml_file["str_type"].as<std::string>();
			fed_items.push_back("str_type");
			MENTOC_FEED_BASE_MEMBERS
			MENTOC_FEED_ARMOR
			this->durability_profile_enum = mods::yaml::to_durability_profile(yaml_file["durability_profile"].as<std::string>());
			this->durability_profile = mods::yaml::to_string_from_durability_profile(this->durability_profile_enum);
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}

	int16_t consumable_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(consumable);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			auto type_string = yaml_file["str_type"].as<std::string>();
			fed_items.push_back("str_type");
			MENTOC_FEED_CONSUMABLE
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}

	int16_t trap_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(trap);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			auto type_string = yaml_file["str_type"].as<std::string>();
			fed_items.push_back("str_type");
			MENTOC_FEED_TRAP
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}
	int16_t container_description_t::feed(std::string_view in_file) {
		MENTOC_FILE_EXISTS_PREAMBLE(container);
		std::vector<std::string> fed_items;
		try {
			feed_file = file;
			YAML::Node yaml_file = YAML::LoadFile(std::string(file.data()));
			auto type_string = yaml_file["str_type"].as<std::string>();
			fed_items.push_back("str_type");
			MENTOC_FEED_CONTAINER
			MENTOC_FEED_BASE_MEMBERS
			this->feed_status = 0;
			return 0;
		} catch(YAML::Exception& e) {
			mods::object_utils::report_yaml_exception(e,fed_items);
			REPORT_DB_ISSUE("error",e.what());
			this->feed_status = -2;
			return -2;
		}
	}


};
