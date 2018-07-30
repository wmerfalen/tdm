#include "loops.hpp"
namespace mods::loops {
	void foreach_mob(mob_function_t func){
		for(auto &mob : mob_list){
			if(!func(&mob)){ return; }
		}
	}
	void foreach_player(player_function_t func){
		for(auto player_ptr : mods::globals::player_list){
			if(!func(player_ptr->cd())){ return; }
		}
	}
	void foreach_all_chars(all_function_t func){
		foreach_mob(func);
		foreach_player(func);
	}
};
