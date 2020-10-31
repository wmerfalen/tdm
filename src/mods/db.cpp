#include "db.hpp"
#include "lmdb.hpp"
#include "meta_utils.hpp"
#include "player.hpp"
#include "pq.hpp"
#define __MENTOC_SHOW_LMDB_DEBUG__

extern std::string sanitize_key(std::string key);
namespace mods::db {
	std::string section_key(std::string section_name,std::string prefix, std::string type){
		return db_key({section_name,prefix,type}) + "|";
	}
	std::string section_key_index(std::string section_name,std::string prefix, std::string type){
		return section_key(section_name.data(),prefix,type) + "index";
	}
	std::string section_key_count(std::string section_name,std::string prefix, std::string type){
		return section_key(section_name.data(),prefix,type) + "count";
	}
	bool vector_exists(std::string section_name,std::string prefix){
		return mods::globals::db->exists(section_key_count(section_name,prefix,"vector"));
	}

	int put_section_map(std::string_view section_name,std::string prefix, std::map<std::string,std::string> values){
		mods::db::lmdb_renew();
		std::string key = "";
		key = section_key(section_name.data(),prefix,"map");
		int i =0;
		std::string keys;
		for(auto & pair : values){
			keys += pair.first + "|";
			mods::db::lmdb_put(key + sanitize_key(pair.first),pair.second);
			++i;
		}
		mods::db::lmdb_put(section_key_index(section_name.data(),prefix,"map"),keys);
		mods::db::lmdb_commit();
		return i;
	}

	int get_section_map(std::string_view section_name,std::string prefix, std::map<std::string,std::string>& values){
		mods::db::lmdb_renew();
		std::string key = "";
		key = section_key(section_name.data(),prefix,"map");
		std::string index_key = section_key_index(section_name.data(),prefix,"map");
		int i= 0;
		std::string current = "";
		for(auto ch : mods::db::lmdb_get(index_key)){
			if(ch == '|' && current.length()){
				values[current] = mods::db::lmdb_get(key + current);
				current = "";
				continue;
			}
			current += ch;
		}
		mods::db::lmdb_commit();
		return i;
	}
	int put_section_vector(std::string_view section_name,std::string prefix, std::vector<std::string> values){
		mods::db::lmdb_renew();
		std::string key = "";
		key = section_key(section_name.data(),prefix,"vector");
		int i= 0;
		for(auto & val : values){
			mods::db::lmdb_put(key + std::to_string(i),val);
			++i;
		}
		mods::db::lmdb_put(section_key_count(section_name.data(),prefix,"vector"),std::to_string(i));
		mods::db::lmdb_commit();
		return i;
	}

	int delete_section_vector(std::string_view section_name,std::string prefix){
		mods::db::lmdb_renew();
		std::string key = "";
		key = section_key(section_name.data(),prefix,"vector");
		std::string count_key = key + "count";
		int i= 0;
		auto count = atoi(mods::db::lmdb_get(section_key_count(section_name.data(),prefix,"vector")).c_str());
		for(; i < count;++i){
			mods::db::lmdb_del(key + std::to_string(i));
		}
		mods::db::lmdb_del(count_key);
		mods::db::lmdb_commit();
		return i;
	}

	int get_section_vector(std::string_view section_name,std::string prefix, std::vector<std::string>& values){
		mods::db::lmdb_renew();
		std::string key = "";
		key = section_key(section_name.data(),prefix,"vector");
		std::string count_key = key + "count";
		int i= 0;
		auto count = atoi(mods::db::lmdb_get(section_key_count(section_name.data(),prefix,"vector")).c_str());
		for(; i < count;++i){
			std::string value = mods::db::lmdb_get(key + std::to_string(i));
			values.emplace_back(value);
		}
		mods::db::lmdb_commit();
		return i;
	}

