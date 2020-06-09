#ifndef __MENTOC_MODS_ROOMS_HEADER__
#define __MENTOC_MODS_ROOMS_HEADER__
#include "../globals.hpp"
extern obj_ptr_t optr_by_uuid(uuid_t);

namespace mods::rooms {
	using fire_status_t = room_data::fire_status_t;
	template <typename TRoom>
	static inline bool is_dark(TRoom room) {
		if(!VALID_ROOM_RNUM(room)) {
			log("room_is_dark: Invalid room rnum %d. (0-%d)", room, top_of_world);
			return false;
		}

		if(world[room].light) {
			return false;
		}

		if(ROOM_FLAGGED(room, ROOM_DARK)) {
			return true;
		}

		if(SECT(room) == SECT_INSIDE || SECT(room) == SECT_CITY) {
			return false;
		}

		if(weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK) {
			return true;
		}

		return false;
	}
	template <>
	inline bool is_dark(room_data& room) {
		return is_dark(real_room(room.number));
	}

	template <typename TRoom>
	static inline bool is_smoked(TRoom room) {
		auto & textures = world[room].textures();
		if(textures.size() == 0){
			return false;
		}
		std::array<room_data::texture_type_t,2> smokes = {
			room_data::texture_type_t::NON_HAZARDOUS_SMOKE,
			room_data::texture_type_t::HAZARDOUS_SMOKE
		};
		return std::find_first_of(textures.begin(),textures.end(),smokes.begin(),smokes.end()) != textures.end();
	}
	template <typename TRoom>
	static inline bool is_on_fire(TRoom room) {
		auto & textures = world[room].textures();
		if(textures.size() == 0){
			return false;
		}
		return std::find(textures.begin(),textures.end(),room_data::texture_type_t::ON_FIRE) != textures.end();
	}
	template <typename TRoom>
	static inline fire_status_t get_fire_status(TRoom room) {
		return world[room].fire_status();
	}

	template <typename TRoom>
	static inline void start_fire_dissolver(TRoom room) {

	}

	template <typename TRoom>
	static inline bool can_see_through_fire(TRoom room) {
		auto fire_status = get_fire_status(room);
		return fire_status >= mods::rooms::fire_status_t::COMPLETELY_ON_FIRE &&
				 fire_status < mods::rooms::fire_status_t::OUT;
	}
	template <typename TRoom>
	static inline room_rnum real_room(TRoom room_id){
		return room_id;
	}
	template <>
	inline room_rnum real_room(room_vnum room_id){
		for(room_rnum i=0; i < world.size();++i){
			if(world[i].number == room_id){
				return i;
			}
		}
		return NOWHERE;
	}
};//end namespace
#endif
