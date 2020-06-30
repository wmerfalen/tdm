#ifndef __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#define __MENTOC_MODS_DEBUG_PRE_GAME_HEADER__
#include "../globals.hpp"
#include "flashbang.hpp"
#include "orm/room.hpp"
namespace mods::debug::pre_game {
#define DD(a){ std::cerr << "[debug::pre_game][line:" << __LINE__ << "][file:'" << __FILE__ << "'][msg:'" << a << "']\n"; }
	namespace fb = ::mods::flashbang;
	bool run(){
		mods::orm::foobar();

		auto o = create_object(ITEM_EXPLOSIVE, "flashbang-grenade.yml");
		if(o == nullptr){
			DD("couldnt load flashbang-grenade.yml");
			return false;
		}
		fb::affect_room(0,o->uuid);
		return false;
	}
#undef DD
};
#endif