	/**
	 * @brief saves a record and returns the pk id of that record
	 *
	 * @param table
	 * @param values
	 *
	 * @return 
	 */
aligned_int_t save_record_get_id(
		const std::string& table,
		mutable_map_t* values){
	auto status = mods::db::save_record(table,values,(*values)["id"]);
	if(std::get<0>(status)){
		return std::get<2>(status);
	}
	else{
		return 0;
	}
}

/**
 * @brief initializes a row on the table, thus filling default values for each column
 *
 * @param table
 *
 * @return 
 */
aligned_int_t initialize_row(
		const std::string& table){
	auto ptr_db = mods::globals::db.get();
	ptr_db->renew_txn();
	std::string id_list = "";
	id_list = db_get(db_key({table,"id_list"}));
	//std::cout << "id_list: '" << id_list << "' .size(): " << id_list.length() << "\n";
	//std::cout << "debug: id_list buffer: '" << id_list << "'\n";
	aligned_int_t next_id = 0;
	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.length() == 0){
		deserialized_id_list.push_back(0);
		next_id = 0;
	}
	else{
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
		next_id = ++deserialized_id_list[0];
	}
	id_list.clear();
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	auto ret = ptr_db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "failed to place id_list!!!\n";
#endif
		return 0;
	}

	ret = ptr_db->put(db_key({table,"id",std::to_string(next_id)}),"active");
	ret = ptr_db->put(db_key({table,"active",std::to_string(next_id)}),"active");
	if(ret < 0){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "failed to place active key!!!\n";
#endif
		return 0;
	}
	auto tuple_return = ptr_db->commit();
	if(!std::get<0>(tuple_return)){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "error: commit failed (initialize_row)\n";
#endif
		return 0;
	}
	return next_id;
}

tuple_status_t new_record(const std::string& table,mutable_map_t* values){
	auto pk_id = mods::db::initialize_row(table);
	//std::cout << "debug: new pk_id: '" << pk_id << "'\n";
	return save_record(table,values,std::to_string(pk_id));
}

/**
 * @brief saves an already existing record into the db specified by pk_id 
 *
 * @param table
 * @param values
 * @param pk_id
 *
 * @return 
 */
tuple_status_t save_record(const std::string& table,mutable_map_t* values,std::string pk_id){
	mods::globals::db->renew_txn();
	for(const auto & meta_key : mods::meta_utils::get_all_meta_values(table,values)){
		//std::cout << "debug: save_record. Putting: '" << meta_key << "' as '" << pk_id << "'\n";
		lmdb_put(meta_key,pk_id);
	}
		const auto write_status =  lmdb_write_values(
				table,
				values,pk_id);
		if(!std::get<0>(write_status)){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
			std::cerr << "error: lmdb_write_values failed with: '" << std::get<1>(write_status) << "'\n";
#endif
		}
	const auto tuple_status = mods::globals::db->commit();
	if(std::get<0>(tuple_status)){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "debug: save_record success\n";
#endif
		return {true,"saved",mods::util::stoi<aligned_int_t>(pk_id)};
	}else{
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "debug: save_record FAILURE: '" << std::get<1>(tuple_status) << "'\n";
#endif
		return {false,std::get<1>(tuple_status),0};
	}
}

/**
 * @brief renews the transaction on the lmdb interface
 */
void lmdb_renew() {
	mods::globals::db->renew_txn();
}

bool lmdb_exists(std::string key) {
	return mods::globals::db->exists(key);
}
/**
 * @brief grabs a key and stores it in an untyped void* buffer
 *
 * @param key
 * @param k_size
 * @param value
 *
 * @return 
 */
int lmdb_nget_void(void* key,std::size_t k_size,void* value){
	const auto status = mods::globals::db->nget(key,k_size,value);
	return status;
}
/**
 * @brief grabs a key and stores it in a string (value)
 *
 * @param key
 * @param k_size
 * @param value
 *
 * @return 
 */
int lmdb_nget(void* key,std::size_t k_size,std::string& value){
	value = "";
	const auto status = mods::globals::db->nget(key,k_size,value);
	return status;
	//if(status == EINVAL){
	//	std::cerr << "[lmdb_nget] got EINVAL -- retrying...\n";
	//	lmdb_renew();
	//	value = "";
	//	status = mods::globals::db->nget(key,k_size,value);
	//	std::cerr << "[lmdb_nget] retry status: " << status << "\n";
	//}
	//return status;
}
/**
 * @brief grabs a key and returns it by string
 *
 * @param key
 *
 * @return 
 */
