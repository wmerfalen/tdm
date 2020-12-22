#ifndef __MENTOC_MODS_FORGE_ENGINE_GENERATOR_HEADER__
#define __MENTOC_MODS_FORGE_ENGINE_GENERATOR_HEADER__
#include "../../structs.h"
#include "../pq.hpp"
#include "../sql.hpp"
#include <memory>
#include "../object-utils.hpp"
#include "../rand.hpp"

namespace mods::forge_engine {
	using sql_compositor = mods::sql::compositor<mods::pq::transaction>;
	using random_number_type_t = uint64_t;
	static constexpr random_number_type_t MAX_FORGE_ENGINE_VALUE = 4294967295;
		enum item_types_t {
			TYPE_ITEM_RIFLE = 1,
			TYPE_ITEM_EXPLOSIVE ,
			TYPE_ITEM_GADGET   ,
			TYPE_ITEM_DRONE    ,
			TYPE_ITEM_WEAPON   ,
			TYPE_ITEM_ATTACHMENT ,
			TYPE_ITEM_ARMOR    ,
			TYPE_ITEM_CONSUMABLE ,
			TYPE_ITEM_TRAP    ,
			__TYPE_ITEM_FIRST = TYPE_ITEM_RIFLE,
			__TYPE_ITEM_LAST = TYPE_ITEM_TRAP
		};
		enum rifle_types_t {
			RIFLE_TYPE_SHOTGUN = 1,
			RIFLE_TYPE_ASSAULT_RIFLE,
			RIFLE_TYPE_SUB_MACHINE_GUN,
			RIFLE_TYPE_SNIPER,
			RIFLE_TYPE_HANDGUN,
			RIFLE_TYPE_PISTOL,
			RIFLE_TYPE_MACHINE_PISTOL,
			RIFLE_TYPE_LIGHT_MACHINE_GUN,
			__RIFLE_TYPE_FIRST=RIFLE_TYPE_SHOTGUN,
			__RIFLE_TYPE_LAST=RIFLE_TYPE_LIGHT_MACHINE_GUN
		};
		enum rifle_attributes_t {
			RIFLE_ATTRIBUTES_AMMO_MAX = 1,
			RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,
			RIFLE_ATTRIBUTES_CLIP_SIZE,
			RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,
			RIFLE_ATTRIBUTES_CRITICAL_CHANCE,
			RIFLE_ATTRIBUTES_CRITICAL_RANGE,
			RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND,
			RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,
			RIFLE_ATTRIBUTES_HEADSHOT_BONUS,
			RIFLE_ATTRIBUTES_MAX_RANGE,
			RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,
			RIFLE_ATTRIBUTES_RELOAD_TIME,
			RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,
			RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,
			RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,
			RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
			RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES,
			__RIFLE_ATTRIBUTES_FIRST = RIFLE_ATTRIBUTES_AMMO_MAX,
			__RIFLE_ATTRIBUTES_LAST = RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
		};
		enum explosive_types_t {
			EXPLOSIVE_TYPE_FRAG_GRENADE = 1,
			EXPLOSIVE_TYPE_INCENDIARY_GRENADE,
			EXPLOSIVE_TYPE_REMOTE_EXPLOSIVE,
			EXPLOSIVE_TYPE_REMOTE_CHEMICAL,
			EXPLOSIVE_TYPE_EMP_GRENADE,
			EXPLOSIVE_TYPE_CLAYMORE_MINE,
			EXPLOSIVE_TYPE_SMOKE_GRENADE,
			EXPLOSIVE_TYPE_C4,
			EXPLOSIVE_TYPE_BREACH_CHARGE,
			EXPLOSIVE_TYPE_THERMITE_CHARGE,
			EXPLOSIVE_TYPE_FLASHBANG_GRENADE,
			EXPLOSIVE_TYPE_SENSOR_GRENADE,
			__EXPLOSIVE_TYPE_FIRST = EXPLOSIVE_TYPE_FRAG_GRENADE,
			__EXPLOSIVE_TYPE_LAST = EXPLOSIVE_TYPE_SENSOR_GRENADE
		};
		enum attachment_types_t {
			ATTACHMENT_TYPE_SIGHT = 1,
			ATTACHMENT_TYPE_UNDER_BARREL,
			ATTACHMENT_TYPE_GRIP,
			ATTACHMENT_TYPE_BARREL,
			ATTACHMENT_TYPE_MUZZLE,
			ATTACHMENT_TYPE_MAGAZINE,
			ATTACHMENT_TYPE_STOCK,
			ATTACHMENT_TYPE_STRAP,
			__ATTACHMENT_TYPE_FIRST = ATTACHMENT_TYPE_SIGHT,
			__ATTACHMENT_TYPE_LAST = ATTACHMENT_TYPE_STRAP
		};

