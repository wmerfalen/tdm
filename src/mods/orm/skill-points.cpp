#include "skill-points.hpp"
#include "util.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> skill_points::delete_by_level(uint16_t level) {
		return mods::orm::util::delete_where<skill_points,sql_compositor>(
		           table_name(),
		           "sp_level",
		           "=",
		           std::to_string(level)
		       );
	}
	int16_t skill_points::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<skill_points_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving skill_points:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}

	void skill_points::populate(std::map<uint16_t,uint16_t>* data) {
		for(const auto& pair : *data) {
			skill_points_record_t record;
			record.level = pair.first;
			record.skill_points = pair.second;
			rows.emplace_back(record);
		}
	}
	strmap_t skill_points::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void skill_points::init() {
		id = 0;
		loaded = 0;
	}

	void skill_points::feed_multi(pqxx::result& in_rows) {
		skill_points_record_t r;
		for(auto row : in_rows) {
			r.level = row["sp_level"].as<int>();
			r.skill_points = row["sp_points"].as<int>();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> skill_points::load_all() {
		mods::orm::util::load_all<skill_points,sql_compositor>(this);
		return {0,""};
	}
	uint16_t skill_points::calculate_skill_points_by_level(uint16_t player_level) {
		std::map<uint16_t,uint16_t> levels;
		load_all_skill_points(&levels);
		uint16_t skill_points = 0;
		for(const auto& row : levels) {
			if(row.first <= player_level) {
				skill_points += row.second;
			}
		}
		return skill_points;
	}

	std::tuple<int16_t,std::string> load_all_skill_points(std::map<uint16_t,uint16_t>* storage_ptr) {
		int16_t count = 0;
		std::string message = "";
		static skill_points data;
		data.load_all();
		for(auto& row : data.rows) {
			(*storage_ptr)[row.level] = row.skill_points;
		}
		return {count,message};
	}
};
