#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"

namespace mods {
	namespace weapon {
		enum mask_type { SNIPE, SHOTGUN };
	};
    class player {
		public:
			player() = delete;
			player(char_data* ch);
			bool has_weapon_capability(weapon::mask_type);
			bool can_snipe(char_data *target);
			~player() = default;
			char_data* cd() const { return m_char_data; }
		protected:
			char_data* m_char_data;
    };
};

#endif