		enum armor_attributes_t {
			ARMOR_ATTR_THAC0 = 1,
			ARMOR_ATTR_WEIGHT,
			ARMOR_ATTR_INCENDIARY_RESISTANCE_PERCENT,
			ARMOR_ATTR_EXPLOSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHRAPNEL_RESISTANCE_PERCENT,
			ARMOR_ATTR_CORROSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_CRYOGENIC_RESISTANCE_PERCENT,
			ARMOR_ATTR_RADIATION_RESISTANCE_PERCENT,
			ARMOR_ATTR_EMP_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHOCK_RESISTANCE_PERCENT,
			ARMOR_ATTR_ANTI_MATTER_RESISTANCE_PERCENT,
			ARMOR_ATTR_BALISTIC_RESISTANCE_PERCENT,
			ARMOR_ATTR_SPEED_PROFILE,
			ARMOR_ATTR_DURABLITY_PROFILE,
			ARMOR_ATTR_REPAIR_COST_PROFILE,
			ARMOR_ATTR_HP,
			ARMOR_ATTR_CLASSIFICATION,
			ARMOR_ATTR_WORTH,
			__ARMOR_ATTR_FIRST = ARMOR_ATTR_THAC0,
			__ARMOR_ATTR_LAST = ARMOR_ATTR_WORTH,
		};

		enum armor_wear_types_t {
			ARMOR_ITEM_WEAR_FINGER	=  1,
			ARMOR_ITEM_WEAR_NECK		,
			ARMOR_ITEM_WEAR_BODY		,
			ARMOR_ITEM_WEAR_HEAD		,
			ARMOR_ITEM_WEAR_LEGS		,
			ARMOR_ITEM_WEAR_FEET		,
			ARMOR_ITEM_WEAR_HANDS		,
			ARMOR_ITEM_WEAR_ARMS		,
			ARMOR_ITEM_WEAR_SHIELD	,
			ARMOR_ITEM_WEAR_ABOUT		,
			ARMOR_ITEM_WEAR_WAIST 	,
			ARMOR_ITEM_WEAR_WRIST		,
			ARMOR_ITEM_WEAR_WIELD		,
			ARMOR_ITEM_WEAR_HOLD		,
			ARMOR_ITEM_WEAR_SECONDARY	,
			ARMOR_ITEM_WEAR_SHOULDERS	,
			ARMOR_ITEM_WEAR_VEST_PACK	,
			ARMOR_ITEM_WEAR_ELBOW	,
			ARMOR_ITEM_WEAR_BACKPACK ,
			ARMOR_ITEM_WEAR_GOGGLES ,
			ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT ,
			__ARMOR_WEAR_FIRST = ARMOR_ITEM_WEAR_FINGER,
			__ARMOR_WEAR_LAST = ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT
		};
		enum stat_types_t {
			SKILL_STR = 1,
			SKILL_STR_ADD ,
			SKILL_INTEL ,
			SKILL_WIS ,
			SKILL_DEX ,
			SKILL_CON ,
			SKILL_CHA ,
			SKILL_ELECTRONICS ,
			SKILL_ARMOR ,
			SKILL_MARKSMANSHIP ,
			SKILL_SNIPING ,
			SKILL_DEMOLITIONS ,
			SKILL_CHEMISTRY ,
			SKILL_WEAPON_HANDLING ,
			SKILL_STRATEGY ,
			SKILL_MEDICAL ,
			__SKILL_TYPE_FIRST = SKILL_STR,
			__SKILL_TYPE_LAST = SKILL_MEDICAL
		};
		enum elemental_types_t {
			ELEM_INCENDIARY = 1,
			ELEM_EXPLOSIVE,
			ELEM_SHRAPNEL ,
			ELEM_CORROSIVE ,
			ELEM_CRYOGENIC ,
			ELEM_RADIOACTIVE,
			ELEM_EMP ,
			ELEM_SHOCK,
			ELEM_ANTI_MATTER, /** combined damage of EMP and RADIOACTIVE */
			__ELEM_FIRST = ELEM_INCENDIARY,
			__ELEM_LAST = ELEM_ANTI_MATTER,
		};
		enum player_class_types_t {
			PLCLASS_SNIPER = 1,
			PLCLASS_MARINE ,
			PLCLASS_BREACHER ,
			PLCLASS_ENGINEER ,
			PLCLASS_MEDIC ,
			PLCLASS_GHOST ,
			PLCLASS_SUPPORT ,
			__PLCLASS_FIRST = PLCLASS_SNIPER,
			__PLCLASS_LAST = PLCLASS_SUPPORT
		};

