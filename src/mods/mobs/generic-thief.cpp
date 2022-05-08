#include "generic-thief.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../calc-visibility.hpp"
#include "../object-utils.hpp"
#include "../combat-composer/includes.hpp"
#include "../melee/main.hpp"

#ifdef m_debug
#undef m_debug
#endif

#define  __MENTOC_MODS_MOBS_generic_thief_SHOW_TODO_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_generic_thief_SHOW_TODO_OUTPUT__
#define m_todo(a) mentoc_prefix_debug("m|m|generic-thief") << yellow_str("TODO:") << a << "\n"
#else
#define m_todo(a) ;;
#endif

//#define  __MENTOC_MODS_MOBS_generic_thief_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_generic_thief_SHOW_DEBUG_OUTPUT__
//#define m_debug(a) if(this->is_fighting()){ mentoc_prefix_debug("m|m|mps") << a << "\n"; }
#define m_debug(a) mentoc_prefix_debug("m|m|generic-thief") << a << "\n"
#else
#define m_debug(a) ;;
#endif

#ifdef __MENTOC_MODS_MOBS_generic_thief_MUTE_ERROR_OUTPUT__
#define m_error(a) ;;
#else
#define m_error(a) mentoc_prefix_debug("m|m|generic-thief") << red_str("ERROR:") << a << "\n";
#endif

namespace mods::mobs {
	/**! @NEW_BEHAVIOUR_TREE@ !**/
#if 0
	generic_thief_map_t& generic_thief_map() {
		static generic_thief_map_t m;
		return m;
	}
#endif

