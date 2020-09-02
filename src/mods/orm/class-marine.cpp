#include "class-marine.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a marine player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t marine::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		marine_secondary_type = CZP10;
		if(primary_choice == primary_choice_t::M16A4){
			auto m16a4 = create_object(ITEM_RIFLE,"m16a4.yml");
			marine_primary_weapon_id = m16a4->rifle()->attributes->flush_to_db();
			marine_primary_type = M16A4;
		}
		if(primary_choice == primary_choice_t::M4){
			auto m4 = create_object(ITEM_RIFLE,"m4.yml");
			marine_primary_weapon_id = m4->rifle()->attributes->flush_to_db();
			marine_primary_type = M4;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");
		marine_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		marine_player_id = player->db_id();
		auto status = this->create<marine>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = marine_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t marine::export_class() {
		strmap_t values;
		values["marine_player_id"] = std::to_string(marine_player_id);
		values["marine_primary_type"] = marine_primary_type;
		values["marine_primary_weapon_id"] = std::to_string( marine_primary_weapon_id);
		values["marine_secondary_type"] = CZP10;
		values["marine_secondary_weapon_id"] = std::to_string(marine_secondary_weapon_id);
		return std::move(values);
	}
	int16_t marine::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = marine_id = 0;
		marine_player_id = 0;
		marine_secondary_type = marine_primary_type ="NONE";
		marine_primary_weapon_id = marine_secondary_weapon_id = 0;
		return std::get<0>(this->read<marine>(this,"marine_player_id",std::to_string(marine_player_id)));
	}
	int16_t marine::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["marine_id"].as<uint64_t>();
		marine_id = row["marine_id"].as<uint64_t>();
		marine_player_id = row["marine_player_id"].as<uint64_t>();
		marine_primary_type = row["marine_primary_type"].c_str();
		marine_primary_weapon_id = row["marine_primary_weapon_id"].as<uint64_t>();
		marine_secondary_type = row["marine_secondary_type"].c_str();
		marine_secondary_weapon_id = row["marine_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void marine::init(){
		id = 0;
		marine_id = 0;
		marine_player_id = 0;
		marine_primary_type.clear();
		marine_primary_weapon_id = 0;
		marine_secondary_type.clear();
		marine_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
