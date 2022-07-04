#ifndef __MENTOC_MODS_stats_rifle_HEADER__
#define __MENTOC_MODS_stats_rifle_HEADER__
#include <map>
#include <string>
#include <vector>
#include <string_view>

//#include "../globals.hpp"

namespace mods::rifle_stats {
	std::string format_rifle_stats_page(std::map<std::string,std::string>* page);
	std::string format_rifle_stats_page(std::string_view feed_file,std::map<std::string,std::string>* page);
	bool add_modified_file(std::string_view);
	bool clear_cache_of_files();
	std::vector<std::string> get_processed_files();
	void clear_files(std::vector<std::string>& files);
};
#endif
