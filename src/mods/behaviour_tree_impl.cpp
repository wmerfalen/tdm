#include "behaviour_tree_impl.hpp"
#include "weapon.hpp"
#include <string>
#include "../spells.h"
#include "weapons/damage-types.hpp"
#include "scan.hpp"
#include "../mobs/mini-gunner.hpp"
extern void set_fighting(char_data *ch, char_data *vict);
extern void remember(char_data*,char_data*);
extern void hit(char_data *ch, char_data *victim, int type);
#define bti_debug(a) std::cerr << "[mods::behaviour_tree_imp::dispatch][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";

namespace mods::behaviour_tree_impl {
	using vec_player_data = mods::scan::vec_player_data;
	container_t trees;
	container_mapping_t tree_mapping;
	//mob_list_t mobs_with_trees;
	void run_trees(){
		std::cerr << "[behaviour_tree][run_trees] -- STUB\n";
	}
	int8_t dispatch(uuid_t mob_uuid){
		auto it = mods::globals::mob_map.find(mob_uuid);
		if(it == mods::globals::mob_map.end()){
			bti_debug("uuid doesn't have mob ptr value: " << mob_uuid);
			return mods::behaviour_tree_impl::dispatch_status_t::MOB_DOESNT_EXIST;
		}
		return dispatch_ptr(*(it->second));
	}
	int8_t dispatch_ptr(argument_type& ch){
		std::cerr << "dispatching behaviour tree on: " << ch.name().c_str() << "\n";
		if(ch.mob_specials().behaviour_tree == 0){
			bti_debug("As you were... ");
			return dispatch_status_t::AS_YOU_WERE;
		}
		if(ch.mob_specials().behaviour_tree){
			bti_debug("mob has this behaviour_tree:" << ch.mob_specials().behaviour_tree);
			auto btree_status = trees[ch.mob_specials().behaviour_tree].run(ch);
			switch(btree_status.status){
				case mods::behaviour_tree_status::SUCCESS:
					bti_debug("Return success...");
					return mods::behaviour_tree_impl::dispatch_status_t::RETURN_IMMEDIATELY;
				default:
					bti_debug("defaulted value from dispatch: " << btree_status.status << " (processing as 'as you were')");
					return mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE;
			}
		}
		return mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE;
	}
	int8_t register_mob(argument_type mob,std::string tree_name){
		auto it = tree_mapping.find(tree_name);
		if(it == tree_mapping.end()){
			return -1;
		}
		std::cerr << "registering mob '" << tree_name << "'\n";
		mob.mob_specials().behaviour_tree = std::distance(tree_mapping.begin(),it);
		return 0;
	}

