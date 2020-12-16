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
	using random_number_type_t = uint32_t;
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
			__ARMOR_WEAR_FIRST ,
			__ARMOR_WEAR_LAST = ARMOR_ITEM_WEAR_WEAPON_ATTACHMENT
		};
		enum skill_types_t {
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
			__SKILL_TYPE_FIRST ,
			__SKILL_TYPE_LAST = SKILL_MEDICAL
		};
		enum elemental_types_t {
			ELEM_INCENDIARY = 1,
			ELEM_SHRAPNEL ,
			ELEM_CORROSIVE ,
			ELEM_CRYOGENIC ,
			ELEM_EMP ,
			__ELEM_FIRST ,
			__ELEM_LAST = ELEM_EMP
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

		static const std::vector<rifle_attributes_t> valid_rifle_affects = {
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
		static const  std::vector<skill_types_t> valid_armor_affects  = {
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
		static const  std::vector<skill_types_t> valid_armor_requirements_based_on  = {
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

		static const  std::vector<skill_types_t> valid_rifle_requirements_based_on  = {
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



	struct generated_rifle_t {
		rifle_types_t type;
		std::vector<std::pair<skill_types_t,uint32_t>> requirements;
		std::vector<std::pair<rifle_attributes_t,uint32_t>> attributes;
	};

	struct generator {
		using exponential_distr_type_t = std::exponential_distribution<double>;
		using distr_type_t = std::uniform_int_distribution<uint32_t>;
	
		template <typename TGenerator,typename TDistribution>
		void init(){
			m_random_device = std::make_unique<std::random_device>();
			m_generator = std::make_unique<TGenerator>((*m_random_device)());
			m_distribution = std::make_unique<distr_type_t>(1,MAX_FORGE_ENGINE_VALUE);
			m_exp_distribution = std::make_unique<exponential_distr_type_t>((*m_distribution)(*m_generator) / 3.5);
			m_player_level = 10;
			m_bernouli = std::make_unique<std::bernoulli_distribution>(0.5);
			m_bernouli_generator = std::make_unique<std::default_random_engine>();
		}
		bool bool_roll(){
			return (*m_bernouli)(*m_bernouli_generator);
		}
		uint32_t op_based_roll(uint32_t v){
			int operations[4] = { '*', '-', '/', '+' };
			for(int i = 0; i < rand_number(1,36); i++){
				if(v <= 0 || v >= MAX_FORGE_ENGINE_VALUE-20){
					v = 1;
				}
				switch(operations[this->roll() % 4]){
					case '*':
						v *= roll();
						break;
					case '-':
						v -= roll();
						break;
					case '+':
						v += roll();
						break;
				}
			}
			v = std::max((uint32_t)1,v);
			v = std::min((uint32_t)MAX_FORGE_ENGINE_VALUE,v);
			return v;
		}
		bool exp_based_bool_roll(){
			return (0.0421230 * exp_roll() * 0.410 >= ((0.48901824 * exp_roll()) * (bool_roll() ? 0.8 : 0.3)));
		}
		double exp_roll(){
			static std::default_random_engine generator;
			return (*m_exp_distribution)(generator);
		}
		generator();
		~generator();
		rifle_attributes_t random_rifle_attribute();
		std::vector<std::pair<skill_types_t,uint32_t>> generate_rifle_requirements();
		std::vector<std::pair<rifle_attributes_t,uint32_t>> generate_rifle_attributes();

		rifle_types_t random_rifle_type();
		item_types_t random_item_type();


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
		template <typename TEnumType>
		std::vector<std::pair<TEnumType,uint32_t>> generate_random(const std::vector<TEnumType>& valid_values){
			/*
			if(this->roll() >= this->roll()){
				m_generator->discard(m_generator->state_size);
				std::cerr << "D";
			}else{
				std::cerr << ".";
			}
			*/
			std::vector<std::pair<TEnumType,uint32_t>> requirements;
			int i = rand_number(1,3);
			while(--i){
				if((*m_bernouli)(*m_bernouli_generator) && (this->roll() % rand_number(1,10)) <= rand_number(1,3)){
					requirements.emplace_back(valid_values.at(this->roll() % valid_values.size()), this->roll());
				}
			}
			return requirements;
		}

		void run_exp_rolls(int count){
			int exp_rolls[2] = {};
			for(int i = count; i > 0; --i){
				if(exp_based_bool_roll()){
					++exp_rolls[1];
				}else{
					++exp_rolls[0];
				}
			}
			std::cerr << "false: " << exp_rolls[0] << " true: " << exp_rolls[1] << "\n";
		}

			random_number_type_t roll();
		private:
			void init();
			uint32_t m_player_level;
			std::unique_ptr<std::random_device> m_random_device;
			std::unique_ptr<std::mt19937> m_generator;
			std::unique_ptr<distr_type_t> m_distribution;
			std::unique_ptr<exponential_distr_type_t> m_exp_distribution;
			std::unique_ptr<std::bernoulli_distribution> m_bernouli;
			std::unique_ptr<std::default_random_engine> m_bernouli_generator;
	};

};

#endif
