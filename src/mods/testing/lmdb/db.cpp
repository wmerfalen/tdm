#include "../../globals.hpp"
#include "../lmdb.hpp"
#include <string_view>
#include <string>
#include "../../testing_index.hpp"

using namespace mods::lmdb;
using aligned_int_t = uint64_t; 
#define debug(d) std::cerr << "[debug]: " << d << "\n";
namespace mods::lmdb::error { 
	using int_status_t = int64_t;
	constexpr static int_status_t success = 0;
	constexpr static int_status_t e_put = -1;
	constexpr static int_status_t e_commit = -2;
	constexpr static int_status_t e_get = -3;
};

using int_status_t = mods::lmdb::error::int_status_t;

int_status_t initialize_table_schema(std::string_view table,db_handle* ptr_db){
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
		std::cerr << "initialize_table_schema failed to set schema: " << ret << "\n";
		ptr_db->dump_status();
		return mods::lmdb::error::e_put;
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		debug("initialize_table_schema commit succeeded!\n");
	}else{
		std::cerr << "initialize_table_schema commit FAILED: " << std::get<1>(tuple_return) << "\n";
		return mods::lmdb::error::e_commit;
	}
	return mods::lmdb::error::success;
}


/**
 * initliaze_row
 */
aligned_int_t initialize_row(std::string_view table,db_handle* ptr_db,bool & error){
	ptr_db->renew_txn();
	std::string id_list;
	debug("getting id_list of table: " << table.data() << "\n");
	debug("db_key for id_list: '" << db_key({"player","id_list","foobar"}) << "'\n");
	auto ret = ptr_db->get(db_key({table,"id_list"}),id_list);
	if(ret < 0){
		std::cerr << "initialize_row failed to get id_list: " << ret << "\n";
		ptr_db->dump_status();
		error = true;
		return 0;
	}
	debug("fetched");

	debug("Size of retrieved list: " << id_list.size() << "\n");

	aligned_int_t next_id = 0;
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
		debug("number of ids currently: " << deserialized_id_list[0]);
	}
	++deserialized_id_list[0];
	next_id = deserialized_id_list[0];
	if(next_id == 0){
		std::cerr << "[WARNING] lmdb NEXT_ID has reached ZERO. This is UNSIGNED INTEGER OVERFLOW!!!\n";
	}

	id_list.clear();
	deserialized_id_list.push_back(next_id);
	debug("saving deserialized_id_list with the next_id: "<< next_id << " and a new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	ret = ptr_db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
		std::cerr << "initialize_row failed to set id: " << ret << "\n";
		ptr_db->dump_status();
		error = true;
		return 0;
	}

	ret = ptr_db->put(db_key({table.data(),std::to_string(next_id)}),"active");
	if(ret < 0){
		std::cerr << "initialize_row failed to set id: " << ret << "\n";
		ptr_db->dump_status();
		error = true;
		return 0;
	}else{
		debug("initialize_row put payload succeeded!");
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		debug("initialize_row commit succeeded!\n");
	}else{
		std::cerr << "initialize_row commit FAILED: " << std::get<1>(tuple_return) << "\n";
	}
	error = false;
	return next_id;
}

/**
 * new_record
 */
aligned_int_t new_record(std::string_view table,db_handle* ptr_db,bool & error){
	bool init_error = false;
	auto id = initialize_row(table,ptr_db,init_error);
	if(init_error){
		std::cerr << "new_record['" << table.data() << "']: ERROR: Couldn't initialize row\n";
		return 0;
	}

	assert(mods::schema::db.find(std::string(table.data())) != 
			mods::schema::db.end());
	ptr_db->renew_txn();
	for(auto & column : mods::schema::db[table.data()]){
		std::string key = db_key({table.data(),column,std::to_string(id)});
		debug("new_record: Creating key: '" << key.c_str() << "'");	
		auto ret = ptr_db->put(key,"{}");
		if(ret == 0){
			debug("key was successfully placed");
		}else{
			std::cerr << "new_record: couldn't put key: '" << key.c_str() << "' " <<
				"reason: ";
			ptr_db->dump_status();
			error = true;
			return 0;
		}
	}
	auto tuple_return = ptr_db->commit();
	if(std::get<0>(tuple_return)){
		debug("new_record commit succeeded!\n");
	}else{
		std::cerr << "new_record commit FAILED: " << std::get<1>(tuple_return) << "\n";
	}
	error = false;
	return id;
}

namespace mods::testing::lmdb {
	db::db(int argc,char** argv){

		bool b_error = false;
		initialize_row("player",mods::globals::db.get(),b_error);
		initialize_table_schema("player",mods::globals::db.get());
		b_error = false;
		auto ret = new_record("player",mods::globals::db.get(),b_error);
		if(ret == 0 && b_error){
			std::cerr << "new_Record failed\n";
		}else{
			std::cerr << "[new_record] created\n";
		}

	}
};
