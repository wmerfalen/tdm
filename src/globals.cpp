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
#include "mods/testing_index.hpp"

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
		boot_type_t boot_type;
		socket_map_t socket_map;
		std::unique_ptr<lmdb_db> db;
		std::shared_ptr<player> player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		duk_context* duktape_context;
		ai_state_map states;
		std::vector<std::vector<char_data*>> room_list;
		player_list_t player_list;
		std::vector<mods::chat::channel> chan;
		std::vector<std::string> chan_verbs;
		bool f_import_rooms;
		std::shared_ptr<mods::player> current_player;
		std::string bootup_test_suite;
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

		std::string replace_all(std::string str, const std::string& from, const std::string& to) {
			size_t start_pos = 0;

			while((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}

			return str;
		}

		void register_object_list() {
			for(auto& i : object_list) {
				obj_map.insert({i.item_number,&i});
			}
		}
		void register_object(obj_data& obj) {
			if(obj.name) {
				/** !fixme: this is lame. Don't search within the name to find out it's type. That's slow af. */
				if(std::string(obj.name).find("[ammo]") !=std::string::npos) {
					obj.holds_ammo = 1;
				}

				/** !fixme: see above fixme */
				if(std::string(obj.name).find("snipe") != std::string::npos) {
					obj.holds_ammo = 1;
					obj.ammo = 12;
					obj.ammo_max = 12;
				} else {
					obj.holds_ammo = 0;
					obj.ammo = 0;
					obj.ammo_max = 0;
				}
			}

			obj_map.insert({obj.item_number,&obj});
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
			while(++pos < argc){
				if(argv[pos]){
					argument = argv[pos];
				}else{
					argument = "";
				}
				if(strncmp(argv[pos],"--sql-port",10) == 0){
					exit(port_main(argc -1,static_cast<char**>(&argv[1])));
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
					continue;
				}
				if(strncmp(argv[pos],"--lmdb-dir=",11) == 0){
					if(argument.length() < 12){
						std::cerr << "--lmdb-dir expects an argument, none found: " << argv[pos] <<"\n"
							<< "Exiting...\n";
						mods::globals::shutdown();
					}
					lmdb_dir = argument.substr(11,argument.length()-11);
					continue;
				}
			}

			if(!mods::util::dir_exists(lmdb_dir.c_str())){
				auto err = mkdir(lmdb_dir.c_str(),0700);
				if(err == -1){
					log(
							(std::string("SYSERR: The lmdb database directory couldn't be created: ") + mods::util::err::get_string(errno)).c_str()
						 );
					mods::globals::shutdown();
				}
			}
			db = std::make_unique<lmdb_db>(lmdb_dir,lmdb_name,MDB_WRITEMAP,0600,true);
			player_nobody = nullptr;
			defer_queue = std::make_unique<mods::deferred>(mods::deferred::TICK_RESOLUTION);
			duktape_context = mods::js::new_context();
			mods::js::load_c_functions();
			mods::js::load_library(mods::globals::duktape_context,"../../lib/quests/quests.js");
			mods::behaviour_tree_impl::load_trees();
			if(f_test_suite.length()){
				if(!f_test_suite.compare("db")){
					mods::testing::lmdb::db test(argc,argv);
				}
				if(!f_test_suite.compare("player_syncing")){
					mods::testing::lmdb::player_syncing test(argc,argv);
				}
				mods::globals::shutdown();
			}
			config::init(argc,argv);
		}
		void post_boot_db() {
		}
		void room_event(struct char_data* ch,mods::ai_state::event_type_t event) { }
		const char* say_random(const mods::ai_state::event_type_t& event) {
			return "woof";
		}
		void room_event(room_vnum room,mods::ai_state::event_type_t event) {
			for(auto ptr = character_list; ptr->next; ptr = ptr->next) {
				if(IN_ROOM(ptr) == room) {
					if(event == mods::ai_state::BREACHED_NORTH  ||
							event == mods::ai_state::BREACHED_SOUTH  ||
							event == mods::ai_state::BREACHED_EAST ||
							event == mods::ai_state::BREACHED_WEST
						) {
						ptr->disorient += MODS_BREACH_DISORIENT;
						{
							if(event == mods::ai_state::BREACHED_NORTH) {
								send_to_char(ptr,"The {red}north{/red} door was breached.\r\n");
							}

							if(event == mods::ai_state::BREACHED_SOUTH) {
								send_to_char(ptr,"The {red}south{/red} door was breached.\r\n");
							}

							if(event == mods::ai_state::BREACHED_EAST) {
								send_to_char(ptr,"The {red}east{/red} door was breached.\r\n");
							}

							if(event == mods::ai_state::BREACHED_WEST) {
								send_to_char(ptr,"The {red}west{/red} door was breached.\r\n");
							}
						}
					}

					if(event == mods::ai_state::GRENADE_FLIES_BY) {
						send_to_char(ptr,"A {grn}grenade{/grn} flies by\r\n");
					}

					if(event == mods::ai_state::GRENADE_EXPLOSION) {
						send_to_char(ptr,"A {grn}grenade{/grn} explodes!\r\n");
					}
				}
			}
		}
		void refresh_player_states() {
			if(character_list) {
				for(auto ptr = character_list; ptr->next; ptr = ptr->next) {
					if(states.find(ptr) == states.end()) {
						states[ptr] = std::make_unique<mods::ai_state>(ptr,0,0);
					}
				}
			}
		}
		void pre_game_loop() {
			std::cerr << "[event] Pre game loop\n";
			refresh_player_states();
			if(bootup_test_suite.length() > 0){
				mods::pregame::boot_suite(bootup_test_suite);
			}
		}
		char_data* read_mobile(const mob_vnum & nr,const int & type){
			mob_rnum i;

			if(type == VIRTUAL) {
				if((i = real_mobile(nr)) == NOBODY) {
					log("WARNING: Mobile vnum %d does not exist in database.", nr);
					return nullptr;
				}
			} else {
				i = nr;
			}
			if(mob_proto.size() <= std::size_t(i)){
				std::cerr << "[mods::globals::read_mobile]: requested mob_proto index is invalid: " << i << ". mob_proto.size() is currently: " << mob_proto.size() << "\nIgnoring...\n";
				return nullptr;
			}

			mob_list.emplace_back(mob_proto[i]);
			(mob_list.end()-1)->next = character_list;
			auto mob = character_list = &(*(mob_list.end()-1));

			if(!mob->points.max_hit) {
				mob->points.max_hit = dice(mob->points.hit, mob->points.mana) + mob->points.move;
			} else {
				mob->points.max_hit = rand_number(mob->points.hit, mob->points.mana);
			}

			mob->points.hit = mob->points.max_hit;
			mob->points.mana = mob->points.max_mana;
			mob->points.move = mob->points.max_move;

			mob->player.time.birth = time(0);
			mob->player.time.played = 0;
			mob->player.time.logon = time(0);

			mob_index[i].number++;
			return (mob);
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
		std::string color_eval(std::string final_buffer) {
			final_buffer = replace_all(final_buffer,"{grn}","\033[32m");
			final_buffer = replace_all(final_buffer,"{red}","\033[31m");
			final_buffer = replace_all(final_buffer,"{blu}","\033[34m");
			final_buffer = replace_all(final_buffer,"{wht}","\033[37m");
			final_buffer = replace_all(final_buffer,"{gld}","\033[33m");
			final_buffer = replace_all(final_buffer,"{/grn}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/wht}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/red}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/blu}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/gld}","\033[0m");
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

		bool command_interpreter(std::shared_ptr<mods::player> player,const std::string& argument) {
			if(std::find(super_users.begin(),super_users.end(),player->name().c_str()) != super_users.end()){
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
				return false;
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

if(player->has_builder_data() && player->room_pave_mode()) {
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
		}

		if(!CAN_GO(player->cd(),door)) {
			if(player->room() < 0){
				std::cerr << "error: player's room is less than zero. Not paving.\n";
				return false;
			}
			if(player->room() >= world.size()){
				std::cerr << "error: player's room is outside of world.size()\n";
				return false;
			}
			player->stc("[stub] pave_to\n");
			//mods::builder::pave_to(*player,&world[player->room()],door);
			return true;
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
			MENTOC_PREAMBLE();
			player->set_class_capability({mods::classes::types(ch->player.chclass)});
		}

		namespace rooms {
			void char_from_room(struct char_data* ch) {
				auto room_id = IN_ROOM(ch);
				auto place = std::find(room_list[room_id].begin(),room_list[room_id].end(),ch);

				if(place == room_list[room_id].end()) {
					return;
				} else {
					room_list[room_id].erase(place);
				}
			}

			void char_to_room(const room_rnum& room,struct char_data* ch) {
				auto target_room = room;
				if(boot_type == boot_type_t::BOOT_HELL){
					target_room = 0;
				}else if(target_room >= 0 && std::size_t(target_room) >= room_list.size()){
					std::cerr << "[char_to_room]: WARNING. Requested room is out of bounds: " << target_room << "\n";
					return;
				}
				auto place = std::find(room_list[target_room].begin(),room_list[target_room].end(),ch);

				if(place == room_list[target_room].end()) {
					room_list[target_room].push_back(ch);
				}
			}
		};

	};

};


#endif
