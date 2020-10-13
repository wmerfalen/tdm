#include "pyrexia.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a pyrexia player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t pyrexia::initialize_row(player_ptr_t &player){
		init();
		pyrexia_player_id = player->db_id();
		auto status = this->create<pyrexia>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = pyrexia_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t pyrexia::export_class() {
		strmap_t values;
		values["pyrexia_player_id"] = std::to_string(pyrexia_player_id);
		return std::move(values);
	}
	int16_t pyrexia::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = pyrexia_id = 0;
		pyrexia_player_id = 0;
		return std::get<0>(this->read<pyrexia>(this,"pyrexia_player_id",std::to_string(pyrexia_player_id)));
	}
	int16_t pyrexia::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["pyrexia_id"].as<uint64_t>();
		pyrexia_id = row["pyrexia_id"].as<uint64_t>();
		pyrexia_player_id = row["pyrexia_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void pyrexia::init(){
		id = 0;
		pyrexia_id = 0;
		pyrexia_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

