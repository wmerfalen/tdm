#include "support.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a support player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return 
	 */
	uint64_t support::initialize_row(player_ptr_t &player){
		init();
		support_player_id = player->db_id();
		auto status = this->create<support>(this);
		if(ORM_SUCCESS(status)){
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = support_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t support::export_class() {
		strmap_t values;
		values["support_player_id"] = std::to_string(support_player_id);
		return std::move(values);
	}
	int16_t support::load_by_player(uint64_t player_id){
		loaded = 0;
		created_at = updated_at = 0;
		id = support_id = 0;
		support_player_id = 0;
		return std::get<0>(this->read<support>(this,"support_player_id",std::to_string(player_id)));
	}

	int16_t support::feed(const pqxx::result::reference & row){
		init();
		loaded = 0;
		id = row["support_id"].as<uint64_t>();
		support_id = row["support_id"].as<uint64_t>();
		support_player_id = row["support_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void support::init(){
		id = 0;
		support_id = 0;
		support_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
