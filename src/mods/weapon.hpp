#ifndef __MENTOC_MODS_WEAPON_HEADER__
#define __MENTOC_MODS_WEAPON_HEADER__

#include <iostream>
#include <string>

#define AMMO(weapon) (weapon->ammo)
#define LOADED(weapon) (weapon->loaded)
#define IS_WEAPON_LOADED(weapon) (weapon->loaded == 1)

namespace mods {
	namespace weapon {
		struct mask {
			static constexpr uint64_t snipe = (1 << 0);
			static constexpr uint64_t grenade = (1 << 1);
		};
	};
};

#endif

