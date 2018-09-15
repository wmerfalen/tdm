#include "../../globals.hpp"
#include "../lmdb.hpp"
#include <string_view>
#include <string>

using namespace mods::lmdb;
using aligned_int_t = uint64_t; 
#define debug(d) std::cerr << "[debug]: " << d << "\n";
void initialize_row(std::string_view table,db_handle* ptr_db){
	std::string id_list;
	debug("getting id_list of table: " << table.data() << "\n");
	auto ret = ptr_db->get(db_key({table.data(),"id_list"}),id_list);
	debug("fetched");
	if(ret < 0){
		std::cerr << "initialize_row failed to get id_list: " << ret << "\n";
		return;
	}

	aligned_int_t next_id = 0;
	aligned_int_t num_of_ids = 0;
	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.begin() == id_list.end()){
		debug("empty list found in lmdb");
		deserialized_id_list.push_back(0);
		next_id = 0;
	}
	else{
		debug("copying...");
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
		debug("copy complete");
		num_of_ids = deserialized_id_list[0];
		debug("number of ids currently: " << num_of_ids);
	}
	++next_id;
	if(next_id == 0){
		std::cerr << "[WARNING] lmdb NEXT_ID has reached ZERO. This is UNSIGNED INTEGER OVERFLOW!!!\n";
	}

	id_list.clear();
	deserialized_id_list[0] += 1;

	ret = ptr_db->set(db_key({table.data(),std::to_string(next_id)}),"active");
	if(ret < 0){
		std::cerr << "initialize_row failed to set id: " << ret << "\n";
		return;
	}
}

namespace mods::testing::lmdb{
struct db {
db(int argc,char** argv){
	using mods::lmdb;
	auto db_ptr = std::make_unique<mods::lmdb::db_handle>(LMDB_DB_DIRECTORY,LMDB_DB_NAME,MDB_WRITEMAP | MDB_NOLOCK,0600,true);
	initialize_row("player",db_ptr.get());
}
~db() = default;
};
};
