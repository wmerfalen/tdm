//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "notch.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::notch::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::notch::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	void notch::feed_multi(pqxx::result& in_rows) {
		notch_record_t r;
		for(auto row : in_rows) {
			r.id = row["id"].as<uint64_t>();
			r.n_name = row["n_name"].c_str();
			r.n_points = row["n_points"].as<int>();
			r.n_player_id = row["n_player_id"].as<uint64_t>();
			rows.emplace_back(r);
		}
	}

	std::tuple<int16_t,std::string> increment_player_notch(player_ptr_t& player,std::string_view notch) {
		mods::orm::notch n;
		m_debug("start");
		auto status = n.load_by_player_and_notch(player->db_id(),notch);
		auto results = std::get<2>(status);
		if(results.size() == 0) {
			n.n_name = notch;
			n.n_player_id = player->db_id();
			n.n_points = 1;
			n.create(&n);
		} else {
			n.feed(results[0]);
		}
		++n.n_points;
		m_debug("n_points: " << n.n_points << "\n");
		mods::util::maps::dump(n.export_class());
		m_debug("end of dump");
		return n.update(&n);
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
