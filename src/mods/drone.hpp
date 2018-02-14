#ifndef __MENTOC_MODS_DRONE_HEADER__
#define  __MENTOC_MODS_DRONE_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include "extern.hpp"

#define DRONE_WEAPON_SET_NUM 2
extern size_t send_to_char(struct char_data *ch, const char *messg, ...);

namespace mods {
	class drone {
		public:
			typedef short weapon_set;
			drone();
			drone(char_data* ch);
			static struct char_data* create(struct char_data* owner);
			static void start(struct char_data * owner);
			static void stop(struct char_data * owner);
			static bool started(struct char_data *owner);
			static void simulate(struct char_data *owner,bool value);
			static bool interpret(struct char_data *owner,const std::string& argument);
			static struct char_data * get_existing(struct char_data* owner);
			static void get_drone(struct char_data* owner);
			/*
			bool has_weapon_capability(int);
			bool has_inventory_capability(int);
			*/
			void set_cd(char_data* ch) {
				m_char_data = ch;
			}
			const char* name() {
				return m_char_data->player.name;
			}
			uuid_t uuid() const {
				return m_char_data->uuid;
			}
			~drone() = default;
			char_data* cd() const {
				return m_char_data;
			}
			/*
			bool is_weapon_loaded();
			bool has_ammo();
			bool has_equipment_tag(const std::string&);
			void weapon_cooldown_start(unsigned long duration,weapon_set set);
			bool weapon_cooldown_expired(weapon_set);
			bool carrying_ammo_of_type(const weapon_type_t &);
			void stc_room(const room_rnum &);
			void stc_room_desc(const room_rnum &);
			void exits();
			*/
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
			/*
			void ammo_adjustment(int);
			int ammo_type_adjustment(int,const weapon_type_t&);
			obj_data* get_first_ammo_of_type(const weapon_type_t&) const;
			*/
			void stc(int m) {
				send_to_char(m_char_data,"%s",std::to_string(m).c_str());
			}
			void stc(const std::string m) {
				send_to_char(m_char_data,"%s",m.c_str());
			}
			void stc(const char* m) {
				send_to_char(m_char_data,"%s",m);
			}
			/* TODO: Operator << for sending to the character */
			drone& operator<<(const char* m) {
				stc(m);
				return *this;
			}
			drone& operator<<(const std::string m) {
				stc(m);
				return *this;
			}
			drone& operator<<(int m) {
				stc(m);
				return *this;
			}
			obj_data* weapon();
			obj_data* get_ammo(const weapon_type_t&);
		protected:
			char_data* m_char_data;
			/*
			std::array<unsigned long,DRONE_WEAPON_SET_NUM> m_weapon_cooldown;
			weapon_set m_weapon_set;
			*/
	};
};

#endif
