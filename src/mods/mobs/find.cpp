#ifndef __MENTOC_MODS_mobs_find_HEADER__
#define __MENTOC_MODS_mobs_find_HEADER__
#include "find.hpp"

#ifdef __MENTOC_SHOW_MODS_mobs_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::mobs::find]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif

using mob_proto_t = std::deque<char_data>;
extern mob_proto_t mob_proto;

namespace mods::mobs {
	std::optional<mob_proto_t::const_iterator> find_by_vnum(mob_vnum v) {
		static std::map<mob_vnum,mob_proto_t::const_iterator> cache;
		if(cache.find(v) != cache.end()) {
			return cache[v];
		}
		for(auto it=mob_proto.cbegin();
		        it != mob_proto.cend(); ++it) {
			if((*it).nr == v) {
				cache[v] = it;
				return it;
			}
		}
		return std::nullopt;
	}

};

#undef m_debug
#endif
