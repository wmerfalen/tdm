#include "behaviour_tree_impl.hpp"
#include "weapon.hpp"
#include <string>
#include "../spells.h"
extern void set_fighting(char_data *ch, char_data *vict);
extern void remember(char_data*,char_data*);
extern void hit(char_data *ch, char_data *victim, int type);
extern int snipe_hit(player_ptr_t&,obj_ptr_t,player_ptr_t&, uint16_t* distance);

namespace mods::behaviour_tree_impl {
	container_t trees;
	container_mapping_t tree_mapping;
	//mob_list_t mobs_with_trees;
	void run_trees(){
		std::cerr << "[behaviour_tree][run_trees] -- STUB\n";
	}
	int8_t dispatch(argument_type& ch){
		std::cerr << "dispatching behaviour tree on: " << ch.name().c_str() << "\n";
		if(ch.mob_specials().behaviour_tree == 0){
			return dispatch_status_t::AS_YOU_WERE;
		}
		if(trees.size() > static_cast<std::size_t>(ch.mob_specials().behaviour_tree)){
			auto btree_status = trees[ch.mob_specials().behaviour_tree].run(ch);
			switch(btree_status.status){
				case mods::behaviour_tree_status::SUCCESS:
					return mods::behaviour_tree_impl::dispatch_status_t::RETURN_IMMEDIATELY;
				default:
					return mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE;
			}
		}
		return mods::behaviour_tree_impl::dispatch_status_t::AS_YOU_WERE;
	}
	int8_t register_mob(argument_type mob,std::string tree_name){
		auto it = tree_mapping.find(tree_name.data());
		if(it == tree_mapping.end()){
			return -1;
		}
		std::cerr << "registering mob '" << tree_name.data() << "'\n";
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
		trees.emplace_back(sv_tree_name.data(),n);
		tree_mapping[sv_tree_name.data()] = trees.end()-1;
	}
	void load_trees(){
		/**
		 * tracking down snipers 
		 */
		using node_type = node::node_type_t;
		node do_nothing(node_type::DO_NOTHING);
		node snipe_tracking(node_type::SELECTOR);
		node sentinel_snipe_tracking(node_type::SELECTOR);
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
						/** If we've made it to this node, that means the mob does
						 * indeed have the snipe capability. Let's attempt to snipe
						 * the player who last sniped us.
						 */
						for(auto & remembered_sniper : mob.mob_specials().memory){
							if(mob.can_snipe(remembered_sniper)){
							/*
int snipe_hit(*ch, char_data *victim, int type,uint16_t distance) {
	struct obj_data *wielded = GET_EQ(ch, WEAR_WIELD);
	*/
							/** TODO: if no ammo, search for ammo */
							/** TODO: */
								auto find_results = mods::scan::los_find(std::make_shared<mods::player>(mob),std::make_shared<mods::player>(remembered_sniper));
								auto mob_ptr = ptr(mob);
								auto remembered_sniper_ptr = ptr(remembered_sniper);
								snipe_hit(mob_ptr,mob_ptr->primary(),remembered_sniper_ptr,&find_results.distance);
								return status::SUCCESS;
							}
						}
						return status::FAILURE;
					}
				)
			})
		);
		snipe_tracking.append_child(node::create_sequence({
			node::create_leaf(
				[](argument_type mob) -> status{
				auto find_results = mods::scan::los_find(
						std::make_shared<mods::player>(mob),
						std::make_shared<mods::player>(mob.mob_specials().snipe_tracking)
						);
				if(find_results.found){
				/**
				 * Move find_results.dinstance steps toward the player
				 */
				for(decltype(find_results.distance) steps = find_results.distance; 
					steps > 0; --steps){
					/**
					 * The 'direction' member will tell us where we need to go.
					 */
					perform_move(mob,find_results.direction,0);
					if(mob.room() == IN_ROOM(mob.mob_specials().snipe_tracking)){
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
				unsigned ctr = 0;
				for(auto & memorized_attacker : mob.mob_specials().memory){
				std::cerr << "memorized attacker--iter[" << ++ctr << "\n";
					if(mob.room() == IN_ROOM(memorized_attacker)){
						//act("$n recognizes ", FALSE, mob, 0, 0, TO_ROOM);
					//std::cerr << "[recognize]: " << mob.name() << "->" << mods::player(memorized_attacker).name().c_str() << "\n";
						set_fighting(mob,memorized_attacker);
						hit(mob, memorized_attacker, TYPE_UNDEFINED);
						/**
						 * If the memorized_attacker is dead, we can forget and return a success.
						 * But if not, we need to continue attacking until that attacker is dead
						 * or until the mob dies
						 */
						return status::SUCCESS;
					}else{
						return status::FAILURE;
					}
				}
				return status::FAILURE;
			})
			})/** End create_sequence */
		);// end append_child
		add_tree("do_nothing",do_nothing);
		add_tree("snipe_tracking",snipe_tracking);
		add_tree("sentinel_snipe_tracking",sentinel_snipe_tracking);

		/**
		 * Suspicious roaming tree. 
		 */
		node suspicious_roaming(node_type::SELECTOR);
		suspicious_roaming.append_child(node::create_selector({ })
		);
		add_tree("suspicious_roaming",suspicious_roaming);
	}

};
