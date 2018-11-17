#include "lmdb.hpp"
#include "db.hpp"
#include "meta_utils.hpp"
#include "util.hpp"
#include "util-map.hpp"

namespace mods::meta_utils {
	constexpr static const char* meta = "meta";
	constexpr static const char* delim = "|";
	constexpr static std::size_t reference_field_index = 1;
	using mutable_map_t = mods::lmdb::mutable_map_t;
	enum return_codes {
		COULDNT_FIND_USER_ID = -1
	};
	void write_meta(std::string table,mutable_map_t* values){
		if((*values)["id"].length() == 0){
			std::cerr << "error: write_meta called with invalid id field\n";
			return;
		}
		if(mods::util::stoi_optional<aligned_int_t>((*values)["id"]).has_value() == false){
			std::cerr << "error: write_meta called but pk is not a valid int. value: '" << (*values)["id"] << "'\n";
			return;
		}
		for(auto &key : get_all_meta_values(table,values)){
			db_put(key,(*values)["id"]);
		}
	}
	std::string extract_reference_field_from_meta_key(const std::string& key){
		/**
		 * Since I've decided that putting "meta|<column>" for each key in the 
		 * db_meta_values container is extremely redundant, there is no need
		 * to attempt to parse the key. In the future when the key gets more complex
		 * and needs a pipe-delimited syntax, this function will need to change.
		 * For now, let's keep it simple and just have the column name. As of
		 * 2018-11-10, the values in db_meta_values went from "meta|player_name"
		 * to just "player_name".
		 * -wm 2018-11-10
		 */
		return key;
		/*
		std::string temp = "";
		std::vector<std::string> parsed_fields;

		for(unsigned i =0; i < key.length();i++){
			if(std::string(&key[i]).compare(mods::meta_utils::delim) == 0){
				parsed_fields.emplace_back(temp);
				temp.clear();
			}else{
				temp += key[i];
			}
			if(reference_field_index == i){
				if(parsed_fields.size() > mods::meta_utils::reference_field_index){
					return parsed_fields.at(mods::meta_utils::reference_field_index);
				}
			}
		}
		return "";
		*/
	}
	std::vector<std::string> get_all_meta_values(
			const std::string& table,mutable_map_t* mapped_values_ptr){
		std::cout << "debug: get_all_meta_values has received: \n";
		mods::util::maps::dump<std::string,std::string>(*mapped_values_ptr);
		std::cout << "debug: end dump\n";
		std::vector<std::string> result;
		for(std::string & str_meta_key : mods::schema::db_meta_values[table]){
			std::string field = extract_reference_field_from_meta_key(str_meta_key);
			std::string field_value = (*mapped_values_ptr)[field];
			if(field_value.length() == 0){
				std::cerr << "warning: meta field_value empty for field: '" << field << "'\n";
				continue;
			}
			std::cout << "debug: get_all_meta_values field:'" << field << "' value: '" << field_value << "'\n";
			result.emplace_back(db_key({table,"meta",field,field_value}));
		}
		return result;
	}
};//End namespace
