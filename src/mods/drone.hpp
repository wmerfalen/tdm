#ifndef __MENTOC_MODS_DRONE_HEADER__
#define  __MENTOC_MODS_DRONE_HEADER__

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"
#include "../structs.h"
#include "../types.hpp"
#include "../globals.hpp"
#include "extern.hpp"

#define DRONE_WEAPON_SET_NUM 2
extern size_t send_to_char(char_data *ch, const char *messg, ...);

namespace mods {
	class drone {
		public:
			typedef short weapon_set;
			drone();
			drone(char_data* ch);
			static char_data* create(char_data* owner);
			static void start(char_data * owner);
			static void stop(char_data * owner);
			static bool started(char_data *owner);
			static void simulate(char_data *owner,bool value);
			static bool interpret(char_data *owner,const std::string& argument);
			static char_data * get_existing(char_data* owner);
			static void get_drone(char_data* owner);
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
			obj_data* rifle();
			obj_data* get_ammo(const weapon_type_t&);
		protected:
			char_data* m_char_data;
	};
};

#endif
