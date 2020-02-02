#include "pfind.hpp"

namespace mods::pfind {
	player_ptr_t by_name(const char* player_name){
		auto it = mods::globals::player_name_map.find(player_name);
		if(it != mods::globals::player_name_map.end()){
			return it->second;
		}
		return mods::globals::player_nobody;
	}
	player_ptr_t by_uuid(uuid_t player_uuid){
		auto it = mods::globals::player_map.find(player_uuid);
		if(it != mods::globals::player_map.end()){
			return it->second;
		}
		return mods::globals::player_nobody;
	}
};
