#ifndef __MENTOC_MODS_NPC_HEADER__
#define  __MENTOC_MODS_NPC_HEADER__

#include <iostream>
#include "../structs.h"
#include "player.hpp"

namespace mods {
	struct npc : public player {
		/* constructors and destructors */
		npc() = delete;
		npc(const mob_rnum& i) : player(mods::player::player_type_enum_t::MOB) {
			name().assign(mob_proto[i].player.name.c_str());
			short_descr().assign(mob_proto[i].player.short_descr.c_str());
		}

		mob_special_data& mob_specials();
		bool has_tree() ;
		const mob_vnum& vnum() const;
	};
};

#endif
