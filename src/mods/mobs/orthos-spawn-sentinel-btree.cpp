#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "orthos-spawn-sentinel.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_ORTHOS_SPAWN_SENTINEL_BTREE_DEBUG_OUTPUT__
#include "../behaviour_tree_impl.hpp"
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

#define  __MENTOC_SHOW_BEHAVIOUR_TREE_ORTHOS_SPAWN_SENTINEL_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_ORTHOS_SPAWN_SENTINEL_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[m.m.oss.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

#define m_report(a) std::cerr << "[m.m.oss.btree:" << __LINE__ << "{REPORT}]->" << a << "\n";

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);


namespace mods::mobs::orthos_spawn_sentinel_btree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = mods::npc&;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;

	using node = mods::behaviour_tree_impl::node;
	using node_type = mods::behaviour_tree_impl::node::node_type_t;

	TChildNode shotgun_attack() {
		return TNode::create_leaf([](mods::npc& mob) -> TStatus {
			orthos_spawn_sentinel_ptr(mob.uuid())->shotgun_attack_within_range();
			return TStatus::SUCCESS;
		});
	}




	TChildNode debug_echo_tree_name(std::string n) {
		static std::string name;
		name = n;
		return TNode::create_leaf([&](mods::npc& mob) -> TStatus {
			auto g = orthos_spawn_sentinel_ptr(mob.uuid());
			m_report("running tree name: " << name.c_str());
			return TStatus::SUCCESS;
		});
	}



	TChildNode shout_random() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			/** TODO: replace this mg->shout(random_key_string(LOWLY_SECURITY_RANDOM_ATTACK_YELL_STRINGS())); */
			return TStatus::SUCCESS;
		});
	}



	TChildNode random_trivial_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto lsec = orthos_spawn_sentinel_ptr(mob.uuid());
			if(!lsec->should_do(orthos_spawn_sentinel::SHOULD_DO_RANDOM_TRIVIAL)) {
				m_debug("should_do returns false");
				return TStatus::SUCCESS;
			}
			static constexpr uint8_t RANDOM_THINGS = 4;
			auto roll = rand_number(1,RANDOM_THINGS);
			switch(roll) {
				default:
					act("$n shifts $s weight and slowly goes back to standing still.",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 2:
					act("$n tilts $s sunglasses to see you with $s eyes.",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 3:
					act("$n scans the room slowly.",0,mob.cd(),0,0,TO_ROOM);
					break;
			}
			return TStatus::SUCCESS;
		});
	}


	TChildNode set_behaviour_tree_to_engage() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			mg->set_behaviour_tree("orthos_sentinel_engage");
			return TStatus::SUCCESS;
		});
	}






	TChildNode set_behaviour_tree_to_pursuit() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			mg->set_behaviour_tree("orthos_sentinel_pursuit");
			return TStatus::SUCCESS;
		});
	}





	TChildNode trigger_guard_neutralize() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			mg->act_to_room(CAT("$n places $s index finger on the trigger guard of his ",mg->primary()->name.c_str(),"."));
			return TStatus::SUCCESS;
		});
	}


	TChildNode can_still_see_target() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			return TStatus::SUCCESS;
		});
	}



	TChildNode find_targets() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid, v.distance)) {
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
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid, v.distance)) {
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
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid,v.distance)) {
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
			auto mg = orthos_spawn_sentinel_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid,v.distance)) {
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



	void make_sentinel_discipline(TNode& tree) {
		/**
		 * Orthos Sentinel places his index finger on the trigger guard of his [primary].
		 */
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("orthos_sentinel_discipline"),
			trigger_guard_neutralize(),
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
			auto g = orthos_spawn_sentinel_ptr(mob.uuid());
			return TStatus::SUCCESS;
		});
	}




	TChildNode scan_to_find_hostile_activity() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto g = orthos_spawn_sentinel_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> fighting_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mob.uuid(),v.uuid,v.distance)) {
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
				const auto& g = orthos_spawn_sentinel_ptr(mob.uuid());
				m_debug("mps attempting move 1");
				return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = orthos_spawn_sentinel_ptr(mob.uuid());
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
				const auto& g = orthos_spawn_sentinel_ptr(mob.uuid());
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
			const auto& g = orthos_spawn_sentinel_ptr(mob.uuid());
			m_debug("reporting hostile activity");
			mods::response_team::radio::report_violence(mob.uuid(),"Disciple 1, 810 charlie at {exact_location}");
			return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
		});
	}





	TChildNode engage_hostile() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			const auto& g = orthos_spawn_sentinel_ptr(mob.uuid());
			m_debug("engaging with hostile");
			g->hunt_hostile_targets();
			return TStatus::SUCCESS;
		});
	}




	void make_sentinel_hostile(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("orthos_sentinel_hostile"),
			move_toward_heading(),
			engage_hostile(),
			set_behaviour_tree_to_engage()
		})
		);
	}




	void make_sentinel_roam(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name("orthos_sentinel_roam"),
			random_trivial_action(),
			scan_to_find_hostile_activity(),
			set_behaviour_tree_to_pursuit()
		})
		);
	}



	std::map<std::string,node&> get_trees() {
		m_debug("get_trees()");

		static node sentinel_roam(node_type::SELECTOR);
		static node sentinel_hostile(node_type::SELECTOR);
		static node sentinel_witness_hunting(node_type::SELECTOR);
		static node sentinel_wimpy(node_type::SELECTOR);
		static node sentinel_discipline(node_type::SELECTOR);

		make_sentinel_roam(sentinel_roam);
		make_sentinel_hostile(sentinel_hostile);
		make_sentinel_discipline(sentinel_discipline);

		m_debug("returning trees");

		return {
			{"orthos_sentinel_roam",sentinel_roam},
			{"orthos_sentinel_hostile",sentinel_hostile},
			{"orthos_sentinel_discipline",sentinel_discipline},
		};

	}

};

#undef m_debug
#undef m_report
