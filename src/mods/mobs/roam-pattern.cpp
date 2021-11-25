#include "roam-pattern.hpp"
#include "../util.hpp"

#ifdef  __MENTOC_MODS_MOBS_ROAM_PATTERN_SHOW_DEBUG_OUTPUT__
#define m_debug(a) mentoc_prefix_debug("m|m|roam_pattern") << a << "\n";
#else
#define m_debug(a) ;;
#endif

namespace mods::mobs::roam_pattern {
	static std::vector<std::pair<mob_vnum,std::string>> roam_patterns;
	void register_roam_pattern(const mob_vnum& mob_vnum,std::string_view pattern) {
		m_debug("registering roam pattern: " << mob_vnum << ", pattern:'" << pattern << "'");
		roam_patterns.emplace_back(std::make_pair<>(mob_vnum,pattern.data()));
	}
	bool has_roam_pattern(const mob_vnum& vnum) {
		for(const auto& pair : roam_patterns) {
			if(pair.first == vnum) {
				return true;
			}
		}
		return false;
	}
	bool has_roam_pattern(const mob_vnum& vnum,std::string* store) {
		for(const auto& pair : roam_patterns) {
			if(pair.first == vnum) {
				store->assign(pair.second);
				return true;
			}
		}
		return false;
	}
	bool can_roam_to(const mob_vnum& vnum,const room_rnum& room_id) {
		std::string storage;
		if(has_roam_pattern(vnum,&storage)) {
			m_debug("mob has roam pattern: " << vnum << ", itis: '" << storage << "'");
			if(world[room_id].name.str().find(storage.c_str()) != std::string::npos) {
				m_debug("matched");
				return true;
			}
		}
		return false;
	}
};
#undef m_debug
