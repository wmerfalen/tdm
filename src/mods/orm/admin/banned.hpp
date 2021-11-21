#ifndef __MENTOC_MODS_ORM_ADMIN_BANNED_HEADER__
#define __MENTOC_MODS_ORM_ADMIN_BANNED_HEADER__

#include "../orm-base.hpp"

namespace mods::orm::admin {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;

	/**
	 * it should be noted that this class is the first orm class to allow
	 * multiple values for a single variable (f_room_vnum_list).
	 * This is the proposed model that should be utilized for normalized
	 * data. I don't know of a better way to perform this (given what's
	 * already been coded).
	 *
	 * will - 2020-09-28
	 */
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
		uint64_t initialize_row(player_ptr_t& player);
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
		int16_t load_by_player_name(std::string_view player_name);
		int16_t load_by_player_ip(std::string_view ip);

		std::string player_hostname_column() {
			return "b_hostname";
		}
		std::string player_ip_column() {
			return "b_ip_address";
		}
		std::string player_name_column() {
			return "b_username";
		}
		std::string player_name_value() {
			return b_username;
		}

		void load_all();

		id_type_t id;
		std::string b_ip_address;
		std::string b_hostname;
		std::string b_username;
		bool b_enforce;
		long created_at;
		bool loaded;
	};

};

#endif
