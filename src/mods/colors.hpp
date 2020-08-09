#ifndef  __MENTOC_MODS_COLORS_HEADER__
#define  __MENTOC_MODS_COLORS_HEADER__

#include <iostream>

namespace mods::colors {
template <typename TStr>
static inline std::string reset_str(){
	return "\033[0m";
}
template <typename TStr>
static inline std::string blue_str(TStr str){
	return std::string("\033[34m") + str + "\033[0m";
}
template <typename TStr>
static inline std::string gold_str(TStr str){
	return std::string("\033[33m") + str + "\033[0m";
}
template <typename TStr>
static inline std::string green_str(TStr str){
	return std::string("\033[32m") + str + "\033[0m";
}
template <typename TStr>
static inline std::string red_str(TStr str){
	return std::string("\033[31m") + str + "\033[0m";
}
template <typename TStr>
static inline std::string white_str(TStr str){
	return std::string("\033[37m") + str + "\033[0m";
}
template <typename TStr>
static inline std::string yellow_str(TStr str){
	return std::string("\033[93m") + str + "\033[0m";
}

};

#endif
