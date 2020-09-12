#ifndef __MENTOC_MODS_HELP_HEADER__
#define __MENTOC_MODS_HELP_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"
#include <istream>

namespace mods::object_utils {
	extern std::vector<std::string> object_types();
};
namespace mods::util {
	extern bool parse_help(std::string_view argument);
};
namespace mods::help {
	bool matches_many(const std::string& items,std::string_view from);
	bool send_help(std::string_view from,std::shared_ptr<mods::player>& player);
	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help);
};
#endif