		static const std::vector<armor_attributes_t> valid_armor_attributes = {
			ARMOR_ATTR_THAC0,
			ARMOR_ATTR_WEIGHT,
			ARMOR_ATTR_INCENDIARY_RESISTANCE_PERCENT,
			ARMOR_ATTR_EXPLOSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHRAPNEL_RESISTANCE_PERCENT,
			ARMOR_ATTR_CORROSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_CRYOGENIC_RESISTANCE_PERCENT,
			ARMOR_ATTR_RADIATION_RESISTANCE_PERCENT,
			ARMOR_ATTR_EMP_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHOCK_RESISTANCE_PERCENT,
			ARMOR_ATTR_ANTI_MATTER_RESISTANCE_PERCENT,
			ARMOR_ATTR_BALISTIC_RESISTANCE_PERCENT,
			ARMOR_ATTR_SPEED_PROFILE,
			ARMOR_ATTR_DURABLITY_PROFILE,
			ARMOR_ATTR_REPAIR_COST_PROFILE,
			ARMOR_ATTR_HP,
			ARMOR_ATTR_CLASSIFICATION,
			ARMOR_ATTR_WORTH
		};
		static const std::vector<elemental_types_t> valid_elemental_types = {
			ELEM_INCENDIARY,
			ELEM_EXPLOSIVE,
			ELEM_SHRAPNEL ,
			ELEM_CORROSIVE ,
			ELEM_CRYOGENIC ,
			ELEM_RADIOACTIVE,
			ELEM_EMP ,
			ELEM_SHOCK,
			ELEM_ANTI_MATTER
		};

		static const std::vector<armor_attributes_t> armor_percentages = {
			ARMOR_ATTR_INCENDIARY_RESISTANCE_PERCENT,
			ARMOR_ATTR_EXPLOSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHRAPNEL_RESISTANCE_PERCENT,
			ARMOR_ATTR_CORROSIVE_RESISTANCE_PERCENT,
			ARMOR_ATTR_CRYOGENIC_RESISTANCE_PERCENT,
			ARMOR_ATTR_RADIATION_RESISTANCE_PERCENT,
			ARMOR_ATTR_EMP_RESISTANCE_PERCENT,
			ARMOR_ATTR_SHOCK_RESISTANCE_PERCENT,
			ARMOR_ATTR_ANTI_MATTER_RESISTANCE_PERCENT,
			ARMOR_ATTR_BALISTIC_RESISTANCE_PERCENT,
		};

