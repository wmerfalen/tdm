#ifndef __MENTOC_MODS_ORM_ADMIN_user_logins_HEADER__
#define __MENTOC_MODS_ORM_ADMIN_user_logins_HEADER__

#include "orm-base.hpp"
#include <set>

namespace mods::orm {
	struct user_logins_row {
		std::string ip;
		std::string username;
	};
	struct user_logins : public mods::orm::orm_base<user_logins,std::string> {
		using id_type_t = uint64_t;
		std::string table_name() const {
			return "user_logins";
		}
		std::string column_prefix() const {
			return "u_";
		}
		std::string id_column() const {
			return "id";
		}
		user_logins() : id(0) {}
		~user_logins() = default;

		void user_logged_in(std::string ip,std::string user_name);

		int16_t feed(const pqxx::result::reference&);
		int16_t load_by_player(player_ptr_t& player);
		void init();
		strmap_t export_class();
		int16_t save() {
			return std::get<0>(this->update<user_logins>(this));
		}

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}
		int16_t load_by_username(std::string player_name);
		int16_t load_by_ip(std::string ip);

		std::string ip_column() {
			return "u_ip_address";
		}
		std::string username_column() {
			return "u_username";
		}
		std::string username_value() {
			return u_username;
		}
		std::string ip_value() {
			return u_ip_address;
		}

		void feed_multi(pqxx::result&);

		id_type_t id;
		std::string u_ip_address;
		std::string u_username;
		bool loaded;
		bool saved_success;
		std::vector<user_logins_row> rows;
		std::set<std::string> ip_addresses;
	};

};

#endif
