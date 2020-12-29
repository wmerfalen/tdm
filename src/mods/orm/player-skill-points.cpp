#include "player-skill-points.hpp"
#include "util.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> player_skill_points::delete_by_player(const uint64_t& player_id) {
		return mods::orm::util::delete_where<player_skill_points,sql_compositor>(
		           table_name(),
		           "sp_player_id",
		           "=",
		           std::to_string(player_id)
		       );
	}
	int16_t player_skill_points::save() {
		for(auto& record : this->rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<player_skill_points_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving player_skill_points:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}

	void player_skill_points::populate(const uint64_t& player_id,const std::map<uint32_t,uint16_t>* data) {
		for(const auto& pair : *data) {
			player_skill_points_record_t record;
			record.skill_id = pair.first;
			record.skill_points = pair.second;
			record.player_id = player_id;
			rows.emplace_back(record);
		}
	}

	strmap_t player_skill_points::export_class() {
		strmap_t values;
		return std::move(values);
	}

	void player_skill_points::init() {
		id = 0;
		loaded = 0;
	}

	void player_skill_points::feed_multi(pqxx::result& in_rows) {
		player_skill_points_record_t r;
		for(auto row : in_rows) {
			r.skill_id = row["ps_skill_id"].as<int>();
			r.skill_points = row["ps_points"].as<int>();
			r.player_id = row["ps_player_id"].as<int>();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> player_skill_points::load_by_player(const uint64_t& player_id) {
		return mods::orm::util::load_by_column<player_skill_points,sql_compositor>(this,
		                                                                           "ps_player_id",
		                                                                           std::to_string(player_id)
		                                                                          );
	}

	std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<uint32_t,uint16_t>* storage) {
		int16_t count = 0;
		std::string message = "";
		player_skill_points data;
		data.load_by_player(player->db_id());
		for(auto& row : data.rows) {
			++count;
			(*storage)[row.skill_id] = row.skill_points;
		}
		return {count,message};
	}
};
