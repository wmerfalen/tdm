#ifndef __MENTOC_MODS_CLASSES_MARINE_HEADER__
#define  __MENTOC_MODS_CLASSES_MARINE_HEADER__

#include "base.hpp"

namespace mods::classes {
	struct marine : base {
		types kind() {
			return types::MARINE;
		}
		typedef player_ptr_t player_ptr;
		player_ptr player_obj;
		/* constructors and destructors */
		marine() = delete;
		marine(player_ptr& p) : player_obj(p) {}
		~marine() = default;
	};
};

#endif
