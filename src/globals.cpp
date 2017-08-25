#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__ 1

#include "globals.hpp"

namespace mods {
    namespace globals {
		std::unique_ptr<mods::acl::FileParser> config;
		bool acl_good = false;
		int acl_parse_code = 0;
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd){
			return false;
		}
		void init(){
			config = std::make_unique<mods::acl::FileParser>();
			config->setFile("acl.conf");
			auto ret = config->parse();
			acl_parse_code = ret;
			if(ret < 0){
				std::cerr << "Error parsing config file: " << ret << " error code\n";
				acl_good = false;
			}else{
				acl_good = true;
			}
		}
    };

};


#endif
