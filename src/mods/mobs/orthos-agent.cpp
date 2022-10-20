#include "orthos-agent.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"

//#define  __MENTOC_MODS_MOBS_orthos_agent_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_orthos_agent_SHOW_DEBUG_OUTPUT__
	#define m_debug(a) mentoc_prefix_debug("m|m|orthos_agent") << "(" << player_ptr->name().c_str() << ") " << a << "\n";
	#define m_debug_plain(a) mentoc_prefix_debug("m|m|orthos_agent") << a << "\n";
	#define cmem(a) mentoc_prefix_debug("[orthos_agent][memory_footprint]") << a << "\n";
#else
	#define m_debug(a)
	#define m_debug_plain(a)
	#define cmem(a)
#endif
namespace mods::mobs {
	static constexpr std::size_t BEST_DISTANCE = 1;
	namespace orthos_agent_btree {
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
				m_debug_plain("[stub] should_fire is -1, choose random direction");
			}
			return should_fire;
		}
	};// end namespace orthos_agent_btree
	void orthos_agent::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug_plain("orthos_agent create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate orthos_agent with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<orthos_agent>(mob_uuid,targets.data());
		g->btree_roam();
		mods::mobs::orthos_agent_list().push_front(g);
		orthos_agent_uuid_list.emplace_back(mob_uuid);
	}
	void orthos_agent::perform_random_act() {
		//act_to_room(m_random_acts[rand_number(0,m_random_acts.size()-1)]);
	}

	/**
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void orthos_agent::enemy_spotted(room_rnum room,uuid_t player) {
		std::cerr << "##################################################################################" <<
		    "[orthos_agent] enemy spotted:" << room << "\n" <<
		    "##################################################################################\n";
		m_debug("##################################################################################" <<
		    "[orthos_agent] enemy spotted:" << room << "\n" <<
		    "##################################################################################");
		//this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}
	void orthos_agent::set_variation(const std::string& v) {
		for(const auto& type : EXPLODE(v,' ')) {
			std::cerr << green_str("orthos_agent::variation:") << type << "\n";
		}
	}
	str_map_t orthos_agent::report() {
		return usages();
	}
	str_map_t orthos_agent::usages() {
		str_map_t m;
		m = base_usages();
		std::size_t rem_size = 0, attackers = 0, hostiles = 0;
		rem_size = std::distance(m_remembered_items.cbegin(),m_remembered_items.cend());
		attackers = std::distance(m_attackers.cbegin(),m_attackers.cend());
		hostiles = std::distance(m_hostiles.cbegin(),m_hostiles.cend());
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
	void orthos_agent::set_behavior_tree_directly(const orthos_agent::btree_t& t) {
		m_debug("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}
	bool orthos_agent::has_tree() {
		return cd()->mob_specials.behaviour_tree != orthos_agent::btree_t::SHOPL_NONE;
	}
	orthos_agent::btree_t orthos_agent::get_tree() {
		return (btree_t)cd()->mob_specials.behaviour_tree;
	}
	void orthos_agent::btree_none() {
		set_behaviour_tree_directly(orthos_agent::btree_t::SHOPL_NONE);
	}
	void orthos_agent::btree_roam() {
		set_behavior_tree_directly(orthos_agent::btree_t::SHOPL_ROAM);

	}
	void orthos_agent::btree_hostile() {
		set_behavior_tree_directly(orthos_agent::btree_t::SHOPL_HOSTILE);

	}
	/**
	 * @brief damage_events registered here
	 */
	void orthos_agent::setup_damage_callbacks() {
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
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			auto attacker = ptr_by_uuid(feedback.attacker);
			auto weapon = player_ptr->primary();

			if(player_ptr->room() != attacker->room()) {
				if(mods::calc_visibility::roll_victim_spots_attacker(player_ptr,attacker,feedback)) {
					m_debug("orthos_agent (as victim) spots attacker!");
					auto decision = feedback.from_direction;
					move_to(decision);
					set_heading(decision);
				} else {
					m_debug("orthos_agent (as victim) ***DOES NOT*** spot attacker!");
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
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			switch(feedback.damage_event) {
				case de::OUT_OF_AMMO_EVENT:
					m_debug("DAMN! OUT OF AMMO!");
					refill_ammo();
					break;
				case de::NO_PRIMARY_WIELDED_EVENT:
					log(CAT("SYSERR: orthos_agent -> No primary wielded... wtf? (",player_ptr->name(),")").c_str());
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
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			if(player_ptr->room() >= world.size()) {
				return;
			}
			auto& room = world[player_ptr->room()];
			int decision = orthos_agent_btree::weighted_direction_decider(player_ptr,nullptr);
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
	bool orthos_agent::is_rival(player_ptr_t& player) {
		return false;
	}
	//void orthos_agent::door_entry_event(player_ptr_t& player) {
	//	if(player->is_npc()) {
	//		std::cerr << "man, you gotta be a real low life to go around sniping other npcs\n";
	//		return;
	//	}
	//	player->sendln("Oh, I got yo azz now");
	//}
	void orthos_agent::init() {
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
		m_watching_everywhere = false;
		this->RCT = nullptr;
		cmem("m_random_acts:" << m_random_acts.size());
	};

	void orthos_agent::watch_everywhere() {
		mods::mobs::helpers::watch_multiple(world[this->room()].directions(),this->cd(),10);
	}
	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	orthos_agent::orthos_agent(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate orthos_agent with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
#ifdef __MENTOC_SHOW_ORTHOS_AGENT_VNUM__
		log("Orthos agent vnum: %d",ch->mob_specials.vnum);
#endif
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::ORTHOS_AGENT;
		this->cd()->mob_specials.vnum = p->cd()->mob_specials.vnum;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		bootstrap_equipment();
		m_weapon = player()->primary();
		m_optimal_range = optimal_range();
	}
	void orthos_agent::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			cmem("{m_attackers.size}:" << std::distance(m_attackers.cbegin(),m_attackers.cend()));
		}
	}
	player_ptr_t orthos_agent::get_next_attacking_priority() {
		return m_attackers.front();
	}
	int orthos_agent::best_distance() {
		m_optimal_range = optimal_range();
		m_debug("optimal range return: '" << (int)m_optimal_range << "'");
		if(m_optimal_range < 0) {
			m_debug("best distance for me (" << player_ptr->name().c_str() << ") is:" << BEST_DISTANCE);
			return BEST_DISTANCE;
		}
		m_debug("best distance for me (" << player_ptr->name().c_str() << ") is:" << m_optimal_range);
		return m_optimal_range;
	}
	void orthos_agent::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
			mods::weapons::damage_types::melee_damage_with_feedback(player(),m_weapon,attacker);
		}
	}
	void orthos_agent::melee_attack_within_range() {
		m_debug("melee_attack_within_range");
		if(m_last_attacker) {
			if(m_last_attacker->position() == POS_DEAD) {
				m_debug("Our target is dead!");
				m_attackers.remove(m_last_attacker);
				m_last_attacker = get_next_attacking_priority();
			}
		}
		for(auto& attacker : m_attackers) {
			auto results = mods::scan::los_find(player_ptr,attacker);
			if(results.found == false) {
				if(results.distance == best_distance()) {
					this->snipe_target(attacker,results.direction,results.distance);
					return;
				}

				if(m_attackers_last_direction.has_value()) {
					move_to(m_attackers_last_direction.value());
				}
				continue;
			}
			m_debug("distance:" << results.distance << ", direction: " << results.direction);
			m_attackers_last_direction = results.direction;

		}
	}

	void orthos_agent::attack(player_ptr_t& player) {
		m_last_attacker = player;
		melee_attack_within_range();
	}
	void orthos_agent::move_closer_to_target() {
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
	int8_t orthos_agent::determine_heading_from_found_victims() {
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
	bool orthos_agent::has_found_item() {
		return m_found_item;
	}
	void orthos_agent::set_found_item(bool status) {
		m_found_item = status;
	}
	void orthos_agent::found_witness(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_hostiles,10);
		m_hostiles.emplace_front(data);
		cmem("m_hostiles:" << std::distance(m_hostiles.cbegin(),m_hostiles.cend()));
	}
	void orthos_agent::clear_scanned_items() {
		m_scanned_items.clear();
	}
	void orthos_agent::remember_item(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_remembered_items,10);
		m_remembered_items.push_front(data.uuid);
		cmem("m_remembered_items: (" << player_ptr->name().c_str() << ") " << std::distance(m_remembered_items.cbegin(),m_remembered_items.cend()));
	}
	const orthos_agent::uuidlist_t& orthos_agent::get_remembered_items() const {
		return m_remembered_items;
	}
	void orthos_agent::found_item(mods::scan::vec_player_data_element const& item) {
		clear_list_if_count(&m_scanned_items,10);
		m_scanned_items.emplace_back(item);
		cmem("m_scanned_items: (" << player_ptr->name().c_str() << ") " << m_scanned_items.size());
	}
	std::forward_list<std::shared_ptr<orthos_agent>>& orthos_agent_list() {
		static std::forward_list<std::shared_ptr<orthos_agent>> s;
		return s;
	}
	uint8_t orthos_agent::scan_depth() const {
		return CHAOTIC_METH_ADDICT_SCAN_DEPTH();
	}
	player_ptr_t orthos_agent::spawn_near_someone() {
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
	bool orthos_agent::attack_anyone_near_room() {
		struct res {
			direction_t direction;
			uint8_t distance;
			char_data* player;
			res() = delete;
			res(direction_t d, uint8_t dist,char_data* p) : direction(d), distance(dist),player(p) {}
		};
		std::vector<res> finds;
		for(const auto& direction : world[room()].directions()) {
			for(const auto& scan : this->scan_attackable(direction)) {
				if(scan.distance == best_distance() && !IS_NPC(scan.ch)) {
					m_debug("best distance found: " << ptr(scan.ch)->name());
					finds.emplace_back(res(direction,scan.distance,scan.ch));
				}
			}
		}
		if(finds.size() == 0) {
			return false;
		}
		std::size_t index = rand_number(0,finds.size()-1);
		this->set_hunting({finds[index].player->uuid});
		auto target = ptr(finds[index].player);
		this->snipe_target(
		    target,
		    finds[index].direction,
		    finds[index].distance
		);
		return true;// TODO FIXME figure this out
	}
	void orthos_agent::clear_state() {
		m_scanned_items.clear();
		m_hostiles.clear();
		m_attackers.clear();
		m_remembered_items.clear();
	}
	void orthos_agent::door_entry_event(player_ptr_t& victim,const room_rnum room_id) {
		//std::tuple<bool,direction_t,int> r = mods::scan::los_find_player_with_depth(player_ptr,victim,RCT->max_range);
		//if(std::get<0>(r)) {
		//	this->set_hunting(victim->uuid());
		//}
	}

	void orthos_agent::attack_mode() {
		m_debug(green_str("-----------------------------"));
		m_debug(green_str("Orthos agent simplified btree"));
		m_debug(green_str("-----------------------------"));

		if(!this->RCT) {
			this->RCT = player()->get_ranged_combat_totals();
		}
		/** it is imperative that this be called AFTER get ranged combat totals */
		if(!m_watching_everywhere) {
			this->watch_everywhere();
			m_watching_everywhere = true;
		}
		/** Scan for players */
		int depth = RCT->max_range;
		m_debug(green_str(CAT("This is my depth: ",depth,"!!!")));
		mods::scan::vec_player_data vpd;
		mods::scan::los_scan_for_players(this->cd(),depth,&vpd);
		std::map<uint8_t,int> scores;
		for(auto&& v : vpd) {
			auto victim = ptr_by_uuid(v.uuid);
			if(!victim) {
				continue;
			}
			if(mods::rooms::is_peaceful(v.room_rnum)) {
				continue;
			}
			if(IS_NPC(victim->cd())) {
				continue;
			}
			this->snipe_target(v);
		}

	}
	namespace orthos_callbacks {
		bool dispatch_watcher(const uuid_t& orthos_agent_uuid,player_ptr_t& player, const room_rnum& room_id) {
			for(auto& agent : orthos_agent_list()) {
				if(agent->uuid == orthos_agent_uuid) {
					agent->door_entry_event(player,room_id);
					return true;
				}
			}
			return false;
		}
	};
};
#undef m_debug
#undef m_debug_plain
#undef cmem
