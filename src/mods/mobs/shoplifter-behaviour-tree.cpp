#include "../behaviour_tree.hpp"
#include "../behaviour_tree_status.hpp"
#include "../npc.hpp"
#include "../behaviour_tree_impl.hpp"
#include "shoplifter.hpp"
#include "../weapons/damage-types.hpp"
#include "../../comm.h"
#include "../calc-visibility.hpp"
#include "../rooms.hpp"
#include "helpers.hpp"
#include "../radio.hpp"
#include "../mob-roam.hpp"
#include "../query-objects.hpp"

//#define  __MENTOC_SHOW_BEHAVIOUR_TREE_shoplifter_BTREE_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_BEHAVIOUR_TREE_shoplifter_BTREE_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[m.m.shoplifter.btree:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#endif

extern void act(const std::string& str, int hide_invisible, char_data *ch, obj_data *obj, void *vict_obj, int type);

namespace mods::behaviour_tree_impl {
	extern void add_tree(std::string sv_tree_name,node& n);
};

namespace mods::mobs::shoplifter_behaviour_tree {
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = std::shared_ptr<mods::mobs::shoplifter>;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;


	void run_trees() {
		m_debug("run trees");
		for(auto& shoplifter : shoplifter_list()) {
			if(shoplifter->alive() && shoplifter->capable()) {
				m_debug("alive and capable. attacking...");
				shoplifter->btree_hostile();
			}
		}
	}

};

#undef m_debug
