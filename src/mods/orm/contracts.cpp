#include "contracts.hpp"
#include "contract-steps.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	int16_t contracts::feed(const pqxx::result::reference& row) {
		init();
		id = row["id"].as<uint64_t>();
		c_title = row["c_title"].c_str();
		c_description = row["c_description"].c_str();
		c_vnum = row["c_vnum"].as<contract_vnum_t>();
		//updated_at = row["updated_at"].as<int>();
		//created_at = row["created_at"].as<int>();
		loaded = 1;
		return 0;
	}
	void contracts::destroy() {
		this->destroy_status = mods::orm::util::delete_where<contracts,sql_compositor>(
		                           table_name(),
		                           primary_key_name(),
		                           "=",
		                           primary_key_value()
		                       );
	}

	uint64_t contracts::initialize_row(const contract_vnum_t& inc_vnum,std::string_view title, std::string_view desc) {
		init();
		this->c_title = title;
		this->c_vnum = inc_vnum;
		this->c_description = desc;
		auto status = this->create<contracts>(this);
		if(ORM_SUCCESS(status)) {
			updated_at = created_at = time(nullptr);
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	std::tuple<int16_t,std::string> contracts::delete_by_contract_vnum(const contract_vnum_t& in_c_vnum) {
		return mods::orm::util::delete_where<contracts,sql_compositor>(
		           table_name(),
		           "c_vnum",
		           "=",
		           std::to_string(in_c_vnum)
		       );
	}
	std::tuple<int16_t,std::string> contracts::update() {
		return mods::orm::util::update<contracts,sql_compositor>(this);
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

	/**
	 * ========================================================
	 * Please take notice:
	 * -------------------
	 *  This function loads into a pointer of type mods::contracts::contract.
	 *  This is __NOT__ an ORM object!
	 * ========================================================
	 */
	std::tuple<int16_t,std::string> load_all_non_orm_contracts(std::deque<std::shared_ptr<mods::contracts::contract>>* list) {
		auto count = 0;
		auto message = "";
		auto result = db_get_all(contracts_table_name.data());
		if(result.size() == 0) {
			return {0,""};
		}
		for(const auto& row : result) {
			list->emplace_back(std::make_shared<mods::contracts::contract>(row["c_vnum"].as<uint64_t>(),row["c_title"].c_str(),row["c_description"].c_str()));
			auto& ref = list->back();
			mods::orm::contract_steps steps(row["c_vnum"].as<uint64_t>());
			for(auto& row : steps.rows) {
				ref->steps.emplace_back();
				auto& r = ref->steps.back();
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
	std::deque<std::shared_ptr<mods::orm::contracts>>& orm_contract_list() {
		static std::deque<std::shared_ptr<mods::orm::contracts>> list;
		return list;
	}
};
