#ifndef __MENTOC_MODS_scripted_stepS_HEADER__
#define __MENTOC_MODS_scripted_stepS_HEADER__
#include "../structs.h"
#include "contracts.hpp"

namespace mods {
	using contract_vnum_t = mods::contracts::contract_vnum_t;
	using task_type_t = mods::contracts::contract_step::task_type_t;
	using task_target_t = mods::contracts::contract_step::task_target_t;
	using scripted_step_vnum_t = uint32_t;
	struct scripted_step {
		uint16_t wait_ticks; /** how many ticks to wait until executing this */
		std::string dialogue; /** text to display */
		mob_vnum mob;
		obj_vnum obj;
		room_vnum room;
		std::string yaml;
		std::string type;
		std::vector<std::string> interpret;
		uint8_t quantity;
		scripted_step_vnum_t vnum;
	};
	void register_scripted_sequence(uint32_t sequence_vnum, std::vector<int> sequence);
	void register_contract_step_callback(contract_vnum_t, task_type_t, task_target_t, uint32_t task_vnum, uint32_t sequence_vnum);
};
#endif
