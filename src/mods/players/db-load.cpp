#include "db-load.hpp"
#include "../classes/includes.hpp"
#include "../replenish.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
	#define m_debug(a) std::cerr << "[mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
	#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
	#define m_debug(a)
	#define m_crit(a)
#endif


namespace mods::players::db_load {
	static reporter_t report_function;
	static bool reporter_function_set = false;
	void set_reporter_lambda(reporter_t f){
		report_function = f;
		reporter_function_set = true;
	}
	void report(int64_t code, std::string_view msg){
		if(!reporter_function_set){
			m_debug("[fallback reporter]: " << red_str(CAT("code:",(code))) << 
				" message: '" << red_str(msg.data()) << "'");
			return;
		}
		report_function(code,msg);
	}
	void set_class(player_ptr_t& player, player_class_t p_class){
		m_debug(green_str("set_class called for player uuid:") << player->uuid());
		player->set_class(p_class);
		switch(p_class){
			case GHOST:
				player->set_ghost(mods::classes::create_ghost(player));
				mods::replenish::register_ghost(player->uuid());
				break;
			default:
				report(p_class,"Unable to create class. unknown class");
				break;
		}
	}
};
