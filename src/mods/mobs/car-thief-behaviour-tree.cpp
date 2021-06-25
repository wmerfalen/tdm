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
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_car_thief_BTREE_DEBUG_OUTPUT__
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

	TChildNode melee_attack() {
		return TNode::create_leaf([](mods::npc& mob) -> TStatus {
			car_thief_ptr(mob.uuid())->melee_attack_within_range();
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
			auto ct = car_thief_ptr(mob.uuid());
			int depth = CAR_THIEF_SCAN_DEPTH();
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
				ct->set_heading(go_random_direction(mob));
				/** this HAS to be after the previous line because
				 * in the previous line we are going in a random direction
				 */
				ct->watch_directions(world[ct->room()].directions());
				return TStatus::FAILURE;
			}
			ct->watch_directions(world[ct->room()].directions());
			ct->set_heading(should_fire);
			return TStatus::SUCCESS;
		});
	}
	TChildNode set_behaviour_tree_to_hostile() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto ct = car_thief_ptr(mob.uuid());
			ct->set_behaviour_tree("car_thief_hostile");
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
			melee_attack()
		})
		);
	}

	TChildNode scan_for_witness() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("scanning for witnesses...");
				return TStatus::SUCCESS;
				//return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = car_thief_ptr(mob.uuid());
				return TStatus::SUCCESS;
			}),
		});
	}

	TChildNode find_vehicle_near_me() {
		return TNode::create_sequence({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				if(mods::query_objects::room_has_vehicle(mob.room())) {
					return TStatus::SUCCESS;
				}

				auto ch = mob.cd();
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("finding vehicle near me");
				mods::scan::vec_player_data los_room_list;
				if(mob.room() >= world.size()) {
					m_debug("mob room invalid");
					return TStatus::FAILURE;
				}
				for(const auto& dir : world[mob.room()].directions()) {
					m_debug("direction: " << dirstr(dir));
					los_scan_direction(ch,5,&los_room_list,dir, mods::scan::find_type_t::OBJECTS);
					m_debug("scanned direction. found " << los_room_list.size() << " entries...");
					for(const auto& scanned : los_room_list) {
						m_debug("checking if has vehicle: " << scanned.uuid);
						auto obj = optr_by_uuid(scanned.uuid);
						if(obj->has_vehicle()) { // TODO: std::get<0>(mods::calc_visibility::can_see_object(g->player(),obj)) && obj->has_vehicle()) {
							g->found_vehicle(scanned);
						}
					}
				}
				auto dir = g->determine_heading_from_found_vehicles();
				g->clear_scanned_cars();
				if(dir == -1) {
					m_debug("Couldnt decide on direction. moving randomly");
					/** go in mob roam direction */
					auto m = mods::rand::shuffle_container(world[mob.room()].directions());
					for(const auto& dir : m) {
						if(mods::mob_roam::can_roam_to(ch,EXIT(ch,dir)->to_room)) {
							g->set_heading(dir);
							g->move_to(dir);
							return TStatus::FAILURE;
						}
					}
				}
				g->move_to(dir);
				g->set_heading(dir);
				return TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = car_thief_ptr(mob.uuid());
				//g->shout("WHAT THE FUUUUUUUUUUUUUUCK");
				return TStatus::SUCCESS;
			}),
		});
	}

	TChildNode look_suspicious() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("look suspicious");
				return TStatus::SUCCESS;
				//return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = car_thief_ptr(mob.uuid());
				return TStatus::SUCCESS;
			}),
		});
	}

	TChildNode get_hostile_toward_witnesses() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("get hostile toward witnesses");
				return TStatus::SUCCESS;
				//return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = car_thief_ptr(mob.uuid());
				return TStatus::SUCCESS;
			}),
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
			})
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
			find_vehicle_near_me()
			//move_toward_heading(),
			//look_suspicious(),
			//scan_for_witness(),
			//get_hostile_toward_witnesses()
		})
		);

	}
	void make_car_thief_hostile(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_hostile"),
		})
		);

	}

	void make_car_thief_wimpy(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("car_thief_wimpy"),
		})
		);

	}
};

#undef m_debug
