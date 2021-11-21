#include "ban-user.hpp"
#include "../classes/includes.hpp"
#include "../replenish.hpp"
#include "../orm/inventory.hpp"
#include "../db.hpp"
#include "../orm/player-base-ability.hpp"
#include "../pq.hpp"
#include "../sql.hpp"
#include "../levels.hpp"
#include "../orm/player-skill-points.hpp"
#include "../orm/player-skill-usage.hpp"
#include "../orm/skill-trees.hpp"
#include "../orm/rifle-attachment.hpp"

#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::banning][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::banning][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif

extern void write_aliases(char_data *ch);
extern void read_aliases(char_data *ch);

namespace mods::banning::user {
	static reporter_t report_function;
	static bool reporter_function_set = false;
	void game_entry(player_ptr_t& player) {
	}

	void set_to_banned(player_ptr_t& player_ptr) {
		std::map<std::string,std::string> values;
		values["player_banned"] = std::string("1");
		try {
			auto up_txn = txn();
			mods::sql::compositor comp("player",&up_txn);
			auto up_sql = comp
			              .update("player")
			              .set(values)
			              .where("id","=",std::to_string(player_ptr->db_id()))
			              .sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
		} catch(std::exception& e) {
			log(CAT("SYSERR:Failed saving player!:", e.what()).c_str());
		}
	}
	void remove_banned_status(player_ptr_t& player_ptr) {
		std::map<std::string,std::string> values;
		values["player_banned"] = std::string("0");
		try {
			auto up_txn = txn();
			mods::sql::compositor comp("player",&up_txn);
			auto up_sql = comp
			              .update("player")
			              .set(values)
			              .where("id","=",std::to_string(player_ptr->db_id()))
			              .sql();
			mods::pq::exec(up_txn,up_sql);
			mods::pq::commit(up_txn);
		} catch(std::exception& e) {
			log(CAT("SYSERR:Failed saving player!:", e.what()).c_str());
		}
	}

	void set_reporter_lambda(reporter_t f) {
		report_function = f;
		reporter_function_set = true;
	}
	void report(int64_t code, std::string_view msg) {
		if(!reporter_function_set) {
			m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) <<
			        " message: '" << red_str(msg.data()) << "'");
			return;
		}
		report_function(code,msg);
	}

	int16_t load_char_pkid(player_ptr_t& player) {
		try {
			auto select_transaction = txn();
			mods::sql::compositor comp("player",&select_transaction);
			auto player_sql = comp.select("id")
			                  .from("player")
			                  .where("player_name","=",player->name())
			                  .sql();
			auto player_record = mods::pq::exec(select_transaction,player_sql);
			if(player_record.size()) {
				player->set_db_id(player_record[0]["id"].as<int>(0));
				return 0;
			}
			log("SYSERR: couldn't grab player's pkid: '%s'",player->name().c_str());
			return -1;
		} catch(std::exception& e) {
			REPORT_DB_ISSUE("error loading character by pkid",e.what());
			return -2;
		}
	}

	namespace commands {
		void init() {

		}
	};
};
