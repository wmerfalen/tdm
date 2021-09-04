#include "friendly-reminders.hpp"
#include "../replenish.hpp"
#include "../movement.hpp"
#include "../interpreter.hpp"
#include "../loops.hpp"
#include "../prefs.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_friendly_reminders_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_friendly_reminders_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::friendly_reminders][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::friendly_reminders][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::friendly_reminders {
	static std::vector<std::string> reminders;
	static std::map<std::string,std::size_t> player_reminder_index;
	void send() {
		if(reminders.size() == 0) {
			return;
		}
		mods::loops::foreach_player([&](auto player) -> bool {
			m_debug("noreminder value:" << PLAYER_GET("noreminder"));
			bool no_reminders = mods::prefs::get_player_pref(player,"noreminder");
			if(no_reminders || player->position() <= POS_STUNNED || player->fighting()) {
				return true; /** skip user as they requested reminders off */
			}

			auto& ref = player_reminder_index[player->name().c_str()];
			if(ref >= reminders.size()) {
				ref = 0;
			}
			player->sendln(CAT("{grn}Reminder:{/grn} {yel}",reminders[ref],"{/yel}"));
			ref++;
			return true;
		});
	}
	void load_from_db() {
		for(const auto& row : db_get_all("friendly_reminders")) {
			m_debug("found reminder str: '" << row["fr_msg"].c_str() << "'");
			reminders.emplace_back(row["fr_msg"].c_str());
		}
	}
	std::tuple<bool,std::string> add_message(std::string_view msg) {
		try {
			auto txn2 = txn();
			auto sql = sql_compositor("friendly_reminders",&txn2)
			           .insert()
			           .into("friendly_reminders")
			.values({
				{"fr_msg",msg.data()}
			})
			.sql();
			mods::pq::exec(txn2,sql);
			mods::pq::commit(txn2);
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("inserting into friendly_reminders failed",e.what());
			return {false,std::string("inserting into friendly_reminders failed: ") + e.what()};
		}
		return {1,"ok"};
	}
	std::tuple<bool,std::string> delete_message(std::size_t index) {
		m_debug("delete event_message... " << index);
		try {
			auto del_txn = txn();
			sql_compositor comp("friendly_reminders",&del_txn);
			auto room_sql = comp.del()
			                .from("friendly_reminders")
			                .where("id",index)
			                .sql();
			auto row = mods::pq::exec(del_txn,room_sql.data());
			mods::pq::commit(del_txn);
			return {1,"deleted"};
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("delete_message (friendly_reminders) exception: ",e.what());
			return {false,CAT("failed: '",e.what(),"'")};
		}
		return {1,"ok"};
	}

	void add_reminder(std::string_view msg) {

	}
	SUPERCMD(do_delete_reminder) {

	}
	SUPERCMD(do_add_reminder) {

	}
	SUPERCMD(do_list_reminder) {
		std::vector<std::string> msg_list;
		for(const auto& row : db_get_all("friendly_reminders")) {
			player->sendln(CAT("[id]: ",row["id"].as<int>()," '",row["fr_msg"].c_str(),"'"));
		}
	}
	void init() {
		ADD_BUILDER_COMMAND("list_reminder",do_list_reminder);
		ADD_BUILDER_COMMAND("add_reminder",do_add_reminder);
		ADD_BUILDER_COMMAND("delete_reminder",do_delete_reminder);
	}
};
