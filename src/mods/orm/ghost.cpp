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
	uint64_t ghost::initialize_row(player_ptr_t &player, primary_choice_t primary_choice) {
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
		return std::move(values);
	}
	int16_t ghost::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = ghost_id = 0;
		ghost_player_id = 0;
		return std::get<0>(this->read<ghost>(this,"ghost_player_id",std::to_string(ghost_player_id)));
	}
	int16_t ghost::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["ghost_id"].as<uint64_t>();
		ghost_id = row["ghost_id"].as<uint64_t>();
		ghost_player_id = row["ghost_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void ghost::init(){
		id = 0;
		ghost_id = 0;
		ghost_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

