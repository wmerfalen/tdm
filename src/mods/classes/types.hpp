#ifndef __MENTOC_MODS_CLASSES_TYPES_HEADER__
#define  __MENTOC_MODS_CLASSES_TYPES_HEADER__
#include <string>

namespace mods::classes {
	enum types {
		ENGINEER = 0,
		MEDIC,
		SNIPER,
		MARINE,
		SUPPORT,
		SENTINEL,
		CONTAGION,
		PSYOP,
		GHOST,
		UNDEFINED
	};
	static inline std::string to_string(mods::classes::types t){
		using namespace mods::classes;
		switch(t){
			case types::ENGINEER: return "ENGINEER";
			case types::MEDIC: return "MEDIC";
			case types::SNIPER: return "SNIPER";
			case types::MARINE: return "MARINE";
			case types::SUPPORT: return "SUPPORT";
			case types::SENTINEL: return "SENTINEL";
			case types::CONTAGION: return "CONTAGION";
			case types::PSYOP: return "PSYOP";
			case types::GHOST: return "GHOST";
			default:
				return "UNDEFINED";
		}
		return "UNDEFINED";
	}
};
#endif
