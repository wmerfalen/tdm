#ifndef __MENTOC_MODS_SCREEN_SEARCHER_HEADER__
#define __MENTOC_MODS_SCREEN_SEARCHER_HEADER__

#include <iostream>
#include <vector>
#include <map>
#include "util.hpp"

namespace mods {

	template <typename TPlayerPtr>
	void search_screen(TPlayerPtr& player, const std::vector<std::string>& screen, const std::vector<std::string>& vec_args, std::size_t limit) {
		if(vec_args.size()) {
			for(auto& line : screen) {
				for(auto& arg : vec_args) {
					if(mods::util::fuzzy_match(arg.substr(0,std::min<std::size_t>(limit,arg.length())),line)) {
						player->sendx(
						    CAT(
						        "This line matched your search of {grn}'",arg.c_str(),"'{/grn}:"
						    )
						);
						player->sendln(line);
					}
				}
			}
		}
	}
};

#endif
