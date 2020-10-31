#ifndef __MENTOC_MODS_INTEGRAL_OBJECTS_HEADER__
#define __MENTOC_MODS_INTEGRAL_OBJECTS_HEADER__

#include "../globals.hpp"
#include "orm/integral-object.hpp"

namespace mods::integral_objects {
	struct camera_feed_t {

	};
	void init();
	void feed_weapon_locker(room_vnum);
	void feed_armor_locker(room_vnum);
	void feed_camera_feed(room_vnum);
	void rotate_camera_feed(room_vnum);
	void new_room(room_data* room);
};
#endif
