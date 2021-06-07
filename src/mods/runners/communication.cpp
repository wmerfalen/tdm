#include "../../structs.h"
#include "../player.hpp"
#include "includes.hpp"

#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug[communication]{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::runners::communication {
	using namespace mods::scripted_sequence_runner;
	void act(step_ptr_t step,const uuid_t& player_uuid) {
		auto mob = qmob(step->room,step->mob);
		if(mob == nullptr) {
			std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
		} else {
			act(step->dialogue.c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
		}
		queue_for_deferred_removal(step);
	}
	void dialogue(step_ptr_t step,const uuid_t& player_uuid) {
		auto mob = qmob(step->room,step->mob);
		if(mob == nullptr) {
			std::cerr << red_str("Error finding mob in room:") << step->dump() << "\n";
		} else {
			act(CAT("$n says, \"",step->dialogue,"\"").c_str(), FALSE, mob->cd(), 0, 0, TO_ROOM);
		}
		queue_for_deferred_removal(step);
	}
};//end namespace runners

