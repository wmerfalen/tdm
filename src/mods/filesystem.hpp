#ifndef __MENTOC_MODS_FILESYSTEM_HEADER__
#define __MENTOC_MODS_FILESYSTEM_HEADER__
#include "../globals.hpp"
#include <dirent.h>
#include <sys/stat.h>
namespace mods::filesystem {
	static inline bool exists(const std::string& path){
		return access(path.c_str(),F_OK) != -1;
	}
	static inline bool is_directory(const std::string& path){
		return opendir(path.c_str());
	}
	static inline bool is_file(const std::string& path){
		struct stat buffer;   
		return stat(path.c_str(), &buffer) == 0; 
	}
	static inline bool file_exists(const std::string& path){
		return is_file(path) && exists(path);
	}
};//end namespace

#endif
