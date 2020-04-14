#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__ 1


#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <set>
#include "types.hpp"
#include "mods/acl/config-parser.hpp"
#include "conf.h"
#include "config.hpp"
#include "sysdep.h"
#include "structs.h"
#include "mods/player.hpp"
#include <memory>
#include "mods/deferred.hpp"
#include "mods/lmdb.hpp"
#include "mods/projectile.hpp"
#include "duktape/src/duktape.h"
#include "mods/js.hpp"
#include "mods/drone.hpp"
#include "mods/conf.hpp"
#include "mods/chat.hpp"
#include "mods/crypto.hpp"
#include "mods/affects.hpp"
#include <deque>
#include "db.h"
#include <pqxx/pqxx>
#include <unordered_map>
#include "mods/util.hpp"

#define MENTOC_PREAMBLE() auto player = IS_NPC(ch) ? std::make_shared<mods::player>(ch) \
																				: mods::globals::socket_map[ch->desc->descriptor];

#define MENTOC_DEFER(secs,lambda) mods::globals::defer_queue->push_secs(secs,lambda);
#define IS_DIRECTION(a) (strcmp(a,"north") == 0 || strcmp(a,"south") == 0 || \
		strcmp(a,"east") == 0 || strcmp(a,"west") == 0 || strcmp(a,"up") == 0 || strcmp(a,"down") == 0)
#define OPPOSITE_DIR(a) mods::globals::opposite_dir(a)
#define DBSET(key,value){ mods::globals::ram_db[key] = value; }
#define DBGET(key,value) { value = mods::globals::ram_db[key]; }

#define LMDBNSET(_lmdb_n_set_key,_lmdb_n_set_k_size,_lmdb_n_set_value,_lmdb_n_set_v_size) mods::db::lmdb_nput(_lmdb_n_set_key,_lmdb_n_set_k_size,_lmdb_n_set_value,_lmdb_n_set_v_size)

#define LMDBSET(key,value) mods::db::lmdb_put(key,value)
#define LMDBNGET(key,k_size,value_buffer) mods::db::lmdb_nget(key,k_size,value_buffer)
#define LMDBVGET(key,k_size,value_buffer) mods::db::lmdb_nget_void(key,k_size,value_buffer)
#define LMDBNDEL(key,k_size) mods::db::lmdb_ndel(key,k_size)
#define LMDBDEL(key) mods::db::lmdb_del(key)
#define LMDBGET(key) mods::db::lmdb_get(key)
#define LMDBCOMMIT() mods::db::lmdb_commit()
#define LMDBRENEW() mods::db::lmdb_renew()
#define CREATE_ARG(size,m) std::array<char,size> arg_##m ; std::fill(arg_##m.begin(),arg_##m.end(),0);
/** vim-sorcery: :%s/std::cerr << \(.*\);/d(\1);/ */
#ifdef __MENTOC_DEBUG__
#define d(a) std::cerr << "[**DEBUG**]->[file:" << __FILE__ << "][line:" << __LINE__ << "][msg]: " << a << "\n" << std::flush;
#else
#define d(a) /** */
#endif
#include <string>
#define tostr(a) std::to_string(a)
#define svtostr(a) std::string(a.data())
/*
	template<typename... Args>
		std::string str(Args const&... args)
		{
			std::string result;
			int unpack[]{0, (result += std::to_string(args), 0)...};
			static_cast<void>(unpack);
			return result;
		}
		*/

extern void clear_char(char_data*);
extern char_data* character_list;
//extern std::deque<mods::npc> mob_list;
#define CREATE_CHAR(ch) \
	CREATE(ch,char_data,1);\
clear_char(ch);\
ch->player_specials = std::make_unique<player_special_data>();\
(ch)->player_specials->saved.pref = 0;\
CREATE((ch)->affected, affected_type, 1);\
memset(&((ch)->affected),0,sizeof((ch)->affected));\
(ch)->next = character_list;\
character_list = ch;

