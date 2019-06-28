#ifndef __MENTOC_MODS_CONF_HEADER__
#define __MENTOC_MODS_CONF_HEADER__
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
#include "../mods/util.hpp"
#include "../globals.hpp"
#include <map>

namespace mods::conf {
#ifdef MENTOC_NO_CONSTEXPR_CONF
	static std::string postgres_user = "postgres";
	static std::string postgres_dbname = "postgres_mud";
	static std::string postgres_password = "postgresmudpassword";
	static std::string postgres_host = "localhost";
	static std::string postgres_port = "5432";
#else
	constexpr static std::string_view postgres_user = "postgres";
	constexpr static std::string_view postgres_dbname = "postgres_mud";
	constexpr static std::string_view postgres_password = "postgresmudpassword";
	constexpr static std::string_view postgres_host = "localhost";
	constexpr static std::string_view postgres_port = "5432";
#endif
	std::string pq_connection(std::map<std::string,std::string>);
};
#endif
