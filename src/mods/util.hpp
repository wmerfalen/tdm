#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>
#include "../globals.hpp"
#include <optional>
#include <stdlib.h>

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
			if(c == ' ' && !escaped){
				if(current.length()){
					flush(current);
					current = "";
				}
				continue;
			}
			if(c == ' ' && escaped){
				current += c;
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
	int constexpr length(const char* str)
	{
			return *str ? 1 + length(str + 1) : 0;
	}

	template<int max_char,typename Container>
	inline std::optional<Container> subcmd_args(std::string_view argument,const char* subcmd){
		std::array<char,max_char> command;
		std::fill(command.begin(),command.end(),0);
		one_argument(argument.data(),&command[0],max_char);
		if(std::string(&command[0]).compare(subcmd) == 0){
			std::string arg = argument.data();
			return mods::util::arglist<Container>(arg);
		}
		return {};
	}
	inline std::string itoa(int number){
		std::array<char,16> buf;
		std::fill(buf.begin(),buf.end(),0);
		snprintf(&buf[0],16,"%d",number);
		return &buf[0];
	}
};

#endif
