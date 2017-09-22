#ifndef __MENTOC_MODS_PLAYER_SOURCE__
#define  __MENTOC_MODS_PLAYER_SOURCE__

#include "player.hpp"
#include "../structs.h"
#include "../utils.h"
#include "weapon.hpp"
#include "../globals.hpp"

extern struct obj_data* object_list;
namespace mods {
	using mask_t = mods::weapon::mask_type;
	player::player(char_data* ch) : m_char_data(ch) { };
	bool player::can_snipe(char_data *target){
		return true;
	}
	bool player::has_weapon_capability(int type){
		auto w = weapon();
		if(!w){
			return false;
		}
		switch(type){
			case mods::weapon::mask::snipe:
				return !!(std::string(w->name).find("snipe") != std::string::npos);
				break;
			default:
				return false;
		}
		return false;
	}
	bool player::is_weapon_loaded(){
		auto w = weapon();
		if(!w){
			return false;
		}
		return true;
	}
	bool player::has_ammo(){
		auto w = weapon();
		if(w->ammo > 0){
			return true;
		}else{
			return false;
		}
	}
	bool player::has_equipment_tag(const std::string& tag) const {
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(std::string(item->name).find(tag) != std::string::npos)
				return true;
		}
		return false;
	}
	void player::ammo_adjustment(int increment){
		weapon()->ammo += increment;
	}
	obj_data* player::weapon(){
		return GET_EQ(m_char_data, WEAR_WIELD);
	}
};

#endif
