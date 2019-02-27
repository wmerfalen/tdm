#ifndef __MENTOC_MODS_EXTRA_DESC_DATA_HEADER__
#define __MENTOC_MODS_EXTRA_DESC_DATA_HEADER__

#include "string.hpp"
#include <string_view>
#include <iostream>
#include <vector>

namespace mods {
	struct extra_desc_data {
		extra_desc_data();
		extra_desc_data(std::string_view keyword,std::string_view descr);
		~extra_desc_data() = default;
		mods::string keyword;
		mods::string description;
	};
	using container_type_t = std::vector<extra_desc_data>;
};
#endif

