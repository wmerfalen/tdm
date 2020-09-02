#include "class-contagion.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a contagion player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t contagion::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		contagion_secondary_type = CZP10;
		if(primary_choice == primary_choice_t::M3){
			auto m3 = create_object(ITEM_RIFLE,"m3.yml");
			contagion_primary_weapon_id = m3->rifle()->attributes->flush_to_db();
			contagion_primary_type = M3;
		}
		if(primary_choice == primary_choice_t::FAMAS){
			auto famas = create_object(ITEM_RIFLE,"famas.yml");
			contagion_primary_weapon_id = famas->rifle()->attributes->flush_to_db();
			contagion_primary_type = FAMAS;
		}
		auto czp10 = create_object(ITEM_RIFLE,"czp10.yml");
		contagion_secondary_weapon_id = czp10->rifle()->attributes->flush_to_db();

		contagion_player_id = player->db_id();
		auto status = this->create<contagion>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = contagion_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t contagion::export_class() {
		strmap_t values;
		values["contagion_player_id"] = std::to_string(contagion_player_id);
		values["contagion_primary_type"] = contagion_primary_type;
		values["contagion_primary_weapon_id"] = std::to_string( contagion_primary_weapon_id);
		values["contagion_secondary_type"] = CZP10;
		values["contagion_secondary_weapon_id"] = std::to_string(contagion_secondary_weapon_id);
		return std::move(values);
	}
	int16_t contagion::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = contagion_id = 0;
		contagion_player_id = 0;
		contagion_secondary_type = contagion_primary_type ="NONE";
		contagion_primary_weapon_id = contagion_secondary_weapon_id = 0;
		return std::get<0>(this->read<contagion>(this,"contagion_player_id",std::to_string(contagion_player_id)));
	}
	int16_t contagion::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["contagion_id"].as<uint64_t>();
		contagion_id = row["contagion_id"].as<uint64_t>();
		contagion_player_id = row["contagion_player_id"].as<uint64_t>();
		contagion_primary_type = row["contagion_primary_type"].c_str();
		contagion_primary_weapon_id = row["contagion_primary_weapon_id"].as<uint64_t>();
		contagion_secondary_type = row["contagion_secondary_type"].c_str();
		contagion_secondary_weapon_id = row["contagion_secondary_weapon_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void contagion::init(){
		id = 0;
		contagion_id = 0;
		contagion_player_id = 0;
		contagion_primary_type.clear();
		contagion_primary_weapon_id = 0;
		contagion_secondary_type.clear();
		contagion_secondary_weapon_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
