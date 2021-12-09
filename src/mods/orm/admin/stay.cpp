#include "stay.hpp"
#include "../../player.hpp"

namespace mods::orm::admin {

	/*
	 * @brief this should be called when you create a stay player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return
	 */
	uint64_t stay::initialize_row(player_ptr_t& player, const room_vnum& r_vnum) {
		init();
		s_username = player->name().c_str();
		s_room_vnum = r_vnum;
		auto status = this->create<stay>(this);
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	int16_t stay::remove_for(player_ptr_t& player) {
		init();
		load_by_player_name(player->name().c_str());
		if(loaded) {
			std::cerr << "REMOVING WHERE PLAYER IS: '" << player->name().c_str() << "'\n";
			return std::get<0>(this->remove<stay>(this));
		}
		return -1;
	}
	strmap_t stay::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		values[room_vnum_column()] = room_vnum_value();
		values[player_name_column()] = player_name_value();
		return std::move(values);
	}
	int16_t stay::load_by_player(player_ptr_t& player) {
		init();
		return load_by_player_name(player->name().c_str());
	}
	int16_t stay::load_by_player_name(std::string_view player_name) {
		init();
		auto s = this->read<stay>(this,this->player_name_column(),player_name);
		std::cerr << green_str("read:") << "player: '" << player_name << "', [0]:" << std::get<0>(s) << ", [1]: '" << std::get<1>(s) << "'\n";
		if(ORM_SUCCESS(s)) {
			loaded = true;
		}
		return std::get<0>(s);
	}
	int16_t stay::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		s_username = row[this->player_name_column()].c_str();
		s_room_vnum = row[this->room_vnum_column()].as<room_vnum>();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		return 0;
	}
	void stay::init() {
		id = 0;
		created_at = 0;
		loaded = 0;
		s_username.clear();
		s_room_vnum = 0;
	}

};
