#ifndef __MENTOC_MODS_WEAPONS_DAMAGE_TYPES_HEADER__
#define __MENTOC_MODS_WEAPONS_DAMAGE_TYPES_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../../globals.hpp"
#include "../scan.hpp"
#include "../damage-event.hpp"
#include "../forge-engine/elemental-enum.hpp"

extern void remember(char_data *ch,char_data *victim);
extern void stop_fighting(char_data *ch);
extern int damage(char_data *ch, char_data *victim, int dam, int attacktype);
namespace mods::weapons::damage_types {
	using vpd = mods::scan::vec_player_data;
	void rifle_attack_by_name(player_ptr_t& player,std::string_view victim_name,int direction);
	void rifle_attack_object_by_name(player_ptr_t& player,std::string_view object_name,int direction);
	void spray_direction(player_ptr_t& player,int direction);
	void rifle_attack(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	);
	feedback_t spray_direction_with_feedback(player_ptr_t& player,int direction);
	feedback_t rifle_attack_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim,
	    uint16_t distance,
	    uint8_t direction
	);
	feedback_t melee_damage_with_feedback(
	    player_ptr_t& player,
	    obj_ptr_t weapon,
	    player_ptr_t victim
	);
	void decrease_spray_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon);
	void decrease_single_shot_ammo(player_ptr_t& attacker,obj_ptr_t& weapon);
	void deal_hp_damage(player_ptr_t& player, uint16_t damage);
	int calculate_tracked_damage(player_ptr_t& player,int original_damage);
	feedback_t perform_damage(
	    player_ptr_t& player,
	    player_ptr_t& victim,
	    obj_ptr_t weapon,
	    int dam,
	    direction_t direction
	);
	namespace legacy {
		int damage(char_data *ch, char_data *victim, int dam, int attacktype);
	};
	int get_legacy_attack_type(obj_ptr_t& weapon);
};
#endif

