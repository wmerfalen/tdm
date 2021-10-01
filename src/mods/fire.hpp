#ifndef __MENTOC_MODS_fire_HEADER__
#define __MENTOC_MODS_fire_HEADER__
#include "../globals.hpp"

namespace mods::fire {
	/* number of tics (usually 75 seconds) before PC/NPC fires decompose */
	static constexpr uint8_t max_npc_fire_time = 5;
	static constexpr uint8_t max_pc_fire_time = 10;
	obj_ptr_t make_fire(player_ptr_t& victim);
	void fire_explode(player_ptr_t& attacker,obj_ptr_t& fire,const uint16_t& damage);
	void queue_fire_explode(obj_ptr_t& fire,player_ptr_t& attacker,const uint16_t& damage);
	void queue_hellfire_fire_explode(obj_ptr_t& fire,player_ptr_t& attacker,const uint16_t& damage);

	bool deploy_fire_damage_to_victim_via_device(player_ptr_t& victim,obj_ptr_t& device);
	void set_fire_to_room_via_device(const room_rnum& room,obj_ptr_t& device);

	void queue_shrapnel_fire_explode(obj_ptr_t& fire,player_ptr_t& attacker,const uint16_t& damage);
	void process_fire_explosion();
	/**
	 * Return true if target died
	 */
	bool perform_fire_blast_radius(player_ptr_t& victim,obj_ptr_t& explosive_device,uint8_t bast_count,const direction_t& from);
	/**
	 * Return true if target died
	 */
	bool deal_fire_explosion_damage_to(player_ptr_t& victim,obj_ptr_t& explosive_device);
	constexpr uint8_t tick_resolution() {
		return 20;
	}
};
#endif
