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
	namespace drone {
			player_ptr_t create(uuid_t);
			void start(uuid_t);
			void stop(uuid_t);
			bool started(uuid_t);
			void simulate(uuid_t,bool value);
			bool interpret(uuid_t,const std::string& argument);
			uuid_t get_existing(uuid_t owner);
			void get_drone(uuid_t owner);
	};
	struct drone_impl {
			typedef short weapon_set;
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
			~drone_impl() = default;
			drone_impl() = delete;
			drone_impl(uuid_t u);
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
			drone_impl& operator<<(const char* m) {
				stc(m);
				return *this;
			}
			drone_impl& operator<<(const std::string m) {
				stc(m);
				return *this;
			}
			drone_impl& operator<<(int m) {
				stc(m);
				return *this;
			}
			obj_data* rifle();
			obj_data* get_ammo(const weapon_type_t&);
			void set_scanned(std::vector<uuid_t>);
			std::vector<uuid_t> get_scanned() const;
		protected:
			std::vector<uuid_t> m_scanned;
			char_data* m_char_data;
	};
};

#endif
