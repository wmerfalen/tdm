//#define __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#include "class-ability-help-pages.hpp"
#include "util.hpp"
#include "../player.hpp"
#include "../util-map.hpp"

#ifdef __MENTOC_SHOW_MODS_ORM_NOTCH_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::orm::class_ability_help_pages::debug]")  << MSG << "\n";
#define m_error(MSG) mentoc_prefix_debug(red_str("[mods::orm::class_ability_help_pages::ERROR]"))  << MSG << "\n";
#else
#define m_debug(MSG)
#define m_error(MSG)
#endif


namespace mods::orm {

	class_ability_help_pages_list_t& class_ability_help_pages_list() {
		static class_ability_help_pages_list_t list;
		return list;
	}

	class_ability_help_pages_list_t load_all_class_ability_help_pages_list() {
		class_ability_help_pages_list_t list;
		class_ability_help_pages r;
		r.read_all(&r);
		for(const auto& r : r.rows) {
			list.emplace_back(std::make_shared<class_ability_help_pages>(r));
		}
		return list;
	}
#undef __MENTOC_SHOW_UTIL_MAP_DUMP_OUTPUT__
#undef __MENTOC_ORM_UTIL_DEBUG_OUTPUT__
};
