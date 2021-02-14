#include "breacher.hpp"
#include "../player.hpp"

namespace mods::orm {
	/**
	 * @brief this should be called when you create a breacher player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return
	 */
	uint64_t breacher::initialize_row(player_ptr_t& player) {
		init();
		breacher_player_id = player->db_id();
		auto status = this->create<breacher>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = breacher_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t breacher::export_class() {
		strmap_t values;
		values["breacher_player_id"] = std::to_string(breacher_player_id);
		return std::move(values);
	}
	int16_t breacher::load_by_player(uint64_t player_id) {
		loaded = 0;
		created_at = updated_at = 0;
		id = breacher_id = 0;
		breacher_player_id = 0;
		return std::get<0>(this->read<breacher>(this,"breacher_player_id",std::to_string(breacher_player_id)));
	}
	int16_t breacher::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["breacher_id"].as<uint64_t>();
		breacher_id = row["breacher_id"].as<uint64_t>();
		breacher_player_id = row["breacher_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void breacher::init() {
		id = 0;
		breacher_id = 0;
		breacher_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};
