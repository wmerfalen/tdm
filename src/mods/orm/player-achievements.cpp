#include "player-achievements.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> player_achievements::delete_by_player(const uint64_t& player_id) {
		return mods::orm::util::delete_where<player_achievements,sql_compositor>(
		           table_name(),
		           "pa_player_id",
		           "=",
		           std::to_string(player_id)
		       );
	}
	int16_t player_achievements::save() {
		for(auto& record : this->rows) {
			this->name = record.name;
			this->points = record.points;
			this->player_id = record.player_id;
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<player_achievements_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving player_achievements:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	std::string player_achievements::primary_key_value() {
		return std::to_string(this->primary_key_id);
	}

	void player_achievements::populate(const uint64_t& player_id,const std::map<std::string,uint16_t>& data) {
		for(const auto& pair : data) {
			player_achievements_record_t record;
			record.name = pair.first;
			record.points = pair.second;
			record.player_id = player_id;
			rows.emplace_back(record);
		}
	}

	strmap_t player_achievements::export_class() {
		strmap_t values;
		values["pa_name"] = this->name;
		values["pa_points"] = std::to_string(this->points);
		values["pa_player_id"] = std::to_string(this->player_id);
		return std::move(values);
	}

	void player_achievements::init() {
		id = 0;
		loaded = 0;
		primary_key_id = 0;
	}

	void player_achievements::feed_multi(pqxx::result& in_rows) {
		player_achievements_record_t r;
		for(auto row : in_rows) {
			r.id = row["id"].as<uint64_t>();
			r.name = row["pa_name"].c_str();
			r.points = row["pa_points"].as<int>();
			r.player_id = row["pa_player_id"].as<uint64_t>();
			rows.emplace_back(r);
		}
	}
	std::tuple<int16_t,std::string> player_achievements::load_by_player(const uint64_t& player_id) {
		return mods::orm::util::load_multi_by_column<player_achievements,sql_compositor>(
		           this,
		           "pa_player_id",
		           std::to_string(player_id)
		       );
	}

	std::map<std::string,uint16_t> player_achievements::get_player_levels(const uint64_t& player_id, std::string_view player_class) {
		std::map<std::string,uint16_t> mapped;
		if(ORM_SUCCESS(load_by_player(player_id))) {
			for(const auto& row : rows) {
				mapped[row.name] = row.points;
			}
		}
		return mapped;
	}

	std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<std::string,uint16_t>* storage) {
		int16_t count = 0;
		std::string message = "";
		player_achievements data;
		data.load_by_player(player->db_id());
		for(auto& row : data.rows) {
			++count;
			(*storage)[row.name] = row.points;
		}
		return {count,message};
	}
};
