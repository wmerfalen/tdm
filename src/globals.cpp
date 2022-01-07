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
#include "mods/contracts.hpp"
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
#include "mods/debug-pre-game.hpp"
#include "mods/prefs.hpp"
#include "mods/replenish.hpp"
#include "config.hpp"
#include "mods/movement.hpp"
#include "mods/interpreter.hpp"

namespace mods::mob_equipment {
	extern void decorate(const uuid_t& mob_uuid);
};

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
extern void obj_from_room(obj_ptr_t in_object);
namespace mods::chat {
	extern void setup_public_channels();
	extern bool handle_chat(player_ptr_t& player,std::string_view argument);
};

#include "mods/init.hpp"

namespace mods::unit_tests {
	extern int run();
};
namespace mods::mobs::room_watching::events {
	extern void room_entry(player_ptr_t&);
};
namespace config {
	extern void init(int,char**);
};
namespace mods::skills {
	extern void game_init();
};
namespace mods::super_users {
	extern bool player_is(player_ptr_t&);
	extern void upkeep(player_ptr_t&);
};
namespace mods {
	namespace globals {
		using player = mods::player;
		uint64_t current_tick;
		std::unordered_map<std::string,std::string> ram_db;
		boot_type_t boot_type;
		socket_map_t socket_map;
		std::unique_ptr<lmdb_db> db;
		player_ptr_t player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		//duk_context* duktape_context;
		ai_state_map states;
		std::map<std::string,player_ptr_t> player_name_map;
		//std::vector<std::vector<char_data*>> room_list; /**!TODO turn this into std::shared_ptr<...> */
		room_list_t room_list;
		player_list_t player_list;
		std::set<player_ptr_t> dissolver_queue;
		//std::vector<mods::chat::channel> chan;
		//std::vector<std::string> chan_verbs;
		bool f_import_rooms;
		player_ptr_t current_player;
		std::string bootup_test_suite;
		std::unique_ptr<pqxx::connection> pq_con;
		//builder_data_map_t builder_data;

		/* Maps */
		map_object_list obj_map;
		std::map<uuid_t,player_ptr_t> player_map;
		std::map<uint64_t,uuid_t> db_id_to_uuid_map;

		std::map<obj_data*,obj_ptr_t> obj_odmap;
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
			obj_map[obj->uuid] = obj;
			obj_odmap[obj.get()] = obj;
			register_object_db_id(obj->db_id(),obj->uuid);
		}
		void register_object_db_id(uint64_t db_id,uuid_t uuid) {
			db_id_to_uuid_map[db_id] = uuid;
		}
		void register_player(player_ptr_t player) {
			player_chmap[player->cd()] = player;
			player_map[player->uuid()] = player;
		}
		void register_authenticated_player(player_ptr_t player) {
			log(CAT("registering authenticated user...:'",player->name(),"'").c_str());
			player_name_map[player->name()] = player;
			mods::globals::db_id_to_uuid_map[player->db_id()] = player->uuid();
		}
		void unregister_authenticated_player(player_ptr_t player) {
			player_name_map.erase(player->name());
		}

