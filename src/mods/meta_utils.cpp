#include "lmdb.hpp"
#include "db.hpp"
#include "meta_utils.hpp"
#include "util.hpp"

namespace mods::meta_utils {
	constexpr static const char* meta = "meta";
	constexpr static const char* delim = "|";
	constexpr static std::size_t reference_field_index = 1;
	using mutable_map_t = mods::lmdb::mutable_map_t;
	enum return_codes {
		COULDNT_FIND_USER_ID = -1
	};
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
	std::vector<std::pair<std::string,std::string>> get_all_meta_values(
			const std::string& table,mutable_map_t* mapped_values_ptr){
		std::vector<std::pair<std::string,std::string>> result;
		for(std::string & str_meta_key : mods::schema::db_meta_values[table]){
			std::string field = extract_reference_field_from_meta_key(str_meta_key);
			std::string field_value = (*mapped_values_ptr)[field];
			if(field_value.length() == 0){
				continue;
			}
			std::cout << "debug: get_all_meta_values field:'" << field << "' value: '" << field_value << "'\n";
			result.emplace_back(std::make_pair(field,field_value));
		}
		return result;
	}
	std::optional<aligned_int_t> get_pk_by_meta(
			const std::string& table,mutable_map_t* users_data){
		auto meta_vals = get_all_meta_values(table,users_data);
		if(meta_vals.size() == 0){
			return std::nullopt;
		}
		auto ptr_db = mods::globals::db.get();
		std::string str_pk = "";
		for(auto & [field_name,field_value] : meta_vals){
			if(field_value.length()){
				ptr_db->get(db_key({table,"meta",field_name,field_value}),str_pk);
				if(str_pk.length()){
					auto i = mods::util::stoi_optional<aligned_int_t>(str_pk);
					if(i.has_value()){
						return i;
					}
					str_pk.clear();
				}
			}
		}
		return std::nullopt;
	}
	/**
	 * This function will generate all the meta keys that you'll need to place in the db depending on which table you pass in. If the table has no meta values, it'll return an empty vector.
	 * @returns a vector<string> of lmdb keys that you'll need to write
	 */
	std::vector<std::string> generate_meta_keys(const std::string& table,
			mutable_map_t* mapped_values_ptr){
		if(mapped_values_ptr == nullptr){
			std::cerr << "WARNING: received a null mapped_values_ptr in generate_meta_keys!\n";
			return {};
		}
		std::vector<std::string> result;
		result.reserve(mods::schema::db_meta_values[table].size());
		for(std::string & str_meta_key : mods::schema::db_meta_values[table]){
			std::string field = extract_reference_field_from_meta_key(str_meta_key);
			std::string field_value = (*mapped_values_ptr)[field];
			result.emplace_back(db_key({table.data(),
						"meta",field,field_value}));
		}
		return result;

	}
	int do_meta_easy_by_pk( const std::string & table, mutable_map_t* values){
		std::stringstream ss;
		ss << (*values)["id"];
		aligned_int_t id = 0;
		ss >> id;
		auto id_ptr = std::make_shared<aligned_int_t>(id);
		auto status = do_meta_easy(table,values,id_ptr,nullptr);
		if(status < 0){
			if(status == -1){
				std::cerr << "do_meta_easy: error. primary key ('id') is either not in db, or in mutable map\n";
			}
		}else if(status == 0){
			std::cout << "info: do_meta_easy didn't place anything in the db. meta schema is likely empty for given table '" <<
				table << "'\n";
		}
		if(status > 0){
			std::cout << "debug: placed " << status << " items in meta for table '" << table << "'\n";
		}
		return status;
	}
	int do_meta_easy(  const std::string & table, mutable_map_t* values,
			aligned_int_t* out_place_user_id_here){
		aligned_int_t found_user_id = 0;
		int status = do_meta_easy(table,values,nullptr,&found_user_id);
		if(status < 0){
			if(status == -1){
				std::cerr << "do_meta_easy: error. primary key ('id') is either not in db, or in mutable map\n";
			}
		}else if(status == 0){
			std::cout << "info: do_meta_easy didn't place anything in the db. meta schema is likely empty for given table '" <<
				table << "'\n";
		}
		if(status > 0){
			std::cout << "debug: placed " << status << " items in meta for table '" << table << "'\n";
		}
		if(found_user_id != 0 && out_place_user_id_here != nullptr){
			*out_place_user_id_here = found_user_id;
		}
		return status;
	}

	int do_meta_easy(  const std::string & table, mutable_map_t* values,
			std::shared_ptr<aligned_int_t> optional_user_id,aligned_int_t* out_place_found_user_id_here){
		auto ptr_db = mods::globals::db.get();
		aligned_int_t found_user_id = 0;
		std::string existing_user_id = "";
		int items_put = 0;
		auto vector_meta_keys = mods::meta_utils::generate_meta_keys(table,values);
		if(!optional_user_id){
			//Then we have to search the meta values for the user_id in the database
			for(auto & key : vector_meta_keys){
				found_user_id = 0;
				ptr_db->get(
						key.data(),
						existing_user_id);
						std::stringstream stream;
						stream << existing_user_id;
						stream >> found_user_id;
				if(found_user_id != 0){
					break;
				}
			}
		}else{
			found_user_id = *optional_user_id;
		}
		if(found_user_id == 0){
			std::cout << "info: attemping to get id from values\n";
			if((*values)["id"].length()){
				std::stringstream stream;
				stream << (*values)["id"];
				stream >> found_user_id;
				std::cout << "debug: stringstream extracted: 'id' field as: " << found_user_id << "\n";
			}
		}
		if(found_user_id != 0){
			for(auto & str_meta_key : vector_meta_keys){
				ptr_db->put(str_meta_key, std::to_string(found_user_id));
			}
		}else{ 
			std::cerr << "primary key ('id') is zero (do_meta_easy)\n";
			items_put = -1;
		}
		if(out_place_found_user_id_here != nullptr){
			*out_place_found_user_id_here = found_user_id;
		}
		return items_put;
	}
};//End namespace