std::string lmdb_get(std::string_view key){
	std::string value = "";
	auto status = mods::globals::db->get(key.data(),value);
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
	std::cerr << "[lmbd_get] key:'" << key.data() << "', value: '" << value << "'\n";
#endif
	if(status == EINVAL){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "[lmdb_get] got EINVAL -- retrying...\n";
#endif
		lmdb_renew();
		status = mods::globals::db->get(key.data(),value);
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "retry status: " << status << "\n";
		std::cerr << "[lmbd_get] key:'" << key.data() << "', value: '" << value << "'\n";
#endif
	}
	return value;
}
/**
 * @brief places a key into lmdb using a void* buffer as both key and type
 * Use the key_size and v_size parameters to dictate how big your untyped buffers are
 *
 * @param key
 * @param key_size
 * @param value
 * @param v_size
 *
 * @return 
 */
int lmdb_nput(void* key,std::size_t key_size,
		void* value,std::size_t v_size){
	const auto status = mods::globals::db->nput(key,key_size,value,v_size);
	return status;
	//if(status == EINVAL){
	//	std::cerr << "[lmdb_put] got EINVAL -- retrying...\n";
	//	lmdb_renew();
	//	status = mods::globals::db->nput(key,key_size,value,v_size);
	//	std::cerr << "retry status: " << status << "\n";
	//}
	//return status;
}
/**
 * @brief places a string value into lmdb by key
 *
 * @param key
 * @param value
 */
void lmdb_put(std::string key,std::string value){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
	std::cerr << "[lmdb_put]: '" << key << "', value:'" << value << "'\n";
#endif
	auto status = mods::globals::db->put(key,value);
	if(status == EINVAL){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "[lmdb_put] got EINVAL -- retrying...\n";
#endif
		lmdb_renew();
		status = mods::globals::db->put(key,value);
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "retry status: " << status << "\n";
#endif
	}
}
/**
 * @brief deletes a string key
 *
 * @param key
 */
void lmdb_del(std::string key){
	mods::globals::db->del(key);
}
/**
 * @brief deletes a potentially binary key using untyped buffer
 *
 * @param key
 * @param key_size
 */
void lmdb_ndel(void* key,std::size_t key_size){
	mods::globals::db->ndel(key,key_size);
	//if(status == EINVAL){
	//	std::cerr << "[lmdb_put] got EINVAL -- retrying...\n";
	//	lmdb_renew();
	//	status = mods::globals::db->nput(key,key_size,value,v_size);
	//	std::cerr << "retry status: " << status << "\n";
	//}
	//return status;
}
/**
 * @brief commits all lmdb transactions
 */
void lmdb_commit(){
	mods::globals::db->commit();
}

/**
 * @brief exports and saves the player
 *
 * @param player_ptr
 *
 * @return 
 */
tuple_status_t save_char(
		player_ptr_t player_ptr){
	mutable_map_t values;
	lmdb_export_char(player_ptr,values);
	return save_record("player",&values,std::to_string(player_ptr->get_db_id()));
}
/**
 * @brief exports and saves a new player
 *
 * @param player_ptr
 *
 * @return 
 */
tuple_status_t save_new_char(
		player_ptr_t player_ptr){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
	std::cerr << "[DEPRECATED]: calls to mods::lmdb::save_new_char will be ignored!\n";
#endif
	//player_ptr->set_db_id(initialize_row("player"));
	mutable_map_t values;
	lmdb_export_char(player_ptr,values);
	return save_record("player",&values,values["id"]);
}

/**
 * @brief writes lmdb values to the db using table, values, and pk_id
 *
 * @param table
 * @param values
 * @param pk_id
 *
 * @return 
 */
tuple_status_t lmdb_write_values(
		const std::string& table,
		mutable_map_t* values,std::string pk_id){
	mods::globals::db->renew_txn();
	for(const auto & [key,value] : *values){
		 lmdb_put(db_key({table,key,pk_id}),value);
	}
	return {true,"saved",0};
}

/**
 * @brief grabs all relevant keys for a player and store them in the values map
 *
 * @param player_ptr
 * @param values
 */
