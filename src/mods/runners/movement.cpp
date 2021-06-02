#include "../../structs.h"
#include "../player.hpp"
#include "includes.hpp"

#define __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::runners::movement {
	using namespace mods::scripted_sequence_runner;
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

