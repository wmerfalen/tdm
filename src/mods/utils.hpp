#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>

#ifdef MENTOC_DEBUG
#define dbg(a){ std::cerr << "debug[]: " << a << "\n"; }
#else
#define dbg(a)/* */
#endif
namespace mods {
    namespace util {
		bool fuzzy_match(const char* _needle,const char* _haystack);
    };
};

#endif
