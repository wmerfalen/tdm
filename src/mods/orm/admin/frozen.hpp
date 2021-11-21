#ifndef __MENTOC_MODS_ORM_ADMIN_FROZEN_HEADER__
#define __MENTOC_MODS_ORM_ADMIN_FROZEN_HEADER__

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
	struct frozen : public mods::orm::orm_base<frozen,std::string> {
		using id_type_t = uint64_t;
		using room_vnum_t = uint32_t;
		using f_vnum_t = uint32_t;
		std::string table_name() const {
			return "frozen";
		}
		std::string column_prefix() const {
			return "f_";
		}
		std::string id_column() const {
			return "id";
		}
		frozen() : id(0) {}
		~frozen() = default;

		uint64_t initialize_row(player_ptr_t& player);
		int16_t feed(const pqxx::result::reference&);
		int16_t load_by_player(player_ptr_t& player);
		void init();
		strmap_t export_class();
		int16_t save() {
			return std::get<0>(this->update<frozen>(this));
		}

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}
		int16_t load_by_player_name(std::string_view player_name);
		int16_t load_by_player_ip(std::string_view ip);

		std::string player_ip_column() {
			return "f_ip_address";
		}
		std::string player_name_column() {
			return "f_username";
		}
		std::string player_name_value() {
			return f_username;
		}

		void load_all();

		id_type_t id;
		std::string f_ip_address;
		std::string f_username;
		bool f_enforce;
		long created_at;
		bool loaded;
	};

};

#endif
