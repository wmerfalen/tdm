#include "lowly-security.hpp"
#include <map>
#include <memory>
#include "../weapons/damage-types.hpp"
#include "../damage-event.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"

#define __MENTOC_MODS_MOBS_LOWLY_SECURITY_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_LOWLY_SECURITY_SHOW_DEBUG_OUTPUT__
#define mini_debug(a) mentoc_prefix_debug("mods::mobs::lowly_security") << a << "\n";
#else
#define mini_debug(a) ;;
#endif
namespace mods::mobs {
	lowlysec_map_t& lowlysec_map() {
		static lowlysec_map_t m;
		return m;
	}
	namespace lowly_sec {
		/**
		 * @brief find the room with the most enemies, and go towards that direction
		 *
		 * @param mob
		 *
		 * @return
		 */
		int weighted_direction_decider(player_ptr_t& mob) {
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			mods::scan::vec_player_data vpd;
			mods::scan::los_scan_for_players(mob->cd(),depth,&vpd);
			std::map<int,int> scores;
			for(auto v : vpd) {
				if(!ptr_by_uuid(v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				++scores[v.direction];
			}
			int should_fire = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			/** TODO when was the last time this mob saw a target? if should_fire is -1, go there */
			if(should_fire == -1) {
				/** FIXME */
				mini_debug("[stub] should_fire is -1, choose random direction");
			}
			return should_fire;
		}
	};

	/**
	 * @brief factory function for mg's
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	void lowly_security::create(uuid_t mob_uuid,std::string variation) {
		mini_debug("lowly_security create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate lowly_security with: %d",mob_uuid);
			return;
		}
		lowlysec_map().insert({mob_uuid,std::make_shared<lowly_security>(mob_uuid,variation)});
	}
	/**
	 * @brief set variation of mg.
	 *
	 * @param v valid types: "sentinel"
	 */
	void lowly_security::set_variation(std::string v) {
		this->variation = v;
		if(v.compare("sentinel") == 0) {
			auto row = db_get_by_meta("lowly_security_sentinel","mgs_mob_vnum",std::to_string(this->cd()->nr));
			if(row.size() == 0) {
				mini_debug("[lowly_security][set_variation]-> cannot load data from postgres...");
				return;
			}
#define MG_REPORT(A)\
	mini_debug("[[[[ MINI GUNNER SENTINEL DUMP ]]]]" << \
	#A << ": '" << row[0][#A].c_str() << "'" << \
	"[[[[ -- MINI GUNNER SENTINEL DUMP -- ]]]]");

			mini_debug("[status][lowly_security][setting variation data]->");
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
	void lowly_security::free_mob(uuid_t u) {
		lowlysec_map().erase(u);
	}
	/**
	 * @brief wear a piece of eq
	 *
	 * @param where
	 * @param yaml
	 */
	void lowly_security::wear(int where,std::string_view yaml) {
		mini_debug("lowly_security wearing: [where:" << where << "]->'" << yaml.data() << "'");
		std::tuple<int,std::string> yaml_tuple = mods::util::extract_yaml_info_from_path(yaml);
		if(std::get<0>(yaml_tuple) < 0) {
			return;
		}
		if(!mods::util::yaml_file_exists(yaml.data())) {
			mini_debug("[lowly_security] WARNING: yaml file doesn't exist!->'" << yaml.data() << "'");
			return;
		}
		auto obj = create_object(std::get<0>(yaml_tuple),std::get<1>(yaml_tuple));
		this->player_ptr->equip(obj,where);
	}
	/**
	 * @brief damage_events registered here
	 */
	void lowly_security::setup_damage_callbacks() {
		using de = damage_event_t;
		this->player_ptr->register_damage_event_callback(de::TARGET_DEAD_EVENT,[&](feedback_t feedback,uuid_t player) {
			this->set_behaviour_tree("lowly_security_roam");
		});

		this->player_ptr->register_damage_event_callback(de::YOURE_IN_PEACEFUL_ROOM,[&](feedback_t feedback,uuid_t player) {
			auto& room = world[this->player_ptr->room()];
			int decision = lowly_sec::weighted_direction_decider(this->player_ptr);
			if(decision == -1) {
				for(auto dir : room.directions()) {
					if(mods::rooms::is_peaceful(room.dir_option[dir]->to_room) == false) {
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
	lowly_security::lowly_security(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate lowly_security with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		this->player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::LOWLY_SECURITY;
		this->set_behaviour_tree("lowly_security_roam");
		//MENTOC_MOB_WEARS(MINI_GUNNER);
		/** TODO: wear all equipment as per the list setup in meqbuild command */
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
	}
	lowly_security::~lowly_security() {
		this->uuid = 0;
	}
	/**
	 * @brief initialize
	 */
	void lowly_security::init() {
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
	void lowly_security::set_heading(int dir) {
		this->heading = this->cd()->mob_specials.heading = dir;
	}
	/**
	 * @brief spray direction
	 *
	 * @param dir
	 *
	 * @return
	 */
	feedback_t& lowly_security::spray(int dir) {
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
	void lowly_security::shout(std::string_view msg) {
		mini_debug("[stub]shout:'" << msg.data() << "'");
		act(CAT("$n shouts '",msg.data(),"'").c_str(), TRUE, this->cd(), 0, 0, TO_ROOM);
	}
	/**
	 * @brief set behaviour tree
	 *
	 * @param name
	 */
	void lowly_security::set_behaviour_tree(std::string_view name) {
		mini_debug("Setting behaviour tree to: '" << name << "'");
		this->cd()->mob_specials.set_behaviour_tree(name);
	}
	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void lowly_security::enemy_spotted(room_rnum room,uuid_t player) {
		mini_debug("##################################################################################" <<
		           "[lowly_security] enemy spotted:" << room << "\n" <<
		           "##################################################################################");
		this->spray(this->player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	/**
	 * @brief set hunting uuid_t list
	 *
	 * @param hunting
	 */
	void lowly_security::set_hunting(const uuidvec_t& hunting) {
		this->hunting = hunting;
	}
	/**
	 * @brief get hunting uuid's
	 *
	 * @return
	 */
	uuidvec_t& lowly_security::get_hunting() {
		return this->hunting;
	}
	/**
	 * @brief get last seen
	 *
	 * @param player
	 *
	 * @return
	 */
	tick_t lowly_security::get_last_seen(uuid_t player) {
		return this->last_seen[player];
	}
	/**
	 * @brief get the last tiem in ticks when we saw that player
	 *
	 * @param player
	 *
	 * @return
	 */
	tick_t lowly_security::get_last_seen_diff(uuid_t player) {
		auto t = this->last_seen[player];
		return t - CURRENT_TICK();
	}
	/**
	 * @brief same as mob forget
	 *
	 * @param player
	 */
	void lowly_security::forget(uuid_t player) {
		this->last_seen.erase(player);
		mods::util::vector_erase(this->hunting,player);
	}
	/**
	 * @brief char data ptr
	 *
	 * @return
	 */
	char_data* lowly_security::cd() {
		return this->player_ptr->cd();
	}
	/**
	 * @brief which room
	 *
	 * @return
	 */
	room_rnum lowly_security::room() {
		return IN_ROOM(this->cd());
	}
	void lowly_security::watch_directions(vec_t<uint8_t> directions) {
		mods::mobs::helpers::watch_multiple(directions,this->cd(),LOWLY_SECURITY_SCAN_DEPTH());
	}
	void lowly_security::watch_heading() {
		this->watch(this->heading);
	}
	void lowly_security::save_targets(vec_t<uuid_t>& t) {
		this->targeting = t;
	}
	void lowly_security::watch_nothing() {
		mods::mobs::helpers::clear_watching(this->uuid);
	}
	/**
	 * @brief watch acertain dir
	 *
	 * @param direction
	 */
	void lowly_security::watch(uint8_t direction) {
		this->watching = direction;
		mini_debug("[lowly_security] watching:" << dirstr(direction) << "uuid:" << this->uuid);
		mods::mobs::helpers::watch(direction,this->cd(),LOWLY_SECURITY_SCAN_DEPTH());
	}
	obj_ptr_t lowly_security::primary() {
		return this->player_ptr->primary();
	}
};
