#ifndef  __MENTOC_MODS_MOBS_LOWLY_SECURITY_BEHAVIOUR_TREE_HEADER__
#define  __MENTOC_MODS_MOBS_LOWLY_SECURITY_BEHAVIOUR_TREE_HEADER__

#include "lowly-security.hpp"
#include "../radio.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_LOWLY_SECURITY_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_LOWLY_SECURITY_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::mobs::lowly_security_behaviour_tree][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::mobs::lowly_security_behaviour_tree {
#define TSUCCESS TStatus::SUCCESS
#define TFAILURE TStatus::FAILURE
	/**
	 * Lowly Security Guard behaviour
	 * ------------------------------
	    [1] always scanning. scan in each direction for suspicious behaviour
	    [2] if violence against store owner, fire hk45 at player
				- report disturbance at current position over radio
	    [3] always attempt to get close to player to tackle
	    	- if in same room as player, tackle
	    	- if tackle fails, fire hk45 at player
	    [4] if target moves GIVEUP_N rooms away, give up chasing
			[5] if target

	    [events]
	    	[blinded]
					[1] move in random direction
					[2] fire blindly
				[wimpy]
					[1] grab radio and call for backup
					[2] spray twice at target
				[disoriented]
					[1] move in random direction
				[forced prone]
					[1] stand up
					[2] move in random direction

	 */
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
	auto shout_random() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = lowly_security_ptr(mob.uuid());
			/** TODO: replace this mg->shout(random_key_string(LOWLY_SECURITY_RANDOM_ATTACK_YELL_STRINGS())); */
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto random_trivial_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto lsec = lowly_security_ptr(mob.uuid());
			if(!lsec->should_do(lowly_security::SHOULD_DO_RANDOM_TRIVIAL)) {
				return TSUCCESS;
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
			auto mg = lowly_security_ptr(mob.uuid());
			mg->set_behaviour_tree("lowly_security_engage");
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto set_behaviour_tree_to_pursuit() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = lowly_security_ptr(mob.uuid());
			mg->set_behaviour_tree("lowly_security_pursuit");
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
			auto mg = lowly_security_ptr(mob.uuid());
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
			auto mg = lowly_security_ptr(mob.uuid());
			return TSUCCESS;
		});
	}

#if 0
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto find_targets_with_compromised_line_of_sight() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto mg = lowly_security_ptr(mob.uuid());
			int depth = LOWLY_SECURITY_SCAN_DEPTH() * LOWLY_SECURITY_DECREASED_SIGHT_MULTIPLIER();
			m_debug("has decreased line of sight: " << depth << ". normal line of site depth:" << LOWLY_SECURITY_SCAN_DEPTH());
			return TStatus::SUCCESS;
		});
	}
#endif

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
			auto mg = lowly_security_ptr(mob.uuid());
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
	void make_lowly_security_roam(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("lowly_security_roam"),
			find_targets<TNode,TArgumentType,TStatus>(),
			spray_direction<TNode,TArgumentType,TStatus>(),
			set_behaviour_tree_to_engage<TNode,TArgumentType,TStatus>()
		})
		);
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_lowly_security_engage(TNode& tree) {
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
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("lowly_security_engage"),
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
	auto perform_random_non_hostile_action() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto g = lowly_security_ptr(mob.uuid());
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto scan_to_find_hostile_activity() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			auto g = lowly_security_ptr(mob.uuid());
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
				return TFAILURE;
			}
			g->save_hostile_targets(fighting_players[should_move]);
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto move_toward_heading() {
		return TNode::create_selector({
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				const auto& g = lowly_security_ptr(mob.uuid());
				m_debug("lsg attempting move 1");
				return perform_move(g->cd(), g->get_heading(),0) ? TSUCCESS : TFAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				const auto& g = lowly_security_ptr(mob.uuid());
				m_debug("lsg attempting move 2");
				switch(attempt_open(g->room(),g->get_heading())) {
					case DOOR_OPENED_SUCCESSFULLY:
						m_debug("lsg opened door successfully");
						break;
					case INVALID_DIR_OPTION:
					case NOT_A_DOOR:
					case DOOR_IS_LOCKED:
					case DOOR_IS_REINFORCED:
					default:
						m_debug("lsg FAILED to open door");
						return TFAILURE;
						break;
				}
				m_debug("lsg attempting perform_move 2");
				return perform_move(g->cd(), g->get_heading(),0) ? TSUCCESS : TFAILURE;
			}),
			TNode::create_leaf([](TArgumentType& mob) -> TStatus {
				using namespace mods::doors;
				m_debug("lsg last ditch attempt. breaching door");
				const auto& g = lowly_security_ptr(mob.uuid());
				/** Attempt to breach door */
				auto obj = create_object(ITEM_EXPLOSIVE,"breach-charge.yml");
				const auto obj_uuid = obj->uuid;
				g->player()->equip(std::move(obj),WEAR_HOLD);
				perform_breach(obj_uuid,mob.uuid(),g->get_heading());
				return perform_move(g->cd(), g->get_heading(),0) ? TSUCCESS : TFAILURE;
			})
		});

	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto report_hostile_activity() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			const auto& g = lowly_security_ptr(mob.uuid());
			m_debug("lsg reporting hostile activity");
			mods::response_team::radio::report_violence(mob.uuid(),"Disciple 1, 810 charlie at {exact_location}");
			return perform_move(g->cd(), g->get_heading(),0) ? TSUCCESS : TFAILURE;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	auto engage_hostile() {
		return TNode::create_leaf([](TArgumentType& mob) -> TStatus {
			const auto& g = lowly_security_ptr(mob.uuid());
			m_debug("lsg engaging with hostile");
			g->hunt_hostile_targets();
			return TSUCCESS;
		});
	}
	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_lowly_security_pursuit(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("losec_pursuit"),
			move_toward_heading<TNode,TArgumentType,TStatus>(),
			report_hostile_activity<TNode,TArgumentType,TStatus>(),
			engage_hostile<TNode,TArgumentType,TStatus>(),
			set_behaviour_tree_to_engage<TNode,TArgumentType,TStatus>()
		})
		);
	}

	template <typename TNode,typename TArgumentType,typename TStatus>
	void make_lowly_security(TNode& tree) {
		tree.append_child(
		TNode::create_sequence({
			debug_echo_tree_name<TNode,TArgumentType,TStatus>("losec_default"),
			random_trivial_action<TNode,TArgumentType,TStatus>(),
			scan_to_find_hostile_activity<TNode,TArgumentType,TStatus>(),
			set_behaviour_tree_to_pursuit<TNode,TArgumentType,TStatus>()
		})
		);
	}
};

#undef m_debug
#endif
