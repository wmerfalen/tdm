#include "stance.hpp"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rand.hpp"
#include <math.h>

#define __MENTOC_SHOW_stance_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_stance_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::stance][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::melee::stance {
	using de = damage_event_t;

};

