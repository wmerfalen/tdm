#ifndef __MENTOC_MODS_META_UTILS_HEADER__
#define __MENTOC_MODS_META_UTILS_HEADER__
#include <iostream>
#include <vector>
#include <string>
#include <utility>

namespace mods::meta_utils {
	using mutable_map_t = mods::lmdb::mutable_map_t;
	std::string extract_reference_field_from_meta_key(const std::string& key);
	std::vector<std::string> get_all_meta_values(
			const std::string& table,mutable_map_t* mapped_values_ptr);
	void write_meta(std::string table,mutable_map_t* values);
	
};//End namespace

#endif
