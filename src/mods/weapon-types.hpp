#ifndef __MENTOC_MODS_WEAPON_TYPES_HEADER__
#define __MENTOC_MODS_WEAPON_TYPES_HEADER__

#include <iostream>
namespace mods::weapon::type {
	enum rifle {
		RIFLE_NONE = 0,
		SHOTGUN,
		ASSAULT_RIFLE,
		SUB_MACHINE_GUN,
		SNIPER,
		HANDGUN,
		MACHINE_PISTOL,
		LIGHT_MACHINE_GUN,
		__RIFLE_FIRST = SHOTGUN,
		__RIFLE_LAST = LIGHT_MACHINE_GUN
	};
	enum explosive {
		EXPLOSIVE_NONE = 0,
		FRAG_GRENADE,
		INCENDIARY_GRENADE,
		REMOTE_EXPLOSIVE,
		REMOTE_CHEMICAL,
		EMP_GRENADE,
		CLAYMORE_MINE,
		SMOKE_GRENADE,
		FLASHBANG_GRENADE,
		__EXPLOSIVE_FIRST=FRAG_GRENADE,
		__EXPLOSIVE_LAST=FLASHBANG_GRENADE
	};
	enum drone {
		DRONE_NONE = 0,
		GROUND_DRONE,
		AERIAL_DRONE,
		AQUATIC_DRONE,
		__DRONE_FIRST = GROUND_DRONE,
		__DRONE_LAST = AQUATIC_DRONE
	};
};

using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;
#endif
