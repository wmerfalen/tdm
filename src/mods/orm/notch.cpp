#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#define __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
#include "notch.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

//#ifdef __MENTOC_SHOW_MODS_BLEED_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::notch::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::notch::ERROR]"))  << MSG << "\n";
//#else
//#define m_debug(MSG) ;;
//#define m_error(MSG) ;;
//#endif


namespace mods::orm {
	std::tuple<int16_t,std::string> notch::delete_by_player(const uint64_t& player_id) {
		return mods::orm::util::delete_where<notch,sql_compositor>(
		           table_name(),
		           "n_player_id",
		           "=",
		           std::to_string(player_id)
		       );
	}
	int16_t notch::save() {
		for(auto& record : this->rows) {
			this->n_name = record.n_name;
			this->n_points = record.n_points;
			this->n_player_id = record.n_player_id;
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<notch_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving notch:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	std::string notch::primary_key_value() {
		return std::to_string(id);
	}

	void notch::populate(const uint64_t& player_id,const std::map<std::string,uint16_t>& data) {
		for(const auto& pair : data) {
			notch_record_t record;
			record.n_name = pair.first;
			record.n_points = pair.second;
			record.n_player_id = player_id;
			rows.emplace_back(record);
		}
	}

	strmap_t notch::export_class() {
		strmap_t values;
		values["n_name"] = n_name;
		values["n_points"] = std::to_string(n_points);
		values["n_player_id"] = std::to_string(n_player_id);
		values["id"] = std::to_string(id);
		return std::move(values);
	}

	void notch::init() {
		id = 0;
		loaded = 0;
		primary_key_id = 0;
	}

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
	std::tuple<int16_t,std::string> notch::load_by_player(const uint64_t& player_id) {
		return mods::orm::util::load_multi_by_column<notch,sql_compositor>(
		           this,
		           "n_player_id",
		           std::to_string(player_id)
		       );
	}
	std::tuple<int16_t,std::string> notch::load_by_player_and_notch(const uint64_t& player_id,std::string_view notch) {
		using statement = std::vector<mods::orm::util::statement_t>;
		using c = std::vector<pqxx::result::reference>;
		c container;
		statement statements;
		statements.emplace_back("n_player_id","=",std::to_string(player_id),true,false);
		statements.emplace_back("n_name","=",notch.data(),false,false);
		mods::orm::util::load_where<c,sql_compositor,statement>(
		    container,
		    table_name(),
		    statements
		);
		if(container.size() == 0) {
			std::cerr << "couldn't find anything. inserting...\n";
			auto status = this->create(this);
			if(ORM_SUCCESS(status)) {
				loaded = 1;
				id = std::get<2>(status);
				n_player_id = container[0]["id"].as<uint64_t>();
				n_name = container[0]["n_name"].c_str();
				n_points = container[0]["n_points"].as<uint16_t>();
			}
			return {std::get<0>(status),std::get<1>(status)};
		}
		std::cerr << "found a record. loading\n";
		id = container[0]["id"].as<uint64_t>();
		n_player_id = container[0]["n_player_id"].as<uint64_t>();
		n_name = container[0]["n_name"].c_str();
		n_points = container[0]["n_points"].as<uint16_t>();
		mods::util::maps::dump(export_class());
		return {0,"loaded"};
	}


	std::map<std::string,uint16_t> notch::get_player_levels(const uint64_t& player_id, std::string_view player_class) {
		std::map<std::string,uint16_t> mapped;
		if(ORM_SUCCESS(load_by_player(player_id))) {
			for(const auto& row : rows) {
				mapped[row.n_name] = row.n_points;
			}
		}
		return mapped;
	}

	std::tuple<int16_t,std::string> increment_player_notch(player_ptr_t& player,std::string_view notch) {
		mods::orm::notch n;
		m_debug("start");
		auto status = n.load_by_player_and_notch(player->db_id(),notch);
		++n.n_points;
		m_debug("n_points: " << n.n_points << "\n");
		mods::util::maps::dump(n.export_class());
		m_debug("end of dump");
		return n.update(&n);
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
