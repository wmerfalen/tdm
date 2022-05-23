#include "orthos-spawn-sentinel.hpp"
#include "../weapons/damage-types.hpp"
#include "../combat-composer/snipe-target.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"

#ifdef m_debug
#undef m_debug
#endif

//#define  __MENTOC_MODS_MOBS_ORTHOS_SPAWN_SENTINEL_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_ORTHOS_SPAWN_SENTINEL_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|m|oss") << a << "\n";
#define mps_debug(a) mentoc_prefix_debug("m|m|oss") << a << "\n";
#define m_error(a) mentoc_prefix_debug("m|m|oss[ERROR]:") << a << "\n";
#else
#define m_debug(a)
#define mps_debug(a)
#define m_error(a)
#endif
namespace mods::mobs {
	/**! @NEW_BEHAVIOUR_TREE@ !**/
	orthos_spawn_sentinel_map_t& orthos_spawn_sentinel_map() {
		static orthos_spawn_sentinel_map_t m;
		return m;
	}
	namespace orthos_spawn_sentinel_btree {
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
	};// end namespace orthos_spawn_sentinel_btree
	void orthos_spawn_sentinel::watch_everywhere() {
		mods::mobs::helpers::watch_multiple(world[this->room()].directions(),this->cd(),10);
	}
	void orthos_spawn_sentinel::attack_mode() {
		for(auto& victim : m_attackers) {
			m_weapon = player_ptr->primary();
			m_debug(green_str("-----------------------------"));
			m_debug(green_str("Orthos spawn sentinel simplified btree"));
			m_debug(green_str("-----------------------------"));

			if(!this->RCT) {
				this->RCT = player()->get_ranged_combat_totals();
			}
			auto depth = this->RCT->max_range;
			int direction = mods::scan::los_find_player(player_ptr,victim,depth);
			if(direction == -1) {
				m_debug("Can't find attacker " << victim->name().c_str());
				continue;
			}
			snipe_target(victim,direction,depth);
		}
	}

	void orthos_spawn_sentinel::create(uuid_t mob_uuid, std::string variation) {
		mps_debug("orthos_spawn_sentinel create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate orthos_spawn_sentinel with: %d",mob_uuid);
			return;
		}
		mods::mobs::orthos_spawn_sentinel_map().insert({mob_uuid,std::make_shared<orthos_spawn_sentinel>(mob_uuid,variation)});
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void orthos_spawn_sentinel::enemy_spotted(room_rnum room,uuid_t player) {
		mps_debug("##################################################################################" <<
		          "[orthos_spawn_sentinel] enemy spotted:" << room << "\n" <<
		          "##################################################################################");
		this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void orthos_spawn_sentinel::set_variation(std::string v) {
		mps_debug("setting variation: '" << v << "'");
		this->variation = v;
		if(v.compare("sentinel") == 0) {
			auto row = db_get_by_meta("orthos_spawn_sentinel_sentinel","mgs_mob_vnum",std::to_string(this->cd()->mob_specials.vnum));
			if(row.size() == 0) {
				mps_debug("[orthos_spawn_sentinel][set_variation]-> cannot load data from postgres...");
				return;
			}
#define MG_REPORT(A)\
	mps_debug("[[[[ MINI GUNNER SENTINEL DUMP ]]]]" << \
	#A << ": '" << row[0][#A].c_str() << "'" << \
	"[[[[ -- MINI GUNNER SENTINEL DUMP -- ]]]]");

			mps_debug("[status][orthos_spawn_sentinel][setting variation data]->");
			MG_REPORT(mgs_face_direction);
			MG_REPORT(mgs_room_vnum);
			MG_REPORT(mgs_mob_vnum);
#undef MG_REPORT

			this->set_heading(mods::globals::dir_int(row[0]["mgs_face_direction"].c_str()[0]));
			char_to_room(this->cd(),real_room(row[0]["mgs_room_vnum"].as<int>()));
		}
	}
	str_map_t orthos_spawn_sentinel::report() {
		return usages();
	}
	str_map_t orthos_spawn_sentinel::usages() {
		str_map_t m;
		m = base_usages();
		std::size_t attackers = std::distance(m_attackers.cbegin(),m_attackers.cend());

		if(attackers) {
			m["attackers"] = std::to_string(attackers);
		}
		return m;

	}
	void dispatch_attacked(const uuid_t& mob, feedback_t feedback) {
		if(has_orthos_spawn_sentinel_ptr(mob)) {
			auto ptr = orthos_spawn_sentinel_ptr(mob);
			ptr->attacked(feedback);
		}
	}

	/**
	 * @brief damage_events registered here
	 */
	void orthos_spawn_sentinel::setup_damage_callbacks() {
#ifdef __MENTOC_SHOW_ORTHOS_SPAWN_SENTINEL_DAMAGE_CALLBACKS__
#define m(A) std::cerr << green_str("[orthos_spawn_sentinel::setup_damage_callbacks]") << A << "\n";
#else
#define m(A)
#endif
		using de = damage_event_t;
		static const std::vector<de> pacify_events = {
			de::TARGET_DEAD_EVENT,
			de::TARGET_IN_PEACEFUL_ROOM_EVENT,
			de::YOURE_IN_PEACEFUL_ROOM,
		};
		player_ptr->register_damage_event_callback(pacify_events,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			m("pacify events");
			set_behaviour_tree("orthos_sentinel_roam");
		});

		static const std::vector<de> move_closer = {
			de::TARGET_IS_OUT_OF_RANGE,
		};

		player_ptr->register_damage_event_callback(move_closer,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			m("move closer");
			this->move_closer_to_target();
		});

