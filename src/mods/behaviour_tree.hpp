#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

/** __MENTOC_NO_DEPS__ Essentially means that
 * we only want the behaviour tree implementation and no
 * other dependencies, hence the "NO_DEPS".
 */
#ifdef __MENTOC_NO_DEPS__
#else
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#endif
#include "behaviour_tree_node.hpp"

namespace mods {
	template <typename LambdaArgument>
	struct behaviour_tree {
		static constexpr int NONE = 0;
		using node = behaviour_tree_node<LambdaArgument>;
		using run_status = typename node::run_status_t;
		/** constructors */
		behaviour_tree() : m_tree_name("default") {}
		behaviour_tree(const std::string_view & name) : m_tree_name(name) {}
		behaviour_tree(const std::string_view & name,
			const node & head) : m_tree_name(name),
			m_node(head){};
		~behaviour_tree() = default;

		/** accessors/mutators */
		std::string_view name() const { return m_tree_name; }
		void set_node(const node & btree_node){
			m_node = btree_node;
		}
		node & get_node() const {
			return m_node;
		}
		/** proxy function to run the tree */
		run_status run(LambdaArgument arg) {
			return m_node.run(arg);
		}
		private:
		std::string m_tree_name;
		node m_node;
	};
};

#endif
