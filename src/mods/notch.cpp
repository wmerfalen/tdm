#include "notch.hpp"
#include "levels.hpp"

#include "super-users.hpp"
#include "interpreter.hpp"
#include "rand.hpp"

#define __MENTOC_SHOW_MODS_NOTCH_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::notch::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::notch::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif

namespace mods::notch {

	void elite_level_achievement(player_ptr_t& player,std::string_view type) {

	}
	void master_level_achievement(player_ptr_t& player,std::string_view type) {

	}
	void advanced_level_achievement(player_ptr_t& player,std::string_view type) {

	}
	void medium_level_achievement(player_ptr_t& player,std::string_view type) {

	}
	void easy_level_achievement(player_ptr_t& player,std::string_view type) {

	}

	void init() {

	}

#undef m_error
#undef m_debug
};
