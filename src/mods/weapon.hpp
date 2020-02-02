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
	/*

Weapons and Equipment of Tom Clancy's Rainbow Six Siege
	 L85A2 · AR33 · G36C · R4-C · 556xi · F2 · AK-12 · AUG A2 · 552
Assault Rifles     Commando · 416-C Carbine · C8-SFW · Mk17 CQB · PARA-308 · Type-89
	 · C7E · M762 · V308 · Spear .308 · AR-15.50 · M4 · AK-74M · ARX200
	 · F90 · Commando 9
	 FMG-9 · MP5K · UMP45 · MP5 · P90 · 9×19VSN · MP7 · 9mm C1 · MPX ·
Submachine Guns    M12 · MP5SD · PDW9 · Vector .45 ACP · T-5 SMG · Scorpion EVO 3 A1
	 · K1A · Mx4 Storm · AUG A3 · P10 RONI
	 M590A1 · M1014 · SG-CQB · SASG-12 · M870 · Super 90 · SPAS-12 ·
Shotguns           SPAS-15 · SuperNova · ITA12L · ITA12S · SIX12 · FO-12 · BOSG.12.2
	 · ACS12 · TCSG12 · Super Shorty
Marksman Rifles    417 · OTs-03 · CAMRS · SR-25 · Mk 14 EBR · CSRX 300
Light Machine Guns 6P41 · G8A1 · M249 · T-95 LSW · LMG-E · ALDA 5.56
	 P226 Mk 25 · M45 MEUSOC · 5.7 USG · P9 · LFP586 · GSh-18 · PMM ·
Handguns           P12 · Mk1 9mm · D-50 · PRB92 · P229 · USP40 · Q-929 · RG15 ·
	 Bailiff 410 · Keratos .357 · 1911 TACOPS · P-10C · .44 Mag
	 Semi-Auto · SDP 9mm
Machine Pistols    SMG-11 · Bearing 9 · C75 Auto · SMG-12 · SPSMG9
	 Red Dot Sight · Reflex Sight · Holographic Sight · ACOG Sight ·
Weapon Attachments Suppressor · Flash Hider · Compensator · Muzzle Brake · Extended
	 Barrel · Vertical Grip · Angled Grip · Laser
	 Attackers
	 Ballistic Shield · Breach Charge · Claymore · Frag Grenade · Smoke
	 Grenade · Stun Grenade
Gadgets            Drone · Grappling Hook
	 Defenders
	 Barbed Wire · Bulletproof Camera · Deployable Shield · Impact
	 Grenade · Nitro Cell
	 Barricade · CCTV · Reinforcement
Italic
	 *
	 */
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
	mw_gadget gadget(std::shared_ptr<obj_data>& object);
	std::variant<mw_rifle,mw_explosive,mw_drone> get_type(
			std::shared_ptr<obj_data>&);
	enum attachment_t {
		SCOPE,
		GRIP,
		BARREL,
		MAGAZINE,
		SIDERAIL
	};
	enum barrel_t {
		COMPENSATOR, MUZZLE_BRAKE,EXTENDED,
		FLASH_HIDER, SUPPRESSOR
	};
	enum scope_t {
		ACOG, LASER_DOT, HOLOGRAPHIC,
		MAGNIFICATION_SIX,
		MAGNIFICATION_TWELVE,
		THERMAL
	};
	enum magazine_t {
		EXTENDED_MAGAZINE,
		TRACER_AMMUNITION,
		INCENDIARY_AMMUNITION
	};
	enum grip_t {
		VERTICAL_GRIP, ANGLED_GRIP
	};
	enum siderail_t {
		LASER_SIGHT
	};
	/** Generic attachment methods */
	obj_data_ptr_t attachment(attachment_t type);

	/** Rifle factory methods */
	obj_data_ptr_t new_sniper_rifle_object();

	/** Grenade factory methods */
	obj_data_ptr_t new_frag_grenade_object();
	obj_data_ptr_t new_incendiary_grenade_object();
	obj_data_ptr_t new_emp_grenade_object();
	obj_data_ptr_t new_sensor_grenade_object();
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
			int rounds_per_minute;
			int muzzle_velocity;	// Feet per second
			int effective_firing_range; // feet
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
using weapon_rifle_t = mods::weapon::type::rifle;
using weapon_explosive_t = mods::weapon::type::explosive;
using weapon_drone_t = mods::weapon::type::drone;
namespace mods::weapon {
	static inline std::string to_string(weapon_rifle_t rifle_type){
#define MENTOC_TO_STR(a){ case mw_rifle::a: return #a; }
		switch(rifle_type){
			MENTOC_TO_STR(SHOTGUN);
			MENTOC_TO_STR(ASSAULT_RIFLE);
			MENTOC_TO_STR(SUB_MACHINE_GUN);
			case weapon_rifle_t::SNIPER: return "SNIPER RIFLE";
			MENTOC_TO_STR(HANDGUN);
			MENTOC_TO_STR(MACHINE_PISTOL);
			MENTOC_TO_STR(LIGHT_MACHINE_GUN);
			default: return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline std::string to_string(weapon_explosive_t explosive_type){
#define MENTOC_TO_STR(a){ case mw_explosive::a: return #a; }
		switch(explosive_type){
			MENTOC_TO_STR(FRAG_GRENADE);
			MENTOC_TO_STR(INCENDIARY_GRENADE);
			MENTOC_TO_STR(REMOTE_EXPLOSIVE);
			MENTOC_TO_STR(REMOTE_CHEMICAL);
			MENTOC_TO_STR(EMP_GRENADE);
			MENTOC_TO_STR(CLAYMORE_MINE);
			MENTOC_TO_STR(SMOKE_GRENADE);
			MENTOC_TO_STR(FLASHBANG_GRENADE);
			default: return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline std::string to_string(weapon_drone_t drone_type){
#define MENTOC_TO_STR(a){ case mw_drone::a: return #a; }
		switch(drone_type){
			MENTOC_TO_STR(GROUND_DRONE);
			MENTOC_TO_STR(AERIAL_DRONE);
			MENTOC_TO_STR(AQUATIC_DRONE);
			default: return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline bool has_clip(rifle_data_t* weapon){
		if(!weapon){ return false; }
			switch(weapon->type){
				case mw_rifle::SHOTGUN:
				case mw_rifle::ASSAULT_RIFLE:
				case mw_rifle::SUB_MACHINE_GUN:
				case mw_rifle::SNIPER:
				case mw_rifle::HANDGUN:
				case mw_rifle::MACHINE_PISTOL:
				case mw_rifle::LIGHT_MACHINE_GUN:
					return true;
				default:
					return false;
			}
		return false;
	}

};
#endif

