#include "pregame.hpp"
#include <regex>
#include "util.hpp"
namespace mods::pregame {
	void boot_suite(std::string_view suite){
		constexpr unsigned int arr_size = 1024;
		std::array<char,arr_size> arr;
		std::fill(arr.begin(),arr.end(),0);
		auto ptr = getcwd(&arr[0],arr_size);
		if(ptr == nullptr){
			std::cerr << "Couldn't grab PWD!\n";
			return;
		}
		std::string directory = &arr[0];
		directory += "/test-suites/";
		std::string sanitized = "";
		/**
		 * Important: sanitize only allows alphabetic chars
		 */
		mods::util::sanitize(suite.data(),sanitized);
		if(sanitized.length() == 0){
			std::cerr << "[fail] Invalid test suite name\n";
			return;
		}else{
			for(auto &fname : mods::util::glob(directory)){
				std::smatch match;
				if(std::regex_search(fname,match,
							std::regex(
								sanitized + "-([a-z]{1})")
							)
				){
					auto command = directory + sanitized + 
							std::string("-") + 
							std::string(
								match[1].first,
								match[1].second
							) + 
							" &"
					;	
					system(command.c_str()); 
				}
			}
		}
	}
};//End namespace
