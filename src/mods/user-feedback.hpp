#ifndef __MENTOC_MODS_USER_FEEDBACK_HEADER__
#define __MENTOC_MODS_USER_FEEDBACK_HEADER__

#include <utility>
#include <string>

namespace mods::user {
	template <typename TStatus>
	struct feedback {
		TStatus status;
		std::string message;
		feedback(TStatus s,std::string_view m) : status(s), message(m.data()) {
		}
		feedback(const feedback& copy) {
			status = copy.status;
			message = copy.message;
		}
		~feedback() {
			message.clear();
		}
	};
};

using user_feedback = mods::user::feedback<bool>;

#endif
