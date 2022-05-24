#ifndef __MENTOC_MODS_ORM_player_achievements_HEADER__
#define __MENTOC_MODS_ORM_player_achievements_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view player_achievements_table_name = "player_achievements";
	struct player_achievements_record_t {
		uint64_t id;
		std::string name;
		uint16_t points;
		uint64_t player_id;
		strmap_t export_class() {
			strmap_t v;
			v["pa_name"] = name;
			v["pa_points"] = std::to_string(this->points);
			v["ps_player_id"] = std::to_string(this->player_id);
			return std::move(v);
		}
		std::string table_name() const {
			return player_achievements_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct player_achievements : public mods::orm::orm_base<player_achievements,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return player_achievements_table_name.data();
		}
		std::string column_prefix() const {
			return "pa_";
		}
		std::string id_column() const {
			return "id";
		}
		player_achievements() {
			this->init();
			loaded = 0;
			id = 0;
		}
		~player_achievements() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);
		void populate(const uint64_t& player_id, const std::map<std::string,uint16_t>& data);
		std::map<std::string,uint16_t> get_player_levels(const uint64_t& player_id, std::string_view player_class);
		void init();

		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();

		std::tuple<int16_t,std::string> delete_by_player(const uint64_t& player_id);
		std::tuple<int16_t,std::string> load_by_player(const uint64_t& player_id);

		std::vector<player_achievements_record_t> rows;
		bool loaded;

		uint64_t primary_key_id;
		std::string name;
		uint16_t points;
		uint64_t player_id;
	};
	std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<std::string,uint16_t>* data);
	std::tuple<int16_t,std::string> sync_player_with_class_skills(const uint64_t& player_id, std::string_view player_class);
};

#endif
