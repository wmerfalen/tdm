#ifndef __MENTOC_MODS_PLAYER_SCAFFOLDING_HEADER__
#define  __MENTOC_MODS_PLAYER_SCAFFOLDING_HEADER__

#include "player.hpp"

namespace mods {
	struct player_scaffolding {
		public:
			player_scaffolding() = delete;
			player_scaffolding(std::shared_ptr<mods::player>);
			int8_t sniping(bool start_stop);
			int8_t breaching(bool start_stop,bool soft_walls);
			
		protected:
			std::shared_ptr<mods::player> m_player;
	};
};

#endif
