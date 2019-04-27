#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_IMPL_HEADER__

#include <iterator>
#include <vector>
#include <map>
#include "behaviour_tree.hpp"
#include "npc.hpp"
#ifdef __MENTOC_NO_DEPS__
#else
#include "scan.hpp"
#include "util.hpp"
#endif

namespace mods {
	namespace behaviour_tree_impl { 
#ifdef __MENTOC_NO_DEPS__
		template <typename T> 
		using argument_type = T;
#else
		using argument_type = mods::npc;
#endif
		using container_t = std::vector<behaviour_tree<argument_type>>;
		using container_mapping_t = std::map<std::string,container_t::iterator>;
		using mob_list_t = std::vector<argument_type>;
		using node = mods::behaviour_tree_node<argument_type>;
		using node_wrapper = mods::behaviour_tree<argument_type>;
		using status_t = node::run_status_t;
		using status = mods::behaviour_tree_status;
		enum dispatch_status_t {
			RETURN_IMMEDIATELY,
			RETURN_FALSE_IMMEDIATELY,
			AS_YOU_WERE
		};
		enum type { 
			NONE = 0,
			snipe_tracking = 1,
			suspicious_roaming,
		};
		int8_t dispatch(argument_type ch);
		void load_trees();
		void run_trees();
		int8_t register_mob(argument_type,std::string);
		int8_t unregister_mob(argument_type);
		uint8_t grab_tree_by_name(const std::string&);

		constexpr int mob_has_tree = ((1 << 19));
#ifndef TYPE_UNDEFINED
		#define TYPE_UNDEFINED -1
#endif
#ifdef __MENTOC_NO_DEPS__
		bool inline flagged(argument_type &);
		bool inline flagged(argument_type &,const type&);
#else
		bool inline flagged(argument_type & mob){
			return mob.mob_specials().behaviour_tree != 0;
		}
		bool inline flagged(argument_type & mob,const type & type){
			return mob.mob_specials().behaviour_tree != 0 &&
				mob.mob_specials().behaviour_tree == type;
		}
#endif
	};
};


#endif
