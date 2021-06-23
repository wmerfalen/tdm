#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "car-thief.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"

#define  __MENTOC_SHOW_BEHAVIOUR_TREE_car_thief_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_car_thief_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[m.m.ct.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);


namespace mods::mobs::car_thief_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = mods::npc&;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;

	TChildNode shotgun_attack() {
		return TNode::create_leaf([](mods::npc& mob) -> TStatus {
			car_thief_ptr(mob.uuid())->shotgun_attack_within_range();
			return TStatus::SUCCESS;
		});
	}
	/**
	 * MP Shotgunner behaviour tree
	 * ----------------------------
	    [1] When attacked
				[1] radio nearby units for help at coordinates
				[2] immediately pursue until in same room
				[3] fire shotgun
				[4] repeat until dead or target dead

			[2] When radio'd for help
				[1] change behaviour tree to mps_backup_pursuit
				[2] add reported player uuid to remember event

			[mps_backup_pursuit behaviour tree]
				[1] scan for specific target
				[2] if found, head to that direction
				[3] engage target if found
				[4] repeat until dead or target dead
				[5] forget target
				[6] teleport back to zone cmd room

	 */

	TChildNode debug_echo_tree_name(str_t name) {
		return TNode::create_leaf([&](mods::npc& mob) -> TStatus {
#ifdef __MENTOC_SHOW_TREE_NAME__
			std::cerr << "[debug_echo_tree_name][mob_uuid:" << mob.uuid() << "]" <<
			          green_str(CAT("[tree:'",name,"']\n"));
#endif
			return TStatus::SUCCESS;
		});
	}
	TChildNode shout_random() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			/** TODO: replace this mg->shout(random_key_string(LOWLY_SECURITY_RANDOM_ATTACK_YELL_STRINGS())); */
			return TStatus::SUCCESS;
		});
	}
	TChildNode random_trivial_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto lsec = car_thief_ptr(mob.uuid());
			if(!lsec->should_do(car_thief::SHOULD_DO_RANDOM_TRIVIAL)) {
				return TStatus::SUCCESS;
			}
			static constexpr uint8_t RANDOM_THINGS = 4;
			auto roll = rand_number(1,RANDOM_THINGS);
			switch(roll) {
				default:
					act("$n shifts $s weight and slowly goes back to standing still.",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 0:
					act("$n tilts $s sunglasses to see you with $s eyes.",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 1:
					act("$n scans the room slowly.",0,mob.cd(),0,0,TO_ROOM);
					break;
			}
			return TStatus::SUCCESS;
		});
	}
	/**
	 * @brief set behaviour tree
	 *
	 * @tparam TNode
	 * @tparam TArgumentType
	 * @tparam TStatus
	 *
	 * @return
	 */
	TChildNode set_behaviour_tree_to_engage() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			mg->set_behaviour_tree("car_thief_engage");
			return TStatus::SUCCESS;
		});
	}
	TChildNode set_behaviour_tree_to_pursuit() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			mg->set_behaviour_tree("car_thief_pursuit");
			return TStatus::SUCCESS;
		});
	}
	/**
	 * @brief spray dir
	 *
	 * @tparam TNode
	 * @tparam TArgumentType
	 * @tparam TStatus
	 *
	 * @return
	 */
	TChildNode spray_direction() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			mg->spray(mg->get_heading());
			return TStatus::SUCCESS;
		});
	}
	/**
	 * @brief returns success if mg can still see a target
	 *
	 * @tparam TNode
	 * @tparam TArgumentType
	 * @tparam TStatus
	 *
	 * @return
	 */
	TChildNode can_still_see_target() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			return TStatus::SUCCESS;
		});
	}

	/**
	 * @brief find targets, set heading and watching
	 *
	 * @tparam TNode
	 * @tparam TArgumentType
	 * @tparam TStatus
	 *
	 * @return
	 */
	TChildNode find_targets() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				++scores[v.direction];
				dir_players[v.direction].emplace_back(v.uuid);
			}
			int should_fire = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			if(should_fire == -1) {
				mg->set_heading(go_random_direction(mob));
				/** this HAS to be after the previous line because
				 * in the previous line we are going in a random direction
				 */
				mg->watch_directions(world[mg->room()].directions());
				return TStatus::FAILURE;
			}
			mg->watch_directions(world[mg->room()].directions());
			mg->set_heading(should_fire);
			return TStatus::SUCCESS;
		});
	}
	TChildNode find_attackers() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				++scores[v.direction];
				dir_players[v.direction].emplace_back(v.uuid);
			}
			int should_fire = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			if(should_fire == -1) {
				mg->set_heading(go_random_direction(mob));
				/** this HAS to be after the previous line because
				 * in the previous line we are going in a random direction
				 */
				mg->watch_directions(world[mg->room()].directions());
				return TStatus::FAILURE;
			}
			mg->watch_directions(world[mg->room()].directions());
			mg->set_heading(should_fire);
			return TStatus::SUCCESS;
		});
	}
	TChildNode move_toward_target() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				++scores[v.direction];
				dir_players[v.direction].emplace_back(v.uuid);
			}
			int should_fire = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			if(should_fire == -1) {
				mg->set_heading(go_random_direction(mob));
				/** this HAS to be after the previous line because
				 * in the previous line we are going in a random direction
				 */
				mg->watch_directions(world[mg->room()].directions());
				return TStatus::FAILURE;
			}
			mg->watch_directions(world[mg->room()].directions());
			mg->set_heading(should_fire);
			return TStatus::SUCCESS;
		});
	}



	TChildNode attackers_optimal_distance_away() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				++scores[v.direction];
				dir_players[v.direction].emplace_back(v.uuid);
			}
			int should_fire = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			if(should_fire == -1) {
				mg->set_heading(go_random_direction(mob));
				/** this HAS to be after the previous line because
				 * in the previous line we are going in a random direction
				 */
				mg->watch_directions(world[mg->room()].directions());
				return TStatus::FAILURE;
			}
			mg->watch_directions(world[mg->room()].directions());
			mg->set_heading(should_fire);
			return TStatus::SUCCESS;
		});
	}
	mods::behaviour_tree_node<mods::npc&>::child_node_t shotgun_attack();


	void make_car_thief_engage(TNode& tree) {
		/**
		 * SEQUENCE TREE
		 * --------------
		 * [ engage ]
		 * [A-1] roll for chance of knowing attacker
		 * 	[found person]
		 * 		[1] set heading toward direction
		 * 		[2] if not in same room, scan room
		 * 			[-] repeat until 1 room away
		 * 		[3] fire shotgun at target
		 * 	[not found]
		 * 		[1] head towards reported room
		 * 		[2] loop A-1
		 *
		 */
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_engage"),
			shotgun_attack()
		})
		);
	}

	void make_car_thief_backup(TNode& tree) {
		/**
		 * [ backup ]
		 * [A-1] scan for reported person
		 * 	[found person]
		 * 		[1] set heading toward direction
		 * 		[2] if not in same room, head to that room
		 * 			[-] repeat until inside same room
		 * 		[3] fire shotgun at target
		 * 	[not found]
		 * 		[1] head towards reported room
		 * 		[2] loop A-1
		 *
		 */
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_backup"),
			spray_direction(),
			find_targets(),
			TNode::create_sequence({
				can_still_see_target(),
				shout_random(),
			})
		})
		);
	}
	TChildNode perform_random_non_hostile_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto g = car_thief_ptr(mob.uuid());
			return TStatus::SUCCESS;
		});
	}
	TChildNode scan_to_find_hostile_activity() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto g = car_thief_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> fighting_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid)) {
					continue;
				}
				if(mods::rooms::is_peaceful(v.room_rnum)) {
					continue;
				}
				if(ptr->fighting() && ptr->fighting()->is_npc()) {
					++scores[v.direction];
					fighting_players[v.direction].emplace_back(v.uuid);
				}
			}
			int should_move = -1;
			int max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_move = pair.first;
				}
			}
			g->set_heading(should_move);
			if(should_move == -1) {
				return TStatus::FAILURE;
			}
			g->save_hostile_targets(fighting_players[should_move]);
			return TStatus::SUCCESS;
		});
	}
	TChildNode move_toward_heading() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("mps attempting move 1");
				return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("mps attempting move 2");
				switch(attempt_open(g->room(),g->get_heading())) {
					case DOOR_OPENED_SUCCESSFULLY:
						m_debug("mps opened door successfully");
						break;
					case INVALID_DIR_OPTION:
					case NOT_A_DOOR:
					case DOOR_IS_LOCKED:
					case DOOR_IS_REINFORCED:
					default:
						m_debug("mps FAILED to open door");
						return TStatus::FAILURE;
						break;
				}
				m_debug("mps attempting perform_move 2");
				return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				m_debug("mps last ditch attempt. breaching door");
				const auto& g = car_thief_ptr(mob.uuid());
				/** Attempt to breach door */
				auto obj = create_object(ITEM_EXPLOSIVE,"breach-charge.yml");
				const auto obj_uuid = obj->uuid;
				g->player()->equip(std::move(obj),WEAR_HOLD);
				perform_breach(obj_uuid,mob.uuid(),g->get_heading());
				return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			})
		});

	}
	TChildNode report_hostile_activity() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			const auto& g = car_thief_ptr(mob.uuid());
			m_debug("mps reporting hostile activity");
			mods::response_team::radio::report_violence(mob.uuid(),"Disciple 1, 810 charlie at {exact_location}");
			return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
		});
	}
	TChildNode engage_hostile() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			const auto& g = car_thief_ptr(mob.uuid());
			m_debug("mps engaging with hostile");
			g->hunt_hostile_targets();
			return TStatus::SUCCESS;
		});
	}

	void make_car_thief_roam(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_roam"),
			random_trivial_action(),
			scan_to_find_hostile_activity(),
			set_behaviour_tree_to_pursuit()
		})
		);

	}
	void make_car_thief_hostile(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_hostile"),
			move_toward_heading(),
			report_hostile_activity(),
			engage_hostile(),
			set_behaviour_tree_to_engage()
		})
		);

	}

	void make_car_thief_wimpy(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_wimpy"),
			random_trivial_action(),
			scan_to_find_hostile_activity(),
			set_behaviour_tree_to_pursuit()
		})
		);

	}
};

#undef m_debug
