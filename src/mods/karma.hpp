#ifndef __MENTOC_MODS_KARMA_HEADER__
#define __MENTOC_MODS_KARMA_HEADER__

#include "../globals.hpp"
#include "orm/karma.hpp"

namespace mods {
	void load(player_ptr_t&);
	struct karma {
		using player_primary_key_t = uint64_t;
		player_primary_key_t player_id;
		float karmic_index;
		float cold_blooded_index;
	};
};

ACMD(do_reputation);

#endif
