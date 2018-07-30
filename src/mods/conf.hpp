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

namespace mods::conf {
	extern const std::string pq_connection;
	extern std::string postgres_user;
	extern std::string postgres_db;
	extern std::string postgres_password;
	extern short postgres_port;
};
#endif
