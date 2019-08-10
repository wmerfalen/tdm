#ifndef __MENTOC_MODS_WEAPON_HEADER__
#define __MENTOC_MODS_WEAPON_HEADER__

#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include "../structs.h"
#include "pqxx-types.hpp"

#define AMMO(weapon) (weapon->ammo)
#define LOADED(weapon) (weapon->loaded)
#define IS_WEAPON_LOADED(weapon) (weapon->loaded == 1)

struct obj_data;
using obj_data_ptr_t = std::shared_ptr<obj_data>;
using attachment_list_t = std::array<obj_data_ptr_t,6>;
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
	enum attachment_t {
		SCOPE = 0,
		GRIP = 1,
		BARREL = 2,
		MAGAZINE = 3,
		SIDERAIL = 4,
		UNDER_BARREL = 5
	};
	obj_data_ptr_t attachment(attachment_t type);
	obj_data_ptr_t new_sniper_rifle_object();

	namespace yaml {
		constexpr static uint8_t MAX_ROOM_DISTANCE = 10;
		struct rifle_description_t {
			rifle_description_t() : 
				ammo_max(0),
			chance_to_injure(0),
			clip_size(0),
			cooldown_between_shots(0),
			critical_chance(0),
			critical_range(0),
			range_multiplier(0),
			damage_per_second(0),
			disorient_amount(0),
			headshot_bonus(0),
			max_range(0),
			reload_time(0),
			ammo_type("")
			{
				std::fill(damage_map.begin(),damage_map.end(),0);
			}
			base::rifle type;
			int ammo_max;
			int chance_to_injure;
			int clip_size;
			float cooldown_between_shots;
			float critical_chance;
			int critical_range;
			float range_multiplier;
			std::array<float,MAX_ROOM_DISTANCE> damage_map;
			int damage_per_second;
			int disorient_amount;
			int headshot_bonus;
			int max_range;
			int reload_time;
			std::string ammo_type;
		};
	};
};
#endif

