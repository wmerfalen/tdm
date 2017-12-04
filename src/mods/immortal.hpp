#ifndef __MENTOC_MODS_IMMORTAL_HEADER__
#define __MENTOC_MODS_IMMORTAL_HEADER__
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
#include "../duktape/src/duktape.h"
#include <sys/stat.h>
#include <string_view>
#include "../interpreter.h"
namespace mods::immortal {
};

ACMD(do_rnumtele);
ACMD(do_rnumlist);
#endif
