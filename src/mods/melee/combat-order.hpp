#ifndef __MENTOC_MODS_MELEE_combat_order_HEADER__
#define __MENTOC_MODS_MELEE_combat_order_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../interpreter.hpp"

namespace mods::melee::combat_order {
	using acmd_function = mods::interpreter::acmd_function;
	/**
	 * set first move
	 */
	void set_first(player_ptr_t& player, std::string_view technique);
	void set_second(player_ptr_t& player, std::string_view technique);
	void set_third(player_ptr_t& player, std::string_view technique);
	std::optional<acmd_function> get_melee_function(std::string_view name);
	void dispatch_player(player_ptr_t& attacker,player_ptr_t& victim);
};
#endif

