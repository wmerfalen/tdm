#ifndef __MENTOC_MODS_RUNNERS_INCLUDES_HEADER__
#define __MENTOC_MODS_RUNNERS_INCLUDES_HEADER__
#include "../../structs.h"
#include "../scripted-sequences.hpp"
#include "../scripted-step.hpp"
#include "../contract-steps.hpp"
#include "../interpreter.hpp"
#include "../../comm.h"
#include <list>
#include "../doors.hpp"

using step_t = mods::contracts::contract_step;
using contract_vnum_t = mods::contracts::contract_vnum_t;
using task_type_t = mods::contracts::contract_step::task_type_t;
using task_target_t = mods::contracts::contract_step::task_target_t;
using sequence_vnum_t = mods::scripted_sequences_vnum_t;
namespace mods::runners {
	using step_ptr_t = std::shared_ptr<mods::scripted_step>;
	namespace communication {
		void act(step_ptr_t step,const uuid_t& player_uuid);
		void dialogue(step_ptr_t step,const uuid_t& player_uuid);
	};
	namespace movement {
		void walk_impl(player_ptr_t& mob,const direction_t& dir,const uuid_t& player_uuid);
		void walk(step_ptr_t step,direction_t dir,const uuid_t& player_uuid);
		void unlock(step_ptr_t step,direction_t dir,const uuid_t& player_uuid);
		void open(step_ptr_t step,direction_t dir,const uuid_t& player_uuid);
		void seal_room(step_ptr_t step,direction_t dir,const uuid_t& player_uuid);
		void force(step_ptr_t step,direction_t dir,const uuid_t& player_uuid);
	};
	namespace items {
		void mob_gives_yaml(step_ptr_t step,const uuid_t& player_uuid);
	};
};

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
	//extern std::list<step_ptr_t> deferred_list;

	std::size_t hash_step(step_ptr_t s,const uuid_t& player_uuid);
	void defer_step(uint32_t ticks,step_ptr_t s,const uuid_t& player_uuid);
	void queue_for_deferred_removal(step_ptr_t step);
};
#endif
