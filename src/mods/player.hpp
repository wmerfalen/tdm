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
			player() = delete;
			player(char_data* ch);
			bool has_weapon_capability(int);
			bool can_snipe(char_data *target);
			void set_cd(char_data* ch){ m_char_data = ch; }
			const char* name(){ return m_char_data->player.name; }
			uuid_t uuid() const { return m_char_data->uuid; }
			~player() = default;
			char_data* cd() const { return m_char_data; }
			bool is_weapon_loaded();
			bool has_ammo();
			bool has_equipment_tag(const std::string&);
			void weapon_cooldown_start(unsigned long duration,weapon_set set);
			bool weapon_cooldown_expired(weapon_set);
			bool carrying_ammo_of_type(const weapon_type_t &);
			void stc_room(const room_rnum &);
			void stc_room_desc(const room_rnum &);
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
				send_to_char(m_char_data,m.c_str());
			}
			void stc(const char* m){
				send_to_char(m_char_data,m);
			}
			/* TODO: Operator << for sending to the character */
			player& operator<<(const char* m){
				stc(m);	
				return *this;
			}
			player& operator<<(const std::string m){
				stc(m);	
				return *this;
			}
			player& operator<<(int m){
				stc(m);	
				return *this;
			}
			obj_data* weapon();
			obj_data* get_ammo(const weapon_type_t &);
		protected:
			char_data* m_char_data;
			std::array<unsigned long,WEAPON_SET_NUM> m_weapon_cooldown;
			weapon_set m_weapon_set;
    };
};

#endif
