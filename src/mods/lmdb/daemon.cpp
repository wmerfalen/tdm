#define GHOSTDNS_USE_SHM_CONFIG
#include <memory>
#define GDNS_STL
#define GDNS_STFU
#include "conf.hpp"

int main(int argc,char** argv){
	/** Parse config file */
	gdns::conf<std::string> c;
	if(c.parse() < 0){
		std::cerr << "Unable to parse config file!\n";
		return 1;
	}
	c.dump_list();
	/** Store entries in lmdb */
	std::unique_ptr<gdns::lmdb::server> db = std::make_unique<gdns::lmdb::server>("/tmp/ghostdns","ghostdns");
	if(db->good() == false){
		std::cerr << "failed to initialize lmdb\n";
		return 1;
	}
	std::string key="0",value = "bar";
	int ret = 0;
	/*
	int ret = db->get("foo",value);
	std::cout << "ret: " << ret << "\n";
	if(ret > 0){
		std::cout << "foo: " << value << "\n";
	}
	*/
	auto settings = c.settings();
	auto translations = c.translations();
	for(auto s : settings){
		std::cout << "setting: " << s.first << "\n";
	}
	for(auto t : translations){
		std::cout << "translation: " << t.first << "\n";
	}
	ret = db->put(key,value);
	//std::cout << "ret: " << ret << "\n";
	if(ret <= 0){
		std::cerr << "failed to save entries in lmdb\n";
		return 1;
	}
	/** Make lmdb entries read-only */
	
	/** idle forever :) */
    return 0;
}
