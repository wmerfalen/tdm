#ifndef __MENTOC_MODS_scripted_sequencesS_HEADER__
#define __MENTOC_MODS_scripted_sequencesS_HEADER__
#include "../structs.h"
#include "scripted-step.hpp"
#include <vector>

namespace mods {
	using scripted_sequences_vnum_t = uint32_t;
	struct scripted_sequences {
		scripted_sequences_vnum_t vnum;
		std::vector<std::shared_ptr<scripted_step>> steps;
	};
	std::tuple<int32_t,std::string> compile_scripted_data();
	std::deque<std::shared_ptr<scripted_sequences>>& scripted_sequences_master_list();
};

namespace mods::scripted_sequence_events {
	using task_type_t = mods::contracts::contract_step::task_type_t;
	using task_target_t = mods::contracts::contract_step::task_target_t;
	using sequence_vnum_t = mods::scripted_sequences_vnum_t;
	extern std::vector<contract_vnum_t> contracts_with_callbacks;
	extern std::map<contract_vnum_t,std::vector<sequence_vnum_t>> contract_sequences;
	extern std::map<sequence_vnum_t,std::vector<std::tuple<task_type_t,task_target_t,uint32_t>>> sequence_criteria;
};

namespace mods::scripts::triton {
	static constexpr mob_vnum TRITON_SCIENTIST_VNUM = 600;
	static constexpr mob_vnum TRITON_LAB_ROOM_VNUM = 140;
};
#endif
