#include "rifle-instance.hpp"

#include <sstream>

template <typename T>
static inline std::string to_string_with_precision(const T a_value, const int n = 6) {
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}
namespace mods::yaml {
	extern float parse_rarity(const std::string& rarity_string);
};
namespace mods::orm {
	rifle_instance::rifle_instance(obj_ptr_t& obj) {
		this->init();
		this->initialize_row(obj);
	}
	void rifle_instance::import_object(obj_ptr_t& obj) {
		this->rifle_id = obj->rifle()->attributes->id;
		this->rifle_accuracy_map_0 = obj->rifle()->attributes->accuracy_map[0];
		this->rifle_accuracy_map_1 = obj->rifle()->attributes->accuracy_map[1];
		this->rifle_accuracy_map_2 = obj->rifle()->attributes->accuracy_map[2];
		this->rifle_accuracy_map_3 = obj->rifle()->attributes->accuracy_map[3];
		this->rifle_damage_map_0 = obj->rifle()->attributes->damage_map[0];
		this->rifle_damage_map_1 = obj->rifle()->attributes->damage_map[1];
		this->rifle_damage_map_2 = obj->rifle()->attributes->damage_map[2];
		this->rifle_damage_map_3 = obj->rifle()->attributes->damage_map[3];
		this->rifle_rarity = obj->rifle()->attributes->rarity;
		if(this->rifle_rarity.length() == 0) {
			this->rifle_rarity = "COMMON";
		}
		this->rifle_file = obj->rifle()->attributes->feed_file;
		if(this->rifle_file.length() == 0) {
			this->rifle_file = "g36c.yml";
		}
		this->rifle_str_type = obj->rifle()->attributes->str_type;
		if(this->rifle_str_type.length() == 0) {
			this->rifle_str_type = "ASSAULT_RIFLE";
		}
		this->rifle_type = std::to_string(obj->rifle()->attributes->type);
		if(this->rifle_type.length() == 0) {
			this->rifle_type = "0";
		}
		this->rifle_manufacturer = obj->rifle()->attributes->manufacturer;
		if(this->rifle_manufacturer.length() == 0) {
			this->rifle_manufacturer = "TN-X";
		}
		this->rifle_name = obj->rifle()->attributes->name;
		if(this->rifle_name.length() == 0) {
			this->rifle_name = "G36C Assault Rifle";
		}
		this->rifle_ammo_type = obj->rifle()->attributes->ammo_type;
		if(this->rifle_ammo_type.length() == 0) {
			this->rifle_ammo_type = "ASSAULT_RIFLE";
		}
		this->rifle_vnum = obj->rifle()->attributes->vnum;
		this->rifle_ammo_max = obj->rifle()->attributes->ammo_max;
		this->rifle_chance_to_injure = obj->rifle()->attributes->chance_to_injure;
		this->rifle_clip_size = obj->rifle()->attributes->clip_size;
		this->rifle_cooldown_between_shots = obj->rifle()->attributes->cooldown_between_shots;
		this->rifle_critical_chance = obj->rifle()->attributes->critical_chance;
		this->rifle_critical_range = obj->rifle()->attributes->critical_range;
		this->rifle_base_damage = obj->rifle()->attributes->base_damage;
		this->rifle_disorient_amount = obj->rifle()->attributes->disorient_amount;
		this->rifle_headshot_bonus = obj->rifle()->attributes->headshot_bonus;
		this->rifle_max_range = obj->rifle()->attributes->max_range;
		this->rifle_range_multiplier = obj->rifle()->attributes->range_multiplier;
		this->rifle_reload_time = obj->rifle()->attributes->reload_time;
		this->rifle_rounds_per_minute = obj->rifle()->attributes->rounds_per_minute;
		this->rifle_muzzle_velocity = obj->rifle()->attributes->muzzle_velocity;
		this->rifle_effective_firing_range = obj->rifle()->attributes->effective_firing_range;
		this->rifle_damage_dice_count = obj->rifle()->attributes->damage_dice_count;
		this->rifle_damage_dice_sides = obj->rifle()->attributes->damage_dice_sides;
		this->rifle_incendiary_damage = obj->rifle()->attributes->incendiary_damage;
		this->rifle_explosive_damage = obj->rifle()->attributes->explosive_damage;
		this->rifle_shrapnel_damage = obj->rifle()->attributes->shrapnel_damage;
		this->rifle_corrosive_damage = obj->rifle()->attributes->corrosive_damage;
		this->rifle_cryogenic_damage = obj->rifle()->attributes->cryogenic_damage;
		this->rifle_radioactive_damage = obj->rifle()->attributes->radioactive_damage;
		this->rifle_emp_damage = obj->rifle()->attributes->emp_damage;
		this->rifle_shock_damage = obj->rifle()->attributes->shock_damage;
		this->rifle_anti_matter_damage = obj->rifle()->attributes->anti_matter_damage;
		this->rifle_stat_strength = obj->rifle()->attributes->stat_strength;
		this->rifle_stat_intelligence = obj->rifle()->attributes->stat_intelligence;
		this->rifle_stat_wisdom = obj->rifle()->attributes->stat_wisdom;
		this->rifle_stat_dexterity = obj->rifle()->attributes->stat_dexterity;
		this->rifle_stat_constitution = obj->rifle()->attributes->stat_constitution;
		this->rifle_stat_electronics = obj->rifle()->attributes->stat_electronics;
		this->rifle_stat_armor = obj->rifle()->attributes->stat_armor;
		this->rifle_stat_marksmanship = obj->rifle()->attributes->stat_marksmanship;
		this->rifle_stat_sniping = obj->rifle()->attributes->stat_sniping;
		this->rifle_stat_demolitions = obj->rifle()->attributes->stat_demolitions;
		this->rifle_stat_chemistry = obj->rifle()->attributes->stat_chemistry;
		this->rifle_stat_weapon_handling = obj->rifle()->attributes->stat_weapon_handling;
		this->rifle_stat_strategy = obj->rifle()->attributes->stat_strategy;
		this->rifle_stat_medical = obj->rifle()->attributes->stat_medical;
	}
	void rifle_instance::populate_object(obj_ptr_t& obj) {
		obj->feed(ITEM_RIFLE, this->rifle_file);
		obj->set_db_id(this->rifle_id);
		obj->rifle()->attributes->id = this->rifle_id;
		obj->rifle()->attributes->accuracy_map[0] = this->rifle_accuracy_map_0;
		obj->rifle()->attributes->accuracy_map[1] = this->rifle_accuracy_map_1;
		obj->rifle()->attributes->accuracy_map[2] = this->rifle_accuracy_map_2;
		obj->rifle()->attributes->accuracy_map[3] = this->rifle_accuracy_map_3;
		obj->rifle()->attributes->damage_map[0] = this->rifle_damage_map_0;
		obj->rifle()->attributes->damage_map[1] = this->rifle_damage_map_1;
		obj->rifle()->attributes->damage_map[2] = this->rifle_damage_map_2;
		obj->rifle()->attributes->damage_map[3] = this->rifle_damage_map_3;
		obj->rifle()->attributes->rarity = mods::yaml::parse_rarity(this->rifle_rarity);
		obj->rifle()->attributes->str_type = this->rifle_str_type;
		obj->rifle()->attributes->type = mods::weapon::from_string_to_rifle(this->rifle_str_type);
		obj->rifle()->attributes->manufacturer = this->rifle_manufacturer;
		obj->rifle()->attributes->name = this->rifle_name;
		obj->rifle()->attributes->ammo_type = this->rifle_ammo_type;
		obj->rifle()->attributes->vnum = this->rifle_vnum;
		obj->rifle()->attributes->ammo_max = this->rifle_ammo_max;
		obj->rifle()->attributes->chance_to_injure = this->rifle_chance_to_injure;
		obj->rifle()->attributes->clip_size = this->rifle_clip_size;
		obj->rifle()->attributes->cooldown_between_shots = this->rifle_cooldown_between_shots;
		obj->rifle()->attributes->critical_chance = this->rifle_critical_chance;
		obj->rifle()->attributes->critical_range = this->rifle_critical_range;
		obj->rifle()->attributes->base_damage = this->rifle_base_damage;
		obj->rifle()->attributes->disorient_amount = this->rifle_disorient_amount;
		obj->rifle()->attributes->headshot_bonus = this->rifle_headshot_bonus;
		obj->rifle()->attributes->max_range = this->rifle_max_range;
		obj->rifle()->attributes->range_multiplier = this->rifle_range_multiplier;
		obj->rifle()->attributes->reload_time = this->rifle_reload_time;
		obj->rifle()->attributes->rounds_per_minute = this->rifle_rounds_per_minute;
		obj->rifle()->attributes->muzzle_velocity = this->rifle_muzzle_velocity;
		obj->rifle()->attributes->effective_firing_range = this->rifle_effective_firing_range;
		obj->rifle()->attributes->damage_dice_count = this->rifle_damage_dice_count;
		obj->rifle()->attributes->damage_dice_sides = this->rifle_damage_dice_sides;
		obj->rifle()->attributes->incendiary_damage = this->rifle_incendiary_damage;
		obj->rifle()->attributes->explosive_damage = this->rifle_explosive_damage;
		obj->rifle()->attributes->shrapnel_damage = this->rifle_shrapnel_damage;
		obj->rifle()->attributes->corrosive_damage = this->rifle_corrosive_damage;
		obj->rifle()->attributes->cryogenic_damage = this->rifle_cryogenic_damage;
		obj->rifle()->attributes->radioactive_damage = this->rifle_radioactive_damage;
		obj->rifle()->attributes->emp_damage = this->rifle_emp_damage;
		obj->rifle()->attributes->shock_damage = this->rifle_shock_damage;
		obj->rifle()->attributes->anti_matter_damage = this->rifle_anti_matter_damage;
		obj->rifle()->attributes->stat_strength = this->rifle_stat_strength;
		obj->rifle()->attributes->stat_intelligence = this->rifle_stat_intelligence;
		obj->rifle()->attributes->stat_wisdom = this->rifle_stat_wisdom;
		obj->rifle()->attributes->stat_dexterity = this->rifle_stat_dexterity;
		obj->rifle()->attributes->stat_constitution = this->rifle_stat_constitution;
		obj->rifle()->attributes->stat_electronics = this->rifle_stat_electronics;
		obj->rifle()->attributes->stat_armor = this->rifle_stat_armor;
		obj->rifle()->attributes->stat_marksmanship = this->rifle_stat_marksmanship;
		obj->rifle()->attributes->stat_sniping = this->rifle_stat_sniping;
		obj->rifle()->attributes->stat_demolitions = this->rifle_stat_demolitions;
		obj->rifle()->attributes->stat_chemistry = this->rifle_stat_chemistry;
		obj->rifle()->attributes->stat_weapon_handling = this->rifle_stat_weapon_handling;
		obj->rifle()->attributes->stat_strategy = this->rifle_stat_strategy;
		obj->rifle()->attributes->stat_medical = this->rifle_stat_medical;
		obj->rifle()->attributes->fill_flags(obj.get());
	}
	uint64_t rifle_instance::initialize_row(obj_ptr_t& obj) {
		init();
		this->import_object(obj);
		auto status = this->create<rifle_instance>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			rifle_id = std::get<2>(status);
			obj->set_db_id(this->rifle_id);
		}
		return rifle_id;
	}
	strmap_t rifle_instance::export_class() {
		strmap_t values;
		values["rifle_accuracy_map_0"] = std::to_string(this->rifle_accuracy_map_0);
		values["rifle_accuracy_map_1"] = std::to_string(this->rifle_accuracy_map_1);
		values["rifle_accuracy_map_2"] = std::to_string(this->rifle_accuracy_map_2);
		values["rifle_accuracy_map_3"] = std::to_string(this->rifle_accuracy_map_3);
		values["rifle_damage_map_0"] = std::to_string(this->rifle_damage_map_0);
		values["rifle_damage_map_1"] = std::to_string(this->rifle_damage_map_1);
		values["rifle_damage_map_2"] = std::to_string(this->rifle_damage_map_2);
		values["rifle_damage_map_3"] = std::to_string(this->rifle_damage_map_3);

		values["rifle_rarity"] = (this->rifle_rarity);
		if(values["rifle_rarity"].length() == 0) {
			values["rifle_rarity"] = "COMMON";
		}
		values["rifle_rarity"] = "COMMON";
		values["rifle_file"] = (this->rifle_file);
		if(values["rifle_file"].length() == 0) {
			values["rifle_file"] = "g36c.yml";
		}
		values["rifle_str_type"] = (this->rifle_str_type);
		if(values["rifle_str_type"].length() == 0) {
			values["rifle_str_type"] = "ASSAULT_RIFLE";
		}
		values["rifle_type"] = this->rifle_type;
		if(values["rifle_type"].length() == 0) {
			values["rifle_type"] = "0";
		}
		values["rifle_manufacturer"] = (this->rifle_manufacturer);
		if(values["rifle_manufacturer"].length() == 0) {
			values["rifle_manufacturer"] = "TNX";
		}
		values["rifle_name"] = (this->rifle_name);
		if(values["rifle_name"].length() == 0) {
			values["rifle_name"] = "G36C Assault Rifle";
		}
		values["rifle_ammo_type"] = (this->rifle_ammo_type);
		if(values["rifle_ammo_type"].length() == 0) {
			values["rifle_ammo_type"] = "ASSAULT_RIFLE";
		}

		values["rifle_vnum"] = std::to_string(this->rifle_vnum);
		values["rifle_ammo_max"] = std::to_string(this->rifle_ammo_max);
		values["rifle_chance_to_injure"] = to_string_with_precision(this->rifle_chance_to_injure,2);
		values["rifle_clip_size"] = std::to_string(this->rifle_clip_size);
		values["rifle_cooldown_between_shots"] = std::to_string(this->rifle_cooldown_between_shots);
		values["rifle_critical_chance"] = std::to_string(this->rifle_critical_chance);
		values["rifle_critical_range"] = std::to_string(this->rifle_critical_range);
		values["rifle_base_damage"] = std::to_string(this->rifle_base_damage);
		values["rifle_disorient_amount"] = to_string_with_precision(this->rifle_disorient_amount,2);
		values["rifle_headshot_bonus"] = std::to_string(this->rifle_headshot_bonus);
		values["rifle_max_range"] = std::to_string(this->rifle_max_range);
		values["rifle_range_multiplier"] = to_string_with_precision(this->rifle_range_multiplier,2);
		values["rifle_reload_time"] = std::to_string(this->rifle_reload_time);
		values["rifle_rounds_per_minute"] = std::to_string(this->rifle_rounds_per_minute);
		values["rifle_muzzle_velocity"] = std::to_string(this->rifle_muzzle_velocity);
		values["rifle_effective_firing_range"] = std::to_string(this->rifle_effective_firing_range);
		values["rifle_damage_dice_count"] = std::to_string(this->rifle_damage_dice_count);
		values["rifle_damage_dice_sides"] = std::to_string(this->rifle_damage_dice_sides);
		values["rifle_incendiary_damage"] = std::to_string(this->rifle_incendiary_damage);
		values["rifle_explosive_damage"] = std::to_string(this->rifle_explosive_damage);
		values["rifle_shrapnel_damage"] = std::to_string(this->rifle_shrapnel_damage);
		values["rifle_corrosive_damage"] = std::to_string(this->rifle_corrosive_damage);
		values["rifle_cryogenic_damage"] = std::to_string(this->rifle_cryogenic_damage);
		values["rifle_radioactive_damage"] = std::to_string(this->rifle_radioactive_damage);
		values["rifle_emp_damage"] = std::to_string(this->rifle_emp_damage);
		values["rifle_shock_damage"] = std::to_string(this->rifle_shock_damage);
		values["rifle_anti_matter_damage"] = std::to_string(this->rifle_anti_matter_damage);
		values["rifle_stat_strength"] = std::to_string(this->rifle_stat_strength);
		values["rifle_stat_intelligence"] = std::to_string(this->rifle_stat_intelligence);
		values["rifle_stat_wisdom"] = std::to_string(this->rifle_stat_wisdom);
		values["rifle_stat_dexterity"] = std::to_string(this->rifle_stat_dexterity);
		values["rifle_stat_constitution"] = std::to_string(this->rifle_stat_constitution);
		values["rifle_stat_electronics"] = std::to_string(this->rifle_stat_electronics);
		values["rifle_stat_armor"] = std::to_string(this->rifle_stat_armor);
		values["rifle_stat_marksmanship"] = std::to_string(this->rifle_stat_marksmanship);
		values["rifle_stat_sniping"] = std::to_string(this->rifle_stat_sniping);
		values["rifle_stat_demolitions"] = std::to_string(this->rifle_stat_demolitions);
		values["rifle_stat_chemistry"] = std::to_string(this->rifle_stat_chemistry);
		values["rifle_stat_weapon_handling"] = std::to_string(this->rifle_stat_weapon_handling);
		values["rifle_stat_strategy"] = std::to_string(this->rifle_stat_strategy);
		values["rifle_stat_medical"] = std::to_string(this->rifle_stat_medical);
		//values["created_at"] = std::to_string(this->created_at);
		//values["updated_at"] = std::to_string(this->updated_at);
		return std::move(values);
	}
	std::string& rifle_instance::get_dump() {
		static std::string dump = "";
		dump = "";
		for(const auto& pair : this->export_class()) {
			dump += CAT(pair.first,": '",pair.second,"'\r\n");
		}
		return dump;
	}
	int16_t rifle_instance::load_by_id(uint64_t in_rifle_id) {
		loaded = 0;
		created_at = updated_at = 0;
		rifle_id = in_rifle_id;
		return std::get<0>(this->read<rifle_instance>(this,"rifle_id",std::to_string(rifle_id)));
	}
	int16_t rifle_instance::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		rifle_id = row["rifle_id"].as<uint64_t>();
		if(!row["rifle_accuracy_map_0"].is_null()) {
			this->rifle_accuracy_map_0 = row["rifle_accuracy_map_0"].as<int16_t>();
		}
		if(!row["rifle_accuracy_map_1"].is_null()) {
			this->rifle_accuracy_map_1 = row["rifle_accuracy_map_1"].as<int16_t>();
		}
		if(!row["rifle_accuracy_map_2"].is_null()) {
			this->rifle_accuracy_map_2 = row["rifle_accuracy_map_2"].as<int16_t>();
		}
		if(!row["rifle_accuracy_map_3"].is_null()) {
			this->rifle_accuracy_map_3 = row["rifle_accuracy_map_3"].as<int16_t>();
		}
		if(!row["rifle_damage_map_0"].is_null()) {
			this->rifle_damage_map_0 = row["rifle_damage_map_0"].as<int16_t>();
		}
		if(!row["rifle_damage_map_1"].is_null()) {
			this->rifle_damage_map_1 = row["rifle_damage_map_1"].as<int16_t>();
		}
		if(!row["rifle_damage_map_2"].is_null()) {
			this->rifle_damage_map_2 = row["rifle_damage_map_2"].as<int16_t>();
		}
		if(!row["rifle_damage_map_3"].is_null()) {
			this->rifle_damage_map_3 = row["rifle_damage_map_3"].as<int16_t>();
		}
		if(!row["rifle_rarity"].is_null()) {
			this->rifle_rarity = row["rifle_rarity"].c_str();
		}
		if(!row["rifle_file"].is_null()) {
			this->rifle_file = row["rifle_file"].c_str();
		}
		if(!row["rifle_str_type"].is_null()) {
			this->rifle_str_type = row["rifle_str_type"].c_str();
		}
		if(!row["rifle_type"].is_null()) {
			this->rifle_type = row["rifle_type"].as<int>();
		}
		if(!row["rifle_manufacturer"].is_null()) {
			this->rifle_manufacturer = row["rifle_manufacturer"].c_str();
		}
		if(!row["rifle_name"].is_null()) {
			this->rifle_name = row["rifle_name"].c_str();
		}
		if(!row["rifle_vnum"].is_null()) {
			this->rifle_vnum = row["rifle_vnum"].as<int16_t>();
		}
		if(!row["rifle_ammo_max"].is_null()) {
			this->rifle_ammo_max = row["rifle_ammo_max"].as<int16_t>();
		}
		if(!row["rifle_ammo_type"].is_null()) {
			this->rifle_ammo_type = row["rifle_ammo_type"].c_str();
		}
		if(!row["rifle_chance_to_injure"].is_null()) {
			this->rifle_chance_to_injure = row["rifle_chance_to_injure"].as<float>();
		}
		if(!row["rifle_clip_size"].is_null()) {
			this->rifle_clip_size = row["rifle_clip_size"].as<int16_t>();
		}
		if(!row["rifle_cooldown_between_shots"].is_null()) {
			this->rifle_cooldown_between_shots = row["rifle_cooldown_between_shots"].as<int16_t>();
		}
		if(!row["rifle_critical_chance"].is_null()) {
			this->rifle_critical_chance = row["rifle_critical_chance"].as<int16_t>();
		}
		if(!row["rifle_critical_range"].is_null()) {
			this->rifle_critical_range = row["rifle_critical_range"].as<int16_t>();
		}
		if(!row["rifle_base_damage"].is_null()) {
			this->rifle_base_damage = row["rifle_base_damage"].as<int>();
		}
		if(!row["rifle_disorient_amount"].is_null()) {
			this->rifle_disorient_amount = row["rifle_disorient_amount"].as<float>();
		}
		if(!row["rifle_headshot_bonus"].is_null()) {
			this->rifle_headshot_bonus = row["rifle_headshot_bonus"].as<float>();
		}
		if(!row["rifle_max_range"].is_null()) {
			this->rifle_max_range = row["rifle_max_range"].as<int16_t>();
		}
		if(!row["rifle_range_multiplier"].is_null()) {
			this->rifle_range_multiplier = row["rifle_range_multiplier"].as<float>();
		}
		if(!row["rifle_reload_time"].is_null()) {
			this->rifle_reload_time = row["rifle_reload_time"].as<int16_t>();
		}
		if(!row["rifle_rounds_per_minute"].is_null()) {
			this->rifle_rounds_per_minute = row["rifle_rounds_per_minute"].as<int16_t>();
		}
		if(!row["rifle_muzzle_velocity"].is_null()) {
			this->rifle_muzzle_velocity = row["rifle_muzzle_velocity"].as<int16_t>();
		}
		if(!row["rifle_effective_firing_range"].is_null()) {
			this->rifle_effective_firing_range = row["rifle_effective_firing_range"].as<int16_t>();
		}
		if(!row["rifle_damage_dice_count"].is_null()) {
			this->rifle_damage_dice_count = row["rifle_damage_dice_count"].as<int16_t>();
		}
		if(!row["rifle_damage_dice_sides"].is_null()) {
			this->rifle_damage_dice_sides = row["rifle_damage_dice_sides"].as<int16_t>();
		}
		if(!row["rifle_incendiary_damage"].is_null()) {
			this->rifle_incendiary_damage = row["rifle_incendiary_damage"].as<int16_t>();
		}
		if(!row["rifle_explosive_damage"].is_null()) {
			this->rifle_explosive_damage = row["rifle_explosive_damage"].as<int16_t>();
		}
		if(!row["rifle_shrapnel_damage"].is_null()) {
			this->rifle_shrapnel_damage = row["rifle_shrapnel_damage"].as<int16_t>();
		}
		if(!row["rifle_corrosive_damage"].is_null()) {
			this->rifle_corrosive_damage = row["rifle_corrosive_damage"].as<int16_t>();
		}
		if(!row["rifle_cryogenic_damage"].is_null()) {
			this->rifle_cryogenic_damage = row["rifle_cryogenic_damage"].as<int16_t>();
		}
		if(!row["rifle_radioactive_damage"].is_null()) {
			this->rifle_radioactive_damage = row["rifle_radioactive_damage"].as<int16_t>();
		}
		if(!row["rifle_emp_damage"].is_null()) {
			this->rifle_emp_damage = row["rifle_emp_damage"].as<int16_t>();
		}
		if(!row["rifle_shock_damage"].is_null()) {
			this->rifle_shock_damage = row["rifle_shock_damage"].as<int16_t>();
		}
		if(!row["rifle_anti_matter_damage"].is_null()) {
			this->rifle_anti_matter_damage = row["rifle_anti_matter_damage"].as<int16_t>();
		}
		if(!row["rifle_stat_strength"].is_null()) {
			this->rifle_stat_strength = row["rifle_stat_strength"].as<int16_t>();
		}
		if(!row["rifle_stat_intelligence"].is_null()) {
			this->rifle_stat_intelligence = row["rifle_stat_intelligence"].as<int16_t>();
		}
		if(!row["rifle_stat_wisdom"].is_null()) {
			this->rifle_stat_wisdom = row["rifle_stat_wisdom"].as<int16_t>();
		}
		if(!row["rifle_stat_dexterity"].is_null()) {
			this->rifle_stat_dexterity = row["rifle_stat_dexterity"].as<int16_t>();
		}
		if(!row["rifle_stat_constitution"].is_null()) {
			this->rifle_stat_constitution = row["rifle_stat_constitution"].as<int16_t>();
		}
		if(!row["rifle_stat_electronics"].is_null()) {
			this->rifle_stat_electronics = row["rifle_stat_electronics"].as<int16_t>();
		}
		if(!row["rifle_stat_armor"].is_null()) {
			this->rifle_stat_armor = row["rifle_stat_armor"].as<int16_t>();
		}
		if(!row["rifle_stat_marksmanship"].is_null()) {
			this->rifle_stat_marksmanship = row["rifle_stat_marksmanship"].as<int16_t>();
		}
		if(!row["rifle_stat_sniping"].is_null()) {
			this->rifle_stat_sniping = row["rifle_stat_sniping"].as<int16_t>();
		}
		if(!row["rifle_stat_demolitions"].is_null()) {
			this->rifle_stat_demolitions = row["rifle_stat_demolitions"].as<int16_t>();
		}
		if(!row["rifle_stat_chemistry"].is_null()) {
			this->rifle_stat_chemistry = row["rifle_stat_chemistry"].as<int16_t>();
		}
		if(!row["rifle_stat_weapon_handling"].is_null()) {
			this->rifle_stat_weapon_handling = row["rifle_stat_weapon_handling"].as<int16_t>();
		}
		if(!row["rifle_stat_strategy"].is_null()) {
			this->rifle_stat_strategy = row["rifle_stat_strategy"].as<int16_t>();
		}
		if(!row["rifle_stat_medical"].is_null()) {
			this->rifle_stat_medical = row["rifle_stat_medical"].as<int16_t>();
		}
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void rifle_instance::init() {
		this->rifle_id = 0;
		this->rifle_accuracy_map_0 = 0;
		this->rifle_accuracy_map_1 = 0;
		this->rifle_accuracy_map_2 = 0;
		this->rifle_accuracy_map_3 = 0;
		this->rifle_damage_map_0 = 0;
		this->rifle_damage_map_1 = 0;
		this->rifle_damage_map_2 = 0;
		this->rifle_damage_map_3 = 0;
		this->rifle_rarity = "COMMON";
		this->rifle_file = "";
		this->rifle_str_type = "";
		this->rifle_type = "";
		this->rifle_manufacturer = "";
		this->rifle_name = "";
		this->rifle_vnum = 0;
		this->rifle_ammo_max = 0;
		this->rifle_ammo_type = "";
		this->rifle_chance_to_injure = 0;
		this->rifle_clip_size = 0;
		this->rifle_cooldown_between_shots = 0;
		this->rifle_critical_chance = 0;
		this->rifle_critical_range = 0;
		this->rifle_base_damage = 0;
		this->rifle_disorient_amount = 0;
		this->rifle_headshot_bonus = 0;
		this->rifle_max_range = 0;
		this->rifle_range_multiplier = 0;
		this->rifle_reload_time = 0;
		this->rifle_rounds_per_minute = 0;
		this->rifle_muzzle_velocity = 0;
		this->rifle_effective_firing_range = 0;
		this->rifle_damage_dice_count = 0;
		this->rifle_damage_dice_sides = 0;
		this->rifle_incendiary_damage = 0;
		this->rifle_explosive_damage = 0;
		this->rifle_shrapnel_damage = 0;
		this->rifle_corrosive_damage = 0;
		this->rifle_cryogenic_damage = 0;
		this->rifle_radioactive_damage = 0;
		this->rifle_emp_damage = 0;
		this->rifle_shock_damage = 0;
		this->rifle_anti_matter_damage = 0;
		this->rifle_stat_strength = 0;
		this->rifle_stat_intelligence = 0;
		this->rifle_stat_wisdom = 0;
		this->rifle_stat_dexterity = 0;
		this->rifle_stat_constitution = 0;
		this->rifle_stat_electronics = 0;
		this->rifle_stat_armor = 0;
		this->rifle_stat_marksmanship = 0;
		this->rifle_stat_sniping = 0;
		this->rifle_stat_demolitions = 0;
		this->rifle_stat_chemistry = 0;
		this->rifle_stat_weapon_handling = 0;
		this->rifle_stat_strategy = 0;
		this->rifle_stat_medical = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

