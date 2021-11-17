#include "ban-by-ip.hpp"
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

namespace mods::banning::by_ip {
	static reporter_t report_function;
	static bool reporter_function_set = false;
	void game_entry(player_ptr_t& player) {
		/**
		 * TODO: check if user's IP address is banned...
		 */
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

};
