#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__ 1

#include "globals.hpp"
#include "mods/player.hpp"
#include "mods/deferred.hpp"
#include <map>
#include <memory>
#include <random>
#include <iterator>
#include "mods/lmdb/db.hpp"
#include "mods/ai_state.hpp"
#include "mods/quests.hpp"
#include "mods/builder.hpp"
#include "utils.h"
#include "mods/behaviour_tree_impl.hpp"
#include "mods/util.hpp"
#include "mods/pregame.hpp"
#include "mods/loops.hpp"
#include "mods/testing_index.hpp"
#include "mods/auto-login.hpp"
#include "mods/debug.hpp"
#include "mods/orm/chargen.hpp"
#include "mods/world-configuration.hpp"


extern int errno;
#define MODS_BREACH_DISORIENT 50
#define MODS_GRENADE_BASE_DAMAGE 66
char_data* character_list = NULL;
extern void do_look(char_data *ch, char *argument, int cmd, int subcmd);
extern void char_from_room(char_data*);
extern void char_to_room(char_data*,room_rnum);
extern void clear_char(char_data*);
extern void do_look(char_data*,char *argument, int cmd, int subcmd);
extern void parse_sql_rooms();
extern struct player_special_data dummy_mob;
extern int circle_shutdown;
extern int port_main(int,char**);
extern int next_room_number();
extern int next_room_vnum();
extern void look_at_room(char_data* ch,int ignore_brief);
namespace config {
	extern void init(int,char**);
};
namespace mods {
	namespace globals {
		using player = mods::player;
		std::map<char_data*,player_ptr_t> mob_ptrmap;
		std::unordered_map<std::string,std::string> ram_db;
		boot_type_t boot_type;
		socket_map_t socket_map;
		std::unique_ptr<lmdb_db> db;
		player_ptr_t player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		duk_context* duktape_context;
		ai_state_map states;
		std::map<std::string,player_ptr_t> player_name_map;
		//std::vector<std::vector<char_data*>> room_list; /**!TODO turn this into std::shared_ptr<...> */
		room_list_t room_list;
		player_list_t player_list;
		std::set<player_ptr_t> dissolver_queue;
		std::vector<mods::chat::channel> chan;
		std::vector<std::string> chan_verbs;
		bool f_import_rooms;
		player_ptr_t current_player;
		std::string bootup_test_suite;
		std::unique_ptr<pqxx::connection> pq_con;
		//builder_data_map_t builder_data;

		/* Maps */
		map_object_list obj_map;
		std::map<uuid_t,std::shared_ptr<mods::npc>> mob_map;
		std::map<uuid_t,player_ptr_t> player_map;
		std::map<uint64_t,uuid_t> db_id_to_uuid_map;

		std::map<obj_data*,obj_ptr_t> obj_odmap;
		std::map<char_data*,std::shared_ptr<mods::npc>> mob_chmap;
		std::map<char_data*,std::shared_ptr<mods::player>> player_chmap;
		std::map<obj_vnum,std::string> obj_stat_pages;

		template <typename I>
			I random_element(I begin, I end) {
				const unsigned long n = std::distance(begin, end);
				const unsigned long divisor = (RAND_MAX + 1) / n;

				unsigned long k;

				do {
					std::srand(std::time(0));
					k = std::rand() / divisor;
				} while(k >= n);

				std::advance(begin, k);
				return begin;
			}

		template<typename Iter, typename RandomGenerator>
			Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
				std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
				std::advance(start, dis(g));
				return start;
			}

		template<typename Iter>
			Iter select_randomly(Iter start, Iter end) {
				static std::random_device rd;
				static std::mt19937 gen(rd());
				return select_randomly(start, end, gen);
			}

