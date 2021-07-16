#include "chaotic-meth-addict.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"

#define  __MENTOC_MODS_MOBS_chaotic_meth_addict_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_chaotic_meth_addict_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|m|cma") << a << "\n";
#else
#define m_debug(a) ;;
#endif
#define cmem(a) mentoc_prefix_debug("[chaotic_meth_addict][memory_footprint]") << a << "\n";
namespace mods::mobs {
	namespace chaotic_meth_addict_btree {
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
	};// end namespace chaotic_meth_addict_btree
	void chaotic_meth_addict::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug("chaotic_meth_addict create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate chaotic_meth_addict with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<chaotic_meth_addict>(mob_uuid,targets.data());
		g->btree_roam();
		mods::mobs::chaotic_meth_addict_list().push_front(g);
	}
	void chaotic_meth_addict::perform_random_act() {
		act_to_room(m_random_acts[rand_number(0,m_random_acts.size()-1)]);
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void chaotic_meth_addict::enemy_spotted(room_rnum room,uuid_t player) {
		m_debug("##################################################################################" <<
		        "[chaotic_meth_addict] enemy spotted:" << room << "\n" <<
		        "##################################################################################");
		//this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void chaotic_meth_addict::set_variation(const std::string& v) {
		for(const auto& type : EXPLODE(v,' ')) {
			std::cerr << green_str("chaotic_meth_addict::variation:") << type << "\n";
		}
	}
	str_map_t chaotic_meth_addict::report() {
		return {{"foo","todo"}};
	}
	void chaotic_meth_addict::set_behavior_tree_directly(const chaotic_meth_addict::btree_t& t) {
		m_debug("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}
	bool chaotic_meth_addict::has_tree() {
		return cd()->mob_specials.behaviour_tree != chaotic_meth_addict::btree_t::CMA_NONE;
	}
	chaotic_meth_addict::btree_t chaotic_meth_addict::get_tree() {
		return (btree_t)cd()->mob_specials.behaviour_tree;
	}
	void chaotic_meth_addict::btree_none() {
		set_behaviour_tree_directly(chaotic_meth_addict::btree_t::CMA_NONE);
	}
	void chaotic_meth_addict::btree_roam() {
		set_behavior_tree_directly(chaotic_meth_addict::btree_t::CMA_ROAM);

	}
	void chaotic_meth_addict::btree_hostile() {
		set_behavior_tree_directly(chaotic_meth_addict::btree_t::CMA_HOSTILE);

	}
	/**
	 * @brief damage_events registered here
	 */
	void chaotic_meth_addict::setup_damage_callbacks() {
		using de = damage_event_t;
		static const std::vector<de> pacify_events = {
			de::TARGET_DEAD_EVENT,
			de::TARGET_IN_PEACEFUL_ROOM_EVENT,
			de::YOURE_IN_PEACEFUL_ROOM,
		};
		player_ptr->register_damage_event_callback(pacify_events,[&](const feedback_t& feedback,const uuid_t& player) {
			m_debug("pacify events");
			btree_roam();
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
					m_debug("generic thief (as victim) spots attacker!");
					auto decision = feedback.from_direction;
					move_to(decision);
					set_heading(decision);
				} else {
					m_debug("generic thief (as victim) ***DOES NOT*** spot attacker!");
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
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					m_debug("No primary wieldded... wtf?");
					m_weapon = player_ptr->primary();
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
			int decision = chaotic_meth_addict_btree::weighted_direction_decider(player_ptr,nullptr);
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
	}
	bool chaotic_meth_addict::is_rival(player_ptr_t& player) {
		return false;
	}
	void chaotic_meth_addict::door_entry_event(player_ptr_t& player) {
		if(player->is_npc()) {
			if(is_rival(player)) {
				btree_roam();
				//TODO: attack_with_melee(player);
				//player->sendln(CAT("I am:",uuid," and I'm Watching you"));
			}
		}
	}
	void chaotic_meth_addict::init() {
		smart_mob::init();
		m_should_do_max[SHOULD_DO_ROAM] = LOWLY_SECURITY_ROAM_TICK();
		m_should_do_max[SHOULD_DO_RANDOM_TRIVIAL] = LOWLY_SECURITY_RANDOM_TRIVIAL_TICK();
		m_found_item = 0;
		m_last_attacker = nullptr;
		m_attackers.clear();
		m_weapon = nullptr;
		m_attackers_last_direction = std::nullopt;
		m_scanned_items.clear();
		m_remembered_items.clear();
		for(const auto& msg : EXPLODE(CHAOTIC_METH_ADDICT_PSV_RANDOM_ACT(),'|')) {
			m_random_acts.emplace_back(msg);
		}
		cmem("m_random_acts:" << m_random_acts.size());
	};

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	chaotic_meth_addict::chaotic_meth_addict(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate chaotic_meth_addict with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::CHAOTIC_METH_ADDICT;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		bootstrap_equipment();
		m_weapon = player()->primary();
	}
	void chaotic_meth_addict::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			cmem("{m_attackers.size}:" << std::distance(m_attackers.cbegin(),m_attackers.cend()));
		}
	}
	player_ptr_t chaotic_meth_addict::get_next_attacking_priority() {
		return m_attackers.front();
	}
	void chaotic_meth_addict::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
			mods::weapons::damage_types::melee_damage_with_feedback(player(),m_weapon,attacker);
		}
	}
	void chaotic_meth_addict::melee_attack_within_range() {
		m_debug("melee_attack_within_range");
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
			m_attackers_last_direction = results.direction;

			if(attacker->room() == player_ptr->room()) {
				auto feedback = mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,m_weapon,attacker);
				if(feedback.hits == 0 || feedback.damage == 0) {
					continue;
				}
			}
		}
	}

	void chaotic_meth_addict::attack(player_ptr_t& player) {
		m_last_attacker = player;
		melee_attack_within_range();
	}
	void chaotic_meth_addict::move_closer_to_target() {
		uint8_t loops = 1;
		if(mods::rand::chance(CAR_THIEF_EXTRA_LOOP_CHANCE())) {
			++loops;
		}
		for(int i =0; i < loops; ++i) {
			auto results = mods::scan::los_find(player_ptr,m_last_attacker);
			if(results.found && results.distance > 0) {
				move_to(m_attackers_last_direction.value());
			}
		}
	}
	int8_t chaotic_meth_addict::determine_heading_from_found_victims() {
		std::map<direction_t,std::size_t> weights;
		for(const auto& spot : m_scanned_items) {
			++weights[spot.direction];
		}
		int direction = -1, count = 0;
		for(const auto& pair : weights) {
			if(pair.second > count) {
				direction = pair.first;
				count = pair.second;
			}
		}
		return direction;
	}
	bool chaotic_meth_addict::has_found_item() {
		return m_found_item;
	}
	void chaotic_meth_addict::set_found_item(bool status) {
		m_found_item = status;
	}
	void chaotic_meth_addict::found_witness(const mods::scan::vec_player_data_element& data) {
		m_hostiles.emplace_front(data);
		cmem("m_hostiles:" << std::distance(m_hostiles.cbegin(),m_hostiles.cend()));
	}
	void chaotic_meth_addict::clear_scanned_items() {
		m_scanned_items.clear();
	}
	void chaotic_meth_addict::remember_item(const mods::scan::vec_player_data_element& data) {
		m_remembered_items.push_front(data.uuid);
		cmem("m_remembered_items:" << std::distance(m_remembered_items.cbegin(),m_remembered_items.cend()));
	}
	const chaotic_meth_addict::uuidlist_t& chaotic_meth_addict::get_remembered_items() const {
		return m_remembered_items;
	}
	void chaotic_meth_addict::found_item(mods::scan::vec_player_data_element const& item) {
		m_scanned_items.emplace_back(item);
		cmem("m_scanned_items:" << m_scanned_items.size());
	}
	std::forward_list<std::shared_ptr<chaotic_meth_addict>>& chaotic_meth_addict_list() {
		static std::forward_list<std::shared_ptr<chaotic_meth_addict>> s;
		return s;
	}
	uint8_t chaotic_meth_addict::scan_depth() const {
		return CHAOTIC_METH_ADDICT_SCAN_DEPTH();
	}
	player_ptr_t chaotic_meth_addict::spawn_near_someone() {
		player_ptr_t who = nullptr;
		mods::loops::foreach_player([&](auto player) -> bool {
			if(rand_number(1,10) > rand_number(1,10)) {
				who = player;
				player->sendln("Beware! A meth addict approaches!");
				char_from_room(cd());
				char_to_room(cd(),player->room());
				return false;
			}
			return true;
		});
		return who;
	}
	bool chaotic_meth_addict::attack_anyone_in_same_room() {
		for(auto& victim : mods::globals::get_room_list(room())) {
			if(victim->is(cd())) {
				continue;
			}
			auto feedback = mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,m_weapon,victim);
			if(feedback.hits || feedback.damage) {
				return true;
			}
		}
		return false;
	}
	void chaotic_meth_addict::clear_state() {
		m_scanned_items.clear();
		m_hostiles.clear();
		m_attackers.clear();
		m_remembered_items.clear();
	}
};
#undef m_debug
#undef cmem
