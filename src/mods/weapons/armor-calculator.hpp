#ifndef __MENTOC_MODS_WEAPONS_armor_calculator_HEADER__
#define __MENTOC_MODS_WEAPONS_armor_calculator_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../../globals.hpp"
#include "../scan.hpp"
#include "../damage-event.hpp"
#include "../forge-engine/elemental-enum.hpp"
#include "../rifle-attachments.hpp"

namespace mods::weapons::armor_calculator {
	int calculate_armor_attribute_resistance(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	);

	/**
	 * @brief returns the amount of damage that was absorbed by the critical hit
	 *
	 * @param player
	 * @param weapon
	 * @param victim
	 * @param
	 *
	 * @return  the amount of damage absorbed by the victim's armor trait
	 */
	float roll_critical_damage_reduction(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	);
};
#endif

