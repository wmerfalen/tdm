#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"

#define WEAPON_SET_NUM 1
extern size_t send_to_char(struct char_data *ch, const char *messg, ...);

namespace mods {
	namespace weapon {
		enum mask_type { SNIPE, SHOTGUN };
	};
    class player {
		public:
			typedef short weapon_set;
			using chdata = struct char_data;
			using chdata_ptr = struct char_data *;
			player() = delete;
			player(char_data* ch);
			static constexpr int PAGE_SIZE = 40;
			bool has_weapon_capability(int);
			bool has_inventory_capability(int);
			bool can_snipe(char_data *target);
			void set_cd(char_data* ch){ m_char_data = ch; }
			const char* name(){ return m_char_data->player.name; }
			uuid_t uuid() const { return m_char_data->uuid; }
			~player() = default;
			char_data* cd() const { return m_char_data; }
			bool is_weapon_loaded();
			bool has_ammo();
			bool has_thermite();
			bool has_equipment_tag(const std::string&);
			void weapon_cooldown_start(unsigned long duration,weapon_set set);
			bool weapon_cooldown_expired(weapon_set);
			bool carrying_ammo_of_type(const weapon_type_t &);
			void stc_room(const room_rnum &);
			void stc_room_desc(const room_rnum &);
			void exits();
			player& pager_start();
			player& pager_end();
			void pager_next_page();
			void page(int page_number);
			/*
			void weapon_cooldown_clear(weapon_set);
			bool has_weapon_cooldown(weapon_set set){
				if(m_cooldowns.find(set) == m_cooldowns.end()){
					return false;
				}
				return true;
			}
			int weapon_cooldown_increment(){
				return ++m_weapon_set;
			}
			*/
			void ammo_adjustment(int);
			int ammo_type_adjustment(int,const weapon_type_t&);
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;
			void stc(int m){ 
				send_to_char(m_char_data,std::to_string(m).c_str());
			}
			void stc(const std::string m){
				if(m.length()){
					send_to_char(m_char_data,m.c_str());
				}
			}
			void stc(const char* m){
				if(m){
					send_to_char(m_char_data,m);
				}
			}
			void queue_page_fragment(std::string_view fragment){
				for(auto c : fragment){
					m_current_page_fragment += c;
					if(c == '\n'){
						m_pages.push_back(m_current_page_fragment);
						m_current_page_fragment = "";
					}
				}
			}
			/* conversion operator to char_data* */
			operator chdata_ptr() const {
				return m_char_data;
			}
			player& operator<<(const char* m){
				if(m_do_paging){
					queue_page_fragment(m);
				}else{
					stc(m);	
				}
				return *this;
			}
			player& operator<<(const std::string m){
				if(m_do_paging){
					queue_page_fragment(m);
				}else{
					stc(m);	
				}
				return *this;
			}
			player& operator<<(int m){
				if(m_do_paging){
					queue_page_fragment(std::to_string(m));
				}else{
					stc(m);	
				}
				return *this;
			}
			void pager_clear();
			bool paging() const { return m_pages.size(); }
			obj_data* weapon();
			obj_data* get_ammo(const weapon_type_t &);
		protected:
			char_data* m_char_data;
			std::array<unsigned long,WEAPON_SET_NUM> m_weapon_cooldown;
			weapon_set m_weapon_set;
			bool m_do_paging;
			unsigned int m_page;
			unsigned int m_current_page;
			std::string m_current_page_fragment;
			std::vector<std::string> m_pages;
    };
};

#include "util.hpp"

#endif
