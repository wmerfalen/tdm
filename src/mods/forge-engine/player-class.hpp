#ifndef __MENTOC_MODS_FORGE_ENGINE_PLAYER_CLASS_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_PLAYER_CLASS_HEADER__
#include "../../structs.h"

namespace mods::forge_engine {
		enum player_class_types_t {
			PLCLASS_SNIPER = 1,
			PLCLASS_MARINE ,
			PLCLASS_BREACHER ,
			PLCLASS_ENGINEER ,
			PLCLASS_MEDIC ,
			PLCLASS_GHOST ,
			PLCLASS_SUPPORT ,
			__PLCLASS_FIRST = PLCLASS_SNIPER,
			__PLCLASS_LAST = PLCLASS_SUPPORT
		};
};
#undef m_debug
#endif
