#ifndef  __MENTOC_MODS_COLORS_HEADER__
#define  __MENTOC_MODS_COLORS_HEADER__

#include <iostream>

namespace mods::colors {
	static inline std::string reset_str() {
		return "\033[0m";
	}
	template <typename TStr>
	static inline std::string blue_str(TStr str) {
		return std::string("\033[34m") + str + "\033[0m";
	}
	template <typename TStr>
	static inline std::string gold_str(TStr str) {
		return std::string("\033[33m") + str + "\033[0m";
	}
	template <typename TStr>
	static inline std::string green_str(TStr str) {
		return std::string("\033[32m") + str + "\033[0m";
	}
	template <typename TStr>
	static inline std::string red_str(TStr str) {
		return std::string("\033[31m") + str + "\033[0m";
	}
	template <typename TStr>
	static inline std::string white_str(TStr str) {
		return std::string("\033[37m") + str + "\033[0m";
	}
	template <typename TStr>
	static inline std::string yellow_str(TStr str) {
		return std::string("\033[93m") + str + "\033[0m";
	}

	template <typename TStr>
	static inline TStr strip_color(TStr str) {
		std::string acc;
		bool color_code = false;
		for(const auto& ch : str) {
			if(ch == '{') {
				color_code = true;
				continue;
			}
			if(color_code && ch != '}') {
				continue;
			}
			if(color_code && ch == '}') {
				color_code = false;
				continue;
			}
			acc += ch;
		}
		return acc;
	}

};

#endif
