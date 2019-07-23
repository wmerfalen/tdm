#ifndef __MENTOC_MODS_WEAPON_HEADER__
#define __MENTOC_MODS_WEAPON_HEADER__

#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include "../structs.h"

#define AMMO(weapon) (weapon->ammo)
#define LOADED(weapon) (weapon->loaded)
#define IS_WEAPON_LOADED(weapon) (weapon->loaded == 1)

struct obj_data;
namespace mods::weapon {
	namespace base { 
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
			FLASH_BANG,
			__EXPLOSIVE_FIRST=FRAG_GRENADE,
			__EXPLOSIVE_LAST=FLASH_BANG
		};
		enum drone {
			DRONE_NONE = 0,
			GROUND_DRONE,
			AERIAL_DRONE,
			AQUATIC_DRONE,
			__DRONE_FIRST = GROUND_DRONE,
			__DRONE_LAST = AQUATIC_DRONE
		};
	};/** end namespace mods::weapon::base */
		struct mask {
			static constexpr uint64_t snipe = (1 << 0);
			static constexpr uint64_t grenade = (1 << 1);
		};
		enum type_t { 
			TYPE_NONE = 0,
			RIFLE,
			EXPLOSIVE,
			DRONE,
			__TYPE_FIRST = RIFLE,
			__TYPE_LAST = DRONE
		};

		base::rifle rifle(std::shared_ptr<obj_data>& object);
		base::explosive explosive(std::shared_ptr<obj_data>& object);
		base::drone drone(std::shared_ptr<obj_data>& object);
		std::variant<base::rifle,base::explosive,base::drone> type(
				std::shared_ptr<obj_data>&);

};
#endif

