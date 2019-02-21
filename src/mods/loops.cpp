#include "loops.hpp"
namespace mods::loops {
	void foreach_mob(mob_function_t func){
		for(auto &mob : mob_list){
			if(!func(&mob)){ return; }
		}
	}
	void foreach_player(player_function_t func){
		for(auto player_ptr : mods::globals::player_list){
			if(!player_ptr->authenticated()){ return; }
			if(!func(player_ptr->cd())){ return; }
		}
	}
	//template <typename PointerType,typename ReturnType>
	//ReturnType foreach_player(player_function_t<PointerType,ReturnType> func,PointerType* param){
	//	for(auto player_ptr : mods::globals::player_list){
	//		if(!func(player_ptr->cd(),param)){ return ReturnType(param->lambda_return_value); }
	//	}
	//}
	//template <typename PointerType>
	//PointerType* foreach_player(mods_player_function_t<PointerType> func,PointerType* param){
	//	for(auto player_ptr : mods::globals::player_list){
	//		if(!func(player_ptr,param)){ return param; }
	//	}
	//	return nullptr;
	//}
	void foreach_all_chars(all_function_t func){
		foreach_mob(func);
		foreach_player(func);
	}
};
