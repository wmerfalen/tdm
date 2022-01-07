#include "destroy-player.hpp"
#include "messages.hpp"
#include "../replenish.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_DESTROY_PLAYER_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_DESTROY_PLAYER_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::destroy_player][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::destroy_player][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::destroy_player {
	void destroy_player(uuid_t player_uuid) {
		/**
		 * This whole function and the called function shouldn't
		 * be named this way. it's very mis-leading.
		 * Especially as long as destroy_player exists in comm.cpp
		 */
		mods::replenish::destroy_player(player_uuid);
		mods::players::messages::consume_messages_by_player(player_uuid);
	}
};
