#ifndef __MENTOC_MODS_MOB_EQUIPMENT_HEADER__
#define __MENTOC_MODS_MOB_EQUIPMENT_HEADER__

#include "orm/mob-equipment.hpp"
#include "orm/mob-equipment-map.hpp"

namespace mods::mob_equipment {
	std::array<std::string,NUM_WEARS> fetch_list_by_mob_vnum(mob_vnum mobvnum);
	void refresh_mappings();
};

#endif
