#ifndef __MENTOC_MODS_RESPONSE_TEAM_RADIO_HEADER__
#define __MENTOC_MODS_RESPONSE_TEAM_RADIO_HEADER__
#include "../../structs.h"
#include "../util.hpp"
#include "../../globals.hpp"

namespace mods::response_team::radio {
	enum severity_t {
		HELP_WANTED =1,
		REPORT_VIOLENCE,
		OFFICER_IS_DAZED
	};
	/**
	 * give exact position
	 */
	void help(uuid_t mob_uuid,std::string_view message);
	/**
	 * tell hq there has been an incident.
	 */
	void report_violence(uuid_t mob_uuid,std::string_view message);
	/**
	 * Instead of giving exact position, give general position in panic
	 */
	void help_dazed(uuid_t mob_uuid,std::string_view message);
	static std::map<room_vnum,int32_t> incidents;

	void dispatch_response_team(room_rnum room);
	void register_help_message(room_rnum room,severity_t severity);
};

#endif
