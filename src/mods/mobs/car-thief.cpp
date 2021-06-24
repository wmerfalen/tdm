#include "car-thief.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../calc-visibility.hpp"

#ifdef  __MENTOC_MODS_MOBS_car_thief_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|m|mps") << a << "\n";
#else
#define m_debug(a) ;;
#endif
namespace mods::mobs {
	/**! @NEW_BEHAVIOUR_TREE@ !**/
	car_thief_map_t& car_thief_map() {
		static car_thief_map_t m;
		return m;
	}
	namespace car_thief_btree {
		/**
		 * @brief find the room with the most enemies, and go towards that direction
		 *
		 * @param mob
		 * @param victim
		 *
		 * @return
		 */
		uint8_t weighted_direction_decider(player_ptr_t& mob,player_ptr_t victim) {
			int depth = CAR_THIEF_SCAN_DEPTH();
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
				if(victim && victim->uuid() == v.uuid) {
					scores[v.direction] += 2;
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
				m_debug("[stub] should_fire is -1, choose random direction");
			}
			return should_fire;
		}
	};// end namespace car_thief_btree
	void car_thief::create(uuid_t mob_uuid, std::string variation) {
		m_debug("car_thief create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate car_thief with: %d",mob_uuid);
			return;
		}
		mods::mobs::car_thief_map().insert({mob_uuid,std::make_shared<car_thief>(mob_uuid,variation)});
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void car_thief::enemy_spotted(room_rnum room,uuid_t player) {
		m_debug("##################################################################################" <<
		        "[car_thief] enemy spotted:" << room << "\n" <<
		        "##################################################################################");
		this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void car_thief::set_variation(std::string v) {
		//
	}
	str_map_t car_thief::report() {
		return {{"foo","todo"}};
	}
	/**
	 * @brief damage_events registered here
	 */
	void car_thief::setup_damage_callbacks() {
#define m(A) std::cerr << green_str("[car_thief::setup_damage_callbacks]") << A << "\n";
		using de = damage_event_t;
		static const std::vector<de> pacify_events = {
			de::TARGET_DEAD_EVENT,
			de::TARGET_IN_PEACEFUL_ROOM_EVENT,
			de::YOURE_IN_PEACEFUL_ROOM,
		};
		player_ptr->register_damage_event_callback(pacify_events,[&](const feedback_t& feedback,const uuid_t& player) {
			m("pacify events");
			set_behaviour_tree("car_thief_roam");
		});

		static const std::vector<de> enrage_if = {
			de::ATTACKER_NARROWLY_MISSED_YOU_EVENT,
			de::HIT_BY_RIFLE_ATTACK,
			de::HIT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_SPRAY_ATTACK,
			de::YOU_GOT_HEADSHOT_BY_RIFLE_ATTACK,
		};
		player_ptr->register_damage_event_callback(enrage_if,[&](const feedback_t& feedback,const uuid_t& player) {
			auto attacker = ptr_by_uuid(feedback.attacker);
			auto weapon = player_ptr->primary();

			if(player_ptr->room() != attacker->room()) {
				if(mods::calc_visibility::roll_victim_spots_attacker(player_ptr,attacker,feedback)) {
					m_debug("car thief (as victim) spots attacker!");
					auto decision = feedback.from_direction;
					move_to(decision);
					set_heading(decision);
				} else {
					m_debug("car thief (as victim) ***DOES NOT*** spot attacker!");
				}
			}
			if(player_ptr->room() == attacker->room()) {
				mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,weapon,attacker);
			}
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
		};

		static const std::vector<de> upkeep_if = {
			de::OUT_OF_AMMO_EVENT,
			de::NO_PRIMARY_WIELDED_EVENT,
			de::COOLDOWN_IN_EFFECT_EVENT,
			de::COULDNT_FIND_TARGET_EVENT,
		};
		player_ptr->register_damage_event_callback(upkeep_if,[&](const feedback_t& feedback,const uuid_t& player) {
			switch(feedback.damage_event) {
				case de::OUT_OF_AMMO_EVENT:
					m_debug("DAMN! OUT OF AMMO!");
					player_ptr->primary()->rifle_instance->ammo = 255;
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					m_debug("No primary wieldded... wtf?");
					break;
				case de::COOLDOWN_IN_EFFECT_EVENT:
					m_debug("cooldown in effect for primary");
					break;
				case de::COULDNT_FIND_TARGET_EVENT:
					m_debug("Can't find target");
					break;
				default:
					m_debug("Weird status. unknown");
					break;
			}
		});

		player_ptr->register_damage_event_callback({de::YOURE_IN_PEACEFUL_ROOM},[&](const feedback_t& feedback,const uuid_t& player) {
			if(player_ptr->room() >= world.size()) {
				return;
			}
			auto& room = world[player_ptr->room()];
			int decision = car_thief_btree::weighted_direction_decider(player_ptr,nullptr);
			if(decision == -1) {
				for(auto dir : room.directions()) {
					if(room.dir_option[dir] && mods::rooms::is_peaceful(room.dir_option[dir]->to_room) == false) {
						decision = dir;
						break;
					}
				}
			}
			this->cd()->mob_specials.previous_room = player_ptr->room();
			move_to(decision);
			this->set_heading(decision);
		});
#undef m
	}
	void car_thief::init() {
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
	car_thief::car_thief(uuid_t mob_uuid, std::string variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate car_thief with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::CAR_THIEF;
		this->set_behaviour_tree("car_thief_roam");
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
	feedback_t& car_thief::spray(uint8_t dir) {
		m_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(player_ptr,dir);
		this->weapon_heat += 20; /** TODO: */
		return this->last_attack;
	}
	void car_thief::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
		}
	}
	player_ptr_t car_thief::get_next_attacking_priority() {
		return m_attackers.front();
	}
	std::pair<bool,std::string> car_thief::move_to(const direction_t& dir) {
		auto room_id = player_ptr->room();
		auto opt = world[room_id].dir_option[dir];
		if(opt && opt->to_room <= world.size()) {
			perform_move(player_ptr->cd(),dir,0);
			return {true,"moved"};
		}
		return {false,"stayed"};
	}
	void car_thief::melee_attack_within_range() {
		m_debug("melee_attack_within_range");
		if(!m_weapon) {
			m_weapon = player_ptr->primary();
		}
		if(m_last_attacker) {
			if(m_last_attacker->position() == POS_DEAD) {
				m_debug("Our target is dead!");
				m_attackers.remove(m_last_attacker);
				m_last_attacker = get_next_attacking_priority();
			}
		}
		for(const auto& attacker : m_attackers) {
			auto results = mods::scan::los_find(player_ptr,attacker);
			if(results.found == false) {
				if(m_attackers_last_direction.has_value()) {
					move_to(m_attackers_last_direction.value());
				}
				continue;
			}
			m_debug("distance:" << results.distance << ", direction: " << results.direction);
			if(results.distance > m_weapon->rifle()->attributes->max_range) {
				move_to(results.direction);
			}
			m_attackers_last_direction = results.direction;

			if(attacker->room() == player_ptr->room()) {
				auto feedback = mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,player_ptr->primary(),attacker);
				if(feedback.hits == 0 || feedback.damage == 0) {
					continue;
				}
			}
		}
	}

	void car_thief::move_closer_to_target() {
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
#undef m_debug