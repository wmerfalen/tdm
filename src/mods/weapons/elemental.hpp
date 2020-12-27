#ifndef __MENTOC_MODS_WEAPONS_ELEMENTAL_HEADER__
#define __MENTOC_MODS_WEAPONS_ELEMENTAL_HEADER__

#include "../../globals.hpp"
#include "../damage-event.hpp"
#include "../forge-engine/elemental-enum.hpp"

namespace mods::weapons::elemental {
	void room_fire_damage(player_ptr_t&,uint16_t);
	void room_smoke_damage(player_ptr_t& player, uint16_t damage);
	void process_elemental_damage(player_ptr_t& player,obj_ptr_t& weapon,player_ptr_t& victim,feedback_t& feedback);
	int reduce_elemental_resistance(int requested_damage, int16_t resistance);
	void incendiary_damage(player_ptr_t& player,int requested_damage);
	void explosive_damage(player_ptr_t& player,int requested_damage);
	void shrapnel_damage(player_ptr_t& player,int requested_damage);
	void corrosive_damage(player_ptr_t& player,int requested_damage);
	void cryogenic_damage(player_ptr_t& player,int requested_damage);
	void radioactive_damage(player_ptr_t& player,int requested_damage);
	void emp_damage(player_ptr_t& player,int requested_damage);
	void shock_damage(player_ptr_t& player,int requested_damage);
	void anti_matter_damage(player_ptr_t& player,int requested_damage);
};
#endif

