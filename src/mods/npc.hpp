#ifndef __MENTOC_MODS_NPC_HEADER__
#define  __MENTOC_MODS_NPC_HEADER__

#include <iostream>
#include "../structs.h"
#include "player.hpp"

namespace mods {
	struct npc : public player{
			/* constructors and destructors */
			npc() = delete;
			npc(const mob_rnum&);
			~npc();

			mob_special_data& mob_specials();
			bool has_tree() ;
			player_ptr_t& player_ptr();
			const mob_vnum& vnum() const;
		private:
			std::shared_ptr<mods::player> m_player_ptr;
	};
};

#endif
