#ifndef __MENTOC_MODS_WEAPON_TYPES_HEADER__
#define __MENTOC_MODS_WEAPON_TYPES_HEADER__

#include <iostream>
#include "rarity.hpp"

#include "yaml.hpp"
#ifndef __MENTOC_ITEM_CONSTANTS_DEFINED__
#define ITEM_RIFLE     1
#define ITEM_EXPLOSIVE 2
#define ITEM_GADGET    3
#define ITEM_DRONE     4
#define ITEM_WEAPON    5		/* Item is a weapon		*/
#define ITEM_WEAPON_ATTACHMENT 6 /* item is a weapon attachment */
#define ITEM_ATTACHMENT 6
#define ITEM_ARMOR     7
#endif

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
		__GADGET_FIRST = GRAPPLING_HOOK,
		__GADGET_LAST = SENSOR_GRENADE
	};

	enum attachment {
		ATTACHMENT_NONE = 0,
		SIGHT,
		MUZZLE,
		MAGAZINE,
		__ATTACHMENT_FIRST = SIGHT,
		__ATTACHMENT_LAST = MAGAZINE
	};

	enum armor {
		ARMOR_NONE = 0,
		VEST,
		LEGS,
		GAUNTLETS,
		SHOULDER_PADS,
		HELMET,
		HAT,
		EYEWEAR,
		GOGGLES,
		BACKPACK,
		WAIST_PACK,
		VEST_PACK,
		ELBOW_GUARDS,
		GLOVES,
		__ARMOR_FIRST = VEST,
		__ARMOR_LAST = GLOVES
	};

	enum type_list {
		RIFLE = ITEM_RIFLE,
		EXPLOSIVE = ITEM_EXPLOSIVE,
		DRONE = ITEM_DRONE,
		GADGET = ITEM_GADGET,
		ATTACHMENT = ITEM_ATTACHMENT,
		ARMOR = ITEM_ARMOR
	};

};

/** item-types.hpp */
MENTOC_USING_CT

namespace mods::weapon_types {
	template <typename AttributesType>
		std::unique_ptr<AttributesType> feed(std::string_view file){
			auto attributes = std::make_unique<AttributesType>();
			attributes->feed(file);
			attributes->feed_file = file;
			return std::move(attributes);
		}
};
/*
struct rifle_data_t {
	rifle_data_t() = default;
	rifle_data_t(std::string_view in_feed_file) : 
		feed_file(in_feed_file),
		ammo(0),ammo_max(0),clip_size(0),
		loaded(0),holds_ammo(0),
		type(0),is_ammo(0),id(0),
		vnum(0), rarity(mods::rarity::DEFAULT)
	{
		this->feed(in_feed_file);
	}
	void feed(std::string_view in_feed_file){
		feed_file = in_feed_file;
		attributes = mods::weapon_types::feed<mods::yaml::rifle_description_t>(feed_file);
	}
	~rifle_data_t() = default;
	std::unique_ptr<mods::yaml::rifle_description_t> attributes;
	std::string feed_file;
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
*/

#define MENTOC_DATA_CLASS(r,data,CLASS_TYPE)\
struct BOOST_PP_CAT(CLASS_TYPE,_data_t) {\
	BOOST_PP_CAT(CLASS_TYPE, _data_t)() = default;\
	BOOST_PP_CAT(CLASS_TYPE, _data_t)(std::string_view in_feed_file) :\
		manufacturer("ACME Industries"), name(""), type(static_cast<BOOST_PP_CAT(mw_,CLASS_TYPE)>(0)), id(0), vnum(0), rarity(mods::rarity::DEFAULT)\
	{\
		this->feed(in_feed_file);\
	}\
	void feed(std::string_view in_feed_file){\
		feed_file = in_feed_file;\
		attributes = mods::weapon_types::feed<mods::yaml::BOOST_PP_CAT(CLASS_TYPE,_description_t)>(feed_file);\
	}\
	std::string feed_file;\
	~BOOST_PP_CAT(CLASS_TYPE,_data_t)() = default;\
	std::unique_ptr<mods::yaml::BOOST_PP_CAT(CLASS_TYPE,_description_t)> attributes;\
	std::string manufacturer;\
	std::string name;\
	BOOST_PP_CAT(mw_,CLASS_TYPE) type;\
	uint32_t id;\
	int vnum;\
	float rarity;\
};\

BOOST_PP_SEQ_FOR_EACH(MENTOC_DATA_CLASS, ~, MENTOC_ITEM_TYPES_SEQ)

#undef MENTOC_DATA_CLASS

#endif
