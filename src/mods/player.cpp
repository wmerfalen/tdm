#ifndef __MENTOC_MODS_PLAYER_SOURCE__
#define  __MENTOC_MODS_PLAYER_SOURCE__

#include "player.hpp"
#include "../structs.h"
#include "../utils.h"
#include "weapon.hpp"
#include "../globals.hpp"
#include "acl/color.hpp"
#include "prefs.hpp"

extern struct obj_data* object_list;
extern struct room_data* world;
extern void do_auto_exits(struct char_data *ch);
namespace mods {
	std::string just_color_evaluation(std::string final_buffer){
		final_buffer = mods::globals::replace_all(final_buffer,"{grn}","\033[32m");
		final_buffer = mods::globals::replace_all(final_buffer,"{red}","\033[31m");
		final_buffer = mods::globals::replace_all(final_buffer,"{blu}","\033[34m");
		final_buffer = mods::globals::replace_all(final_buffer,"{wht}","\033[37m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/grn}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/wht}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/red}","\033[0m");
		final_buffer = mods::globals::replace_all(final_buffer,"{/blu}","\033[0m");
		return final_buffer;
	}
	void stc_color_evaluation(const std::string & title,player* p){
		*p << just_color_evaluation(title) << "\r\n";
	}

	std::string word_wrap(std::string_view paragraph,int width){
		std::string buffer;
		if(width <= 0){
			return std::string(paragraph);
		}
		int position = 1;
		for(auto cint = 0; cint < paragraph.length();cint++){
			if(isspace(paragraph[cint])){ 
				if(buffer.begin() != buffer.end() && 
						isspace(*(buffer.end() - 1))){
					continue;
				}else{
					buffer += ' ';
					++position;
				}
			}else{
				buffer += paragraph[cint];
				++position;
			}
			if(position >= width){
				if(!isspace(paragraph[cint])){
					//Perform backtracking
					//====================
					for(int k = buffer.size();k > 0;k--){
						if(isspace(buffer[k])){
							buffer[k] = '\n';
							break;
						}
					}
					position = 1;
				}
			}
		}
		return buffer;
	}

	using mask_t = mods::weapon::mask_type;
	player::player(char_data* ch) : m_char_data(ch),m_do_paging(false),
		m_page(0),m_current_page(0),m_current_page_fragment("") { };
	bool player::can_snipe(char_data *target){
		return true;
	}
	void player::page(int pg){
		if(m_pages.size() == 0 || pg * mods::player::PAGE_SIZE >= m_pages.size()){
			return;
		}
		std::string acc = "";
		unsigned i = pg * mods::player::PAGE_SIZE;
		for(; i < m_pages.size() && i < (pg + 1) * mods::player::PAGE_SIZE;i++){
			acc += m_pages[i];
		}
		if(i != m_pages.size()){
			acc += std::string("--[ page number ") + std::to_string(pg +1)  + "/"; 
			acc += std::to_string((m_pages.size() / mods::player::PAGE_SIZE) + 1) + \
				   ":: press (q) to quit or page number ]--";
		}else{
			m_do_paging = false;
			m_current_page = 0;
		}
		if(acc.length()){
			stc(acc);
			m_current_page = pg;
		}
	}
	void player::pager_next_page(){
		if((m_current_page +1) * mods::player::PAGE_SIZE >= m_pages.size()){
			m_do_paging = false;
			m_current_page = 0;
			return;
		}
		page(m_current_page +1);
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
	bool player::has_inventory_capability(int type){
		return true; //FIXME: 
	}
	void player::weapon_cooldown_start(unsigned long duration,weapon_set set){
		m_weapon_cooldown[set] = static_cast<unsigned long>(time(NULL)) + duration;
	}
	bool player::weapon_cooldown_expired(weapon_set set) {
		auto cts = static_cast<unsigned long>(time(NULL));
		return cts >= m_weapon_cooldown[set];
	}
	bool player::carrying_ammo_of_type(const weapon_type_t& type){
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
	bool player::has_thermite(){
		if(!m_char_data->carrying){ return false;}
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(m_char_data != item->carried_by){ return false; }
			if(item->weapon_type == std::hash<std::string>{}("thermite")){
				return true;
			}
		}
		return false;
	}
	bool player::has_equipment_tag(const std::string& tag) {
		if(!m_char_data->carrying){ return false; }
		/* TODO: find a better way to do this. this loops through all objects in the game and checks if the person carrying it is the current player */
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(std::string(item->name).find(std::string("[") + std::string(tag) + "]") != std::string::npos
				&&
				m_char_data == item->carried_by
			){
				return true;
			}
		}
		return false;
	}
	void player::ammo_adjustment(int increment){
		weapon()->ammo += increment;
	}
	player& player::pager_start(){
		m_do_paging = true;
		m_pages.clear();
		m_page = 0;
		return *this;
	}
	player& player::pager_end(){
		m_do_paging = false;
		if(m_current_page_fragment.length()){
			if(m_pages.begin() == m_pages.end()){
				m_pages.push_back(m_current_page_fragment);
			}else{
				auto it = m_pages.end() -1;
				*it += m_current_page_fragment;
			}
		}
		m_current_page_fragment = "";
		return *this;
	}
	void player::pager_clear(){
		m_pages.clear();
	}
	obj_data* player::get_first_ammo_of_type(const weapon_type_t & type) const {
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(item->weapon_type == type
				&&
				std::string(item->name).find("[ammo]")
				&&
				m_char_data == item->carried_by
			){
				return item;
			}
		}
		return nullptr;
	}
	/* returns:
	 * 	the `ammo` trait of the obj_data struct after the increment has been applied.
	 * if 0 (zero) is returned, then nothing was subtraced/added from the user's
	 * ammo supply. This means the user doesn't have that ammo type
	 */
	int player::ammo_type_adjustment(int increment,const weapon_type_t & type){
		if(!m_char_data->carrying){ return 0; }
		for(auto item = m_char_data->carrying; item->next; item = item->next){
			if(item->weapon_type == type
				&&
				std::string(item->name).find("[ammo]") != std::string::npos
				&&
				m_char_data == item->carried_by
			){
				item->ammo += increment;
				return item->ammo;
			}
		}
	}
	void player::stc_room(const room_rnum & rnum){
		if(world[rnum].name){
			std::string title = world[rnum].name;
			stc_color_evaluation(std::string("{grn}") + title + "{/grn}",this);
		}
	}


	void player::stc_room_desc(const room_rnum & rnum){
		if(world[rnum].description){
			std::string colored = just_color_evaluation(world[rnum].description);
			/* TODO: get status of outside world, if EMP, then replace phrase with emp phrase */
			auto player = this;
			auto value = PLAYER_GET("screen_width");
			if(value.length()){
				stc(word_wrap(colored,std::stoi(value)) + "\r\n");
				return;
			}else{
				stc(word_wrap(colored,80) + "\r\n");
			}
		}
	}
	obj_data* player::weapon(){
		return GET_EQ(m_char_data, WEAR_WIELD);
	}
	obj_data* player::get_ammo(const weapon_type_t & type){
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
	void player::exits(){
		do_auto_exits(m_char_data);
	}
};

#endif
