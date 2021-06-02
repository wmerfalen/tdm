#ifndef __MENTOC_MODS_scripted_sequence_runner_HEADER__
#define __MENTOC_MODS_scripted_sequence_runner_HEADER__
#include "../structs.h"
#include "scripted-sequences.hpp"
#include "scripted-step.hpp"
#include "contract-steps.hpp"

namespace mods::scripted_sequence_runner {
	using step_ptr_t = std::shared_ptr<mods::scripted_step>;
	using step_t = mods::contracts::contract_step;
	using contract_vnum_t = mods::contracts::contract_vnum_t;
	using task_type_t = mods::contracts::contract_step::task_type_t;
	using task_target_t = mods::contracts::contract_step::task_target_t;
	using sequence_vnum_t = mods::scripted_sequences_vnum_t;
	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum);
	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum,contract_vnum_t contract_vnum,step_t step);
	void step_runner(std::size_t hash);
	extern std::list<step_ptr_t> deferred_list;

	std::size_t hash_step(step_ptr_t s);
	void defer_step(uint32_t ticks,step_ptr_t s);
	void queue_for_deferred_removal(step_ptr_t step);
};
#endif
