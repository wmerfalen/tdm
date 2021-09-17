#include "../../globals.hpp"

namespace mods::weapons::damage_types {
	bool can_be_injured(player_ptr_t& victim);
	bool attack_injures(player_ptr_t& player,player_ptr_t& victim,obj_ptr_t& weapon,feedback_t feedback);

	std::tuple<bool,feedback_t> can_continue(player_ptr_t& player,obj_ptr_t weapon,std::variant<player_ptr_t,obj_ptr_t,int8_t> victim,direction_t direction, const uint8_t& item_type);

	int calculate_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback);

	void set_player_weapon_cooldown(player_ptr_t& attacker,player_ptr_t& victim,obj_ptr_t& attackers_weapon, feedback_t& feedback);

	int calculate_tracked_damage(player_ptr_t& player,int original_damage);

	void remember_event(player_ptr_t& victim,player_ptr_t& attacker);
	bool can_be_injured(player_ptr_t& victim);

	uint8_t calculate_spray_chance(player_ptr_t& player);
};
