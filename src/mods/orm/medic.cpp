#include "medic.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a medic player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t medic::initialize_row(player_ptr_t &player){
		init();
		medic_player_id = player->db_id();
		auto status = this->create<medic>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = medic_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t medic::export_class() {
		strmap_t values;
		values["medic_player_id"] = std::to_string(medic_player_id);
		return std::move(values);
	}
	int16_t medic::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = medic_id = 0;
		medic_player_id = 0;
		return std::get<0>(this->read<medic>(this,"medic_player_id",std::to_string(player_id)));
	}

	int16_t medic::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["medic_id"].as<uint64_t>();
		medic_id = row["medic_id"].as<uint64_t>();
		medic_player_id = row["medic_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void medic::init(){
		id = 0;
		medic_id = 0;
		medic_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

