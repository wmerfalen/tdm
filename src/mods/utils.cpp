#ifndef __MENTOC_UTILS_SOURCE__
#define  __MENTOC_UTILS_SOURCE__

#include "utils.hpp"

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
    };
};

#endif
