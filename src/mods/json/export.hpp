#ifndef __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#define __MENTOC_MODS_JSON_NPC_EXPORT_HEADER__
#include "../../structs.h"
#include "../../globals.hpp"
#include "../duktape/src/duktape.h"
#include <sys/stat.h>
#include <string_view>
#include <array>

namespace mods::json::npc_export {
	std::forward_list<std::string> export_all_table(std::string_view table);
	std::forward_list<std::string> export_all_mobs();

};
#endif
