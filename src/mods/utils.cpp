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
			dbg("needle length greater than 3");
				std::smatch our_matches;
				std::string regex_string = needle;
				regex_string = std::string("^") + regex_string + "([^[:space:]]+)";
				std::regex_search(haystack,our_matches,
						std::regex(regex_string),
						match_not_null);
						dbg("size:" << our_matches.size() );
						
				return our_matches.size() > 1;
			}
			return false;
		}
    };
};

#endif
