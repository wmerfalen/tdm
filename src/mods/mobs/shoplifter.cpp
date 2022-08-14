#include "shoplifter.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"

#define  __MENTOC_MODS_MOBS_shoplifter_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_shoplifter_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|m|shoplifter") << "(" << player_ptr->name().c_str() << ") " << a << "\n";
#define m_debug_plain(a) mentoc_prefix_debug("m|m|shoplifter") << a << "\n";
#define cmem(a) mentoc_prefix_debug("[shoplifter][memory_footprint]") << a << "\n";
#else
#define m_debug(a)
#define m_debug_plain(a)
#define cmem(a)
#endif
namespace mods::mobs {
	static constexpr std::size_t BEST_DISTANCE = 1;
	static constexpr uint8_t SCAN_DEPTH = 3;
#define COMPLAIN(A) this->complain(CAT(__FILE__,"|",__LINE__,"[",__FUNCTION__,"]: SHOPLIFTER COMPLAINT: ",A));
#define PROGRESS(A) this->progress(CAT(__FILE__,"|",__LINE__,"[",__FUNCTION__,"]: SHOPLIFTER COMPLAINT: ",A));

	bool shoplifter_can_move(room_rnum room) {
		return room < world.size() && world[room].directions().size() > 0;
	}

	void shoplifter::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug_plain("shoplifter create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate shoplifter with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<shoplifter>(mob_uuid,targets.data());
		g->btree_hostile();
		mods::mobs::shoplifter_list().push_front(g);
	}

	void shoplifter::enemy_spotted(room_rnum room,uuid_t player) {
		m_debug("##################################################################################" <<
		        "[shoplifter] enemy spotted:" << room << "\n" <<
		        "##################################################################################");
		auto target = ptr_by_uuid(player);
		if(target) {
			this->last_seen[player] = CURRENT_TICK();
			this->attack(target);
		}
	}

