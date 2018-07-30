#ifndef __MENTOC_MODS_LOOPS_HEADER__
#define  __MENTOC_MODS_LOOPS_HEADER__

#include <iostream>
#include <functional>

#include "../structs.h"
extern std::deque<char_data> mob_list;
namespace mods::loops {
	using mob_function_t = std::function<bool(char_data*)>;
	using player_function_t = std::function<bool(char_data*)>;
	using all_function_t = std::function<bool(char_data*)>;
	void foreach_mob(mob_function_t);
	void foreach_player(player_function_t);
	void foreach_all_chars(all_function_t);
};

#include "util.hpp"

#endif
