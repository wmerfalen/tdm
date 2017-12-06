#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "util.hpp"

std::string operator "" _s(const char* s,long unsigned int i){
   return std::string(s);
}
namespace mods {
    namespace util {
		bool fuzzy_match(const char* _needle,const char* _haystack){
			std::string needle = _needle,haystack = _haystack;
			using namespace std::regex_constants;
			/* If matches EXACTLY (strcmp) */
			if(needle.compare(haystack) == 0){
			dbg("haystack compmare");
				return true;
			}
			if(needle.length() >= 1){
				/* If the string is composed of spaces, do a match against each word */
				std::vector<std::string> words;
				std::string current = "";
				for(auto character : haystack){
					if(character == ' ' && current.length()){
						words.push_back(current);
						continue;
					}
					current += character;
				}
				for(auto string : words){
					if(std::regex_search(string, std::regex(needle), match_not_null)){
						return true;
					}
				}
			}
			return false;
		}
		std::string color_eval(std::string final_buffer){
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
		std::optional<int> stoi(std::string_view str,int base =10){
			try{
				return std::stoi(str.data());
			}catch(...){
				return std::nullopt;
			}
		}
    };
};

#endif
