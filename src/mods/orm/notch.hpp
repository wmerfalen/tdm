#ifndef __MENTOC_MODS_ORM_notch_HEADER__
#define __MENTOC_MODS_ORM_notch_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view notch_table_name = "notch";
	struct notch_record_t {
		uint64_t id;
		std::string n_name;
		uint16_t n_points;
		uint64_t n_player_id;
		strmap_t export_class() {
			strmap_t v;
			v["n_name"] = n_name;
			v["n_points"] = std::to_string(this->n_points);
			v["n_player_id"] = std::to_string(this->n_player_id);
			return std::move(v);
		}
		std::string table_name() const {
			return notch_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct notch : public mods::orm::orm_base<notch,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return notch_table_name.data();
		}
		std::string column_prefix() const {
			return "n_";
		}
		std::string id_column() const {
			return "id";
		}
		notch() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~notch() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);
		void populate(const uint64_t& player_id, const std::map<std::string,uint16_t>& data);
		std::map<std::string,uint16_t> get_player_levels(const uint64_t& player_id, std::string_view notch);
		void init();

		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		std::tuple<int16_t,std::string> delete_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player_and_notch(const uint64_t& player_id,std::string_view notch);

		std::vector<notch_record_t> rows;
		bool loaded;

		uint64_t primary_key_id;
		std::string n_name;
		uint16_t n_points;
		uint64_t n_player_id;
		uint64_t id;
	};
	std::tuple<int16_t,std::string> increment_player_notch(player_ptr_t& player,std::string_view notch);
};

#endif
