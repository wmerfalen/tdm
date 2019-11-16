#ifndef __MENTOC_MODS_DB_LIB__
#define __MENTOC_MODS_DB_LIB__
#include "lmdb.hpp"
#include <sstream>

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

using aligned_int_t = uint64_t; 
using int_status_t = mods::lmdb::error::int_status_t;
#ifndef __MENTOC_USE_LMDB_INSTEAD_OF_POSTGRES__
namespace mods::lmdb {
	struct _db_handle;
};
using db_handle = mods::lmdb::_db_handle;
using mutable_map_t = std::map<std::string,std::string>;
#else
using db_handle = mods::lmdb::db_handle;
using mutable_map_t = ::mods::lmdb::mutable_map_t;
#endif
using tuple_status_t  = std::tuple<bool,std::string,aligned_int_t>;

static inline int_status_t initialize_table_schema(std::string table){
	std::cerr << "deprecated: initialize_table_schema\n";
	return mods::lmdb::error::success;
}


namespace mods::globals {
	extern std::unique_ptr<db_handle> db;
};
/**
 * initliaze_row
 */
namespace mods::db{
	void lmdb_renew();
	void lmdb_commit();
void lmdb_put(std::string key,std::string value);
void lmdb_export_char(
		player_ptr_t player_ptr,
		mutable_map_t &
);

aligned_int_t initialize_row(
		const std::string& table);

aligned_int_t lmdb_load_by_meta(
		const std::string& table,
		mutable_map_t* values
);

tuple_status_t lmdb_write_values(
		const std::string & table,
		mutable_map_t* values,
		std::string pk_id
);

aligned_int_t save_record_get_id(
		const std::string& table,
		mutable_map_t* values
);
tuple_status_t new_record(
		const std::string& table,
		mutable_map_t* values
);

tuple_status_t save_record(
		const std::string& table,
		mutable_map_t* values,
		std::string pk_id
);

tuple_status_t save_char(
		player_ptr_t player_ptr
);
tuple_status_t save_new_char(
		player_ptr_t player_ptr
);
/**
 * @return int number of rows loaded 
 */
int load_record(const std::string& table, aligned_int_t pk, mutable_map_t& values);
/**
 * @return int number of rows loaded 
 */
int load_record(const std::string& table, const std::string& pk, mutable_map_t& values);
/**
 * @return int number of rows loaded 
 */
int load_record_by_meta(const std::string& table, mutable_map_t* values,mutable_map_t& out_record);

bool db_insert(const std::string& table, mutable_map_t* values);
};//End namespace

#endif
