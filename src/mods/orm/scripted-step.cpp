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
	uint64_t scripted_step::initialize_row(const sequence_vnum_t& i_vnum,std::string_view type) {
		init();
		s_sequence_vnum = i_vnum;
		s_type = type.data();
		s_order = 0;
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
	int16_t scripted_step::save() {
		auto s = create(this);
		if(ORM_SUCCESS(s)) {
			id = std::get<2>(s);
		} else {
			std::cerr << red_str("Issue saving scripted_step:'") << std::get<1>(s) << "'\n";
		}
		return 0;
	}
	int16_t scripted_sequences::save() {
		auto s = create(this);
		for(auto& record : rows) {
			record.save();
		}
		return 0;
	}
	std::string scripted_step::primary_key_value() {
		return std::to_string(id);
	}

	std::tuple<int16_t,std::string> scripted_sequences::load_by_sequence_vnum(const sequence_vnum_t& scripted_vnum) {
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
	scripted_step::scripted_step(pqxx::row& row) {
		init();
		feed(row);
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
		s_sequence_vnum = i_vnum;
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

	contract_step_callback::~contract_step_callback() {

	}
	contract_step_callback::contract_step_callback() {
		init();
		loaded = 0;
	}
	std::string contract_step_callback::primary_key_value() {
		return std::to_string(id);
	}
	contract_step_callback::contract_step_callback(contract_vnum_t contract, std::string_view type, std::string_view task, task_vnum_t task_vnum, sequence_vnum_t sequence_vnum) {
		initialize_row(contract, type, task, task_vnum, sequence_vnum);
	}
	uint64_t contract_step_callback::initialize_row(contract_vnum_t contract, std::string_view type, std::string_view task, task_vnum_t task_vnum, sequence_vnum_t sequence_vnum) {
		init();
		s_contract_vnum = contract;
		s_sequence_vnum = sequence_vnum;
		s_task_vnum = task_vnum;
		s_task_type = type.data();
		s_task_target = task.data();

		auto status = this->create<contract_step_callback>(this);
		if(ORM_SUCCESS(status)) {
			loaded = 1;
			id = std::get<2>(status);
		}
		return id;

	}

	/**
	 * ====================================
	 * ====================================
	 * Globally available loading functions
	 * ====================================
	 * ====================================
	 */
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

	std::tuple<int16_t,std::string> load_all_contract_step_callbacks() {
		int16_t ctr = 0;
		mods::orm::contract_step_callback_list() = std::move(mods::orm::load_all_by_table<mods::orm::contract_step_callback>());
		return {ctr,"Loaded"};
	}

	std::tuple<int16_t,std::string> load_all_scripted_sequences() {
		int16_t ctr = 0;
		mods::orm::scripted_sequences_list() = std::move(mods::orm::load_all_by_table<mods::orm::scripted_sequences>());
		return {ctr,"Loaded"};
	}

	std::tuple<int16_t,std::string> load_all_scripted_steps() {
		int16_t ctr = 0;
		mods::orm::scripted_step_list() = std::move(mods::orm::load_all_by_table<mods::orm::scripted_step>());
		return {ctr,"Loaded"};
	}
	std::tuple<int16_t,std::string> load_all_scripted_data() {
		load_all_contract_step_callbacks();
		load_all_scripted_sequences();
		load_all_scripted_steps();
		return {1,"stub"};
	}

	std::tuple<int16_t,std::string> gather_scripted_steps_by_sequence_vnum(
	    const sequence_vnum_t& sequence_vnum,
	    std::deque<std::shared_ptr<mods::orm::scripted_step>>* in_list_ptr
	) {
		using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
		return mods::orm::util::foreach_load_by_column_order_by<sql_compositor>(
		           scripted_step::table,
		           "s_sequence_vnum",
		           std::to_string(sequence_vnum),
		           "s_order",
		           "ASC",
		[&](pqxx::row row) -> void {
			in_list_ptr->emplace_back(std::make_shared<mods::orm::scripted_step>(row));
		}
		       );
	}


};
