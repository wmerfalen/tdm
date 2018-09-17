#include "../../globals.hpp"
#include "../lmdb.hpp"
#include <string_view>
#include <string>
#include "../../testing_index.hpp"

using namespace mods::lmdb;
using aligned_int_t = uint64_t; 
#define debug(d) std::cerr << "[debug]: " << d << "\n";
void initialize_row(std::string_view table,db_handle* ptr_db){
	std::string id_list;
	debug("getting id_list of table: " << table.data() << "\n");
	debug("db_key for id_list: '" << db_key({"player","id_list","foobar"}) << "'\n");
	auto ret = ptr_db->get(db_key({table.data(),"id_list"}),id_list);
	debug("fetched");
	if(ret < 0){
		std::cerr << "initialize_row failed to get id_list: " << ret << "\n";
		ptr_db->dump_status();
		return;
	}

	debug("Size of retrieved list: " << id_list.size() << "\n");

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
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer;
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	ret = ptr_db->put(db_key({table.data(),"id_list"}),str_buffer);
	if(ret < 0){
		std::cerr << "initialize_row failed to set id: " << ret << "\n";
		ptr_db->dump_status();
		return;
	}

	ret = ptr_db->put(db_key({table.data(),std::to_string(next_id)}),"active");
	if(ret < 0){
		std::cerr << "initialize_row failed to set id: " << ret << "\n";
		ptr_db->dump_status();
		return;
	}else{
		std::cerr << "initialize_row put payload succeeded!\n";
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		std::cerr << "initialize_row commit succeeded!\n";
	}else{
		std::cerr << "initialize_row commit FAILED: " << std::get<1>(tuple_return) << "\n";
	}
}

namespace mods::testing::lmdb {
	db::db(int argc,char** argv){

initialize_row("player",mods::globals::db.get());
	}
};
