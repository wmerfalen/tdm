#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_HEADER__ 1


#include "structs.h"
#include "mods/deferred.hpp"
#include "mods/js.hpp"
//#include "mods/drone.hpp"
#include "mods/conf.hpp"
//#include "mods/chat.hpp"
#include "mods/crypto.hpp"
#include "mods/affects.hpp"
#include <deque>
#include "db.h"
#include <pqxx/pqxx>
#include <unordered_map>
#include "mods/util.hpp"
#include "mods/help.hpp"

#define MENTOC_PREAMBLE() auto player = ptr(ch)

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
#define MENTOC_DEPRECATED(mentoc_deprecated_argument_a) log("SYSERR:[%s] deprecated %s:%d",mentoc_deprecated_argument_a,__FILE__,__LINE__);
#define MOBTOSTR(__mob) "\n[mob_dump]\nname: '" << __mob->player.name.c_str() << "'\n"\
	"\n[mob_dump]\nuuid: '" << __mob->uuid() << "'\n"
#define DO_HELP(from) if(!mods::help::should_continue(from,argument,player,0)){ return; }
#define DO_HELP_WITH_ZERO(from) if(!mods::help::should_continue(from,argument,player,1)){ return; }

/** vim-sorcery: :%s/std::cerr << \(.*\);/d(\1);/ */
#ifndef __MENTOC_DEBUG__
#define __MENTOC_DEBUG__
#endif

#ifdef __MENTOC_DEBUG__
#ifndef d
#define d(a) std::cerr << "[**DEBUG**]->[file:" << __FILE__ << "][line:" << __LINE__ << "][msg]: " << a << "\n" << std::flush;
#endif
#else
#define d(a) ;
#endif
#include <string>
#define tostr(a) std::to_string(a)
#define svtostr(a) std::string(a.data())
extern void clear_char(char_data*);
extern char_data* character_list;
#define CREATE_CHAR(ch) \
	CREATE(ch,char_data,1);\
clear_char(ch);\
ch->player_specials = std::make_unique<player_special_data>();\
(ch)->player_specials->saved.pref = 0;\
CREATE((ch)->affected, affected_type, 1);\
memset(&((ch)->affected),0,sizeof((ch)->affected));\
(ch)->next = character_list;\
character_list = ch;
#define mentoc_prefix_debug(a) std::cerr << "[" << a << "]" << __FILE__ << "|" << __LINE__ << "->"

using player_ptr_t = std::shared_ptr<mods::player>;
using obj_ptr_t = std::shared_ptr<obj_data>;
using mob_ptr_t = std::shared_ptr<mods::npc>;
using str_map_t = std::map<std::string,std::string>;
using str_t = std::string;
using strview_t = std::string_view;
using mstr_t = mods::string;
using size_t = std::size_t;
using str_list_t = std::list<std::string>;
using str_vec_t = std::vector<std::string>;
using strvec_t = std::vector<std::string>;
using svvec_t = std::vector<std::string_view>;
using ivec_t = std::vector<int>;
using u64vec_t = std::vector<uint64_t>;
using uuidvec_t = std::vector<uuid_t>;
#define cstr(a) std::string(a).c_str()
#include <optional>
template <typename T>
using opt_t = std::optional<T>;
using optcd_t = opt_t<char_data*>;
using optobj_t = opt_t<obj_data*>;
template <typename T>
using vec_t = std::vector<T>;
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
		int init(int,char**);
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
		extern std::map<char_data*,std::shared_ptr<mods::player>> player_chmap;
		extern std::unique_ptr<mods::deferred> defer_queue;
		extern std::unique_ptr<lmdb_db> db;
		extern std::map<std::string,player_ptr_t> player_name_map;
		extern std::map<uuid_t,player_ptr_t> player_map;
		extern ai_state_map states;
		//extern duk_context* duktape_context;
		extern room_list_t room_list;
		extern player_list_t player_list;
		//extern std::vector<mods::chat::channel> chan;
		extern std::vector<std::string> chan_verbs;
		extern bool f_import_rooms;
		extern player_ptr_t current_player;
		extern std::string bootup_test_suite;
		extern std::unique_ptr<pqxx::connection> pq_con;
		extern std::unordered_map<std::string,std::string> ram_db;
		extern std::set<player_ptr_t> needs_dissolve;
		extern std::map<obj_vnum,std::string> obj_stat_pages;
		extern uint64_t current_tick;
		void init_player(char_data*);
		std::unique_ptr<ai_state>& state_fetch(char_data* ch);
		int mobile_activity(char_data*);
		char_data* read_mobile(const mob_vnum&,const int& type);
		std::shared_ptr<mods::npc> read_mobile_ptr(const mob_vnum&,const int& type);
		void register_player(player_ptr_t);
		void register_authenticated_player(player_ptr_t);
		void unregister_authenticated_player(player_ptr_t);
		//void register_object_db_id(obj_ptr_t);
		void register_object(obj_ptr_t);
		template <typename TShopPtr>
		void register_shop(TShopPtr shop) {
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
		std::string strip_colors(std::string_view buffer);
		std::string replace_all(std::string str, const std::string& from, const std::string& to);
		const char* say_random(const mods::ai_state::event_type_t&);
		bool command_interpreter(player_ptr_t,std::string_view argument);
		int dir_int(char);
		void affect_room_light(int room,int offset);
		namespace rooms {
			void char_from_room(char_data*);
			void char_to_room(const room_rnum&,char_data*);
		};
		void pad_room(int room,char_data* ch,int door);
		player_list_t& get_room_list(room_rnum);
		player_list_t& get_room_list(player_ptr_t&);
		std::string dir_to_str(int dir, bool adjective);
		void queue_object_destruct(uuid_t obj_uuid, uint16_t seconds);
		void destruct_object(uuid_t obj);
		void dispose_object(uuid_t obj);
		void dispose_player(uuid_t pl_uuid);
	};
};
extern std::deque<std::shared_ptr<obj_data>> obj_list;
extern std::deque<std::shared_ptr<mods::npc>> mob_list;

