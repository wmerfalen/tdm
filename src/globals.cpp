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


extern int errno;
#define MODS_BREACH_DISORIENT 50
#define MODS_GRENADE_BASE_DAMAGE 66
struct char_data* character_list = NULL;
extern void do_look(struct char_data *ch, char *argument, int cmd, int subcmd);
extern void char_from_room(struct char_data*);
extern void char_to_room(struct char_data*,room_rnum);
extern void clear_char(struct char_data*);
extern void do_look(struct char_data*,char *argument, int cmd, int subcmd);
extern void parse_sql_rooms();
extern struct player_special_data dummy_mob;
extern int circle_shutdown;
extern int port_main(int,char**);
namespace config {
	extern void init(int,char**);
};
namespace mods {
	namespace globals {
		using player = mods::player;
		std::unordered_map<std::string,std::string> ram_db;
		boot_type_t boot_type;
		socket_map_t socket_map;
		std::unique_ptr<lmdb_db> db;
		player_ptr_t player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		duk_context* duktape_context;
		ai_state_map states;
		std::map<const char*,player_ptr_t> player_name_map;
		//std::vector<std::vector<char_data*>> room_list; /**!TODO turn this into std::shared_ptr<...> */
		room_list_t room_list;
		player_list_t player_list;
		std::vector<mods::chat::channel> chan;
		std::vector<std::string> chan_verbs;
		bool f_import_rooms;
		player_ptr_t current_player;
		std::string bootup_test_suite;
		std::unique_ptr<pqxx::connection> pq_con;
		//builder_data_map_t builder_data;

		/* Maps */
		map_object_list obj_map;
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
		void register_object(obj_data& obj) {

		}

		void shutdown(void){
			circle_shutdown = 1;
		}
		/** !todo: phase this function out */
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd) {
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
					//<< "--lmdb-name=<name> use name as lmdb db name\n"
					//<< "--lmdb-dir=<dir> use dir as directory to store lmdb data\n"
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
				//if(strncmp(argv[pos],"--lmdb-name=",12) == 0){
				//	lmdb_name = argument.substr(12,argument.length()-12);
				//	continue;
				//}
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
				//if(strncmp(argv[pos],"--lmdb-dir=",11) == 0){
				//	if(argument.length() < 12){
				//		log("SYSERR: --lmdb-dir expects an argument, none found: '",argv[pos],"'. Exiting...");
				//		mods::globals::shutdown();
				//	}else{
				//		lmdb_dir = argument.substr(11,argument.length()-11);
				//		continue;
				//	}
				//}
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
		//void room_event(struct char_data* ch,mods::ai_state::event_type_t event) { }
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
		char_data* read_mobile(const mob_vnum & nr,const int & type){
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

			SET_BIT(mob_proto[i].char_specials.saved.act, MOB_ISNPC);
			/** !TODO: fix this. We need to copy the mob_proto to the mob_list.back() */
			mob_list.emplace_back(std::make_shared<mods::npc>(i));
			auto mob = mob_list.back();
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
			mob->uuid() = mob_list.size() - 1;
			return mob->cd();
		}
		uuid_t get_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		uuid_t obj_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		std::unique_ptr<ai_state>& state_fetch(struct char_data* ch) {
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
		std::string color_eval(std::string in_buffer) {
			std::map<std::string,std::string> colors = {
				{"blu","\033[34m"},
				{"gld","\033[33m"},
				{"grn","\033[32m"},
				{"red","\033[31m"},
				{"wht","\033[37m"},
				{"yel","\033[93m"}
			};
			unsigned i = 0;
			std::string final_buffer = "";
			unsigned len = in_buffer.length();
			for(;i < len;i++){
				auto current_char = in_buffer[i];
				if(current_char == '{'){
					if(len > i + 5 && in_buffer[i+5] == '}' && 
							in_buffer[i+1] == '/'){
						i += 5;
						final_buffer += "\033[0m";
						continue;
					}
					if(len > i + 4 && in_buffer[i+4] == '}'){
						auto substring = in_buffer.substr(i+1,3);
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

				bool command_interpreter(player_ptr_t player,const std::string& argument) {
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
						return mods::drone::interpret(*player,argument);
					}

					if(!player->cd()->drone && mods::quests::has_quest(*player)) {
						d("Running trigger for quests");
						mods::quests::run_trigger(*player);
					}

					if(player->paging()) {
						if(argument.length() == 0) {
							player->pager_next_page();
							return false;
						}

						if(argument.compare("q") == 0) {
							player->pager_clear();
							player->pager_end();
							return false;
						}

						auto good = mods::util::stoi(argument);

						if(good.has_value()) {
							player->page(good.value() - 1);
						}

						return false;
					}

					if(player->room_pave_mode()) {
						//If is a direction and that direction is not an exit,
						//then pave a way to that exit
						int door = 0;

						if(argument.length() == 1){
							switch(argument[0]) {
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

							if(world[player->room()].dir_option[door] == nullptr){
								if(player->room() < 0){
									log("SYSERR: error: player's room is less than zero. Not paving.");
									return false;
								}
								if(player->room() >= 0 && std::size_t(player->room()) >= world.size()){
									log("SYSERR: error: player's room is outside of world.size().");
									return false;
								}
								player->stc("[stub] pave_to\n");
								mods::builder::pave_to(player,&world[player->room()],door);
								return false;
							}
						}
					}
					return true;
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
							return NORTH;
					}
				}

				void post_command_interpreter(struct char_data *ch,char* argument) {
					return;
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
						MENTOC_PREAMBLE();
						auto room_id = IN_ROOM(ch);
						if(std::size_t(room_id) >= room_list.size()){
							log("SYSERR: char_from_room failed. room_id >= room_list.size()");
							return;
						}
						auto place = std::find(room_list[room_id].begin(),room_list[room_id].end(),player);
						if(place != room_list[room_id].end()){
							room_list[room_id].erase(place);
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
						MENTOC_PREAMBLE();
						auto target_room = room;
						if(boot_type == boot_type_t::BOOT_HELL){
							std::cerr << "boot type hell. NOT sending to requested room of: " << room << "\n";
							target_room = 0;
						}
						if(target_room >= room_list.size()){
							log("SYSERR: char_to_room failed for ch. Requested room is out of bounds: ",target_room);
							return;
						}
						room_list[target_room].push_back(player);
						IN_ROOM(ch) = target_room;
						return;
					}
				};

			};

		};


#endif
