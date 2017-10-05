#ifndef __MENTOC_TYPES_HEADER__
#define __MENTOC_TYPES_HEADER__
#include "conf.h"
#include "sysdep.h"
#include <memory>
#include <map>
#include "mods/ai_state.hpp"

#define EXTERN_ACT(a) extern void a(struct char_data*, char* argument,int,int);

//enum weapon_type_t { WT_GENERIC =0, WT_SNIPE };
//typedef std::map<struct char_data*,std::unique_ptr<mods::ai_state>> ai_state_map;
#include "structs.h"

#endif
