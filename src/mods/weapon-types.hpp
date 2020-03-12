#ifndef __MENTOC_MODS_WEAPON_TYPES_HEADER__
#define __MENTOC_MODS_WEAPON_TYPES_HEADER__

namespace mods::weapon::capabilities {
	enum cap_t {
		INSTALL = 0,
		THROW,
		BURN,
		REMOTELY_DETONATE,
		BREACH_DOORS,
		EXPLODE,
		SNIPE,
		ZOOM,
		RANGED_ATTACK,
		SPRAY_BULLETS,
		HIP_FIRE,
		AIM,
		BLIND,
		DISORIENT,
		SHOOT,
		CQC,
		THERMAL_VISION,
		NIGHT_VISION,
		EMIT_SUBSTANCE,
		ALTERNATE_EXPLOSION,
		SCAN,
		HAS_CLIP,
		EXTENDS_CLIP,
		COUNTDOWN_EXPLOSION,
		RELOAD,
		__LAST = RELOAD
	};
};

using cap_t = mods::weapon::capabilities::cap_t;

#include <iostream>
#include "rarity.hpp"

#include "yaml.hpp"
#ifndef __MENTOC_ITEM_CONSTANTS_DEFINED__
#define ITEM_RIFLE             1
#define ITEM_EXPLOSIVE         2
#define ITEM_GADGET            3
#define ITEM_DRONE             4
#define ITEM_WEAPON            5		/* Item is a weapon		*/
#define ITEM_WEAPON_ATTACHMENT 6 /* item is a weapon attachment */
#define ITEM_ATTACHMENT        6
#define ITEM_ARMOR             7
#define ITEM_CONSUMABLE        8
#define ITEM_TRAP              9
#endif
#include "item-types.hpp"

namespace mods::weapon::type {
	enum rifle {
		RIFLE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_RIFLE_TYPES_SEQUENCE)
	};
	enum explosive {
		EXPLOSIVE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_EXPLOSIVE_TYPES_SEQUENCE)
	};
	enum drone {
		DRONE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_DRONE_TYPES_SEQUENCE)
	};
	enum gadget {
		GADGET_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_GADGET_TYPES_SEQUENCE)
	};

	enum attachment {
		ATTACHMENT_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_ATTACHMENT_TYPES_SEQUENCE)
	};

	enum armor {
		ARMOR_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_ARMOR_TYPES_SEQUENCE)
	};

	enum consumable {
		CONSUMABLE_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_CONSUMABLE_TYPES_SEQUENCE)
	};

	enum trap {
		TRAP_NONE = 0,
		BOOST_PP_SEQ_FOR_EACH(MENTOC_ENUM_TYPES_IMPL,~,MENTOC_TRAP_TYPES_SEQUENCE)
	};


#define MENTOC_TYPE_LIST_IMPL(r,data,ITEM_TYPE) \
	ITEM_TYPE = BOOST_PP_CAT(ITEM_,ITEM_TYPE) BOOST_PP_COMMA()

	enum type_list {
		BOOST_PP_SEQ_FOR_EACH(MENTOC_TYPE_LIST_IMPL,~,MENTOC_ITEM_TYPE_LIST_SEQUENCE)
	};

};

/** item-types.hpp */
MENTOC_USING_CT

namespace mods::weapon_types {
	template <typename AttributesType>
		std::tuple<std::unique_ptr<AttributesType>,uint64_t> feed(std::string_view file){
			auto attributes = std::make_unique<AttributesType>();
			attributes->feed(file);
			attributes->feed_file = file;
			auto id = attributes->db_id();
			return {std::move(attributes),id};
		}
	template <typename AttributesType>
		std::unique_ptr<AttributesType> empty_make(){
			return std::move(std::make_unique<AttributesType>());
		}
};
#define MENTOC_DATA_CLASS(r,data,CLASS_TYPE)\
	struct BOOST_PP_CAT(CLASS_TYPE,_data_t) {\
		/** rifle_data_t() */\
		/** rifle_data_t() */\
		/** rifle_data_t() */\
		BOOST_PP_CAT(CLASS_TYPE, _data_t)(){\
			feed_file = "";\
			id=0;\
			vnum=0;\
			rarity=mods::rarity::DEFAULT;\
			manufacturer = "";\
			name="";\
			type = (decltype(type))0;\
			this->attributes = std::move(mods::weapon_types::empty_make<mods::yaml::BOOST_PP_CAT(CLASS_TYPE,_description_t)>());\
			id=0;\
			\
		}\
		/** rifle_data_t(std::string_view in_feed_file) */\
		/** rifle_data_t(std::string_view in_feed_file) */\
		/** rifle_data_t(std::string_view in_feed_file) */\
		BOOST_PP_CAT(CLASS_TYPE, _data_t)(std::string_view in_feed_file) :\
			manufacturer("ACME Industries"),\
			name(""),\
			type(static_cast<BOOST_PP_CAT(mw_,CLASS_TYPE)>(0)),\
			id(0),\
			vnum(0),\
			rarity(mods::rarity::DEFAULT)\
		{\
			this->feed(in_feed_file);\
		}\
		void feed(std::string_view in_feed_file){\
			this->feed_file = in_feed_file;\
			auto tuple_ptr_id = std::move(mods::weapon_types::feed<mods::yaml::BOOST_PP_CAT(CLASS_TYPE,_description_t)>(feed_file));\
			this->attributes = std::move(std::get<0>(tuple_ptr_id));\
			this->id = std::get<1>(tuple_ptr_id);\
		}\
		uint64_t db_id();\
		std::string feed_file;\
		~BOOST_PP_CAT(CLASS_TYPE,_data_t)() = default;\
		std::unique_ptr<mods::yaml::BOOST_PP_CAT(CLASS_TYPE,_description_t)> attributes;\
		bool has_attr(){ return this->attributes != nullptr; }\
		std::string manufacturer;\
		std::string name;\
		BOOST_PP_CAT(mw_,CLASS_TYPE) type;\
		uint64_t id;\
		int vnum;\
		float rarity;\
	};\

BOOST_PP_SEQ_FOR_EACH(MENTOC_DATA_CLASS, ~, MENTOC_ITEM_TYPES_SEQ)

#undef MENTOC_DATA_CLASS

#endif
