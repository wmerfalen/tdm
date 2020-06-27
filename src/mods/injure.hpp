#ifndef __MENTOC_MODS_INJURE_HEADER__
#define  __MENTOC_MODS_INJURE_HEADER__
#include "../globals.hpp"

namespace mods::injure {
	bool do_injure_roll(uint8_t chance);
	void injure_player(player_ptr_t& person);
	namespace explosive {
		void handle_crit_injure(obj_ptr_t&,player_ptr_t&);
	};
	namespace rifle {
		void handle_crit_injure(obj_ptr_t&,player_ptr_t&);
	};
};

#endif
