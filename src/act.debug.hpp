#ifndef __MENTOC_ACT_DEBUG_HEADER__
#define __MENTOC_ACT_DEBUG_HEADER__
#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "globals.hpp"

extern void point_update(void);

ACMD(do_givemenades);
ACMD(do_point_update);
ACMD(do_uuid);
ACMD(do_zero_socket);

#endif
