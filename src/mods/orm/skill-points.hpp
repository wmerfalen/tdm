#ifndef __MENTOC_MODS_ORM_SKILL_POINTS_HEADER__
#define __MENTOC_MODS_ORM_SKILL_POINTS_HEADER__

#include "orm-base.hpp"

namespace mods::orm {
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view skill_points_table_name = "skill_points";
	struct skill_points_record_t {
		uint16_t level;
		uint16_t skill_points;
		strmap_t export_class() {
			strmap_t v;
			v["sp_level"] = std::to_string(this->level);
			v["sp_points"] = std::to_string(this->skill_points);
			return std::move(v);
		}
		std::string table_name() const {
			return skill_points_table_name.data();
		}
		std::string primary_key_name() {
			return "id";
		}
	};

	struct skill_points : public mods::orm::orm_base<skill_points,std::string> {
		using primary_choice_t = std::string;
		std::string table_name() const {
			return skill_points_table_name.data();
		}
		std::string column_prefix() const {
			return "sp_";
		}
		std::string id_column() const {
			return "id";
		}
		skill_points() {
			this->init();
			loaded = 0;
		}
		~skill_points() = default;

		std::string primary_key_name() {
			return id_column();
		}
		std::string primary_key_value() {
			return std::to_string(this->id);
		}

		void load_multi(const pqxx::result::reference&);
		void populate(std::map<uint16_t,uint16_t>* data);
		void init();

		std::tuple<int16_t,std::string> load_all();
		void feed_multi(pqxx::result&);

		strmap_t export_class();
		int16_t save();
		using id_type_t = uint64_t;
		using room_vnum_t = uint16_t;

		std::tuple<int16_t,std::string> delete_by_level(uint16_t level);
		uint16_t calculate_skill_points_by_level(uint16_t player_level);

		uint16_t level;
		std::vector<skill_points_record_t> rows;
		bool loaded;
	};
	std::tuple<int16_t,std::string> load_all_skill_points(std::map<uint16_t,uint16_t>* storage_ptr);
};

#endif
