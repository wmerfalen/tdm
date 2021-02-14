#ifndef __MENTOC_MODS_BEHAVIOUR_TREE_NODE_HEADER__
#define  __MENTOC_MODS_BEHAVIOUR_TREE_NODE_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#ifdef __MENTOC_NO_DEPS__
#else
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#endif

#include "behaviour_tree_status.hpp"
#include <functional>

namespace mods {
	template <typename LambdaArgument>
	struct behaviour_tree_node {
			enum node_type_t { DO_NOTHING, SELECTOR, SEQUENCE, LEAF };
			using callback_t = std::function<behaviour_tree_status(LambdaArgument)>;
			using child_node_t = std::shared_ptr<behaviour_tree_node<LambdaArgument>>;
			using children_t = std::vector<child_node_t>;
			typedef struct {
				\
				behaviour_tree_status status;
				\
				uint64_t id;
				\
			} run_status_t;
			static child_node_t create(callback_t& c,node_type_t n_type) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(c,n_type);
			}
			static child_node_t create(callback_t& c,node_type_t n_type,children_t& ch) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(c,n_type,ch);
			}
			static child_node_t create(node_type_t n_type) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(n_type);
			}
			static child_node_t create_sequence(const children_t& ch) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(node_type_t::SEQUENCE,ch);
			}
			static child_node_t create_selector(const children_t& ch) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(node_type_t::SELECTOR,ch);
			}
			static child_node_t create_leaf(callback_t c) {
				return std::make_shared<behaviour_tree_node<LambdaArgument>>(c,node_type_t::LEAF);
			}
			behaviour_tree_node(const node_type_t& t,const children_t& ch) :
				m_children(ch),
				m_node_type(t) { }
			behaviour_tree_node(const node_type_t& t) :
				m_node_type(t) { }
			behaviour_tree_node(callback_t& c, const node_type_t& t) :
				m_logic(c),
				m_node_type(t) { }
			behaviour_tree_node(callback_t& c,const node_type_t& t, const children_t& ch) :
				m_logic(c),
				m_node_type(t),
				m_children(ch) { }
			behaviour_tree_node() = delete;
			~behaviour_tree_node() = default;
			run_status_t run(LambdaArgument& arg) {
				run_status_t return_value;
				if(m_node_type == LEAF) {
					if(m_logic) {
						return_value.status = m_logic(arg);
						return_value.id = 0;
						return return_value;
					} else {
						return_value.status = behaviour_tree_status::NO_LOGIC;
						return_value.id = 0;
						return return_value;
					}
				}
				uint64_t ctr = 0;
				for(auto& child : m_children) {
					auto ret = child->run(arg);
					return_value.status = ret.status;
					return_value.id = ctr;
					switch(ret.status) {
						case behaviour_tree_status::FAILURE:
							if(m_node_type == SEQUENCE) {
								return return_value;
							}
							break;
						case behaviour_tree_status::SUCCESS:
							if(m_node_type == SELECTOR) {
								return return_value;
							}
							break;
						case behaviour_tree_status::RUNNING:
							return return_value;
						default:
							return_value.status = behaviour_tree_status::UNKNOWN_STATUS;
							return return_value;
					}
					ctr++;
				}
				if(m_node_type == SEQUENCE) {
					/* All children ran successfully, return success */
					return_value.status = behaviour_tree_status::SUCCESS;
				} else {
					return_value.status = behaviour_tree_status::FAILURE;
				}
				return return_value;
			}
			void set_logic(const callback_t& logic) {
				m_logic = logic;
			}
			void append_child(const child_node_t& child) {
				m_children.push_back(child);
			}
			void set_children(const children_t& child_nodes) {
				m_children = child_nodes;
			}
			void set_node_type(const node_type_t& n) {
				m_node_type = n;
			}
			children_t& get_children() const {
				return m_children;
			}
			callback_t& get_logic() const {
				return m_logic;
			}
		private:
			children_t  m_children;
			callback_t  m_logic;
			node_type_t m_node_type;
	};
};


#endif
