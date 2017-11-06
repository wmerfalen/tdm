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
#include "mods/lmdb/db.hpp"
#include "mods/projectile.hpp"
#include "duktape/src/duktape.h"
#include "mods/js.hpp"

#define MENTOC_PREAMBLE() auto player = mods::globals::players::get(ch->uuid); player->set_cd(ch); 
#define MENTOC_DEFER(secs,lambda) mods::globals::defer_queue->push_secs(secs,lambda);
#define IS_DIRECTION(a) (strcmp(a,"north") == 0 || strcmp(a,"south") == 0 || \
strcmp(a,"east") == 0 || strcmp(a,"west") == 0 || strcmp(a,"up") == 0 || strcmp(a,"down") == 0)
#define OPPOSITE_DIR(a) mods::globals::opposite_dir(a)
#define DBSET(key,value) mods::globals::db->put(key,value);
#define DBGET(key,value) mods::globals::db->get(key,value);
#define CREATE_ARG(size,m) std::array<char,size> arg_##m ; std::fill(arg_##m.begin(),arg_##m.end(),0);
namespace mods {
    namespace globals {
		using lmdb_db = gdns::lmdb::db;
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init();
		void pre_game_loop();
		void load_player_map();
		uuid_t get_uuid();
		uuid_t obj_uuid();
		typedef std::map<uuid_t,std::shared_ptr<mods::player>> map_player_list;
		extern map_player_list player_map;
		extern map_object_list obj_map;
		extern std::shared_ptr<mods::player> player_nobody;
		extern std::unique_ptr<mods::deferred> defer_queue;
		extern std::unique_ptr<lmdb_db> db;
		extern ai_state_map states;
		extern duk_context* duktape_context;
		std::unique_ptr<ai_state>& state_fetch(struct char_data* ch);
		int mobile_activity(char_data*);
		void register_object(obj_data&);
		void register_object_list();
		void refresh_player_states();
		void room_event(struct char_data*,mods::ai_state::event_type_t);
		void room_event(room_vnum,mods::ai_state::event_type_t);
		void post_boot_db();
		int file_to_lmdb(const std::string& file,const std::string& key);
		int opposite_dir(int);
    	std::string color_eval(std::string final_buffer);
		std::string replace_all(std::string str, const std::string& from, const std::string& to);
		const char* say_random(const mods::ai_state::event_type_t&);
		bool command_interpreter(struct char_data *ch,char* argument);
		void post_command_interpreter(struct char_data *ch, char* argument);
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
		};
		/*
		namespace players {
        	template <typename T> std::shared_ptr<mods::player>& get(T ch);
			std::shared_ptr<mods::player>& name(const std::string & n);
		};
		*/
    };
	namespace utils {
		int stoi(const std::string & str,int & in_int);
	};
};


#endif
