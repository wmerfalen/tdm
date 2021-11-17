#ifndef __MENTOC_MODS_ADMIN_TOOLS_LOCKDOWN_PLAYER_HEADER__
#define  __MENTOC_MODS_ADMIN_TOOLS_LOCKDOWN_PLAYER_HEADER__

#include "../../globals.hpp"
#include <functional>

namespace mods::admin_tools::lockdown {
	void set_class(player_ptr_t& player, player_class_t class_type);
	using reporter_t = std::function<void(int64_t,std::string_view)>;
	void set_reporter_lambda(reporter_t f);
	enum save_from_t {
		NONE,
		EXTRACTION,
		QUIT,
		MANUAL_SAVE_ACMD,
		GEN_TOGGLE_ACMD,
		GAIN_EXP,
		CONTRACT_REWARD,
		DISPLAY_ACMD,
		DB_SAVE_CHAR,
		PRACTICE_SKILL,
		PREFS,
	};
	void load_aliases(player_ptr_t& player);
	void save_from(player_ptr_t& player,save_from_t);
	void save_prefs(player_ptr_t& player_ptr);
	void extract_character(player_ptr_t& player);
	void load_base_abilities(player_ptr_t& player);
	int16_t save_new_char(player_ptr_t& player);
	int16_t load_char_pkid(player_ptr_t& player);
	int16_t delete_char(player_ptr_t&);
	void sync_player_with_class_skills(const uint64_t& player_id,const std::string& player_class);
	void game_entry(player_ptr_t& player);
};

#endif
