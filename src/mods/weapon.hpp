#ifndef __MENTOC_MODS_WEAPON_HEADER__
#define __MENTOC_MODS_WEAPON_HEADER__
#include "values.hpp"

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
using obj_ptr_t = std::shared_ptr<obj_data>;
using attachment_list_t = std::array<obj_data_ptr_t,6>;
extern pqxx::result db_get_by_meta(std::string table, std::string col,const std::string& value);



namespace mods::weapon {
	static constexpr int FIND_OBJ_INV       = (1 << 2);
	static constexpr int FIND_OBJ_EQUIP     = (1 << 5);

	using cap_t = mods::weapon::capabilities::cap_t;
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
	/** TODO: store in db. !DIABLO */
	static constexpr uint8_t MAX_RANGE = 4;

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
				ammo_type("") {
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
using weapon_gadget_t = mods::weapon::type::gadget;
using weapon_attachment_t = mods::weapon::type::attachment;
using weapon_armor_t = mods::weapon::type::armor;
using weapon_consumable_t = mods::weapon::type::consumable;
using weapon_trap_t = mods::weapon::type::trap;
using weapon_container_t = mods::weapon::type::container;
namespace mods::weapon {

	static inline std::string to_string(weapon_trap_t trap_type) {
#define MENTOC_TO_STR(a){ case mw_trap::a: return #a; }
		switch(trap_type) {
				MENTOC_TO_STR(BEAR_TRAP);
				MENTOC_TO_STR(POSITION_AMPLIFIER);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}

	static inline std::string to_string(weapon_armor_t armor_type) {
#define MENTOC_TO_STR(a){ case mw_armor::a: return #a; }
		switch(armor_type) {
				MENTOC_TO_STR(VEST);
				MENTOC_TO_STR(LEGS);
				MENTOC_TO_STR(GAUNTLETS);
				MENTOC_TO_STR(SHOULDER_PADS);
				MENTOC_TO_STR(HELMET);
				MENTOC_TO_STR(HAT);
				MENTOC_TO_STR(EYEWEAR);
				MENTOC_TO_STR(GOGGLES);
				MENTOC_TO_STR(BACKPACK);
				MENTOC_TO_STR(WAIST_PACK);
				MENTOC_TO_STR(VEST_PACK);
				MENTOC_TO_STR(ELBOW_GUARDS);
				MENTOC_TO_STR(GLOVES);
				MENTOC_TO_STR(SHIELD);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}

	static inline std::string to_string(weapon_consumable_t consumable_type) {
#define MENTOC_TO_STR(a){ case mw_consumable::a: return #a; }
		switch(consumable_type) {
				MENTOC_TO_STR(AMMUNITION);
				MENTOC_TO_STR(FOOD);
				MENTOC_TO_STR(PILLS);
				MENTOC_TO_STR(HERBS);
				MENTOC_TO_STR(HALLUCINOGENS);
				MENTOC_TO_STR(PEDS);
				MENTOC_TO_STR(GASOLINE);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}

	static inline std::string to_string(weapon_attachment_t attachment_type) {
#define MENTOC_TO_STR(a){ case mw_attachment::a: return #a; }
		switch(attachment_type) {
				MENTOC_TO_STR(SIGHT);
				MENTOC_TO_STR(MUZZLE);
				MENTOC_TO_STR(MAGAZINE);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline weapon_rifle_t from_string_to_rifle(std::string_view rifle_type) {
#define MENTOC_TO_STR(a){ if(rifle_type.compare(#a) == 0){ return mw_rifle::a; } }
		MENTOC_TO_STR(HANDGUN);
		MENTOC_TO_STR(PISTOL);
		MENTOC_TO_STR(MACHINE_PISTOL);
		MENTOC_TO_STR(SHOTGUN);
		MENTOC_TO_STR(ASSAULT_RIFLE);
		MENTOC_TO_STR(SUB_MACHINE_GUN);
		MENTOC_TO_STR(LIGHT_MACHINE_GUN);
		MENTOC_TO_STR(SNIPER);
#undef MENTOC_TO_STR
		return mw_rifle::ASSAULT_RIFLE;
	}
	static inline std::string to_string(weapon_rifle_t rifle_type) {
#define MENTOC_TO_STR(a){ case mw_rifle::a: return #a; }
		switch((mw_rifle)rifle_type) {
				MENTOC_TO_STR(HANDGUN);
				MENTOC_TO_STR(PISTOL);
				MENTOC_TO_STR(MACHINE_PISTOL);
				MENTOC_TO_STR(SHOTGUN);
				MENTOC_TO_STR(ASSAULT_RIFLE);
				MENTOC_TO_STR(SUB_MACHINE_GUN);
			case weapon_rifle_t::SNIPER:
				return "SNIPER RIFLE";
				MENTOC_TO_STR(LIGHT_MACHINE_GUN);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline std::string to_string(weapon_explosive_t explosive_type) {
#define MENTOC_TO_STR(a){ case mw_explosive::a: return #a; }
		switch(explosive_type) {
				MENTOC_TO_STR(FRAG_GRENADE);
				MENTOC_TO_STR(INCENDIARY_GRENADE);
				MENTOC_TO_STR(REMOTE_EXPLOSIVE);
				MENTOC_TO_STR(REMOTE_CHEMICAL);
				MENTOC_TO_STR(EMP_GRENADE);
				MENTOC_TO_STR(CLAYMORE_MINE);
				MENTOC_TO_STR(SMOKE_GRENADE);
				MENTOC_TO_STR(FLASHBANG_GRENADE);
				MENTOC_TO_STR(C4);
				MENTOC_TO_STR(BREACH_CHARGE);
				MENTOC_TO_STR(THERMITE_CHARGE);
				MENTOC_TO_STR(SENSOR_GRENADE);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline std::string to_string(weapon_gadget_t gadget_type) {
#define MENTOC_TO_STR(a){ case mw_gadget::a: return #a; }
		switch(gadget_type) {
				MENTOC_TO_STR(GRAPPLING_HOOK);
				MENTOC_TO_STR(BARBED_WIRE);
				MENTOC_TO_STR(CAMERA);
				MENTOC_TO_STR(THERMAL_CAMERA);
				MENTOC_TO_STR(NIGHT_VISION_CAMERA);
				MENTOC_TO_STR(MOTION_SENSOR);
				MENTOC_TO_STR(DEPLOYABLE_SHIELD);
				MENTOC_TO_STR(TRIPWIRE);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline std::string to_string(weapon_drone_t drone_type) {
#define MENTOC_TO_STR(a){ case mw_drone::a: return #a; }
		switch(drone_type) {
				MENTOC_TO_STR(GROUND_DRONE);
				MENTOC_TO_STR(AERIAL_DRONE);
				MENTOC_TO_STR(AQUATIC_DRONE);
			default:
				return "<unknown>";
#undef MENTOC_TO_STR
		}
	}
	static inline bool has_clip(rifle_data_t* weapon) {
		if(!weapon) {
			return false;
		}
		switch(weapon->type) {
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
	namespace breacher {
		enum primary_choice_t {
			NONE = 0,
			MP5 = 1,
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define SNI_STR(a){ case mods::weapon::breacher::primary_choice_t::a: return #a; }
			switch(s) {
					SNI_STR(NONE);
					SNI_STR(MP5);
#undef SNI_STR
				default:
					return "<unknown>";
			}
		}
	};
	/** ghost class */
	namespace ghost {
		enum primary_choice_t {
			GHOST_NONE = 0,
			GHOST_PRIMARY_PSG1 = 1,
			GHOST_PRIMARY_L96AW = 2,
			NONE = 0,
			PSG1 = 1,
			L96AW = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define SNI_STR(a){ case mods::weapon::ghost::primary_choice_t::a: return #a; }
			switch(s) {
					SNI_STR(NONE);
					SNI_STR(PSG1);
					SNI_STR(L96AW);
#undef SNI_STR
				default:
					return "<unknown>";
			}
		}
		enum secondary_choice_t {
			GHOST_SECONDARY_NONE = 0,
			GHOST_SECONARY_PSG1 = 1,
			GHOST_SECONARY_L96AW = 2,
		};
		static inline std::string_view to_string(secondary_choice_t s) {
#define SNI_STR(a){ case mods::weapon::ghost::secondary_choice_t::a: return #a; }
			switch(s) {
					SNI_STR(GHOST_SECONDARY_NONE);
					SNI_STR(GHOST_SECONARY_PSG1);
					SNI_STR(GHOST_SECONARY_L96AW);
#undef SNI_STR
				default:
					return "<unknown>";
			}
		}
	};

	namespace sniper {
		enum primary_choice_t {
			SNIPER_NONE = 0,
			SNIPER_PRIMARY_PSG1 = 1,
			SNIPER_PRIMARY_L96AW = 2,
			NONE = 0,
			PSG1 = 1,
			L96AW = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define SNI_STR(a){ case mods::weapon::sniper::primary_choice_t::a: return #a; }
			switch(s) {
					SNI_STR(NONE);
					SNI_STR(PSG1);
					SNI_STR(L96AW);
#undef SNI_STR
				default:
					return "<unknown>";
			}
		}
		enum secondary_choice_t {
			SNIPER_SECONDARY_NONE = 0,
			SNIPER_SECONARY_PSG1 = 1,
			SNIPER_SECONARY_L96AW = 2,
		};
		static inline std::string_view to_string(secondary_choice_t s) {
#define SNI_STR(a){ case mods::weapon::sniper::secondary_choice_t::a: return #a; }
			switch(s) {
					SNI_STR(SNIPER_SECONDARY_NONE);
					SNI_STR(SNIPER_SECONARY_PSG1);
					SNI_STR(SNIPER_SECONARY_L96AW);
#undef SNI_STR
				default:
					return "<unknown>";
			}
		}
	};
#undef SNI_STR
	namespace marine {
		enum primary_choice_t {
			MARINE_NONE = 0,
			MARINE_PRIMARY_M16A4 = 1,
			MARINE_PRIMARY_M4 = 2,
			NONE = 0,
			M16A4 = 1,
			M4 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::marine::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(MARINE_NONE);
					MAR_STR(MARINE_PRIMARY_M16A4);
					MAR_STR(MARINE_PRIMARY_M4);
				default:
					return "<unknown>";
			}
		}
	};
#undef MAR_STR
	namespace contagion {
		enum primary_choice_t {
			CONTAGION_NONE = 0,
			CONTAGION_PRIMARY_M3 = 1,
			CONTAGION_PRIMARY_FAMAS = 2,
			NONE = 0,
			M3 = 1,
			FAMAS = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::contagion::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(CONTAGION_NONE);
					MAR_STR(CONTAGION_PRIMARY_M3);
					MAR_STR(CONTAGION_PRIMARY_FAMAS);
				default:
					return "<unknown>";
			}
		}
	};
#undef MAR_STR
	namespace engineer {
		enum primary_choice_t {
			ENGINEER_NONE = 0,
			ENGINEER_PRIMARY_FMG9 = 1,
			ENGINEER_PRIMARY_P90 = 2,
			NONE = 0,
			FMG9 = 1,
			P90 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::engineer::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(ENGINEER_NONE);
					MAR_STR(ENGINEER_PRIMARY_FMG9);
					MAR_STR(ENGINEER_PRIMARY_P90);
				default:
					return "<unknown>";
			}
		}
	};
#undef MAR_STR
	namespace medic {
		enum primary_choice_t {
			MEDIC_NONE = 0,
			MEDIC_PRIMARY_AUGPARA = 1,
			MEDIC_PRIMARY_TAR21 = 2,
			NONE = 0,
			AUGPARA = 1,
			TAR21 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::medic::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(MEDIC_NONE);
					MAR_STR(MEDIC_PRIMARY_AUGPARA);
					MAR_STR(MEDIC_PRIMARY_TAR21);
				default:
					return "<unknown>";
			}
		}
		enum secondary_choice_t {
			CZP10 = 1,
		};
	};
#undef MAR_STR
	namespace psyop {
		enum primary_choice_t {
			PSYOP_NONE = 0,
			PSYOP_PRIMARY_SCARH = 1,
			PSYOP_PRIMARY_UMP45 = 2,
			NONE = 0,
			SCARH = 1,
			UMP45 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::psyop::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(PSYOP_NONE);
					MAR_STR(PSYOP_PRIMARY_SCARH);
					MAR_STR(PSYOP_PRIMARY_UMP45);
				default:
					return "<unknown>";
			}
		}
	};
#undef MAR_STR
	namespace support {
		enum primary_choice_t {
			SUPPORT_NONE = 0,
			SUPPORT_PRIMARY_MK46 = 1,
			SUPPORT_PRIMARY_HK21 = 2,
			NONE = 0,
			MK46 = 1,
			HK21 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define MAR_STR(a){ case mods::weapon::support::primary_choice_t::a: return #a; }
			switch(s) {
					MAR_STR(SUPPORT_NONE);
					MAR_STR(SUPPORT_PRIMARY_MK46);
					MAR_STR(SUPPORT_PRIMARY_HK21);
				default:
					return "<unknown>";
			}
		}
	};
#undef MAR_STR
	namespace sentinel {
		enum primary_choice_t {
			SENTINEL_NONE = 0,
			SENTINEL_PRIMARY_MP5 = 1,
			SENTINEL_PRIMARY_SASG12 = 2,
			NONE = 0,
			MP5 = 1,
			SASG12 = 2
		};
		static inline std::string_view to_string(primary_choice_t s) {
#define SEN_STR(a){ case mods::weapon::sentinel::primary_choice_t::a: return #a; }
			switch(s) {
					SEN_STR(SENTINEL_NONE);
					SEN_STR(SENTINEL_PRIMARY_MP5);
					SEN_STR(SENTINEL_PRIMARY_SASG12);
				default:
					return "<unknown>";
			}
		}
	};
#undef SEN_STR
	obj_data_ptr_t base_rifle_object();
	obj_data_ptr_t new_pistol_object();
	obj_data_ptr_t base_explosive_object();
	obj_data_ptr_t new_frag_grenade_object();
	obj_data_ptr_t new_sensor_grenade_object();
	obj_data_ptr_t new_incendiary_grenade_object();
	obj_data_ptr_t new_emp_grenade_object();
	obj_data_ptr_t new_smoke_grenade_object();
	obj_data_ptr_t new_flashbang_grenade_object();
	obj_data_ptr_t new_sniper_rifle_object();
	obj_data_ptr_t new_pistol_object();

	std::vector<cap_t> get_caps(mw_rifle);
	std::vector<cap_t> get_caps(mw_gadget);
	std::vector<cap_t> get_caps(mw_explosive);
	std::vector<cap_t> get_caps(mw_armor);
	std::vector<cap_t> get_caps(mw_attachment);
	std::vector<cap_t> get_caps(mw_drone);
	std::vector<cap_t> get_caps(mw_consumable);
	std::vector<cap_t> get_caps(mw_trap);
	template <typename TObj,typename T>
	static inline void feed_caps(TObj& obj, T type) {
		obj->clear_capabilities();
		auto& obj_caps = obj->capabilities();
		for(auto&& value : get_caps(type)) {
			obj_caps[value] = true;
		}
	}

#include "weapon-stat-list.hpp"

	template <typename TObject>
	static inline auto get_rounds(TObject& obj) {
		return obj->obj_flags.ammo;
	}
	template <typename TPlayer,typename TObject>
	static inline void list_rounds(TPlayer& player,TObject& obj) {
		if(obj->obj_flags.ammo == 0) {
			player->sendln("{red}Out of ammo!{/red}");
		} else {
			player->send("%d rounds.\r\n", get_rounds(obj));
		}
	}
	static inline std::string yaml_file(support::primary_choice_t s) {
		switch(s) {
			case support::primary_choice_t::MK46:
				return "mk46.yml";
			case support::primary_choice_t::HK21:
				return "hk21.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(ghost::primary_choice_t s) {
		switch(s) {
			case ghost::primary_choice_t::PSG1:
				return "psg1.yml";
			case ghost::primary_choice_t::L96AW:
				return "l96aw.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(sniper::primary_choice_t s) {
		switch(s) {
			case sniper::primary_choice_t::PSG1:
				return "psg1.yml";
			case sniper::primary_choice_t::L96AW:
				return "l96aw.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(marine::primary_choice_t s) {
		switch(s) {
			case marine::primary_choice_t::M16A4:
				return "m16a4.yml";
			case marine::primary_choice_t::M4:
				return "m4.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(contagion::primary_choice_t s) {
		switch(s) {
			case contagion::primary_choice_t::M3:
				return "m3.yml";
			case contagion::primary_choice_t::FAMAS:
				return "famas.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(engineer::primary_choice_t s) {
		switch(s) {
			case engineer::primary_choice_t::FMG9:
				return "fmg9.yml";
			case engineer::primary_choice_t::P90:
				return "p90.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(medic::primary_choice_t s) {
		switch(s) {
			case medic::primary_choice_t::AUGPARA:
				return "augpara.yml";
			case medic::primary_choice_t::TAR21:
				return "tar21.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(psyop::primary_choice_t s) {
		switch(s) {
			case psyop::primary_choice_t::SCARH:
				return "scarh.yml";
			case psyop::primary_choice_t::UMP45:
				return "ump45.yml";
			default:
				return "unknown";
		}
	}
	static inline std::string yaml_file(sentinel::primary_choice_t s) {
		switch(s) {
			case sentinel::primary_choice_t::MP5:
				return "mp5.yml";
			case sentinel::primary_choice_t::SASG12:
				return "sasg12.yml";
			default:
				return "unknown";
		}
	}

};
#endif

