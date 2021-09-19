#include "armor-calculator.hpp"
#include "../object-utils.hpp"
#include "../injure.hpp"
#include "../rooms.hpp"
#include "../skills.hpp"
#include "../projectile.hpp"
#include "../levels.hpp"
#include "../interpreter-include.hpp"
#include "elemental.hpp"
#include "../rifle-attachments.hpp"
#include "../rand.hpp"
#include <math.h>

#define __MENTOC_SHOW_armor_calculator_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_armor_calculator_DEBUG_OUTPUT__
#define dty_debug(a) std::cerr << "[mods::weapons::armor_calculator][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define dty_debug(a)
#endif

#ifndef TO_ROOM
#define TO_ROOM		1
#endif

namespace mods::weapons::armor_calculator {
	using de = damage_event_t;
	using vpd = mods::scan::vec_player_data;
	int16_t get_level_difference(
	    player_ptr_t& attacker,
	    player_ptr_t& victim
	) {
		return attacker->level() - victim->level();
	}

	int calculate_armor_attribute_resistance(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	) {
		float multiplier = ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
		switch(mods::levels::player_tier(victim)) {
			default:
			case 1:
				multiplier += TIER_ONE_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
			case 2:
				multiplier += TIER_TWO_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
			case 3:
				multiplier += TIER_THREE_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
		}
		return player->armor() * multiplier;
	}
	float roll_critical_damage_reduction(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	) {
		auto diff = get_level_difference(attacker,victim);
		float multiplier = diff * ARMOR_CRIT_DAMAGE_MULTIPLIER();
		switch(mods::levels::player_tier(victim)) {
			default:
			case 1:
				multiplier *= TIER_ONE_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
			case 2:
				multiplier *= TIER_TWO_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
			case 3:
				multiplier *= TIER_THREE_ARMOR_ATTRIBUTE_DAMPENER_MULTIPLIER();
				break;
		}
		return attacker->armor() * multiplier * dam;
	}
};