		static const std::vector<de> scan_for_attacker = {
			de::ATTACKER_NARROWLY_MISSED_YOU_EVENT,
			de::HIT_BY_RIFLE_ATTACK,
			de::HIT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
			de::COULDNT_FIND_TARGET_EVENT,
		};
		player_ptr->register_damage_event_callback(scan_for_attacker,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			m("scan for attacker");
			set_behaviour_tree("orthos_sentinel_roam");
		});


		static const std::vector<de> desperation_move = {
			de::YOU_ARE_INJURED_EVENT,
			de::YOU_MISSED_YOUR_TARGET_EVENT,
		};

		static const std::vector<de> taunt_if = {
			de::YOU_INFLICTED_INCENDIARY_DAMAGE,
			de::YOU_INFLICTED_RADIOACTIVE_DAMAGE,
			de::YOU_INFLICTED_ANTI_MATTER_DAMAGE,
			de::YOU_INFLICTED_CORROSIVE_DAMAGE,
			de::YOU_INFLICTED_EMP_DAMAGE,
			de::YOU_INFLICTED_EXPLOSIVE_DAMAGE,
			de::YOU_INFLICTED_SHRAPNEL_DAMAGE,
			de::YOU_INFLICTED_CRYOGENIC_DAMAGE,
			de::YOU_INFLICTED_SHOCK_DAMAGE,
			de::YOU_DEALT_HEADSHOT_WITH_RIFLE_ATTACK,
			de::YOU_DEALT_HEADSHOT_WITH_SPRAY_ATTACK,
			de::YOU_DEALT_CRITICAL_RIFLE_ATTACK,
			de::YOU_INFLICTED_MELEE_ATTACK,
			de::YOU_INFLICTED_BLADED_MELEE_ATTACK,
			de::YOU_INFLICTED_BLUNT_MELEE_ATTACK,
			de::YOU_REFLECTED_MUNITIONS_EVENT,
			de::YOU_INJURED_SOMEONE_EVENT,
			de::YOU_INFLICTED_AR_SHRAPNEL,
			de::YOU_INFLICTED_INCENDIARY_AMMO,
			de::YOU_DISORIENTED_SOMEONE_EVENT,
		};

		static const std::vector<de> whine_if = {
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
			de::HIT_BY_CRITICAL_SPRAY_ATTACK,
			de::HIT_BY_CRITICAL_RIFLE_ATTACK,
			de::YOU_GOT_HIT_BY_REFLECTED_MUNITIONS_EVENT,
			de::YOU_GOT_HIT_BY_AR_SHRAPNEL,
			de::YOU_GOT_HIT_BY_INCENDIARY_AMMO,
			de::YOU_ARE_DISORIENTED_EVENT,
			de::HIT_BY_INCENDIARY_DAMAGE,
			de::HIT_BY_RADIOACTIVE_DAMAGE,
			de::HIT_BY_ANTI_MATTER_DAMAGE,
			de::HIT_BY_CORROSIVE_DAMAGE,
			de::HIT_BY_EMP_DAMAGE,
			de::HIT_BY_EXPLOSIVE_DAMAGE,
			de::HIT_BY_SHRAPNEL_DAMAGE,
			de::HIT_BY_CRYOGENIC_DAMAGE,
			de::HIT_BY_SHOCK_DAMAGE,
			de::HIT_BY_RIFLE_ATTACK,
			de::HIT_BY_MELEE_ATTACK,
			de::HIT_BY_BLADED_MELEE_ATTACK,
			de::HIT_BY_BLUNT_MELEE_ATTACK,

			de::HIT_BY_INCENDIARY_DAMAGE,
			de::HIT_BY_RADIOACTIVE_DAMAGE,
			de::HIT_BY_ANTI_MATTER_DAMAGE,
			de::HIT_BY_CORROSIVE_DAMAGE,
			de::HIT_BY_EMP_DAMAGE,
			de::HIT_BY_EXPLOSIVE_DAMAGE,
			de::HIT_BY_SHRAPNEL_DAMAGE,
			de::HIT_BY_CRYOGENIC_DAMAGE,
			de::HIT_BY_SHOCK_DAMAGE,
			de::HIT_BY_BONUS_INNATE_SNIPER_RIFLE_ATTACK,
			de::HIT_BY_SHOTGUN_BLAST,

		};

		static const std::vector<de> upkeep_if = {
			de::OUT_OF_AMMO_EVENT,
			de::NO_PRIMARY_WIELDED_EVENT,
			de::COOLDOWN_IN_EFFECT_EVENT,
			de::COULDNT_FIND_TARGET_EVENT,
		};
		player_ptr->register_damage_event_callback(whine_if,[&](const feedback_t& feedback,const uuid_t& player) {
			player_ptr->hp() = player_ptr->max_hp();
			dispatch_attacked(player,feedback);
		});
		player_ptr->register_damage_event_callback(upkeep_if,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			switch(feedback.damage_event) {
				case de::OUT_OF_AMMO_EVENT:
					mps_debug("DAMN! OUT OF AMMO!");
					player_ptr->primary()->rifle_instance->ammo = 255;
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					mps_debug("No primary wieldded... wtf?");
					break;
				case de::COOLDOWN_IN_EFFECT_EVENT:
					mps_debug("cooldown in effect for primary");
					break;
				case de::COULDNT_FIND_TARGET_EVENT:
					mps_debug("Can't find target");
					break;
				default:
					mps_debug("Weird status. unknown");
					break;
			}
		});

		player_ptr->register_damage_event_callback({de::YOURE_IN_PEACEFUL_ROOM},[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			if(player_ptr->room() >= world.size()) {
				std::cerr << red_str("WARNING> mp shotgunner got invalid room id!: ") << player_ptr->room() << ". Staying put!\n";
				return;
			}
			auto& room = world[player_ptr->room()];
			int decision = orthos_spawn_sentinel_btree::weighted_direction_decider(player_ptr);
			if(decision == -1) {
				for(auto dir : room.directions()) {
					if(room.dir_option[dir] && mods::rooms::is_peaceful(room.dir_option[dir]->to_room) == false) {
						decision = dir;
						break;
					}
				}
			}
			assert(decision != -1);
			this->cd()->mob_specials.previous_room = player_ptr->room();
			char_from_room(this->cd());
			char_to_room(this->cd(),world[this->room()].dir_option[decision]->to_room);
			this->set_heading(decision);
		});
