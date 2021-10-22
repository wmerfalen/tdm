#ifndef __MENTOC_MODS_SKILLS_HEADER__
#define __MENTOC_MODS_SKILLS_HEADER__

#include "../globals.hpp"
#include "string.hpp"
#include <unordered_map>

namespace mods::skills {
	bool player_can(player_ptr_t& player,std::string_view);
	int update_schema_for_player(std::string player_name);
	void increment_player_combat_usage(player_ptr_t& player,std::string_view skill_name);
	void increment_player_cqc_usage(player_ptr_t& player,std::string_view skill_name);
	void increment_player_defense_usage(player_ptr_t& player,std::string_view skill_name);
};
enum skill_familiarity_t : uint8_t {
	NONE = 0,
	INITIATE = 1,
	FAMILIAR = 2,
	MASTER = 3
};
#endif
