#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "../behaviour_tree_impl.hpp"
#include "generic-thief.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_DEBUG_OUTPUT__
#define m_debug(a)  std::cerr << "[m.m.generic-thief.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_STATUS_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_STATUS_OUTPUT__
#define m_status(a)  mentoc_prefix_debug("[m.m.generic-thief.btree]:") << "[STATUS]->" << a << "\n";
#else
#define m_status(a)
#endif

#define  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_ERROR_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_generic_thief_BTREE_ERROR_OUTPUT__
#define m_error(a)  mentoc_prefix_debug("[m.m.generic-thief.btree]:") << "[ERROR]->" << a << "\n";
#else
#define m_error(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::behaviour_tree_impl {
	extern void add_tree(std::string sv_tree_name,node& n);
};

namespace mods::mobs::generic_thief_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = std::shared_ptr<mods::mobs::generic_thief>;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;
	using GenericThiefPointer = std::shared_ptr<mods::mobs::generic_thief>;


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
		return TNode::create_leaf([](TArgumentType ct) -> TStatus {
			static constexpr int SCAN_DEPTH = 8;
			int depth = SCAN_DEPTH;
			vec_player_data vpd; mods::scan::los_scan_for_players(ct->cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(ct->uuid,v.uuid,v.distance)) {
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
				ct->watch_directions(world[ct->room()].directions());
				return TStatus::FAILURE;
			}
			ct->watch_directions(world[ct->room()].directions());
			ct->set_heading(should_fire);
			ct->move_to(ct->get_heading());
			ct->melee_attack_within_range();
			return TStatus::SUCCESS;
		});
	}
	TChildNode find_attackers() {
		return TNode::create_leaf([](TArgumentType mg) -> TStatus {
			int depth = LOWLY_SECURITY_SCAN_DEPTH();
			vec_player_data vpd; mods::scan::los_scan_for_players(mg->cd(),depth,&vpd);
			std::map<int,int> scores;
			std::map<uint8_t,uuidvec_t> dir_players;
			for(auto v : vpd) {
				auto ptr = ptr_by_uuid(v.uuid);
				if(!ptr) {
					continue;
				}
				if(!mods::calc_visibility::is_visible(mg->uuid,v.uuid, v.distance)) {
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
			auto primary = mg->player()->primary();
			auto secondary = mg->player()->secondary();
			if(!primary && !secondary) {
				m_error("has no primary!");
				return TStatus::FAILURE;
			}
			obj_ptr_t attacking_with = primary;
			if(!primary && secondary) {
				attacking_with = secondary;
			}
			if(!attacking_with) {
				m_error(" has no weapon!");
				return TStatus::FAILURE;
			}
			if(mods::object_utils::is_rifle(attacking_with)) {
				if(should_fire == -1) {
					mg->watch_directions(world[mg->room()].directions());
					return TStatus::FAILURE;
				}
				mg->watch_directions(world[mg->room()].directions());
				mg->set_heading(should_fire);
				mg->rifle_attack_within_range();
				return TStatus::SUCCESS;
			}
			if(mods::object_utils::is_melee(attacking_with)) {
				mg->watch_directions(world[mg->room()].directions());
				mg->set_heading(should_fire);
				mg->move_to(mg->get_heading());
				mg->melee_attack_within_range();
				return TStatus::SUCCESS;
			}
			return TStatus::FAILURE;
		});
	}
	TChildNode attack_if_possible() {
		return TNode::create_leaf([](TArgumentType ct) -> TStatus {
			if(ct->melee_attack_within_range()) {
				return TStatus::SUCCESS;
			}
			if(ct->rifle_attack_within_range()) {
				return TStatus::SUCCESS;
			}
			return TStatus::FAILURE;
		});
	}

	TChildNode increment_hostile_force() {
		return TNode::create_leaf([](TArgumentType ct) -> TStatus {
			ct->increment_last_attack_significance();
			return TStatus::SUCCESS;
		});
	}



	void make_generic_thief_engage(TNode& tree) {
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
			TNode::create_leaf([](TArgumentType ct) -> TStatus {
				ct->set_watching_room(true);
				return TStatus::SUCCESS;
			})
		});
	}

	/**
	 * [1] Have we already found an item of interest?
	 * 	[found item of interest]
	 * 		[1] return SUCCESS
	 * 		[2] go toward heading
	 * 			return failure until in same room as item of interest
	 * [2] in same room as item of interest
	 */
	TChildNode find_target_near_me() {
		return TNode::create_sequence({
			TNode::create_leaf([](TArgumentType ct) -> TStatus {
				int SCAN_DEPTH = 8;
				if(ct->has_found_item()) {
					return TStatus::SUCCESS; // start moving that way
				}
				auto ch = ct->cd();
				mods::scan::vec_player_data los_room_list;
				auto dirs =  world[ct->room()].directions();
				mods::util::shuffle(dirs);
				for(const auto& dir : dirs) {
					los_scan_direction(ch,SCAN_DEPTH,&los_room_list,dir, mods::scan::find_type_t::OBJECTS);
					for(const auto& scanned : los_room_list) {
						auto obj = optr_by_uuid(scanned.uuid);
						if(obj->has_vehicle()) { // TODO: std::get<0>(mods::calc_visibility::can_see_object(ct->player(),obj)) && obj->has_vehicle()) {
							ct->found_item(scanned);
							ct->remember_item(scanned);
							ct->set_heading(dir);
							break;
						}
					}
				}
				ct->move_to(ct->get_heading());
				return TStatus::SUCCESS;
			}),
			/**
			 * check to see if we're in the same room as a car we spotted.
			 * if not, move towards heading
			 */
			TNode::create_leaf([](TArgumentType ct) -> TStatus {
				using namespace mods::doors;
				for(const auto& car_uuid : ct->get_remembered_items()) {
					if(mods::query_objects::room_has_object_uuid(ct->room(),car_uuid)) {
						ct->set_found_item(true);
						return TStatus::SUCCESS;
					}
				}
				m_status("havent found car yet, moving toward");
				ct->set_found_item(false);
				ct->move_to(ct->get_heading());
				return TStatus::FAILURE;
			}),
		});//end create_sequence
	}//end find_target_near_me

	std::deque<TNode>& get_trees() {
		static TNode generic_thief_roam(TNode::SELECTOR);
		static TNode generic_thief_hostile(TNode::SELECTOR);
		static TNode generic_thief_witness_hunting(TNode::SELECTOR);
		static TNode generic_thief_wimpy(TNode::SELECTOR);
		static TNode generic_thief_attempt_thievery(TNode::SELECTOR);
		static bool bootstrapped = false;
		static std::deque<TNode> s;
		if(!bootstrapped) {
			/**
			 * Thief roaming behaviour
			 * --- TODO
			 * [1]
			 *
			 *
			 *
			 */
			generic_thief_roam.append_child(
			TNode::create_selector({
				find_attackers(),
				//find_target_near_me(),
				//watch_room(),
			})
			);
			generic_thief_hostile.append_child(
			TNode::create_sequence({
				find_attackers(),
				attack_if_possible(),
				increment_hostile_force()
			})
			);
			generic_thief_wimpy.append_child(
			TNode::create_sequence({
			})
			);
			generic_thief_attempt_thievery.append_child(
			TNode::create_sequence({
				find_target_near_me(),
				//watch_room(),
				//get_hostile_toward_witnesses()
			})
			);
			s = {
				generic_thief_roam,
				generic_thief_hostile,
				generic_thief_witness_hunting,
				generic_thief_wimpy,
				generic_thief_attempt_thievery,
			};
			bootstrapped = true;
		}
		return s;
	}

	void run_trees() {
		std::vector<GenericThiefPointer> removals;
		for(auto& thief : generic_thief_list()) {
			if(thief->has_tree() && !thief->alive()) {
				m_debug("DEAD-WITH_TREE -- thief has tree but is dead. Removing...");
				thief->btree_none();
				removals.emplace_back(thief);
				continue;
			}
			m_debug("checking thief ptr");
			if(thief->has_tree() && thief->alive() && thief->capable()) {
				m_debug("has tree. dispatching..." << thief->get_tree());
				get_trees()[thief->get_tree()].run(thief);
			}
		}
		generic_thief_list().erase(std::remove_if(generic_thief_list().begin(),
		                                          generic_thief_list().end(),
		[&removals](const auto& thief) -> bool {
			return std::find(removals.cbegin(),removals.cend(),thief) != removals.cend();
		}),
		generic_thief_list().end()
		                          );
	}

};

#undef m_debug
