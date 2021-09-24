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
	using acmd_function = mods::interpreter::acmd_function;
	std::map<std::string,acmd_function> melee_techniques;
	std::optional<acmd_function> get_melee_function(std::string_view name) {
		if(melee_techniques[name.data()]) {
			return melee_techniques[name.data()];
		}
		return std::nullopt;
	}

	ACMD(do_combat_order) {
		/**
		 * combat_order <clear>
		 * combat_order set <attk1> <attk2> <attk3>
		 */
		if(argshave()->first_is("set")->size_gt(1)->passed()) {
			player->clear_combat_order();
			int i=0;
			static constexpr uint8_t MAX_TECHNIQUES = 3;
			for(const auto& technique : args()->gather_tokens_starting_at(1)) {
				if(i++ >= MAX_TECHNIQUES) {
					break;
				}
				player->sendln(CAT("Setting Technique: '",technique,"'..."));
				player->add_combat_order(technique);
			}
			player->sendln("Your combat order is now...");
			i =1;
			for(const auto& technique : player->get_combat_order()) {
				player->sendln(CAT("[",i,"]: ",technique));
				++i;
			}
		}

	}

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
	ACMD(do_jab) {
		/**
		 * Uses dexterity.
		 * 	- Dexterity determines speed of attack
		 *
		 * Force of attack
		 * 	- strength * melee points * constitution
		 */
		auto victim = player->fighting();
		if(!victim) {
			return;
		}
		auto dex = player->dexterity();
		auto force = player->strength() * player->melee_triad() * player->level();
		mods::melee::damage_types::cqc_damage_with_feedback(player,victim,force * (dex * 0.1));
		player->sendln("jab");
	}
	ACMD(do_cross) {
		auto victim = player->fighting();
		if(!victim) {
			return;
		}
		auto dex = player->dexterity();
		auto force = player->strength() * player->melee_triad() * player->level();
		mods::melee::damage_types::cqc_damage_with_feedback(player,victim,force * (dex * 0.1));
		player->sendln("cross");
	}
	ACMD(do_uppercut) {
		auto victim = player->fighting();
		if(!victim) {
			return;
		}
		auto dex = player->dexterity();
		auto force = player->strength() * player->melee_triad() * player->level();
		mods::melee::damage_types::cqc_damage_with_feedback(player,victim,force * (dex * 0.1));
		player->sendln("uppercut");
	}

	ACMD(do_elbow_left) {

	}
	ACMD(do_elbow_right) {

	}

	/**
	* Wrestling techniques
	*/
	ACMD(do_double_leg) {

	}
	ACMD(do_single_leg_left) {

	}
	ACMD(do_single_leg_right) {

	}

	/**
	 * Damaging knee attacks
	 */
	ACMD(do_oblique_left) {

	}
	ACMD(do_oblique_right) {

	}


	/**
	 * Bladed attacks
	 */
	ACMD(do_slash) {

	}
	ACMD(do_stab) {

	}

	/**
	 * Judo throws
	 */
	ACMD(do_hip_toss) {

	}
	ACMD(do_drop_seoinage) {

	}
	ACMD(do_inner_leg_trip_left) {

	}
	ACMD(do_inner_leg_trip_right) {

	}
	ACMD(do_ippon_seoinage) {

	}

	/**
	 * Distancing techniques
	 */
	ACMD(do_teep_left) {

	}
	ACMD(do_teep_right) {

	}

	/**
	 * Kicks
	 */
	ACMD(do_front_kick_left) {

	}
	ACMD(do_front_kick_right) {

	}

	/**
	 * Knees from within clinch
	 */
	ACMD(do_knee_head_left) {

	}
	ACMD(do_knee_head_right) {

	}
	ACMD(do_knee_body_left) {

	}
	ACMD(do_knee_body_right) {

	}

	/**
	 * Disarming techniques
	 */
	ACMD(do_knife_disarm) {

	}
	ACMD(do_pistol_disarm) {

	}

	void dispatch_player(player_ptr_t& attacker,player_ptr_t& victim) {
		mods::melee::main::dispatch_player(attacker);
	}

	void init() {
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
		mods::interpreter::add_user_command("combat_order",do_combat_order);
	}
};

