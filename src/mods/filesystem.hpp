#ifndef __MENTOC_MODS_FILESYSTEM_HEADER__
#define __MENTOC_MODS_FILESYSTEM_HEADER__
#include "../globals.hpp"
namespace mods::filesystem {
	static inline bool exists(std::string path){
		return access(path.c_str(),F_OK) != -1;
	}
};//end namespace

#endif
