#include "pfind.hpp"

namespace mods::pfind {
	mods::globals::player_ptr_t by_name(const char* player_name){
		auto it = mods::globals::player_name_map.find(player_name);
		if(it != mods::globals::player_name_map.end()){
			return it->second;
		}
		return mods::globals::player_nobody;
	}
};
