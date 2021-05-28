#include "scripted-sequence-runner.hpp"
#include "interpreter.hpp"

#define __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#ifdef __MENTOC_SCRIPTED_SEQUENCE_RUNNER_DEBUG__
#define dbg_print(A) *player << "{grn}scripted_sequence_runner_debug{/grn}: " << A << "\r\n";
#else
#define dbg_print(A)
#endif

namespace mods::scripted_sequence_runner {
	std::vector<mods::scripted_step> triton_sequence;
	void dispatch(player_ptr_t& player,sequence_vnum_t sequence_vnum,contract_vnum_t contract_vnum,step_t step_copy) {
#if 0
		{
			mods::scripted_step step;
			step.wait_ticks = 0;
			step.dialogue.clear();
			step.type = "instantiate_mob_to_room";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->room_vnum();
			triton_sequence.emplace_back(step);
		}

		{
			mods::scripted_step step;
			step.wait_ticks = 0;
			step.dialogue = "A TRITON Energy scientist stands here in his white lab coat tending to some very interesting and extremely complex looking devices. Completely immersed in whatever hs is working on, he barely notices your presence. Wearily he looks up at you.";
			step.type = "dialogue";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->room_vnum();
			triton_sequence.emplace_back(step);
		}

		{
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.dialogue = "A TRITON Energy scientist says, \"Oh, I did not see you there.. you must be here for the high velocity magazines...\"";
			step.type = "dialogue";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->room_vnum();
			triton_sequence.emplace_back(step);
		}

		{
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.dialogue = "A TRITON Energy scientist reaches into his pocket and uses the FOB around his neck to unlock the door to a cold locker to the north.";
			step.type = "dialogue";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->room_vnum();
			triton_sequence.emplace_back(step);
		} {
			mods::scripted_step step;
			step.wait_ticks = 10;
			step.type = "walk_north";
			step.mob = mods::scripts::triton::TRITON_SCIENTIST_VNUM;
			step.room = player->room_vnum();
			triton_sequence.emplace_back(step);
		}
#endif

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
	}

};
