#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__

#include <vector>
#include "behaviour_tree.hpp"

namespace mods {
	extern std::vector<behaviour_tree<char_data*>> behaviour_tree_list;
	struct behaviour_tree_impl {
		typedef char_data* argument_type;
		using node = mods::behaviour_tree_node<argument_type>;
		using node_wrapper = mods::behaviour_tree<argument_type>;
		using status_t = node::run_status_t;
		using status = mods::behaviour_tree_status;
		static void dispatch(argument_type ch){
			if(ch->mob_specials.behaviour_tree == node_wrapper::NONE){
				return;
			}
			if(behaviour_tree_list.size() > static_cast<std::size_t>(ch->mob_specials.behaviour_tree)){
				behaviour_tree_list[ch->mob_specials.behaviour_tree].run(ch);
			}
			return;
		}
		static void load_trees(){
			/** tracking down snipers */
			node snipe_tracking(node::node_type_t::SELECTOR);
			snipe_tracking.append_child(node::create_selector({
					node::create_leaf(
						[](argument_type mob) -> status{
							std::cerr << "leaf selector 1";
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
			behaviour_tree_list.emplace_back("snipe-tracking",snipe_tracking);
		}
	};
};

#endif
