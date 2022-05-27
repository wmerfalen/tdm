#include "notch.hpp"
#include "levels.hpp"

#include "super-users.hpp"
#include "interpreter.hpp"
#include "rand.hpp"
#include "orm/notch.hpp"

#define __MENTOC_SHOW_MODS_NOTCH_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::notch::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::notch::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif
#define m_report(MSG) mentoc_prefix_debug("[mods::notch::report]")  << MSG << "\n";

namespace mods::notch {

	void notch_it(auto player, auto type) {
		auto status = mods::orm::increment_player_notch(player,type);
		m_report("achievement: " << type << "status: " << std::get<0>(status) << "|" << std::get<1>(status));
	}
	void elite_level_achievement(player_ptr_t& player,std::string_view type) {
		notch_it(player,type);
	}
	void master_level_achievement(player_ptr_t& player,std::string_view type) {
		notch_it(player,type);

	}
	void advanced_level_achievement(player_ptr_t& player,std::string_view type) {
		notch_it(player,type);
	}
	void medium_level_achievement(player_ptr_t& player,std::string_view type) {
		notch_it(player,type);
	}
	void easy_level_achievement(player_ptr_t& player,std::string_view type) {
		notch_it(player,type);
	}

	void init() {
		m_debug("init()");

	}

#undef m_error
#undef m_debug
};