		static const std::vector<elemental_types_t> empty_elemental_types = { };

		static const std::vector<rifle_attributes_t> valid_rifle_attributes = {
			RIFLE_ATTRIBUTES_AMMO_MAX,
			RIFLE_ATTRIBUTES_CHANCE_TO_INJURE,
			RIFLE_ATTRIBUTES_CLIP_SIZE,
			RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,
			RIFLE_ATTRIBUTES_CRITICAL_CHANCE,
			RIFLE_ATTRIBUTES_CRITICAL_RANGE,
			RIFLE_ATTRIBUTES_DAMAGE_PER_SECOND,
			RIFLE_ATTRIBUTES_DISORIENT_AMOUNT,
			RIFLE_ATTRIBUTES_HEADSHOT_BONUS,
			RIFLE_ATTRIBUTES_MAX_RANGE,
			RIFLE_ATTRIBUTES_RANGE_MULTIPLIER,
			RIFLE_ATTRIBUTES_RELOAD_TIME,
			RIFLE_ATTRIBUTES_ROUNDS_PER_MINUTE,
			RIFLE_ATTRIBUTES_MUZZLE_VELOCITY,
			RIFLE_ATTRIBUTES_EFFECTIVE_FIRING_RANGE,
			RIFLE_ATTRIBUTES_DAMAGE_DICE_COUNT,
			RIFLE_ATTRIBUTES_DAMAGE_DICE_SIDES
		};
		static const  std::vector<stat_types_t> valid_rifle_stats_boosts = {
			SKILL_STR,
			SKILL_INTEL,
			SKILL_WIS,
			SKILL_DEX,
			SKILL_CON,
			SKILL_ELECTRONICS,
			SKILL_MARKSMANSHIP,
			SKILL_SNIPING,
			SKILL_DEMOLITIONS,
			SKILL_CHEMISTRY,
			SKILL_WEAPON_HANDLING,
			SKILL_STRATEGY
		};

		/** dictates what types of items will be generated */
		static const  std::vector<item_types_t> active_item_types = {
			TYPE_ITEM_RIFLE,
			TYPE_ITEM_EXPLOSIVE,
			TYPE_ITEM_ATTACHMENT,
			TYPE_ITEM_ARMOR
		};
		static const  std::vector<rifle_types_t> valid_rifle_types = {
			RIFLE_TYPE_SHOTGUN,
			RIFLE_TYPE_ASSAULT_RIFLE,
			RIFLE_TYPE_SUB_MACHINE_GUN,
			RIFLE_TYPE_SNIPER,
			RIFLE_TYPE_HANDGUN,
			RIFLE_TYPE_PISTOL,
			RIFLE_TYPE_MACHINE_PISTOL,
			RIFLE_TYPE_LIGHT_MACHINE_GUN
		};
		static const  std::vector<armor_wear_types_t> valid_armor_placements  = {
			ARMOR_ITEM_WEAR_FINGER,
			ARMOR_ITEM_WEAR_NECK,
			ARMOR_ITEM_WEAR_BODY,
			ARMOR_ITEM_WEAR_HEAD,
			ARMOR_ITEM_WEAR_LEGS,
			ARMOR_ITEM_WEAR_FEET,
			ARMOR_ITEM_WEAR_HANDS,
			ARMOR_ITEM_WEAR_ARMS,
			ARMOR_ITEM_WEAR_ABOUT,
			ARMOR_ITEM_WEAR_WAIST,
			ARMOR_ITEM_WEAR_WRIST,
			ARMOR_ITEM_WEAR_SHOULDERS,
			ARMOR_ITEM_WEAR_ELBOW
		};
		static const  std::vector<stat_types_t> valid_armor_affects  = {
			SKILL_STR,
			SKILL_INTEL,
			SKILL_WIS,
			SKILL_DEX,
			SKILL_CON,
			SKILL_CHA,
			SKILL_ELECTRONICS,
			SKILL_ARMOR,
			SKILL_MARKSMANSHIP,
			SKILL_SNIPING,
			SKILL_DEMOLITIONS,
			SKILL_CHEMISTRY,
			SKILL_WEAPON_HANDLING,
			SKILL_STRATEGY,
			SKILL_MEDICAL
		};
		static const  std::vector<stat_types_t> valid_armor_requirements_based_on  = {
			SKILL_STR,
			SKILL_INTEL,
			SKILL_WIS,
			SKILL_DEX,
			SKILL_CON,
			SKILL_CHA,
			SKILL_ELECTRONICS,
			SKILL_ARMOR,
			SKILL_MARKSMANSHIP,
			SKILL_SNIPING,
			SKILL_DEMOLITIONS,
			SKILL_CHEMISTRY,
			SKILL_WEAPON_HANDLING,
			SKILL_STRATEGY,
			SKILL_MEDICAL
		};

