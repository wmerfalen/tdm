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
#include "mods/player.hpp"
#include <memory>

namespace mods {
    namespace globals {
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init();
		typedef std::map<char_data*,std::shared_ptr<mods::player>> map_player_list;
		extern map_player_list player_map;
		namespace players {
			template <typename T> std::shared_ptr<mods::player>& get(T ch){
				return mods::globals::player_map.begin()->second;
			};
		};
    };

};


#endif
