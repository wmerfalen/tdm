#if 0
/**
 * Please note: This program is slow. It will get the job
 * done, but if you're looking to replicate this functionality
 * where performance matters, do not copy and paste this.
 * -wm 2018-09-30 
 */
#include "../mods/schema.hpp"
#include "../mods/db.hpp"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <functional>
#include <string>

#ifndef debug
#define debug(a){ std::cerr << "[debug]->" << a << "\n"; }
#endif
std::shared_ptr<db_handle> db;
void usage(){
	std::cerr << "usage: ./a.out <lmdb-dir> <lmdb-name> <lmdb-file>\n" <<
		"example: ./a.out /tmp/bnull-mud bnull rooms.lmdb\n";
}
using aligned_int_t = uint64_t;
static inline void clear_id_list(std::string_view table){
	std::string str_buffer = "";
	db->renew_txn();
	db->del(db_key({table,"id_list"}));
	db->commit();
}
static inline aligned_int_t insert_row(aligned_int_t id,std::string_view table,bool & error){
	db->renew_txn();
	std::string id_list;
	debug("getting id_list of table: " << table.data() << "\n");
	debug("db_key for id_list: '" << db_key({"player","id_list"}) << "'\n");
	auto ret = db->get(db_key({table,"id_list"}),id_list);
	if(ret < 0){
		db->dump_status();
		error = true;
		db->abort_txn();
		return 0;
	}
	debug("fetched");

	debug("Size of retrieved list: " << id_list.size() << "\n");

	std::vector<aligned_int_t> deserialized_id_list;
	if(id_list.begin() == id_list.end()){
		debug("empty list found in lmdb");
		deserialized_id_list.push_back(0);
	}
	else{
		debug("copying...");
		std::copy(id_list.begin(),id_list.end(),std::back_inserter(deserialized_id_list));
		debug("copy complete");
		debug("number of ids currently: " << deserialized_id_list[0]);
	}
	++deserialized_id_list[0];
	id_list.clear();
	deserialized_id_list.push_back(id);
	debug("saving deserialized_id_list. new size of: " << deserialized_id_list.size());
	std::string str_buffer{""};
	std::copy(deserialized_id_list.begin(),deserialized_id_list.end(),
			std::back_inserter(str_buffer));
	ret = db->put(db_key({table,"id_list"}),str_buffer);
	if(ret < 0){
		db->dump_status();
		error = true;
		db->abort_txn();
		return 0;
	}

	ret = db->put(db_key({table.data(),std::to_string(id)}),"active");
	if(ret < 0){
		db->dump_status();
		error = true;
		db->abort_txn();
		return 0;
	}else{
		debug("initialize_row put payload succeeded!");
	}
	auto tuple_return = db->commit();
	if(std::get<0>(tuple_return)){
		debug("initialize_row commit succeeded!\n");
	}
	error = false;
	return id;
}

int port_main(int argc,char** argv){
	if(argc < 4){
		usage();
		return 1;
	}

	FILE* fp = fopen(std::string(argv[3]).c_str(),"r");
	if(!fp){
		std::cerr << "[error]: Can't open file: '" << argv[1] << "'\n";
		return 2;
	}
	else{
		auto lmdb_dir = argv[1];
		auto lmdb_name = argv[2];
		std::cerr << "lmdb_dir: '" << lmdb_dir << "' name: '" << lmdb_name << "\n";
		db = std::make_shared<db_handle>(lmdb_dir,lmdb_name,MDB_WRITEMAP,0600,true);
		if(!db->open()){
			db->dump_status();
			return 3;
		}
		constexpr uint32_t fsize = 1024*1024*10;
		uint64_t line_num = 0;
		uint64_t ci = 0;
		std::string uid = "";
		auto lambda = [&uid](std::string table,short column,std::string value){
			std::cerr << "table: " << table.data() << ", column: " << 
				column << ", value: '" << value.c_str() << "'\n";
						if(column == 0){
							uid = value;
							std::stringstream stream;
							stream << uid;
							aligned_int_t real_id;
							stream >> real_id;
							bool error = 0;
							auto ret = insert_row(real_id,table,error);
							if(ret == 0 || error){
								std::cerr << "error putting id: " << uid << "\n";
							}
						}
						db->renew_txn();
						if(mods::schema::db[table][column].length()){
							auto ret = db->put(db_key({
									table.data(),
									mods::schema::db[table][column].c_str(),
									uid}),value);
							if(ret < 0){
								std::cerr << "error putting key: '" << 
									db_key({table.data(),
											mods::schema::db[table][column].c_str(),
											uid}).c_str() << "'\n";
							}
						}
						db->commit();
					};
		for(auto & table : mods::schema::db){
			std::cerr << "[clear_id_list]: " << table.first.c_str() << "\n";
			clear_id_list(table.first.c_str());
		}
		while(!feof(fp)){
			std::vector<char> buffer;
			buffer.reserve(fsize);
			std::fill(buffer.begin(),buffer.end(),0);
			++line_num;
			ci = 0;
			uid = "";
			if(nullptr == fgets(&buffer[0],fsize-1,fp)){
				std::cerr << "encountered error on line: " << line_num << "\n";
				std::cerr << "attempting to continue...\n";
				continue;
			}else{
				ci = 0;
				bool table_parsed = false;
				std::string key = "";
				std::string value = "";
				std::string table = "";
				key = "";
				table = "";
				do {
					if(!table_parsed){
						do{
							table += buffer[ci++];
						}while(buffer[ci] != ':');
						table_parsed = true;
						++ci;
						continue;
					}
					uid = "0";
					value = "";
					bool escaped = 0;
					bool b_quote = 0;
					escaped = 0;
					b_quote = 0;
					short column_ctr;
					column_ctr = 0;
					do{
						if(b_quote && buffer[ci] == '\\'){
							escaped = true;
							++ci;
							continue;
						}
						if(buffer[ci] == '\''){
							if(b_quote){
								if(escaped){
									value += buffer[ci];
									escaped = false;
									++ci;
									continue;
								}
								b_quote = false;
								++ci;
								lambda(table,column_ctr++,value);
								value = "";
								++ci;
								continue;
							}else{
								b_quote = true;
								++ci;
								continue;
							}
						}
						if(b_quote && buffer[ci] == ' '){
							value += buffer[ci];
							++ci;
							continue;
						}
						if(!b_quote && buffer[ci] == ' '){
							++ci;
							continue;
						}
						if(!b_quote && buffer[ci] == ','){
							lambda(table,column_ctr++,value);
							value = "";
							++ci;
							continue;
						}
						value += buffer[ci];
						++ci;
						if(buffer[ci] == 0xa){
							lambda(table,column_ctr++,value);
						}
					}while(buffer[ci] != '\0' && buffer[ci] != 0xa);
					escaped = 0;
					b_quote = 0;
					if(value.length() == 0){
						break;
					}
				}while(buffer[ci] != 0xa && buffer[ci] != '\0');
				table_parsed = false;
			}
		}
	}
	db->close();
	fclose(fp);
	return 0;
}

#endif
