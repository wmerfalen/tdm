#ifndef __MENTOC_MODS_ITEM_HEADER__
#define  __MENTOC_MODS_ITEM_HEADER__

#include <iostream>
#include <vector>
#include "../globals.hpp"

namespace mods::item {
	/** TODO: finish this */
	enum capability_t {
		INSTALL = (1 << 0),
		THROW = (1 << 2),
		BURN = (1 << 3),
		DETONATE = (1 << 4),
		BREACH = (1 << 5),
		EXPLODE = (1 << 6),
		REMOTE = (1 << 7),
		DOUSE = (1 << 8),
		ARM = (1 << 9)
	};
	using cap_list_t = std::vector<capability_t>;
	bool is_capable_of_single(player_ptr_t& player, obj_ptr_t& object, capability_t action);
	bool is_capable_of_all(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions);
	bool is_capable_of_any(player_ptr_t& player, obj_ptr_t& object, cap_list_t& actions);
	std::tuple<int16_t,std::string> perform_give(player_ptr_t& player,player_ptr_t& target,obj_ptr_t& object);
};
#endif

