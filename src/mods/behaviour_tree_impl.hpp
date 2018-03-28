#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__

#include <vector>
#include "behaviour_tree.hpp"

namespace mods {
	namespace behaviour_tree_impl { 
		typedef char_data* argument_type;
		typedef std::vector<behaviour_tree<argument_type>> container;
		using node = mods::behaviour_tree_node<argument_type>;
		using node_wrapper = mods::behaviour_tree<argument_type>;
		using status_t = node::run_status_t;
		using status = mods::behaviour_tree_status;
		void dispatch(argument_type ch);
		void load_trees();
		extern container list;
		constexpr int snipe_tracking = 0;
		constexpr int NONE = node_wrapper::NONE;
	};
};

#endif
