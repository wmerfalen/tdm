#ifndef __MENTOC_MODS_WEAPONS_DAMAGE_TYPES_HEADER__
#define __MENTOC_MODS_WEAPONS_DAMAGE_TYPES_HEADER__

#include "../item-types.hpp"
#include "../player.hpp"
#include "../weapon.hpp"
#include "../pqxx-types.hpp"
#include "../../globals.hpp"
#include "../scan.hpp"

extern void remember(char_data *ch,char_data *victim);
extern void stop_fighting(char_data *ch);
extern int damage(char_data *ch, char_data *victim, int dam, int attacktype);
namespace mods::weapons::damage_types {
	using vpd = mods::scan::vec_player_data;
	void rifle_attack_by_name(player_ptr_t& player,std::string_view victim_name,int direction);
	void spray_direction(player_ptr_t& player,int direction);
	void rifle_attack(
		player_ptr_t& player,
		obj_ptr_t weapon,
		player_ptr_t victim,
		uint16_t distance
	);
	void decrease_spray_shot_ammo(obj_ptr_t& weapon);
	void decrease_single_shot_ammo(obj_ptr_t& weapon);
	void fire_damage(player_ptr_t&,uint16_t);
	void smoke_damage(player_ptr_t& player, uint16_t damage);
	namespace legacy {
		int damage(char_data *ch, char_data *victim, int dam, int attacktype);
	};
};
#endif

