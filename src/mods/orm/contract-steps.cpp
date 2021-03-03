#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> contract_steps::delete_by_contract_vnum(const contract_vnum_t& in_contract_vnum) {
		return mods::orm::util::delete_where<contract_steps,sql_compositor>(
		           table_name(),
		           "s_contract_vnum",
		           "=",
		           std::to_string(in_contract_vnum)
		       );
	}
	int16_t contract_steps::save() {
		for(auto& record : rows) {
			s_contract_vnum = record.s_contract_vnum;
			s_task_type = record.s_task_type;
			s_task_target = record.s_task_target;
			s_description = record.s_description;
			s_mob_vnum = record.s_mob_vnum;
			s_room_vnum = record.s_room_vnum;
			s_quota = record.s_quota;
			s_is_optional = record.s_is_optional;
			s_order = record.s_order;
			s_object_yaml = record.s_object_yaml;
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<contract_steps_record_t,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving contract_steps:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	std::string contract_steps::primary_key_value() {
		return std::to_string(id);
	}
	strmap_t contract_steps::export_class() {
		strmap_t v;
		v["s_contract_vnum"] = std::to_string(s_contract_vnum);
		v["s_task_type"] = std::to_string(s_task_type);
		v["s_task_target"] = std::to_string(s_task_target);
		v["s_description"] = s_description;
		v["s_mob_vnum"] = std::to_string(s_mob_vnum);
		v["s_room_vnum"] = std::to_string(s_room_vnum);
		v["s_quota"] = std::to_string(s_quota);
		v["s_is_optional"] = std::to_string(s_is_optional);
		v["s_order"] = std::to_string(s_order);
		v["s_object_yaml"] = s_object_yaml;
		return std::move(v);
	}


	void contract_steps::init() {
		id = 0;
		loaded = 0;
		created_at = 0;
		updated_at = 0;
	}

	std::tuple<int16_t,std::string> contract_steps::load_by_contract_vnum(const contract_vnum_t& contract_vnum) {
		//mods::contracts::contract c;
		auto result = db_get_by_meta(contract_steps_table_name.data(),"s_contract_vnum",std::to_string(contract_vnum));
		if(result.size() == 0) {
			return {0,"no results"};
		}

		for(auto&& row : result) {
			std::cerr << green_str("contract_steps filling contract:") << row["s_contract_vnum"].as<int>() << "\n";
			rows.emplace_back();
			auto& step = rows.back();
			step.s_task_type = (mods::contracts::contract_step::task_type_t)row["s_task_type"].as<uint16_t>();
			step.s_task_target = (mods::contracts::contract_step::task_target_t)row["s_task_target"].as<uint16_t>();
			step.s_description = row["s_description"].c_str();
			step.s_mob_vnum = row["s_mob_vnum"].as<mob_vnum>();
			step.s_object_yaml = row["s_object_yaml"].c_str();
			step.s_room_vnum = row["s_room_vnum"].as<room_vnum>();
			step.s_quota = row["s_quota"].as<int>();
			step.s_is_optional = row["s_is_optional"].as<bool>();
		}
		return {result.size(),"loaded"};
	}

#if 0
	std::map<uint32_t,uint16_t> contract_steps::get_player_levels(const uint64_t& player_id, std::string_view player_class) {
		std::map<uint32_t,uint16_t> mapped;
		if(ORM_SUCCESS(load_by_player(player_id))) {
			for(const auto& row : rows) {
				mapped[row.skill_id] = row.skill_points;
			}
		}
		return mapped;
	}

	std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<uint32_t,uint16_t>* storage) {
		int16_t count = 0;
		std::string message = "";
		contract_steps data;
		data.load_by_player(player->db_id());
		for(auto& row : data.rows) {
			++count;
			(*storage)[row.skill_id] = row.skill_points;
		}
		return {count,message};
	}
#endif

};
