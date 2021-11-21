#include "banned.hpp"
#include "../../player.hpp"

namespace mods::orm::admin {

	/*
	 * @brief this should be called when you create a banned player for the first time
	 *
	 * @param player
	 *
	 * @return
	 */
	uint64_t banned::initialize_row(player_ptr_t& player) {
		init();
		b_username = player->name().c_str();
		b_ip_address = player->host().c_str();
		auto status = this->create<banned>(this);
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	strmap_t banned::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		return std::move(values);
	}
	int16_t banned::load_by_player(player_ptr_t& player) {
		return 0;
	}
	bool banned::is_ip_banned(std::string_view ip) {
		auto tup = this->read<banned>(this,this->player_ip_column(),ip);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	bool banned::is_username_banned(std::string_view user_name) {
		auto tup = this->read<banned>(this,this->player_name_column(),user_name);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	bool banned::is_hostname_banned(std::string_view hostname) {
		auto tup = this->read<banned>(this,this->player_hostname_column(),hostname);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	int16_t banned::load_by_player_ip(std::string_view player_ip) {
		init();
		return std::get<0>(this->read<banned>(this,this->player_ip_column(),player_ip));
	}
	int16_t banned::load_by_player_name(std::string_view player_name) {
		init();
		return std::get<0>(this->read<banned>(this,this->player_name_column(),player_name));
	}
	int16_t banned::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		b_username = row[this->player_name_column()].c_str();
		b_ip_address = row[this->player_ip_column()].c_str();
		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		return 0;
	}
	void banned::init() {
		id = 0;
		created_at = 0;
		loaded = 0;
		b_username.clear();
		b_ip_address.clear();
		b_enforce = 1;
	}

};
