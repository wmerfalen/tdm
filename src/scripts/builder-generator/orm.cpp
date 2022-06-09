//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "___[[[TABLE]]]___.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::___[[[TABLE]]]___::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::___[[[TABLE]]]___::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	___[[[TABLE]]]____list_t& ___[[[TABLE]]]____list() {
		static ___[[[TABLE]]]____list_t list;
		return list;
	}

	___[[[TABLE]]]____list_t load_all____[[[TABLE]]]____list() {
		___[[[TABLE]]]____list_t list;
		___[[[TABLE]]]___ r;
		r.read_all(&r);
		for(const auto& r : r.rows) {
			list.emplace_back(std::make_shared<___[[[TABLE]]]___>(r));
		}
		return list;
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
