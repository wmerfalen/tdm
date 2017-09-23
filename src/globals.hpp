#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__ 1


#include <iostream>
#include <string>
#include <memory>
#include <map>
#include "types.hpp"
#include "mods/acl/config-parser.hpp"
#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "mods/player.hpp"
#include <memory>
#include "mods/deferred.hpp"

#define MENTOC_PREAMBLE() auto player = mods::globals::players::get(ch->uuid); player->set_cd(ch); 
namespace mods {
    namespace globals {
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init();
		void load_player_map();
		uuid_t get_uuid();
		uuid_t obj_uuid();
		typedef std::map<uuid_t,std::shared_ptr<mods::player>> map_player_list;
		extern map_player_list player_map;
		extern map_object_list obj_map;
		extern std::shared_ptr<mods::player> player_nobody;
		extern std::unique_ptr<mods::deferred> defer_queue;
		void register_object(obj_data&);
		void register_object_list();
		namespace players {
        	template <typename T>
			inline std::shared_ptr<mods::player>& get(T ch){
				auto it = mods::globals::player_map.find(ch);
				if(it == mods::globals::player_map.end()){
					return player_nobody;
				}else{
					return mods::globals::player_map[ch];
				}
			}
			inline std::shared_ptr<mods::player>& name(const std::string & n){
				//TODO: Find a better way to do this, obbvvvvvviously implement it correctly
				return mods::globals::player_map.begin()->second;
			}
		};
		/*
		namespace players {
        	template <typename T> std::shared_ptr<mods::player>& get(T ch);
			std::shared_ptr<mods::player>& name(const std::string & n);
		};
		*/
    };

};


#endif
