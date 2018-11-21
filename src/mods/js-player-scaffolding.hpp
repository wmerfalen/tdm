#ifndef __MENTOC_MODS_JS_PLAYER_SCAFFOLDING_HEADER__
#define __MENTOC_MODS_JS_PLAYER_SCAFFOLDING_HEADER__
//#include <time.h>
//#include <netdb.h>
//#include <sys/socket.h>
//#include <cstdio>
//typedef int socket_t;
//#include "../structs.h"
//#include "../utils.h"
//#include "../conf.h"
//#include "../sysdep.h"
//#include <array>
//#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../duktape/src/duktape.h"
//#include <sys/stat.h>
#include <string_view>
#include "util.hpp"
//#include "extern.hpp"
#include "loops.hpp"
//#include "../spells.h"
#include "db.hpp"
namespace mods::js::player {
	struct scaffold {
		duk_context* m_context;
	};
	duk_context* new_context();
};
#endif
