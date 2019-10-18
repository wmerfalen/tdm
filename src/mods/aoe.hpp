#ifndef __MENTOC_MODS_AOE_HEADER__
#define __MENTOC_MODS_AOE_HEADER__

#include <iostream>
#include <string>

namespace mods::aoe {
	enum types_t {
		AOE_TYPES_NONE = 0,
		EXPLOSIVE,	/** Typical explosive dmg from a frag grenade */
		FLAME,	/** Ignites material within blast_radius */
		RADIATION, /** Radioactive fallout */
		EMP,	/** Electro magnetic pulse. Destroys/hampers electronics */
		SMOKE_SCREEN,	/** Typical grey smoke screen. Hampers vision */
		BLINDING_LIGHT,	/** Typical blinding light from flash bangs */
		SHRAPNEL,	/** metal fragments flown in random directions */
		DIRECTED_EXPLOSION,	/** Claymore explosions. Directed */
		POISONOUS,	/** Venomous poisons that need urgent attn */
		INK,	/** Marks surroundings and enemies with paint for the purpose of tracking */ 
		SONIC_WAVE,	/** The same type of explosion caused by breaking the sound barrier */
		THERMAL_AGITATOR,	/** Causes visual noise to thermal devices rendering them of no use */
		MOBILITY_RESTRICTING_FOAM,	/** A foam that restricts enemy movement once dried. Dries fast */
		AUDITORY_DISTRACTION,	/** Pre-recorded audio to distract or deceive */
		BEACON,	/** Serves as a radio beacon for communication purposes and tracking */
		SENSOR, /** senses objects or enemies within blast_radius */
		BARBED_TENDRILS, /** explodes into barbed wire causing painful movement */
		CORROSIVE,	/** Corrodes devices, melting them into a useless state. Causes irreversible burns to exposed skin */
		__AOE_FIRST = EXPLOSIVE,
		__AOE_LAST = CORROSIVE
	};
	static inline std::string to_string(const types_t& type) {
#define MENTOC_T(a) case types_t::a: return #a;
		switch(type){
			MENTOC_T(AOE_TYPES_NONE);
			MENTOC_T(EXPLOSIVE);
			MENTOC_T(FLAME);
			MENTOC_T(RADIATION);
			MENTOC_T(EMP);
			MENTOC_T(SMOKE_SCREEN);
			MENTOC_T(BLINDING_LIGHT);
			MENTOC_T(SHRAPNEL);
			MENTOC_T(DIRECTED_EXPLOSION);
			MENTOC_T(POISONOUS);
			MENTOC_T(INK);
			MENTOC_T(SONIC_WAVE);
			MENTOC_T(THERMAL_AGITATOR);
			MENTOC_T(MOBILITY_RESTRICTING_FOAM);
			MENTOC_T(AUDITORY_DISTRACTION);
			MENTOC_T(BEACON);
			MENTOC_T(SENSOR);
			MENTOC_T(BARBED_TENDRILS);
			MENTOC_T(CORROSIVE);
			default: return "<>";
		}
#undef MENTOC_T
	}

	static inline std::string description(const types_t& type) {
		switch(type){
			default:
			case types_t::AOE_TYPES_NONE:
				return "<>";
			case types_t::EXPLOSIVE: return "Typical explosive dmg from a frag grenade";
			case types_t::FLAME: return "Ignites material within blast_radius";
			case types_t::RADIATION: return "Radioactive fallout";
			case types_t::EMP: return "Electro magnetic pulse. Destroys/hampers electronics";
			case types_t::SMOKE_SCREEN: return "Typical grey smoke screen. Hampers vision";
			case types_t::BLINDING_LIGHT: return "Typical blinding light from flash bangs";
			case types_t::SHRAPNEL: return "metal fragments flown in random directions";
			case types_t::DIRECTED_EXPLOSION: return "Claymore explosions. Directed";
			case types_t::POISONOUS: return "Venomous poisons that need urgent attn";
			case types_t::INK: return "Marks surroundings and enemies with paint for the purpose of tracking"; 
			case types_t::SONIC_WAVE: return "The same type of explosion caused by breaking the sound barrier";
			case types_t::THERMAL_AGITATOR: return "Causes visual noise to thermal devices rendering them of no use";
			case types_t::MOBILITY_RESTRICTING_FOAM: return "A foam that restricts enemy movement once dried. Dries fast";
			case types_t::AUDITORY_DISTRACTION: return "Pre-recorded audio to distract or deceive";
			case types_t::BEACON: return "Serves as a radio beacon for communication purposes and tracking";
			case types_t::SENSOR: return "senses objects or enemies within blast_radius";
			case types_t::BARBED_TENDRILS: return "explodes into barbed wire causing painful movement";
			case types_t::CORROSIVE: return "Corrodes devices, melting them into a useless state. Causes irreversible burns to exposed skin";
		}
	}
};
#endif

