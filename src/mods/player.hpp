#ifndef __MENTOC_MODS_PLAYER_HEADER__
#define  __MENTOC_MODS_PLAYER_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"

extern size_t send_to_char(struct char_data *ch, const char *messg, ...);

namespace mods {
	namespace weapon {
		enum mask_type { SNIPE, SHOTGUN };
	};
    class player {
		public:
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
			void ammo_adjustment(int);
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
		protected:
			char_data* m_char_data;
    };
};

#endif
