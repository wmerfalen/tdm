#ifndef __MENTOC_TYPES_HEADER__
#define __MENTOC_TYPES_HEADER__
#include "conf.h"
#include "sysdep.h"
#include <memory>
#include <map>
#include "mods/ai_state.hpp"

#define EXTERN_ACT(a) extern void a(struct char_data*, char* argument,int,int);
#include "structs.h"

#endif
