#ifndef __MENTOC_MODS_FORGE_ENGINE_ELEMENTAL_ENUM_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_ELEMENTAL_ENUM_HEADER__
#include "../elemental.hpp"

namespace mods::forge_engine {
	using elemental_types_t = mods::elemental_types_t;
	static inline const std::vector<elemental_types_t>& fetch_valid_elemental_types() {
		return mods::fetch_valid_elemental_types();
	};

}; //end namespace mods::forge_engine

#endif
