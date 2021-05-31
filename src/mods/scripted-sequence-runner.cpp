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
	std::vector<mods::scripted_step>& triton_sequence() {
		static std::vector<mods::scripted_step> list;
		return list;
	}

	using step_ptr_t = std::shared_ptr<mods::scripted_step>;
	std::list<step_ptr_t> deferred_list;

	std::size_t hash_step(step_ptr_t s) {
		return std::hash<step_ptr_t>()(s);
	}
	void defer_step(uint32_t ticks,const mods::scripted_step& step_ptr) {
		auto s = std::make_shared<mods::scripted_step>(step_ptr);
		auto hash = hash_step(s);
		deferred_list.emplace_back(std::move(s));
		DEFER_STEP(ticks,hash);
	}

	SUPERCMD(do_triton_sequence) {
		player->sendln("[+] Initializing triton sequence...");
		triton_sequence().clear();
		{
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.dialogue = "$n stands here in his white lab coat tending to some very interesting and extremely complex looking devices. Completely immersed in whatever hs is working on, he barely notices your presence. Wearily he looks up at you.";
			step.type = "act";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}

		{
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.dialogue = "Oh, I did not see you there.. ";
			step.type = "dialogue";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}
		{
			mods::scripted_step step;
			step.wait_ticks = 3;
			step.dialogue = "You must be here for the high velocity magazines... Follow me...";
			step.type = "dialogue";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}

		{
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.dialogue = "$n unlocks the door to the north";
			step.type = "unlock_north";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}
		{
			mods::scripted_step step;
			step.wait_ticks = 0;
			step.dialogue = "$n opens the door to the north";
			step.type = "open_north";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}
		{
			mods::scripted_step step;
			step.wait_ticks = 3;
			step.type = "walk_north";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->vnum();
			triton_sequence().emplace_back(step);
		}
		player->sendln("[+] Done.");
		uint64_t ticks = 0;
		for(const auto& step : triton_sequence()) {
			ticks += std::clamp((int)step.wait_ticks,1,UINT16_MAX);
			defer_step(ticks,step);
		}

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
		void walk_random(step_ptr_t step) {
			auto mob = qmob(step->room,step->mob);
			if(mob == nullptr) {
				std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
			} else {
				auto dir = rand_number(0,NUM_OF_DIRS);
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
				if(step->type.compare("dialogue") == 0) {
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
				if(step->type.compare("walk_random") == 0) {
					return runners::walk_random(step);
				}
			}
		}
	}
	//- Zephyr walks north
	//[on event: Player enters room](guard: has contract VNUM 3)
	//A TRITON Energy scientist hands Zephyr a briefcase.
	//[publish event: GOAL_FIND_ITEM, object_vnum: 405]


#if 0
	struct scripted_step {
		uint16_t wait_ticks; /** how many ticks to wait until executing this */
		std::string dialogue; /** text to display */
		mob_vnum mob;
		obj_vnum obj;
		room_vnum room;
		std::string yaml;
		std::string type;
		std::vector<std::string> interpret;
		uint16_t quantity;
		scripted_step_vnum_t vnum;
		uint16_t order;
	}
#endif

	SUPERCMD(do_run_sequence) {
		if(argshave()->int_at(0)->size_gt(0)->passed()) {
			player->sendln(CAT("Run sequence number ",argat(0),", eh?"));
			return;
		}

	}

	void init() {
		ADD_BUILDER_COMMAND("run_sequence",do_run_sequence);
		ADD_BUILDER_COMMAND("triton_sequence",do_triton_sequence);
	}

	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum,contract_vnum_t contract_vnum,step_t step_copy) {

	}


};