		/** This function sucks */
		std::string replace_all(std::string str, const std::string& from, const std::string& to) {
			size_t start_pos = 0;

			while((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}

			return str;
		}

		void register_object_list() {
		}
		void register_object(obj_ptr_t obj) {
			obj->uuid = obj_uuid();
			std::cerr << "register_object: " << obj->uuid << "\n";
			obj_map[obj->uuid] = obj;
			obj_odmap[obj.get()] = obj;
			register_object_db_id(obj->db_id(),obj->uuid);
		}
		void register_object_db_id(uint64_t db_id,uuid_t uuid){
			db_id_to_uuid_map[db_id] = uuid;
		}
		void register_player(player_ptr_t player){
			player_chmap[player->cd()] = player;
			player_map[player->uuid()] = player;
		}
		void register_authenticated_player(player_ptr_t player){
			player_name_map[player->name()] = player;
		}

		void shutdown(void){
			circle_shutdown = 1;
		}
		/** !todo: phase this function out */
		bool acl_allowed(char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd) {
			return false;
		}
		int mobile_activity(char_data* ch) {
			return 1;
		}
		void init(int argc,char** argv) {
			int pos = 0;
			std::string argument,
				lmdb_dir = LMDB_DB_DIRECTORY,
				lmdb_name = LMDB_DB_NAME,
				f_test_suite;
			f_import_rooms = false;
			boot_type = BOOT_DB;
			config::run_profile_scripts = false;
			bool show_tics = false;
			std::string postgres_user = mods::conf::postgres_user.data();
			std::string postgres_dbname = mods::conf::postgres_dbname.data();
			std::string postgres_password = mods::conf::postgres_password.data();
			std::string postgres_host = mods::conf::postgres_host.data();
			std::string postgres_port = mods::conf::postgres_port.data();
			mods::world_conf::toggle::set_obj_from_room(1);

			while(++pos < argc){
				if(argv[pos]){
					argument = argv[pos];
				}else{
					argument = "";
				}
				if(strncmp(argv[pos],"--help",6) == 0 || strncmp(argv[pos],"-h",2) == 0){
					std::cerr << "usage: <circle> --postgres-pw-file=<file> [options]\n" 
						<< "--auto-login=user Automatically login as user on connection [use only for development]\n" 
						<< "--auto-password=password Automatically login and use password on connection [use only for development]\n" 
						<< "--testing=<suite>	Launch test suite\n"
						<< "--import-rooms 	Run the import rooms routine\n"
						<< "--hell 	Start the mud in HELL mode\n"
						<< "--lmdb-name=<name> use name as lmdb db name\n"
						<< "--lmdb-dir=<dir> use dir as directory to store lmdb data\n"
						<< "--postgres-dbname=<db> use db as postgres db. default: mud\n"
						<< "--postgres-user=<user> use user as postgres user. default: postgres\n"
						<< "--postgres-host=<host> use host as postgres host. default: localhost\n"
						<< "--postgres-port=<port> use port as postgres port. default: 5432\n"
						<< "--postgres-pw-file=<file> read postgres password from file. no default. required.\n"
						<< "--run-profile-scripts=<0|1> set to 1 to run profile scripts. default: 0\n"
						<< "--show-tics show a dot for every game tic\n"
						<< "--seed=<what> seed the database with one of the following:\n"
						<< "     'player_classes': character generation\n"
						<< "     '': ''\n"
						;
					mods::globals::shutdown();
					exit(0);
				}

				if(strncmp(argv[pos],"--show-tics",11) == 0){
					show_tics = true;
					continue;
				}

				if(strncmp(argv[pos],"--auto-login=",13) == 0){
					mods::auto_login::set_user(argument.substr(13,argument.length()-13));
					continue;
				}
				if(strncmp(argv[pos],"--run-profile-scripts=",22) == 0){
					if(argument.length() != 23){
						std::cerr << "Please specify 1 or 0(zero) for --run-profile-scripts\n";
						exit(-1);
					}
					if(argument[22] == '1'){
						std::cerr << "[cli conf]: enabling profile scripts\n";
						config::run_profile_scripts = true;
					}
					continue;
				}
				if(strncmp(argv[pos],"--auto-password=",16) == 0){
					mods::auto_login::set_password(argument.substr(16,argument.length()-16));
					continue;
				}
				if(strncmp(argv[pos],"--testing=",10) == 0){
					f_test_suite = argument.substr(10,argument.length()-10);
					continue;
				}
				if(strncmp(argv[pos],"--import-rooms",14) == 0){
					f_import_rooms = true;
					continue;
				}
				if(strncmp(argv[pos],"--hell",6) == 0){
					boot_type = BOOT_HELL;
					continue;
				}
				if(strncmp(argv[pos],"--lmdb-name=",12) == 0){
					lmdb_name = argument.substr(12,argument.length()-12);
					log((std::string("DEBUG: found lmdb db name: ") + lmdb_name).c_str());
					continue;
				}
				if(strncmp(argv[pos],"--postgres-pw-file=",19) == 0){
					if(argument.length()  < 20){
						log("SYSERR: --postgres-pw-file expects an argument, none found: '",argument.c_str(),"'.Exiting...");
						mods::globals::shutdown();
					}
					std::string pw_file = argument.substr(19,argument.length()-19);
					std::ifstream in_file(pw_file.c_str(),std::ios::in | std::ios::binary);
					if(!in_file.good() || !in_file.is_open()){
						log("SYSERR: unable to open password file. Exiting...");
						mods::globals::shutdown();
					}else{
						struct stat statbuf;
						if(stat(pw_file.c_str(), &statbuf) == -1) {
							log("SYSERR:  cannot stat password file. Exiting...");
							mods::globals::shutdown();
						}
						std::vector<char> buffer;
						buffer.resize(statbuf.st_size + 1);
						std::fill(buffer.begin(),buffer.end(),0);
						in_file.read((char*)&buffer[0],statbuf.st_size);
						in_file.close();
						postgres_password.assign(buffer.begin(),buffer.end());
						std::size_t i = postgres_password.length() -1;
						if(i == 1){
							log("SYSERR: the postgres password is one character long. Please check the postgres password file");
							mods::globals::shutdown();
							return;
						}
						while(isspace(postgres_password[i--]) && i > 0){}
						if(i == 0){
							log("SYSERR: while trying to trim the end of the postgres password of spaces (using isspace()), the resulting postgres password is zero length. Exiting...");
							mods::globals::shutdown();
							return;
						}
						postgres_password = postgres_password.substr(0,i);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-host=",16) == 0){
					if(argument.length()  < 16){
						log("SYSERR: --postgres-host expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					}else{
						postgres_host = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-user=",16) == 0){
					if(argument.length()  < 16){
						log("SYSERR: --postgres-user expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					}else{
						postgres_user = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-dbname=",18) == 0){
					if(argument.length() < 18){
						log("SYSERR: --postgres-port expects an argument, none found: '" ,argv[pos],"'.Exiting...");
						mods::globals::shutdown();
					}else{
						postgres_dbname = argument.substr(18,argument.length()-14);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-port=",16) == 0){
					if(argument.length() < 16){
						log("SYSERR: --postgres-port expects an argument, none found: '" ,argv[pos],"'.Exiting...");
						mods::globals::shutdown();
					}else{
						postgres_port = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--lmdb-dir=",11) == 0){
					if(argument.length() < 12){
						log("SYSERR: --lmdb-dir expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					}else{
						lmdb_dir = argument.substr(11,argument.length()-11);
						log((std::string("DEBUG: found lmdb directory: ")+ lmdb_dir).c_str());
						continue;
					}
				}
			}

			if(!mods::util::dir_exists(lmdb_dir.c_str())){
				auto err = mkdir(lmdb_dir.c_str(),0700);
				if(err == -1){
					log("SYSERR: The lmdb database directory couldn't be created: ", mods::util::err::get_string(errno).c_str());
					mods::globals::shutdown();
				}
			}
			db = std::make_unique<lmdb_db>(lmdb_dir,lmdb_name,MDB_WRITEMAP,0600,true);
			player_nobody = nullptr;
			defer_queue = std::make_unique<mods::deferred>(mods::deferred::TICK_RESOLUTION);
			duktape_context = mods::js::new_context();
			mods::js::load_c_functions();
			/** TODO: make configurable */
			mods::js::load_library(mods::globals::duktape_context,"/lib/quests/quests.js"); 
			mods::behaviour_tree_impl::load_trees();
			if(f_test_suite.length()){
				//if(!f_test_suite.compare("db")){
				//	mods::testing::lmdb::db test(argc,argv);
				//}
				//if(!f_test_suite.compare("player_syncing")){
				//	mods::testing::lmdb::player_syncing test(argc,argv);
				//}
				mods::globals::shutdown();
			}
			bool connected_to_postgres = false;
			try{
				std::string connection_string = mods::conf::pq_connection(
						{{"port",postgres_port},
						{"user",postgres_user},
						{"password",postgres_password},
						{"host",postgres_host},
						{"dbname",postgres_dbname}}
						).c_str();
				pq_con = std::make_unique<pqxx::connection>(connection_string.c_str());
				connected_to_postgres = true;
			}catch(const std::exception &e){
				mods::globals::shutdown();
			}
			if(connected_to_postgres){
				log("[postgres] connected :)");
			}else{
				log("SYSERR: Couldn't connect to postgres");
				mods::globals::shutdown();
				return;
			}
			mods::debug::init(show_tics);
		}
		void post_boot_db() {
		}
		const char* say_random(const mods::ai_state::event_type_t& event) {
			return "woof";
		}
		void room_event(room_vnum room,mods::ai_state::event_type_t event) {
			mods::loops::foreach_in_room(room,[&](player_ptr_t player_ptr) -> bool {
					auto ptr = player_ptr->cd();
					std::string text;
					switch(event){
					case mods::ai_state::BREACHED_NORTH:
					text = "The {red}north{/red} door was breached.";
					break;
					case mods::ai_state::BREACHED_SOUTH: 
					text = "The {red}south{/red} door was breached.";
					break;
					case mods::ai_state::BREACHED_EAST: 
					text = "The {red}east{/red} door was breached.";
					break;
					case mods::ai_state::BREACHED_WEST:
					text = "The {red}west{/red} door was breached.";
					break;
					case mods::ai_state::GRENADE_FLIES_BY:
					text = "A {grn}grenade{/grn} flies by!";
					break;
					case mods::ai_state::GRENADE_EXPLOSION:
					text = "A {grn}grenade{/grn} explodes!";
					break;
					default: break;
					}
					if(text.length()){
						text += "\r\n";
						send_to_char(ptr,text.c_str());
					}
					return true;
			});
		}
		void refresh_player_states() {
			mods::loops::foreach_all([&](player_ptr_t player_ptr) -> bool {
					auto ptr = player_ptr->cd();
					if(!ptr){
					return true;
					}
					if(states.find(ptr) == states.end()) {
					states[ptr] = std::make_unique<mods::ai_state>(ptr,0,0);
					}
					return true;
					});
		}
		void pre_game_loop() {
			std::cout << "[event] Pre game loop\n";
			refresh_player_states();
			std::cout << "[event] refreshed player states\n";
			if(bootup_test_suite.length() > 0){
				std::cout << "booting suite: " << bootup_test_suite << "\n";
				mods::pregame::boot_suite(bootup_test_suite);
			}
		}
		uuid_t player_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		uuid_t mob_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		char_data* read_mobile(const mob_vnum & nr,const int & type){
			return read_mobile_ptr(nr,type)->cd();
		}
		std::shared_ptr<mods::npc> read_mobile_ptr(const mob_vnum & nr,const int & type){
			log("DEBUG: mods::globals::read_mobile. vnum: %d",nr);
			mob_rnum i;

			if(type == VIRTUAL) {
				log("DEBUG: mods::globals::read_mobile %d VIRTUAL",nr);
				if((i = real_mobile(nr)) == NOBODY) {
					log("WARNING: Mobile vnum ",nr," does not exist in database.");
					return nullptr;
				}
			} else {
				log("DEBUG: mods::globals::read_mobile %d REAL",nr);
				i = nr;
			}
			if(std::size_t(i) >= mob_proto.size()){
				log("SYSERR: requested mob_proto index is invalid: ", i ,". mob_proto.size() is currently: ", mob_proto.size(), ". Ignoring...");
				return nullptr;
			}

			auto & mob = mob_list.emplace_back(std::make_shared<mods::npc>(i));
			mob->position() = POS_STANDING;
			for(i = 0; i < NUM_WEARS; i++) {
				GET_EQ(mob->cd(), i) = nullptr;
			}
			mob->cd()->carrying = nullptr;
			mob->max_hp() = rand_number(mob->hp(), mob->mana());
			mob->hp() = mob->max_hp();
			mob->mana() = mob->max_mana();
			mob->move() = mob->max_move();

			mob->set_time_birth(time(0));
			mob->set_time_played(0);
			mob->set_time_logon(time(0));

			if(mob_index.size() > i){
				mob_index[i].number++;
			}
			SET_BIT(mob->char_specials().saved.act, MOB_ISNPC);
			GET_POS(mob->cd()) = POS_STANDING;
			mob->uuid() = player_uuid();
			mob_map[mob->uuid()] = mob;
			mob_chmap[mob->cd()] = mob;
			SET_BIT(mob->cd()->char_specials.saved.act, MOB_ISNPC);
			mob_ptrmap[mob->cd()] = mob->player_ptr();
			player_map[mob->uuid()] = mob->player_ptr();
			return mob;
		}
		uuid_t obj_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		std::unique_ptr<ai_state>& state_fetch(char_data* ch) {
			if(states.find(ch) == states.end()) {
				return states[ch] = std::make_unique<ai_state>(ch,0,0);
			}

			return states[ch];
		}
		int opposite_dir(int dir) {
			if(dir == NORTH) {
				return SOUTH;
			}

			if(dir == SOUTH) {
				return NORTH;
			}

			if(dir == EAST) {
				return WEST;
			}

			if(dir == WEST) {
				return EAST;
			}

			if(dir == UP) {
				return DOWN;
			}

			if(dir == DOWN) {
				return UP;
			}

			return NORTH;
		}
		std::string color_eval(std::string_view buffer) {
			std::map<std::string_view,std::string_view> colors = {
				{"blu","\033[34m"},
				{"gld","\033[33m"},
				{"grn","\033[32m"},
				{"red","\033[31m"},
				{"wht","\033[37m"},
				{"yel","\033[93m"}
			};
			std::string final_buffer = "";
			const std::size_t len = buffer.length();
			for(std::size_t i=0;i < len;i++){
				auto current_char = buffer[i];
				if(current_char == '{'){
					if(len > i + 5 && buffer[i+5] == '}' && 
							buffer[i+1] == '/'){
						i += 5;
						final_buffer += "\033[0m";
						continue;
					}
					if(len > i + 4 && buffer[i+4] == '}'){
						const std::string_view substring = buffer.substr(i+1,3);
						if(colors[substring].length()){
							final_buffer += colors[substring];
							i += 4;
							continue;
						}
					}
				}
				final_buffer += current_char;
			}
			return final_buffer;
		}

		int file_to_lmdb(const std::string& file, const std::string& key) {
			std::ifstream include_file(file,std::ios::in);

			if(!include_file.is_open()) {
				return -1;
			} else {
				std::vector<char> buffer;
				struct stat statbuf;

				if(stat(file.c_str(), &statbuf) == -1) {
					return -2;
				}

				buffer.reserve(statbuf.st_size + 1);
				std::fill(buffer.begin(),buffer.end(),0);
				include_file.read((char*)&buffer[0],statbuf.st_size);
				DBSET(key,static_cast<char*>(&buffer[0]));
				return statbuf.st_size;
			}
		}
		const std::vector<std::string> super_users = {
			"far"
		};

		bool command_interpreter(player_ptr_t player,std::string_view in_argument) {
#ifdef __MENTOC_WRITE_HISTFILE__
			if(player->authenticated()){ player->write_histfile(in_argument); }
#endif
			std::string argument = in_argument.data();
			if(std::find(super_users.begin(),super_users.end(),player->name().c_str()) != super_users.end()){
				if(argument.substr(0,4).compare("=pos") == 0){
					if(argument.length() < 6){
						player->stc("usage: =pos=<int>\r\n");
					}else{
						std::string pos = argument.substr(5);
						auto optional_result = mods::util::stoi_optional<int>(pos);
						if(optional_result.has_value() == false){
							player->stc("usage: =pos=<int>\r\n");
						}else{
							player->position() = optional_result.value();
							player->done();
						}
					}
					return false;
				}
				if(argument.substr(0,4).compare("+imp") == 0){
					mods::acl_list::set_access_rights(player,"implementors",true);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-imp") == 0){
					mods::acl_list::set_access_rights(player,"implementors",false);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("+god") == 0){
					mods::acl_list::set_access_rights(player,"gods",true);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-god") == 0){
					mods::acl_list::set_access_rights(player,"gods",false);
					player->done();
					return false;
				}
				if(argument.substr(0,6).compare("+build") == 0){
					mods::acl_list::set_access_rights(player,"builders",true);
					player->done();
					return false;
				}
				if(argument.substr(0,6).compare("-build") == 0){
					mods::acl_list::set_access_rights(player,"builders",false);
					player->done();
					return false;
				}
				if(argument.substr(0,7).compare("-affplr") ==0){
					player->clear_all_affected();
					player->clear_all_affected_plr();
					player->done();
					return false;
				}

				if(argument.substr(0,4).compare("-aff") == 0){
					player->clear_all_affected();
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-plr") == 0){
					player->clear_all_affected_plr();
					player->done();
					return false;
				}
			}



			if(mods::drone::started(*player)) {
				d("drone started. interpretting");
				return mods::drone::interpret(*player,in_argument.data());
			}

			if(!player->cd()->drone && mods::quests::has_quest(*player)) {
				d("Running trigger for quests");
				mods::quests::run_trigger(*player);
			}

			if(player->paging()) {
				if(in_argument.length() == 0) {
					player->pager_next_page();
					return false;
				}

				if(in_argument.compare("q") == 0) {
					player->pager_clear();
					player->pager_end();
					return false;
				}

				auto good = mods::util::stoi(in_argument.data());

				if(good.has_value()) {
					player->page(good.value() - 1);
				}

				return false;
			}

			if(player->room_pave_mode()) {
				player->sendln("You are paving");
				//If is a direction and that direction is not an exit,
				//then pave a way to that exit
				int door = 0;

				if(in_argument.length() == 1){
					switch(in_argument[0]) {
						case 'u':
						case 'U':
							door = UP;
							break;

						case 's':
						case 'S':
							door = SOUTH;
							break;

						case 'w':
						case 'W':
							door = WEST;
							break;

						case 'e':
						case 'E':
							door = EAST;
							break;

						case 'n':
						case 'N':
							door = NORTH;
							break;

						case 'd':
						case 'D':
							door = DOWN;
							break;
						default: return true;
					}

					auto cached_room = player->room();
					int new_room_id = 0;
					if(world[cached_room].dir_option[door] == nullptr){
						player->sendln("Creating room in that direction");
						new_room_id = next_room_number();
						do{
							world.emplace_back();
							world.back().init();
						}while(new_room_id >= world.size());
						register_room(0);
						world[new_room_id].number = next_room_vnum();
						mods::builder::pave_to(player,&world[new_room_id],door,new_room_id);
						player->set_room(new_room_id);
						mods::globals::rooms::char_to_room(player->room(),player->cd());
						look_at_room(player->cd(),0);
						return false;
					}
				}
			}
			return true;
		}

		void pad_room(int room,char_data* ch,int door){
			/** UNUSED TODO CLEAN UP*/
		}
		int dir_int(char dir) {
			switch(dir) {
				case 'n':
				case 'N':
					return NORTH;

				case 'e':
				case 'E':
					return EAST;

				case 's':
				case 'S':
					return SOUTH;

				case 'w':
				case 'W':
					return WEST;

				case 'u':
				case 'U':
					return UP;

				case 'd':
				case 'D':
					return DOWN;

				default:
					return -1;
			}
		}

		void register_room(const room_rnum& r) {
			top_of_world = world.size();
			for(; room_list.size() < world.size();){
				room_list.push_back({});
			}
		}
		void init_player(char_data* ch) {
			//TODO: if this is used, correct it
		}


		namespace rooms {
			/*! \brief takes a character from a room. Moving a character from room to room is a
			 * two step process. This function is the former of said process, the latter being
			 * char_to_room(room_rnum&,char_data*);
			 * \param char_data* the character's player pointer
			 * \return void
			 */
			void char_from_room(char_data* ch) {
				auto player = ptr(ch);
				int room_id = player->room();
				if(room_id >= room_list.size()){
					log("SYSERR: char_from_room failed. room_id >= room_list.size()");
					return;
				}
				auto place = std::find(
					mods::globals::room_list[room_id].begin(),
					mods::globals::room_list[room_id].end(),
					player
				);
				if(place != mods::globals::room_list[room_id].end()){
					mods::globals::room_list[room_id].erase(place);
				} else {
					log("SYSERR: Could NOT find char in room_list");
				}
			}

			/*! \brief moves a character to the room identified by the first parameter. If the mud 
			 * is booted in HELL MODE, the character is merely moved to the only room available
			 * in the mud (zero), otherwise the character is moved to the specified room (if it 
			 * can be found). Moving from one room to another is a two function process. This function
			 * is the latter of that process, the former being char_from_room. 
			 * \param room room id. of type room_rnum
			 * \param char_data* character pointer
			 * \return void will log a SYSERR if the resolved room id (param 1) is out of bounds
			 */
			void char_to_room(const room_rnum& room,char_data* ch) {
				std::cerr << "char_to_room: " << room << " ch:" << ch->player.name.c_str() << "\n";
				auto player = ptr(ch);
				auto target_room = room;
				if(boot_type == boot_type_t::BOOT_HELL){
					target_room = 0;
				}
				if(target_room >= room_list.size()){
					log("SYSERR: char_to_room failed for ch. Requested room is out of bounds: ",target_room);
					return;
				}
				player->set_room(target_room);
				mods::globals::room_list[target_room].push_back(player);
				return;
			}
		};//end namespace rooms

		void affect_room_light(int room,int offset){
			if(room < world.size() && room >= 0){
				world[room].light -= offset;
			}
		}
		static mods::globals::player_list_t blank_room;
		player_list_t& get_room_list(room_rnum room){
			if(room == NOWHERE || room >= mods::globals::room_list.size()){
				return blank_room;
			}
			return mods::globals::room_list[room];
		}
		player_list_t& get_room_list(player_ptr_t& player){
			return mods::globals::get_room_list(player->room());
		}
		std::string dir_to_str(int dir, bool adjective){
			if(adjective){
				switch(dir){
					case NORTH: return "northern";
					case SOUTH: return "southern";
					case EAST: return "eastern";
					case WEST: return "western";
					case UP: return "upper";
					case DOWN: return "below";
					default: return "?";
				}
			}
			switch(dir){
				case NORTH: return "north";
				case SOUTH: return "south";
				case EAST: return "east";
				case WEST: return "west";
				case UP: return "up";
				case DOWN: return "down";
				default: return "?";
			}
		}
		void queue_object_destruct(uuid_t obj_uuid, uint16_t ticks){
			mods::globals::defer_queue->push_ticks_event(ticks, {obj_uuid, mods::deferred::EVENT_OBJECT_DESTRUCT});
		}
		void destruct_object(uuid_t uuid){
			auto obj = optr_by_uuid(uuid);
			if(!obj){
				return;
			}
			auto omap = obj_map.find(obj->uuid);
			if(omap != obj_map.end()){
				obj_map.erase(omap);
			}
			auto odmap = obj_odmap.find(obj.get());
			if(odmap != obj_odmap.end()){
				obj_odmap.erase(odmap);
			}
			d("use count for destructed object: " << obj.use_count());
		}
		void dispose_object(uuid_t obj_uuid){
			auto obj = optr_by_uuid(obj_uuid);
			obj_odmap.erase(obj.get());
			obj_map.erase(obj_uuid);
			auto it = std::find(obj_list.begin(),obj_list.end(),obj);
			if(it != obj_list.end()){
				obj_list.erase(it);
			}
		}
		void dispose_player(uuid_t pl_uuid){
#ifdef __MENTOC_SHOW_DISPOSE_PLAYER_DEBUG_OUTPUT__
#define mgdp_debug(a) mentoc_prefix_debug("mods::globals::dispose_player") << a << "\n";
#else
#define mgdp_debug(a) /**/
#endif
			auto player = ptr_by_uuid(pl_uuid);
			if(!player){
				mgdp_debug("ptr_by_uuid returned an invalid player ptr... not removing from any structures!");
				return;
			}
			{
				auto p_room_list = mods::globals::get_room_list(player->room());
				auto it = std::find(p_room_list.begin(),p_room_list.end(),player);
				if(it != p_room_list.end()){
					mgdp_debug("Erasing from player p_room_list");
					p_room_list.erase(it);
				}
			}
			/** TODO remove from room_list */
			if(player->is_npc()){
				mgdp_debug("Erasing from mob_chmap");
				mods::globals::mob_chmap.erase(const_cast<char_data*>(player->cd()));
				mob_map.erase(player->uuid());
				mob_ptrmap.erase(player->cd());
				player_map.erase(player->uuid());
				{
					auto ch = player->cd();
					std::vector<std::deque<std::shared_ptr<mods::npc>>::iterator> mob_list_erasures;
					for(auto m_it = mob_list.begin();m_it != mob_list.end(); ++m_it){
						if((*m_it)->cd() == ch){
							mgdp_debug("Found one in mob_list... emplacing...");
							mob_list_erasures.emplace_back(m_it);
						}
					}
					for(auto && erase_me : mob_list_erasures){
						mgdp_debug("Erasing..." << "|use_count: " << (erase_me)->use_count());
						mob_list.erase(erase_me);
					}
				}
				player = nullptr;
				return;
			}
			mgdp_debug("Erasing from player_chmap...");
			mods::globals::player_chmap.erase(const_cast<char_data*>(player->cd()));
			{
				auto it = player_map.find(pl_uuid);
				if(it != player_map.end()){
					mgdp_debug("Found in player_map.. erasing...");
					player_map.erase(it);
				}
			}
			{
				mgdp_debug("Looking in player_list...");
				auto it = std::find(player_list.begin(), player_list.end(), player);
				if(it != player_list.end()){
					mgdp_debug("Found in player_list... erasing...");
					player_list.erase(it);
				}
			}
		}
	};//end globals
};
player_ptr_t ptr(char_data* in_ch){
	return ptr((const char_data*)in_ch);
}
player_ptr_t ptr(const char_data* in_ch){
	if(!in_ch){
		return nullptr;
	}
	if(IS_NPC(in_ch)){
		return mods::globals::mob_chmap[const_cast<char_data*>(in_ch)]->player_ptr();
	}
	return mods::globals::player_chmap[const_cast<char_data*>(in_ch)];
}
obj_ptr_t optr(obj_data* in_obj){
	return mods::globals::obj_odmap[in_obj];
}

std::optional<obj_ptr_t> optr_opt(uuid_t obj_uuid){
	auto it = mods::globals::obj_map.find(obj_uuid);
	if(it != mods::globals::obj_map.end()){
		return it->second;
	}
	log("SYSERR: DID NOT find obj_uuid (optr) %d ",obj_uuid);
	return std::nullopt;
}
std::optional<player_ptr_t> ptr_opt(char_data* ch){
	if(IS_NPC(ch)){
		auto it = mods::globals::mob_chmap.find(ch);
		if(it != mods::globals::mob_chmap.end()){
			return it->second;
		}
	}else{
		auto it = mods::globals::player_chmap.find(ch);
		if(it != mods::globals::player_chmap.end()){
			return it->second;
		}
	}
	return std::nullopt;
}
std::optional<player_ptr_t> ptr_opt(uuid_t plr_uuid){
	auto it = mods::globals::player_map.find(plr_uuid);
	if(it != mods::globals::player_map.end()){
		return it->second;
	}
	return std::nullopt;
}
player_ptr_t ptr_by_uuid(uuid_t id){
	return mods::globals::player_map[id];
}
obj_ptr_t optr_by_uuid(uuid_t id){
	return mods::globals::obj_map[id];
}

obj_ptr_t ptr_by_dbid(uint64_t db_id){
	auto it = mods::globals::db_id_to_uuid_map.find(db_id);
	if(it != mods::globals::db_id_to_uuid_map.end()){
		return optr_by_uuid(it->second);
	}
	log("SYSERR: DID NOT find obj by db_id (optr) %d",db_id);
	return nullptr;

}
#endif
