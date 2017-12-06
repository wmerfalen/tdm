#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>
#include "../globals.hpp"

#ifdef MENTOC_DEBUG
#define dbg(a){ std::cerr << "debug[]: " << a << "\n"; }
#else
#define dbg(a)/* */
#endif
namespace mods::util{
	bool fuzzy_match(const char* _needle,const char* _haystack);
	std::string color_eval(std::string final_buffer);
	template <typename T>
	T arglist(const std::string& argument){
		T arglist;
		auto flush = [&](std::string_view f){
			arglist.emplace_back(f.data());
		};
		bool escaped = false;
		std::string current = "";
		unsigned string_length = argument.length();
		for(auto c : argument){
			--string_length;
			if(c == ' '){
				if(!escaped && current.length()){
					flush(current);
					current = "";
				}
			}
			if(c == '"'){
				if(escaped){
					flush(current);
					escaped = false;
					current = "";
				}else{
					escaped = true;
				}
				continue;
			}
			current += c;
			if(string_length ==0 && current.length()){
				flush(current);
				break;
			}
		}
		return arglist;
	}
	std::optional<int> stoi(std::string_view str,int base=10);
};

#endif
