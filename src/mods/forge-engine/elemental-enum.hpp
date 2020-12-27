#ifndef __MENTOC_MODS_FORGE_ENGINE_ELEMENTAL_ENUM_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_ELEMENTAL_ENUM_HEADER__

namespace mods::forge_engine {
	enum elemental_types_t {
		ELEM_INCENDIARY = 1,
		ELEM_EXPLOSIVE,
		ELEM_SHRAPNEL,
		ELEM_CORROSIVE,
		ELEM_CRYOGENIC,
		ELEM_RADIOACTIVE,
		ELEM_EMP,
		ELEM_SHOCK,
		ELEM_ANTI_MATTER,
		__ELEM_FIRST = ELEM_INCENDIARY,
		__ELEM_LAST = ELEM_ANTI_MATTER
	};

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

}; //end namespace mods::forge_engine

#endif
