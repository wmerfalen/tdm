#ifndef __MENTOC_MODS_ELEMENTAL_HEADER__
#define __MENTOC_MODS_ELEMENTAL_HEADER__
#include "item-types.hpp"

namespace mods {
	static inline const std::vector<elemental_types_t>& fetch_valid_elemental_types() {
		static std::vector<elemental_types_t> list = {
			ELEM_INCENDIARY,
			ELEM_EXPLOSIVE,
			ELEM_SHRAPNEL,
			ELEM_CORROSIVE,
			ELEM_CRYOGENIC,
			ELEM_RADIOACTIVE,
			ELEM_EMP,
			ELEM_SHOCK,
			ELEM_ANTI_MATTER
		};
		return list;
	};

}; //end namespace mods

#endif
