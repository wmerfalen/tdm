#ifndef __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#define  __MENTOC_MODS_CLASSES_MEDIC_HEADER__

//#include "base.hpp"
#include "../../comm.h"
#include "types.hpp"
namespace mods::classes {
	using player_ptr = player_ptr_t;
	struct medic {
		types kind() {
			return types::MEDIC;
		}

		uint16_t stim_pistol_ammo;
		uint16_t stim_pistol_ammo_max;
		player_ptr player_obj;


		/* constructors and destructors */
		medic() : stim_pistol_ammo(3),
			stim_pistol_ammo_max(3),
			player_obj(nullptr) {

		}
		medic(player_ptr& plyr_ptr) : stim_pistol_ammo(3),
			stim_pistol_ammo_max(3),
			player_obj(plyr_ptr) {
		}
		~medic() = default;
		static void heal_player(player_ptr&,player_ptr&);
	};
};

#endif
