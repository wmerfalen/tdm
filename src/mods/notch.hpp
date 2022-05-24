#ifndef __MENTOC_MODS_notch_HEADER__
#define __MENTOC_MODS_notch_HEADER__
#include "../globals.hpp"

namespace mods::notch {
	struct record_t {

	};

	/**
	 * Each enumeration is a higher type of achievement
	 */
	enum achievement_levels_t : uint16_t {
		ACH_EASY = 0,
		ACH_MEDIUM,
		ACH_ADVANCED,
		ACH_MASTER,
		ACH_ELITE,
	};

	void elite_level_achievement(player_ptr_t& player,std::string_view type);
	void master_level_achievement(player_ptr_t& player,std::string_view type);
	void advanced_level_achievement(player_ptr_t& player,std::string_view type);
	void medium_level_achievement(player_ptr_t& player,std::string_view type);
	void easy_level_achievement(player_ptr_t& player,std::string_view type);

};
#endif
