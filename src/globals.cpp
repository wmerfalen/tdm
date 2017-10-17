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
#include "utils.h"

#define LMDB_DB_FILE "/home/llvm/code/c++/bnull-mud/lib/"
#define LMDB_DB_NAME "bnull"
#define MODS_BREACH_DISORIENT 50
struct char_data* character_list = NULL;
extern struct obj_data* object_list;
extern void do_look(struct char_data *ch, char *argument, int cmd, int subcmd);
namespace mods {
    namespace globals {
		using player = mods::player;
		using lmdb_db = gdns::lmdb::db;
		std::unique_ptr<lmdb_db> db;
		std::unique_ptr<mods::acl::FileParser> config;
		std::shared_ptr<player> player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		ai_state_map states;
		namespace objects {
			static bool populated = false;
		};
		bool acl_good = false;
		int acl_parse_code = 0;
		/* Maps */
		map_player_list player_map;
		map_object_list obj_map;
template <typename I>
I random_element(I begin, I end)
{
    const unsigned long n = std::distance(begin, end);
    const unsigned long divisor = (RAND_MAX + 1) / n;

    unsigned long k;
    do { std::srand(std::time(0)); k = std::rand() / divisor; } while (k >= n);

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
		 
		void register_object_list(){
			for(auto i = object_list; i->next; i = i->next){
				obj_map.insert({i->item_number,i});
			}
		}
		void register_object(obj_data& obj){
			if(obj.name){
				if(std::string(obj.name).find("[ammo]") !=std::string::npos){
					obj.holds_ammo = 1;
				}
				if(std::string(obj.name).find("snipe") != std::string::npos){
					obj.holds_ammo = 1;
					obj.ammo = 12;
					obj.ammo_max = 12;
					std::cout << "############FOUND SNIPER RIFLE#############\n";
					std::cout << obj.item_number << "\n";
				}else{
					obj.holds_ammo = 0;
					obj.ammo = 0;
					obj.ammo_max = 0;
				}
			}
			obj_map.insert({obj.item_number,&obj});
		}
		bool acl_allowed(struct char_data *ch,const char* command_name,const char* file,int cmd,const char* arg,int subcmd){
			return false;
		}
		int mobile_activity(char_data* ch){
			//TODO: Make calls to stock stuff like this: ::stock::do_look(*ch, nullptr 0,0);
			if(states.find(ch) != states.end()){
				return states[ch]->dispatch(ch);
			}else{
				states[ch] = std::make_unique<mods::ai_state>(ch,mods::ai_state::WANDER,250);
			}
			do_look(ch,nullptr,0,0);
			return 1;
		}
		void init(){
			config = std::make_unique<mods::acl::FileParser>();
			db = std::make_unique<lmdb_db>(LMDB_DB_FILE,LMDB_DB_NAME,MDB_WRITEMAP | MDB_NOLOCK,0600,true);
			config->setFile("acl.conf");
			auto ret = config->parse();
			acl_parse_code = ret;
			if(ret < 0){
				std::cerr << "Error parsing config file: " << ret << " error code\n";
				acl_good = false;
			}else{
				acl_good = true;
			}
			player_nobody = nullptr;
			defer_queue = std::make_unique<mods::deferred>(TICK_RESOLUTION);
		}
		void room_event(struct char_data* ch,mods::ai_state::event_type_t event){
			for(auto c = ch; c->next_in_room; c = c->next_in_room){
				state_fetch(c)->event(ch,event);
			}
		}
		const char* say_random(const mods::ai_state::event_type_t & event){
					const std::vector<const char*> witness = {
						"I really shouldn't be a part of this",
						"Whatever you're doing, you ought to stop! Right NOW!",
						"I'm calling the cops...",
						"Fucking low lives...",
						"Scum of the earth! And this is where our tax dollars go to ladies and gentlemen.",
						"Oh god, it's happening...",
						"JESUS CHRIST WTF",
						"Umm, you betta stop whatchu doin there",
						"So it begins..."
					};
					const std::vector<const char*>wander = {
						"Women themselves always still have in the background of all personal vanity an impersonal contempt -- for \"woman\"",
						"Who has not, for the sake of his good reputation-- sacrificed himself once?",
						"One begins to mistrust very clever people when they become embarrassed",
						"Terrible experiences pose the riddle whether the person who has them is not terrible",
						"Heavy, heavy-spirited people become lighter precisely through what makes others heavier, through hatred and love, and for a time they surface",
						"Affability contains no hatred of men, but for that very reason too much contempt for me",
						"A man's maturity-- consists in having found again the seriousness one had as a child, at play",
						"To be ashamed of one's immorality-- that is a step on the staircase at whose end one is also ashamed of one's morality",
						"One should part from life as Odysseus parted from Nausicaa-- blessing it rather than in love with it",
						"What? A great man? I always see only the actor of his own ideal",
						"If we train our conscience, it kisses us while it hurts us"
					};
			switch(event){
				case mods::ai_state::AI_WITNESS_ATTACK:
					//TODO: Load these strings from LMDB which loads from a file
					return *select_randomly(witness.begin(),witness.end());
					break;
				case mods::ai_state::AI_EVENT_WANDER:
					return *select_randomly(wander.begin(),wander.end());
					break;
				default:
					return "woof";
					break;
			}
			return "woof";
		}
		void room_event(room_vnum room,mods::ai_state::event_type_t event){
			for(auto ptr = character_list; ptr->next; ptr = ptr->next){
				if(IN_ROOM(ptr) == room){
					if(event == mods::ai_state::BREACHED_NORTH  || 
						event == mods::ai_state::BREACHED_SOUTH  || 
						event == mods::ai_state::BREACHED_EAST ||
						event == mods::ai_state::BREACHED_WEST
					){
						ptr->disorient += MODS_BREACH_DISORIENT;
						{
							if(event == mods::ai_state::BREACHED_NORTH){
								send_to_char(ptr,"The {red}north{/red} door was breached.\r\n");
							}
							if(event == mods::ai_state::BREACHED_SOUTH){
								send_to_char(ptr,"The {red}south{/red} door was breached.\r\n");
							}
							if(event == mods::ai_state::BREACHED_EAST){
								send_to_char(ptr,"The {red}east{/red} door was breached.\r\n");
							}
							if(event == mods::ai_state::BREACHED_WEST){
								send_to_char(ptr,"The {red}west{/red} door was breached.\r\n");
							}
						}
					}
				}
			}
		}
		void refresh_player_states(){
			for(auto ptr = character_list; ptr->next; ptr = ptr->next){
				if(states.find(ptr) == states.end()){
					states[ptr] = std::make_unique<mods::ai_state>(ptr,0,0);
				}
			}
		}
		void pre_game_loop(){
			std::cout << "Pre game loop\n";
			refresh_player_states();
		}
		void load_player_map(){
			for(auto it = character_list; it ;it = it->next){
				std::cout << ".";
				it->uuid = get_uuid();
				mods::globals::player_map.insert({it->uuid,std::make_shared<mods::player>(static_cast<char_data*>(it))});
			}
		}
		uuid_t get_uuid(){
			static uuid_t u = 0;
			return ++u;
		}
		uuid_t obj_uuid(){
			static uuid_t u = 0;
			return ++u;
		}
   	  	std::unique_ptr<ai_state>& state_fetch(struct char_data* ch){
            if(states.find(ch) == states.end()){
                states[ch] = std::make_unique<ai_state>(ch,0,0);
            }
            return states[ch];
        }
   		int opposite_dir(int dir){
			if(dir == NORTH){ return SOUTH; }
			if(dir == SOUTH){ return NORTH; }
			if(dir == EAST){ return WEST; }
			if(dir == WEST){ return EAST; }
			if(dir == UP){ return DOWN; }
			if(dir == DOWN){ return UP; }
			return NORTH;
		}
		std::string color_eval(std::string final_buffer){
			final_buffer = replace_all(final_buffer,"{grn}","\033[32m");
			final_buffer = replace_all(final_buffer,"{red}","\033[31m");
			final_buffer = replace_all(final_buffer,"{blu}","\033[34m");
			final_buffer = replace_all(final_buffer,"{wht}","\033[37m");
			final_buffer = replace_all(final_buffer,"{/grn}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/wht}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/red}","\033[0m");
			final_buffer = replace_all(final_buffer,"{/blu}","\033[0m");
			return final_buffer;
		}
 
    };

};


#endif
