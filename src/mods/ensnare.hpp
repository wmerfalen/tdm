#ifndef __MENTOC_MODS_ensnare_HEADER__
#define __MENTOC_MODS_ensnare_HEADER__
#include "../globals.hpp"

namespace mods::ensnare {
	bool is_ensnared(player_ptr_t victim);
	void ensnare_damage(player_ptr_t attacker,player_ptr_t victim, obj_ptr_t item,int damage);
	void cause_glass_damage_to_feet(player_ptr_t victim, int damage);
	void queue_ensnare_player(obj_ptr_t device,player_ptr_t& victim,const int16_t& damage);
	void process_ensnare();
	void unqueue_player(const uuid_t& player_uuid);
	constexpr uint8_t tick_resolution() {
		return 20;
	}
};
#endif
