#include "ghost.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a ghost player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t ghost::initialize_row(player_ptr_t &player) {
		init();
		ghost_player_id = player->db_id();
		auto status = this->create<ghost>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = ghost_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t ghost::export_class() {
		strmap_t values;
		values["ghost_player_id"] = std::to_string(ghost_player_id);
		values["ghost_drone_scan_level"] = std::to_string(ghost_drone_scan_level);
		values["ghost_stealth_level"] = std::to_string(ghost_stealth_level);
		values["ghost_xray_shot_level"] = std::to_string(ghost_xray_shot_level);
		values["ghost_feign_death_level"] = std::to_string(ghost_feign_death_level);
		values["ghost_summon_extraction_level"] = std::to_string(ghost_summon_extraction_level);
		values["ghost_plant_claymore_level"] = std::to_string(ghost_plant_claymore_level);
		values["ghost_penetrating_shot_level"] = std::to_string(ghost_penetrating_shot_level);
		values["ghost_intimidation_level"] = std::to_string(ghost_intimidation_level);
		values["ghost_cryogenic_grenade_level"] = std::to_string(ghost_cryogenic_grenade_level);
		values["ghost_flash_underbarrel_level"] = std::to_string(ghost_flash_underbarrel_level);
		return std::move(values);
	}
	int16_t ghost::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = ghost_id = 0;
		ghost_player_id = 0;
		ghost_drone_scan_level = (decltype(ghost_drone_scan_level))0;
		ghost_stealth_level = (decltype(ghost_stealth_level))0;
		ghost_xray_shot_level = (decltype(ghost_xray_shot_level))0;
		ghost_feign_death_level = (decltype(ghost_feign_death_level))0;
		ghost_summon_extraction_level = (decltype(ghost_summon_extraction_level))0;
		ghost_plant_claymore_level = (decltype(ghost_plant_claymore_level))0;
		ghost_penetrating_shot_level = (decltype(ghost_penetrating_shot_level))0;
		ghost_intimidation_level = (decltype(ghost_intimidation_level))0;
		ghost_cryogenic_grenade_level = (decltype(ghost_cryogenic_grenade_level))0;
		ghost_flash_underbarrel_level = (decltype(ghost_flash_underbarrel_level))0;
		created_at = (decltype(created_at))0;
		return std::get<0>(this->read<ghost>(this,"ghost_player_id",std::to_string(ghost_player_id)));
	}
	int16_t ghost::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["ghost_id"].as<uint64_t>();
		ghost_id = row["ghost_id"].as<uint64_t>();
		ghost_player_id = row["ghost_player_id"].as<uint64_t>();
		ghost_drone_scan_level = (decltype(ghost_drone_scan_level))row["ghost_drone_scan_level"].as<int>();
		ghost_stealth_level = (decltype(ghost_stealth_level))row["ghost_stealth_level"].as<int>();
		ghost_xray_shot_level = (decltype(ghost_xray_shot_level))row["ghost_xray_shot_level"].as<int>();
		ghost_feign_death_level = (decltype(ghost_feign_death_level))row["ghost_feign_death_level"].as<int>();
		ghost_summon_extraction_level = (decltype(ghost_summon_extraction_level))row["ghost_summon_extraction_level"].as<int>();
		ghost_plant_claymore_level = (decltype(ghost_plant_claymore_level))row["ghost_plant_claymore_level"].as<int>();
		ghost_penetrating_shot_level = (decltype(ghost_penetrating_shot_level))row["ghost_penetrating_shot_level"].as<int>();
		ghost_intimidation_level = (decltype(ghost_intimidation_level))row["ghost_intimidation_level"].as<int>();
		ghost_cryogenic_grenade_level = (decltype(ghost_cryogenic_grenade_level))row["ghost_cryogenic_grenade_level"].as<int>();
		ghost_flash_underbarrel_level = (decltype(ghost_flash_underbarrel_level))row["ghost_flash_underbarrel_level"].as<int>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		return 0;
	}
	void ghost::init(){
		id = 0;
		ghost_id = 0;
		ghost_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
		ghost_drone_scan_level = (decltype(ghost_drone_scan_level))0;
		ghost_stealth_level = (decltype(ghost_stealth_level))0;
		ghost_xray_shot_level = (decltype(ghost_xray_shot_level))0;
		ghost_feign_death_level = (decltype(ghost_feign_death_level))0;
		ghost_summon_extraction_level = (decltype(ghost_summon_extraction_level))0;
		ghost_plant_claymore_level = (decltype(ghost_plant_claymore_level))0;
		ghost_penetrating_shot_level = (decltype(ghost_penetrating_shot_level))0;
		ghost_intimidation_level = (decltype(ghost_intimidation_level))0;
		ghost_cryogenic_grenade_level = (decltype(ghost_cryogenic_grenade_level))0;
		ghost_flash_underbarrel_level = (decltype(ghost_flash_underbarrel_level))0;
	}

};

