#ifndef __MENTOC_MODS_FORGE_ENGINE_REQUIREMENTS_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_REQUIREMENTS_HEADER__
#include "../../structs.h"
#include "player-class.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::forge_engine::requirements]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

namespace mods::forge_engine {
		enum stat_types_t {
			SKILL_STR = 1,
			SKILL_STR_ADD ,
			SKILL_INTEL ,
			SKILL_WIS ,
			SKILL_DEX ,
			SKILL_CON ,
			SKILL_CHA ,
			SKILL_ELECTRONICS ,
			SKILL_ARMOR ,
			SKILL_MARKSMANSHIP ,
			SKILL_SNIPING ,
			SKILL_DEMOLITIONS ,
			SKILL_CHEMISTRY ,
			SKILL_WEAPON_HANDLING ,
			SKILL_STRATEGY ,
			SKILL_MEDICAL ,
			__SKILL_TYPE_FIRST = SKILL_STR,
			__SKILL_TYPE_LAST = SKILL_MEDICAL
		};
	struct requirements_t {
		std::vector<std::pair<stat_types_t,uint32_t>> stats;
		uint32_t minimum_player_level;
		std::vector<player_class_types_t> player_classes;
	};
};
#undef m_debug
#endif
