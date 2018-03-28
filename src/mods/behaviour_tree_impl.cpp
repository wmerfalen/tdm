#include "behaviour_tree_impl.hpp"

namespace mods::behaviour_tree_impl {
	container list;
	void dispatch(argument_type ch){
		if(ch->mob_specials.behaviour_tree == node_wrapper::NONE){
			return;
		}
		if(list.size() > static_cast<std::size_t>(ch->mob_specials.behaviour_tree)){
			list[ch->mob_specials.behaviour_tree].run(ch);
		}
		return;
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
		list.emplace_back("snipe-tracking",snipe_tracking);
	}
};