	namespace generic_thief_btree {
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
				//m_debug("[stub] should_fire is -1, choose random direction");
			}
			return should_fire;
		}
	};// end namespace generic_thief_btree


	/**
	 * Grab the targets string and convert them to an in-memory list.
	 */
	std::forward_list<generic_thief::target_t>& generic_thief::parse_targets(std::string_view targets) {
		for(const auto& item : EXPLODE(targets.data(),' ')) {
			auto target = str_to_target(item);
			if(target == target_t::NONE) {
				continue;
			}
			m_targets.emplace_front(target);
		}
		return m_targets;
	}
	/**
	 * Fetches the parsed targets
	 */
	std::forward_list<generic_thief::target_t>& generic_thief::get_targets()  {
		return m_targets;
	}



	/***
	 * Entry point that essentially is an init function of sorts.
	 */
	void generic_thief::create(const uuid_t& mob_uuid, std::string_view targets) {
		//m_debug("generic_thief create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate generic_thief with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<generic_thief>(mob_uuid,targets.data());
		g->btree_roam();
		mods::mobs::generic_thief_list().emplace_back(g);
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void generic_thief::enemy_spotted(room_rnum room,uuid_t player) {
		//m_debug("##################################################################################" <<
		//        "[generic_thief] enemy spotted:" << room << "\n" <<
		//        "##################################################################################");
		//this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void generic_thief::set_variation(const std::string& v) {
		for(const auto& type : EXPLODE(v,' ')) {
			m_todo("Need to set variation for generic_thief [not implemented]: '" << type.c_str() << "'");
			m_debug(green_str("generic_thief::variation:") << type);
		}
	}



	/**
	 * Unfinished idea with very little behind it.
	 */
	bool generic_thief::is_rival(player_ptr_t& player) {
		return false;
	}

	str_map_t generic_thief::report() {
		return usages();
	}
	str_map_t generic_thief::usages() {
		str_map_t m;
		m = base_usages();
		std::size_t targets_size = std::distance(m_targets.cbegin(),m_targets.cend()),
		            rem_size = std::distance(m_remembered_items.cbegin(),m_remembered_items.cend()),
		            attackers = std::distance(m_attackers.cbegin(),m_attackers.cend()),
		            hostiles = std::distance(m_hostiles.cbegin(),m_hostiles.cend());
		if(targets_size) {
			m["targets"] = std::to_string(targets_size);
		}
		if(rem_size) {
			m["remembered_items"] = std::to_string(rem_size);
		}
		if(attackers) {
			m["attackers"] = std::to_string(attackers);
		}
		if(hostiles) {
			m["hostiles"] = std::to_string(hostiles);
		}
		if(m_scanned_items.size()) {
			m["scanned_items"] = std::to_string(m_scanned_items.size());
		}
		return m;
	}
	/**
	 * =====================================================================================
	 * === UTILITY FUNCTIONS
	 * =====================================================================================
	 */
	void generic_thief::set_behavior_tree_directly(const generic_thief::btree_t& t) {
		//m_debug("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}
	bool generic_thief::has_tree() {
		return cd()->mob_specials.behaviour_tree != generic_thief::btree_t::GT_NONE;
	}
	generic_thief::btree_t generic_thief::get_tree() {
		return (btree_t)cd()->mob_specials.behaviour_tree;
	}
	void generic_thief::btree_none() {
		set_behaviour_tree_directly(generic_thief::btree_t::GT_NONE);
	}
	void generic_thief::btree_roam() {
		set_behavior_tree_directly(generic_thief::btree_t::GT_ROAM);

	}
	void generic_thief::btree_hostile() {
		set_behavior_tree_directly(generic_thief::btree_t::GT_HOSTILE);

	}
	void generic_thief::btree_witness_hunting() {
		set_behavior_tree_directly(generic_thief::btree_t::GT_WITNESS_HUNTING);

	}
	void generic_thief::btree_wimpy() {
		set_behavior_tree_directly(generic_thief::btree_t::GT_WIMPY);

	}
	void generic_thief::btree_attempt_thievery() {
		set_behavior_tree_directly(generic_thief::btree_t::GT_ATTEMPT_THIEVERY);
	}




	/**
	 * @brief damage_events registered here
	 */
	void generic_thief::setup_damage_callbacks() {
		using de = damage_event_t;
		static const std::vector<de> pacify_events = {
			de::TARGET_DEAD_EVENT,
		};
		player_ptr->register_damage_event_callback(pacify_events,[&](const feedback_t& feedback,const uuid_t& player) {
			if(!ptr_by_uuid(player)) {
				m_error(type().data() << ":" << red_str("USE AFTER FREE"));
				return;
			}
			//m_debug("pacify events");
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
			if(!ptr_by_uuid(player)) {
				m_error(type().data() << ":" << red_str("USE AFTER FREE"));
				return;
			}
			auto attacker = ptr_by_uuid(feedback.attacker);
			if(!attacker) {
				return;
			}

			this->attacked(feedback);

			auto weapon = player_ptr->primary();
			if(!weapon) {
				btree_hostile();
				return;
			}
			/**
			 * If we have a melee weapon
			 */
			if(mods::object_utils::is_melee(weapon)) {
				if(player_ptr->room() != m_last_attacker->room()) {
					//m_debug("Moving toward sniiper..." << dirstr(feedback.from_direction));
					move_to(feedback.from_direction);
				}
				if(player_ptr->room() == attacker->room()) {
					//m_debug("attacking...");
					mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,weapon,attacker);
					hit(player_ptr->cd(),attacker->cd(),0);
				}
			} else if(mods::object_utils::is_rifle(weapon)) {

			}
			btree_hostile();
		});

		static const std::vector<de> desperation_move = {
			de::YOU_ARE_INJURED_EVENT,
			de::YOU_MISSED_YOUR_TARGET_EVENT,
		};

		static const std::vector<de> steal_money_if = {
			de::YOU_INJURED_SOMEONE_EVENT,
			de::YOU_DISORIENTED_SOMEONE_EVENT,
		};

		static const std::vector<de> panic_if = {
			de::HIT_BY_CRITICAL_SPRAY_ATTACK,
			de::HIT_BY_CRITICAL_RIFLE_ATTACK,
			de::YOU_ARE_DISORIENTED_EVENT,
			de::YOU_ARE_INJURED_EVENT,
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
					//m_debug("DAMN! OUT OF AMMO!");
					player_ptr->primary()->rifle_instance->ammo = 255;
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					m_debug("No primary wieldded... wtf? (" << player_ptr->name().c_str() << ")");
					m_error("No primary wielded..." << player_ptr->name().c_str() << "[uuid:" << player_ptr->uuid() << "]");
					break;
				case de::COOLDOWN_IN_EFFECT_EVENT:
					//m_debug("cooldown in effect for primary");
					break;
				case de::COULDNT_FIND_TARGET_EVENT:
					//m_debug("Can't find target");
					break;
				default:
					m_debug("Weird status. unknown");
					m_error("Weird status. unknown" << player_ptr->name().c_str() << "[uuid:" << player_ptr->uuid() << "]");
					break;
			}
		});

		player_ptr->register_damage_event_callback({de::YOURE_IN_PEACEFUL_ROOM},[&](const feedback_t& feedback,const uuid_t& player) {
			if(player_ptr->room() >= world.size()) {
				m_error("Room out of bounds" << player_ptr->name().c_str() << "[uuid:" << player_ptr->uuid() << "]");
				return;
			}
			auto& room = world[player_ptr->room()];
			int decision = generic_thief_btree::weighted_direction_decider(player_ptr,nullptr);
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
	std::string generic_thief::summarize(bool regen) {
		static std::vector<std::string> sum;
		static std::string str;
		if(sum.size() && !regen) {
			return str;
		}
		sum.clear();
		str.clear();
		const auto p = ptr_by_uuid(this->uuid);
		sum.emplace_back(CAT("uuid: ",this->uuid));
		sum.emplace_back(CAT("name: ",(p ? p->name() : "<UNKNOWN>")));
		sum.emplace_back(CAT("primary: ",(p && p->primary() ? p->primary()->name.c_str() : "-")));
		sum.emplace_back(CAT("secondary: ",(p && p->secondary() ? p->secondary()->name.c_str() : "-")));
		for(const auto& line : sum) {
			str += line + "\r\n";
		}
		return str;
	}
	void generic_thief::door_entry_event(player_ptr_t& player) {
		if(player->is_npc()) {
			if(is_rival(player)) {
				btree_roam();
				//TODO: attack_with_melee(player);
				//player->sendln(CAT("I am:",uuid," and I'm Watching you"));
			}
		}
	}
	void generic_thief::init() {
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
	};

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	generic_thief::generic_thief(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate generic_thief with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::GENERIC_THIEF;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		bootstrap_equipment();
	}
	/**
	 * @brief spray direction
	 *
	 * @param dir
	 *
	 * @return
	 */
	feedback_t& generic_thief::spray(uint8_t dir) {
		//m_debug("SPRAYING: " << dirstr(dir));
		this->spray_direction = dir;
		if(this->player()->primary() && this->player()->primary()->has_rifle()) {
			this->last_attack = mods::weapons::damage_types::spray_direction_with_feedback(player_ptr,dir);
			this->weapon_heat += 20; /** TODO: */
		} else {
			m_debug("Not spraying as this mob doesn't have a sprayable weapon");
		}
		return this->last_attack;
	}
	void generic_thief::snipe(player_ptr_t victim,direction_t direction,uint8_t distance) {
		auto weapon = this->primary();
		auto player = this->player();
		mods::combat_composer::snipe_target(this->player(),victim,direction,distance,weapon);
	}
	void generic_thief::melee_attack(player_ptr_t victim) {
		hit(this->player()->cd(),victim->cd(),0);
	}
	void generic_thief::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			player_ptr->char_specials().fighting = p->cd();
		}
	}
	player_ptr_t generic_thief::get_next_attacking_priority() {
		if(!m_attackers.empty()) {
			return m_attackers.front();
		}
		return nullptr;
	}
	void generic_thief::increment_last_attack_significance() {

	}
	bool generic_thief::rifle_attack_within_range() {
		//m_debug("rifle_attack_within_range");
		if(!m_weapon) {
			m_weapon = player_ptr->primary();
		}
		if(m_last_attacker) {
			if(m_last_attacker->position() == POS_DEAD) {
				//m_debug("Our target is dead!");
				m_attackers.remove(m_last_attacker);
				m_last_attacker = get_next_attacking_priority();
			}
		}
		for(const auto& attacker : m_attackers) {
			auto results = mods::scan::los_find(player_ptr,attacker);
			if(results.found) {
				this->snipe(attacker,results.direction,results.distance);
				//m_debug("distance:" << results.distance << ", direction: " << results.direction);
				m_attackers_last_direction = results.direction;
				return true;
			}
		}
		return false;
	}
	bool generic_thief::melee_attack_within_range() {
		//m_debug("melee_attack_within_range");
		if(!m_weapon) {
			m_weapon = player_ptr->primary();
		}
		if(m_last_attacker) {
			if(m_last_attacker->position() == POS_DEAD) {
				//m_debug("Our target is dead!");
				m_attackers.remove(m_last_attacker);
				m_last_attacker = get_next_attacking_priority();
			}
		}
		auto us = this->player()->cd();
		for(const auto& attacker : m_attackers) {
			if(attacker->room() == player_ptr->room()) {
				hit(us, attacker->cd(), 0);
				return true;
			}

			auto results = mods::scan::los_find(player_ptr,attacker);
			move_to(results.direction);
			if(attacker->room() == player_ptr->room()) {
				hit(us, attacker->cd(), 0);
				return true;
			}
			//m_debug("distance:" << results.distance << ", direction: " << results.direction);
			m_attackers_last_direction = results.direction;
		}
		return false;
	}

	int8_t generic_thief::move_closer_to_target() {
		uint8_t loops = 1;
		if(mods::rand::chance(CAR_THIEF_EXTRA_LOOP_CHANCE())) {
			++loops;
		}
		for(int i =0; i < loops; ++i) {
			auto results = mods::scan::los_find(player_ptr,m_last_attacker);
			if(results.found && m_last_attacker->room() != this->room()) {
				move_to(m_attackers_last_direction.value());
			}
		}
		if(m_last_attacker->room() == this->room()) {
			return 0;
		}
		auto results = mods::scan::los_find(player_ptr,m_last_attacker);
		return results.distance;
	}
	/*
	void generic_thief::move_closer_to_vehicle() {

	}
	*/
	int8_t generic_thief::determine_heading_from_found_vehicles() {
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

	/**
	 * ===================
	 * = Pilfer dynamics =
	 * ===================
	 */
	/**
	 * steal_from and roll_steal_success are both virtual functions
	 * since we want child classes to be able to make their own
	 * decisions for who and how well they can steal from.
	 */
	bool generic_thief::should_steal_from(player_ptr_t& victim) {
		return mods::rand::chance(25);
	}
	bool generic_thief::roll_steal_success(player_ptr_t& victim) {
		float chance = (victim->dexterity() / this->player()->dexterity()) / 5 * 100;
		if(chance <= 0) {
			return mods::rand::chance(1);
		}
		return mods::rand::chance(chance);
	}
	/**
	 * =======================
	 * = END Pilfer dynamics =
	 * =======================
	 */




	bool generic_thief::has_found_item() {
		return m_found_item;
	}
	void generic_thief::set_found_item(bool status) {
		m_found_item = status;
	}
	void generic_thief::found_witness(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_hostiles,10);
		m_hostiles.emplace_front(data);
	}
#if 0
	void generic_thief::found_item(const mods::scan::vec_player_data_element& data) {
		m_scanned_items.emplace_back(data);
	}
#endif
	void generic_thief::clear_scanned_items() {
		m_debug(red_str("I'm clearing my scanned items"));
		m_scanned_items.clear();
	}
	void generic_thief::remember_item(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_remembered_items,10);
		m_remembered_items.push_front(data.uuid);
	}
	const generic_thief::uuidlist_t& generic_thief::get_remembered_items() const {
		return m_remembered_items;
	}
	void generic_thief::found_item(mods::scan::vec_player_data_element const& item) {
		clear_list_if_count(&m_scanned_items,10);
		m_scanned_items.emplace_back(item);
	}
	std::vector<std::shared_ptr<generic_thief>>& generic_thief_list() {
		static std::vector<std::shared_ptr<generic_thief>> s;
		return s;
	}
};
#undef m_debug
