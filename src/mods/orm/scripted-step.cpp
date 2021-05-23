#include "scripted-step.hpp"
#include "util.hpp"
#include "../player.hpp"

namespace mods::orm {
	std::tuple<int16_t,std::string> scripted_step::update_row() {
		auto status = this->update<scripted_step>(this);
		if(ORM_SUCCESS(status)) {
			loaded = 1;
		}
		return status;
	}
	uint64_t scripted_step::initialize_row(const sequence_vnum_t& i_vnum) {
		init();
		auto status = this->create<scripted_step>(this);
		if(ORM_SUCCESS(status)) {
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	void scripted_step::destroy() {
		this->destroy_status = mods::orm::util::delete_where<scripted_step,sql_compositor>(
		                           table_name(),
		                           primary_key_name(),
		                           "=",
		                           primary_key_value()
		                       );
	}

	std::tuple<int16_t,std::string> scripted_step::delete_by_scripted_vnum(const sequence_vnum_t& in_scripted_vnum) {
		return mods::orm::util::delete_where<scripted_step,sql_compositor>(
		           table_name(),
		           "s_scripted_vnum",
		           "=",
		           std::to_string(in_scripted_vnum)
		       );
	}
	int16_t scripted_sequences::save() {
		for(auto& record : rows) {
			std::tuple<int16_t,std::string,uint64_t> insert_result = mods::orm::util::insert_returning<scripted_step,sql_compositor>(&record, "id");
			if(!ORM_SUCCESS(insert_result)) {
				std::cerr << red_str("Issue saving scripted_step:'") << std::get<1>(insert_result) << "'\n";
			}
		}
		return 0;
	}
	std::string scripted_step::primary_key_value() {
		return std::to_string(id);
	}

	std::tuple<int16_t,std::string> scripted_sequences::load_by_sequence_vnum(const sequence_vnum_t& scripted_vnum) {
		//mods::scripteds::scripted c;
		auto result = db_get_by_meta(table_name(),"s_sequence_vnum",std::to_string(scripted_vnum));
		if(result.size() == 0) {
			return {0,"no results"};
		}

		for(auto&& row : result) {
			std::cerr << green_str("scripted_step filling scripted:") << row["s_scripted_vnum"].as<int>() << "\n";
			rows.emplace_back(row);
		}
		return {result.size(),"loaded"};
	}
	std::deque<std::shared_ptr<scripted_step>>& scripted_steps_list() {
		static std::deque<std::shared_ptr<scripted_step>> list;
		return list;

	}
	std::tuple<int16_t,std::string> gather_scripted_steps_by_scripted_vnum(const sequence_vnum_t& scripted_vnum,std::deque<std::shared_ptr<mods::orm::scripted_step>>* in_list_ptr) {
		auto result = db_get_by_meta(scripted_step::table_name_value,"s_scripted_vnum",std::to_string(scripted_vnum));
		if(result.size() == 0) {
			return {0,"no results"};
		}

		for(auto&& row : result) {
			std::cerr << green_str("scripted_step filling scripted:") << row["s_scripted_vnum"].as<int>() << "\n";

			in_list_ptr->emplace_back(std::make_shared<mods::orm::scripted_step>(row));
		}
		return {result.size(),"loaded"};

	}


	/**
	 * ========================
	 * scripted sequences class
	 * ========================
	 */
	void scripted_sequences::destroy() {
		this->destroy_status = mods::orm::util::delete_where<scripted_sequences,sql_compositor>(
		                           table_name(),
		                           primary_key_name(),
		                           "=",
		                           primary_key_value()
		                       );
	}
	scripted_sequences::scripted_sequences() {
		init();
		loaded = 0;
		id = 0;
	}
	scripted_sequences::scripted_sequences(const sequence_vnum_t& scripted_vnum) {
		auto s = load_by_sequence_vnum(scripted_vnum);
		std::cerr << "scripted_sequences(scripted_vnum) constructor status: " << std::get<0>(s) << "|'" << std::get<1>(s) << "'\n";
	}
	uint64_t scripted_sequences::initialize_row(const sequence_vnum_t& i_vnum) {
		init();
		auto status = this->create<scripted_sequences>(this);
		if(ORM_SUCCESS(status)) {
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;
	}
	scripted_sequences::~scripted_sequences() {

	}
	std::string scripted_sequences::primary_key_value() {
		return std::to_string(id);
	}

	std::deque<std::shared_ptr<scripted_sequences>>& scripted_sequences_list() {
		static std::deque<std::shared_ptr<scripted_sequences>> list;
		return list;

	}
	std::deque<std::shared_ptr<scripted_step>>& scripted_step_list() {
		static std::deque<std::shared_ptr<scripted_step>> list;
		return list;
	}
	std::deque<std::shared_ptr<mods::orm::contract_step_callback>>& contract_step_callback_list() {
		static std::deque<std::shared_ptr<mods::orm::contract_step_callback>> list;
		return list;

	}

	std::tuple<int16_t,std::string> gather_scripted_steps_by_sequence_vnum(const sequence_vnum_t& scripted_vnum,std::deque<std::shared_ptr<mods::orm::scripted_sequences>>* list) {
		return {1,"stub"};
	}


	std::tuple<int16_t,std::string> load_all_contract_step_callbacks() {
		int16_t ctr = 0;
		contract_step_callback_list().clear();
		for(const auto& cb :  db_get_all(contract_step_callback::table_name_value)) {
			auto& item = contract_step_callback_list().emplace_back();
			item->feed(cb);
			++ctr;
		}
		return {ctr,"sub"};
	}

	std::tuple<int16_t,std::string> load_all_scripted_sequences() {
		int16_t ctr = 0;
		scripted_sequences_list().clear();
		for(const auto& cb :  db_get_all(scripted_sequences::table_name_value)) {
			auto& item = scripted_sequences_list().emplace_back();
			item->feed(cb);
			++ctr;
		}
		return {ctr,"sub"};
	}

	std::tuple<int16_t,std::string> load_all_scripted_steps() {
		int16_t ctr = 0;
		scripted_step_list().clear();
		for(const auto& cb :  db_get_all(scripted_step::table_name_value)) {
			auto& item = scripted_step_list().emplace_back();
			item->feed(cb);
		}
		return {ctr,"sub"};
	}

	std::tuple<int16_t,std::string> load_all_scripted_data() {
		load_all_contract_step_callbacks();
		load_all_scripted_steps();
		load_all_scripted_sequences();
		return {1,"Loaded"};
	}

};
