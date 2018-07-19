#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__

#include <iterator>
#include <vector>
#include "behaviour_tree.hpp"
#ifdef __MENTOC_NO_DEPS__
#else
#include "scan.hpp"
#include "util.hpp"
#endif

namespace mods {
	namespace behaviour_tree_impl { 
#ifdef __MENTOC_NO_DEPS__
		template <typename T> 
		typedef T argument_type;
#else
		typedef char_data* argument_type;
#endif
		typedef std::vector<behaviour_tree<argument_type>> container_t;
		typedef std::vector<argument_type> mob_list_t;
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
#ifdef __MENTOC_NO_DEPS__
		bool inline flagged(const argument_type &);
		bool inline flagged(const argument_type &,const type&);
#else
		bool inline flagged(const argument_type & mob){
			return mob->char_specials.saved.act & mob_has_tree;
		}
		bool inline flagged(const argument_type & mob,const type & type){
			return mob->char_specials.saved.act & mob_has_tree && 
				mob->mob_specials.behaviour_tree == type;
		}
#endif
	};
};


#endif