#undef m
	}
	void orthos_spawn_sentinel::init() {
		smart_mob::init();
		m_should_do_max[SHOULD_DO_ROAM] = LOWLY_SECURITY_ROAM_TICK();
		m_should_do_max[SHOULD_DO_RANDOM_TRIVIAL] = LOWLY_SECURITY_RANDOM_TRIVIAL_TICK();
		this->RCT = nullptr;
		m_watching_everywhere = false;
	}

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	orthos_spawn_sentinel::orthos_spawn_sentinel(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate orthos_spawn_sentinel with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::ORTHOS_SPAWN_SENTINEL;
		this->set_behaviour_tree("orthos_sentinel_roam");
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation);
		bootstrap_equipment();
		this->player_ptr->add_damage_nerf(100);
	}
	/**
	 * @brief spray direction
	 *
	 * @param dir
	 *
	 * @return
	 */
	feedback_t& orthos_spawn_sentinel::spray(uint8_t dir) {
		mps_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(player_ptr,dir);
		this->weapon_heat += 20; /** TODO: */
		return this->last_attack;
	}
	void orthos_spawn_sentinel::attacked(const feedback_t& feedback) {
		mps_debug("i've been attacked");
		clear_list_if_count(&m_attackers,30);
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			mps_debug("Saved attacker to memory");
		}
	}
	player_ptr_t orthos_spawn_sentinel::get_next_attacking_priority() {
		if(m_attackers.empty()) {
			return nullptr;
		}
		return m_attackers.front();
	}
	std::pair<bool,std::string> orthos_spawn_sentinel::move_to(const direction_t& dir) {
		auto room_id = player_ptr->room();
		auto opt = world[room_id].dir_option[dir];
		if(opt && opt->to_room <= world.size()) {
			char_from_room(player_ptr->cd());
			char_to_room(player_ptr->cd(),opt->to_room);
			return {true,"moved"};
		}
		return {false,"stayed"};
	}
	void orthos_spawn_sentinel::shotgun_attack_within_range() {
		mps_debug("shotgun_attack_within_range");
		if(!m_weapon) {
			m_weapon = player_ptr->primary();
		}
		m_weapon->rifle_instance->ammo = 255;
		if(m_last_attacker) {
			if(m_last_attacker->position() == POS_DEAD) {
				mps_debug("Our target is dead!");
				m_attackers.remove(m_last_attacker);
				m_last_attacker = get_next_attacking_priority();
			}
		}
		for(auto& attacker : m_attackers) {
			auto results = mods::scan::los_find(player_ptr,attacker);
			if(results.found == false) {
				if(m_attackers_last_direction.has_value()) {
					move_to(m_attackers_last_direction.value());
				}
				continue;
			}
			mps_debug("distance:" << results.distance << ", direction: " << results.direction);
			if(results.distance > m_weapon->rifle()->attributes->max_range) {
				mps_debug("moving closer...");
				move_to(results.direction);
			}
			m_attackers_last_direction = results.direction;

			mods::combat_composer::snipe_target(
			    player_ptr, /* attacker (us) */
			    attacker, 	/* our target */
			    results.direction,
			    results.distance,
			    m_weapon
			);
		}
	}

	void orthos_spawn_sentinel::move_closer_to_target() {
		uint8_t loops = 1;
		if(mods::rand::chance(10)) {
			++loops;
		}
		for(int i =0; i < loops; ++i) {
			auto results = mods::scan::los_find(player_ptr,m_last_attacker);
			if(results.found && results.distance >= m_weapon->rifle()->attributes->max_range) {
				move_to(m_attackers_last_direction.value());
			}
		}
	}
};
#undef mps_debug
