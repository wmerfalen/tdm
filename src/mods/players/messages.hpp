#ifndef  __MENTOC_MODS_PLAYER_MESSAGES_HEADER__
#define  __MENTOC_MODS_PLAYER_MESSAGES_HEADER__

#include "../../globals.hpp"

namespace mods::players::messages {
	void queue(uuid_t player_uuid,std::string_view msg);
	void consume_all();
	void consume_messages_by_player(const uuid_t& player_uuid);
	void remove_by_player(const uuid_t& player_uuid);
};

#endif
