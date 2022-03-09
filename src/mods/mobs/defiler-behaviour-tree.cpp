#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "../behaviour_tree_impl.hpp"
#include "defiler.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_defiler_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_defiler_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[m.m.defiler.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::behaviour_tree_impl {
	extern void add_tree(std::string sv_tree_name,node& n);
};

namespace mods::mobs::defiler_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = std::shared_ptr<mods::mobs::defiler>;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;

	std::deque<TNode>& trees() {
		static TNode defiler_roam(TNode::SELECTOR);
		static TNode defiler_hostile(TNode::SELECTOR);
		static bool bootstrapped = false;
		static std::deque<TNode> s;
		if(!bootstrapped) {
			m_debug("bootstrapping");
			defiler_roam.append_child(
			TNode::create_selector({
				//find_someone_to_attack(),
				//random_action(),
				//move_toward_existing_target(),
				//perform_hostile_action(),
			})
			);
			defiler_hostile.append_child(
			TNode::create_sequence({
			})
			);
			s = {
				defiler_roam,
				defiler_hostile,
			};
			bootstrapped = true;
		}
		return s;
	}

	void run_trees() {
		m_debug("run trees");
		for(auto& defiler : defiler_list()) {
			if(defiler->alive() && defiler->capable()) {
				//m_debug("has tree. dispatching..." << defiler->get_tree());
				defiler->start_turn();
				defiler->hostile_phase_1();
				continue;
			}
		}
		//trees()[defiler->get_tree()].run(defiler);
		//		m_debug("checking defiler ptr");
		//		if(defiler->has_tree() && defiler->alive() && defiler->capable()) {
		//			m_debug("has tree. dispatching..." << defiler->get_tree());
		//			defiler_entry(defiler);
		//			//trees()[defiler->get_tree()].run(defiler);
		//		}
		//	}
	}

};

#undef m_debug
