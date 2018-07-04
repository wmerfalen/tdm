#ifndef __MENTOC_MODS_BAN_SYSTEM_HEADER__
#define __MENTOC_MODS_BAN_SYSTEM_HEADER__
typedef int socket_t;
#include "../structs.h"
#include "../utils.h"
#include "../conf.h"
#include "../sysdep.h"
#include <array>
#include "../globals.hpp"
namespace mods {
	namespace ban_system {
		bool isbanned(const std::string &m){ /** FIXME */ return false; }
	};
};
#endif
