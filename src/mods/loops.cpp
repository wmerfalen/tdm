#include "loops.hpp"
namespace mods::loops {
#ifdef __MENTOC_MODS_LOOPS_SHOW_DEBUG_OUTPUT__
#define mloops_debug(a){ std::cerr << "[mods::loops]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n"; }
#else
#define mloops_debug(a) /**/
#endif
	void foreach_in_room(std::size_t room,player_function_t func){
		if(world.size() > room){
			for(auto &ch : mods::globals::get_room_list(room)){
				if(!func(ch)){
					return;
				}
			}
		}
	}
		
	void foreach_mob(mods_npc_function_t func){
		for(auto & mob : mob_list){
			mloops_debug(mob->name().c_str() << " room:"<< mob->room());
			if(!func(mob)){ return; }
		}
	}
	void foreach_mob(mob_function_t func){
		for(auto & mob : mob_list){
			mloops_debug( mob->name().c_str() << " " << mob->room());
			if(!func(mob->cd())){ return; }
		}
	}
	void foreach_player(player_function_t func){
		for(auto player_ptr : mods::globals::player_list){
			if(!player_ptr->authenticated()){
				continue;
			}
			if(!func(player_ptr)){ return; }
		}
	}
	void foreach_all_chars(all_function_t func){
		foreach_mob(func);
		foreach_player(func);
	}
};
