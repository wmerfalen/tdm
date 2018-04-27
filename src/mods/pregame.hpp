#ifndef __MENTOC_MODS_pregame_HEADER__
#define  __MENTOC_MODS_pregame_HEADER__

#include <iostream>
#include <functional>
#include <queue>
#include <sys/socket.h>
#include <netdb.h>
#include "../conf.h"
#include "../sysdep.h"

namespace mods::pregame {
	void boot_suite(std::string_view);
};

#endif