	int8_t unregister_mob(argument_type mob){
		mob.mob_specials().behaviour_tree = 0;
		return 0;
	}
	uint8_t grab_tree_by_name(const std::string& sv_tree){
		return std::distance(trees.begin(),tree_mapping[sv_tree.data()]);
	}
	void add_tree(std::string sv_tree_name,node & n){
		trees.emplace_back(sv_tree_name,n);
		tree_mapping[sv_tree_name] = trees.end()-1;
		bti_debug("Added tree mapping for name: '" << sv_tree_name << "'");
	}
	void load_trees(){
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
		auto node_mob_has_snipe_capability = node::create_leaf(
				[](argument_type mob) -> status {
					if(mob.has_weapon_capability(mods::weapon::mask::snipe)){
						std::cerr << "[node_mob_has_snipe_capability]-> mob has snipe\n";
						return status::SUCCESS;
					}else{
						std::cerr << "[node_mob_has_snipe_capability]-> mob DOEST NOT have snipe\n";
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
						for(auto & remembered_sniper_uuid : mob.mob_specials().memory){
							bti_debug("for loop");
							auto remembered_sniper = ptr_by_uuid(remembered_sniper_uuid);
							if(!remembered_sniper){
								bti_debug("Can't find remembered sniper by uuid: " << remembered_sniper_uuid);
								mob.mob_specials().memory.erase(remembered_sniper_uuid);
								continue;
							}
							bti_debug("checking if Can snipe remembered sniper");
							if(mob.can_snipe(remembered_sniper)){
								bti_debug("Can snipe remembered sniper");
							/*
int snipe_hit(*ch, char_data *victim, int type,uint16_t distance) {
	struct obj_data *wielded = GET_EQ(ch, WEAR_WIELD);
	*/
							/** TODO: if no ammo, search for ammo */
							/** TODO: */
								auto find_results = mods::scan::los_find(std::make_shared<mods::player>(mob),remembered_sniper);
								auto mob_ptr = ptr(mob);
								mods::weapons::damage_types::rifle_attack(mob_ptr,mob_ptr->primary(),remembered_sniper,find_results.distance);
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
				if(!target.has_value()){
					bti_debug("snipe_tracking tree has invalid uuid value:" << mob.cd()->mob_specials.snipe_tracking);
					mob.cd()->mob_specials.snipe_tracking = 0;
					return status::SUCCESS;
				}
				auto find_results = mods::scan::los_find(std::make_shared<mods::player>(mob),target.value());
				if(find_results.found){
					bti_debug("snipe_tracking tree: FOUND OUR TARGET! (snipe_tracking ptr is: " << mob.cd()->mob_specials.snipe_tracking << ")");
				/**
				 * Move find_results.dinstance steps toward the player
				 */
				for(decltype(find_results.distance) steps = find_results.distance; 
					steps > 0; --steps){
					/**
					 * The 'direction' member will tell us where we need to go.
					 */
					perform_move(mob,find_results.direction,0);
					auto target = ptr_opt(mob.mob_specials().snipe_tracking);
					if(!target.has_value()){
						mob.mob_specials().snipe_tracking = 0;
						return status::SUCCESS;
					}
					if(mob.room() == target.value()->room()){
						return status::SUCCESS;
					}else{
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
				for(auto & memorized_attacker_uuid : mob.mob_specials().memory){
					auto memorized_attacker_opt = ptr_opt(memorized_attacker_uuid);
					if(!memorized_attacker_opt.has_value()){
						std::cerr << "[memorized_attacker] grabbing by uuid failed! uuid points to nullptr: " << memorized_attacker_uuid << "\n";
						erase_these.push_back(memorized_attacker_uuid);
						continue;
					}
					auto memorized_attacker = memorized_attacker_opt.value();
					if(mob.room() == memorized_attacker->room()){
						act("$n recognizes ", FALSE, mob, 0, 0, TO_ROOM);
						std::cerr << "[recognize]: " << mob.name() << "->" << memorized_attacker->name().c_str() << "\n";
						set_fighting(mob,memorized_attacker->cd());
						hit(mob, memorized_attacker->cd(), TYPE_UNDEFINED);
						/**
						 * If the memorized_attacker is dead, we can forget and return a success.
						 * But if not, we need to continue attacking until that attacker is dead
						 * or until the mob dies
						 */
						ret_value = status::SUCCESS;
						break;
					}else{
						ret_value = status::FAILURE;
						break;
					}
				}
				for(auto erase_me : erase_these){
					mob.mob_specials().memory.erase(erase_me);
				}
				return ret_value;
			})
			})/** End create_sequence */
		);// end append_child

		/**
		 *
		 * [ default roam ]
		 * [1] roam
		 * [2] look down hallway
		 * [3] see target?
		 * 	[y] -> engage -> [ engage target ]
		 * 	[n] -> roam -> [1]
		 *
		 * [ aggressive scan ]
		 * [1] 
		 *
		 * [ engage target ]
		 * [0] save uuid of player
		 * [1] aim (consume N ticks)
		 * [2] spray at target
		 * 	[2.1] deal damage to target
		 * 	[2.2] continue spray
		 * 		[2.2.1] roll chance of post-shot hit?
		 * 			[2.2.1.1] deal damage
		 * 	[2.3] cooldown
		 * [3] can still see target?
		 * 	[y] -> [1]
		 * 	[n] -> [ aggressive scan ]
		 * 
		 * 
		 */
		mini_gunner_roam.append_child(node::create_sequence({
				/** scan for targets
				 * success means: we found a target and engaged them, don't move, we're fine
				 * failure means: move to another room to see if you can find another player
				 */
				node::create_leaf([](argument_type mob) -> status {
						int depth = MINI_GUNNER_SCAN_DEPTH(); /** TODO calculate depth with default mods::values + buffs */
						vec_player_data vpd;
						mods::scan::los_scan_for_players(mob.cd(),depth,&vpd);
						if(vpd.size() == 0){
							return status::SUCCESS;
						}
						std::map<int,int> scores;
						for(auto v : vpd){
							if(!ptr_by_uuid(v.uuid)){
								continue;
							}
							++scores[v.direction];
						}
						int should_fire = -1;
						int max = 0;
						for(auto pair : scores){
							if(pair.second > max){
								max = pair.second;
								should_fire = pair.first;
							}
						}
						if(should_fire != -1){
							std::cerr << "[should_fire][" << dirstr(should_fire) << "\n";
							auto mg = mini_gunner_ptr(mob.uuid());
							mg->spray(should_fire);
							return status::FAILURE;
						}
							return status::SUCCESS;
					}
				),
					/** move in heading direction
					 * success means: scan for players to kill
					 */
				node::create_leaf([](argument_type mob) -> status {
						int dir = mob.cd()->mob_specials.heading;
						if(CAN_GO(mob.cd(),dir)){
							mob.cd()->mob_specials.previous_room = IN_ROOM(mob.cd());
							char_from_room(mob.cd());
							char_to_room(mob.cd(),world[IN_ROOM(mob.cd())].dir_option[dir]->to_room);
						}
						return status::SUCCESS;
				})
		}));
		mini_gunner_engage.append_child(node::create_selector({
				node::create_leaf([](argument_type mob) -> status {
						return status::SUCCESS;
				}),
				node::create_leaf([](argument_type mob) -> status {
						return status::SUCCESS;
						}
				)
				})
		);
		mini_gunner_aggressive_roam.append_child(node::create_selector({
				node::create_leaf([](argument_type mob) -> status {
						return status::SUCCESS;
				}),
				node::create_leaf([](argument_type mob) -> status {
						return status::SUCCESS;
						}
				)
				})
		);
		add_tree("do_nothing",do_nothing);
		add_tree("snipe_tracking",snipe_tracking);
		add_tree("sentinel_snipe_tracking",sentinel_snipe_tracking);
		add_tree("mini_gunner_roam",mini_gunner_roam);
		add_tree("mini_gunner_engage",mini_gunner_engage);
		add_tree("mini_gunner_aggressive_roam",mini_gunner_aggressive_roam);

		/**
		 * Suspicious roaming tree. 
		 */
		node suspicious_roaming(node_type::SELECTOR);
		suspicious_roaming.append_child(node::create_selector({ })
		);
		add_tree("suspicious_roaming",suspicious_roaming);
	}

};
