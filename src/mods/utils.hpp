#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>

std::string operator "" _s(const char* s,long unsigned int i){
	return std::string(s);
}
#define dbg(a){ std::cerr << "debug[]: " << a << "\n"; }
namespace mods {
    namespace util {
		bool fuzzy_match(const char* _needle,const char* _haystack);
    };
};

#endif
