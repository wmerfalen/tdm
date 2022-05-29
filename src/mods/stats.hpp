#ifndef __MENTOC_MODS_stats_HEADER__
#define __MENTOC_MODS_stats_HEADER__
#include "../globals.hpp"

namespace mods::stats {
	std::string format_stats_page(std::string_view feed_file,std::string_view type,std::map<std::string,std::string>* stats_page);
};
#endif
