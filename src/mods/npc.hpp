#ifndef __MENTOC_MODS_NPC_HEADER__
#define  __MENTOC_MODS_NPC_HEADER__

#include <iostream>
#include "../structs.h"
#include "player.hpp"

namespace mods {
	struct npc : public player{
			/* constructors and destructors */
			npc();
			npc(char_data*);
			npc(mods::npc*);
			npc(const mob_rnum&);
			~npc();

			mob_special_data& mob_specials();
			bool has_tree() ;
	};
};

#endif