namespace txt {
	using namespace mods::util;
	/**
	 * @brief returns true if lowercase compare returns true for any items that match target
	 *
	 * @tparam TString
	 * @param items
	 * @param target
	 *
	 * @return
	 */
	template <typename TString>
	static inline bool match_any_of_lower(std::vector<TString> items,TString target) {
		for(auto&& needle : items) {
			if(is_lower_match(needle,target)) {
				return true;
			}
		}
		return false;
	}
	/**
	 * @brief returns true if case-sensitive compare returns true for any items that match target
	 *
	 * @tparam TString
	 * @param items
	 * @param target
	 *
	 * @return
	 */
	template <typename TString>
	static inline bool match_any_of(std::vector<TString> items,TString target) {
		for(auto&& needle : items) {
			if(needle.compare(target) == 0) {
				return true;
			}
		}
		return false;
	}

};
player_ptr_t ptr_by_db_id(const uint64_t& player_db_id);
player_ptr_t ptr(const char_data*);
player_ptr_t ptr(char_data*);
obj_ptr_t optr(obj_data*);
player_ptr_t ptr_by_uuid(uuid_t);
std::shared_ptr<mods::npc> npc_by_uuid(const uuid_t& u);
obj_ptr_t optr_by_uuid(uuid_t);
std::optional<obj_ptr_t> optr_opt(uuid_t);
std::optional<player_ptr_t> ptr_opt(uuid_t);
std::optional<player_ptr_t> ptr_opt(char_data*);
inline mods::globals::player_list_t& room_list(room_rnum r) {
	return mods::globals::get_room_list(r);
}
#define PARSE_ARGS() mods::util::arglist<std::vector<std::string>>(std::string(argument))
#define ICMP(a,b) mods::util::is_lower_match(a,b)
namespace mods::skills {
	extern bool player_can(player_ptr_t&,int);
};
static inline bool can(player_ptr_t& player,int skill) {
	return mods::skills::player_can(player,skill);
}

inline std::string const& to_string(std::string const& s) {
	return s;
}
template<typename... Args>
static inline std::string CAT(Args const& ... args) {
	std::string result;
	using ::to_string;
	using std::to_string;
	int unpack[] {0, (result += to_string(args), 0)...};
	static_cast<void>(unpack);
	return result;
}
std::string IMPLODE(std::vector<std::string> m,std::string delim);
std::string IMPLODE(std::vector<std::string> m,std::string delim,bool ignore_empty);
std::string IMPLODE_PAD(std::string delim_one,std::vector<std::string> m,std::string delim_two);
std::string IMPLODE_PAD(std::string delim_one,std::vector<std::string> m,std::string delim_two,bool ignore_empty);
std::string dirstr(int);
std::pair<bool,direction_t> to_direction(std::string_view);
std::vector<str_t> map_keys(str_map_t& m);
#define invec(A,B) (std::find(B.begin(),B.end(),A)!=B.end())
str_vec_t EXPLODE(str_t& value,char delimiter);
str_vec_t EXPLODE(str_t value,char delimiter);
static inline uint64_t CURRENT_TICK() {
	return mods::globals::current_tick;
}
static inline uint64_t current_tick() {
	return mods::globals::current_tick;
}
static inline uint64_t future_tick(uint64_t add) {
	uint64_t current = mods::globals::current_tick;
	return current + add;
}
#define ADMIN_DONE() player->sendln(ADMIN_SUCCESS_MESSAGE())
#define ADMIN_FAIL() player->sendln(ADMIN_FAILURE_MESSAGE())
#define ADMIN_REJECT() if(!mods::super_users::player_is(player)){ player->sendln(SUPER_USER_REJECT_MESSAGE()); return; }
#define TODO(a) std::cerr << green_str("[TODO]: ") << a << " [file]: " << __FILE__ << "[line]:" << __LINE__ << "\n";
#define PLAYER_CAN(A) if(!player->can(A)){ player->sendln("Wrong class."); return; }
#define SHOULD_RATE_LIMIT(SRL_EVENT) if(mods::rate_limiting::should_throttle(mods::rate_limiting::action_t::SRL_EVENT,player)){ player->errorln("You must wait until you can do that again."); return; } else{ mods::rate_limiting::rate_limit_hit(mods::rate_limiting::action_t::SRL_EVENT,player); }
#define SEND_HELP(topic) mods::help::send_help(topic,player);
#define tier(player) std::max(player->level() / 5, 1)
namespace util = mods::util;
std::size_t world_size();
static inline std::vector<direction_t>& directions() {
	static std::vector<direction_t> directions = { NORTH,EAST,SOUTH,WEST,UP,DOWN };
	return directions;
}
#endif
