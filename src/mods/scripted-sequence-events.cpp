#include "scripted-sequence-events.hpp"
#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"

#ifdef __MENTOC_DEBUG_EVENT_MATCHES__
#define dbg_print(A) *player << "{grn}event_matching_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::scripted_sequence_events {
	std::vector<contract_vnum_t> contracts_with_callbacks;
	std::map<contract_vnum_t,std::vector<sequence_vnum_t>> contract_sequences;
	std::map<sequence_vnum_t,std::vector<std::tuple<task_type_t,task_target_t,uint32_t>>> sequence_criteria;

	static constexpr std::size_t TASK_TYPE_INDEX = 0;
	static constexpr std::size_t TASK_TARGET_INDEX = 1;
	static constexpr std::size_t TASK_VNUM_INDEX = 2;


	void player_finished_step(player_ptr_t& player,const contract_vnum_t& contract_vnum,step_t* step) {
		auto it = std::find(contracts_with_callbacks.begin(),contracts_with_callbacks.end(),contract_vnum);
		if(it == contracts_with_callbacks.end()) {
			return;
		}
		bool dispatch = false;
		for(const auto& sequence_vnum : contract_sequences[contract_vnum]) {
			for(const auto& criteria : sequence_criteria[sequence_vnum]) {
				dispatch = false;
				if((step->goal & std::get<TASK_TYPE_INDEX>(criteria)) &&
				        (step->target == std::get<TASK_TARGET_INDEX>(criteria))) {
					auto& vnum = std::get<TASK_VNUM_INDEX>(criteria);
					switch(step->target) {
						case task_target_t::TARGET_MOB:
							if(step->mob_vnum == vnum) {
								dbg_print("Target type mob found in sequence events");
								dispatch = true;
							}
							break;
						case task_target_t::TARGET_ITEM: {
								auto obj = optr_by_uuid(step->object_uuid);
								if(obj && obj->item_number == vnum) {
									dbg_print("Target type found by object uuid");
									dispatch = true;
								}
							}
							break;
						case task_target_t::TARGET_ROOM:
							if(step->room == vnum) {
								dbg_print("Target type room found in sequence events");
								dispatch = true;
							}
							break;
						case task_target_t::TARGET_DOOR:
							if(step->room == vnum) {
								dbg_print("Target type door found in sequence events");
								dispatch = true;
							}
							break;
						default:
							dbg_print("Unknown type found in sequence events");
							break;
					}
					if(dispatch) {
						mods::scripted_sequence_runner::dispatch(player,sequence_vnum,contract_vnum,*step);
						return;
					}
				}
			}
		}
	}

	void init() {

	}

};
