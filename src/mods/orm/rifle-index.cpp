#include "rifle-index.hpp"
#include "util.hpp"

namespace mods::forge_engine {
	extern void rifle_index_changed();
};
namespace mods::orm {
	std::tuple<int16_t,std::string> rifle_index::delete_by_rifle_type(std::string_view rifle_type_name) {
		mods::forge_engine::rifle_index_changed();
		return mods::orm::util::delete_where<rifle_index,sql_compositor>(
		           table_name(),
		           "rifle_type",
		           "=",
		           rifle_type_name
		       );
	}
	int16_t rifle_index::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<rifle_index_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving rifle_index:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		mods::forge_engine::rifle_index_changed();
		return 0;
	}
	void rifle_index::populate(std::string_view in_rifle_type, const std::vector<std::string>& in_rifle_filenames) {
		for(const auto& rifle_file_name : in_rifle_filenames) {
			rifle_index_record_t record;
			record.rifle_type = in_rifle_type;
			record.rifle_filename = rifle_file_name;
			rows.emplace_back(record);
		}
	}
	strmap_t rifle_index::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void rifle_index::init() {
		id = 0;
		loaded = 0;
	}

	void rifle_index::feed_multi(pqxx::result& in_rows) {
		rifle_index_record_t r;
		for(auto row : in_rows) {
			r.rifle_type = row["rifle_type"].c_str();
			r.rifle_filename = row["rifle_filename"].c_str();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> rifle_index::load_all() {
		mods::orm::util::load_all<rifle_index,sql_compositor>(this);
		return {0,""};
	}

	std::tuple<int16_t,std::string> load_all_rifle_index_data(std::map<std::string,std::vector<std::string>>* storage_ptr) {
		int16_t count = 0;
		std::string message = "";
		static rifle_index data;
		data.load_all();
		for(auto& row : data.rows) {
			(*storage_ptr)[row.rifle_type].emplace_back(row.rifle_filename);
		}
		return {count,message};
	}
};
