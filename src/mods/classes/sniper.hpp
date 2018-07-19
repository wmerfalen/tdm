#ifndef __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#define  __MENTOC_MODS_CLASSES_SNIPER_HEADER__
#include "base.hpp"
#include <memory>
namespace mods::classes {
	struct sniper : base {
		types kind() {
			return types::SNIPER;
		}
		typedef std::shared_ptr<mods::player> player_ptr;
		player_ptr player_obj;

		/* constructors and destructors */
		sniper() = delete;
		sniper(player_ptr& p) : player_obj(p) {}
		~sniper() = default;
	};
};

#endif
