#ifndef __MENTOC_MODS_CLASSES_BASE_HEADER__
#define  __MENTOC_MODS_CLASSES_BASE_HEADER__

#include "types.hpp"
#include <memory>
namespace mods {
	struct player;
};
using player_ptr_t = std::shared_ptr<mods::player>;
namespace mods::classes {
	struct base {
		virtual types kind() {
			return types::UNDEFINED;
		}
		player_ptr_t player_obj;
		/* constructors and destructors */
		base() = default;
		virtual ~base() = default;
	};
};
#include "../player.hpp"

#endif
