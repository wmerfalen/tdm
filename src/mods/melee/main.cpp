#include "main.hpp"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rand.hpp"
#include <math.h>

#define __MENTOC_SHOW_main_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_main_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::main][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::melee::main {
	using de = damage_event_t;
	void dispatch_player(player_ptr_t& player) {
		auto orders = player->get_combat_order();
		if(orders.size() == 0) {
			player->sendln("You don't have any combat orders...");
			return;
		}
		auto index = player->get_combat_order_index();
		if(index >= orders.size()) {
			player->set_combat_order_index(0);
			index = 0;
		}
		auto combat_function = mods::melee::combat_order::get_melee_function(orders[index]);
		if(combat_function.has_value()) {
			player->sendln(CAT("Combat function: '",orders[index],"'"));
		} else {
			player->sendln(CAT("No combat function for: '",orders[index],"'"));
		}
		std::array<char,10> f = {"jab\0"};
		char* foo = &f[0];
		combat_function.value()(player->cd(),foo,0,0,player);
		++index;
		if(index >= orders.size()) {
			index = 0;
		}
		player->set_combat_order_index(index);

	}

	ACMD(do_dispatch) {
		dispatch_player(player);
	}

	void init() {
		mods::interpreter::add_user_command("dispatch",do_dispatch);
	}
};