void lmdb_export_char(player_ptr_t player_ptr, mutable_map_t &values){
	/** TODO: instead of using the char_data accesses, create functions(or use existing ones) on mods::player object */
	auto ch = player_ptr->cd();
		values["player_password"] = player_ptr->password();
		values["player_affection_plr_bitvector"] = std::to_string(player_ptr->get_affected_plr());
		values["player_affection_bitvector"] = std::to_string(player_ptr->get_affected());

		if(values["player_affection_plr_bitvector"].length() == 0){
			values.erase("player_affection_plr_bitvector");
		}
		if(values["player_affection_bitvector"].length() == 0){
			values.erase("player_affection_bitvector");
		}

		values["player_name"] = player_ptr->name().c_str();
		values["player_short_description"] = std::to_string(ch->player.short_descr);
		values["player_long_description"] = std::to_string(ch->player.long_descr);
		values["player_action_bitvector"] = (std::to_string(ch->char_specials.saved.act));
		values["player_ability_strength"] = (std::to_string(ch->real_abils.str));
		values["player_ability_strength_add"] = (std::to_string(ch->real_abils.str_add));
		values["player_ability_intelligence"] = (std::to_string(ch->real_abils.intel));
		values["player_ability_wisdom"] = (std::to_string(ch->real_abils.wis));
		values["player_ability_dexterity"] = (std::to_string(ch->real_abils.dex));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_ability_charisma"] = (std::to_string(ch->real_abils.cha));
		values["player_ability_alignment"] = (std::to_string(ch->char_specials.saved.alignment));
		values["player_attack_type"] = (std::to_string(ch->real_abils.con));
		values["player_ability_constitution"] = (std::to_string(ch->real_abils.con));
		values["player_max_hitpoints"] = (std::to_string(player_ptr->max_hp()));
		values["player_max_mana"] = (std::to_string(player_ptr->max_mana()));
		values["player_max_move"] = (std::to_string(player_ptr->max_move()));
		values["player_gold"] = (std::to_string(player_ptr->gold()));
		values["player_exp"] = (std::to_string(player_ptr->exp()));
		values["player_sex"] = player_ptr->sex() == SEX_MALE ? std::string("M") : std::string("F");
		values["player_hitpoints"] = (std::to_string(player_ptr->hp()));
		values["player_mana"] = (std::to_string(player_ptr->mana()));
		values["player_move"] = (std::to_string(player_ptr->move()));
		values["player_damroll"] = (std::to_string(player_ptr->damroll()));
		values["player_weight"] = (std::to_string(player_ptr->weight()));
		values["player_height"] = (std::to_string(player_ptr->height()));
		values["player_class"] = std::to_string(player_ptr->get_class());
		values["player_title"] = std::to_string(player_ptr->title());
		values["player_hometown"] = (std::to_string(player_ptr->hometown()));
		values["player_damnodice"] = std::string("0");
		values["player_damsizedice"] = std::string("0");
		values["player_attack_type"] = std::string("0");
		values["player_type"] = std::string("PC");
		values["player_alignment"] = std::to_string(
			ch->char_specials.saved.alignment);
		values["player_level"] = std::to_string(player_ptr->level());
		values["player_hitroll"] = std::to_string(player_ptr->cd()->points.hitroll);
		values["player_armor"] = std::to_string(player_ptr->cd()->points.armor);
		values["player_preferences"] = std::to_string(player_ptr->get_prefs());
		return;
}

/**
 * @brief loads a record from table using pk and stores them in values
 *
 * @param table
 * @param pk
 * @param values
 *
 * @return 
 */
int load_record(const std::string& table, aligned_int_t pk, mutable_map_t& values){
	auto str_pk = std::to_string(pk);
	return load_record(table,str_pk,values);
}
/**
 * @brief loads a record from table using pk and stores them in values
 *
 * @param table
 * @param pk
 * @param values
 *
 * @return 
 */
int load_record(const std::string& table, const std::string& pk, mutable_map_t& values){
	std::string value;
	int count = 0;
	for(auto & key : mods::globals::db->fields_to_grab(table)){
		value = db_get(db_key({table,key,pk}));
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "debug: load_record key: '" << key << "' value: '" << value << "'\n";
#endif
		values[key] = value;
		++count;
	}
	return count;
}


