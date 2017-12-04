#ifndef __MENTOC_MODS_CHAT_HEADER__
#define __MENTOC_MODS_CHAT_HEADER__
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cstdio>
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "util.hpp"
#include "../globals.hpp"
#include <sys/stat.h>
#include <string_view>
#include "../interpreter.h"
#include <utility>
namespace mods::chat {
	struct channel {
		void set_name(std::string_view);
		void set_verb(std::string_view);
		std::string_view get_name() const;
		std::string_view get_verb() const;
		void add_subscriber(const socket_t &);
		void remove_subscriber(const socket_t &);
		void transmit(std::string_view user,std::string_view message);
		private:
			std::vector<socket_t> m_subscribers;
			std::string m_name;
			std::string m_verb;
	};
};

#endif
