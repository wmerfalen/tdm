#include "mp-shotgunner.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"

#define __MENTOC_MODS_MOBS_MP_SHOTGUNNER_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_MP_SHOTGUNNER_SHOW_DEBUG_OUTPUT__
#define mps_debug(a) mentoc_prefix_debug("mods::mobs::mp_shotgunner") << a << "\n";
#else
#define mps_debug(a) ;;
#endif
namespace mods::mobs {
	/**! @NEW_BEHAVIOUR_TREE@ !**/
	mpshotgunner_map_t& mpshotgunner_map() {
		static mpshotgunner_map_t m;
		return m;
	}
	namespace mp_shotgunner_btree {
		/**
		 * @brief find the room with the most enemies, and go towards that direction
		 *
		 * @param mob
		 *
		 * @return
		 */
		uint8_t weighted_direction_decider(player_ptr_t& mob) {
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			mods::scan::vec_player_data vpd;
			mods::scan::los_scan_for_players(mob->cd(),depth,&vpd);
			std::map<uint8_t,int> scores;
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
				mps_debug("[stub] should_fire is -1, choose random direction");
			}
			return should_fire;
		}
	};// end namespace mp_shotgunner_btree
	void mp_shotgunner::create(uuid_t mob_uuid, std::string variation) {
		mps_debug("mp_shotgunner create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate mp_shotgunner with: %d",mob_uuid);
			return;
		}
		mods::mobs::mpshotgunner_map().insert({mob_uuid,std::make_shared<mp_shotgunner>(mob_uuid,variation)});
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void mp_shotgunner::enemy_spotted(room_rnum room,uuid_t player) {
		mps_debug("##################################################################################" <<
		          "[mp_shotgunner] enemy spotted:" << room << "\n" <<
		          "##################################################################################");
		this->spray(this->player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void mp_shotgunner::set_variation(std::string v) {
		this->variation = v;
		if(v.compare("sentinel") == 0) {
			auto row = db_get_by_meta("mp_shotgunner_sentinel","mgs_mob_vnum",std::to_string(this->cd()->nr));
			if(row.size() == 0) {
				mps_debug("[mp_shotgunner][set_variation]-> cannot load data from postgres...");
				return;
			}
#define MG_REPORT(A)\
	mps_debug("[[[[ MINI GUNNER SENTINEL DUMP ]]]]" << \
	#A << ": '" << row[0][#A].c_str() << "'" << \
	"[[[[ -- MINI GUNNER SENTINEL DUMP -- ]]]]");

			mps_debug("[status][mp_shotgunner][setting variation data]->");
			MG_REPORT(mgs_face_direction);
			MG_REPORT(mgs_room_vnum);
			MG_REPORT(mgs_mob_vnum);
#undef MG_REPORT

			this->set_heading(mods::globals::dir_int(row[0]["mgs_face_direction"].c_str()[0]));
			char_to_room(this->cd(),real_room(row[0]["mgs_room_vnum"].as<int>()));
		}
	}
	str_map_t mp_shotgunner::report() {
		return {{"foo","todo"}};
	}
	/**
	 * @brief damage_events registered here
	 */
	void mp_shotgunner::setup_damage_callbacks() {
		using de = damage_event_t;
		this->player_ptr->register_damage_event_callback(de::TARGET_DEAD_EVENT,[&](feedback_t feedback,uuid_t player) {
			this->set_behaviour_tree("mp_shotgunner");
		});

		this->player_ptr->register_damage_event_callback(de::YOURE_IN_PEACEFUL_ROOM,[&](feedback_t feedback,uuid_t player) {
			auto& room = world[this->player_ptr->room()];
			int decision = mp_shotgunner_btree::weighted_direction_decider(this->player_ptr);
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
	void mp_shotgunner::init() {
		smart_mob::init();
		m_should_do_max[SHOULD_DO_ROAM] = LOWLY_SECURITY_ROAM_TICK();
		m_should_do_max[SHOULD_DO_RANDOM_TRIVIAL] = LOWLY_SECURITY_RANDOM_TRIVIAL_TICK();
	}

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	mp_shotgunner::mp_shotgunner(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate mp_shotgunner with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		this->player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MP_SHOTGUNNER;
		this->set_behaviour_tree("mp_shotgunner");
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
		bootstrap_equipment();
	}
	/**
	 * @brief spray direction
	 *
	 * @param dir
	 *
	 * @return
	 */
	feedback_t& mp_shotgunner::spray(uint8_t dir) {
		mps_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(this->player_ptr,dir);
		this->weapon_heat += 20; /** TODO: */
		return this->last_attack;
	}
};
#undef mps_debug
