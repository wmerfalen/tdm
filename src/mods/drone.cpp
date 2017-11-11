#ifndef __MENTOC_MODS_DRONE_SOURCE__
#define  __MENTOC_MODS_DRONE_SOURCE__

#include "drone.hpp"
#include "../structs.h"
#include "../utils.h"
#include "weapon.hpp"
#include "../globals.hpp"
#include "acl/color.hpp"
#include "player.hpp"

extern struct obj_data* object_list;
extern struct room_data* world;
extern void do_auto_exits(struct char_data *ch);
namespace mods {
	drone::drone(char_data* ch) : m_char_data(ch) { };
	static drone& drone::create(struct char_data* owner){
		struct char_data *ch;
		mods::globals::create_char(ch);
		ch->drone = true;
		ch->desc = owner->desc;
		mods::globals::register_player(ch);
	}
	
//	void drone::stc_room(const room_rnum & rnum){
//		if(world[rnum].name){
//			std::string title = world[rnum].name;
//			mods::player::stc_color_evaluation(std::string("{grn}") + title + "{/grn}",this);
//		}
//	}
//
//	void drone::stc_room_desc(const room_rnum & rnum){
//		if(world[rnum].description){
//			std::string colored = mods::player::just_color_evaluation(world[rnum].description);
//			/* TODO: get status of outside world, if EMP, then replace phrase with emp phrase */
//			stc(colored);
//		}
//	}
	/*
	obj_data* drone::weapon(){
		return GET_EQ(m_char_data, WEAR_WIELD);
	}
	*/
	//using mask_t = mods::weapon::mask_type;
	/*
	bool drone::has_weapon_capability(int type){
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
	bool drone::has_inventory_capability(int type){
		return true; //FIXME: 
	}
	void drone::weapon_cooldown_start(unsigned long duration,weapon_set set){
		m_weapon_cooldown[set] = static_cast<unsigned long>(time(NULL)) + duration;
	}
	bool drone::weapon_cooldown_expired(weapon_set set) {
		auto cts = static_cast<unsigned long>(time(NULL));
		return cts >= m_weapon_cooldown[set];
	}
	bool drone::carrying_ammo_of_type(const weapon_type_t& type){
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(std::string(item->name).find("[ammo]") != std::string::npos
				&&
				m_char_data == item->carried_by
				&&
				type == item->weapon_type
			){
				return true;
			}
		}
		return false;
	}
	bool drone::is_weapon_loaded(){
		auto w = weapon();
		if(!w){
			return false;
		}
		return true;
	}
	bool drone::has_ammo(){
		auto w = weapon();
		if(w->ammo > 0){
			return true;
		}else{
			return false;
		}
	}
	bool drone::has_thermite(){
		if(!m_char_data->carrying){ return false;}
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(m_char_data != item->carried_by){ return false; }
			if(item->weapon_type == std::hash<std::string>{}("thermite")){
				return true;
			}
		}
		return false;
	}
	*/
	//bool drone::has_equipment_tag(const std::string& tag) {
	//	if(!m_char_data->carrying){ return false; }
	//	/* TODO: find a better way to do this. this loops through all objects in the game and checks if the person carrying it is the current drone */
	//	for(auto item = m_char_data->carrying; item->next; item = item->next){
	//		if(std::string(item->name).find(std::string("[") + std::string(tag) + "]") != std::string::npos
	//			&&
	//			m_char_data == item->carried_by
	//		){
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	//void drone::ammo_adjustment(int increment){
	//	weapon()->ammo += increment;
	//}
	//obj_data* drone::get_first_ammo_of_type(const weapon_type_t & type) const {
	//	for(auto item = m_char_data->carrying; item->next; item = item->next){
	//		if(item->weapon_type == type
	//			&&
	//			std::string(item->name).find("[ammo]")
	//			&&
	//			m_char_data == item->carried_by
	//		){
	//			return item;
	//		}
	//	}
	//	return nullptr;
	//}
	///* returns:
	// * 	the `ammo` trait of the obj_data struct after the increment has been applied.
	// * if 0 (zero) is returned, then nothing was subtraced/added from the user's
	// * ammo supply. This means the user doesn't have that ammo type
	// */
	//int drone::ammo_type_adjustment(int increment,const weapon_type_t & type){
	//	if(!m_char_data->carrying){ return 0; }
	//	for(auto item = m_char_data->carrying; item->next; item = item->next){
	//		if(item->weapon_type == type
	//			&&
	//			std::string(item->name).find("[ammo]") != std::string::npos
	//			&&
	//			m_char_data == item->carried_by
	//		){
	//			item->ammo += increment;
	//			return item->ammo;
	//		}
	//	}
	//}
	/*
	obj_data* drone::get_ammo(const weapon_type_t & type){
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(std::string(item->name).find("[ammo]") != std::string::npos
				&&
				m_char_data == item->carried_by
				&&
				type == item->weapon_type
			){
				return item;
			}
		}
		return nullptr;
	}
	void drone::exits(){
		do_auto_exits(m_char_data);
	}*/
};

#endif
