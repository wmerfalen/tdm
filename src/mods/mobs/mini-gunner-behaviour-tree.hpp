#ifndef  __MENTOC_MODS_MOBS_MINI_GUNNER_BEHAVIOUR_TREE_HEADER__
#define  __MENTOC_MODS_MOBS_MINI_GUNNER_BEHAVIOUR_TREE_HEADER__

#include "mini-gunner.hpp"
#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../scan.hpp"
#include "../rooms.hpp"
#include "room-watching.hpp"
#include "../doors.hpp"
#include "../rand.hpp"
#include "helpers.hpp"
#include "../weapon.hpp"
#include "../affects.hpp"
#include "../classes/ghost.hpp"
#include "../calc-visibility.hpp"

#define __MENTOC_SHOW_BEHAVIOUR_TREE_MINI_GUNNER_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_MINI_GUNNER_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::mobs::mini_gunner_behaviour_tree][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::mobs::mini_gunner_behaviour_tree {
#define TSUCCESS TStatus::SUCCESS
#define TFAILURE TStatus::FAILURE
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;

	template <typename TNode,typename TArgumentType,typename TStatus>
	auto debug_echo_tree_name(str_t name) {
		return TNode::create_leaf([name](TArgumentType& mob) -> TStatus {
#ifdef __MENTOC_SHOW_TREE_NAME__
			std::cerr << "[debug_echo_tree_name][mob_uuid:" << mob.uuid() << "]" <<
			          green_str(CAT("[tree:'",name,"']\n"));
#endif
			return TSUCCESS;
		});
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	auto shout_where_are_you_random_string() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			mg->shout(random_key_string(MINI_GUNNER_RANDOM_DISORIENT_STRINGS()));
			return TSUCCESS;
		});
	}

	/**
	 * @brief randomly yell something
	 *
	 * @tparam TNode
	 * @tparam TArgumentType
	 * @tparam TStatus
	 *
	 * @return
	 */
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto shout_random() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			mg->shout(random_key_string(MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS()));
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto random_trivial_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			static constexpr uint8_t RANDOM_THINGS = 3;
			switch(rand_number(0,RANDOM_THINGS)) {
				case 0:
					mg->shout(random_key_string(MINI_GUNNER_RANDOM_ATTACK_YELL_STRINGS()));
					break;
				case 1:
					act("$n slams $s fist against $s chest!",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 2:
					act("$n only seems to get angrier with every passing moment...",FALSE,mob.cd(),0,0,TO_ROOM);
					break;
				case 3:
					act("$n scans the room.",0,mob.cd(),0,0,TO_ROOM);
					break;
				default:
					break;
			}
			return TSUCCESS;
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
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto set_behaviour_tree_to_engage() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			mg->set_behaviour_tree("mini_gunner_engage");
			return TSUCCESS;
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
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto spray_direction() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			mg->spray(mg->get_heading());
			return TSUCCESS;
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
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto can_still_see_target() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			return TSUCCESS;
		});
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	auto find_targets_with_compromised_line_of_sight() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			int depth = MINI_GUNNER_SCAN_DEPTH() * MINI_GUNNER_DECREASED_SIGHT_MULTIPLIER();
			m_debug("has decreased line of sight: " << depth << ". normal line of site depth:" << MINI_GUNNER_SCAN_DEPTH());
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
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto find_targets() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			int depth = MINI_GUNNER_SCAN_DEPTH();
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
				return TFAILURE;
			}
			mg->watch_directions(world[mg->room()].directions());
			mg->set_heading(should_fire);
			return TSUCCESS;
		});
	}
	/**
	 * @brief creates mini gunner roam tree
	 *
	 * @tparam TNode the node tree type
	 * @tparam TArgumentType this will most likely be mods::npc
	 * @tparam TStatus enumeration. Usually only ::SUCCESS or ::FAILURE
	 * @param tree reference parameter. the tree to decorate
	 */
	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_mini_gunner_roam(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("mini_gunner_roam"),
			find_targets<TNode,TArgumentType,TStatus>(),
			spray_direction<TNode,TArgumentType,TStatus>(),
			set_behaviour_tree_to_engage<TNode,TArgumentType,TStatus>()
		})
		);
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_mini_gunner_engage(TNode& tree) {
		/**
		 * [ engage target ]
		 * [2] spray at target
		 * [3] cooldown
		 * [4] can still see target?
		 * 	[y] -> [1]
		 * 	[n] -> [ aggressive scan ]
		 */
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("mini_gunner_engage"),
			spray_direction<TNode,TArgumentType,TStatus>(),
			find_targets<TNode,TArgumentType,TStatus>(),
			TNode::create_sequence({
				can_still_see_target<TNode,TArgumentType,TStatus>(),
				shout_random<TNode,TArgumentType,TStatus>(),
			})
		})
		);
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto scan_to_find_targets() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			int depth = MINI_GUNNER_SCAN_DEPTH();
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
			mg->set_heading(should_fire);
			if(should_fire == -1) {
				return TFAILURE;
			}
			mg->save_targets(dir_players[should_fire]);
			return TSUCCESS;
		});
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	auto watch_all_directions() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			/** if stunned */
			auto ad = mg->player()->get_affect_dissolver();
			if(ad.has_any({AFF(BLIND),AFF(DISORIENT)})) {
				mg->watch_nothing();
				mg->set_behaviour_tree("mini_gunner_disoriented");
				return TFAILURE;
			}
			mg->watch_heading();
			return TSUCCESS;
		});
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	auto contact_hq_for_camera_spots() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = mini_gunner_ptr(mob.uuid());
			/** if stunned */
			auto ad = mg->player()->get_affect_dissolver();
			if(ad.has_any({AFF(BLIND),AFF(DISORIENT)})) {
				mg->watch_nothing();
				return TFAILURE;
			}
			mg->watch_heading();
			return TSUCCESS;
		});
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_mini_gunner_aggressive_roam(TNode& tree) {
		/**
		 * [ aggressive roam ]
		 * [1] scan area
		 * [2] found targets? [success]
		 * 	[yes]
		 * 	  |-> [spray direction]
		 * 		|-> [save sprayed player uuids]
		 * 		|-> [go toward direction]
		 * 	[no]
		 * 		|-> [selector]
		 * 		|-> [check for blood trail]
		 * 		|		  |
		 * 		|		  [ found trail? ]
		 * 		|		  	[yes]
		 * 		|		  		|-> [RUN two rooms toward direction]
		 * 		|		  		|-> [scan for target]
		 * 		|		  				  |-> [ found target? ]
		 * 		|		  				  			[yes]
		 * 		|		  				  				|-> [spray direction]
		 * 		|												|-> [set behaviour tree engage]
		 * 		|												|-> return SUCCESS
		 * 		|		  				  			[no]
		 * 		|		  				  				|-> goto [check for blood trail]
		 * 		|												|-> [set behaviour tree blood trail tracking]
		 * 		|												|-> return SUCCESS
		 * 		|-> [scan for targets]
		 * 		|			|
		 * 		|			[ found? ]
		 * 		|				[yes]
		 * 		|					|-> [spray direction]
		 * 		|					|-> return SUCCESS
		 * 		|				[no]
		 * 		|					|-> return FAILURE
		 * 		|-> [mob has scanner grenade?]
		 * 		|			[yes]
		 * 		|				|-> [throw toward direction of last seen player]
		 * 		|				|			|-> [ set behaviour tree to waiting on nade results ]
		 * 		|				|-> return SUCCESS
		 * 		|
		 * 		|-> [contact hq over radio]
		 * 		|			|
		 * 		|			[ "i need eyes on sector N" ]
		 * 		|				|
		 * 		|				|-> [ dispatch scanner effect on sector ]
		 * 		|				|			|-> [ set behaviour tree to waiting on dispatch ]
		 * 		|				|-> return SUCCESS
		 * 		|-> [intelligent guess]
		 * 		|			|
		 * 		|			[sequence]
		 * 		|			|->[ cheat and find each player in the zone ]
		 * 		|			|->[ get x,y,z of players ]
		 * 		|			|->[ calculate closest player based on coords ]
		 * 		|			|->[ move that direction ]
		 * 		|			|->[ scan for players ]
		 * 		|			|->[ found players? ]
		 * 		|				[yes]
		 * 		|					|->[ spray direction ]
		 * 		|					|->[ set behaviour tree engage ]
		 * 		|					|-> return SUCCESS
		 * 		|				[no]
		 * 		|					|->return FAILURE
		 * 		|-> [shuffle order of tree]
		 */
		tree.append_child(
		TNode::create_selector({
			TNode::create_sequence({
				debug_echo_tree_name<TNode,TArgumentType,TStatus>("mini_gunner_aggressive_roam"),
				scan_to_find_targets<TNode,TArgumentType,TStatus>(),
				watch_all_directions<TNode,TArgumentType,TStatus>(),
				spray_direction<TNode,TArgumentType,TStatus>(),
			})
		})
		);
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_mini_gunner_disoriented(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("disoriented"),
			shout_where_are_you_random_string<TNode,TArgumentType,TStatus>(),
			find_targets_with_compromised_line_of_sight<TNode,TArgumentType,TStatus>(),
			spray_direction<TNode,TArgumentType,TStatus>(),
			set_behaviour_tree_to_engage<TNode,TArgumentType,TStatus>()
		})
		);
	}
};

#undef m_debug
#endif
