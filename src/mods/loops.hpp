#ifndef __MENTOC_MODS_LOOPS_HEADER__
#define  __MENTOC_MODS_LOOPS_HEADER__

#include <iostream>
#include <functional>
#include "../globals.hpp"

#include "../structs.h"
#include "npc.hpp"
#include <memory>
extern std::deque<std::shared_ptr<mods::npc>> mob_list;
namespace mods::loops {
	using mob_function_t = std::function<bool(char_data*)>;
	using mods_npc_function_t = std::function<bool(std::shared_ptr<mods::npc>)>;
	using player_function_t = std::function<bool(player_ptr_t)>;
	template <typename PointerType>
	using mods_player_function_t = std::function<bool(player_ptr_t,PointerType*)>;
	using all_function_t = std::function<bool(player_ptr_t)>;
	void foreach_mob(mob_function_t);
	void foreach_mob(mods_npc_function_t);
	void foreach_player(player_function_t);
	namespace shptr {
		template <typename PointerType>
		using function_with_return_t = std::function<bool(player_ptr_t,PointerType*)>;

		using function_t = std::function<bool(player_ptr_t)>;
		using npc_function_t = std::function<bool(std::shared_ptr<mods::npc>)>;

		template <typename PointerType>
		static inline PointerType* foreach_player(function_with_return_t<PointerType> func,PointerType* param){
			for(auto & player_ptr : mods::globals::player_list){
				if(!player_ptr->authenticated()){
					continue;
				}
				if(!func(player_ptr,param)){ return param; }
			}
			return param;
		}
		static inline void foreach_player(function_t func){
			for(auto & player_ptr : mods::globals::player_list){
				if(!player_ptr->authenticated()){
					continue;
				}
				if(!func(player_ptr)){ return; }
			}
		}
		template <typename PointerType>
		static inline PointerType* foreach_mob(function_with_return_t<PointerType> func,PointerType* param){
			for(auto & mob_ptr : mob_list){
				if(!func(mob_ptr,param)){ return param; }
			}
			return param;
		}
		static inline void foreach_mob(npc_function_t func){
			for(auto & mob_ptr : mob_list){
				if(!func(mob_ptr)){ return; }
			}
		}
		static inline void foreach_mob_as_player(function_t func){
			for(auto & mob_ptr : mob_list){
				if(!func(mob_ptr)){ return; }
			}
		}
		template <typename PointerType>
		static inline PointerType* foreach_all(function_with_return_t<PointerType> func,PointerType* param){
			foreach_mob<PointerType>(func,param);
			foreach_player<PointerType>(func,param);
			return param;
		}
		static inline void foreach_all(function_t func){
			foreach_mob_as_player(func);
			foreach_player(func);
		}
	};
	template <typename PointerType>
	static inline PointerType* foreach_player(mods_player_function_t<PointerType> func,PointerType* param){
		for(auto player_ptr : mods::globals::player_list){
			if(!player_ptr->authenticated()){
				continue;
			}
			if(!func(player_ptr,param)){ return param; }
		}
		return nullptr;
	}
	static inline void foreach_all(all_function_t func){
		for(auto & mob_ptr : mob_list){
			if(!func(mob_ptr)){ return; }
		}
		for(auto & player_ptr : mods::globals::player_list){
			if(!player_ptr->authenticated()){ 
				continue;
			}
			if(!func(player_ptr)){ return; }
		}
	}
	void foreach_all_chars(all_function_t);
	void foreach_in_room(std::size_t,player_function_t);
};

#include "util.hpp"

#endif
