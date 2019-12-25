#ifndef __MENTOC_MODS_WEAPON_TYPES_HEADER__
#define __MENTOC_MODS_WEAPON_TYPES_HEADER__

#include <iostream>
#include "rarity.hpp"
#include "yaml.hpp"
namespace mods::weapon::type {
	enum rifle {
		RIFLE_NONE = 0,
		SHOTGUN,
		ASSAULT_RIFLE,
		SUB_MACHINE_GUN,
		SNIPER,
		HANDGUN,
		PISTOL = rifle::HANDGUN,
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
		C4,
		BREACH_CHARGE,
		FLASHBANG_GRENADE,
		IMPACT_GRENADE,
		__EXPLOSIVE_FIRST=FRAG_GRENADE,
		__EXPLOSIVE_LAST=IMPACT_GRENADE
	};
	enum drone {
		DRONE_NONE = 0,
		GROUND_DRONE,
		AERIAL_DRONE,
		AQUATIC_DRONE,
		__DRONE_FIRST = GROUND_DRONE,
		__DRONE_LAST = AQUATIC_DRONE
	};
	enum gadget {
		GADGET_NONE = 0,
		GRAPPLING_HOOK,
		BARBED_WIRE,
		CAMERA,
		MOTION_SENSOR,
		DEPLOYABLE_SHIELD,
		SENSOR_GRENADE,
		__GADGET_FIRST = GADGET_NONE,
		__GADGET_LAST = SENSOR_GRENADE
	};
};

using mw_explosive = mods::weapon::type::explosive;
using mw_rifle = mods::weapon::type::rifle;
using mw_drone = mods::weapon::type::drone;
using mw_gadget = mods::weapon::type::gadget;

	struct weapon_data_t {
		weapon_data_t() : 
			ammo(0),ammo_max(0),clip_size(0),
			loaded(0),holds_ammo(0),
			type(0),is_ammo(0),id(0),
			vnum(0), rarity(mods::rarity::DEFAULT)
		{ }
		~weapon_data_t() = default;
		std::unique_ptr<mods::yaml::rifle_description_t> rifle_attributes;
		int16_t ammo;
		int16_t ammo_max;
		uint8_t clip_size;
		short loaded;
		short holds_ammo;
		uint8_t type;
		bool is_ammo;
		uint32_t id;
		int vnum;
		float rarity;
	};

	struct explosive_data_t {
		explosive_data_t() : 
			name(""),
			type(mw_explosive::EXPLOSIVE_NONE),
			id(0),
			vnum(0), rarity(mods::rarity::DEFAULT)
		{ }
		~explosive_data_t() = default;
		std::unique_ptr<mods::yaml::explosive_description_t> explosive_attributes;
		std::string name;
		mw_explosive type;
		uint32_t id;
		int vnum;
		float rarity;
	};


#endif
