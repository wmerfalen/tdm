#include "scripted-step.hpp"
#include "scripted-sequences.hpp"
#include "interpreter.hpp"
#include "pfind.hpp"
#include "builder/encode.hpp"
#include "orm/scripted-sequences.hpp"

#ifdef  __MENTOC_SHOW_CONTRACT_OUTPUT__
#define dbg_print(a) std::cerr << "[mods::scripted_sequencess][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dbg_print(a)
#endif
#define DX(A) std::cerr << red_str("DEBUG: ") << A << "\n";
namespace mods {
	/**
	 *
	 * Data storage
	 *
	 */
	std::deque<std::shared_ptr<scripted_sequences>>& scripted_sequences_master_list() {
		static std::deque<std::shared_ptr<scripted_sequences>> list;
		return list;
	}

	/**
	 *
	 * Data querying
	 *
	 */
	std::optional<std::shared_ptr<scripted_sequences>> find_scripted_sequences(const scripted_sequences_vnum_t& scripted_sequences_vnum) {
		for(const auto& c : scripted_sequences_master_list()) {
			if(c->vnum == scripted_sequences_vnum) {
				return c;
			}
		}
		return std::nullopt;
	}
	/**
	 * You should load all orm objects before calling this function
	 */
	std::tuple<int32_t,std::string> compile_scripted_data() {
		using namespace mods::scripted_sequence_events;

		std::size_t c_loaded = 0, seq_loaded = 0, step_loaded = 0;
		for(const auto& contract : mods::orm::contract_step_callback_list()) {
			contracts_with_callbacks.emplace_back(contract->s_contract_vnum);
			contract_sequences[contract->s_contract_vnum].emplace_back(contract->s_sequence_vnum);
			sequence_criteria[contract->s_sequence_vnum].emplace_back(
			    std::make_tuple<>(
			        mods::contracts::task_string_map()[contract->s_task_type],
			        mods::contracts::target_string_map()[contract->s_task_target],
			        contract->s_task_vnum
			    )
			);
			c_loaded++;
		}

		std::deque<std::shared_ptr<mods::orm::scripted_step>> step_list;
		for(const auto& sequence : mods::orm::scripted_sequences_list()) {
			step_list.clear();
			auto seq = std::make_shared<mods::scripted_sequences>();
			seq->vnum = sequence->s_sequence_vnum;
			mods::orm::gather_scripted_steps_by_sequence_vnum(
			    sequence->s_sequence_vnum,
			    &step_list
			);
			for(const auto& step : step_list) {
				auto s = std::make_shared<mods::scripted_step>();
				s->wait_ticks = step->s_wait_ticks;
				s->dialogue.assign(step->s_dialogue);
				s->mob = step->s_mob;
				s->obj = step->s_obj;
				s->room = step->s_room;
				s->yaml.assign(step->s_yaml);
				s->type.assign(step->s_type);
				s->interpret.clear(); /** FIXME TODO */
				s->quantity = step->s_quantity;
				s->vnum = step->s_sequence_vnum;
				s->order = step->s_order;
				seq->steps.emplace_back(std::move(s));
				step_loaded++;
			}
			scripted_sequences_master_list().emplace_back(std::move(seq));
			seq_loaded++;
		}

		return {1,CAT(c_loaded," contract step callbacks loaded. ",seq_loaded," sequences loaded. ",step_loaded, " steps loaded")};
	}

	namespace scripted_sequences_init {
		void init() {
			mods::orm::load_all_scripted_data();
			auto s = compile_scripted_data();
			log(CAT("Compiled scripted data: '",std::get<1>(s),"'").c_str());
		}
	};

};
