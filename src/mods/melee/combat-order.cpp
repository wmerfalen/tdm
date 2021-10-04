#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rifle-attachments.hpp"
#include "../rand.hpp"
#include "main.hpp"
#include <math.h>
#include "damage-types.hpp"

#define __MENTOC_SHOW_combat_order_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_combat_order_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::combat_order][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::melee::combat_order {
	using de = damage_event_t;
	/**
	 * Explanation of attack characteristics
	 * -------------------------------------
	 *  Speed of attack
	 *  	- a target is less likely to block an attack if the attacker is faster
	 *  	- this is determined by speed points and dexterity
	 *  Force of attack
	 *  	- force is determined by strength points multiplied by melee points
	 *  Chance of flow state
	 *  	- an attacker has a chance that two attacks follow each other on the same
	 *  		tick as opposed to the next round of ticks
	 *  	- certain attacks flow into each other nicely. i.e.: jab, cross
	 */
	/**
	 * Upper body techniques
	 */

	void init() {
#if 0
		melee_techniques = {
			/**
			 * Upper body techniques
			 */
			{"jab",do_jab},
			{"cross", do_cross},
			{"uppercut", do_uppercut},
			{"elbow-left", do_elbow_left},
			{"elbow-right", do_elbow_right},

			/**
			 * Lower body techniques
			 */
			{"teep-left",do_teep_left},
			{"teep-right",do_teep_right},

			/**
			 * Knees from within clinch
			 */
			{"knee-head-left",do_knee_head_left},
			{"knee-head-right",do_knee_head_right},
			{"knee-body-left",do_knee_body_left},
			{"knee-body-right",do_knee_body_right},

			/**
			 * Damaging knee attack
			 */
			{"oblique-left",do_oblique_left},
			{"oblique-right",do_oblique_right},

			/** Wrestling techniques */
			{"double-leg",do_double_leg},
			{"single-leg-left",do_single_leg_left},
			{"single-leg-right",do_single_leg_right},

			/** Bladed weapons */
			{"slash",do_slash},
			{"stab",do_stab},

			/** Throwing techniques */
			{"hip-toss",do_hip_toss},
			{"drop-seoinage",do_drop_seoinage},
			{"inner-leg-trip-left",do_inner_leg_trip_left},
			{"inner-leg-trip-right",do_inner_leg_trip_right},
			{"ippon-seoinage",do_ippon_seoinage},

			/** Distancing techniques */
			{"teep-left",do_teep_left},
			{"teep-right",do_teep_right},
			{"front-kick-left",do_front_kick_left},
			{"front-kick-right",do_front_kick_right},
			/**
			 * Aikido disarm techniques
			 */
			{"knife-disarm",do_knife_disarm},
			{"pistol-disarm",do_pistol_disarm},

			/**
			 * Judo throws
			 */
			{"ippon-seoinage",do_ippon_seoinage},
		};
#endif
	}
};

