#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__

#include <iterator>
#include <vector>
#include "behaviour_tree.hpp"
#include "scan.hpp"
#include "util.hpp"

namespace mods {
	namespace behaviour_tree_impl { 
		typedef char_data* argument_type;
		typedef std::vector<behaviour_tree<argument_type>> container_t;
		typedef std::vector<char_data*> mob_list_t;
		using node = mods::behaviour_tree_node<argument_type>;
		using node_wrapper = mods::behaviour_tree<argument_type>;
		using status_t = node::run_status_t;
		using status = mods::behaviour_tree_status;
		enum type { 
			NONE = node_wrapper::NONE,
			snipe_tracking = 0,
			suspicious_roaming,
		};
		void dispatch(argument_type ch);
		void load_trees();
		void run_trees();
		void register_mob(const argument_type &,const type &);
		void unregister_mob(const argument_type &);
		constexpr int mob_has_tree = ((1 << 19));
#ifndef TYPE_UNDEFINED
		#define TYPE_UNDEFINED -1
#endif
		bool inline flagged(const argument_type & mob){
			return mob->char_specials.saved.act & mob_has_tree;
		}
		bool inline flagged(const argument_type & mob,const type & type){
			return mob->char_specials.saved.act & mob_has_tree && 
				mob->mob_specials.behaviour_tree == type;
		}
	};
};


#endif
