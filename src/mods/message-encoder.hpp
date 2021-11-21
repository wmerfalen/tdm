#ifndef __MENTOC_MODS_MESSAGE_encoder_HEADER__
#define __MENTOC_MODS_MESSAGE_encoder_HEADER__

#include "../cppzmq/zmq.hpp"
#include <vector>
#include <string>

namespace mods::message_encoder {
	template <typename T>
	static inline std::string encode(const T& list) {
		std::string compiled;
		for(const auto& msg : list) {
			compiled += CAT("{",msg.length(),"}",msg);
		}
		return compiled;
	}

}; //end namespace mods::message_encoder

#endif
