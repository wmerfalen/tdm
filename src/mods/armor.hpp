#ifndef __MENTOC_MODS_ARMOR_HEADER__
#define __MENTOC_MODS_ARMOR_HEADER__
#include "../globals.hpp"
namespace mods::armor {
	bool is_basic_protection(obj_ptr_t& eq);
	bool is_advanced_protection(obj_ptr_t& eq);
	bool is_elite_protection(obj_ptr_t& eq);
};
#endif
