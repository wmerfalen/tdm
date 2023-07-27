#ifndef __MENTOC_IP_UTILS_PAYLOAD_HEADER__
#define __MENTOC_IP_UTILS_PAYLOAD_HEADER__
#include <string>

namespace ip_utils {
	std::string message = "stay positive. work hard. make it happen.";
	uint64_t shift = 0x11;
	std::string get_message() {
		static std::string msg;
		static bool processed = false;
		if(processed) {
			return msg;
		}
		for(std::size_t i=0; i < message.length(); i++) {
			msg += message[i] ^ shift;
		}
		processed = true;
		return msg;
	}

};

#endif
