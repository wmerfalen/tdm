//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "loot-ammo.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::loot_ammo::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::loot_ammo::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	loot_ammo_list_t& loot_ammo_list() {
		static loot_ammo_list_t list;
		return list;
	}

	loot_ammo_list_t load_all_loot_ammo_list() {
		loot_ammo_list_t list;
		loot_ammo r;
		r.read_all(&r);
		for(const auto& r : r.rows) {
			list.emplace_back(std::make_shared<loot_ammo>(r));
		}
		return list;
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