using player_ptr_t = std::shared_ptr<mods::player>;
using obj_ptr_t = std::shared_ptr<obj_data>;
namespace mods {
	namespace lmdb { 
		struct _db_handle;
	};
	namespace globals {
		using player_list_t = std::vector<player_ptr_t>;
		using lmdb_db = mods::lmdb::_db_handle;
		using socket_map_t = std::map<int,player_ptr_t>;
		using room_list_t = std::vector<std::vector<player_ptr_t>>;
		enum boot_type_t { BOOT_DB,BOOT_HELL };
		bool acl_allowed(char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd);
		void init(int,char**);
		void pre_game_loop();
		void load_player_map();
		uuid_t player_uuid();
		uuid_t mob_uuid();
		uuid_t obj_uuid();
		extern boot_type_t boot_type;
		extern map_object_list obj_map;
		extern socket_map_t socket_map;
		extern player_ptr_t player_nobody;
		extern std::map<obj_data*,obj_ptr_t> obj_odmap;
		extern std::map<uuid_t,std::shared_ptr<mods::npc>> mob_map;
		extern std::map<char_data*,std::shared_ptr<mods::npc>> mob_chmap;
		extern std::map<char_data*,std::shared_ptr<mods::player>> player_chmap;
		extern std::unique_ptr<mods::deferred> defer_queue;
		extern std::unique_ptr<lmdb_db> db;
		extern std::map<const char*,player_ptr_t> player_name_map;
		extern std::map<uuid_t,player_ptr_t> player_map;
		extern ai_state_map states;
		extern duk_context* duktape_context;
		extern room_list_t room_list;
		extern player_list_t player_list;
		extern std::vector<mods::chat::channel> chan;
		extern std::vector<std::string> chan_verbs;
		extern bool f_import_rooms;
		extern player_ptr_t current_player;
		extern std::string bootup_test_suite;
		extern std::unique_ptr<pqxx::connection> pq_con;
		extern std::unordered_map<std::string,std::string> ram_db;
		extern std::set<player_ptr_t> needs_dissolve;
		void init_player(char_data*);
		std::unique_ptr<ai_state>& state_fetch(char_data* ch);
		int mobile_activity(char_data*);
		char_data* read_mobile(const mob_vnum &,const int & type);
		void register_player(player_ptr_t);
		//void register_object_db_id(obj_ptr_t);
		void register_object(obj_ptr_t);
		template <typename TShopPtr>
		void register_shop(TShopPtr shop){
			static uuid_t my_uuid = 0;
			shop->uuid = ++my_uuid;
		}
		void register_object_list();
		void register_room(const room_rnum&);
		void register_object_db_id(uint64_t,uuid_t);
		void shutdown();
		void refresh_player_states();
		void room_event(char_data*,mods::ai_state::event_type_t);
		void room_event(room_vnum,mods::ai_state::event_type_t);
		void post_boot_db();
		char_data* create_char();
		int file_to_lmdb(const std::string& file,const std::string& key);
		int opposite_dir(int);
		std::string color_eval(std::string_view buffer);
		std::string replace_all(std::string str, const std::string& from, const std::string& to);
		const char* say_random(const mods::ai_state::event_type_t&);
		bool command_interpreter(player_ptr_t,const std::string& argument);
		void post_command_interpreter(char_data *ch, char* argument);
		int dir_int(char);
		void affect_room_light(int room,int offset);
		namespace rooms {
			void char_from_room(char_data*);
			void char_to_room(const room_rnum &,char_data*);
		};
		void pad_room(int room,char_data* ch,int door);
		player_list_t& get_room_list(room_rnum);
		player_list_t& get_room_list(player_ptr_t&);
		std::string dir_to_str(int dir, bool adjective);
		void queue_object_destruct(uuid_t obj_uuid, uint16_t seconds);
		void destruct_object(uuid_t obj);
	};
};
extern std::deque<std::shared_ptr<obj_data>> obj_list;
extern std::deque<std::shared_ptr<mods::npc>> mob_list;

player_ptr_t ptr(char_data*);
obj_ptr_t optr(obj_data*);
player_ptr_t ptr_by_uuid(uuid_t);
obj_ptr_t optr_by_uuid(uuid_t);
std::optional<obj_ptr_t> optr_opt(uuid_t);
std::optional<player_ptr_t> ptr_opt(uuid_t);
obj_ptr_t ptr_by_dbid(uint64_t);
#define PARSE_ARGS() mods::util::arglist<std::vector<std::string>>(std::string(argument))
#endif
