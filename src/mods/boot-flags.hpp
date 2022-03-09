#ifndef __MODS_BOOT_FLAGS_HEADER__
#define __MODS_BOOT_FLAGS_HEADER__

#include <string>

namespace mods::boot_flags {
	void save_value(std::string arg);
	bool get_flag(std::string option);
};

#define boot_flag(A) mods::boot_flags::get_flag(A)

#endif
