#ifndef __MENTOC_MODS_MELEE_combat_order_HEADER__
#define __MENTOC_MODS_MELEE_combat_order_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::melee::combat_order {
	/**
	 * set first move
	 */
	void set_first(player_ptr_t& player, std::string_view technique);
	void set_second(player_ptr_t& player, std::string_view technique);
	void set_third(player_ptr_t& player, std::string_view technique);
};
#endif

