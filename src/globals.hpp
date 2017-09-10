#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__ 1

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "mods/acl/config-parser.hpp"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"

namespace mods {
    namespace globals {
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init();
		extern std::map<int,char_data> players;
    };

};


#endif
