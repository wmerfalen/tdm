#include "class-engineer.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a engineer player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t engineer::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		engineer_secondary_type = CZP10;
		if(primary_choice == primary_choice_t::FMG9){
			auto fmg9 = create_object(ITEM_RIFLE,"fmg9.yml");
			engineer_primary_weapon_id = fmg9->rifle()->attributes->flush_to_db();
			engineer_primary_type = FMG9;
		}
		if(primary_choice == primary_choice_t::P90){
			auto p90 = create_object(ITEM_RIFLE,"p90.yml");
			engineer_primary_weapon_id = p90->rifle()->attributes->flush_to_db();
			engineer_primary_type = P90;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");
		engineer_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		engineer_player_id = player->db_id();
		auto status = this->create<engineer>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = engineer_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t engineer::export_class() {
		strmap_t values;
		values["engineer_player_id"] = std::to_string(engineer_player_id);
		values["engineer_primary_type"] = engineer_primary_type;
		values["engineer_primary_weapon_id"] = std::to_string( engineer_primary_weapon_id);
		values["engineer_secondary_type"] = CZP10;
		values["engineer_secondary_weapon_id"] = std::to_string(engineer_secondary_weapon_id);
		return std::move(values);
	}
	int16_t engineer::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = engineer_id = 0;
		engineer_player_id = 0;
		engineer_secondary_type = engineer_primary_type ="NONE";
		engineer_primary_weapon_id = engineer_secondary_weapon_id = 0;
		return std::get<0>(this->read<engineer>(this,"engineer_player_id",std::to_string(engineer_player_id)));
	}
	int16_t engineer::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["engineer_id"].as<uint64_t>();
		engineer_id = row["engineer_id"].as<uint64_t>();
		engineer_player_id = row["engineer_player_id"].as<uint64_t>();
		engineer_primary_type = row["engineer_primary_type"].c_str();
		engineer_primary_weapon_id = row["engineer_primary_weapon_id"].as<uint64_t>();
		engineer_secondary_type = row["engineer_secondary_type"].c_str();
		engineer_secondary_weapon_id = row["engineer_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void engineer::init(){
		id = 0;
		engineer_id = 0;
		engineer_player_id = 0;
		engineer_primary_type.clear();
		engineer_primary_weapon_id = 0;
		engineer_secondary_type.clear();
		engineer_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
