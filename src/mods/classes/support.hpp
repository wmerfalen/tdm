#ifndef __MENTOC_MODS_CLASSES_SUPPORT_HEADER__
#define  __MENTOC_MODS_CLASSES_SUPPORT_HEADER__

#include "base.hpp"

namespace mods::classes {
	struct support : base {
		types kind() {
			return types::SUPPORT;
		}
		typedef player_ptr_t player_ptr;
		player_ptr player_obj;
		/* constructors and destructors */
		support() = delete;
		support(player_ptr& p) : player_obj(p) {}
		~support() = default;
	};
};

#endif
