#ifndef __MENTOC_MODS_FLASHBANG_HEADER__
#define __MENTOC_MODS_FLASHBANG_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"

extern std::vector<room_data> world;
namespace mods::flashbang {
#define DD(a) std::cerr << "[flashbang::affect][debug]: '" << a << "'\n"; 
	/**
	 * affect_room does not have to dispose of the object
	 */
	inline void affect_room(room_data& room,obj_ptr_t& object){

	}
	template <
		class TRoom,   class = typename std::enable_if<std::is_arithmetic<TRoom>::value>::type,
		class TObject, class = typename std::enable_if<std::is_arithmetic<TObject>::value>::type
	 >
	inline void affect_room(TRoom room, TObject uuid){
		auto optr = optr_by_uuid(uuid);
		if(room >= world.size() || !optr){
			return;
		}
		mods::flashbang::affect_room(world[room],optr);
	}

	/**
	 * TRoom = room_rnum
	 * TObject = obj_ptr_t
	 */
	template <
		class TRoom,   class = typename std::enable_if<std::is_arithmetic<TRoom>::value>::type,
		class TObject, class = typename std::enable_if<std::is_class<TObject>::value>::type
	 >
	inline void affect_room(TRoom room, TObject& object){
		if(room >= world.size()){
			return;
		}
		mods::flashbang::affect_room(world[room],object);
	}
#undef DD
};
#endif

