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
	void remove_by_player(const uuid_t& player_uuid) {
		player_messages.erase(
		    std::remove_if(player_messages.begin(),
		                   player_messages.end(),
		[player_uuid](std::pair<uuid_t,std::string>& pair) {
			return pair.first == player_uuid;
		}));
		has_messages.erase(
		    std::remove_if(has_messages.begin(),
		                   has_messages.end(),
		[player_uuid](const uuid_t& u) {
			return u == player_uuid;
		})
		);
	}
	void consume_messages_by_player(const uuid_t& player_uuid) {
		if(std::find(has_messages.cbegin(),has_messages.cend(), player_uuid) == has_messages.cend()) {
			return;
		}
		for(auto& item : player_messages) {
			if(item.first == player_uuid) {
				auto ptr = ptr_by_uuid(player_uuid);
				if(!ptr) {
					continue;
				}
				ptr->sendln(item.second.data());
				ptr->desc().flush_output();
			}
		}
		remove_by_player(player_uuid);
	}
	void consume_all() {
		for(auto&& item : player_messages) {
			auto ptr = ptr_by_uuid(item.first);
			if(!ptr) {
				continue;
			}
			ptr->sendln(item.second.c_str());
			ptr->desc().flush_output();
		}
		player_messages.clear();
		has_messages.clear();
	}
	SUPERCMD(do_queue_message) {
		ADMIN_REJECT();
		queue(player->uuid(),argument);
	}
	void init() {
		mods::interpreter::add_command("queue_message", POS_RESTING, do_queue_message, LVL_BUILDER,0);
	}
};
