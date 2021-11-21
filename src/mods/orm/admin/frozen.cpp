#include "frozen.hpp"
#include "../../player.hpp"

namespace mods::orm::admin {

	/*
	 * @brief this should be called when you create a frozen player for the first time
	 *
	 * @param player
	 * @param primary_choice IGNORED DEPRECATED
	 *
	 * @return
	 */
	uint64_t frozen::initialize_row(player_ptr_t& player) {
		init();
		f_username = player->name().c_str();
		f_ip_address = player->host().c_str();
		auto status = this->create<frozen>(this);
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	strmap_t frozen::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		return std::move(values);
	}
	int16_t frozen::load_by_player(player_ptr_t& player) {
		return 0;
	}
	int16_t frozen::load_by_player_ip(std::string_view player_ip) {
		init();
		return std::get<0>(this->read<frozen>(this,this->player_ip_column(),player_ip));
	}
	int16_t frozen::load_by_player_name(std::string_view player_name) {
		init();
		return std::get<0>(this->read<frozen>(this,this->player_name_column(),player_name));
	}
	int16_t frozen::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		f_username = row[this->player_name_column()].c_str();
		f_ip_address = row[this->player_ip_column()].c_str();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		return 0;
	}
	void frozen::init() {
		id = 0;
		created_at = 0;
		loaded = 0;
		f_username.clear();
		f_ip_address.clear();
		f_enforce = 1;
	}

};
