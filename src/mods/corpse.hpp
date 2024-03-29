#ifndef __MENTOC_MODS_corpse_HEADER__
#define __MENTOC_MODS_corpse_HEADER__
#include "../globals.hpp"

namespace mods::corpse {
	/* number of tics (usually 75 seconds) before PC/NPC corpses decompose */
	static constexpr uint8_t max_npc_corpse_time = 5;
	static constexpr uint8_t max_pc_corpse_time = 10;
	obj_ptr_t make_corpse(player_ptr_t& victim);
	void corpse_explode(player_ptr_t& attacker,obj_ptr_t& corpse,const uint16_t& damage);
	void queue_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage);
	void queue_hellfire_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage);
	void queue_shrapnel_corpse_explode(obj_ptr_t& corpse,player_ptr_t& attacker,const uint16_t& damage);
	void process_corpse_explosion();
	std::tuple<int16_t,std::string> drag_corpse(player_ptr_t& dragger,obj_ptr_t& corpse,const direction_t& direction,const uint16_t& force);
	std::tuple<bool,std::string,obj_ptr_t> pick_corpse_from_room_by_argument(player_ptr_t& player, std::string_view argument);
	/**
	 * Return true if target died
	 */
	bool perform_corpse_blast_radius(player_ptr_t& victim,obj_ptr_t& explosive_device,uint8_t bast_count,const direction_t& from);
	/**
	 * Return true if target died
	 */
	bool deal_corpse_explosion_damage_to(player_ptr_t& victim,obj_ptr_t& explosive_device);
	constexpr uint8_t tick_resolution() {
		return 20;
	}
};
#endif
