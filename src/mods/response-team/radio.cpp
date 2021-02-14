#include "../../structs.h"
#include "../builder_util.hpp"
#include "../../globals.hpp"
#include "../npc.hpp"
#include "../mobs/extended-types.hpp"

namespace mods::response_team::radio {
	void help(uuid_t mob_uuid) {
		std::cerr << "[stub] radio " << __FILE__ << ":" << __LINE__ << "\n";
	}
	void report_violence(uuid_t mob_uuid) {
		std::cerr << "[stub] radio " << __FILE__ << ":" << __LINE__ << "\n";
	}
	void help_dazed(uuid_t mob_uuid) {
		std::cerr << "[stub] radio dazed " << __FILE__ << ":" << __LINE__ << "\n";
	}

};//end namespace
