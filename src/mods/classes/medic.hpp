#ifndef __MENTOC_MODS_CLASSES_MEDIC_HEADER__
#define  __MENTOC_MODS_CLASSES_MEDIC_HEADER__

#include "base.hpp"
#include "../../comm.h"
namespace mods::classes {
	struct medic : base {
		typedef std::shared_ptr<mods::player> player_ptr;
		types kind() {
			return types::MEDIC;
		}

		uint16_t stim_pistol_ammo;
		uint16_t stim_pistol_ammo_max;
		player_ptr player_obj;

		void heal_player(player_ptr& target);

		/* constructors and destructors */
		medic() = delete;
		medic(player_ptr& plyr_ptr) : stim_pistol_ammo(3),
			stim_pistol_ammo_max(3),
			player_obj(plyr_ptr) {
		}
		~medic() = default;
	};
};

#endif
