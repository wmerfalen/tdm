#ifndef __MENTOC_MODS_MOBS_find_HEADER__
#define __MENTOC_MODS_MOBS_find_HEADER__
#include "../../structs.h"

using mob_proto_t = std::deque<char_data>;
extern mob_proto_t mob_proto;

namespace mods::mobs {
	std::optional<mob_proto_t::const_iterator> find_by_vnum(mob_vnum v);

};

#endif
