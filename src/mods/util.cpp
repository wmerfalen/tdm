#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "util.hpp"
#include "flags.hpp"


namespace mods {
	namespace util {
			//uint64_t aff2legacy(mods::flags::aff f){
			//	for(unsigned i=0; i < mods::flags::aff_flags.size();i++){
			//		if(mods::flags::aff_flags[i].first == f){
			//			return mods::flags::aff_flags[i].second;
			//		}
			//	}
			//	return 0;
			//}
			//mods::flags::aff legacy2aff(uint64_t f){
			//	for(unsigned i=0; i < mods::flags::aff_flags.size();i++){
			//		if(mods::flags::aff_flags[i].second == f){
			//			return mods::flags::aff_flags[i].first;
			//		}
			//	}
			//	return mods::flags::aff::__AFF_FIRST;
			//}
			///**
			// * @return returns the legacy PLR_ flag given the modern plr flag
			// */
			//uint64_t plr2legacy(mods::flags::plr f){
			//	for(unsigned i=0; i < mods::flags::plr_flags.size();i++){
			//		if(mods::flags::plr_flags[i].first == f){
			//			return mods::flags::plr_flags[i].second;
			//		}
			//	}
			//	return 0;
			//}
			//mods::flags::plr legacy2plr(uint64_t f){
			//	for(unsigned i=0; i < mods::flags::plr_flags.size();i++){
			//		if(mods::flags::plr_flags[i].second == f){
			//			return mods::flags::plr_flags[i].first;
			//		}
			//	}
			//	return mods::flags::plr::__PLR_FIRST;
			//}
		bool preg_match(std::string_view regex,std::string_view haystack) {
			using namespace std::regex_constants;
			return std::regex_search(haystack.data(), std::regex(regex.data()), match_not_null);
		}

		bool fuzzy_match(const std::string& _needle,const std::string & _haystack) {
			std::string needle = _needle,haystack = _haystack;

			/* If matches EXACTLY (strcmp) */
			if(needle.compare(haystack) == 0) {
				return true;
			}

			if(needle.length()) {
				/* If the string is composed of spaces, do a match against each word */
				std::vector<std::string> words;
				std::string current = "";

				for(auto character : haystack) {
					if(character == ' ' && current.length()) {
						words.push_back(current);
						current.clear();
						continue;
					}
					current += character;
				}

				if(current.size()){
					words.push_back(current);
				}

				for(auto str : words) {
					if(std::regex_search(str, std::regex(needle.c_str(),std::regex_constants::ECMAScript | std::regex_constants::icase), std::regex_constants::match_not_null)) {
						return true;
					}
				}
			}

			return false;
		}
		std::string color_eval(std::string final_buffer) {
			final_buffer = mods::globals::replace_all(final_buffer,"{grn}","\033[32m");
			final_buffer = mods::globals::replace_all(final_buffer,"{red}","\033[31m");
			final_buffer = mods::globals::replace_all(final_buffer,"{blu}","\033[34m");
			final_buffer = mods::globals::replace_all(final_buffer,"{wht}","\033[37m");
			final_buffer = mods::globals::replace_all(final_buffer,"{/grn}","\033[0m");
			final_buffer = mods::globals::replace_all(final_buffer,"{/wht}","\033[0m");
			final_buffer = mods::globals::replace_all(final_buffer,"{/red}","\033[0m");
			final_buffer = mods::globals::replace_all(final_buffer,"{/blu}","\033[0m");
			return final_buffer;
		}
		std::optional<unsigned> stoul(std::string_view str) {
			try {
				return std::stoul(str.data());
			} catch(...) {
				return std::nullopt;
			}
		}
		std::optional<int> stoi(std::string_view str) {
			try {
				return std::stoi(str.data());
			} catch(...) {
				return std::nullopt;
			}
		}
	std::string itoa(int number) {
		std::array<char,16> buf;
		std::fill(buf.begin(),buf.end(),0);
		snprintf(&buf[0],16,"%d",number);
		return &buf[0];
	}
		directory_list_t glob(std::string_view path){
			directory_list_t files;
#ifdef MENTOC_CPP_2017
			std::string path = dir.data();
			for (auto & p : std::filesystem::directory_iterator(path))
				files.emplace_back(p);
#else
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir (path.data())) != NULL) {
			  /* print all the files and directories within directory */
			  while ((ent = readdir (dir)) != NULL) {
				files.emplace_back(ent->d_name);
			  }
			  closedir (dir);
			  return files;
			} else {
				return {};
			}
#endif
		}
		std::function<void(const std::string&,std::string&)> sanitize = [](const std::string & command,std::string& sanitized){
		sanitized = "";
		for(auto ch: command){
			if(std::isalpha(ch)){
				sanitized += ch;
			}
		}
	};

	bool dir_exists(const char* dir){
		auto dir_fp = opendir(dir);
		if(dir_fp == nullptr){
			return false;
		}
		else{
		closedir(dir_fp);
		return true;
		}
	}

	};
};

namespace mods::util::err {
	std::string get_string(int _errno){
		std::array<char,256> buf;
		std::fill(buf.begin(),buf.end(),0);
		strerror_r(_errno,static_cast<char*>(&buf[0]),255);
		return std::string(static_cast<const char*>(&buf[0]));
	}
};

#endif
