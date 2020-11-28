#include "forge.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a forge player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t forge::initialize_row(player_ptr_t &player) {
		init();
		forge_player_id = player->db_id();
		auto status = this->create<forge>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = forge_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t forge::export_class() {
		strmap_t values;
		values["forge_player_id"] = std::to_string(forge_player_id);
		values["forge_craft_shotgun_underbarrel_level"] = std::to_string(forge_craft_shotgun_underbarrel_level);
		values["forge_craft_incendiary_underbarrel_level"] = std::to_string(forge_craft_incendiary_underbarrel_level);
		values["forge_craft_flash_underbarrel_level"] = std::to_string(forge_craft_flash_underbarrel_level);
		values["forge_craft_silencer_level"] = std::to_string(forge_craft_silencer_level);
		values["forge_craft_ammunition_level"] = std::to_string(forge_craft_ammunition_level);
		values["forge_craft_armor_plating_level"] = std::to_string(forge_craft_armor_plating_level);
		values["forge_craft_high_velocity_ammo_level"] = std::to_string(forge_craft_high_velocity_ammo_level);
		values["forge_dismantle_weapon_level"] = std::to_string(forge_dismantle_weapon_level);
		values["forge_trade_attribute_level"] = std::to_string(forge_trade_attribute_level);
		values["forge_repair_armor_level"] = std::to_string(forge_repair_armor_level);
		values["forge_repair_weapon_level"] = std::to_string(forge_repair_weapon_level);
		values["forge_reinforce_weapon_level"] = std::to_string(forge_reinforce_weapon_level);

		return std::move(values);
	}
	int16_t forge::load_by_player(uint64_t player_id){
		this->init();
		return std::get<0>(this->read<forge>(this,"forge_player_id",std::to_string(forge_player_id)));
	}
	int16_t forge::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["forge_id"].as<uint64_t>();
		forge_id = row["forge_id"].as<uint64_t>();
		forge_player_id = row["forge_player_id"].as<uint64_t>();
		forge_craft_shotgun_underbarrel_level = (decltype(forge_craft_shotgun_underbarrel_level))row["forge_craft_shotgun_underbarrel_level_shotgun_underbarrel_level"].as<int>();
		forge_craft_shotgun_underbarrel_level = (decltype(forge_craft_shotgun_underbarrel_level))row["forge_craft_shotgun_underbarrel_level"].as<int>();
		forge_craft_incendiary_underbarrel_level = (decltype(forge_craft_incendiary_underbarrel_level))row["forge_craft_incendiary_underbarrel_level"].as<int>();
		forge_craft_flash_underbarrel_level = (decltype(forge_craft_flash_underbarrel_level))row["forge_craft_flash_underbarrel_level"].as<int>();
		forge_craft_silencer_level = (decltype(forge_craft_silencer_level))row["forge_craft_silencer_level"].as<int>();
		forge_craft_ammunition_level = (decltype(forge_craft_ammunition_level))row["forge_craft_ammunition_level"].as<int>();
		forge_craft_armor_plating_level = (decltype(forge_craft_armor_plating_level))row["forge_craft_armor_plating_level"].as<int>();
		forge_craft_high_velocity_ammo_level = (decltype(forge_craft_high_velocity_ammo_level))row["forge_craft_high_velocity_ammo_level"].as<int>();
		forge_dismantle_weapon_level = (decltype(forge_dismantle_weapon_level))row["forge_dismantle_weapon_level"].as<int>();
		forge_trade_attribute_level = (decltype(forge_trade_attribute_level))row["forge_trade_attribute_level"].as<int>();
		forge_repair_armor_level = (decltype(forge_repair_armor_level))row["forge_repair_armor_level"].as<int>();
		forge_repair_weapon_level = (decltype(forge_repair_weapon_level))row["forge_repair_weapon_level"].as<int>();
		forge_reinforce_weapon_level = (decltype(forge_reinforce_weapon_level))row["forge_reinforce_weapon_level"].as<int>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		return 0;
	}
	void forge::init(){
		id = 0;
		forge_id = 0;
		forge_player_id = 0;
		loaded = 0;
		forge_craft_shotgun_underbarrel_level = (decltype(forge_craft_shotgun_underbarrel_level))0;
		forge_craft_incendiary_underbarrel_level = (decltype(forge_craft_incendiary_underbarrel_level))0;
		forge_craft_flash_underbarrel_level = (decltype(forge_craft_flash_underbarrel_level))0;
		forge_craft_silencer_level = (decltype(forge_craft_silencer_level))0;
		forge_craft_ammunition_level = (decltype(forge_craft_ammunition_level))0;
		forge_craft_armor_plating_level = (decltype(forge_craft_armor_plating_level))0;
		forge_craft_high_velocity_ammo_level = (decltype(forge_craft_high_velocity_ammo_level))0;
		forge_dismantle_weapon_level = (decltype(forge_dismantle_weapon_level))0;
		forge_trade_attribute_level = (decltype(forge_trade_attribute_level))0;
		forge_repair_armor_level = (decltype(forge_repair_armor_level))0;
		forge_repair_weapon_level = (decltype(forge_repair_weapon_level))0;
		forge_reinforce_weapon_level = (decltype(forge_reinforce_weapon_level))0;
		created_at = (decltype(created_at))0;
		updated_at = (decltype(updated_at))0;
	}

};

