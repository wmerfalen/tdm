#ifndef __MENTOC_MODS_DB_LIB__
#define __MENTOC_MODS_DB_LIB__
#include "lmdb.hpp"
#include <sstream>

using aligned_int_t = uint64_t; 
#ifdef __MENTOC_DEBUG__
#define debug(d) std::cerr << "[debug]: " << d << "\n";
#else
#define debug(d) /** */
#endif

namespace mods::lmdb::error { 
	using int_status_t = int64_t;
	constexpr static int_status_t success = 0;
	constexpr static int_status_t e_put = -1;
	constexpr static int_status_t e_commit = -2;
	constexpr static int_status_t e_get = -3;
};

using int_status_t = mods::lmdb::error::int_status_t;
using db_handle = mods::lmdb::db_handle;
using tuple_status_t  = std::tuple<bool,std::string,aligned_int_t>;
static inline int_status_t initialize_table_schema(std::string_view table,db_handle* ptr_db){
	ptr_db->renew_txn();

	debug("initialize_table_schema: table: '" << table.data() << "'");
	std::string str_buffer;
	assert(mods::schema::db.find(std::string(table.data())) != 
			mods::schema::db.end());
	for(auto & m : mods::schema::db[std::string(table)]){
		str_buffer += m + ",";
	}
	assert(str_buffer.length() > 0);
	str_buffer.resize(str_buffer.length()-1);
	debug("initialize_table_schema str_buffer: " << str_buffer.c_str());
	auto ret = ptr_db->put(db_key({table,"schema"}),str_buffer);
	if(ret < 0){
		ptr_db->dump_status();
		return mods::lmdb::error::e_put;
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		debug("initialize_table_schema commit succeeded!\n");
	}else{
		return mods::lmdb::error::e_commit;
	}
	return mods::lmdb::error::success;
}


/**
 * initliaze_row
 */
static inline aligned_int_t initialize_row(
		std::string_view table,
		db_handle* ptr_db,
		bool & error){
	ptr_db->renew_txn();
	std::string id_list;
	auto ret = ptr_db->get(db_key({table,"id_list"}),id_list);
	if(ret < 0){
		ptr_db->dump_status();
		error = true;
		ptr_db->abort_txn();
		return 0;
	}

	aligned_int_t next_id = 0;
	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.begin() == id_list.end()){
		deserialized_id_list.push_back(0);
		next_id = 0;
	}
	else{
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
	}
	++next_id;
	deserialized_id_list[0] = next_id;
	id_list.clear();
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	ret = ptr_db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
		ptr_db->dump_status();
		error = true;
		ptr_db->abort_txn();
		return 0;
	}

	ret = ptr_db->put(db_key({table.data(),std::to_string(next_id)}),"active");
	if(ret < 0){
		ptr_db->dump_status();
		error = true;
		ptr_db->abort_txn();
		return 0;
	}else{
		debug("initialize_row put payload succeeded!");
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		debug("initialize_row commit succeeded!\n");
	}else{
	}
	error = false;
	return next_id;
}

/**
 * new_record
 */
inline tuple_status_t new_record_with_values(std::string_view table,
		db_handle* ptr_db,
		mods::lmdb::mutable_map_t & values,
		bool & error,
		std::optional<aligned_int_t> use_this_id){
	bool init_error = false;
	aligned_int_t id = 0;
	if(!use_this_id.has_value()){
		id = initialize_row(table,ptr_db,init_error);
		if(init_error){
			return {false,"init-error",0};
		}
	}else{
		id = use_this_id.value();
	}

	if(mods::schema::db.find(std::string(table.data())) != 
			mods::schema::db.end()){
		ptr_db->renew_txn();
		for(auto & column : mods::schema::db[table.data()]){
			std::string key = db_key({table.data(),column,std::to_string(id)});
			debug("new_record: Creating key: '" << key << "'");	
			int ret = 0;
			if(values.size()){
				ret = ptr_db->put(key,mods::schema::PLACE_HOLDER_VALUE);
			}else{
				std::string value = values[key];
				if(value.length()){
					ret = ptr_db->put(key,value);
				}else{
					debug("new_record: '" << key << "' is empty. skipping");
				}
			}
			if(ret == 0){
				debug("key '" << key << "' was successfully placed in '" << table.data() << "'");
			}else{
				debug("failed to place key '" << key << "' in table '" << table.data() << "'");
			}
		}
		auto tuple_return = ptr_db->commit();
		if(std::get<0>(tuple_return)){
			debug("new_record commit succeeded!\n");
		}else{
			debug("failed to commit new record on table '" << table.data() << "'");
			error = true;
			return {false,"commit",id};
		}
		return {true,"success",id};
	}else{
		error = true;
		return {false,"missing-table",id};
	}
}
static inline tuple_status_t new_record(
		std::string_view table,db_handle* ptr_db,bool & error){
	mods::lmdb::mutable_map_t empty_map;
	return new_record_with_values(table,
			ptr_db,
			empty_map,
			error,{}
			);
}
namespace mods::db{

using aligned_int_t = uint64_t; 
void lmdb_export_char(
		char_data* ch, 
		mods::lmdb::mutable_map_t &
);

using tuple_status_t = std::tuple<bool,std::string,aligned_int_t>;
using mutable_map_t = ::mods::lmdb::mutable_map_t;

tuple_status_t lmdb_load_by_meta(
		std::string_view meta_key,
		std::string_view meta_value,
		std::string_view table,
		db_handle* ptr_db,
		mutable_map_t& values
);

tuple_status_t lmdb_commit(
		db_handle*,
		std::string_view
);

tuple_status_t lmdb_write_meta_values(
		std::string_view,
		db_handle*,
		std::map<std::string,std::string>);

tuple_status_t lmdb_write_values(
		std::string_view str_id,
		std::string_view table,
		db_handle* ptr_db,
		const mods::lmdb::mutable_map_t& values);

tuple_status_t lmdb_insert_char(
		std::string_view name, 
		db_handle*,
		const mods::lmdb::mutable_map_t &);

tuple_status_t lmdb_update_char(
		std::string_view,
		db_handle*,
		const mods::lmdb::mutable_map_t &,
		std::string_view);

tuple_status_t lmdb_save_char(
		std::string_view,
		char_data *,
		db_handle* ptr_db);

#define lmdb_return_if_fail(function_call,from_where)\
{\
	auto tuple_return = function_call;\
	if(std::get<0>(tuple_return)){\
		debug(from_where << " succeeded!\n");\
	}else{\
		return tuple_status_t({false,std::string(from_where) + std::string(" failed: '") + std::string(std::get<1>(tuple_return)) + "'",0});\
	}\
}

#define lmdb_return(function_call,from_where)\
{\
	auto tuple_return = function_call;\
	if(std::get<0>(tuple_return)){\
		debug(from_where << " succeeded!\n");\
		return {true,"",0};\
	}else{\
		return {false,std::string(from_where) + std::string(" failed: '") + std::string(std::get<1>(tuple_return)) + "'",0};\
	}\
}

#define lmdb_get(ptr_db,key,value,okay_func,ill_func)\
	ptr_db->renew_txn();\
	if(ptr_db->get(\
		key,value\
	) == db_handle::KEY_FETCHED_OKAY){okay_func;}\
	else{ill_func ;}

#define lmdb_put(ptr_db,key,value,okay_func,ill_func)\
	ptr_db->renew_txn();\
	if(0 == ptr_db->put(\
		key,value\
	)){ okay_func; }else{ill_func;}

#define lmdb_del(ptr_db,key)\
	ptr_db->renew_txn();\
	ptr_db->del(key);

};//End namespace

#endif
