#include "scripted-sequence-events.hpp"
#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"

#define __MENTOC_DEBUG_EVENT_MATCHES__
#ifdef __MENTOC_DEBUG_EVENT_MATCHES__
#define dbg_print(A) *player << "{grn}event_matching_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::scripted_sequence_events {
	using task_type_t = mods::contracts::contract_step::task_type_t;
	using task_target_t = mods::contracts::contract_step::task_target_t;
	using sequence_vnum_t = mods::scripted_sequences_vnum_t;
	static std::vector<contract_vnum_t> contracts_with_callbacks;
	static std::map<contract_vnum_t,std::vector<sequence_vnum_t>> contract_sequences;
	static std::map<sequence_vnum_t,std::vector<std::tuple<task_type_t,task_target_t,uint32_t>>> sequence_criteria;

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

	SUPERCMD(do_finish_step) {
		step_t step;
		step.goal = task_type_t::GOAL_FIND;
		step.target = task_target_t::TARGET_MOB;
		step.mob_vnum = 405;
		player_finished_step(player,1,&step);
		player->sendln("This next one should fail...");
		step.goal = task_type_t::GOAL_FIND;
		step.target = task_target_t::TARGET_MOB;
		step.mob_vnum = 4;
		player_finished_step(player,1,&step);
		player->sendln("Did you see a message for that one? If so, it's {red}BROKEN{/red}.");
		player->sendln("This next one should print. (mob:406)...");
		step.goal = task_type_t::GOAL_FIND;
		step.target = task_target_t::TARGET_MOB;
		step.mob_vnum = 406;
		player_finished_step(player,1,&step);
		player->sendln("Did you see a message for that one? If so, it's {grn}WORKING{/grn}.");
		player->sendln("This next one should print. (mob:407)...");
		step.goal = task_type_t::GOAL_FIND;
		step.target = task_target_t::TARGET_MOB;
		step.mob_vnum = 407;
		player_finished_step(player,1,&step);
		player->sendln("Did you see a message for that one? If so, it's {grn}WORKING{/grn}.");
	}

	void init() {
		contracts_with_callbacks.emplace_back(1);
		contract_sequences[1].emplace_back(1);
		sequence_criteria[1].emplace_back(std::make_tuple<>(task_type_t::GOAL_FIND,task_target_t::TARGET_MOB,405));
		sequence_criteria[1].emplace_back(std::make_tuple<>(task_type_t::GOAL_FIND,task_target_t::TARGET_MOB,406));
		sequence_criteria[1].emplace_back(std::make_tuple<>(task_type_t::GOAL_FIND,task_target_t::TARGET_MOB,407));
		ADD_BUILDER_COMMAND("finish_step",do_finish_step);
	}

};
