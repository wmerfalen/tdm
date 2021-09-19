#include "contagion.hpp"
#include "../player.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a contagion player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return
	 */
	uint64_t contagion::initialize_row(player_ptr_t& player) {
		init();
		contagion_player_id = player->db_id();
		auto status = this->create<contagion>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = contagion_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t contagion::export_class() {
		strmap_t values;
		values["contagion_player_id"] = std::to_string(contagion_player_id);
		return std::move(values);
	}
	int16_t contagion::load_by_player(uint64_t player_id) {
		loaded = 0;
		created_at = updated_at = 0;
		id = contagion_id = 0;
		contagion_player_id = 0;
		created_at = (decltype(created_at))0;
		return std::get<0>(this->read<contagion>(this,"contagion_player_id",std::to_string(contagion_player_id)));
	}
	int16_t contagion::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["contagion_id"].as<uint64_t>();
		contagion_id = row["contagion_id"].as<uint64_t>();
		contagion_player_id = row["contagion_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		return 0;
	}
	void contagion::init() {
		id = 0;
		contagion_id = 0;
		contagion_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

