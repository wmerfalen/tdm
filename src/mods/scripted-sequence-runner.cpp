#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"
#include "../comm.h"
#include <list>
#include "doors.hpp"
#include "runners/includes.hpp"

#define __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::scripted_sequence_runner {

	using step_ptr_t = std::shared_ptr<mods::scripted_step>;
	std::list<step_ptr_t> deferred_list;

	std::size_t hash_step(step_ptr_t s) {
		return std::hash<step_ptr_t>()(s);
	}
	void defer_step(uint32_t ticks,step_ptr_t s) {
		auto hash = hash_step(s);
		deferred_list.emplace_back(s);
		DEFER_STEP(ticks,hash);
	}

	std::vector<mods::scripted_step>& triton_sequence() {
		static std::vector<mods::scripted_step> list;
		return list;
	}
	void queue_for_deferred_removal(step_ptr_t step) {
		static std::set<std::size_t> steps_to_remove;
		steps_to_remove.insert(hash_step(step));
		DEFER_TICKS(20,[&]() {
			std::remove_if(deferred_list.begin(),deferred_list.end(),[&](const auto& step) -> bool {
				auto hash = hash_step(step);
				auto it = steps_to_remove.find(hash);
				if(it != steps_to_remove.end()) {
					steps_to_remove.erase(it);
					return true;
				}
				return false;
			});
		});
	}

	/**
	 * This gets called by the deferred class when you register
	 * a step using push_step()
	 */
	void step_runner(std::size_t hash) {
		using namespace mods::runners;
		for(const auto& step : deferred_list) {
			if(std::hash<step_ptr_t>()(step) == hash) {
				if(step->type.compare("act") == 0) {
					return movement::act(step);
				}
				if(step->type.compare("say") == 0 || step->type.compare("dialogue") == 0) {
					return movement::dialogue(step);
				}
				if(step->type.compare("open_east") == 0) {
					return movement::open(step,EAST);
				}
				if(step->type.compare("open_south") == 0) {
					return movement::open(step,SOUTH);
				}
				if(step->type.compare("open_west") == 0) {
					return movement::open(step,WEST);
				}
				if(step->type.compare("open_north") == 0) {
					return movement::open(step,NORTH);
				}
				if(step->type.compare("open_up") == 0) {
					return movement::open(step,UP);
				}
				if(step->type.compare("open_down") == 0) {
					return movement::open(step,DOWN);
				}
				if(step->type.compare("unlock_east") == 0) {
					return movement::unlock(step,EAST);
				}
				if(step->type.compare("unlock_south") == 0) {
					return movement::unlock(step,SOUTH);
				}
				if(step->type.compare("unlock_west") == 0) {
					return movement::unlock(step,WEST);
				}
				if(step->type.compare("unlock_north") == 0) {
					return movement::unlock(step,NORTH);
				}
				if(step->type.compare("unlock_up") == 0) {
					return movement::unlock(step,UP);
				}
				if(step->type.compare("unlock_down") == 0) {
					return movement::unlock(step,DOWN);
				}

				if(step->type.compare("walk_north") == 0) {
					return movement::walk(step,NORTH);
				}
				if(step->type.compare("walk_east") == 0) {
					return movement::walk(step,EAST);
				}
				if(step->type.compare("walk_south") == 0) {
					return movement::walk(step,SOUTH);
				}
				if(step->type.compare("walk_west") == 0) {
					return movement::walk(step,WEST);
				}
				if(step->type.compare("walk_up") == 0) {
					return movement::walk(step,UP);
				}
				if(step->type.compare("walk_down") == 0) {
					return movement::walk(step,DOWN);
				}
			}
		}
	}

	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum) {
		uint64_t ticks = 0;
		for(const auto& seq : mods::scripted_sequences_master_list()) {
			if(seq->vnum == sequence_vnum) {
				for(const auto& step : seq->steps) {
					ticks += std::clamp((int)step->wait_ticks,1,UINT16_MAX);
					defer_step(ticks,step);
				}
			}
		}
	}
	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum,contract_vnum_t contract_vnum,step_t step_copy) {
		uint64_t ticks = 0;
		for(const auto& seq : mods::scripted_sequences_master_list()) {
			if(seq->vnum == sequence_vnum) {
				for(const auto& step : seq->steps) {
					ticks += std::clamp((int)step->wait_ticks,1,UINT16_MAX);
					defer_step(ticks,step);
				}
			}
		}
	}

	SUPERCMD(do_run_sequence) {
		if(argshave()->first_is("list")->passed()) {
			player->sendln("Listing...");
			for(const auto& seq : mods::scripted_sequences_master_list()) {
				player->sendln(CAT(seq->vnum).c_str());
			}
			player->sendln("Done listing...");
			return;
		}
		if(argshave()->int_at(0)->size_gt(0)->passed()) {
			for(const auto& seq : mods::scripted_sequences_master_list()) {
				if(seq->vnum == intat(0)) {
					player->sendln(CAT("Running sequence [",intat(0),"]...").c_str());
					dispatch(player,seq->vnum);
					player->sendln("Dispatched.");
				}
			}
			return;
		}
	}

	void init() {
		ADD_BUILDER_COMMAND("run_sequence",do_run_sequence);
	}



};
