#include "behaviour_tree_impl.hpp"
#include "weapon.hpp"
#include <string>
#include "../spells.h"
#include "weapons/damage-types.hpp"
#include "scan.hpp"
#include "mobs/mini-gunner.hpp"
#include "rand.hpp"
#include "rooms.hpp"
#include "mobs/room-watching.hpp"
#include "mobs/mini-gunner-behaviour-tree.hpp"
#include "mobs/lowly-security-behaviour-tree.hpp"
#include "mobs/mp-shotgunner-behaviour-tree.hpp"
#include "mobs/car-thief-behaviour-tree.hpp"
#include "mobs/orthos-spawn-sentinel-btree.hpp"

extern void set_fighting(char_data *ch, char_data *vict);
extern void remember(char_data*,char_data*);
extern void hit(char_data *ch, char_data *victim, int type);
#ifdef __MENTOC_SHOW_BEHAVIOUR_TREE_IMPL_DEBUG_OUTPUT__
#define bti_debug(a) std::cerr << "[mods::behaviour_tree_imp::dispatch][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define bti_debug(a)
#endif
namespace mods::mobs::car_thief_behaviour_tree {
	extern std::map<std::string,mods::behaviour_tree_impl::node&> get_trees();
};
namespace mods::mobs::orthos_spawn_sentinel_btree {
	extern std::map<std::string,mods::behaviour_tree_impl::node&> get_trees();
};
namespace mods::mobs::generic_thief_behaviour_tree {
	extern std::map<std::string,mods::behaviour_tree_impl::node&> get_trees();
};
namespace mods::behaviour_tree_impl {
	using vec_player_data = mods::scan::vec_player_data;
	container_t trees;
	container_mapping_t tree_mapping;
	//mob_list_t mobs_with_trees;
	void run_trees() {
		bti_debug("[behaviour_tree][run_trees] -- STUB\n");
	}
	int8_t dispatch(uuid_t mob_uuid) {
		auto it = npc_by_uuid(mob_uuid);
		if(!it) {
			bti_debug("uuid doesn't have mob ptr value: " << mob_uuid);
			return mods::behaviour_tree_impl::dispatch_status_t::MOB_DOESNT_EXIST;
		}
		return dispatch_ptr(*it);
	}
	int8_t dispatch_ptr(argument_type& ch) {
		//switch(ch.mob_specials().extended_mob_type) {
		//	case mob_special_data::extended_mob_type_t::GENERIC_THIEF:
		//	case mob_special_data::extended_mob_type_t::CHAOTIC_METH_ADDICT:
		//		return dispatch_status_t::AS_YOU_WERE;
		//	default:
		//		break;
		//}
		bti_debug("dispatching behaviour tree on: " << ch.name().c_str());
		if(ch.mob_specials().behaviour_tree == 0) {
			bti_debug("As you were... ");
			return dispatch_status_t::AS_YOU_WERE;
		}
		if(ch.mob_specials().behaviour_tree >= trees.size()) {
			bti_debug(red_str(CAT("invalid behaviour tree index:",std::to_string(ch.mob_specials().behaviour_tree))));
			//log("SYSERR: behaviour tree out of range: %d. not running",ch.mob_specials().behaviour_tree);
			return dispatch_status_t::AS_YOU_WERE;
		}
		bti_debug("mob has this behaviour_tree:" << ch.mob_specials().behaviour_tree);
		if(ch.mob_specials().behaviour_tree < trees.size()) {
			auto btree_status = trees[ch.mob_specials().behaviour_tree].run(ch);
			switch(btree_status.status) {
				case mods::behaviour_tree_status::SUCCESS:
					bti_debug("Return success...");
					return mods::behaviour_tree_impl::dispatch_status_t::RETURN_IMMEDIATELY;
				default:
					break;
			}
		}
		return mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE;
	}
	int8_t register_mob(argument_type mob,std::string tree_name) {
		auto it = tree_mapping.find(tree_name);
		if(it == tree_mapping.end()) {
			return -1;
		}
		bti_debug("registering mob '" << tree_name);
		mob.mob_specials().behaviour_tree = std::distance(tree_mapping.begin(),it);
		mods::behaviour_tree_impl::mob_list().emplace_back(mob.uuid());
		return 0;
	}
	int8_t unregister_by_uuid(const uuid_t& m_uuid) {
		std::remove_if(mods::behaviour_tree_impl::mob_list().begin(),
		               mods::behaviour_tree_impl::mob_list().end(),[m_uuid](const auto & u) -> bool { return u == m_uuid; }
		              );
		return 0;
	}
	int8_t unregister_mob(argument_type mob) {
		mob.mob_specials().behaviour_tree = 0;
		auto m_uuid = mob.uuid();
		std::remove_if(mods::behaviour_tree_impl::mob_list().begin(),
		               mods::behaviour_tree_impl::mob_list().end(),[m_uuid](const auto & u) -> bool { return u == m_uuid; }
		              );
		return 0;
	}
	uint8_t grab_tree_by_name(const std::string& sv_tree) {
		return std::distance(trees.begin(),tree_mapping[sv_tree.data()]);
	}
	void add_tree(std::string sv_tree_name,node& n) {
		trees.emplace_back(sv_tree_name,n);
		tree_mapping[sv_tree_name] = trees.end()-1;
		bti_debug("Added tree mapping for name: '" << sv_tree_name << "'");
	}
	void load_trees() {
		/**
		 * tracking down snipers
		 */
		using node_type = node::node_type_t;
		node do_nothing(node_type::DO_NOTHING);
		node snipe_tracking(node_type::SELECTOR);
		node sentinel_snipe_tracking(node_type::SELECTOR);
		node mini_gunner_roam(node_type::SELECTOR);
		node mini_gunner_engage(node_type::SELECTOR);
		node mini_gunner_aggressive_roam(node_type::SELECTOR);
		node mini_gunner_disoriented(node_type::SELECTOR);
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		node mp_shotgunner(node_type::SELECTOR);
		node mp_shotgunner_engage(node_type::SEQUENCE);
		node mp_shotgunner_backup(node_type::SELECTOR);
		node lowly_security(node_type::SELECTOR);
		node lowly_security_roam(node_type::SELECTOR);
		node lowly_security_engage(node_type::SELECTOR);
		node lowly_security_pursuit(node_type::SELECTOR);
		mods::mobs::mini_gunner_behaviour_tree::make_mini_gunner_roam<node,argument_type,status>(mini_gunner_roam);
		mods::mobs::mini_gunner_behaviour_tree::make_mini_gunner_engage<node,argument_type,status>(mini_gunner_engage);
		mods::mobs::mini_gunner_behaviour_tree::make_mini_gunner_aggressive_roam<node,argument_type,status>(mini_gunner_aggressive_roam);
		mods::mobs::mini_gunner_behaviour_tree::make_mini_gunner_disoriented<node,argument_type,status>(mini_gunner_disoriented);
		mods::mobs::lowly_security_behaviour_tree::make_lowly_security<node,argument_type,status>(lowly_security);
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		mods::mobs::mp_shotgunner_behaviour_tree::make_mp_shotgunner(mp_shotgunner);
		mods::mobs::mp_shotgunner_behaviour_tree::make_mp_shotgunner_backup(mp_shotgunner_backup);
		mods::mobs::mp_shotgunner_behaviour_tree::make_mp_shotgunner_engage(mp_shotgunner_engage);
		mods::mobs::lowly_security_behaviour_tree::make_lowly_security_roam<node,argument_type,status>(lowly_security_roam);
		mods::mobs::lowly_security_behaviour_tree::make_lowly_security_engage<node,argument_type,status>(lowly_security_engage);
		mods::mobs::lowly_security_behaviour_tree::make_lowly_security_pursuit<node,argument_type,status>(lowly_security_pursuit);

		auto node_mob_has_snipe_capability = node::create_leaf(
		[](argument_type mob) -> status {
			if(mob.has_weapon_capability(mods::weapon::mask::snipe)) {
				bti_debug("[node_mob_has_snipe_capability]-> mob has snipe");
				return status::SUCCESS;
			} else {
				bti_debug("[node_mob_has_snipe_capability]-> mob DOEST NOT have snipe");
				return status::FAILURE;
			}
		}
		                                     );
		/**
		 * We are a sentinel npc. We do not move from the room which we are
		 * stationed in. Instead, we remember who sniped us for
		 * N seconds. If anyone on that list of snipers enters the
		 * room where we are stationed, we attack them with the same
		 * 'remember' string ("Hey! You're the <> that attacked me!")
		 */
		sentinel_snipe_tracking.append_child(node::create_sequence({
			node_mob_has_snipe_capability,
			node::create_leaf(
			[](argument_type mob) -> status {
				bti_debug("before... for loop");
				/** If we've made it to this node, that means the mob does
				 * indeed have the snipe capability. Let's attempt to snipe
				 * the player who last sniped us.
				 */
				for(auto& remembered_sniper_uuid : mob.mob_specials().memory) {
					bti_debug("for loop");
					auto remembered_sniper = ptr_by_uuid(remembered_sniper_uuid);
					if(!remembered_sniper) {
						bti_debug("Can't find remembered sniper by uuid: " << remembered_sniper_uuid);
						mob.mob_specials().memory.erase(remembered_sniper_uuid);
						continue;
					}
					bti_debug("checking if Can snipe remembered sniper");
					if(mob.can_snipe(remembered_sniper)) {
						bti_debug("Can snipe remembered sniper");
						/*
						int snipe_hit(*ch, char_data *victim, int type,uint16_t distance) {
						struct obj_data *wielded = GET_EQ(ch, WEAR_WIELD);
						*/
						/** TODO: if no ammo, search for ammo */
						/** TODO: */
						auto find_results = mods::scan::los_find(std::make_shared<mods::player>(mob),remembered_sniper);
						auto mob_ptr = ptr(mob);
						mods::weapons::damage_types::rifle_attack(mob_ptr,mob_ptr->primary(),remembered_sniper,find_results.distance,find_results.direction);
						return status::SUCCESS;
					}
				}
				bti_debug("returning FAILURE from sentinel_snipe_tracking");
				return status::FAILURE;
			}
			)
		})
		                                    );
		snipe_tracking.append_child(node::create_sequence({
			node::create_leaf(
			[](argument_type mob) -> status{
				bti_debug("snipe_tracking tree: Looking for target (snipe_tracking ptr is: " << mob.cd()->mob_specials.snipe_tracking << ")");
				auto target = ptr_opt(mob.cd()->mob_specials.snipe_tracking);
				if(!target.has_value()) {
					bti_debug("snipe_tracking tree has invalid uuid value:" << mob.cd()->mob_specials.snipe_tracking);
					mob.cd()->mob_specials.snipe_tracking = 0;
					return status::SUCCESS;
				}
				auto find_results = mods::scan::los_find(std::make_shared<mods::player>(mob),target.value());
				if(find_results.found) {
					bti_debug("snipe_tracking tree: FOUND OUR TARGET! (snipe_tracking ptr is: " << mob.cd()->mob_specials.snipe_tracking << ")");
					/**
					 * Move find_results.dinstance steps toward the player
					 */
					for(decltype(find_results.distance) steps = find_results.distance;
					        steps > 0; --steps) {
						/**
						 * The 'direction' member will tell us where we need to go.
						 */
						perform_move(mob,find_results.direction,0);
						auto target = ptr_opt(mob.mob_specials().snipe_tracking);
						if(!target.has_value()) {
							mob.mob_specials().snipe_tracking = 0;
							return status::SUCCESS;
						}
						if(mob.room() == target.value()->room()) {
							return status::SUCCESS;
						} else {
							return status::FAILURE;
						}
					}
				}
				return status::FAILURE;
			}),
			node::create_leaf(
			[](argument_type mob) -> status{

				status ret_value = status::SUCCESS;
				std::vector<uuid_t> erase_these;
				for(auto& memorized_attacker_uuid : mob.mob_specials().memory) {
					auto memorized_attacker_opt = ptr_opt(memorized_attacker_uuid);
					if(!memorized_attacker_opt.has_value()) {
						bti_debug("[memorized_attacker] grabbing by uuid failed! uuid points to nullptr: " << memorized_attacker_uuid);
						erase_these.push_back(memorized_attacker_uuid);
						continue;
					}
					auto memorized_attacker = memorized_attacker_opt.value();
					if(mob.room() == memorized_attacker->room()) {
						act(CAT("$n recognizes ",memorized_attacker->name()), 0, mob, 0, 0, TO_ROOM);
						bti_debug("[recognize]: " << mob.name() << "->" << memorized_attacker->name().c_str());
						set_fighting(mob,memorized_attacker->cd());
						hit(mob, memorized_attacker->cd(), TYPE_UNDEFINED);
						/**
						 * If the memorized_attacker is dead, we can forget and return a success.
						 * But if not, we need to continue attacking until that attacker is dead
						 * or until the mob dies
						 */
						ret_value = status::SUCCESS;
						break;
					} else {
						ret_value = status::FAILURE;
						break;
					}
				}
				for(auto erase_me : erase_these) {
					mob.mob_specials().memory.erase(erase_me);
				}
				return ret_value;
			})
		})/** End create_sequence */
		                           );// end append_child

		add_tree("do_nothing",do_nothing);
		add_tree("snipe_tracking",snipe_tracking);
		add_tree("sentinel_snipe_tracking",sentinel_snipe_tracking);
		add_tree("mini_gunner_roam",mini_gunner_roam);
		add_tree("mini_gunner_engage",mini_gunner_engage);
		add_tree("mini_gunner_aggressive_roam",mini_gunner_aggressive_roam);
		add_tree("mini_gunner_disoriented",mini_gunner_disoriented);
		add_tree("lowly_security",lowly_security);
		add_tree("lowly_security_roam",lowly_security_roam);
		add_tree("lowly_security_engage",lowly_security_engage);
		add_tree("lowly_security_pursuit",lowly_security_pursuit);
		/**! @NEW_BEHAVIOUR_TREE@ !**/
		add_tree("mp_shotgunner",mp_shotgunner);
		add_tree("mp_shotgunner_engage",mp_shotgunner_engage);
		/**
		 * Suspicious roaming tree.
		 */
		node suspicious_roaming(node_type::SELECTOR);
		suspicious_roaming.append_child(node::create_selector({
			node::create_leaf([](argument_type mob) -> status {
				return status::SUCCESS;
			})
		}));
		add_tree("suspicious_roaming",suspicious_roaming);

		for(const auto& collection : {
		            mods::mobs::car_thief_behaviour_tree::get_trees(),
		            mods::mobs::orthos_spawn_sentinel_btree::get_trees(),
		            //mods::mobs::generic_thief_behaviour_tree::get_trees(),
		        }) {
			for(const auto& pair : collection) {
				std::cerr << "adding tree: " << pair.first << "\n";
				add_tree(pair.first,pair.second);
			}
		}
	}
	std::vector<uuid_t>& mob_list() {
		static std::vector<uuid_t> list;
		return list;
	}
	/** use this when you just need to add the mob to the vector */
	void register_mob(uuid_t u) {
		mods::behaviour_tree_impl::mob_list().emplace_back(u);
	}

	void unregister_mob(uuid_t m_uuid) {
		std::remove_if(mods::behaviour_tree_impl::mob_list().begin(),
		               mods::behaviour_tree_impl::mob_list().end(),[m_uuid](const auto & u) -> bool { return u == m_uuid; }
		              );
	}

};
