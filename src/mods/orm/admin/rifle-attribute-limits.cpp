#include "rifle-attribute-limits.hpp"

namespace mods::orm::admin {
#define __MENTOC_SHOW_MODS_ORM_ADMIN_RAL_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_ORM_ADMIN_RAL_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::admin::rifle_attribute_limits]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

	strmap_t rifle_attribute_limits::export_class() {
		strmap_t values;
		values[this->primary_key_name()] = this->primary_key_value();
		values[this->low_column()] = this->low_value();
		values[this->high_column()] = this->high_value();
		values[this->overpowered_column()] = this->overpowered_value();
		values[this->type_column()] = this->type_value();
		values[this->attribute_column()] = this->attribute_value();
		values[this->created_at_column()] = this->created_at_value();
		return std::move(values);
	}
	int16_t rifle_attribute_limits::load_by_type(type_t type) {
		init();
		return std::get<0>(this->read<rifle_attribute_limits>(this,this->type_column(),type));
	}
	int16_t rifle_attribute_limits::feed(const pqxx::result::reference& row) {
		init();
		id = row[this->primary_key_name()].as<uint64_t>();
		ral_type = row[this->type_column()].c_str();
		ral_attribute = row[this->attribute_column()].c_str();
		ral_low = row[this->low_column()].as<int>();
		ral_high = row[this->high_column()].as<int>();
		ral_overpowered = row[this->overpowered_column()].as<int>();

		created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		return 0;
	}
	void rifle_attribute_limits::init() {
		id = 0;
		created_at = 0;
		loaded = 0;
		ral_attribute.clear();
		ral_type.clear();
		ral_low = ral_high = ral_overpowered = 0;
		saved_success = false;
	}


	std::deque<rifle_attribute_limits_record_t> load_all_rifle_attribute_limits() {
		std::deque<rifle_attribute_limits_record_t> rows;

		auto result = db_get_all(rifle_attribute_limits::table.data());
		if(result.size() == 0) {
			return rows;
		}

		for(auto&& row : result) {
			m_debug(green_str("rifle_attribute_limits filling:") << row["ral_type"].c_str() << "|" << row["ral_attribute"].c_str());
			rows.emplace_back();
			auto& r = rows.back();
			r.type = row["ral_type"].c_str();
			r.attribute = row["ral_attribute"].c_str();
			r.id = row["id"].as<uint64_t>();
			r.low = row["ral_low"].as<int>();
			r.high = row["ral_high"].as<int>();
			r.overpowered = row["ral_overpowered"].as<int>();
			r.created_at = mods::util::pg_timestamp_to_long(row["created_at"].c_str());
		}
		return rows;
	}
#undef m_debug
};
