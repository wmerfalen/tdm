
#ifndef __MENTOC_MODS_scripted_sequencesS_HEADER__
#define __MENTOC_MODS_scripted_sequencesS_HEADER__
#include "../structs.h"

namespace mods {
	using scripted_sequences_vnum_t = uint32_t;
	struct scripted_sequences {
		scripted_sequences_vnum_t vnum;
		std::vector<scripted_step> steps;
	};
};
#endif
