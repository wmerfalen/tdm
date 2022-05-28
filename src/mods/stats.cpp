#include "stats.hpp"
#include "affects.hpp"
#include "skills.hpp"
#include "armor.hpp"

#include "rooms.hpp"
#include "injure.hpp"
#include "doors.hpp"
#include "super-users.hpp"
#include "interpreter.hpp"
#include "weapons/damage-types.hpp"
#include "rand.hpp"
#include "weapons/damage-calculator.hpp"
#include "weapons/elemental.hpp"

#include "stats-rifle.hpp"

#define __MENTOC_SHOW_MODS_STATS_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_STATS_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::stats::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::stats::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#define m_error(MSG) ;;
#endif


namespace mods::stats {
	std::string format_stats_page(std::string_view type,std::map<std::string,std::string>&& page) {
		if(type.compare("rifle") == 0) {
			return mods::stats_rifle::format_rifle_stats_page(page);
		}
		return mods::util::map2str(page);
	}

};
#undef m_error
#undef m_debug
