#ifndef  __MENTOC_MODS_MOBS_ORTHOS_SPAWN_SENTINEL_BEHAVIOUR_TREE_HEADER__
#define  __MENTOC_MODS_MOBS_ORTHOS_SPAWN_SENTINEL_BEHAVIOUR_TREE_HEADER__

#include "orthos-spawn-sentinel.hpp"
#include "../radio.hpp"

namespace mods::mobs::orthos_spawn_sentinel_btree {
	template <typename T>
	concept NotPointer = !std::is_pointer_v<T>;

	template <typename T>
	requires(NotPointer<T>)
	struct btree {
		T value;
	};
	/**
	 * MP Shotgunner behaviour tree
	 * ----------------------------
	    [1] When attacked
				[1] radio nearby units for help at coordinates
				[2] immediately pursue until in same room
				[3] fire shotgun
				[4] repeat until dead or target dead

			[2] When radio'd for help
				[1] change behaviour tree to mps_backup_pursuit
				[2] add reported player uuid to remember event

			[mps_backup_pursuit behaviour tree]
				[1] scan for specific target
				[2] if found, head to that direction
				[3] engage target if found
				[4] repeat until dead or target dead
				[5] forget target
				[6] teleport back to zone cmd room

	 */
	using namespace helpers;
	using vec_player_data = mods::scan::vec_player_data;
	using TArgumentType = mods::npc&;
	using TNode = mods::behaviour_tree_node<TArgumentType>;
	using TStatus = mods::behaviour_tree_status;
	using vec_player_data = mods::scan::vec_player_data;
	using TChildNode = mods::behaviour_tree_node<TArgumentType>::child_node_t;

	//TChildNode debug_echo_tree_name(str_t name);
	//TChildNode shotgun_attack();
	//TChildNode debug_echo_tree_name(str_t name);
	//TChildNode shout_random();
	//TChildNode random_trivial_action();
	//TChildNode set_behaviour_tree_to_engage();
	//TChildNode set_behaviour_tree_to_pursuit();
	//TChildNode spray_direction();
	//TChildNode can_still_see_target();
	//TChildNode find_targets();
	//TChildNode find_attackers();
	//TChildNode move_toward_target();
	//TChildNode attackers_optimal_distance_away();
	//TChildNode perform_random_non_hostile_action();
	//TChildNode scan_to_find_hostile_activity();
	//TChildNode move_toward_heading();
	//TChildNode report_hostile_activity();
	//TChildNode engage_hostile();

	//void make_orthos_spawn_sentinel(TNode&);
	//void make_orthos_spawn_sentinel_engage(TNode&);
	//void make_orthos_spawn_sentinel_backup(TNode&);
	//void make_orthos_spawn_sentinel_pursuit(TNode&);
	void run_trees();

};//end namespace

#endif