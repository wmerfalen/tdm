#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "../behaviour_tree_impl.hpp"
#include "shoplifter.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_shoplifter_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_shoplifter_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[m.m.shoplifter.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::behaviour_tree_impl {
	extern void add_tree(std::string sv_tree_name,node& n);
};

namespace mods::mobs::shoplifter_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = std::shared_ptr<mods::mobs::shoplifter>;
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
		return TNode::create_leaf([](TArgumentType ct) -> TStatus {
			static constexpr int SCAN_DEPTH = 8;
			int depth = SCAN_DEPTH;
			vec_player_data vpd; mods::scan::los_scan_for_players(ct->cd(),depth,&vpd);
			std::map<direction_t,uint8_t> scores;
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
			int8_t should_fire = -1;
			uint8_t max = 0;
			for(auto pair : scores) {
				if(pair.second > max) {
					max = pair.second;
					should_fire = pair.first;
				}
			}
			if(should_fire == -1) {
				//FIXME ct->set_heading(go_random_direction(ct->player()));
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
				if(!mods::calc_visibility::is_visible(mg->uuid,v.uuid,v.distance)) {
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
				//FIXME mg->set_heading(go_random_direction(mg->player()));
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

	void make_shoplifter_engage(TNode& tree) {
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
			TNode::create_leaf([](TArgumentType g) -> TStatus {
				m_debug("watching room for witnesses...");
				g->set_watching_room(true);
				return TStatus::SUCCESS;
				//return perform_move(g->cd(), g->get_heading(),0) ? TStatus::SUCCESS : TStatus::FAILURE;
			})
		});
	}

	TChildNode find_someone_to_attack() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType g) -> TStatus {
				if(g->attack_anyone_near_room()) {
					return TStatus::SUCCESS;
				}
				if(g->has_found_item()) {
					m_debug("start moving that way");
					return TStatus::SUCCESS; // start moving that way
				}
				m_debug("finding npcs/players near me to attack");
				g->clear_state();
				for(const auto& dir : g->shuffle_directions()) {
					m_debug("direction: " << dirstr(dir));
					auto los_room_list = g->scan_attackable(dir);
					m_debug("scanned direction. found " << los_room_list.size() << " entries...");
					for(const auto& scanned : los_room_list) {
						g->found_item(scanned);
						g->remember_item(scanned);
						g->set_heading(dir);
						g->move_to(g->get_heading());
						return TStatus::SUCCESS;
					}
				}
				return TStatus::FAILURE;
			}),
			TNode::create_leaf([](TArgumentType c) -> TStatus {
				using namespace mods::doors;
				for(const auto& car_uuid : c->get_remembered_items()) {
					if(mods::query_objects::room_has_object_uuid(c->room(),car_uuid)) {
						m_debug("found car");
						c->set_found_item(true);
						return TStatus::SUCCESS;
					}
				}
				m_debug("havent found car yet, moving toward");
				c->set_found_item(false);
				c->move_to(c->get_heading());
				return TStatus::FAILURE;
			}),
		});//end create_sequence
	}//end find_target_near_me

	TChildNode random_action() {
		//return TNode::create_sequence({
		return TNode::create_leaf([](TArgumentType g) -> TStatus {
			m_debug("performing random chaotic action");
			auto list = mods::globals::get_room_list(g->room());
			if(list.size() > 1 && rand_number(0,10) > rand_number(0,10)) {
				g->perform_random_act();
			}
			if(g->player()->fighting()) {
				if(mods::rand::chance(CHAOTIC_DOUBLE_ATTACK_CHANCE())) {
					g->extra_attack();
					return TStatus::SUCCESS;
				}
			}
			return TStatus::SUCCESS;
		});
		//});//end create_sequence
	}//end find_target_near_me

	std::deque<TNode>& trees() {
		static TNode shoplifter_roam(TNode::SELECTOR);
		static TNode shoplifter_hostile(TNode::SELECTOR);
		static bool bootstrapped = false;
		static std::deque<TNode> s;
		if(!bootstrapped) {
			m_debug("bootstrapping");
			shoplifter_roam.append_child(
			TNode::create_selector({
				find_someone_to_attack(),
				random_action(),
				//move_toward_existing_target(),
				//perform_hostile_action(),
			})
			);
			shoplifter_hostile.append_child(
			TNode::create_sequence({
			})
			);
			s = {
				shoplifter_roam,
				shoplifter_hostile,
			};
			bootstrapped = true;
		}
		return s;
	}

	void run_trees() {
		m_debug("run trees");
		for(auto& shoplifter : shoplifter_list()) {
			m_debug("checking shoplifter ptr");
			if(shoplifter->has_tree() && shoplifter->alive() && shoplifter->capable()) {
				m_debug("has tree. dispatching..." << shoplifter->get_tree());
				trees()[shoplifter->get_tree()].run(shoplifter);
			}
		}
	}

};

#undef m_debug
