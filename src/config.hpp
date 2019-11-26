#ifndef __MENTOC_CONFIG_HEADER__
#define __MENTOC_CONFIG_HEADER__
#include "structs.h"
#include "mods/db.hpp"
#include <memory>
#include <variant>
#include <unordered_map>
namespace mods::globals {
	extern std::unordered_map<std::string,std::string> ram_db;
};
namespace config {
	extern bool run_profile_scripts;
};
#endif
