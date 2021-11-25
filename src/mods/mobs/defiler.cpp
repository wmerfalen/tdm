#include "defiler.hpp"
#include "../weapons/damage-types.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "extended-types.hpp"
#include "roam-pattern.hpp"
#include "../scan.hpp"
#include "../loops.hpp"
#include "../calc-visibility.hpp"
#include "../contract-steps.hpp"
#include "../interpreter.hpp"
#include "../weapons/reload.hpp"
#include "../combat-composer/includes.hpp"
#include "../sneak.hpp"

#ifdef m_debug
#undef m_debug
#endif

#define  __MENTOC_MODS_MOBS_defiler_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_MOBS_defiler_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("defiler") << a << "\n";
#define cmem(a) mentoc_prefix_debug("[defiler][memory_footprint]") << a << "\n";
#else
#define m_debug(a)
#define cmem(a)
#endif

#ifdef __MENTOC_MODS_MOBS_DEFILER_VERBOSE__
#define m_verbose(a) mentoc_prefix_debug("defiler") << a << "\n";
#else
#define m_verbose(a)
#endif

namespace mods::mobs {

	namespace defiler_btree {
		/**
		 * @brief find the room with the most enemies, and go towards that direction
		 *
		 * @param mob
		 * @param victim
		 *
		 * @return
		 */
		uint8_t weighted_direction_decider(player_ptr_t& mob,player_ptr_t victim) {
			uint8_t depth = DEFILER_SCAN_DEPTH();
			mods::scan::vec_player_data vpd;
			mods::scan::los_scan_for_players(mob->cd(),depth,&vpd);
			std::map<uint8_t,int> scores;
			for(auto v : vpd) {
				if(!ptr_by_uuid(v.uuid)) {
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
	};// end namespace defiler_btree

	std::vector<std::string>& defiler::roaming_patterns() const {
		static std::vector<std::string> patterns = {
			"Butcher", "Allied Market"
		};
		return patterns;
	}

	/**
	 * - TODO: as soon as we spawn the defiler, we need to place him in the
	 *   room where he will meet the quest taker
	 */
	void defiler::create(const uuid_t& mob_uuid, std::string_view targets) {
		m_debug("defiler create on uuid:" << mob_uuid);
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate defiler with: %d",mob_uuid);
			return;
		}
		auto g = std::make_shared<defiler>(mob_uuid,targets.data());
		g->btree_roam();
		mods::mobs::defiler_list().push_front(g);
		for(const auto& pat : {
		            "Butcher","Allied Market"
		        }) {
			mods::mobs::roam_pattern::register_roam_pattern(defiler::MOB_VNUM,pat);
		}
	}


	/**
	 * - TODO: This will likely have to involve telegraphs
	 */
	void defiler::perform_random_act() {
		act_to_room(m_random_acts[rand_number(0,m_random_acts.size()-1)]);
	}

	/**
	 * - TODO: when enemies are spotted, we give away the defiler
	 *   location by shouting a taunt. This will be part of the
	 *   telegraph
	 * @brief callback when someone spotted
	 *
	 * @param room
	 * @param player
	 */
	void defiler::enemy_spotted(room_rnum room,uuid_t player) {
		m_verbose("##################################################################################" <<
		          "[defiler] enemy spotted:" << room << "\n" <<
		          "##################################################################################");
		//this->spray(player_ptr->get_watching());
		this->last_seen[player] = CURRENT_TICK();
	}

	/**
	 * There are no variations to the defiler, so this code
	 * should be ignored
	 */
	void defiler::set_variation(const std::string& v) {
		/*
		for(const auto& type : EXPLODE(v,' ')) {
			m_verbose("defiler::variation:" << type << "\n");
		}
		*/
	}

	/**
	 * TODO: include stats (hp,mana,etc) and who the Defiler is currently
	 * hunting
	 */
	str_map_t defiler::report() {
		return usages();
	}


	str_map_t defiler::usages() {
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


	/** =================================================================*/
	/** =================================================================*/
	/** === B E H A V I O U R  T R E E S                                 */
	/** =================================================================*/
	/** =================================================================*/

	void defiler::set_behavior_tree_directly(const defiler::btree_t& t) {
		m_verbose("setting tree id directly to: " << t);
		cd()->mob_specials.behaviour_tree = (uint16_t)t;
	}
	bool defiler::has_tree() {
		return cd()->mob_specials.behaviour_tree != defiler::btree_t::DEFILER_NONE;
	}
	defiler::btree_t defiler::get_tree() {
		return (btree_t)cd()->mob_specials.behaviour_tree;
	}
	void defiler::btree_none() {
		set_behaviour_tree_directly(defiler::btree_t::DEFILER_NONE);
	}
	void defiler::btree_roam() {
		set_behavior_tree_directly(defiler::btree_t::DEFILER_ROAM);


	}
	void defiler::btree_hostile() {
		set_behavior_tree_directly(defiler::btree_t::DEFILER_HOSTILE);

	}

	/** =================================================================*/
	/** =================================================================*/
	/** === DAMAGE CALLBACKS */
	/** =================================================================*/
	/** =================================================================*/
	/**
	 * @brief damage_events registered here
	 */
	void defiler::setup_damage_callbacks() {
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
			if(!ptr_by_uuid(player)) {
				std::cerr << type().data() << ":" << red_str("USE AFTER FREE") << "\n";
				return;
			}
			switch(feedback.damage_event) {
				case de::OUT_OF_AMMO_EVENT: {
						/** TODO: I *REALLY* need this to work! */
						auto ammo = create_object(ITEM_CONSUMABLE,"opsix-incendiary-ar-clip.yml");
						player_ptr->carry(ammo);
					}
					mods::weapons::reload::reload_weapon(player_ptr,player_ptr->primary());
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
					++m_cant_find;
					if(m_cant_find > 3) {
						m_debug("Resetting target");
						this->reset_last_attacker();
					}
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
			int decision = defiler_btree::weighted_direction_decider(player_ptr,nullptr);
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

	/**
	 * TODO: if this is used, it needs to return true
	 */
	bool defiler::is_rival(player_ptr_t& player) {
		return true;
	}

	/**
	 * - TODO: React according to which mode the Defiler is in
	 * 		- [ ] Ensnaring mode causes snare to be immediately set on player
	 * 			- [ ] Defiler moves away and throws daggers
	 * 		- [ ] Corpse explosion mode causes corpse to explode
	 * 		- [ ] Dagger mode causes Defiler to throw daggers at player's legs
	 * 			- [ ] Player cannot move until daggers taken out
	 * 		- [ ] Chainsaw mode causes Defiler to attack player with Chainsaw
	 */
	void defiler::door_entry_event(player_ptr_t& player) {
		if(player->is_npc()) {
			if(is_rival(player)) {
				btree_roam();
				//TODO: attack_with_melee(player);
				//player->sendln(CAT("I am:",uuid," and I'm Watching you"));
			}
		}
	}

	/**
	 * - TODO: add quest taker permanently to memory
	 */
	void defiler::init() {
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
		m_cant_find = 0;
		cmem("m_random_acts:" << m_random_acts.size());
	};

	/**
	 * @brief preferred constructor method
	 *
	 * @param mob_uuid
	 * @param variation
	 */
	defiler::defiler(const uuid_t& mob_uuid, std::string_view variation) {
		this->init();
		this->uuid = mob_uuid;
		auto p = ptr_by_uuid(mob_uuid);
		if(!p) {
			log("SYSERR: did not find player to populate defiler with: %d",mob_uuid);
			this->loaded = false;
			this->error = true;
			return;
		}
		player_ptr = p;
		auto ch = p->cd();
		ch->mob_specials.extended_mob_type = mob_special_data::extended_mob_type_t::MELEE_COMBATANT;
		this->setup_damage_callbacks();
		this->loaded = true;
		this->error = false;
		this->set_variation(variation.data());
		bootstrap_equipment();
		m_weapon = player()->primary();
		std::fill(m_weapons.begin(),m_weapons.end(),nullptr);
		std::size_t i= 0;
		if(player_ptr->primary()) {
			m_weapons[i++] = player_ptr->primary();
		}
		if(player_ptr->secondary()) {
			m_weapons[i++] = player_ptr->secondary();
		}
	}
	//std::vector<std::pair<direction_t,player_ptr_t>> scan_for_any_targets(player_ptr_t& player) {
	//	std::vector<std::pair<direction_t,player_ptr_t>> targets;
	//	for(const auto& dir : room(player)->directions()) {
	//		auto f = scan_attackable(dir);
	//		auto target = ptr_by_uuid(f.uuid);
	//		if(target) {
	//			targets.emplace_back(std::make_pair<>(dir,target));
	//		}
	//	}
	//	return targets;
	//}
	//










	defiler::weapons_list_t& defiler::weapons() {
		return m_weapons;
	}



	direction_t random_room_direction(auto room_id) {
		if(world.size() < room_id) {
			return NORTH;
		}
		auto& room = world[room_id];
		return room.directions()[rand_number(0,room.directions().size()-1)];
	}


	void defiler::reset_last_attacker() {
		m_debug("Resetting last attacker data");
		m_attackers.clear();
		m_last_attacker = nullptr;
		m_cant_find = 0;
		m_attackers_last_direction = std::nullopt;
	}

	void defiler::attack(player_ptr_t& victim) {
		m_last_attacker = victim;

		for(auto weapon : {
		            player_ptr->primary(),player_ptr->secondary()
		        }) {
			if(!weapon) {
				continue;
			}
			if(mods::object_utils::is_rifle(weapon)) {
				if(!victim) {
					this->reset_last_attacker();
					return;
				}
				if(victim->room() == player_ptr->room()) {
					/** move any direction away to do a snipe attack */
					auto dir = random_room_direction(victim->room());
					this->move_to(dir);
					mods::combat_composer::snipe_target(player_ptr,victim->name().c_str(), dir,0,weapon);
					return;
				} else {
					for(const auto& dir : world[player_ptr->room()].directions()) {
						if(!victim) {
							this->reset_last_attacker();
							return;
						}
						mods::combat_composer::snipe_target(player_ptr,victim->name().c_str(),dir,0,weapon);
					}
					return;
				}
			} else if(mods::object_utils::is_melee(weapon)) {
				if(!victim) {
					this->reset_last_attacker();
					return;
				}
				if(victim->room() == player_ptr->room()) {
					melee_attack_within_range();
				} else {
					char_from_room(cd());
					char_to_room(cd(),victim->room());
					melee_attack_within_range();
				}
				return;
			}
		}
	}






	void defiler::hostile_phase_1() {

		if(m_last_attacker && m_last_attacker->position() != POS_DEAD) {
			this->attack(m_last_attacker);
			return;
		}
		auto target = spawn_near_someone();
		if(!target) {
			return;
		}
		if(target->position() > POS_DEAD) {
			m_verbose("spawned near someone. attacking them");
			this->attack(target);
		}
	}

	/**
	 * - TODO: Agony scream and disappearance
	 */
	void defiler::attacked(const feedback_t& feedback) {
		auto p = ptr_by_uuid(feedback.attacker);
		if(p) {
			m_last_attacker = p;
			m_attackers.emplace_front(p);
			//cmem("{m_attackers.size}:" << std::distance(m_attackers.cbegin(),m_attackers.cend()));
		}
	}

	/**
	 * - TODO: Must always return quest taker
	 */
	player_ptr_t defiler::get_next_attacking_priority() {
		if(!m_attackers.empty()) {
			return m_attackers.front();
		}
		return nullptr;
	}

	/**
	 * - TODO: for each mode, determines if the following will happen
	 *   - [ ] Ensnaring, throw dagger
	 *   - [ ] Dagger, snipe
	 *   - [ ] Corpse Explosion, snipe/engage
	 *   - [ ] Chainsaw,
	 */
	void defiler::extra_attack() {
		m_debug("extra attack roll success");
		auto attacker = player()->fighting();
		if(m_weapon && attacker->room() == this->room()) {
			m_debug("i have a weapon and i'm using it against who i'm fighting");
			mods::weapons::damage_types::melee_damage_with_feedback(player(),m_weapon,attacker);
		}
	}
	void defiler::melee_attack_within_range() {
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
	std::pair<bool,std::string> defiler::move_to(const direction_t& dir) {
		auto old_vis = player_ptr->visibility();
		auto room_id = player_ptr->room();
		auto opt = world[room_id].dir_option[dir];
		if(opt && opt->to_room <= world.size()) {
			mods::mobs::room_watching::unwatch_room(player_ptr);
			player_ptr->visibility() = 0;
			mods::sneak::apply_absolute_sneak(player_ptr,100);
			perform_move(player_ptr->cd(),dir,0);
			player_ptr->visibility() = old_vis;
			mods::sneak::clear_sneak(player_ptr);
			return {true,"moved"};
		}
		return {false,"stayed"};

	}

	void defiler::move_closer_to_target() {
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
	int8_t defiler::determine_heading_from_found_victims() {
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
	bool defiler::has_found_item() {
		return m_found_item;
	}
	void defiler::set_found_item(bool status) {
		m_found_item = status;
	}
	void defiler::found_witness(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_hostiles,10);
		m_hostiles.emplace_front(data);
		cmem("m_hostiles:" << std::distance(m_hostiles.cbegin(),m_hostiles.cend()));
	}
	void defiler::clear_scanned_items() {
		m_scanned_items.clear();
	}
	void defiler::remember_item(const mods::scan::vec_player_data_element& data) {
		clear_list_if_count(&m_remembered_items,10);
		m_remembered_items.push_front(data.uuid);
		cmem("m_remembered_items:" << std::distance(m_remembered_items.cbegin(),m_remembered_items.cend()));
	}
	const defiler::uuidlist_t& defiler::get_remembered_items() const {
		return m_remembered_items;
	}
	void defiler::found_item(mods::scan::vec_player_data_element const& item) {
		clear_list_if_count(&m_scanned_items,10);
		m_scanned_items.emplace_back(item);
		cmem("m_scanned_items:" << m_scanned_items.size());
	}
	std::forward_list<std::shared_ptr<defiler>>& defiler_list() {
		static std::forward_list<std::shared_ptr<defiler>> s;
		return s;
	}
	uint8_t defiler::scan_depth() const {
		return 8;
	}

	void defiler::found_target(player_ptr_t& player) {

	}

	/**
	 * - This is actually something that would prove very useful
	 * - This would fulfill the "unfair" role that bosses in video games usually have
	 */
	player_ptr_t defiler::spawn_near_someone() {
		player_ptr_t who = nullptr;
		mods::loops::foreach_player([&,this](auto player) -> bool {
			if(player->position() <= POS_STUNNED) {
				return true;
			}
			if(mods::mobs::roam_pattern::can_roam_to(defiler::MOB_VNUM,player->room()) && chance(50)) {
				m_debug("You madman! The DEFILER IS GOING TO TELEPORT!!!");
				who = player;
				//player->sendln("Beware! The DEFILER approaches!");
				char_from_room(cd());
				char_to_room(cd(),player->room());
				return false;
			}
			return true;
		});
		return who;
	}

	/**
	 * - TODO: we'll want to make this not just _anyone_ but instead the
	 *   person who is undergoing the quest to take down the boss
	 */
	bool defiler::attack_anyone_in_same_room() {
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

	/**
	 * - TODO: we'll want to not clear the quest taker
	 */
	void defiler::clear_state() {
		m_scanned_items.clear();
		m_hostiles.clear();
		m_attackers.clear();
		m_remembered_items.clear();
	}
};
namespace mods::mobs::defiler_init {
	static constexpr std::array<obj_vnum,2> has_human_remains  = {
		197, /** 12 ounce steak */
		198, /** frozen liver */
	};

	ACMD(do_bioscan) {
		static constexpr std::string_view usage = "usage: bioscan <target>\r\nexample: bioscan meat\r\n";
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln(usage);
			return;
		}
		if(!player->has_contract()) {
			player->sendln("Scan all you want, but you're not currently under contract.");
			return;
		}
		if(argshave()->size_gt(0)->passed()) {
			auto target = argat(0);
			auto obj = OBJFIND(target,player);
			if(!obj) {
				player->sendln("Couldn't find anything!");
				return;
			} else {
				if(std::find(has_human_remains.cbegin(),has_human_remains.cend(),obj->item_number) != has_human_remains.cend()) {
					player->contract_custom_event(mods::contracts::custom_events_t::CEV_HUMAN_REMAINS_FOUND,obj->uuid);
					player->sendln("{grn}[+] Positive read: {red}HUMAN REMAINS{/red}");
				} else {
					player->sendln("{red}[-] Negative read{/red}");
				}
			}
		}
	}
	void init() {
		/**
		 * Builds resistance to shrapnel and incendiary damage
		 */
		using namespace mods::mobs::resistance;
		using ele = mods::elemental_types_t;
		using un = unit_type_t;
		std::vector<resistance_t> rezzes;
		static constexpr const resistance_t r[] = {
			{100.0,un::PERCENTAGE,ele::ELEM_SHRAPNEL},
			{100.0,un::PERCENTAGE,ele::ELEM_INCENDIARY},
		};
		for(const auto& rez : r) {
			rezzes.emplace_back(rez);
		}

		register_resistances_for_mob_vnum(
		    mods::mobs::defiler::MOB_VNUM,
		    rezzes
		);
		mods::interpreter::add_user_command("bioscan",do_bioscan);
	}
};//end mods::mobs::defiler_init
#undef m_debug
#undef cmem
