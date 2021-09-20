#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rifle-attachments.hpp"
#include "../rand.hpp"
#include <math.h>

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
	using acmd_function = mods::interpreter::acmd_function;
	std::map<std::string,acmd_function> aggro_techniques;
	std::map<std::string,acmd_function> balanced_techniques;
	std::map<std::string,acmd_function> passive_techniques;

	ACMD(do_combat_order) {
		/**
		 * combat_order <clear>
		 * combat_order set <attk1> <attk2> <attk3>
		 */
		if(argshave()->first_is("set")->size_gt(1)->passed()) {
			for(const auto& technique : args()->gather_tokens_starting_at(1)) {
				player->sendln(CAT("Technique: '",technique,"'"));
			}
		}

		/**
		 * Example:
		 * combat_order set jab cross uppercut
		 */

		/**
		 * Example:
		 * stance aggro
		 * combat_order set elbow knee takedown
		 *
		 * stance defense
		 * combat_order set disarm flip knee-stomp
		 *
		 */
	}
	ACMD(do_jab) { }
	ACMD(do_cross) { }
	ACMD(do_uppercut) { }
	ACMD(do_elbow) { }
	ACMD(do_oblique) { }
	ACMD(do_double_leg) { }
	ACMD(do_single_leg_left) { }
	ACMD(do_single_leg_right) { }
	ACMD(do_slash) { }
	ACMD(do_stab) { }
	ACMD(do_hip_toss) { }
	ACMD(do_drop_seoinage) { }
	ACMD(do_inner_leg_trip_left) { }
	ACMD(do_inner_leg_trip_right) { }
	ACMD(do_ippon_seoinage) { }
	ACMD(do_teep_left) { }
	ACMD(do_teep_right) { }
	ACMD(do_front_kick_left) { }
	ACMD(do_front_kick_right) { }
	ACMD(do_knife_disarm) { }
	ACMD(do_knee_left) { }
	ACMD(do_knee_right) { }
	void init() {
		aggro_techniques = {
			/**
			 * Upper body techniques
			 */
			{"jab",do_jab},
			{"cross", do_cross},
			{"uppercut", do_uppercut},
			{"elbow", do_elbow},

			/**
			 * Lower body techniques
			 */
			{"teep-left",do_teep_left},
			{"teep-right",do_teep_right},
			{"knee-left",do_knee_left},
			{"knee-right",do_knee_right},
			{"oblique",do_oblique},

			/** Wrestling techniques */
			{"double-leg",do_double_leg},
			{"single-leg-left",do_single_leg_left},
			{"single-leg-right",do_single_leg_right},

			/** Bladed weapons */
			{"slash",do_slash},
			{"stab",do_stab},
		};
		balanced_techniques = {

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
		};

		passive_techniques = {
			/**
			 * Aikido disarm techniques
			 */
			{"knife-disarm",do_knife_disarm},

			/**
			 * Judo throws
			 */
			{"ippon-seoinage",do_ippon_seoinage},
		};
		mods::interpreter::add_user_command("combat_order",do_combat_order);
	}
};

