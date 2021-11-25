#include "user-logins.hpp"
#include "../player.hpp"

namespace mods::orm {
#define __MENTOC_SHOW_MODS_ORM_ADMIN_USER_LOGINS_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_ORM_ADMIN_USER_LOGINS_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::admin::user_logins]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

	/*
	 * @brief this should be called when you create a user_logins player for the first time
	 *
	 * @param player
	 *
	 * @return
	 */
	void user_logins::user_logged_in(std::string ip,std::string user_name) {
		init();
		u_username = user_name;
		u_ip_address = ip;
		auto status = this->create<user_logins>(this);
		saved_success = false;
		if(ORM_SUCCESS(status)) {
			loaded = 1;
			id = std::get<2>(status);
			saved_success = true;
		}
	}
	int16_t user_logins::load_by_ip(std::string ip) {
		init();
		return std::get<0>(this->read<user_logins>(this,this->ip_column(),ip));
	}
	int16_t user_logins::load_by_username(std::string username) {
		init();
		return std::get<0>(this->read_normalized<user_logins>(this,this->username_column(),username));
	}
	void user_logins::feed_multi(pqxx::result& in_rows) {
		rows.clear();
		ip_addresses.clear();
		user_logins_row r;
		for(auto row : in_rows) {
			r.ip = row[this->ip_column()].c_str();
			r.username = row[this->username_column()].c_str();
			rows.emplace_back(r);
			ip_addresses.insert(r.ip);
		}
	}
	int16_t user_logins::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		u_username = row[this->username_column()].c_str();
		u_ip_address = row[this->ip_column()].c_str();
		return 0;
	}
	void user_logins::init() {
		id = 0;
		loaded = 0;
		u_username.clear();
		u_ip_address.clear();
	}
	strmap_t user_logins::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		values[this->ip_column()] = this->ip_value();
		values[this->username_column()] = this->username_value();
		return std::move(values);
	}

#undef m_debug
};
