#ifndef __MENTOC_MODS_CLASSES_INCLUDES_HEADER__
#define __MENTOC_MODS_CLASSES_INCLUDES_HEADER__
#include "marine.hpp"
#include "breacher.hpp"
#include "ghost.hpp"
#include "contagion.hpp"
namespace mods::classes {
	template <typename T>
	static inline void increment(player_ptr_t& attacker,T& ex_class,std::string_view technique) {
		if(technique.compare("wpn_assault_rifles") == 0) {
			ex_class->wpn_assault_rifles().use_skill(attacker);
		}
		if(technique.compare("wpn_sniper_rifles") == 0) {
			ex_class->wpn_sniper_rifles().use_skill(attacker);
		}
		if(technique.compare("wpn_sub_machine_guns") == 0) {
			ex_class->wpn_sub_machine_guns().use_skill(attacker);
		}
		if(technique.compare("wpn_shotguns") == 0) {
			ex_class->wpn_shotguns().use_skill(attacker);
		}
		if(technique.compare("wpn_pistols") == 0) {
			ex_class->wpn_pistols().use_skill(attacker);
		}
		if(technique.compare("wpn_machine_pistols") == 0) {
			ex_class->wpn_machine_pistols().use_skill(attacker);
		}
		if(technique.compare("wpn_light_machine_guns") == 0) {
			ex_class->wpn_light_machine_guns().use_skill(attacker);
		}
		if(technique.compare("cqc_jab_to_head") == 0) {
			ex_class->cqc_jab_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_jab_to_body") == 0) {
			ex_class->cqc_jab_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_cross_to_head") == 0) {
			ex_class->cqc_cross_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_cross_to_body") == 0) {
			ex_class->cqc_cross_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_left_hook_to_head") == 0) {
			ex_class->cqc_left_hook_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_right_hook_to_head") == 0) {
			ex_class->cqc_right_hook_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_left_hook_to_body") == 0) {
			ex_class->cqc_left_hook_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_right_hook_to_body") == 0) {
			ex_class->cqc_right_hook_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_left_uppercut") == 0) {
			ex_class->cqc_left_uppercut().use_skill(attacker);
		}
		if(technique.compare("cqc_right_uppercut") == 0) {
			ex_class->cqc_right_uppercut().use_skill(attacker);
		}
		if(technique.compare("cqc_left_elbow") == 0) {
			ex_class->cqc_left_elbow().use_skill(attacker);
		}
		if(technique.compare("cqc_right_elbow") == 0) {
			ex_class->cqc_right_elbow().use_skill(attacker);
		}
		if(technique.compare("cqc_right_upward_elbow") == 0) {
			ex_class->cqc_right_upward_elbow().use_skill(attacker);
		}
		if(technique.compare("cqc_left_upward_elbow") == 0) {
			ex_class->cqc_left_upward_elbow().use_skill(attacker);
		}
		if(technique.compare("cqc_right_oblique") == 0) {
			ex_class->cqc_right_oblique().use_skill(attacker);
		}
		if(technique.compare("cqc_left_oblique") == 0) {
			ex_class->cqc_left_oblique().use_skill(attacker);
		}
		if(technique.compare("cqc_left_teep") == 0) {
			ex_class->cqc_left_teep().use_skill(attacker);
		}
		if(technique.compare("cqc_right_teep") == 0) {
			ex_class->cqc_right_teep().use_skill(attacker);
		}
		if(technique.compare("cqc_left_front_kick") == 0) {
			ex_class->cqc_left_front_kick().use_skill(attacker);
		}
		if(technique.compare("cqc_right_front_kick") == 0) {
			ex_class->cqc_right_front_kick().use_skill(attacker);
		}
		if(technique.compare("cqc_left_knee_to_head") == 0) {
			ex_class->cqc_left_knee_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_right_knee_to_head") == 0) {
			ex_class->cqc_right_knee_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_left_knee_to_body") == 0) {
			ex_class->cqc_left_knee_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_right_knee_to_body") == 0) {
			ex_class->cqc_right_knee_to_body().use_skill(attacker);
		}
		if(technique.compare("cqc_knife_disarm") == 0) {
			ex_class->cqc_knife_disarm().use_skill(attacker);
		}
		if(technique.compare("cqc_pistol_disarm") == 0) {
			ex_class->cqc_pistol_disarm().use_skill(attacker);
		}
		if(technique.compare("cqc_right_leg_kick") == 0) {
			ex_class->cqc_right_leg_kick().use_skill(attacker);
		}
		if(technique.compare("cqc_left_leg_kick") == 0) {
			ex_class->cqc_left_leg_kick().use_skill(attacker);
		}
		if(technique.compare("cqc_right_kick_to_head") == 0) {
			ex_class->cqc_right_kick_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_left_kick_to_head") == 0) {
			ex_class->cqc_left_kick_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_right_stomp_to_head") == 0) {
			ex_class->cqc_right_stomp_to_head().use_skill(attacker);
		}
		if(technique.compare("cqc_left_stomp_to_head") == 0) {
			ex_class->cqc_left_stomp_to_head().use_skill(attacker);
		}
	}
	static inline void increment_technique(player_ptr_t& attacker,std::string technique) {
		if(attacker->ghost()) {
			increment(attacker,attacker->ghost(),technique);
			return;
		}
		if(attacker->contagion()) {
			increment(attacker,attacker->contagion(),technique);
			return;
		}
		if(attacker->marine()) {
			increment(attacker,attacker->marine(),technique);
			return;
		}
		if(attacker->breacher()) {
			increment(attacker,attacker->breacher(),technique);
			return;
		}
	}
};
#endif
