#ifndef __MENTOC_MODS_FLASHBANG_HEADER__
#define __MENTOC_MODS_FLASHBANG_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"

extern std::deque<room_data> world;
namespace mods::flashbang {
#define mentoc_flashbang_debug(a) std::cerr << "[flashbang::affect][debug]: '" << a << "'\n"; 
	inline void affect_room(room_rnum room, obj_ptr_t& object){
		if(room >= world.size()){
			return;
		}
		mentoc_flashbang_debug("room_data+obj_ptr");
		for(auto & player : room_list(room)){
			player->sendln("You are blinded by a bright flash of light!");
		}
		return;
	}
	inline void affect_room(room_rnum room, uuid_t uuid){
		auto optr = optr_by_uuid(uuid);
		if(room >= world.size() || !optr){
			return;
		}
		mods::flashbang::affect_room(room,optr);
	}

#undef mentoc_flashbang_debug
};
#endif