		void shutdown(void) {
			circle_shutdown = 1;
		}
		/** !todo: phase this function out */
		bool acl_allowed(char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd) {
			return false;
		}
		int mobile_activity(char_data* ch) {
			return 1;
		}
		bool is_argument(std::string_view argv,std::string_view test) {
			std::string s = argv.data();
			if(s.length() < test.length()) {
				return false;
			}
			s.substr(0,test.length());
			return strncmp(s.data(),test.data(),test.length()) == 0;
		}
		std::string slice_option_off(std::string_view argv,std::string_view option_including_equals) {
			std::string sliced = argv.data();
			sliced = sliced.substr(option_including_equals.length());
			return sliced;
		}
		int init(int argc,char** argv) {
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
			std::vector<std::tuple<std::string,std::string>> migrations;
			bool run_unit_tests = false;

			while(++pos < argc) {
				if(argv[pos]) {
					argument = argv[pos];
				} else {
					argument = "";
				}
				if(strncmp(argv[pos],"--help",6) == 0 || strncmp(argv[pos],"-h",2) == 0) {
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
					          << "--run-migration-up=<identifier> run the specified 'up' migration\n"
					          << "--run-migration-down=<identifier> run the specified 'down' migration\n"
					          << "--unit-tests Run unit tests and exit\n"
					          << "     'player_classes': character generation\n"
					          << "     '': ''\n"
					          ;
					mods::globals::shutdown();
					exit(0);
				}
				if(is_argument(argument,"--run-migration-up=")) {
					std::string migration_id = slice_option_off(argument,"--run-migration-up=");
					if(migration_id.length() == 0) {
						std::cerr << "[ERROR]: what is this (UP) migration? it's of zero length: '" << argument << "'\nExiting...\n";
						exit(1);
					}
					migrations.push_back(std::make_tuple<>("up",migration_id));
					continue;
				}
				if(is_argument(argument,"--run-migration-down=")) {
					std::string migration_id = slice_option_off(argument,"--run-migration-down=");
					std::cout << "migration_id:'" << migration_id << "'\n";
					if(migration_id.length() == 0) {
						std::cerr << "[ERROR]: what is this (DOWN) migration? it's of zero length: '" << argument << "'\nExiting...\n";
						exit(1);
					}
					migrations.push_back(std::make_tuple<>("down",migration_id));
					continue;
				}

				if(strncmp(argv[pos],"--show-tics",11) == 0) {
					show_tics = true;
					continue;
				}

				if(strncmp(argv[pos],"--auto-login=",13) == 0) {
					mods::auto_login::set_user(argument.substr(13,argument.length()-13));
					continue;
				}
				if(strncmp(argv[pos],"--run-profile-scripts=",22) == 0) {
					if(argument.length() != 23) {
						std::cerr << "Please specify 1 or 0(zero) for --run-profile-scripts\n";
						exit(-1);
					}
					if(argument[22] == '1') {
						std::cerr << "[cli conf]: enabling profile scripts\n";
						config::run_profile_scripts = true;
					}
					continue;
				}
				if(strncmp(argv[pos],"--auto-password=",16) == 0) {
					mods::auto_login::set_password(argument.substr(16,argument.length()-16));
					continue;
				}
				if(strncmp(argv[pos],"--testing=",10) == 0) {
					f_test_suite = argument.substr(10,argument.length()-10);
					continue;
				}
				if(strncmp(argv[pos],"--unit-tests",12) == 0) {
					run_unit_tests = true;
					continue;
				}
				if(strncmp(argv[pos],"--import-rooms",14) == 0) {
					f_import_rooms = true;
					continue;
				}
				if(strncmp(argv[pos],"--hell",6) == 0) {
					boot_type = BOOT_HELL;
					continue;
				}
				if(strncmp(argv[pos],"--lmdb-name=",12) == 0) {
					lmdb_name = argument.substr(12,argument.length()-12);
					log((std::string("DEBUG: found lmdb db name: ") + lmdb_name).c_str());
					continue;
				}
				if(strncmp(argv[pos],"--postgres-pw-file=",19) == 0) {
					if(argument.length()  < 20) {
						log("SYSERR: --postgres-pw-file expects an argument, none found: '",argument.c_str(),"'.Exiting...");
						mods::globals::shutdown();
					}
					std::string pw_file = argument.substr(19,argument.length()-19);
					std::ifstream in_file(pw_file.c_str(),std::ios::in | std::ios::binary);
					if(!in_file.good() || !in_file.is_open()) {
						log("SYSERR: unable to open password file. Exiting...");
						mods::globals::shutdown();
					} else {
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
						if(i == 1) {
							log("SYSERR: the postgres password is one character long. Please check the postgres password file");
							mods::globals::shutdown();
							return 0;
						}
						while(isspace(postgres_password[i--]) && i > 0) {}
						if(i == 0) {
							log("SYSERR: while trying to trim the end of the postgres password of spaces (using isspace()), the resulting postgres password is zero length. Exiting...");
							mods::globals::shutdown();
							return 0;
						}
						postgres_password = postgres_password.substr(0,i);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-host=",16) == 0) {
					if(argument.length()  < 16) {
						log("SYSERR: --postgres-host expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					} else {
						postgres_host = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-user=",16) == 0) {
					if(argument.length()  < 16) {
						log("SYSERR: --postgres-user expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					} else {
						postgres_user = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-dbname=",18) == 0) {
					if(argument.length() < 18) {
						log("SYSERR: --postgres-port expects an argument, none found: '",argv[pos],"'.Exiting...");
						mods::globals::shutdown();
					} else {
						postgres_dbname = argument.substr(18,argument.length()-14);
						continue;
					}
				}
				if(strncmp(argv[pos],"--postgres-port=",16) == 0) {
					if(argument.length() < 16) {
						log("SYSERR: --postgres-port expects an argument, none found: '",argv[pos],"'.Exiting...");
						mods::globals::shutdown();
					} else {
						postgres_port = argument.substr(16,argument.length()-16);
						continue;
					}
				}
				if(strncmp(argv[pos],"--lmdb-dir=",11) == 0) {
					if(argument.length() < 12) {
						log("SYSERR: --lmdb-dir expects an argument, none found: '",argv[pos],"'. Exiting...");
						mods::globals::shutdown();
					} else {
						lmdb_dir = argument.substr(11,argument.length()-11);
						log((std::string("DEBUG: found lmdb directory: ")+ lmdb_dir).c_str());
						continue;
					}
				}
			}

			if(!mods::util::dir_exists(lmdb_dir.c_str())) {
				auto err = mkdir(lmdb_dir.c_str(),0700);
				if(err == -1) {
					log("SYSERR: The lmdb database directory couldn't be created: ", mods::util::err::get_string(errno).c_str());
					mods::globals::shutdown();
				}
			}
			db = std::make_unique<lmdb_db>(lmdb_dir,lmdb_name,MDB_WRITEMAP,0600,true);
			player_nobody = nullptr;
			defer_queue = std::make_unique<mods::deferred>(mods::deferred::TICK_RESOLUTION);
			mods::js::create_new_context();
			mods::js::load_c_functions();
			/** TODO: make configurable */
			mods::js::load_library("/lib/contracts/contracts.js");
			mods::behaviour_tree_impl::load_trees();
			if(f_test_suite.length()) {
				//if(!f_test_suite.compare("db")){
				//	mods::testing::lmdb::db test(argc,argv);
				//}
				//if(!f_test_suite.compare("player_syncing")){
				//	mods::testing::lmdb::player_syncing test(argc,argv);
				//}
				mods::globals::shutdown();
			}
			bool connected_to_postgres = false;
			try {
				mods::conf::set_credentials({
					{"port",postgres_port},
					{"user",postgres_user},
					{"password",postgres_password},
					{"host",postgres_host},
					{"dbname",postgres_dbname}}
				                           );
				std::string connection_string = mods::conf::pq_connection().c_str();
				pq_con = std::make_unique<pqxx::connection>(connection_string.c_str());
				connected_to_postgres = true;
			} catch(const std::exception& e) {
				log(CAT("SYSERR: [POSTGRES CONNECTION ERROR]:'",e.what(),"'. Shutting down in 10 seconds...").c_str());
				std::cerr << red_str("[POSTGRES CONNECTION ERROR]:'") << red_str(e.what()) << "'\n";
				std::cerr << red_str("Shutting down in 10 seconds...\n");
				sleep(10);
				mods::globals::shutdown();
				exit(1);
			}
			if(connected_to_postgres) {
				log("OKAY: [postgres] connected :)");
			} else {
				log("SYSERR: Couldn't connect to postgres");
				mods::globals::shutdown();
				return 0;
			}
			if(migrations.size()) {
				for(auto t : migrations) {
					auto direction = std::get<0>(t);
					auto identifier = std::get<1>(t);
					mods::migrations::report_migration_status(CAT("running [",direction,"] migration: '",identifier,"'"),"status");
					std::tuple<bool,int,std::string> status = mods::migrations::run_migration(identifier,"run a migration from the cli",direction);
					int code = std::get<1>(status);
					std::string error_message = std::get<2>(status);
					if(code < 0) {
						mods::migrations::report_migration_status(CAT("Error running [",direction,"] migration: '",identifier,"': error message: '", error_message, "'"),"error");
						exit(1);
					} else {
						mods::migrations::report_migration_status(CAT("Successfully ran [",direction,"] migration: '",identifier,"'"),"success");
					}
				}
				std::cout << "All migrations ran. Exiting...\n";
				exit(0);
			}
			mods::debug::init(show_tics);
			mods::init::init();
			if(run_unit_tests) {
				mods::unit_tests::run();
				mods::globals::shutdown();
			}
			return 1; //proceed with game boot
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
				switch(event) {
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
					default:
						break;
				}
				if(text.length()) {
					text += "\r\n";
					send_to_char(ptr,text.c_str());
				}
				return true;
			});
		}
		void refresh_player_states() {
			mods::loops::foreach_all([&](player_ptr_t player_ptr) -> bool {
				auto ptr = player_ptr->cd();
				if(!ptr) {
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
			mods::chat::setup_public_channels();
			if(bootup_test_suite.length() > 0) {
				std::cout << "booting suite: " << bootup_test_suite << "\n";
				mods::pregame::boot_suite(bootup_test_suite);
			}
			if(!mods::debug::pre_game::run()) {
				exit(0);
			}
		}
		uuid_t player_uuid() {
			static uuid_t u = 0;
			return ++u;
		}
		uuid_t mob_uuid() {
			return player_uuid();
		}
		char_data* read_mobile(const mob_vnum& nr,const int& type) {
			return read_mobile_ptr(nr,type)->cd();
		}
		std::shared_ptr<mods::npc> read_mobile_ptr(const mob_vnum& nr,const int& type) {
			mob_rnum i;

			if(type == VIRTUAL) {
				if((i = real_mobile(nr)) == NOBODY) {
					log("WARNING: Mobile vnum ",nr," does not exist in database.");
					return nullptr;
				}
			} else {
				i = nr;
			}
			if(std::size_t(i) >= mob_proto.size()) {
				log("SYSERR: recontracted mob_proto index is invalid: ", i,". mob_proto.size() is currently: ", mob_proto.size(), ". Ignoring...");
				return nullptr;
			}

			auto& mob = mob_list.emplace_back(std::make_shared<mods::npc>(i));
			mob->position() = POS_STANDING;
			for(unsigned num_wears_i = 0; num_wears_i < NUM_WEARS; num_wears_i++) {
				GET_EQ(mob->cd(), num_wears_i) = nullptr;
			}
			mob->cd()->carrying = nullptr;
			mob->cd()->mob_specials.import(mob_proto[i].mob_specials);
			mob->cd()->mob_specials.vnum = nr;
			mob->cd()->points = mob_proto[i].points;
			mob->cd()->real_abils = mob_proto[i].real_abils;
			mob->cd()->player.level = mob_proto[i].player.level;

			mob->set_time_birth(time(0));
			mob->set_time_played(0);
			mob->set_time_logon(time(0));

			if(mob_index.size() > i) {
				mob_index[i].number++;
			}
			SET_BIT(mob->char_specials().saved.act, MOB_ISNPC);
			GET_POS(mob->cd()) = POS_STANDING;
			mob->uuid() = player_uuid();
			register_player(mob);
			mob->cd()->mob_specials.vnum = nr;
			mods::mobs::decorate(mob->uuid());
			mods::mob_equipment::decorate(mob->uuid());
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
		using coordinate_type_t = int32_t;
		std::map<std::tuple<coordinate_type_t,coordinate_type_t,coordinate_type_t>,room_vnum> coordinate_map;
		coordinate_type_t get_room_by_coordinates(coordinate_type_t target_x,coordinate_type_t target_y,coordinate_type_t target_z) {
			auto v = coordinate_map[std::make_tuple<>(target_x,target_y,target_z)];
			if(real_room(v) == NOWHERE) {
				return -1;
			}
			return world[real_room(v)].number;
		}
		void register_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room) {
			coordinate_map[std::make_tuple<>(x,y,z)] = room;
		}
		void glue_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room) {
			auto n = real_room(coordinate_map[std::make_tuple<>(x,y+1,z)]);
			auto e = real_room(coordinate_map[std::make_tuple<>(x+1,y,z)]);
			auto s = real_room(coordinate_map[std::make_tuple<>(x,y-1,z)]);
			auto w = real_room(coordinate_map[std::make_tuple<>(x-1,y,z)]);
			auto u = real_room(coordinate_map[std::make_tuple<>(x,y,z+1)]);
			auto d = real_room(coordinate_map[std::make_tuple<>(x,y,z-1)]);
			auto room_id = real_room(room);
			if(n != NOWHERE) {
				world[n].set_dir_option(
				    SOUTH,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
			if(e != NOWHERE) {
				world[e].set_dir_option(
				    WEST,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
			if(s != NOWHERE) {
				world[s].set_dir_option(
				    NORTH,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
			if(w != NOWHERE) {
				world[w].set_dir_option(
				    EAST,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
			if(u != NOWHERE) {
				world[u].set_dir_option(
				    DOWN,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
			if(d != NOWHERE) {
				world[d].set_dir_option(
				    UP,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    room_id
				);
			}
		}

		static const std::map<std::string_view,std::string_view> default_colors = {
			{"blu","\033[34m"},
			{"gld","\033[33m"},
			{"grn","\033[32m"},
			{"red","\033[31m"},
			{"wht","\033[37m"},
			{"yel","\033[93m"}
		};
		std::string custom_color_eval(std::string_view buffer,std::map<std::string_view,std::string_view> custom_color_map) {
			std::map<std::string_view,std::string_view> colors = custom_color_map;
			std::string final_buffer = "";
			const std::size_t len = buffer.length();
			for(std::size_t i=0; i < len; i++) {
				auto current_char = buffer[i];
				if(current_char == '{') {
					if(len > i + 5 && buffer[i+5] == '}' &&
					        buffer[i+1] == '/') {
						i += 5;
						final_buffer += "\033[0m";
						continue;
					}
					if(len > i + 4 && buffer[i+4] == '}') {
						const std::string_view substring = buffer.substr(i+1,3);
						if(colors.find(substring) != colors.end()) {
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

		std::string color_eval(std::string_view buffer) {
			return custom_color_eval(buffer,default_colors);
		}
		std::string strip_colors(std::string_view buffer) {
			std::string stripped = custom_color_eval(buffer, {
				{"blu",""},
				{"gld",""},
				{"grn",""},
				{"red",""},
				{"wht",""},
				{"yel",""}
			});
			std::string even_more_stripped = "";
			for(auto ch : stripped) {
				if(!isprint(ch)) {
					continue;
				}
				even_more_stripped += ch;
			}
			return even_more_stripped;
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

		bool command_interpreter(player_ptr_t player,std::string_view in_argument) {
			const bool locked_down = player->is_locked_down();
#ifdef __MENTOC_WRITE_HISTFILE__
			if(player->authenticated()) {
				player->write_histfile(in_argument);
			}
#endif
			std::string argument = in_argument.data();
			auto vec_args = PARSE_ARGS();
			if(player->paging()) {
				if(locked_down) {
					player->pager_clear();
					player->pager_end();
					mods::interpreter::douchebags::locked_down_player_always(player);
					return false;
				}
				if(vec_args.size() == 0) {
					player->pager_next_page();
					return false;
				}
				if(vec_args.size() && vec_args[0].compare("q") == 0) {
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
			if(vec_args.size()) {
				if(locked_down) {
					mods::interpreter::douchebags::locked_down_player_always(player);
					return false;
				}
				if(mods::chat::handle_chat(player,argument)) {
					return false;
				}
			}
			if(mods::super_users::player_is(player)) {
				mods::super_users::upkeep(player);
				if(argument.substr(0,4).compare("=pos") == 0) {
					if(argument.length() < 6) {
						player->stc("usage: =pos=<int>\r\n");
					} else {
						std::string pos = argument.substr(5);
						auto optional_result = mods::util::stoi_optional<int>(pos);
						if(optional_result.has_value() == false) {
							player->stc("usage: =pos=<int>\r\n");
						} else {
							player->position() = optional_result.value();
							player->done();
						}
					}
					return false;
				}
				if(argument.substr(0,4).compare("+imp") == 0) {
					mods::acl_list::set_access_rights(player,"implementors",true);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-imp") == 0) {
					mods::acl_list::set_access_rights(player,"implementors",false);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("+god") == 0) {
					mods::acl_list::set_access_rights(player,"gods",true);
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-god") == 0) {
					mods::acl_list::set_access_rights(player,"gods",false);
					player->done();
					return false;
				}
				if(argument.substr(0,6).compare("+build") == 0) {
					mods::acl_list::set_access_rights(player,"builders",true);
					player->done();
					return false;
				}
				if(argument.substr(0,6).compare("-build") == 0) {
					mods::acl_list::set_access_rights(player,"builders",false);
					player->done();
					return false;
				}
				if(argument.substr(0,7).compare("-affplr") ==0) {
					player->clear_all_affected();
					player->clear_all_affected_plr();
					player->done();
					return false;
				}

				if(argument.substr(0,4).compare("-aff") == 0) {
					player->clear_all_affected();
					player->done();
					return false;
				}
				if(argument.substr(0,4).compare("-plr") == 0) {
					player->clear_all_affected_plr();
					player->done();
					return false;
				}
			}
			if(locked_down) {
				mods::interpreter::douchebags::locked_down_player_always(player);
				return false;
			}
			if(mods::drone::started(player->uuid())) {
				d("drone started. interpretting");
				return mods::drone::interpret(player->uuid(),in_argument.data());
			}
			if(player->room_pave_mode()) {
				player->send("[paving]\r\n");
				//If is a direction and that direction is not an exit,
				//then pave a way to that exit
				int door = 0;
				int x = 0, y = 0, z = 0;
				if(in_argument.length() == 1) {
					switch(in_argument[0]) {
						case 'u':
						case 'U':
							door = UP;
							z += 1;
							break;

						case 's':
						case 'S':
							y -= 1;
							door = SOUTH;
							break;

						case 'w':
						case 'W':
							x -= 1;
							door = WEST;
							break;

						case 'e':
						case 'E':
							x += 1;
							door = EAST;
							break;

						case 'n':
						case 'N':
							y += 1;
							door = NORTH;
							break;

						case 'd':
						case 'D':
							z -= 1;
							door = DOWN;
							break;
						default:
							return true;
					}

					auto cached_room = player->room();
					int target_x = world[cached_room].x + x;
					int target_y = world[cached_room].y + y;
					int target_z = world[cached_room].z + z;
					auto current_zone = world[cached_room].zone;
					room_vnum existing_room_vnum = get_room_by_coordinates(target_x,target_y,target_z);
					room_data* existing_room = nullptr;
					int real_room_id = 0;
					if(existing_room_vnum > -1) {
						real_room_id = real_room(existing_room_vnum);
						if(real_room_id != NOWHERE) {
							existing_room = &world[real_room_id];
						}
					}
					if(world[cached_room].dir_option[door] == nullptr) {
						if(existing_room) {
							world[cached_room].set_dir_option(
							    door,
							    "general_description",
							    "keyword",
							    EX_ISDOOR,
							    0,
							    real_room_id
							);
							existing_room->set_dir_option(
							    OPPOSITE_DIR(door),
							    "general description",
							    "keyword",
							    EX_ISDOOR,
							    0,
							    cached_room
							);
							mods::builder::add_room_to_pavements(player,cached_room);
							mods::builder::add_room_to_pavements(player,real_room_id);
							glue_room_at_coordinates(existing_room->x,existing_room->y,existing_room->z,existing_room->number);
							return true;
						}
						player->sendln("Creating room in that direction");
						int new_room_rnum = 0;
						world.emplace_back();
						register_room(0);
						int new_room_vnum = next_room_vnum();
						auto& w = world.back();
						w.number = new_room_vnum;
						w.zone = current_zone;
						new_room_rnum = world.size() - 1;

						world[cached_room].set_dir_option(
						    door,
						    "general_description",
						    "keyword",
						    EX_ISDOOR,
						    0,
						    new_room_rnum
						);
						w.set_dir_option(
						    OPPOSITE_DIR(door),
						    "general description",
						    "keyword",
						    EX_ISDOOR,
						    0,
						    cached_room
						);
						w.x = target_x;
						w.y = target_y;
						w.z = target_z;
						register_room_at_coordinates(w.x,w.y,w.z,w.number);
						glue_room_at_coordinates(w.x,w.y,w.z,w.number);
						mods::builder::add_room_to_pavements(player,cached_room);
						mods::builder::add_room_to_pavements(player,new_room_rnum);
					}
				}
			}
			return true;
		}

		void pad_room(int room,char_data* ch,int door) {
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
			for(; room_list.size() < world.size();) {
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
				ch->was_in_room = room_id;
				if(room_id >= room_list.size()) {
					log("SYSERR: char_from_room failed. room_id >= room_list.size()");
					return;
				}
				bool found = 0;
				do {
					auto place = std::find(
					                 mods::globals::room_list[room_id].begin(),
					                 mods::globals::room_list[room_id].end(),
					                 player
					             );
					if(place != mods::globals::room_list[room_id].end()) {
						mods::globals::room_list[room_id].erase(place);
						found = true;
					} else {
						found = false;
					}
				} while(found);
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
			bool char_to_room(const room_rnum& room,char_data* ch) {
				auto player = ptr(ch);
				if(player->builder_data) {
					player->builder_data->room_recorder.char_to_room(room);
				}
				auto target_room = room;
				if(boot_type == boot_type_t::BOOT_HELL) {
					target_room = 0;
				}
				if(target_room >= room_list.size()) {
					log("SYSERR: char_to_room failed for ch. Recontracted room is out of bounds: ",target_room);
					return false;
				}
				if(!mods::movement::char_move_to(player,target_room)) {
#ifdef __MENTOC_SHOW_CHAR_MOVE_TO_FAILURE_MESSAGE__
					std::cerr << red_str("cannot move to target room") << "\n";
#endif
					player->set_room(ch->was_in_room);
					mods::globals::room_list[ch->was_in_room].push_back(player);
					player->moving_to_room() = false;
					return false;
				}
				player->set_room(target_room);
				mods::globals::room_list[target_room].push_back(player);
				if(!IS_NPC(ch)) {
#ifdef __MENTOC_SHOW_ROOM_EVENTS_DEBUG__
					std::cerr << "[room_entry] watching events: " << player->name().c_str() << "\n";
#endif
					mods::mobs::room_watching::events::room_entry(player);
				}
				player->moving_to_room() = false;
				return true;
			}
		};//end namespace rooms

		void affect_room_light(int room,int offset) {
			if(room < world.size() && room >= 0) {
				world[room].light -= offset;
			}
		}
		static mods::globals::player_list_t blank_room;
		player_list_t& get_room_list(room_rnum room) {
			if(room == NOWHERE || room >= mods::globals::room_list.size()) {
				return blank_room;
			}
			return mods::globals::room_list[room];
		}
		player_list_t& get_room_list(player_ptr_t& player) {
			return mods::globals::get_room_list(player->room());
		}
		std::string dir_to_str(int dir, bool adjective) {
			if(adjective) {
				switch(dir) {
					case NORTH:
						return "northern";
					case SOUTH:
						return "southern";
					case EAST:
						return "eastern";
					case WEST:
						return "western";
					case UP:
						return "upper";
					case DOWN:
						return "below";
					default:
						return "?";
				}
			}
			switch(dir) {
				case NORTH:
					return "north";
				case SOUTH:
					return "south";
				case EAST:
					return "east";
				case WEST:
					return "west";
				case UP:
					return "up";
				case DOWN:
					return "down";
				default:
					return "?";
			}
		}
		void queue_object_destruct(uuid_t obj_uuid, uint16_t ticks) {
			mods::globals::defer_queue->push_ticks_event(ticks, {obj_uuid, mods::deferred::EVENT_OBJECT_DESTRUCT});
		}
		void destruct_object(uuid_t uuid) {
			auto obj = optr_by_uuid(uuid);
			if(!obj) {
				return;
			}
			for(const auto& pair : obj_map) {
				if(pair.second.get() == obj.get()) {
					obj_map.erase(pair.first);
					return destruct_object(uuid);
				}
			}
			for(const auto& pair : obj_odmap) {
				if(pair.second.get() == obj.get()) {
					obj_odmap.erase(pair.first);
					return destruct_object(uuid);
				}
			}
			for(const auto& pair : db_id_to_uuid_map) {
				if(pair.second == obj->uuid) {
					db_id_to_uuid_map.erase(pair.first);
					return destruct_object(uuid);
				}
			}
		}
		void recursive_obj_list_erase(obj_data* obj) {
			for(auto it = obj_list.begin();
			        it != obj_list.end(); ++it) {
				if(it->get() == obj) {
					obj_list.erase(it);
					return recursive_obj_list_erase(obj);
				}
			}
		}

		void dispose_object(uuid_t obj_uuid) {
			auto obj = optr_by_uuid(obj_uuid);
			if(!obj) {
				return;
			}
			obj_from_room(obj);
			destruct_object(obj_uuid);
			recursive_obj_list_erase(obj.get());
			mods::rifle_attachments::erase(obj_uuid);
#ifdef __MENTOC_SHOW_DISPOSE_OBJECT_DEBUG_OUTPUT__
			std::cerr << green_str("USE_COUNT:") << obj.use_count() << "\n";
#endif
			if(obj.use_count()) {
				obj.reset();
			}
		}
		void dispose_player(uuid_t pl_uuid) {
#define __MENTOC_SHOW_DISPOSE_PLAYER_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_DISPOSE_PLAYER_DEBUG_OUTPUT__
#define mgdp_debug(a) mentoc_prefix_debug("mods::globals::dispose_player") << a << "\n";
#else
#define mgdp_debug(a) /**/
#endif
			auto player = ptr_by_uuid(pl_uuid);
			if(!player) {
				mgdp_debug("ptr_by_uuid returned an invalid player ptr... not removing from any structures!");
				return;
			}
			{
				auto p_room_list = mods::globals::get_room_list(player->room());
				auto it = std::find(p_room_list.begin(),p_room_list.end(),player);
				if(it != p_room_list.end()) {
					mgdp_debug("Erasing from player p_room_list");
					p_room_list.erase(it);
				}
			}
			/** TODO remove from room_list */
			if(player->is_npc()) {
				mgdp_debug("Erasing from mob_chmap");
				player_map.erase(player->uuid());
				{
					auto ch = player->cd();
					std::vector<std::deque<std::shared_ptr<mods::npc>>::iterator> mob_list_erasures;
					for(auto m_it = mob_list.begin(); m_it != mob_list.end(); ++m_it) {
						if((*m_it)->cd() == ch) {
							mgdp_debug("Found one in mob_list... emplacing...");
							mob_list_erasures.emplace_back(m_it);
						}
					}
					for(auto&& erase_me : mob_list_erasures) {
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
				if(it != player_map.end()) {
					mgdp_debug("Found in player_map.. erasing...");
					player_map.erase(it);
				}
			}
			{
				mgdp_debug("Looking in player_list...");
				auto it = std::find(player_list.begin(), player_list.end(), player);
				if(it != player_list.end()) {
					mgdp_debug("Found in player_list... erasing...");
					player_list.erase(it);
				}
			}
			mgdp_debug("At end of function");
		}
	};//end globals
};
player_ptr_t ptr(char_data* in_ch) {
	return ptr((const char_data*)in_ch);
}
player_ptr_t ptr(const char_data* in_ch) {
	if(!in_ch) {
		return nullptr;
	}
	auto it = mods::globals::player_chmap.find(const_cast<char_data*>(in_ch));
	if(it == mods::globals::player_chmap.cend()) {
		return nullptr;
	}
	return it->second;
}
obj_ptr_t optr(obj_data* in_obj) {
	if(!in_obj) {
		return nullptr;
	}
	auto it = mods::globals::obj_odmap.find(in_obj);
	if(it == mods::globals::obj_odmap.cend()) {
		return nullptr;
	}
	return it->second;
}

std::optional<obj_ptr_t> optr_opt(uuid_t obj_uuid) {
	auto it = mods::globals::obj_map.find(obj_uuid);
	if(it != mods::globals::obj_map.end()) {
		return it->second;
	}
	log("SYSERR: DID NOT find obj_uuid (optr) %d ",obj_uuid);
	return std::nullopt;
}
std::optional<player_ptr_t> ptr_opt(char_data* ch) {
	auto it = mods::globals::player_chmap.find(ch);
	if(it != mods::globals::player_chmap.end()) {
		return it->second;
	}
	return std::nullopt;
}
std::optional<player_ptr_t> ptr_opt(uuid_t plr_uuid) {
	auto it = mods::globals::player_map.find(plr_uuid);
	if(it != mods::globals::player_map.end()) {
		return it->second;
	}
	return std::nullopt;
}
player_ptr_t ptr_by_uuid(uuid_t id) {
	auto it = mods::globals::player_map.find(id);
	if(it != mods::globals::player_map.end()) {
		return it->second;
	}
	return nullptr;
}
obj_ptr_t optr_by_uuid(uuid_t id) {
	return mods::globals::obj_map[id];
}
std::shared_ptr<mods::npc> npc_by_uuid(const uuid_t& u) {
	auto it = std::find_if(mob_list.cbegin(),mob_list.cend(),[&u](const auto& ptr) -> bool {
		return ptr->uuid() == u;
	});
	if(it == mob_list.cend()) {
		return nullptr;
	}
	return *it;
}
std::string dirstr(int dir) {
	switch(dir) {
		case NORTH:
			return "NORTH";
		case SOUTH:
			return "SOUTH";
		case EAST:
			return "EAST";
		case WEST:
			return "WEST";
		case UP:
			return "UP";
		case DOWN:
			return "DOWN";
		default:
			return "UNKNOWN";
	}
}
std::string IMPLODE_PAD(std::string delim_one,std::vector<std::string> m,std::string delim_two) {
	return IMPLODE_PAD(delim_one,m,delim_two,false);
}
std::string IMPLODE_PAD(std::string delim_one,std::vector<std::string> m,std::string delim_two,bool ignore_empty) {
	std::string f = "";
	std::size_t size = 0;
	for(auto s : m) {
		size += s.length() + delim_one.length() + delim_two.length();
	}
	f.reserve(size);
	for(auto s : m) {
		if(s.length() == 0 && ignore_empty) {
			continue;
		}
		f += delim_one + s + delim_two;
	}
	return f;
}
std::string IMPLODE(std::vector<std::string> m,std::string delim) {
	return IMPLODE(m,delim,false);
}
std::string IMPLODE(std::vector<std::string> m,std::string delim, bool ignore_empty) {
	std::string f = "";
	std::size_t size = 0;
	for(auto s : m) {
		size += s.length() + delim.length();
	}
	f.reserve(size);
	for(auto s : m) {
		if(s.length() == 0 && ignore_empty) {
			continue;
		}
		f += s;
		f += delim;
	}
	return f;
}
std::vector<str_t> map_keys(str_map_t& m) {
	std::vector<str_t> s;
	for(auto& pair : m) {
		s.emplace_back(pair.first);
	}
	return s;
}

str_vec_t EXPLODE(str_t value,char delimiter) {
	std::vector<std::string> strings;
	std::string current = "";
	for(auto ch : value) {
		if(ch == delimiter && current.length()) {
			strings.emplace_back(current);
			current = "";
			continue;
		}
		if(ch != delimiter) {
			current += ch;
		}
	}
	if(current.length()) {
		strings.emplace_back(current);
	}
	return strings;
}
str_vec_t EXPLODE(str_t& value,char delimiter) {
	std::vector<std::string> strings;
	std::string current = "";
	for(auto ch : value) {
		if(ch == delimiter && current.length()) {
			strings.emplace_back(current);
			current = "";
			continue;
		}
		if(ch != delimiter) {
			current += ch;
		}
	}
	if(current.length()) {
		strings.emplace_back(current);
	}
	return strings;
}
std::pair<bool,direction_t> to_direction(std::string_view str) {
	auto s = mods::util::trim_view(str);
	if(s.length() == 0 || !std::isalpha(s[0])) {
		return {0,NORTH};
	}
	switch(std::tolower(s[0])) {
		case 'n':
			return {1,NORTH};
		case 'e':
			return {1,EAST};
		case 's':
			return {1,SOUTH};
		case 'w':
			return {1,WEST};
		case 'u':
			return {1,UP};
		case 'd':
			return {1,DOWN};
		default:
			return {0,NORTH};
	}
}
player_ptr_t ptr_by_db_id(const uint64_t& player_db_id) {
	auto it = mods::globals::db_id_to_uuid_map.find(player_db_id);
	if(it == mods::globals::db_id_to_uuid_map.end()) {
		return nullptr;
	}
	return ptr_by_uuid(it->second);
}
#endif
