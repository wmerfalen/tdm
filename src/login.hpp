#ifndef __MENTOC_LOGIN_HEADER__
#define __MENTOC_LOGIN_HEADER__
#define __INTERPRETER_C__

#include "globals.hpp"
#include "mods/ban_system.hpp"
#include "structs.h"
/** mods/js.hpp potentially for calling js scripts at login */
#include "mods/js.hpp"
#include "mods/crypto.hpp"

namespace login {
	bool is_valid_name(std::string_view name);
};

#endif
