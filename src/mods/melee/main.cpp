#include "main.hpp"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "../rand.hpp"
#include <math.h>
#include "damage-types.hpp"
#include "../classes/includes.hpp"

#define __MENTOC_SHOW_main_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_main_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::melee::main][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::melee::main::moves {
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
	void JAB_TO_HEAD(player_ptr_t& attacker, player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("jab to head");
		mods::classes::increment_technique(attacker,"cqc_jab_to_head");
	}
	void JAB_TO_BODY(player_ptr_t& attacker, player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("jab to body");
		mods::classes::increment_technique(attacker,"cqc_jab_to_body");
	}
	void CROSS_TO_HEAD(player_ptr_t& attacker, player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("cross to head");
		mods::classes::increment_technique(attacker,"cqc_cross_to_head");
	}
	void CROSS_TO_BODY(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("cross to body");
		mods::classes::increment_technique(attacker,"cqc_cross_to_body");
	}
	void LEFT_UPPERCUT(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left uppercut");
		mods::classes::increment_technique(attacker,"cqc_left_uppercut");
	}

	void LEFT_HOOK_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left hook to head");
		mods::classes::increment_technique(attacker,"cqc_left_hook_to_head");
	}
	void RIGHT_HOOK_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right hook to head");
		mods::classes::increment_technique(attacker,"cqc_right_hook_to_head");
	}
	void LEFT_HOOK_TO_BODY(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left hook to body");
		mods::classes::increment_technique(attacker,"cqc_left_hook_to_body");
	}
	void RIGHT_HOOK_TO_BODY(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right hook to body");
		mods::classes::increment_technique(attacker,"cqc_right_hook_to_body");
	}

	void RIGHT_UPPERCUT(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right uppercut");
		mods::classes::increment_technique(attacker,"cqc_right_uppercut");
	}

	void LEFT_ELBOW(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left_elbow");
		mods::classes::increment_technique(attacker,"cqc_left_elbow");
	}
	void RIGHT_ELBOW(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right_elbow");
		mods::classes::increment_technique(attacker,"cqc_right_elbow");

	}
	void RIGHT_OBLIQUE(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right_oblique");
		mods::classes::increment_technique(attacker,"cqc_right_oblique");

	}
	void LEFT_OBLIQUE(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left_oblique");
		mods::classes::increment_technique(attacker,"cqc_left_oblique");

	}
	void LEFT_TEEP(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left_teep");
		mods::classes::increment_technique(attacker,"cqc_left_teep");

	}
	void RIGHT_TEEP(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right_teep");

		mods::classes::increment_technique(attacker,"cqc_right_teep");
	}
	void LEFT_FRONT_KICK(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left_front_kick");
		mods::classes::increment_technique(attacker,"cqc_left_front_kick");

	}
	void RIGHT_FRONT_KICK(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right front kick");
		mods::classes::increment_technique(attacker,"cqc_right_front_kick");

	}
	void RIGHT_KICK_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right kick to head");
		mods::classes::increment_technique(attacker,"cqc_right_kick_to_head");
	}
	void LEFT_KICK_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right kick to head");
		mods::classes::increment_technique(attacker,"cqc_left_kick_to_head");
	}
	void RIGHT_KNEE_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right knee to head");
		mods::classes::increment_technique(attacker,"cqc_right_knee_to_head");
	}
	void LEFT_KNEE_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left knee to head");
		mods::classes::increment_technique(attacker,"cqc_left_knee_to_head");

	}
	void LEFT_KNEE_TO_BODY(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left knee to body");
		mods::classes::increment_technique(attacker,"cqc_left_knee_to_body");

	}
	void RIGHT_KNEE_TO_BODY(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right knee to body");
		mods::classes::increment_technique(attacker,"cqc_right_knee_to_body");

	}
	void KNIFE_DISARM(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("knife disarm");
		mods::classes::increment_technique(attacker,"cqc_knife_disarm");

	}
	void PISTOL_DISARM(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("pistol disarm");
		mods::classes::increment_technique(attacker,"cqc_pistol_disarm");

	}
	void RIGHT_UPWARD_ELBOW(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("right upward elbow");
		mods::classes::increment_technique(attacker,"cqc_right_upward_elbow");
	}
	void LEFT_UPWARD_ELBOW(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left upward elbow");
		mods::classes::increment_technique(attacker,"cqc_left_upward_elbow");
	}
	void RIGHT_LEG_KICK(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left upward elbow");
		mods::classes::increment_technique(attacker,"cqc_right_leg_kick");
	}
	void LEFT_LEG_KICK(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left upward elbow");
		mods::classes::increment_technique(attacker,"cqc_left_leg_kick");
	}
	void RIGHT_STOMP_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left upward elbow");
		mods::classes::increment_technique(attacker,"cqc_right_stomp_to_head");
	}
	void LEFT_STOMP_TO_HEAD(player_ptr_t& attacker,player_ptr_t& victim) {
		auto dex = attacker->dexterity();
		auto force = attacker->strength() * attacker->melee_triad() * attacker->level();
		mods::melee::damage_types::cqc_damage_with_feedback(attacker,victim,force * (dex * 0.1));
		attacker->sendln("left upward elbow");
		mods::classes::increment_technique(attacker,"cqc_left_stomp_to_head");
	}
};//end namespace moves

