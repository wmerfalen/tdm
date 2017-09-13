#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"

namespace mods {
    class player {
		public:
			player() = delete;
			player(char_data* ch);
			bool can_snipe(char_data *target);
			~player() = default;
		protected:
			char_data* m_char_data;
    };
};

#endif
