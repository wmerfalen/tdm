#include "behaviour_tree_impl.hpp"

namespace mods::behaviour_tree_impl {
	container_t trees;
	mob_list_t mobs_with_trees;
	void run_trees(){
		for(auto &mob : mobs_with_trees){
			dispatch(mob);
		}
	}
	void dispatch(argument_type ch){
		if(ch->mob_specials.behaviour_tree == node_wrapper::NONE){
			return;
		}
		if(trees.size() > static_cast<std::size_t>(ch->mob_specials.behaviour_tree)){
			trees[ch->mob_specials.behaviour_tree].run(ch);
		}
		return;
	}
	void register_mob(const argument_type & mob,const type & tree){
		mob_list_t::const_iterator found_at;
		if(!mods::util::exists<decltype(mobs_with_trees),decltype(mob)>(
					&mobs_with_trees,
					mob,
					found_at)){
			(mob)->char_specials.saved.act |= mob_has_tree;
			mob->mob_specials.behaviour_tree = tree;
			mobs_with_trees.push_back(mob);
		}else{
			(*found_at)->char_specials.saved.act |= mob_has_tree;
			(*found_at)->mob_specials.behaviour_tree = tree;
		}
	}

	void unregister_mob(const argument_type & mob){
		std::remove_if(mobs_with_trees.begin(),
			mobs_with_trees.end(),
			[mob](const argument_type & supplied_mob){
				return mob == supplied_mob;
			}
		);
	}
	void load_trees(){
		/**
		 * tracking down snipers 
		 */
		node snipe_tracking(node::node_type_t::SELECTOR);
		snipe_tracking.append_child(node::create_sequence({
			node::create_leaf(
				[](argument_type mob) -> status{
				/** 
				 * Scan all directions. If the player can be seen, 
				 * then move there.
				 */
				if(mob->mob_specials.snipe_tracking == nullptr){
					return status::FAILURE;
				}
				auto find_results = mods::scan::los_find(mob,mob->mob_specials.snipe_tracking);
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
					}
					return status::SUCCESS;
				}
				return status::FAILURE;
			}),
			node::create_leaf(
				[](argument_type mob) -> status{
				if(IN_ROOM(mob->mob_specials.snipe_tracking)){
					act("$n attempts to drag you down by your equipment.", FALSE, mob, 0, 0, TO_ROOM);
					hit(mob, mob->mob_specials.snipe_tracking, TYPE_UNDEFINED);
					unregister_mob(mob);
					return status::SUCCESS;
				}else{
					return status::FAILURE;
				}
			})
			})/** End create_sequence */
		);
		trees.emplace_back("snipe-tracking",snipe_tracking);

		/**
		 * Suspicious roaming tree. 
		 */
		node suspicious_roaming(node::node_type_t::SELECTOR);
		suspicious_roaming.append_child(node::create_selector({

			})
		);
		trees.emplace_back("suspicious-roaming",suspicious_roaming);
	}

};
