#include "player-skill-usage.hpp"
#include "skill-trees.hpp"
#include "util.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> player_skill_usage::delete_by_player(const uint64_t& player_id) {
		return mods::orm::util::delete_where<player_skill_usage,sql_compositor>(
		           table_name(),
		           "ps_player_id",
		           "=",
		           std::to_string(player_id)
		       );
	}
	int16_t player_skill_usage::save() {
		for(auto& record : this->rows) {
			this->skill_id = record.skill_id;
			this->usage_count = record.usage_count;
			this->player_id = record.player_id;
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<player_skill_usage_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving player_skill_usage:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	std::string player_skill_usage::primary_key_value() {
		return std::to_string(this->primary_key_id);
	}

	void player_skill_usage::populate(const uint64_t& player_id,const std::map<uint64_t,uint16_t>& data) {
		for(const auto& pair : data) {
			player_skill_usage_record_t record;
			record.skill_id = pair.first;
			record.usage_count = pair.second;
			record.player_id = player_id;
			rows.emplace_back(record);
		}
	}

	strmap_t player_skill_usage::export_class() {
		strmap_t values;
		values["ps_skill_id"] = std::to_string(this->skill_id);
		values["ps_usage_count"] = std::to_string(this->usage_count);
		values["ps_player_id"] = std::to_string(this->player_id);
		return std::move(values);
	}

	void player_skill_usage::init() {
		id = 0;
		loaded = 0;
		primary_key_id = 0;
		rows.clear();
	}

	void player_skill_usage::feed_multi(pqxx::result& in_rows) {
		player_skill_usage_record_t r;
		for(auto row : in_rows) {
			r.id = row["id"].as<uint64_t>();
			r.skill_id = row["ps_skill_id"].as<int>();
			r.usage_count = row["ps_usage_count"].as<int>();
			r.player_id = row["ps_player_id"].as<int>();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> player_skill_usage::load_by_player(const uint64_t& player_id) {
		return mods::orm::util::load_multi_by_column<player_skill_usage,sql_compositor>(
		           this,
		           "ps_player_id",
		           std::to_string(player_id)
		       );
	}

	std::map<uint64_t,uint16_t> player_skill_usage::get_player_levels(const uint64_t& player_id, std::string_view player_class) {
		std::map<uint64_t,uint16_t> mapped;
		if(ORM_SUCCESS(load_by_player(player_id))) {
			for(const auto& row : rows) {
				mapped[row.skill_id] = row.usage_count;
			}
		}
		return mapped;
	}

	std::tuple<int16_t,std::string> sync_player_skill_usage(const uint64_t& player_id, std::map<uint64_t,uint16_t>& usage) {
		return {0,""};
	}
	std::tuple<int16_t,std::string> load_player_skill_usage_data(player_ptr_t& player, std::map<uint64_t,uint16_t>* storage) {
		int16_t count = 0;
		std::string message = "";
		player_skill_usage data;
		data.load_by_player(player->db_id());
		for(auto& row : data.rows) {
			++count;
			(*storage)[row.skill_id] = row.usage_count;
		}
		return {count,message};
	}
	void player_skill_usage_upkeep(const uint64_t& player_id, std::string_view in_class) {
		std::string c = "";
		for(auto& ch : in_class) {
			c += std::tolower(ch);
		}
		skill_trees s;
		s.load_by_class(c);
		player_skill_usage psu;
		auto psu_rows = psu.get_player_levels(player_id, c);
		for(const auto& row : s.rows) {
			std::cerr << green_str("checking s.rows:") << row.id << "\n";
			if(!psu_rows[row.id]) {
				std::cerr << green_str("Fixing unfound row.id:") << row.id << "\n";
				psu_rows[row.id] = 1;
			}
		}
		psu.delete_by_player(player_id);
		psu.init();
		psu.populate(player_id,psu_rows);
		psu.save();
	}
};