	void shoplifter::set_variation(const std::string& v) {
		for(const auto& type : EXPLODE(v,' ')) {
			std::cerr << green_str("shoplifter::variation:") << type << "\n";
		}
	}
	str_map_t shoplifter::report() {
		return usages();
	}
	str_map_t shoplifter::usages() {
		return base_usages();
	}
	void shoplifter::set_behavior_tree_directly(const shoplifter::btree_t& t) {
		m_debug("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}
	bool shoplifter::has_tree() {
		return cd()->mob_specials.behaviour_tree != shoplifter::btree_t::SHOPL_NONE;
	}
	shoplifter::btree_t shoplifter::get_tree() {
		return (btree_t)cd()->mob_specials.behaviour_tree;
	}
	void shoplifter::btree_none() {
		set_behaviour_tree_directly(shoplifter::btree_t::SHOPL_NONE);
		this->btree_hostile();
	}
	void shoplifter::btree_hostile() {
		set_behavior_tree_directly(shoplifter::btree_t::SHOPL_HOSTILE);
		this->attack_anyone_near_room();
	}
	void shoplifter::setup_damage_callbacks() {
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
			btree_hostile();
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
				return;
			}
			PROGRESS("Okay, enrage_if processing...");
			auto attacker = ptr_by_uuid(feedback.attacker);
			auto weapon = player_ptr->primary();

			if(player_ptr->room() != attacker->room()) {
				if(mods::calc_visibility::roll_victim_spots_attacker(player_ptr,attacker,feedback)) {
					m_debug("shoplifter (as victim) spots attacker!");
					auto decision = feedback.from_direction;
					auto distance = distance_to(attacker);
					if(distance.has_value()) {
						if(distance.value() == BEST_DISTANCE) {
							m_debug("DIE FUCKER!");
							this->attack(feedback,attacker);
							return;
						}
						move_to(decision);
						set_heading(decision);
					} else {
						m_debug("shoplifter (as victim) ***DOES NOT*** spot attacker!");
					}
				}
				if(player_ptr->room() == attacker->room()) {
					mods::weapons::damage_types::melee_damage_with_feedback(player_ptr,weapon,attacker);
				}
			}
		});

	}
	//bool shoplifter::is_rival(player_ptr_t& player) {
	//	return false;
	//}
	//void shoplifter::door_entry_event(player_ptr_t& player) {
	//	if(player->is_npc()) {
	//		if(is_rival(player)) {
	//			btree_roam();
	//			//TODO: attack_with_melee(player);
	//			//player->sendln(CAT("I am:",uuid," and I'm Watching you"));
	//		}
	//	}
	//}
	void shoplifter::init() {
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
	shoplifter::shoplifter(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate shoplifter with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::SHOPLIFTER;
		this->cd()->mob_specials.vnum = p->cd()->mob_specials.vnum;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		bootstrap_equipment();
		m_weapon = player()->primary();
		m_optimal_range = BEST_DISTANCE;
	}
	void shoplifter::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			cmem("{m_attackers.size}:" << std::distance(m_attackers.cbegin(),m_attackers.cend()));
		}
	}
	player_ptr_t shoplifter::get_next_attacking_priority() {
		return m_attackers.front();
	}

	int shoplifter::optimal_range() const {
		return BEST_DISTANCE;
	}

	void shoplifter::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
			mods::weapons::damage_types::melee_damage_with_feedback(player(),m_weapon,attacker);
		}
	}
	void shoplifter::melee_attack_within_range() {
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
				if(results.distance == this->optimal_range()) {
					auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(player_ptr,primary(),attacker,results.distance,results.direction);
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

	void shoplifter::move_closer_to_target() {
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
	int8_t shoplifter::determine_heading_from_found_victims() {
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
	bool shoplifter::has_found_item() {
		return m_found_item;
	}
	void shoplifter::set_found_item(bool status) {
		m_found_item = status;
	}
	void shoplifter::found_witness(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_hostiles,10);
		m_hostiles.emplace_front(data);
		cmem("m_hostiles:" << std::distance(m_hostiles.cbegin(),m_hostiles.cend()));
	}
	void shoplifter::clear_scanned_items() {
		m_scanned_items.clear();
	}
	void shoplifter::remember_item(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_remembered_items,10);
		m_remembered_items.push_front(data.uuid);
		cmem("m_remembered_items: (" << player_ptr->name().c_str() << ") " << std::distance(m_remembered_items.cbegin(),m_remembered_items.cend()));
	}
	const shoplifter::uuidlist_t& shoplifter::get_remembered_items() const {
		return m_remembered_items;
	}
	void shoplifter::found_item(mods::scan::vec_player_data_element const& item) {
		clear_list_if_count(&m_scanned_items,10);
		m_scanned_items.emplace_back(item);
		cmem("m_scanned_items: (" << player_ptr->name().c_str() << ") " << m_scanned_items.size());
	}
	std::forward_list<std::shared_ptr<shoplifter>>& shoplifter_list() {
		static std::forward_list<std::shared_ptr<shoplifter>> s;
		return s;
	}
	uint8_t shoplifter::scan_depth() const {
		return SCAN_DEPTH;
	}
	player_ptr_t shoplifter::spawn_near_someone() {
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
	bool shoplifter::attack_anyone_near_room() {
		if(!shoplifter_can_move(this->room())) {
			COMPLAIN("I CANNOT MOVE! Room has zero exits!");
			return false;
		}
		struct res {
			direction_t direction;
			char_data* player;
			res() = delete;
			res(direction_t d, char_data* p) : direction(d), player(p) {}
		};
		std::vector<res> finds;
		const auto& directions = world[room()].directions();
		for(const auto& direction : directions) {
			for(const auto& scan : this->scan_attackable(direction)) {
				if(scan.distance == this->optimal_range() && !IS_NPC(scan.ch)) {
					m_debug("best distance found: " << ptr(scan.ch)->name());
					finds.emplace_back(res(direction,scan.ch));
				}
			}
		}
		if(finds.size() == 0) {
			move_to(rand_item(world[room()].directions()));
			return false;
		}
		std::size_t index = rand_number(0,finds.size()-1);
		this->set_hunting({finds[index].player->uuid});
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		                    player_ptr,
		                    this->primary(),
		                    ptr(finds[index].player),
		                    this->optimal_range(),
		                    finds[index].direction
		                );
		return feedback.hits || feedback.damage;
	}
	void shoplifter::clear_state() {
		m_scanned_items.clear();
		m_hostiles.clear();
		m_attackers.clear();
		m_remembered_items.clear();
	}
	void shoplifter::attack(player_ptr_t attacker) {
		struct res {
			direction_t direction;
			char_data* player;
			res() = delete;
			res(direction_t d, char_data* p) : direction(d), player(p) {}
		};
		std::vector<res> finds;
		for(const auto& direction : world[room()].directions()) {
			for(const auto& scan : this->scan_attackable(direction)) {
				if(scan.distance == BEST_DISTANCE && scan.ch->uuid == attacker->uuid()) {
					m_debug("best distance found: " << ptr(scan.ch)->name());
					auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
					                    player_ptr,
					                    this->primary(),
					                    ptr(scan.ch),
					                    scan.distance,
					                    direction
					                );
					return;
				}
			}
		}
	}
	void shoplifter::attack(const feedback_t& feedback,player_ptr_t attacker) {
		auto result = mods::weapons::damage_types::rifle_attack_with_feedback(
		                  player_ptr,
		                  this->primary(),
		                  attacker,
		                  this->optimal_range(),
		                  feedback.from_direction
		              );
		if(result.hits) {
			m_debug("Apparently hit for " << result.hits);
		}
	}
	void shoplifter::complain(std::string_view msg) {
		std::cerr << "------------------------------------------------------\n";
		std::cerr << "| SHOPLIFTER " << this->uuid << " complains: '" << msg.data() << "'\n";
	}
	void shoplifter::progress(std::string_view msg) {
		std::cerr << "------------------------------------------------------\n";
		std::cerr << "| SHOPLIFTER " << this->uuid << " progresses: '" << msg.data() << "'\n";
	}
};
#undef m_debug
#undef m_debug_plain
#undef cmem
