#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rifle-attachments.hpp"
#include "../rand.hpp"
#include <math.h>

#define __MENTOC_SHOW_combat_order_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_combat_order_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::combat_order][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::melee::combat_order {
	using de = damage_event_t;
};