/**
 * @brief loads a record according to metadata that matches the values pointer from table, stores them in out_record
 *
 * @param table
 * @param values
 * @param out_record
 *
 * @return 
 */
int load_record_by_meta(const std::string& table, mutable_map_t* values,mutable_map_t& out_record){
	if(values == nullptr){
#ifdef __MENTOC_SHOW_LMDB_DEBUG__
		std::cerr << "error: values is a nullptr in load_record_by_meta\n";
#endif
		return -1;
	}
	int count = 0;
	for(auto & meta_key : mods::meta_utils::get_all_meta_values(table,values)){
		std::string pk_id;
		pk_id = db_get(meta_key);
		auto opt_pk = mods::util::stoi_optional<aligned_int_t>(pk_id);
		if(opt_pk.has_value() && opt_pk.value() != 0){
			count = 0;
			//std::cout << "debug: found pk: '" << opt_pk.value() << "'\n";
			out_record["id"] = pk_id;
			std::vector<std::string> fields_to_grab = mods::schema::db[table];
			if(mods::globals::db->is_using_pluck_filter()){
				fields_to_grab = mods::globals::db->fields_to_grab(table);
			}
			for(auto & key : fields_to_grab){
				std::string fetched = db_get(db_key({table,key,pk_id}));
				//std::cout << "debug: load_record key: '" << key << "' value: '" << fetched << "'\n";
				out_record[key] = fetched;
				count++;
			}
			return count;
		}
	}
	return count;
}


};

std::string player_key(std::string player_name,std::string prefix, std::string type){
	return db_key({"player",player_name,prefix,type}) + "|";
}
std::string player_key_index(std::string player_name,std::string prefix, std::string type){
	return player_key(player_name.data(),prefix,type) + "index";
}
std::string player_key_count(std::string player_name,std::string prefix, std::string type){
	return player_key(player_name.data(),prefix,type) + "count";
}
int put_player_map(std::string_view player_name,std::string prefix, std::map<std::string,std::string> values){
	mods::db::lmdb_renew();
	std::string key = "";
	key = player_key(player_name.data(),prefix,"map");
	int i =0;
	std::string keys;
	for(auto & pair : values){
		keys += pair.first + "|";
		mods::db::lmdb_put(key + sanitize_key(pair.first),pair.second);
		++i;
	}
	mods::db::lmdb_put(player_key_index(player_name.data(),prefix,"map"),keys);
	mods::db::lmdb_commit();
	return i;
}

int get_player_map(std::string_view player_name,std::string prefix, std::map<std::string,std::string>& values){
	mods::db::lmdb_renew();
	std::string key = "";
	key = player_key(player_name.data(),prefix,"map");
	std::string index_key = player_key_index(player_name.data(),prefix,"map");
	int i= 0;
	std::string current = "";
	for(auto ch : mods::db::lmdb_get(index_key)){
		if(ch == '|' && current.length()){
			values[current] = mods::db::lmdb_get(key + current);
			current = "";
			continue;
		}
		current += ch;
	}
	mods::db::lmdb_commit();
	return i;
}
int put_player_vector(std::string_view player_name,std::string prefix, std::vector<std::string> values){
	mods::db::lmdb_renew();
	std::string key = "";
	key = player_key(player_name.data(),prefix,"vector");
	int i= 0;
	for(auto & val : values){
		mods::db::lmdb_put(key + std::to_string(i),val);
		++i;
	}
	mods::db::lmdb_put(player_key_count(player_name.data(),prefix,"vector"),std::to_string(i));
	mods::db::lmdb_commit();
	return i;
}

int get_player_vector(std::string_view player_name,std::string prefix, std::vector<std::string>& values){
	mods::db::lmdb_renew();
	std::string key = "";
	key = player_key(player_name.data(),prefix,"vector");
	std::string count_key = key + "count";
	int i= 0;
	auto count = atoi(mods::db::lmdb_get(player_key_count(player_name.data(),prefix,"vector")).c_str());
	for(; i < count;++i){
		std::string value = mods::db::lmdb_get(key + std::to_string(i));
		values.emplace_back(value);
	}
	mods::db::lmdb_commit();
	return i;
}

bool key_exists(std::string key){
	return mods::db::lmdb_exists(key);
}
