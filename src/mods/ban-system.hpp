#ifndef __MENTOC_MODS_BAN_SYSTEM_HEADER__
#define __MENTOC_MODS_BAN_SYSTEM_HEADER__
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../globals.hpp"
namespace mods::ban_system {
	namespace ip {
		bool is_banned(std::string ip_address);
		bool is_banned(const sockaddr_in& peer);
	};
	namespace hostname {
		bool is_banned(std::string hostname);
		bool is_banned(const sockaddr_in& peer);
	};
	namespace username {
		bool is_banned(std::string user_name);
	};
};
#endif
