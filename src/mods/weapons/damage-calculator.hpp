#ifndef __MENTOC_MODS_WEAPONS_DAMAGE_CALCULATOR_HEADER__
#define __MENTOC_MODS_WEAPONS_DAMAGE_CALCULATOR_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../../globals.hpp"
#include "../scan.hpp"
#include "../damage-event.hpp"
#include "../forge-engine/elemental-enum.hpp"
#include "../rifle-attachments.hpp"

namespace mods::weapons::damage_calculator {
	void clear_static_roll_weapon_amount();
	void set_static_roll_weapon_amount(int16_t amount);
	int calculate_strength_bonus(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	);
	int calculate_constitution_resistance(
	    player_ptr_t& player,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int16_t dam
	);
	int16_t calculate(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	);
	int16_t max_range(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	);
	int16_t ammunition_amount(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	);
	bool roll_free_ammo_chance(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	);
	int16_t regenerate_ammo(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	);
	int16_t reduce_durability_points(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon
	);
	int16_t reduce_ammo(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    int16_t wants_to_reduce
	);
	int16_t calculate_bonus_damage(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    int16_t requested_damage
	);
	bool attack_disorients(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	);
	int16_t disorient_ticks(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim
	);
	int calculate_sniper_extra_damage(
	    player_ptr_t& attacker,
	    obj_ptr_t& weapon,
	    player_ptr_t& victim,
	    int dam
	);
	struct explosive_damage_t {
		int chemical;
		int fire;
		int radiation;
		int electric;
		int armor_pen;
		int total;
		int damage;
		int critical;
		bool injured;
		explosive_damage_t() :
			chemical(0),
			fire(0),
			radiation(0),
			electric(0),
			armor_pen(0),
			total(0),
			damage(0),
			critical(0),
			injured(false) {}
	};
	explosive_damage_t calculate_explosive_damage(player_ptr_t victim, obj_ptr_t item);
};
#endif

