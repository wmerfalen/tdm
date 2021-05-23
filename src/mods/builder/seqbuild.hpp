#ifndef __MENTOC_MODS_BUILDER_CONTRACT_HEADER__
#define __MENTOC_MODS_BUILDER_CONTRACT_HEADER__
#include "../structs.h"
#include <array>
#include "../mods/util.hpp"
#include "../globals.hpp"
#include "../interpreter.h"

extern void r_error(const player_ptr_t& player,std::string_view msg);
extern void r_success(const player_ptr_t& player,std::string_view msg);
extern void r_status(const player_ptr_t& player,std::string_view msg);
namespace mods::builder {
};

#endif
