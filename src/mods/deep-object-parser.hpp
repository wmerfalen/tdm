#ifndef __MENTOC_MODS_DEEP_OBJECT_PARSER_HEADER__
#define __MENTOC_MODS_DEEP_OBJECT_PARSER_HEADER__
#include "builder.hpp"
#include "pq.hpp"
#include "util.hpp"
#include "object.hpp"
#include "../structs.h"
#include <string>
#include <optional>

#define mdo_debug(A)\
			std::cerr << "[mods::deep_object_parser:" << A << "\n";
namespace mods {
	using str_map_t = std::map<std::string,std::string>;
	struct deep_object_parser_t {
		str_map_t extract_line_items(const std::string& line, const std::vector<std::string> & slot_names);
		private:
		std::vector<obj_ptr_t> m_line_items;
	};
};

#undef mdo_debug

#endif
