#ifndef __MENTOC_UTILS_HEADER__
#define  __MENTOC_UTILS_HEADER__

#include <iostream>
#include <string>
#include <regex>
#include "../globals.hpp"
#include <optional>
#include <stdlib.h>
#include <sys/types.h>	/** for opendir() */
#include <dirent.h>			/** for opendir() */
#include <sys/stat.h>		/** for mkdir() */
#include "flags.hpp"
#ifdef MENTOC_CPP_2017
#include <filesystem>
#else
#include <dirent.h>
#endif
	
#ifndef __MENTOC_STRING_LIT__
#define __MENTOC_STRING_LIT__
static inline std::string operator "" _s(const char* s,long unsigned int i) {
	return std::string(s);
}
#endif

namespace mods::util {
	using directory_list_t = std::vector<std::string>;

	/**========================================*/
	/** affected/plr flag conversion utilities */
	/**========================================*/
	uint64_t aff2legacy(mods::flags::aff f);
	mods::flags::aff legacy2aff(uint64_t f);
	uint64_t plr2legacy(mods::flags::plr f);
	mods::flags::plr legacy2plr(uint64_t f);
	/**========================================*/
	/**========================================*/
	/**========================================*/

	directory_list_t glob(std::string_view);
	bool fuzzy_match(const std::string& _needle,const std::string& _haystack);
	std::string color_eval(std::string final_buffer);
	template <typename T>
		T arglist(const std::string& argument) {
			T arglist;
			auto flush = [&](std::string_view f) {
				arglist.emplace_back(f.data());
			};
			bool escaped = false;
			std::string current = "";
			unsigned string_length = argument.length();

			for(auto c : argument) {
				--string_length;

				if(c == '"') {
					if(escaped) {
						flush(current);
						escaped = false;
						current = "";
					} else {
						escaped = true;
					}

					continue;
				}

				if(c == ' ' && !escaped) {
					if(current.length()) {
						flush(current);
						current = "";
					}

					continue;
				}

				if(c == ' ' && escaped) {
					current += c;
					continue;
				}

				current += c;

				if(string_length ==0 && current.length()) {
					flush(current);
					break;
				}
			}

			return arglist;
		}
	template <typename T>
		T stoi(const std::string& i){
			T m;
			std::stringstream ss;
			ss.str(i);
			ss >> m;
			return m;
		}
	template <typename T>
		std::optional<T> stoi_optional(std::string_view str){
			T i;
			std::stringstream ss;
			ss << str;
			ss >> i;
			if(ss.fail()){
				return std::nullopt;
			}else{
				return i;
			}
		}
	std::optional<int> stoi(std::string_view str);

	template<int max_char,typename Container>
		inline std::optional<Container> subcmd_args(std::string argument,const char* subcmd) {
			std::array<char,max_char> command;
			std::fill(command.begin(),command.end(),0);
			one_argument(const_cast<char*>(argument.c_str()),&command[0],max_char);

			if(std::string(&command[0]).compare(subcmd) == 0) {
				std::string arg = argument.data();
				return mods::util::arglist<Container>(arg);
			}

			return {};
		}
	std::string itoa(int number);

	template<typename T>
		inline void linked_list_remove(T item, T head) {
			if((item) == (head)) {
				head = (item)->next;
			} else {
				T temp = head;

				while(temp && (temp->next != (item))) {
					temp = temp->next;
				}

				if(temp) {
					temp->next = (item)->next;
				}
			}
		}

	template <typename ContainerType,typename FindType>
		bool exists(const ContainerType * container_ptr,
				const FindType & find_me,
				typename ContainerType::const_iterator & found_at ){
			return (
					found_at =
					std::find(
						container_ptr->begin(),
						container_ptr->end(),
						find_me
						)
					)!= container_ptr->end();
		}
	extern std::function<void(const std::string&,std::string&)> sanitize;
	bool dir_exists(const char*);

};

namespace mods::util::err {
	std::string get_string(int);
};

#endif
