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
#include "mods/drone.hpp"
#include <pqxx/pqxx>
#include "mods/conf.hpp"
#include "mods/chat.hpp"

#define MENTOC_PREAMBLE() auto player = std::make_shared<mods::player>(&mods::globals::player_list.at(ch->uuid));
#define MENTOC_DEFER(secs,lambda) mods::globals::defer_queue->push_secs(secs,lambda);
#define IS_DIRECTION(a) (strcmp(a,"north") == 0 || strcmp(a,"south") == 0 || \
strcmp(a,"east") == 0 || strcmp(a,"west") == 0 || strcmp(a,"up") == 0 || strcmp(a,"down") == 0)
#define OPPOSITE_DIR(a) mods::globals::opposite_dir(a)
#define DBSET(key,value) mods::globals::db->put(key,value);
#define DBGET(key,value) mods::globals::db->get(key,value);
#define CREATE_ARG(size,m) std::array<char,size> arg_##m ; std::fill(arg_##m.begin(),arg_##m.end(),0);
extern void clear_char(struct char_data*);
extern struct char_data* character_list;
#define CREATE_CHAR(ch) \
	CREATE(ch,struct char_data,1);\
	clear_char(ch);\
	ch->player_specials = (struct player_special_data*) calloc(1,sizeof( struct player_special_data));\
	memset(&((ch)->player_specials),0,sizeof((ch)->player_specials));\
	std::cerr << (int)ch->player_specials << "\r\n";\
	(ch)->player_specials->poofin = (ch)->player_specials->poofout = strdup("poof");\
	(ch)->player_specials->saved.pref = 0;\
	CREATE((ch)->affected, struct affected_type, 1);\
	memset(&((ch)->affected),0,sizeof((ch)->affected));\
	(ch)->next = character_list;\
	character_list = ch;
namespace mods {
    namespace globals {
		using lmdb_db = gdns::lmdb::db;
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init();
		void pre_game_loop();
		void load_player_map();
		uuid_t get_uuid();
		uuid_t obj_uuid();
		extern map_object_list obj_map;
		extern std::shared_ptr<mods::player> player_nobody;
		extern std::unique_ptr<mods::deferred> defer_queue;
		extern std::unique_ptr<lmdb_db> db;
		extern std::unique_ptr<pqxx::connection> pq_con;
		extern ai_state_map states;
		extern duk_context* duktape_context;
		extern std::vector<std::vector<struct char_data*>> room_list;
		extern std::vector<mods::player> player_list;
		extern std::vector<mods::chat::channel> chan;
		extern std::vector<std::string> chan_verbs;
		extern bool f_import_rooms;
		std::unique_ptr<ai_state>& state_fetch(struct char_data* ch);
		int mobile_activity(char_data*);
		void register_object(obj_data&);
		void register_object_list();
		void register_room(const room_rnum&);
		void register_player(struct char_data* ch);
		void refresh_player_states();
		void room_event(struct char_data*,mods::ai_state::event_type_t);
		void room_event(room_vnum,mods::ai_state::event_type_t);
		void post_boot_db();
		struct char_data* create_char();
		int file_to_lmdb(const std::string& file,const std::string& key);
		int opposite_dir(int);
    	std::string color_eval(std::string final_buffer);
		std::string replace_all(std::string str, const std::string& from, const std::string& to);
		const char* say_random(const mods::ai_state::event_type_t&);
		bool command_interpreter(struct char_data *ch,char* argument);
		void post_command_interpreter(struct char_data *ch, char* argument);
		int dir_int(char);
		namespace rooms {
			void char_from_room(struct char_data*);
			void char_to_room(const room_rnum &,struct char_data*);
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
