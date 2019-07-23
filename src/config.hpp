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
	namespace rooms {
		using room_vnum = int64_t;
		using room_rnum = uint64_t;
		constexpr static room_vnum IDLE = 27089;
		constexpr static room_vnum MORTAL_START = 27089;
		room_vnum virtual_idle();
		room_vnum virtual_mortal_start();
		room_rnum real_idle();
		room_rnum real_mortal_start();
	};
	extern bool run_profile_scripts;
}
#endif
