#if 0
#include "sniper.hpp"
#include "../player.hpp"

namespace mods::orm {
	void sniper::load_skill_usages() {
		//skill_usage = player_skill_usage.get_player_levels(sniper_player_id,"sniper");
	}
	void sniper::increment_skill_usage(const uint64_t& skill_id) {
		++skill_usage[skill_id];
	}
	/**
	 * @brief this should be called when you create a sniper player for the first time
	 *
	 * @param player
	 * @param primary_choice
	 *
	 * @return
	 */
	uint64_t sniper::initialize_row(player_ptr_t& player) {
		init();
		sniper_player_id = player->db_id();
		auto status = this->create<sniper>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = sniper_id = std::get<2>(status);
		}
		return id;
	}
	strmap_t sniper::export_class() {
		strmap_t values;
		values["sniper_player_id"] = std::to_string(sniper_player_id);
		return std::move(values);
	}
	int16_t sniper::load_by_player(uint64_t player_id) {
		loaded = 0;
		created_at = updated_at = 0;
		id = sniper_id = 0;
		sniper_player_id = 0;
		return std::get<0>(this->read<sniper>(this,"sniper_player_id",std::to_string(sniper_player_id)));
	}
	int16_t sniper::feed(const pqxx::result::reference& row) {
		init();
		loaded = 0;
		id = row["sniper_id"].as<uint64_t>();
		sniper_id = row["sniper_id"].as<uint64_t>();
		sniper_player_id = row["sniper_player_id"].as<uint64_t>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		updated_at = mods::util::pg_timestamp_to_long(row["updated_at"].c_str());
		loaded = 1;
		return 0;
	}
	void sniper::init() {
		id = 0;
		sniper_id = 0;
		sniper_player_id = 0;
		created_at = 0;
		updated_at = 0;
		loaded = 0;
	}

};

#endif
