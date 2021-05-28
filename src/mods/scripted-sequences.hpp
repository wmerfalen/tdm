#ifndef __MENTOC_MODS_scripted_sequencesS_HEADER__
#define __MENTOC_MODS_scripted_sequencesS_HEADER__
#include "../structs.h"
#include "scripted-step.hpp"
#include <vector>

namespace mods {
	using scripted_sequences_vnum_t = uint32_t;
	struct scripted_sequences {
		scripted_sequences_vnum_t vnum;
		std::vector<scripted_step> steps;
	};
	std::tuple<int32_t,std::string> compile_scripted_data();
};

namespace mods::scripts::triton {
	static constexpr mob_vnum TRITON_SCIENTIST_VNUM = 600;
};
#endif
