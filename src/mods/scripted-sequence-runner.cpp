#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"
#include "../comm.h"
#include <list>
#include "doors.hpp"

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
	namespace runners {
		void act(step_ptr_t step) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				act(step->dialogue.c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
			}
			queue_for_deferred_removal(step);
		}
		void dialogue(step_ptr_t step) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				act(CAT("$n says, \"",step->dialogue,"\"").c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
			}
			queue_for_deferred_removal(step);
		}
		void walk_impl(player_ptr_t& mob,const direction_t& dir) {
			do_simple_move(mob->cd(), dir, 0);
		}
		void walk(step_ptr_t step,direction_t dir) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				walk_impl(mob,dir);
			}
			queue_for_deferred_removal(step);
		}
		/**
		 * unlocks a door.
		 *
		 * If at anytime we support zone commands for locking doors, there very well
		 * may be race conditions where this function finishes and the next game tick
		 * the zone is reset and a door lock command is run.
		 */
		void unlock(step_ptr_t step,direction_t dir) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				mods::doors::unlock(real_room(step->room),dir);
				if(step->dialogue.length()) {
					act(step->dialogue.c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
				}
			}
			queue_for_deferred_removal(step);
		}

		/**
		 * open a direction specified.
		 * This is basically a forced open.. if the door is locked to
		 * player characters and requires a key this command essentially ignores
		 * that. It will just open the door. So to be more organic, you would
		 * want to have an unlock step precede this type of step (of course if
		 * the door requires a key to unlock it).
		 *
		 * Possible race conditions can occur if we start supporting zone commands
		 * which lock/close doors in specific intervals. For example, lets say this
		 * step finishes at tick 10. A zone command that runs at tick 11 will
		 * lock and close the door. This is problematic because the "walk_<dir>"
		 * step types just use do_simple_move and that function will fail to move
		 * your mob if the door is locked/closed.
		 *
		 */
		void open(step_ptr_t step,direction_t dir) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				mods::doors::open(real_room(step->room),dir);
				if(step->dialogue.length()) {
					act(step->dialogue.c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
				}
			}
			queue_for_deferred_removal(step);
		}
	};//end namespace runners

	/**
	 * This gets called by the deferred class when you register
	 * a step using push_step()
	 */
	void step_runner(std::size_t hash) {
		for(const auto& step : deferred_list) {
			if(std::hash<step_ptr_t>()(step) == hash) {
				if(step->type.compare("act") == 0) {
					return runners::act(step);
				}
				if(step->type.compare("say") == 0 || step->type.compare("dialogue") == 0) {
					return runners::dialogue(step);
				}
				if(step->type.compare("open_east") == 0) {
					return runners::open(step,EAST);
				}
				if(step->type.compare("open_south") == 0) {
					return runners::open(step,SOUTH);
				}
				if(step->type.compare("open_west") == 0) {
					return runners::open(step,WEST);
				}
				if(step->type.compare("open_north") == 0) {
					return runners::open(step,NORTH);
				}
				if(step->type.compare("open_up") == 0) {
					return runners::open(step,UP);
				}
				if(step->type.compare("open_down") == 0) {
					return runners::open(step,DOWN);
				}
				if(step->type.compare("unlock_east") == 0) {
					return runners::unlock(step,EAST);
				}
				if(step->type.compare("unlock_south") == 0) {
					return runners::unlock(step,SOUTH);
				}
				if(step->type.compare("unlock_west") == 0) {
					return runners::unlock(step,WEST);
				}
				if(step->type.compare("unlock_north") == 0) {
					return runners::unlock(step,NORTH);
				}
				if(step->type.compare("unlock_up") == 0) {
					return runners::unlock(step,UP);
				}
				if(step->type.compare("unlock_down") == 0) {
					return runners::unlock(step,DOWN);
				}

				if(step->type.compare("walk_north") == 0) {
					return runners::walk(step,NORTH);
				}
				if(step->type.compare("walk_east") == 0) {
					return runners::walk(step,EAST);
				}
				if(step->type.compare("walk_south") == 0) {
					return runners::walk(step,SOUTH);
				}
				if(step->type.compare("walk_west") == 0) {
					return runners::walk(step,WEST);
				}
				if(step->type.compare("walk_up") == 0) {
					return runners::walk(step,UP);
				}
				if(step->type.compare("walk_down") == 0) {
					return runners::walk(step,DOWN);
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


};
