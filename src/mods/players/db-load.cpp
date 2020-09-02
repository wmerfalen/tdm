#include "db-load.hpp"
#include "../classes/includes.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_DB_LOAD_DEBUG_OUTPUT__
	#define m_debug(a) std::cerr << "[mods::armor::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
	#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::armor::db_load][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
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
			std::cerr << "[fallback reporter]: " << red_str(CAT("code:",(code))) << 
				" message: '" << red_str(msg.data()) << "'\n";
			return;
		}
		report_function(code,msg);
	}
	void set_class(player_ptr_t& player, player_class_t p_class){
		switch(p_class){
			case CLASS_MARINE:
				player->set_marine(std::make_shared<mods::classes::marine>(player));
				break;
			case CLASS_SNIPER:
				player->set_sniper(std::make_shared<mods::classes::sniper>(player));
				break;
			case CLASS_SENTINEL:
				player->set_sentinel(std::make_shared<mods::classes::sentinel>(player));
				break;
			case CLASS_MEDIC:
				player->set_medic(std::make_shared<mods::classes::medic>(player));
				break;
			case CLASS_CONTAGION:
				player->set_contagion(std::make_shared<mods::classes::contagion>(player));
				break;
			case CLASS_ENGINEER:
				player->set_engineer(std::make_shared<mods::classes::engineer>(player));
				break;
			case CLASS_SUPPORT:
				player->set_support(std::make_shared<mods::classes::support>(player));
				break;
			case CLASS_PSYOP:
				player->set_psyop(std::make_shared<mods::classes::psyop>(player));
				break;
			default:
				report(p_class,"Unable to create class. unknown class");
				break;
		}
	}
};