		static const  std::vector<stat_types_t> valid_rifle_requirements_based_on  = {
			SKILL_STR,
			SKILL_INTEL,
			SKILL_WIS,
			SKILL_DEX,
			SKILL_CON,
			SKILL_ELECTRONICS,
			SKILL_MARKSMANSHIP,
			SKILL_SNIPING,
			SKILL_DEMOLITIONS,
			SKILL_CHEMISTRY,
			SKILL_WEAPON_HANDLING,
			SKILL_STRATEGY
		};

		static const std::vector<rifle_attributes_t> negative_buffs = {
			RIFLE_ATTRIBUTES_COOLDOWN_BETWEEN_SHOTS,
			RIFLE_ATTRIBUTES_CRITICAL_RANGE,
			RIFLE_ATTRIBUTES_RELOAD_TIME
		};

	struct generated_rifle_t {
		rifle_types_t type;
		std::vector<std::pair<stat_types_t,uint32_t>> requirements;
		std::vector<std::pair<rifle_attributes_t,uint32_t>> attributes;
	};

	struct generator {
		using exponential_distr_type_t = std::exponential_distribution<double>;
		using distr_type_t = std::uniform_int_distribution<uint32_t>;
	
		generator();
		~generator();

		rifle_attributes_t random_rifle_attribute();
		rifle_types_t random_rifle_type();
		item_types_t random_item_type();
		armor_wear_types_t random_armor_type(){
			return (armor_wear_types_t)(this->roll_between((uint8_t)__ARMOR_WEAR_FIRST,(uint8_t)__ARMOR_WEAR_LAST));
		}
		elemental_types_t random_elemental_type(){
			return (elemental_types_t)(this->roll_between((uint8_t)__ELEM_FIRST, (uint8_t)__ELEM_LAST));
		}
		elemental_types_t random_elemental_resistance(armor_wear_types_t type){
			return (elemental_types_t)(this->roll_between((uint8_t)__ELEM_FIRST, (uint8_t)__ELEM_LAST));
		}
		explosive_types_t random_explosive_type(){
			return (explosive_types_t)(this->roll_between((uint8_t)__EXPLOSIVE_TYPE_FIRST, (uint8_t)__EXPLOSIVE_TYPE_LAST));
		}
		attachment_types_t random_attachment_type(){
			return (attachment_types_t)(this->roll_between((uint8_t)__ATTACHMENT_TYPE_FIRST, (uint8_t)__ATTACHMENT_TYPE_LAST));
		};

		/** random rifle functions */
		/** random rifle functions */
		std::vector<std::pair<stat_types_t,uint32_t>> generate_rifle_requirements(player_ptr_t& player);

		std::vector<std::pair<rifle_attributes_t,std::variant<uint32_t,float>>> generate_rifle_attributes(player_ptr_t& player);
		std::vector<std::pair<stat_types_t,std::variant<uint32_t,float>>> generate_rifle_stat_boosts(player_ptr_t& player);
		std::vector<std::pair<elemental_types_t,std::variant<uint32_t,float>>> generate_rifle_elemental_boosts(player_ptr_t& player);
		/** end random rifle functions */

