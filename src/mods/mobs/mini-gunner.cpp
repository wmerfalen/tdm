#include "mini-gunner.hpp"
#include <map>
#include <memory>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"

#define __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_MINI_GUNNER_SHOW_DEBUG_OUTPUT__
#define mini_debug(a) mentoc_prefix_debug("mods::mobs::mini_gunner") << a << "\n";
#else
#define mini_debug(a) ;;
#endif
namespace mods::mobs {
	mg_map_t& mg_map(){
		static mg_map_t m;
		return m;
	}
	/**
	 * @brief find the room with the most enemies, and go towards that direction
	 *
	 * @param mob
	 *
	 * @return 
	 */
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
			mini_debug("[stub] should_fire is -1, choose random direction");
		}
		return should_fire;
	}
	
	/**
	 * @brief factory function for mg's
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	void mini_gunner::create(uuid_t mob_uuid,std::string variation){
		mini_debug("mini_gunner create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p){
			log("SYSERR: did not find player to populate mini_gunner with: %d",mob_uuid);
			return;
		}
		mg_map().insert({mob_uuid,std::make_shared<mini_gunner>(mob_uuid,variation)});
	}
	/**
	 * @brief set variation of mg. 
	 *
	 * @param v valid types: "sentinel"
	 */
	void mini_gunner::set_variation(std::string v){
		this->variation = v;
		if(v.compare("sentinel") == 0){
			auto row = db_get_by_meta("mini_gunner_sentinel","mgs_mob_vnum",std::to_string(this->cd()->nr));
			if(row.size() == 0){
				mini_debug("[mini_gunner][set_variation]-> cannot load data from postgres...");
				return;
			}
#define MG_REPORT(A)\
	mini_debug("[[[[ MINI GUNNER SENTINEL DUMP ]]]]" << \
	#A << ": '" << row[0][#A].c_str() << "'" << \
	"[[[[ -- MINI GUNNER SENTINEL DUMP -- ]]]]");

			mini_debug("[status][mini_gunner][setting variation data]->");
			MG_REPORT(mgs_face_direction);
			MG_REPORT(mgs_room_vnum);
			MG_REPORT(mgs_mob_vnum);
#undef MG_REPORT

			this->set_heading(mods::globals::dir_int(row[0]["mgs_face_direction"].c_str()[0]));
			char_to_room(this->cd(),real_room(row[0]["mgs_room_vnum"].as<int>()));
		}
	}
	/**
	 * @brief erase the mg instance from our list of mgs
	 *
	 * @param uuid
	 */
	void mini_gunner::free_mob(uuid_t u){
		mg_map().erase(u);
	}
	/**
	 * @brief wear a piece of eq
	 *
	 * @param where
	 * @param yaml
	 */
	void mini_gunner::wear(int where,std::string_view yaml){
		mini_debug("mini_gunner wearing: [where:" << where << "]->'" << yaml.data() << "'");
		std::tuple<int,std::string> yaml_tuple = mods::util::extract_yaml_info_from_path(yaml);
		if(std::get<0>(yaml_tuple) < 0){
			return;
		}
		if(!mods::util::yaml_file_exists(yaml.data())){
			mini_debug("[mini_gunner] WARNING: yaml file doesn't exist!->'" << yaml.data() << "'");
			return;
		}
		auto obj = create_object(std::get<0>(yaml_tuple),std::get<1>(yaml_tuple));
		this->player_ptr->equip(obj,where);
	}
	/**
	 * @brief damage_events registered here
	 */
	void mini_gunner::setup_damage_callbacks(){
		using de = damage_event_t;
		this->player_ptr->register_damage_event_callback(de::TARGET_DEAD_EVENT,[&](feedback_t feedback,uuid_t player){
			this->set_behaviour_tree("mini_gunner_roam");
		});
				
		this->player_ptr->register_damage_event_callback(de::YOURE_IN_PEACEFUL_ROOM,[&](feedback_t feedback,uuid_t player){
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
				this->cd()->mob_specials.previous_room = this->player_ptr->room();
				char_from_room(this->cd());
				char_to_room(this->cd(),world[this->room()].dir_option[decision]->to_room);
				this->set_heading(decision);
		});
	}
	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	mini_gunner::mini_gunner(uuid_t mob_uuid, std::string variation){
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
		this->set_behaviour_tree("mini_gunner_roam");
		MENTOC_MOB_WEARS(MINI_GUNNER);
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
	}
	mini_gunner::~mini_gunner(){
		this->uuid = 0;
	}
	/**
	 * @brief initialize
	 */
	void mini_gunner::init(){
		this->uuid = 0;
		this->loaded = false;
		this->weapon_heat =0;
		this->player_ptr = nullptr;
		this->heading = NORTH;
		this->spray_direction = NORTH;
	};
	/**
	 * @brief set heading
	 *
	 * @param dir
	 */
	void mini_gunner::set_heading(int dir){
		this->heading = this->cd()->mob_specials.heading = dir;
	}
	/**
	 * @brief spray direction
	 *
	 * @param dir
	 *
	 * @return 
	 */
	feedback_t& mini_gunner::spray(int dir){
		mini_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(this->player_ptr,dir);
		this->weapon_heat += 20; /** TODO: */
		return this->last_attack;
	}
	/**
	 * @brief yell. stub
	 *
	 * @param msg
	 */
	void mini_gunner::shout(std::string_view msg){
		mini_debug("[stub]shout:'" << msg.data() << "'");
		act(CAT("$n shouts '",msg.data(),"'").c_str(), TRUE, this->cd(), 0, 0, TO_ROOM);
	}
	/**
	 * @brief set behaviour tree
	 *
	 * @param name
	 */
	void mini_gunner::set_behaviour_tree(std::string_view name){
		mini_debug("Setting behaviour tree to: '" << name << "'");
		this->cd()->mob_specials.set_behaviour_tree(name);
	}
	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void mini_gunner::enemy_spotted(room_rnum room,uuid_t player){
		mini_debug("##################################################################################" <<
		"[mini_gunner] enemy spotted:" << room << "\n" <<
		"##################################################################################");
		this->spray(this->player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	/**
	 * @brief set hunting uuid_t list
	 *
	 * @param hunting
	 */
	void mini_gunner::set_hunting(const uuidvec_t& hunting){
		this->hunting = hunting;
	}
	/**
	 * @brief get hunting uuid's
	 *
	 * @return 
	 */
	uuidvec_t& mini_gunner::get_hunting(){
		return this->hunting;
	}
	/**
	 * @brief get last seen
	 *
	 * @param player
	 *
	 * @return 
	 */
	tick_t mini_gunner::get_last_seen(uuid_t player){
		return this->last_seen[player];
	}
	/**
	 * @brief get the last tiem in ticks when we saw that player
	 *
	 * @param player
	 *
	 * @return 
	 */
	tick_t mini_gunner::get_last_seen_diff(uuid_t player){
		auto t = this->last_seen[player];
		return t - CURRENT_TICK();
	}
	/**
	 * @brief same as mob forget
	 *
	 * @param player
	 */
	void mini_gunner::forget(uuid_t player){
		this->last_seen.erase(player);
		mods::util::vector_erase(this->hunting,player);
	}
	/**
	 * @brief char data ptr
	 *
	 * @return 
	 */
	char_data* mini_gunner::cd(){
		return this->player_ptr->cd();
	}
	/**
	 * @brief which room
	 *
	 * @return 
	 */
	room_rnum mini_gunner::room(){
		return IN_ROOM(this->cd());
	}
	void mini_gunner::watch_directions(vec_t<uint8_t> directions){
		mods::mobs::helpers::watch_multiple(directions,this->cd(),MINI_GUNNER_SCAN_DEPTH());
	}
	void mini_gunner::watch_heading(){
		this->watch(this->heading);
	}
	void mini_gunner::save_targets(vec_t<uuid_t>& t){
		this->targeting = t;
	}
	void mini_gunner::watch_nothing(){
		mods::mobs::helpers::clear_watching(this->uuid);
	}
	/**
	 * @brief watch acertain dir
	 *
	 * @param direction
	 */
	void mini_gunner::watch(uint8_t direction){
		this->watching = direction;
		mini_debug("[mini_gunner] watching:" << dirstr(direction) << "uuid:" << this->uuid);
		mods::mobs::helpers::watch(direction,this->cd(),MINI_GUNNER_SCAN_DEPTH());
	}
	obj_ptr_t mini_gunner::primary(){
		return this->player_ptr->primary();
	}
};
