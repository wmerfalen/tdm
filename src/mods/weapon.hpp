#ifndef __MENTOC_MODS_WEAPON_HEADER__
#define __MENTOC_MODS_WEAPON_HEADER__

#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include "weapon-types.hpp"
#include "../structs.h"
#include "pqxx-types.hpp"

#define AMMO(weapon) (weapon->ammo)
#define LOADED(weapon) (weapon->loaded)
#define IS_WEAPON_LOADED(weapon) (weapon->loaded == 1)

struct obj_data;
using obj_data_ptr_t = std::shared_ptr<obj_data>;
using attachment_list_t = std::array<obj_data_ptr_t,6>;
namespace mods::weapon {
	struct mask {
		static constexpr uint64_t snipe = (1 << 0);
		static constexpr uint64_t grenade = (1 << 1);
	};
	enum mask_type { SMG = 1, SNIPE = (1 << 2), SHOTGUN = (1 << 3), 
		GRENADE = (1 << 4), ASSAULT_RIFLE = (1 << 5), PISTOL = (1 << 6),
		MACHINE_PISTOL = (1 << 7), ENERGY = (1 << 8), FUMES = (1 << 9),
		FLAME = (1 << 10), CLAYMORE = (1 << 11), REMOTE_EXPLOSIVE = (1 << 12),
		ATTACK_DRONE = (1 << 13)
	};
	
	mw_rifle rifle(std::shared_ptr<obj_data>& object);
	mw_explosive explosive(std::shared_ptr<obj_data>& object);
	mw_drone drone(std::shared_ptr<obj_data>& object);
	std::variant<mw_rifle,mw_explosive,mw_drone> get_type(
			std::shared_ptr<obj_data>&);
	enum attachment_t {
		SCOPE = 0,
		GRIP = 1,
		BARREL = 2,
		MAGAZINE = 3,
		SIDERAIL = 4,
		UNDER_BARREL = 5
	};
	/** Generic attachment methods */
	obj_data_ptr_t attachment(attachment_t type);

	/** Rifle factory methods */
	obj_data_ptr_t new_sniper_rifle_object();

	/** Grenade factory methods */
	obj_data_ptr_t new_frag_grenade_object();
	obj_data_ptr_t new_incendiary_grenade_object();
	obj_data_ptr_t new_emp_grenade_object();
	obj_data_ptr_t new_smoke_grenade_object();
	obj_data_ptr_t new_flashbang_grenade_object();

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
			mw_rifle type;
			int ammo_max;
			float chance_to_injure;
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
	using explosive_description_t = mods::yaml::explosive_description_t;
	
//		~explosive_description_t() = default;
//		explosive_description_t() :
//			chance_to_injure(0.0),	 /** Percent */
//			critical_chance(0.0), /** Percent */
//			critical_range(0), /** Rooms */
//			blast_radius(1),	/** 1 will only affect room it detonates in */
//			damage_per_second(0.0), /** static amount of damage done per second after detonation */
//			disorient_amount(0.0) /** percent */
//		{}
//		float chance_to_injure;
//		float critical_chance;
//		int critical_range;
//		int blast_radius;	/** In rooms */
//		float damage_per_second;
//		float disorient_amount;
//		std::string manufacturer;
//		type::explosive type;
//	};
};
#endif

