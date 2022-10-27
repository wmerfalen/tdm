#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "../behaviour_tree_impl.hpp"
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

namespace mods::behaviour_tree_impl {
	extern void add_tree(std::string sv_tree_name,node& n);
};

namespace mods::mobs::car_thief_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = mods::npc&;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;


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
			//report(ct);
			int depth = CAR_THIEF_SCAN_DEPTH();
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
	TChildNode find_attackers() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = car_thief_ptr(mob.uuid());
			//report(mg);
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
		})
		);
	}

	TChildNode watch_room() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("watching room for witnesses...");
				g->set_watching_room(true);
				return TStatus::SUCCESS;
				//return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			})
		});
	}

	/**
	 * [1] Have we already found a car?
	 * 	[found car]
	 * 		[1] return SUCCESS
	 * 		[2] go toward heading
	 * 			return failure until in same room as car
	 * [2] in same room as car
	 */
	TChildNode find_vehicle_near_me() {
		return TNode::create_sequence({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = car_thief_ptr(mob.uuid());
				m_debug("has found car check");
				if(g->has_found_car()) {
					m_debug("start moving that way");
					return TStatus::SUCCESS; // start moving that way
				}
				auto ch = mob.cd();
				m_debug("finding vehicle near me");
				mods::scan::vec_player_data los_room_list;
				if(mob.room() >= world.size()) {
					return TStatus::FAILURE;
				}
				auto dirs =  world[mob.room()].directions();
				mods::util::shuffle(dirs);
				for(const auto& dir : dirs) {
					m_debug("direction: " << dirstr(dir));
					los_scan_direction(ch,CAR_THIEF_SCAN_DEPTH(),&los_room_list,dir, mods::scan::find_type_t::OBJECTS);
					m_debug("scanned direction. found " << los_room_list.size() << " entries...");
					for(const auto& scanned : los_room_list) {
						m_debug("checking if has vehicle: " << scanned.uuid);
						auto obj = optr_by_uuid(scanned.uuid);
						if(obj->has_vehicle()) { // TODO: std::get<0>(mods::calc_visibility::can_see_object(g->player(),obj)) && obj->has_vehicle()) {
							g->found_vehicle(scanned);
							g->remember_car(scanned);
							g->set_heading(dir);
							break;
						}
					}
				}
				g->move_to(g->get_heading());
				return TStatus::SUCCESS;
			}),
			/**
			 * check to see if we're in the same room as a car we spotted.
			 * if not, move towards heading
			 */
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				auto c = car_thief_ptr(mob.uuid());
				for(const auto& car_uuid : c->get_remembered_cars()) {
					if(mods::query_objects::room_has_object_uuid(mob.room(),car_uuid)) {
						m_debug("found car");
						c->set_found_car(true);
						return TStatus::SUCCESS;
					}
				}
				m_debug("havent found car yet, moving toward");
				c->set_found_car(false);
				c->move_to(c->get_heading());
				return TStatus::FAILURE;
			}),
		});//end create_sequence
	}//end find_vehicle_near_me

	void make_car_thief_roam(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			/**
			 * find vehicle will successfully execute all of it's
			 * nodes when it has scanned for, found, walked toward,
			 * and successfully made it to the same room as a car.
			 */
			find_vehicle_near_me(),
			watch_room(),
			//get_hostile_toward_witnesses()
		})
		);

	}
	void make_car_thief_hostile(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
		})
		);

	}

	void make_car_thief_wimpy(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
		})
		);

	}
	using namespace mods::behaviour_tree_impl;
	using node_type = node::node_type_t;

	std::map<std::string,node&> get_trees() {
		static node car_thief_roam(node_type::SELECTOR);
		static node car_thief_hostile(node_type::SELECTOR);
		static node car_thief_witness_hunting(node_type::SELECTOR);
		static node car_thief_wimpy(node_type::SELECTOR);
		make_car_thief_roam(car_thief_roam);
		make_car_thief_hostile(car_thief_hostile);
		make_car_thief_wimpy(car_thief_wimpy);
		return {
			{"car_thief_roam",car_thief_roam},
			{"car_thief_hostile",car_thief_hostile},
			{"car_thief_wimpy",car_thief_wimpy},
		};
	}

};

#undef m_debug
