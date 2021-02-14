#ifndef __MENTOC_MODS_RESPONSE_TEAM_RADIO_HEADER__
#define __MENTOC_MODS_RESPONSE_TEAM_RADIO_HEADER__
#include "../../structs.h"
#include "../util.hpp"
#include "../../globals.hpp"

namespace mods::response_team::radio {
	/**
	 * give exact position
	 */
	void help(uuid_t mob_uuid);
	/**
	 * tell hq there has been an incident.
	 */
	void report_violence(uuid_t mob_uuid);
	/**
	 * Instead of giving exact position, give general position in panic
	 */
	void help_dazed(uuid_t mob_uuid);
};

#endif
