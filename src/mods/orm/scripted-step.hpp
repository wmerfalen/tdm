#ifndef __MENTOC_MODS_ORM_scripted_steps_HEADER__
#define __MENTOC_MODS_ORM_scripted_steps_HEADER__

#include "orm-base.hpp"
#include "../scripted-sequences.hpp"

namespace mods::orm {
	static std::vector<std::string> m_blank_str_vector;
	using strmap_t = std::map<std::string,std::string>;
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	static constexpr std::string_view callback_table_name = "contract_step_callback";
	using task_type_t = uint16_t;
	using task_target_t = uint16_t;
	using quota_t = uint8_t;
	using sequence_vnum_t = uint32_t;
	using task_vnum_t = uint32_t;
	struct scripted_step : public mods::orm::orm_base<scripted_step,std::string> {

		/**
		 * member tuple csv columns:
		 * Member Var Type, Member Var Name, initailized value, pqxx conversion type, native object field, slot list
		 *
		 * Member var type: int, std::string, etc
		 * Member Var Name: id, s_sequence_vnum, etc
		 * initialized value: the value to set it to when ::init() is called
		 * pqxx conversion type: 1 = use .as<var_type>(), 2 = use vectorize, 3 = use .c_str()
		 * native object field: the member variable name of mods::scripted_step
		 * slot list: 1 = list this var in slot_list(), 0 = don't list in slot_list
		 */
#define SCSTEP_MEMBERS_TUPLE ( \
(uint64_t,id,0,1,0,0), \
(sequence_vnum_t,s_sequence_vnum,0,1,vnum,1), \
(uint16_t,s_wait_ticks,0,1,wait_ticks,1), \
(mob_vnum,s_mob,0,1,mob,1), \
(obj_vnum,s_obj,0,1,obj,1), \
(room_vnum,s_room,0,1,room,1), \
(uint16_t,s_quantity,0,1,quantity,1), \
(uint16_t,s_order,0,1,order,1), \
(std::vector<std::string>,s_interpret,m_blank_str_vector,2,interpret,1), \
(std::string,s_yaml,"",3,yaml,1), \
(std::string,s_type,"",3,type,1), \
(std::string,s_dialogue,"",3,dialogue,1) \
)
		MENTOC_ORM_CLASS(SCSTEP_MEMBERS_TUPLE,"scripted_steps");

		std::string primary_key_value();
		uint64_t initialize_row(const sequence_vnum_t& i_vnum);
		void load_multi(const pqxx::result::reference&);

		void destroy();
		std::tuple<int16_t,std::string> destroy_status;

		void feed_multi(pqxx::result&);

		int16_t save();

		std::tuple<int16_t,std::string> delete_by_scripted_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> load_by_scripted_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> update_row();
		scripted_step() {
			init();
			loaded = 0;
		}
		scripted_step(const pqxx::result::reference& row) {
			feed(row);
		}

