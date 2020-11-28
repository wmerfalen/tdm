#include "syndrome.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a syndrome player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return 
	 */
	uint64_t syndrome::initialize_row(player_ptr_t &player){
		init();
		syndrome_player_id = player->db_id();
		auto status = this->create<syndrome>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = syndrome_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t syndrome::export_class() {
		strmap_t values;
		values["syndrome_player_id"] = std::to_string(syndrome_player_id);
		return std::move(values);
	}
	int16_t syndrome::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = syndrome_id = 0;
		syndrome_player_id = 0;
		return std::get<0>(this->read<syndrome>(this,"syndrome_player_id",std::to_string(syndrome_player_id)));
	}
	int16_t syndrome::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["syndrome_id"].as<uint64_t>();
		syndrome_id = row["syndrome_id"].as<uint64_t>();
		syndrome_player_id = row["syndrome_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void syndrome::init(){
		id = 0;
		syndrome_id = 0;
		syndrome_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

