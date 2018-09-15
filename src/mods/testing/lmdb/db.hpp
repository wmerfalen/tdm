#ifndef __MENTOC_MODS_TESTING_LMDB_DB_HEADER__
#define __MENTOC_MODS_TESTING_LMDB_DB_HEADER__
using aligned_int_t = uint64_t; 

namespace mods::testing::lmdb{
void initialize_row(std::string_view table,db_handle* ptr_db);
struct db {
	db(int argc,char** argv);
};
};

#endif
