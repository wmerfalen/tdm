#include "super-user-fiddler.hpp"
#include "pyrexia.hpp"

namespace mods::classes {
	void super_user_fiddler::set_fire_nades(player_ptr_t& player,int charges){
		player->pyrexia()->m_fire_nade_charges = charges;
	}
	void super_user_fiddler::set_fire_nade_level(player_ptr_t& player,int level){
		player->pyrexia()->m_fire_nade_level = level;
	}
};
