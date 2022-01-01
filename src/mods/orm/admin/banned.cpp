#include "banned.hpp"
#include "../../player.hpp"

namespace mods::orm::admin {
#define __MENTOC_SHOW_MODS_ORM_ADMIN_BAN_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_ORM_ADMIN_BAN_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::admin::banned]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

	/*
	 * @brief this should be called when you create a banned player for the first time
	 *
	 * @param player
	 *
	 * @return
	 */
	uint64_t banned::ban_player(player_ptr_t& player) {
		init();
		b_username = player->name().c_str();
		b_ip_address = player->ip().c_str();
		b_hostname = player->host().c_str();
		auto status = this->create<banned>(this);
		saved_success = false;
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
			saved_success = true;
		}
		return id;
	}
	uint64_t banned::ban_hostname(std::string_view hostname) {
		init();
		b_username.clear();
		b_hostname = hostname;
		b_ip_address.clear();
		auto status = this->create<banned>(this);
		saved_success = false;
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
			saved_success = true;
		}
		return id;
	}
	uint64_t banned::ban_username(std::string_view username) {
		init();
		b_username = username;
		b_ip_address.clear();
		b_hostname.clear();
		auto status = this->create<banned>(this);
		saved_success = false;
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
			saved_success = true;
		}
		return id;
	}
	bool banned::unban_ip(std::string_view ip) {
		auto tup = this->read<banned>(this,this->ip_column(),ip);
		if(std::get<0>(tup) == NO_RESULTS) {
			m_debug("could not find a row to delete by ip '" << ip << "'");
			return false;
		}
		auto removal_status = this->remove(this);
		if(ORM_SUCCESS(removal_status)) {
			m_debug("unban_ip successfully removed a record from the db for ip: '" << ip << "'");
			return true;
		}
		return true;
	}

	bool banned::unban_hostname(std::string_view hostname) {
		auto tup = this->read<banned>(this,this->hostname_column(),hostname);
		if(std::get<0>(tup) == NO_RESULTS) {
			m_debug("could not find a row to delete by hostname '" << hostname << "'");
			return false;
		}
		auto removal_status = this->remove(this);
		if(ORM_SUCCESS(removal_status)) {
			m_debug("unban_hostname successfully removed a record from the db for hostname: '" << hostname << "'");
			return true;
		}
		return true;
	}

	bool banned::unban_username(std::string_view username) {
		auto tup = this->read<banned>(this,this->username_column(),username);
		if(std::get<0>(tup) == NO_RESULTS) {
			m_debug("could not find a row to delete by username '" << username << "'");
			return false;
		}
		auto removal_status = this->remove(this);
		if(ORM_SUCCESS(removal_status)) {
			m_debug("unban_username successfully removed a record from the db for username: '" << username << "'");
			return true;
		}
		return true;
	}
	uint64_t banned::ban_ip(std::string_view ip) {
		init();
		b_username.clear();
		b_ip_address = ip;
		b_hostname.clear();
		auto status = this->create<banned>(this);
		saved_success = false;
		if(ORM_SUCCESS(status)) {
			created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
			saved_success = true;
		}
		return id;
	}
	strmap_t banned::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		values[this->ip_column()] = this->ip_value();
		values[this->hostname_column()] = this->hostname_value();
		values[this->username_column()] = this->username_value();
		return std::move(values);
	}
	int16_t banned::load_by_player(player_ptr_t& player) {
		return 0;
	}
	bool banned::is_ip_banned(std::string_view ip) {
		auto tup = this->read<banned>(this,this->ip_column(),ip);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	bool banned::is_username_banned(std::string_view user_name) {
		auto tup = this->read<banned>(this,this->username_column(),user_name);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	bool banned::is_hostname_banned(std::string_view hostname) {
		if(hostname.length() == 0) {
			return false;
		}
		auto tup = this->read<banned>(this,this->hostname_column(),hostname);
		if(std::get<0>(tup) == NO_RESULTS) {
			return false;
		}
		return true;
	}
	int16_t banned::load_by_ip(std::string_view ip) {
		init();
		return std::get<0>(this->read<banned>(this,this->ip_column(),ip));
	}
	int16_t banned::load_by_username(std::string_view username) {
		init();
		return std::get<0>(this->read<banned>(this,this->username_column(),username));
	}
	int16_t banned::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		b_username = row[this->username_column()].c_str();
		b_ip_address = row[this->ip_column()].c_str();
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

#undef m_debug
};