		int pclass_to_real_pclass(player_class_types_t type){
			switch(type){
				case PLCLASS_SNIPER:
					return CLASS_SNIPER;
				case PLCLASS_MARINE:
					return CLASS_MARINE;
				case PLCLASS_BREACHER:
					return CLASS_BREACHER;
				case PLCLASS_ENGINEER:
					return CLASS_ENGINEER;
				case PLCLASS_MEDIC:
					return CLASS_MEDIC;
				case PLCLASS_GHOST:
					return CLASS_GHOST;
				case PLCLASS_SUPPORT:
					return CLASS_SUPPORT;
				default:
					return -1;
			}
		}

		float roll_float(float LO, float HI){
			return LO + static_cast <float> (rand_xoroshiro()) /( static_cast <float> (UINT64_MAX/(HI-LO)));
		}

		template <typename TUintWidth>
		TUintWidth roll_between(TUintWidth LO, TUintWidth HI){
			return LO + static_cast <TUintWidth> (rand_xoroshiro()) /( static_cast <TUintWidth> (std::numeric_limits<TUintWidth>::max()/(HI-LO)));
		}


		/**
		 * Generate a vector of pairs.
		 * Pair.first = a randomly chosen enum in valid_attributes.
		 * Pair.second = a variant of type TUintWidth or float
		 * Does NOT check for duplicates.
		 */
		template <typename TEnumType,typename TUintWidth>
		std::vector<std::pair<TEnumType,std::variant<TUintWidth,float>>> generate_random_mixed(
				const std::vector<TEnumType>& valid_attributes,
				float float_low,
				float float_high,
				TUintWidth uint_low,
				TUintWidth uint_high,
				uint8_t max_attributes
		){
			std::vector<std::pair<TEnumType,std::variant<TUintWidth,float>>> attributes;
			if(valid_attributes.size() == 0){
				return attributes;
			}
			uint8_t i = std::clamp(this->roll<uint8_t>(),(uint8_t)0,(uint8_t)(max_attributes));
			if(!i){
				return attributes;
			}
			while(i-- > 0){
				if(roll<bool>()){
					attributes.emplace_back(
							valid_attributes.at(this->roll<uint16_t>() % valid_attributes.size()),
							roll_float(float_low,float_high)
					);
				}else{
					attributes.emplace_back(
							valid_attributes.at(this->roll<uint16_t>() % valid_attributes.size()),
							this->roll_between<TUintWidth>(uint_low,uint_high)
					);
				}
			}
			return attributes;
		}

		/**
		 * Generate a vector of pairs.
		 * Pair.first = a randomly chosen enum in valid_attributes.
		 * Pair.second = a value of type TMeasurementType
		 * Does NOT check for duplicates.
		 */
		template <typename TEnumType,typename TMeasurementType,uint8_t TNarrowAmount>
		std::vector<std::pair<TEnumType,TMeasurementType>> generate_random(
				const std::vector<TEnumType>& valid_values,
				uint8_t max_requirements
		){
			std::vector<std::pair<TEnumType,TMeasurementType>> requirements;
			if(valid_values.size() == 0){
				return requirements;
			}
			auto i = std::clamp(this->roll<uint8_t>(),(uint8_t)0,max_requirements);
			if(!i){
				return requirements;
			}
			while(i-- > 0){
				requirements.emplace_back(valid_values.at(this->roll<uint16_t>() % valid_values.size()), this->roll<TMeasurementType>());
			}
			return requirements;
		}

		/**
		 * roll a random integer
		 */
		template <typename TRandomType>
		TRandomType roll(){
			TRandomType x = rand_xoroshiro();
			if(x == 0){
				return 0;
			}
			return x % std::numeric_limits<TRandomType>::max();
		}

		private:
			void init();
			uint32_t m_player_level;
	};

};

#endif