namespace mods::melee::main {
	using t = technique_t;
	static inline std::vector<std::pair<technique_t,technique_t>>& flow_states() {
		static std::vector<std::pair<technique_t,technique_t>> state;
		static bool set = false;
		if(!set) {
			state.emplace_back(std::make_pair<>(t::JAB_TO_HEAD,t::CROSS_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::JAB_TO_HEAD,t::CROSS_TO_BODY));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_BODY,t::LEFT_UPPERCUT));
			state.emplace_back(std::make_pair<>(t::JAB_TO_HEAD,t::JAB_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::JAB_TO_HEAD,t::RIGHT_ELBOW));

			state.emplace_back(std::make_pair<>(t::RIGHT_ELBOW,t::RIGHT_UPWARD_ELBOW));

			state.emplace_back(std::make_pair<>(t::LEFT_ELBOW,t::LEFT_UPWARD_ELBOW));
			state.emplace_back(std::make_pair<>(t::LEFT_HOOK_TO_HEAD,t::CROSS_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::LEFT_UPPERCUT,t::RIGHT_HOOK_TO_BODY));
			state.emplace_back(std::make_pair<>(t::LEFT_UPPERCUT,t::CROSS_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::LEFT_HOOK_TO_BODY,t::CROSS_TO_BODY));
			state.emplace_back(std::make_pair<>(t::LEFT_HOOK_TO_BODY,t::CROSS_TO_HEAD));

			state.emplace_back(std::make_pair<>(t::RIGHT_UPPERCUT,t::LEFT_HOOK_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::RIGHT_HOOK_TO_BODY,t::LEFT_HOOK_TO_BODY));

			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::RIGHT_FRONT_KICK));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::LEFT_ELBOW));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::RIGHT_TEEP));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::LEFT_UPPERCUT));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::LEFT_HOOK_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::CROSS_TO_HEAD,t::LEFT_HOOK_TO_BODY));

			state.emplace_back(std::make_pair<>(t::RIGHT_TEEP,t::LEFT_KICK_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::LEFT_ELBOW,t::RIGHT_ELBOW));
			state.emplace_back(std::make_pair<>(t::LEFT_ELBOW,t::RIGHT_KNEE_TO_HEAD));
			state.emplace_back(std::make_pair<>(t::LEFT_ELBOW,t::RIGHT_KNEE_TO_BODY));
		}
		return state;
	}
	bool roll_flows_to_next(player_ptr_t& attacker,player_ptr_t& victim,const auto& index) {
		auto& orders =attacker->get_combat_order();
		if(index + 1 >= orders.size()) {
			return 0;
		}
		auto current_attack = orders[index].first;
		auto next_attack = orders[index+1].first;
		for(const auto& pair : flow_states()) {
			if(pair.first == current_attack && pair.second == next_attack) {
				return 1;
			}
		}
		return 0;
	}

	using de = damage_event_t;
	void dispatch_player(player_ptr_t& attacker,player_ptr_t& victim) {
		auto orders = attacker->get_combat_order();
		if(orders.size() == 0) {
			attacker->sendln("You don't have any combat orders...");
			return;
		}
		auto index = attacker->get_combat_order_index();
		if(index >= orders.size()) {
			attacker->set_combat_order_index(0);
			index = 0;
		}
		auto pair = orders[index];
		bool flows_to_next = roll_flows_to_next(attacker,victim,index);
		pair.second(attacker,victim);
		if(victim->position() == POS_DEAD) {
			attacker->set_combat_order_index(0);
			return;
		}
		++index;
		if(index >= orders.size()) {
			index = 0;
		}
		attacker->set_combat_order_index(index);

		if(flows_to_next) {
			return dispatch_player(attacker,victim);
		}

	}



	ACMD(do_set_combat_order) {
		/**
		 * combat_order <attk1> <attk2> <attk3>
		 */
		player->clear_combat_order();
		int i=0;
		static constexpr uint8_t MAX_TECHNIQUES = __MENTOC_MAX_COMBAT_ORDER_ENTRIES__;
		for(const auto& technique : args()->gather_max_tokens_starting_at(0,MAX_TECHNIQUES)) {
			if(i++ >= MAX_TECHNIQUES) {
				break;
			}
			auto tech = to_technique_enum(technique);
			auto func = get_technique_function((t)tech);
			if(func.has_value() == false) {
				player->sendln(CAT("Cannot set combat order for: {red}'",technique,"'{/red}!"));
				continue;
			}
			player->sendln(CAT("Setting Technique: '",technique,"'..."));
			player->add_combat_order(std::make_pair<> (tech,func.value()));
		}
		player->sendln("Your combat order is now...");
		i =1;
		for(const auto& technique : player->get_combat_order()) {
			player->sendln(CAT("[",i,"]: ",to_string((t)technique.first)));
			++i;
		}

	}
	ACMD(do_list_combat_order) {
		player->sendln("Listing...");
		uint8_t i = 1;
		for(const auto& technique : player->get_combat_order()) {
			player->sendln(CAT("[",i,"]: ",to_string((t)technique.first)));
			++i;
		}
		player->sendln("Done.");
	}

	ACMD(do_clear_combat_order) {
		player->sendln("Clearing your combat order...");
		player->clear_combat_order();
		player->sendln("Done.");
	}

	ACMD(do_dispatch) {
		auto victim = player->fighting();
		if(!victim) {
			player->sendln("You aren't fighting anyone!");
			return;
		}
		dispatch_player(player,victim);
	}

	void init() {
		mods::interpreter::add_user_command("dispatch",do_dispatch);
		mods::interpreter::add_user_command("set_combat_order",do_set_combat_order);
		mods::interpreter::add_user_command("list_combat_order",do_list_combat_order);
		mods::interpreter::add_user_command("clear_combat_order",do_clear_combat_order);
	}
}; //end namespace mods::melee::main::moves
