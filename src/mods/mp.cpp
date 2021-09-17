#include "mp.hpp"

#define __MENTOC_MODS_mp_SHOW_DEBUG_OUTPUT__
#ifdef  __MENTOC_MODS_mp_SHOW_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::mp]: '" << a << "'\n";
#else
#define m_debug(a)
#endif

namespace mods::mp {
	uint32_t get(player_ptr_t& player) {
		return player->mp();
	}

	/**
	 * manually call the mp system and get an object in your inventory
	 */
	ACMD(do_mp) {
		if(GET_GOLD(ch) == 0) {
			player->sendln(STOCK_GOLD_BROKE_MESSAGE());
		} else if(GET_GOLD(ch) == 1) {
			player->sendln(STOCK_GOLD_ONE_MISERABLE_MESSAGE());
		} else {
			player->sendln(CAT(STOCK_GOLD_PREFIX_MESSAGE(),get(player),STOCK_GOLD_SUFFIX_MESSAGE()));
		}
	}

	/** game init */
	void init() {
		mods::interpreter::add_user_command("mp",do_mp);
		mods::interpreter::add_user_command("gold",do_mp);
	}
};

