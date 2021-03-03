#include "contracts.hpp"
#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> contracts::delete_by_contract_vnum(const contract_vnum_t& in_c_vnum) {
		return mods::orm::util::delete_where<contracts,sql_compositor>(
		           table_name(),
		           "c_vnum",
		           "=",
		           std::to_string(in_c_vnum)
		       );
	}
	int16_t contracts::save() {
		std::tuple<int16_t,std::string,uint64_t> insert_result =
		    mods::orm::util::insert_returning<contracts,sql_compositor>(
		        this,
		        "id"
		    );
		if(!ORM_SUCCESS(insert_result)) {
			std::cerr << red_str("Issue saving contracts:'") << std::get<1>(insert_result) << "'\n";
		}
		return 0;
	}
	std::string contracts::primary_key_value() {
		return std::to_string(id);
	}

	void contracts::init() {
		id = 0;
		loaded = 0;
		c_vnum = 0;
		c_description.clear();
		c_title.clear();
		created_at = updated_at = 0;
	}

	std::tuple<int16_t,std::string> load_all_contracts(std::deque<mods::contracts::contract>* list) {
		auto count = 0;
		auto message = "";
		auto result = db_get_all(contracts_table_name.data());
		if(result.size() == 0) {
			return {0,""};
		}
		for(const auto& row : result) {
			list->emplace_back(row["c_vnum"].as<uint64_t>(),row["c_title"].c_str(),row["c_description"].c_str());
			auto& ref = list->back();
			mods::orm::contract_steps steps(row["c_vnum"].as<uint64_t>());
			for(auto& row : steps.rows) {
				ref.steps.emplace_back();
				auto& r = ref.steps.back();
				r.goal = (mods::contracts::contract_step::task_type_t)row.s_task_type;
				r.target = (mods::contracts::contract_step::task_target_t)row.s_task_target;
				r.description = row.s_description;
				r.mob_vnum = row.s_mob_vnum;
				r.mob_uuid = 0;//TODO FIXME
				r.object_yaml = row.s_object_yaml;
				r.object_uuid = 0;//TODO FIXME
				r.room = row.s_room_vnum;
				r.quota = row.s_quota;
				r.is_optional = row.s_is_optional;
			}
			++count;
		}
		return {count,message};
	}
};