		operator mods::scripted_step() const {
			mods::scripted_step s;
			s.vnum = s_sequence_vnum;
			s.wait_ticks = s_wait_ticks;
			s.dialogue = s_dialogue;
			s.mob = s_mob;
			s.obj = s_obj;
			s.room = s_room;
			s.yaml = s_yaml;
			s.type = s_type;
			s.interpret = s_interpret;
			s.quantity = s_quantity;
			s.order = s_order;
			return s;
		}
	};

	/**
	 * Explanation of structure relationships
	 *
	 * A SEQUENCE has several steps that execute sequentially. The SEQUENCE
	 * is represented by a 'scripted_sequence' orm object. The sequence has
	 * a 'rows' member variable which are orm objects to the 'scripted_step'.
	 * A scripted_step has a member variable s_sequence_vnum which points to
	 * it's parent scripted_sequences orm object.
	 *
	 * A contract has several steps that a player can accomplish. The player
	 * spawns events every time they complete a step of a contract. Each
	 * contract step can have callbacks. Those callbacks can trigger sequences
	 * which then trigger sequence steps. Callback data is stored in the
	 * contract_step_callback orm structure.
	 *
	 */
	struct scripted_sequences : public mods::orm::orm_base<scripted_sequences,std::string> {
		using primary_choice_t = std::string;
		/**
		 * member tuple csv columns:
		 * Member Var Type, Member Var Name, initailized value, pqxx conversion type, native object field, slot list
		 */
#define SSEQ_MEMBERS_TUPLE ( \
(uint64_t,id,0,1,0,0), \
(sequence_vnum_t,s_sequence_vnum,0,1,vnum,1) \
)

		MENTOC_ORM_CLASS(SSEQ_MEMBERS_TUPLE,"scripted_sequences");

		std::string table_name() const {
			return table_name_value;
		}
		virtual ~scripted_sequences();
		scripted_sequences();
		scripted_sequences(const sequence_vnum_t& scripted_vnum);

		void fill(mods::scripted_sequences& c) {
			c.vnum = s_sequence_vnum;
			for(auto& row : rows) {
				std::cerr << green_str("scripted_sequences filling scripted:") << c.vnum << "\n";
				c.steps.emplace_back(row);
			}
		}

		std::string primary_key_value();

		void load_multi(const pqxx::result::reference&);

		void destroy();
		std::tuple<int16_t,std::string> destroy_status;
		uint64_t initialize_row(const sequence_vnum_t& i_vnum);


		void feed_multi(pqxx::result&);

		int16_t save();

		std::tuple<int16_t,std::string> delete_by_sequence_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> load_by_sequence_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> update_row();

		auto vnum() const {
			return s_sequence_vnum;
		}

		std::vector<scripted_step> rows;
		bool loaded;
	};
	struct contract_step_callback : public mods::orm::orm_base<contract_step_callback,std::string> {
		/**
		 * member tuple csv columns:
		 * Member Var Type, Member Var Name, initailized value, pqxx conversion type, native object field, slot list
		 */
#define CSTEP_MEMBERS_TUPLE ( \
(uint64_t,id,0,1,0,0), \
(contract_vnum_t,s_contract_vnum,0,1,contract_vnum,1), \
(sequence_vnum_t,s_sequence_vnum,0,1,sequence_vnum,1), \
(task_vnum_t,s_task_vnum,0,1,task_vnum,1), \
(std::string,s_task_type,"",3,task_type,1), \
(std::string,s_task_target,"",3,task_target,1) \
)
		MENTOC_ORM_CLASS(CSTEP_MEMBERS_TUPLE,"contract_step_callback");

		contract_step_callback(const sequence_vnum_t& scripted_vnum) {
			auto s = load_by_scripted_vnum(scripted_vnum);
			std::cerr << "contract_step_callback(scripted_vnum) constructor status: " << std::get<0>(s) << "|'" << std::get<1>(s) << "'\n";
		}
		/*
		void fill(mods::contract_step_callback& c) {
			for(auto& row : rows) {
				std::cerr << green_str("contract_step_callback filling scripted:") << c.vnum << "\n";
				c.steps.emplace_back(row);
			}
		}
		*/
		~contract_step_callback() = default;

		std::string primary_key_value();

		void destroy();
		std::tuple<int16_t,std::string> destroy_status;
		uint64_t initialize_row(contract_vnum_t, task_type_t, task_target_t, task_vnum_t task_vnum, sequence_vnum_t sequence_vnum);

		int16_t save();

		std::tuple<int16_t,std::string> delete_by_scripted_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> load_by_scripted_vnum(const sequence_vnum_t& scripted_vnum);
		std::tuple<int16_t,std::string> update_row();

		bool loaded;
	};
	std::deque<std::shared_ptr<mods::orm::contract_step_callback>>& contract_step_callback_list();
	std::deque<std::shared_ptr<mods::orm::scripted_sequences>>& scripted_sequences_list();
	std::deque<std::shared_ptr<mods::orm::scripted_step>>& scripted_step_list();

	std::tuple<int16_t,std::string> load_all_contract_step_callbacks();
	std::tuple<int16_t,std::string> load_all_scripted_sequences();
	std::tuple<int16_t,std::string> load_all_scripted_steps();

	std::tuple<int16_t,std::string> load_all_scripted_data();
	std::tuple<int16_t,std::string> gather_scripted_steps_by_sequence_vnum(const sequence_vnum_t& scripted_vnum,std::deque<std::shared_ptr<mods::orm::scripted_sequences>>* list);
//std::tuple<int16_t,std::string> load_player_skill_data(player_ptr_t& player, std::map<uint32_t,uint16_t>* data);
//std::tuple<int16_t,std::string> sync_player_with_class_skills(const uint64_t& player_id, std::string_view player_class);
};

#endif
