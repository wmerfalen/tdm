#include "striker.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a striker player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t striker::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
		init();
		striker_player_id = player->db_id();
		auto status = this->create<striker>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = striker_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t striker::export_class() {
		strmap_t values;
		values["striker_player_id"] = std::to_string(striker_player_id);
		return std::move(values);
	}
	int16_t striker::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = striker_id = 0;
		striker_player_id = 0;
		return std::get<0>(this->read<striker>(this,"striker_player_id",std::to_string(striker_player_id)));
	}
	int16_t striker::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["striker_id"].as<uint64_t>();
		striker_id = row["striker_id"].as<uint64_t>();
		striker_player_id = row["striker_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void striker::init(){
		id = 0;
		striker_id = 0;
		striker_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

