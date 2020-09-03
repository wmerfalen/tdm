#ifndef __MENTOC_MODS_BUGS_FIXTURES_HEADER__
#define  __MENTOC_MODS_BUGS_FIXTURES_HEADER__

#include "../../globals.hpp"
#include <fstream>

namespace mods::bugs {
	void fixtures(std::string name,std::string msg);
	void fixtures(std::string_view from);
	void close();
};

#endif
