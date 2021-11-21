#ifndef __MENTOC_MODS_ORM_ADMIN_BANNED_HEADER__
#define __MENTOC_MODS_ORM_ADMIN_BANNED_HEADER__

#include "../orm-base.hpp"

namespace mods::orm::admin {
	struct banned : public mods::orm::orm_base<banned,std::string> {
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;
		using f_vnum_t = uint32_t;
		std::string table_name() const {
			return "banned";
		}
		std::string column_prefix() const {
			return "b_";
		}
		std::string id_column() const {
			return "id";
		}
		banned() : id(0) {}
		~banned() = default;

		bool is_ip_banned(std::string_view ip);
		bool is_hostname_banned(std::string_view hostname);
		bool is_username_banned(std::string_view user_name);

		uint64_t ban_player(player_ptr_t& player);
		uint64_t ban_username(std::string_view username);
		uint64_t ban_hostname(std::string_view hostname);
		uint64_t ban_ip(std::string_view ip);

		bool unban_ip(std::string_view ip);
		bool unban_hostname(std::string_view hostname);
		bool unban_username(std::string_view username);

		int16_t feed(const pqxx::result::reference&);
		int16_t load_by_player(player_ptr_t& player);
		void init();
		strmap_t export_class();
		int16_t save() {
			return std::get<0>(this->update<banned>(this));
		}

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}
		int16_t load_by_username(std::string_view player_name);
		int16_t load_by_ip(std::string_view ip);
		int16_t load_by_hostname(std::string_view ip);

		std::string hostname_column() {
			return "b_hostname";
		}
		std::string ip_column() {
			return "b_ip_address";
		}
		std::string username_column() {
			return "b_username";
		}
		std::string username_value() {
			return b_username;
		}
		std::string hostname_value() {
			return b_hostname;
		}
		std::string ip_value() {
			return b_ip_address;
		}

		void load_all();

		id_type_t id;
		std::string b_ip_address;
		std::string b_hostname;
		std::string b_username;
		bool b_enforce;
		long created_at;
		bool loaded;
		bool saved_success;
	};

};

#endif
