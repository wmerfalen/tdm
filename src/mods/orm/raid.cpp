//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "raid.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::raid::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::raid::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	void raid::feed_multi(pqxx::result& in_rows) {
		raid r;
		for(auto row : in_rows) {
			r.id = row["id"].as<uint64_t>();
			r.r_name = row["r_name"].c_str();
			r.r_level = row["r_level"].c_str();
			r.r_type = row["r_type"].c_str();
			r.r_status = row["r_status"].c_str();
			rows.emplace_back(r);
		}
	}

	raid_status_t create_raid(
	    std::string_view name,
	    std::string_view level,
	    std::string_view type) {
		raid_status_t r;
		r.record = std::make_unique<raid>();
		auto status = r.record->create(r.record.get());
		r.orm_status = {std::get<0>(status),std::get<1>(status)};
		return r;
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
