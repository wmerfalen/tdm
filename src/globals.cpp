#ifndef __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__
#define __CIRCLEMUD_SRC_MODS_GLOBALS_SOURCE__ 1

#include "globals.hpp"
#include "mods/player.hpp"
#include "mods/deferred.hpp"
#include <map>
#include <memory>

struct char_data* character_list = NULL;
extern struct obj_data* object_list;
namespace mods {
    namespace globals {
		using player = mods::player;
		std::unique_ptr<mods::acl::FileParser> config;
		std::shared_ptr<player> player_nobody;
		std::unique_ptr<mods::deferred> defer_queue;
		namespace objects {
			static bool populated = false;
		};
		bool acl_good = false;
		int acl_parse_code = 0;
		/* Maps */
		map_player_list player_map;
		map_object_list obj_map;
		void register_object_list(){
			for(auto i = object_list; i->next; i = i->next){
				obj_map.insert({i->item_number,i});
			}
		}
		void register_object(obj_data& obj){
			if(obj.name){
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
		void init(){
			config = std::make_unique<mods::acl::FileParser>();
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
    };

};


#endif
