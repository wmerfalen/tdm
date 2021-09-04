#include "event-messages.hpp"
#include "../replenish.hpp"
#include "../movement.hpp"
#include "../interpreter.hpp"
#include "../loops.hpp"
#include "../prefs.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_event_messages_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_event_messages_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::event_messages][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::event_messages][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::event_messages {
	static std::vector<std::string> event_messages;
	static std::map<std::string,std::size_t> player_event_message_index;
	void send() {
		if(event_messages.size() == 0) {
			return;
		}
		mods::loops::foreach_player([&](auto player) -> bool {
			m_debug("noevent value:" << PLAYER_GET("noevent"));
			bool no_event_messages = mods::prefs::get_player_pref(player,"noevent");
			if(no_event_messages || player->position() <= POS_STUNNED || player->fighting()) {
				return true; /** skip user as they requested event_messages off */
			}

			auto& ref = player_event_message_index[player->name().c_str()];
			if(ref >= event_messages.size()) {
				ref = 0;
			}
			player->sendln(CAT("{grn}Reminder:{/grn} {yel}",event_messages[ref],"{/yel}"));
			ref++;
			return true;
		});
	}
	void load_from_db() {
		player_event_message_index.clear();
		event_messages.clear();
		for(const auto& row : db_get_all("event_messages")) {
			event_messages.emplace_back(row["em_msg"].c_str());
		}
	}
	std::tuple<bool,std::string> add_message(std::string_view msg) {
		try {
			auto txn2 = txn();
			auto sql = sql_compositor("event_messages",&txn2)
			           .insert()
			           .into("event_messages")
			.values({
				{"em_msg",msg.data()}
			})
			.sql();
			mods::pq::exec(txn2,sql);
			mods::pq::commit(txn2);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("inserting into event_messages failed",e.what());
			return {false,std::string("inserting into event_messages failed: ") + e.what()};
		}
		return {1,"ok"};
	}


	std::tuple<bool,std::string> delete_message(std::size_t index) {
		m_debug("delete event_messages... " << index);
		try {
			auto del_txn = txn();
			sql_compositor comp("event_messages",&del_txn);
			auto room_sql = comp.del()
			                .from("event_messages")
			                .where("id",index)
			                .sql();
			auto row = mods::pq::exec(del_txn,room_sql.data());
			mods::pq::commit(del_txn);
			return {1,"deleted"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("delete_message (event_messages) exception: ",e.what());
			return {false,CAT("failed: '",e.what(),"'")};
		}
	}

	SUPERCMD(do_delete_event_message) {
		if(argshave()->int_at(0)->passed()) {
			auto index = intat(0);
			delete_message(index);
			load_from_db();
			player->sendln("Ran delete command and reloaded strings from db");
			return;
		}
		player->sendln("Usage: delete_event_message <id>");
	}
	SUPERCMD(do_add_event_message) {

	}
	SUPERCMD(do_list_event_message) {
		std::vector<std::string> msg_list;
		for(const auto& row : db_get_all("event_messages")) {
			player->sendln(CAT("[id]: ",row["id"].as<int>()," '",row["em_msg"].c_str(),"'"));
		}
	}
	void init() {
		ADD_BUILDER_COMMAND("list_event_message",do_list_event_message);
		ADD_BUILDER_COMMAND("add_event_message",do_add_event_message);
		ADD_BUILDER_COMMAND("delete_event_message",do_delete_event_message);
	}
};
