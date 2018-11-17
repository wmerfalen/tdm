#ifndef __MENTOC_MODS_LOOPS_HEADER__
#define  __MENTOC_MODS_LOOPS_HEADER__

#include <iostream>
#include <functional>
#include "../globals.hpp"

#include "../structs.h"
extern std::deque<char_data> mob_list;
namespace mods::loops {
	using mob_function_t = std::function<bool(char_data*)>;
	using player_function_t = std::function<bool(char_data*)>;
	template <typename PointerType>
	using mods_player_function_t = std::function<bool(std::shared_ptr<mods::player>,PointerType*)>;
	using all_function_t = std::function<bool(char_data*)>;
	void foreach_mob(mob_function_t);
	void foreach_player(player_function_t);
	//template <typename PointerType>
	//PointerType* foreach_player(mods_player_function_t<PointerType>,PointerType* param);
	template <typename PointerType>
	static inline PointerType* foreach_player(mods_player_function_t<PointerType> func,PointerType* param){
		for(auto player_ptr : mods::globals::player_list){
			if(!func(player_ptr,param)){ return param; }
		}
		return nullptr;
	}
	static inline void foreach_all(all_function_t func){
		for(auto mob_ptr : mob_list){
			if(!func(&mob_ptr)){ return; }
		}
		for(auto player_ptr : mods::globals::player_list){
			if(!func(player_ptr->cd())){ return; }
		}
	}
	void foreach_all_chars(all_function_t);
};

#include "util.hpp"

#endif
