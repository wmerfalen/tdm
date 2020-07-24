#include "mini-gunner.hpp"
#include <map>
#include <memory>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"

#define __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#define mini_debug(a) mentoc_prefix_debug("mods::mobs::mini_gunner") << a << "\n";
#else
#define mini_debug(a) ;;
#endif
namespace mods::mobs {

	int weighted_direction_decider(player_ptr_t& mob){
		int depth = MINI_GUNNER_SCAN_DEPTH();
		mods::scan::vec_player_data vpd;
		mods::scan::los_scan_for_players(mob->cd(),depth,&vpd);
		std::map<int,int> scores;
		for(auto v : vpd){
			if(!ptr_by_uuid(v.uuid)){
				continue;
			}
			if(mods::rooms::is_peaceful(v.room_rnum)){
				continue;
			}
			++scores[v.direction];
		}
		int should_fire = -1;
		int max = 0;
		for(auto pair : scores){
			if(pair.second > max){
				max = pair.second;
				should_fire = pair.first;
			}
		}
		/** TODO when was the last time this mob saw a target? if should_fire is -1, go there */
		if(should_fire == -1){
			/** FIXME */
			std::cerr << "[stub] should_fire is -1, choose random direction\n";
		}
		return should_fire;
	}
	std::map<uuid_t,std::shared_ptr<mini_gunner>> mg_map;
	void mini_gunner::create(uuid_t mob_uuid,std::string variation){
		mini_debug("mini_gunner create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			return;
		}
		mg_map.insert({mob_uuid,std::make_shared<mini_gunner>(mob_uuid)});
		mg_map.back()->set_variation(variation);
	}
	void mini_gunner::set_variation(std::string v){
		this->variation = v;
		if(v.compare("sentinel") == 0){

		}
	}
	void mini_gunner::free_mob(uuid_t uuid){
		mg_map.erase(uuid);
	}
	void mini_gunner::wear(int where,std::string_view yaml){
		std::cerr << "mini_gunner wearing: [where:" << where << "]->'" << yaml.data() << "'\n";
		std::tuple<int,std::string> yaml_tuple = mods::util::extract_yaml_info_from_path(yaml);
		if(std::get<0>(yaml_tuple) < 0){
			return;
		}
		if(!mods::util::yaml_file_exists(yaml.data())){
			std::cerr << "[mini_gunner] WARNING: yaml file doesn't exist!->'" << yaml.data() << "'\n";
			return;
		}
		auto obj = create_object(std::get<0>(yaml_tuple),std::get<1>(yaml_tuple));
		this->player_ptr->equip(obj,where);
	}
	void mini_gunner::setup_damage_callbacks(){
		using de = damage_event_t;
		this->player_ptr->register_damage_event_callback(de::YOURE_IN_PEACEFUL_ROOM,[&](de event,damage_info_t dinfo) {
				auto & room = world[this->player_ptr->room()];
				int decision = weighted_direction_decider(this->player_ptr);
				if(decision == -1){
					for(auto dir : room.directions()){
						if(mods::rooms::is_peaceful(room.dir_option[dir]->to_room) == false){
							decision = dir;
							break;
						}
					}
				}
				assert(decision != -1);
				this->player_ptr->cd()->mob_specials.previous_room = this->player_ptr->room();
				char_from_room(this->player_ptr->cd());
				char_to_room(this->player_ptr->cd(),world[this->player_ptr->room()].dir_option[decision]->to_room);
				this->set_heading(decision);
		});
	}
	mini_gunner::mini_gunner(uuid_t mob_uuid){
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		this->player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MINI_GUNNER;
		ch->mob_specials.set_behaviour_tree("mini_gunner_roam");
		MENTOC_MOB_WEARS(MINI_GUNNER);
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
	}
	mini_gunner::~mini_gunner(){
		this->uuid = 0;
	}
	void mini_gunner::init(){
		this->uuid = 0;
		this->loaded = false;
		this->weapon_heat =0;
		this->player_ptr = nullptr;
		this->heading = NORTH;
		this->spray_direction = NORTH;
	};
	void mini_gunner::set_heading(int dir){
		this->heading = this->player_ptr->cd()->mob_specials.heading = dir;
	}
	feedback_t& mini_gunner::spray(int dir){
		mini_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(this->player_ptr,dir);
		this->weapon_heat += 20;
		return this->last_attack;
	}
	void mini_gunner::shout(std::string msg){
		std::cerr << "[stub]shout:'" << msg.data() << "'\n";
	}
	void mini_gunner::set_behaviour_tree(std::string_view name){
		this->player_ptr->cd()->mob_specials.set_behaviour_tree(name);
	}
};
