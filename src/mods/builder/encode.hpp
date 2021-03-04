#ifndef __MENTOC_MODS_BUILDER_ENCODE_HEADER__
#define __MENTOC_MODS_BUILDER_ENCODE_HEADER__

#include <iostream>
#include <vector>
#include <map>
#include <string>

namespace mods::builder {
	using strmap_t = std::map<std::string,std::string>;
	std::string dump(const strmap_t& data);
	std::string encode_map(const strmap_t& data);
	std::string encode(std::string_view data);
	strmap_t decode(std::string_view data);
};

#endif
