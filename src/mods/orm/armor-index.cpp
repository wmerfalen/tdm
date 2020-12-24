#include "armor-index.hpp"
#include "util.hpp"

namespace mods::forge_engine {
	extern void armor_index_changed();
};
namespace mods::orm {
	std::tuple<int16_t,std::string> armor_index::delete_by_armor_type(std::string_view armor_type_name) {
		mods::forge_engine::armor_index_changed();
		return mods::orm::util::delete_where<armor_index,sql_compositor>(
		           table_name(),
		           "armor_type",
		           "=",
		           armor_type_name
		       );
	}
	int16_t armor_index::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<armor_index_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving armor_index:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		mods::forge_engine::armor_index_changed();
		return 0;
	}
	void armor_index::populate(std::string_view in_armor_type, const std::vector<std::string>& in_armor_filenames) {
		for(const auto& armor_file_name : in_armor_filenames) {
			armor_index_record_t record;
			record.armor_type = in_armor_type;
			record.armor_filename = armor_file_name;
			rows.emplace_back(record);
		}
	}
	strmap_t armor_index::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void armor_index::init() {
		id = 0;
		loaded = 0;
	}

	void armor_index::feed_multi(pqxx::result& in_rows) {
		armor_index_record_t r;
		for(auto row : in_rows) {
			r.armor_type = row["armor_type"].c_str();
			r.armor_filename = row["armor_filename"].c_str();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> armor_index::load_all() {
		mods::orm::util::load_all<armor_index,sql_compositor>(this);
		return {0,""};
	}

	std::tuple<int16_t,std::string> load_all_armor_index_data(std::map<std::string,std::vector<std::string>>* storage_ptr) {
		int16_t count = 0;
		std::string message = "";
		static armor_index data;
		data.load_all();
		for(auto& row : data.rows) {
			(*storage_ptr)[row.armor_type].emplace_back(row.armor_filename);
		}
		return {count,message};
	}
};
