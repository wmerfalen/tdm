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
	std::tuple<bool,std::string> ban_player(player_ptr_t& player);

	std::tuple<bool,std::string> ban_username(std::string_view username);
	std::tuple<bool,std::string> ban_ip(std::string_view ip);
	std::tuple<bool,std::string> ban_hostname(std::string_view hostname);

	std::tuple<bool,std::string> unban_username(std::string_view username);
	std::tuple<bool,std::string> unban_ip(std::string_view ip);
	std::tuple<bool,std::string> unban_hostname(std::string_view hostname);
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
