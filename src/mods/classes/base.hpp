#ifndef __MENTOC_MODS_CLASSES_BASE_HEADER__
#define  __MENTOC_MODS_CLASSES_BASE_HEADER__

#include "types.hpp"
#include "../weapon.hpp"
#include "../bugs-fixtures.hpp"

namespace mods::classes {
	struct base {
		virtual types kind() {
			return types::UNDEFINED;
		}
		void report(std::vector<std::string> msgs) {
			mods::bugs::fixtures(mods::classes::to_string(this->kind()), IMPLODE(msgs,""));
		}
		void report(std::string_view msg) {
			mods::bugs::fixtures(mods::classes::to_string(this->kind()), msg.data());
		}
	};
	void unblock_event(uint32_t unblock_event,uuid_t player);
};

#endif
