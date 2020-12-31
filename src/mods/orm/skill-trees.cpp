#include "skill-trees.hpp"
#include "util.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> skill_trees::delete_by_player_class(std::string_view player_class) {
		return mods::orm::util::delete_where<skill_trees,sql_compositor>(
		           table_name(),
		           "skill_player_class",
		           "=",
		           player_class.data()
		       );
	}
	int16_t skill_trees::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<skill_trees_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving skill_trees:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}

	void skill_trees::populate(std::map<uint16_t,uint16_t>* data) {
#if 0
		for(const auto& pair : *data) {
			skill_trees_record_t record;
			/** TODO
			record.level = pair.first;
			record.skill_trees = pair.second;
			rows.emplace_back(record);
			*/
		}
#endif
	}
	strmap_t skill_trees::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void skill_trees::init() {
		id = 0;
		loaded = 0;
	}

	void skill_trees::feed_multi(pqxx::result& in_rows) {
		skill_trees_record_t r;
		for(auto row : in_rows) {
			r.id = row["id"].as<int>();
			r.skill_name = row["skill_name"].c_str();
			r.skill_category = row["skill_category"].c_str();
			r.skill_player_class = row["skill_player_class"].c_str();
			if(row["skill_parent_id"].is_null()) {
				r.skill_parent_id = 0;
			} else {
				r.skill_parent_id = row["skill_parent_id"].as<int>();
			}
			if(row["skill_order"].is_null()) {
				r.skill_order = 0;
			} else {
				r.skill_order = row["skill_order"].as<int>();
			}
			if(row["skill_description"].is_null()) {
				r.skill_description = "";
			} else {
				r.skill_description = row["skill_description"].c_str();
			}
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> skill_trees::load_all() {
		mods::orm::util::load_all<skill_trees,sql_compositor>(this);
		return {0,""};
	}
	std::tuple<int16_t,std::string> skill_trees::load_by_class(std::string_view player_class) {
		return this->read_normalized<skill_trees>(this,"skill_player_class",player_class.data());
	}
	std::tuple<int16_t,std::string> load_player_class_skill_trees(std::string_view player_class,std::vector<skill_trees_record_t>* storage_ptr) {
		std::string message = "";
		skill_trees data;
		data.rows.clear();
		data.load_by_class(player_class);
		*storage_ptr = std::move(data.rows);
		return {storage_ptr->size(),message};
	}
};
