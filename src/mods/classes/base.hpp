#ifndef __MENTOC_MODS_CLASSES_BASE_HEADER__
#define  __MENTOC_MODS_CLASSES_BASE_HEADER__

#include "types.hpp"
#include "../player.hpp"
#include <memory>
namespace mods::classes {
	struct base {
		virtual types kind() {
			return types::UNDEFINED;
		}
		typedef std::shared_ptr<mods::player> player_ptr;
		player_ptr player_obj;
		/* constructors and destructors */
		base() = default;
		virtual ~base() = default;
	};
};

#endif
