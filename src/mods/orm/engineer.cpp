#include "engineer.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a engineer player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t engineer::initialize_row(player_ptr_t &player){
		init();
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
		return std::move(values);
	}
	int16_t engineer::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = engineer_id = 0;
		engineer_player_id = 0;
		return std::get<0>(this->read<engineer>(this,"engineer_player_id",std::to_string(engineer_player_id)));
	}
	int16_t engineer::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["engineer_id"].as<uint64_t>();
		engineer_id = row["engineer_id"].as<uint64_t>();
		engineer_player_id = row["engineer_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void engineer::init(){
		id = 0;
		engineer_id = 0;
		engineer_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
