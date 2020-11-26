#ifndef __MENTOC_MODS_WEAPON_TYPES_HEADER__
#define __MENTOC_MODS_WEAPON_TYPES_HEADER__
#include <vector>
namespace mods::weapon {
	using weapon_accuracy_t = uint16_t;
	using weapon_damage_t = uint16_t;
	using room_distance_t = uint8_t;
	using allow_shot_t = bool;
	struct weapon_type_statistics_t {
		allow_shot_t allow;
		weapon_accuracy_t accuracy;
		weapon_damage_t damage;
	};
	using weapon_stat_list_t = std::vector<weapon_type_statistics_t>;
};

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
		AFFECT_MOVE,
		REMOTELY_VIEW,
		TRIGGER_EXPLOSION,
		PREVENT_RUN,
		TRIGGER_ALERT,
		NOTIFY_OWNER,
		SENSE_MOTION,
		ABSORB_DAMAGE,
		GRAPPLE,
		__LAST = GRAPPLE
	};
};

using cap_t = mods::weapon::capabilities::cap_t;

#include <iostream>
#include "rarity.hpp"

#include "yaml.hpp"
#include "item-types.hpp"


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
