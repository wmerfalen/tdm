#include "boot-flags.hpp"
#include <vector>
#include <algorithm>

namespace mods::boot_flags {
	static std::vector<std::string> flags;
	void save_value(std::string arg) {
		flags.emplace_back(arg);
	}
	bool get_flag(std::string option) {
		return std::find(flags.cbegin(),flags.cend(),option) != flags.cend();
	}
};
