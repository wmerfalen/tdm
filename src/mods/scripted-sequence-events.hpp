#ifndef __MENTOC_MODS_scripted_sequences_events_HEADER__
#define __MENTOC_MODS_scripted_sequences_events_HEADER__
#include "../structs.h"
#include "scripted-sequences.hpp"
#include "scripted-step.hpp"
#include "contract-steps.hpp"

namespace mods::scripted_sequence_events {
	using step_t = mods::contracts::contract_step;
	using contract_vnum_t = mods::contracts::contract_vnum_t;
	void player_finished_step(player_ptr_t& player,const contract_vnum_t& contract_vnum,step_t* step);
};
#endif
