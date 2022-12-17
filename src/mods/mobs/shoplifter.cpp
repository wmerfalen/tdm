#include "shoplifter.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "roam-pattern.hpp"
#include "extended-types.hpp"
#include "../scan.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"

//#define  __MENTOC_MODS_MOBS_shoplifter_SHOW_DEBUG_OUTPUT__
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

	std::forward_list<std::shared_ptr<shoplifter>>& shoplifter_list() {
		static std::forward_list<std::shared_ptr<shoplifter>> s;
		return s;
	}

#ifdef __MENTOC_SHOPLIFTER_VOCALIZE_DEBUG__
	#define COMPLAIN(A) this->complain(CAT(__FILE__,"|",__LINE__,"[",__FUNCTION__,"]: SHOPLIFTER COMPLAINT: ",A));
	#define PROGRESS(A) this->progress(CAT(__FILE__,"|",__LINE__,"[",__FUNCTION__,"]: SHOPLIFTER PROGRESS: ",A));
#else
	#define COMPLAIN(A) /** */
	#define PROGRESS(A) /** */
#endif

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
		mods::mobs::shoplifter_list().emplace_front(std::make_shared<shoplifter>(mob_uuid,targets));
	}


	void shoplifter::set_variation(const std::string& v) {
		//for(const auto& type : EXPLODE(v,' ')) {
		//	std::cerr << green_str("shoplifter::variation:") << type << "\n";
		//}
	}

	void shoplifter::set_behavior_tree_directly(const shoplifter::btree_t& t) {
		//m_debug("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}

	void shoplifter::btree_hostile() {
		set_behavior_tree_directly(shoplifter::btree_t::SHOPL_HOSTILE);
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
						if(!can_roam_to(decision)) {
							return;
						}
						move_to(decision);
						set_heading(decision);
					} else {
						m_debug("shoplifter (as victim) ***DOES NOT*** spot attacker!");
					}
				}
			}
		});

	}



	void shoplifter::init() {
		smart_mob::init();
		m_found_item = 0;
		m_last_attacker = nullptr;
		m_attackers.clear();
		m_weapon = nullptr;
		m_attackers_last_direction = std::nullopt;
		m_scanned_items.clear();
		m_remembered_items.clear();
		/** TODO:
		for(const auto& msg : EXPLODE(CHAOTIC_METH_ADDICT_PSV_RANDOM_ACT(),'|')) {
			m_random_acts.emplace_back(msg);
		}
		*/
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
		player_ptr = ptr_by_uuid(mob_uuid);
		if(!player_ptr) {
			log("SYSERR: did not find player to populate shoplifter with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr->cd();
		player_ptr->cd()->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::SHOPLIFTER;
		player_ptr->cd()->mob_specials.vnum = player_ptr->cd()->mob_specials.vnum;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		m_weapon = player_ptr->primary();
		m_optimal_range = BEST_DISTANCE;
	}





	void shoplifter::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
			mods::weapons::damage_types::melee_damage_with_feedback(player(),m_weapon,attacker);
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
				move_to_with_roam_check(m_attackers_last_direction.value());
			}
		}
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
				if(scan.distance == BEST_DISTANCE  && !IS_NPC(scan.ch)) {
					m_debug("best distance found: " << ptr(scan.ch)->name());
					finds.emplace_back(res(direction,scan.ch));
				}
			}
		}
		if(finds.size() == 0) {
			move_to_with_roam_check(rand_item(world[room()].directions()));
			return false;
		}
		std::size_t index = rand_number(0,finds.size()-1);
		this->set_hunting({finds[index].player->uuid});
		auto feedback = mods::weapons::damage_types::rifle_attack_with_feedback(
		        player_ptr,
		        this->primary(),
		        ptr(finds[index].player),
		        BEST_DISTANCE,
		        finds[index].direction
		    );
		return feedback.hits || feedback.damage;
	}


	void shoplifter::enemy_spotted(room_rnum room,uuid_t player) {
		auto target = ptr_by_uuid(player);
		if(target) {
			this->attack(target);
		}
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
		        BEST_DISTANCE,
		        feedback.from_direction
		    );
		if(result.hits) {
			m_debug("Apparently hit for " << result.hits);
		}
	}

#ifdef __MENTOC_SHOPLIFTER_VOCALIZE_DEBUG__
	void shoplifter::complain(std::string_view msg) {
		std::cerr << "------------------------------------------------------\n";
		std::cerr << "| SHOPLIFTER " << this->uuid << " complains: '" << msg.data() << "'\n";
	}

	void shoplifter::progress(std::string_view msg) {
		std::cerr << "------------------------------------------------------\n";
		std::cerr << "| SHOPLIFTER " << this->uuid << " progresses: '" << msg.data() << "'\n";
	}
#endif

	str_map_t shoplifter::usages() {
		return base_usages();
	}
	str_map_t shoplifter::report() {
		return base_usages();
	}
};
#undef m_debug
#undef m_debug_plain
#undef cmem
#undef COMPLAIN
#undef PROGRESS
