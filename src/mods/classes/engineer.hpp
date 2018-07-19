#ifndef __MENTOC_MODS_CLASSES_ENGINEER_HEADER__
#define  __MENTOC_MODS_CLASSES_ENGINEER_HEADER__

#include "base.hpp"

namespace mods::classes {
	struct engineer : base {
		types kind() {
			return types::ENGINEER;
		}
		typedef std::shared_ptr<mods::player> player_ptr;
		player_ptr player_obj;
		/* constructors and destructors */
		engineer() = delete;
		engineer(player_ptr& p) : player_obj(p) {
		}
		~engineer() = default;
	};
};

#endif
