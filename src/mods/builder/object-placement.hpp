#ifndef __MENTOC_MODS_BUILDER_OBJECT_PLACEMENT_HEADER__
#define __MENTOC_MODS_BUILDER_OBJECT_PLACEMENT_HEADER__
#include "../deep-object-parser.hpp"

#define mbop_debug(A)\
			std::cerr << "[mods::builder::object_placement]:" << A << "\n";
namespace mods::builder::object_placement {
	using str_map_t = std::map<std::string,std::string>;
	str_map_t extract_line_items(const std::string& line,const std::vector<std::string>&);
	using deep_object_parser_t = mods::deep_object_parser_t;
};

ACMD(do_place_object);
#undef mbop_debug

#endif
