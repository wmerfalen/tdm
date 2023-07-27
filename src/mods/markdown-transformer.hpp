#ifndef __MENTOC_MODS_markdown_transformer_HEADER__
#define __MENTOC_MODS_markdown_transformer_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"
#include <istream>

namespace mods::markdown_transformer {
	std::string transform(std::string_view page);
};
#endif

