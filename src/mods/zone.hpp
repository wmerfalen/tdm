#ifndef  __MENTOC_MODS_ZONE_HEADER__
#define  __MENTOC_MODS_ZONE_HEADER__

#include <iostream>

#define __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#ifdef __MENTOC_MODS_AFFECTS_SHOW_DEBUG_OUTPUT__
#define mzone_debug(a) std::cerr << "[mods::zone]" << __FILE__ << "|" << __LINE__ << "->" << a << "\n";
#else
#define mzone_debug(a) /**/
#endif
#include <string>
#include "../structs.h"
namespace mods::zone {
	struct replenish_command {
		room_vnum room;
		std::string type;
	};
	void build_weapon_locker(room_vnum room);
	void build_armor_locker(room_vnum room);
	void register_replenish(room_vnum room,std::string type);
	void zone_update();
	void run_replenish();
	void new_room(room_data*);
};

#endif
