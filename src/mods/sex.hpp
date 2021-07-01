#ifndef __MENTOC_sex_HEADER__
#define  __MENTOC_sex_HEADER__

#include "../globals.hpp"

namespace mods::sex {
	static inline std::string to_db_string(int s) {
		switch(s) {
			case SEX_NEUTRAL:
				return "0";
			default:
			case SEX_MALE:
				return "1";
			case SEX_FEMALE:
				return "2";
		}
	}
};
#endif
