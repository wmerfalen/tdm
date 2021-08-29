#ifndef __MENTOC_MODS_MOBS_ROAM_PATTERN_HEADER__
#define  __MENTOC_MODS_MOBS_ROAM_PATTERN_HEADER__

#include "../../structs.h"

namespace mods::mobs::roam_pattern {
	void register_roam_pattern(const mob_vnum& mob_vnum,std::string_view pattern);
	bool has_roam_pattern(const mob_vnum&);
	bool has_roam_pattern(const mob_vnum&,std::string* store);
	bool can_roam_to(const mob_vnum& vnum,const room_rnum& room_id);
};
#endif
