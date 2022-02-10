#ifndef __MENTOC_MODS_DEMOLITIONS_HEADER__
#define __MENTOC_MODS_DEMOLITIONS_HEADER__

#include "../globals.hpp"

namespace mods::demolitions {
	void plant_claymore(player_ptr_t& player,int direction,obj_ptr_t& object);
	void uninstall_claymore(player_ptr_t& player,int direction,obj_ptr_t& object);
	void set_done_installing(uuid_t object,uuid_t owner);
	void set_done_uninstalling(uuid_t object,uuid_t owner);
	std::vector<uuid_t> claymores_in(const room_rnum& room);
};
#endif
