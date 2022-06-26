#ifndef __MENTOC_MODS_RADIO_HEADER__
#define __MENTOC_MODS_RADIO_HEADER__

#include "response-team/radio.hpp"

namespace mods::radio {
	void init();
	void transmit_globally(std::string_view msg);
};


#endif
