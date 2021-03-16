#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> contract_steps::update_row() {
		auto status = this->update<contract_steps>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = time(nullptr);
			loaded = 1;
		}
		return status;
	}
	uint64_t contract_steps::initialize_row(const contract_vnum_t& i_vnum) {
		init();
		s_contract_vnum = i_vnum;
		s_task_type = 0;
		s_task_target = 0;
		s_description = "description";
		s_mob_vnum = 0;
		s_room_vnum = 0;
		s_quota = 0;
		s_is_optional = 0;
		s_order = 0;
		s_reward_xp = 0;
		s_reward_money = 0;
		s_reward_1.clear();
		s_reward_2.clear();
		s_reward_3.clear();
		s_reward_4.clear();
		s_reward_5.clear();
		s_reward_6.clear();
		s_reward_7.clear();
		s_reward_8.clear();
		s_reward_9.clear();
		s_reward_10.clear();
		auto status = this->create<contract_steps>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	int16_t contract_steps::feed(const pqxx::result::reference& row) {
		init();
		id = row["id"].as<uint64_t>();
		s_contract_vnum = row["s_contract_vnum"].as<int>();
		s_task_type = row["s_task_type"].as<int>();
		s_task_target = row["s_task_target"].as<int>();
		s_description = row["s_description"].c_str();
		s_mob_vnum = row["s_mob_vnum"].as<int>();
		s_room_vnum = row["s_room_vnum"].as<int>();
		s_quota = row["s_quota"].as<int>();
		s_is_optional = std::string(row["s_is_optional"].c_str())[0] == 'f' ? false : true;
		s_order = row["s_order"].as<int>();
		s_object_yaml = row["s_object_yaml"].c_str();
		s_reward_xp = NULLABLE_U32("s_reward_xp");
		s_reward_money = NULLABLE_U32("s_reward_money");
		s_reward_1 = row["s_reward_1"].c_str();
		s_reward_2 = row["s_reward_2"].c_str();
		s_reward_3 = row["s_reward_3"].c_str();
		s_reward_4 = row["s_reward_4"].c_str();
		s_reward_5 = row["s_reward_5"].c_str();
		s_reward_6 = row["s_reward_6"].c_str();
		s_reward_7 = row["s_reward_7"].c_str();
		s_reward_8 = row["s_reward_8"].c_str();
		s_reward_9 = row["s_reward_9"].c_str();
		s_reward_10 = row["s_reward_10"].c_str();
		//updated_at = row["updated_at"].as<int>();
		//created_at = row["created_at"].as<int>();
		loaded = 1;
		return 0;
	}
	void contract_steps::destroy() {
		this->destroy_status = mods::orm::util::delete_where<contract_steps,sql_compositor>(
		                           table_name(),
		                           primary_key_name(),
		                           "=",
		                           primary_key_value()
		                       );
	}

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
			s_reward_xp = record.s_reward_xp;
			s_reward_money = record.s_reward_money;
			s_reward_1 = record.s_reward_1;
			s_reward_2 = record.s_reward_2;
			s_reward_3 = record.s_reward_3;
			s_reward_4 = record.s_reward_4;
			s_reward_5 = record.s_reward_5;
			s_reward_6 = record.s_reward_6;
			s_reward_7 = record.s_reward_7;
			s_reward_8 = record.s_reward_8;
			s_reward_9 = record.s_reward_9;
			s_reward_10 = record.s_reward_10;
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
		v["s_reward_xp"] = std::to_string(s_reward_xp);
		v["s_reward_money"] = std::to_string(s_reward_money);
		v["s_reward_1"] = s_reward_1;
		v["s_reward_2"] = s_reward_2;
		v["s_reward_3"] = s_reward_3;
		v["s_reward_4"] = s_reward_4;
		v["s_reward_5"] = s_reward_5;
		v["s_reward_6"] = s_reward_6;
		v["s_reward_7"] = s_reward_7;
		v["s_reward_8"] = s_reward_8;
		v["s_reward_9"] = s_reward_9;
		v["s_reward_10"] = s_reward_10;
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
			step.s_reward_xp = row["s_reward_xp"].as<uint32_t>(0);
			step.s_reward_money = row["s_reward_money"].as<uint32_t>(0);
			step.s_reward_1 = row["s_reward_1"].c_str();
			step.s_reward_2 = row["s_reward_2"].c_str();
			step.s_reward_3 = row["s_reward_3"].c_str();
			step.s_reward_4 = row["s_reward_4"].c_str();
			step.s_reward_5 = row["s_reward_5"].c_str();
			step.s_reward_6 = row["s_reward_6"].c_str();
			step.s_reward_7 = row["s_reward_7"].c_str();
			step.s_reward_8 = row["s_reward_8"].c_str();
			step.s_reward_9 = row["s_reward_9"].c_str();
			step.s_reward_10 = row["s_reward_10"].c_str();
		}
		return {result.size(),"loaded"};
	}
	std::deque<std::shared_ptr<contract_steps>>& contract_steps_list() {
		static std::deque<std::shared_ptr<contract_steps>> list;
		return list;

	}
	std::tuple<int16_t,std::string> gather_contract_steps_by_contract_vnum(const contract_vnum_t& contract_vnum,std::deque<std::shared_ptr<mods::orm::contract_steps>>* in_list_ptr) {
		auto result = db_get_by_meta(contract_steps_table_name.data(),"s_contract_vnum",std::to_string(contract_vnum));
		if(result.size() == 0) {
			return {0,"no results"};
		}

		for(auto&& row : result) {
			std::cerr << green_str("contract_steps filling contract:") << row["s_contract_vnum"].as<int>() << "\n";
			auto step = std::make_shared<mods::orm::contract_steps>();
			step->id = row["id"].as<uint64_t>();
			step->s_contract_vnum = row["s_contract_vnum"].as<int>();
			step->s_task_type = (mods::contracts::contract_step::task_type_t)row["s_task_type"].as<uint16_t>();
			step->s_task_target = (mods::contracts::contract_step::task_target_t)row["s_task_target"].as<uint16_t>();
			step->s_description = row["s_description"].c_str();
			step->s_mob_vnum = row["s_mob_vnum"].as<mob_vnum>();
			step->s_object_yaml = row["s_object_yaml"].c_str();
			step->s_room_vnum = row["s_room_vnum"].as<room_vnum>();
			step->s_quota = row["s_quota"].as<int>();
			step->s_is_optional = std::string(row["s_is_optional"].c_str())[0] == 'f' ? false : true;
			step->s_reward_xp = row["s_reward_xp"].as<uint32_t>(0);
			step->s_reward_money = row["s_reward_money"].as<uint32_t>(0);
			step->s_reward_1 = row["s_reward_1"].c_str();
			step->s_reward_2 = row["s_reward_2"].c_str();
			step->s_reward_3 = row["s_reward_3"].c_str();
			step->s_reward_4 = row["s_reward_4"].c_str();
			step->s_reward_5 = row["s_reward_5"].c_str();
			step->s_reward_6 = row["s_reward_6"].c_str();
			step->s_reward_7 = row["s_reward_7"].c_str();
			step->s_reward_8 = row["s_reward_8"].c_str();
			step->s_reward_9 = row["s_reward_9"].c_str();
			step->s_reward_10 = row["s_reward_10"].c_str();
			in_list_ptr->emplace_back(std::move(step));
		}
		return {result.size(),"loaded"};

	}
};
