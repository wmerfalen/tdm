#ifndef __MENTOC_MODS_META_UTILS_HEADER__
#define __MENTOC_MODS_META_UTILS_HEADER__
#include <iostream>
#include <vector>
#include <string>
#include <utility>

namespace mods::meta_utils {
	using mutable_map_t = mods::lmdb::mutable_map_t;
	std::string extract_reference_field_from_meta_key(const std::string& key);
	std::vector<std::pair<std::string,std::string>> get_all_meta_values(
			const std::string& table,mutable_map_t* mapped_values_ptr);
	std::vector<std::string> generate_meta_keys(const std::string& table,
			mutable_map_t* mapped_values_ptr);
	std::optional<aligned_int_t> get_pk_by_meta( const std::string& table,mutable_map_t* users_data);
	int do_meta_easy_by_pk( const std::string & table, mutable_map_t* values);
	int do_meta_easy( const std::string & table, mutable_map_t* values,
			aligned_int_t* optional_place_found_id_here);
	int do_meta_easy( const std::string & table, mutable_map_t* values,
			std::shared_ptr<aligned_int_t> optional_user_id,aligned_int_t*);
	
};//End namespace

#endif
