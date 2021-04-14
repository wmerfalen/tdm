#include "messages.hpp"
#include "../player.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_MESSAGES_DEBUG_OUTPUT__
#ifdef  __MENTOC_SHOW_MODS_PLAYERS_MESSAGES_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::messages][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::destroy_player][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::messages {
	static std::vector<std::pair<uuid_t,std::string>> player_messages;
	static std::vector<uuid_t> has_messages;
	void queue(uuid_t player_uuid,std::string_view msg) {
		player_messages.emplace_back(std::make_pair<>(player_uuid,msg));
		has_messages.emplace_back(player_uuid);
	}
	void consume_all() {
		for(auto&& item : player_messages) {
			auto ptr = ptr_by_uuid(item.first);
			if(!ptr) {
				continue;
			}
			ptr->send(item.second.data());
			ptr->desc().flush_output();
		}
		player_messages.clear();
	}
	SUPERCMD(do_queue_message) {
		ADMIN_REJECT();
		queue(player->uuid(),argument);
	}
	void init() {
		mods::interpreter::add_command("queue_message", POS_RESTING, do_queue_message, LVL_BUILDER,0);
	}
};
