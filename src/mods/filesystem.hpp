#ifndef __MENTOC_MODS_FILESYSTEM_HEADER__
#define __MENTOC_MODS_FILESYSTEM_HEADER__
#include "../globals.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>           /* Definition of AT_* constants */



namespace mods::filesystem {
	static inline bool exists(const std::string& path) {
		return access(path.c_str(),F_OK) != -1;
	}
	static inline bool is_directory(const std::string& path) {
		return opendir(path.c_str());
	}
	static inline bool is_file(const std::string& path) {
		struct stat buffer;
		return stat(path.c_str(), &buffer) == 0;
	}
	static inline bool file_exists(const std::string& path) {
		return is_file(path) && exists(path);
	}
	static inline int file_get_contents(std::string_view path, std::string& guts,std::string& error) {
		std::ifstream include_file(path.data(),std::ios::in);

		if(!include_file.is_open()) {
			error = "couldn't find file. is_open failed";
			return -1;
		}
		if(!include_file.good()) {
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
	static inline std::string current_working_directory() {
		char* cwd = ::get_current_dir_name();
		std::string path = cwd == nullptr ? "" : cwd;
		if(cwd) {
			free(cwd);
		}
		return path;
	}

	static inline void file_append(std::string_view path,const std::string& guts) {
		std::ofstream outfile;
		outfile.open(path.data(), std::ios_base::app); // append instead of overwrite
		outfile << guts;
	}

	static inline std::string sanitize_user_name(std::string_view user_name) {
		std::string cleaned = "";

		for(const auto& ch : user_name) {
			if(isalpha(ch) || isdigit(ch)) {
				cleaned += ch;
			}
		}
		return cleaned;
	}


	static inline std::string sanitize_file(std::string_view file) {
		std::string cleaned = "";

		for(const auto& ch : file) {
			if(isalpha(ch) || isdigit(ch) || ch == '.' || ch == '_') {
				cleaned += ch;
			}
		}
		return cleaned;
	}

	static inline void append_to_logins_file(std::string_view user_name, std::string_view ip) {
		std::string cleaned = sanitize_user_name(user_name);

		std::string base_dir = current_working_directory() + "/../log";
		for(const std::string& dir : {
		            "logins",cleaned.c_str()
		        }) {
			base_dir += CAT("/",dir);
			if(is_directory(base_dir) == false) {
				mkdir(base_dir.data(),0755);
			}
		}
		base_dir += CAT("/logins.csv");
		file_append(base_dir,CAT(user_name.data(),",",ip.data(),",",mods::util::time_string()));
	}

	static inline void append_to_user_logins_file(std::string_view user_name, std::string_view file,const std::string& guts) {
		std::string cleaned = sanitize_user_name(user_name);

		std::string base_dir = current_working_directory() + "/../log";
		for(const std::string& dir : {
		            "logins",cleaned.c_str()
		        }) {
			base_dir += CAT("/",dir);
			if(is_directory(base_dir) == false) {
				mkdir(base_dir.data(),0755);
			}
		}
		base_dir += CAT("/",sanitize_file(file.data()));
		file_append(base_dir,guts);
	}

	static inline void append_to_forge_engine_log_file(std::string_view type, std::string_view msg) {
		std::string cleaned = sanitize_user_name(type);

		std::string base_dir = current_working_directory() + "/../log";
		for(const std::string& dir : {
		            "forge-engine",cleaned.c_str()
		        }) {
			base_dir += CAT("/",dir);
			if(is_directory(base_dir) == false) {
				mkdir(base_dir.data(),0755);
			}
		}
		base_dir += CAT("/generated.txt");
		file_append(base_dir,CAT(mods::util::time_string(),":",type.data(),"->",msg.data(),"\n"));
	}
};//end namespace

#endif
