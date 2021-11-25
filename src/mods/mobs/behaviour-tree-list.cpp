#include "behaviour-tree-list.hpp"
namespace mods::mobs::generic_thief_behaviour_tree {
	extern void run_trees();
};
namespace mods::mobs::chaotic_meth_addict_behaviour_tree {
	extern void run_trees();
};
namespace mods::mobs::shoplifter_behaviour_tree {
	extern void run_trees();
};
namespace mods::mobs::armed_guard_behaviour_tree {
	extern void run_trees();
};
namespace mods::mobs::defiler_behaviour_tree {
	extern void run_trees();
};
namespace mods::mobs::behaviour_tree_list {
	void run() {
		mods::mobs::generic_thief_behaviour_tree::run_trees();
		mods::mobs::chaotic_meth_addict_behaviour_tree::run_trees();
		mods::mobs::shoplifter_behaviour_tree::run_trees();
		mods::mobs::armed_guard_behaviour_tree::run_trees();
		mods::mobs::defiler_behaviour_tree::run_trees();
	}
};
