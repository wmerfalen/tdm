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
	static inline int file_get_contents(std::string_view path, std::string& guts,std::string& error){
			std::ifstream include_file(path.data(),std::ios::in);

			if(!include_file.is_open()) {
				error = "couldn't find file. is_open failed";
				return -1;
			}
			if(!include_file.good()){
				error = "file not good. good() is false";
				return -2;
			}
			struct stat statbuf;

			if(stat(path.data(), &statbuf) == -1) {
				error = "couldn't stat file.";
				return -3;
			}

			include_file.seekg(0,include_file.end);
			int length = include_file.tellg();
			guts.reserve(length);
			include_file.seekg(0,include_file.beg);
			std::vector<char> buffer;
			buffer.resize(length+1);
			std::fill(buffer.begin(),buffer.end(),0);
			include_file.read((char*)&buffer[0],length);
			include_file.close();
			guts.resize(length);
			std::copy(buffer.begin(),buffer.end(),guts.begin());
			return length;
		}
};//end namespace

#endif
