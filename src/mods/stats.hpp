#ifndef __MENTOC_MODS_stats_HEADER__
#define __MENTOC_MODS_stats_HEADER__
#include "../globals.hpp"

namespace mods::stats {
	std::string format_stats_page(std::map<std::string,std::string>&& page);
};
#endif
