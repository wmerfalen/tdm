#include "behaviour_tree_impl.hpp"
#include <iterator>

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
	template <typename ContainerType,typename FindType,typename IteratorType>
	bool exists(const ContainerType * container_ptr, 
			const FindType & find_me,
			IteratorType & found_at ){
		return (
				found_at = 
				std::find(
					container_ptr->begin(),
					container_ptr->end(),
					find_me
				)
			)!= container_ptr->end();
	}
	void register_mob(const argument_type & mob,const type & tree){
		mob_list_t::const_iterator found_at;
		if(!exists<decltype(mobs_with_trees),decltype(mob),decltype(found_at)>(
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
		/** tracking down snipers */
		node snipe_tracking(node::node_type_t::SELECTOR);
		snipe_tracking.append_child(node::create_sequence({
				node::create_leaf(
					[](argument_type mob) -> status{
						std::cerr << "scanning for attacker";
						return status::FAILURE;
					}
				),
				node::create_leaf(
					[](argument_type mob) -> status{
						std::cerr << "leaf selector 2";
						return status::SUCCESS;
					}
				),
				node::create_leaf(
					[](argument_type mob) -> status{
						std::cerr << "leaf selector 3";
						return status::FAILURE;
					}
				)
			})
		);
		trees.emplace_back("snipe-tracking",snipe_tracking);
	}
};
