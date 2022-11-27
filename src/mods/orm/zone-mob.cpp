//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "zone-mob.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::zone_mob::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::zone_mob::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	zone_mob_list_t& zone_mob_list() {
		static zone_mob_list_t list;
		return list;
	}

	zone_mob_list_t load_all_zone_mob_list() {
		zone_mob_list_t list;
		zone_mob r;
		r.read_all(&r);
		for(const auto& r : r.rows) {
			list.emplace_back(std::make_shared<zone_mob>(r));
		}
		return list;
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
