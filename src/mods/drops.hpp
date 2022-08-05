#ifndef __MENTOC_MODS_drops_HEADER__
#define __MENTOC_MODS_drops_HEADER__
#include "../globals.hpp"

namespace mods::drops {
	/**
	 * API:
	 */
	void random_drop_to(player_ptr_t& victim,obj_ptr_t& corpse);
	const std::vector<std::string>& get_common_ammos();
	const std::vector<std::string>& get_uncommon_ammos();
	const std::vector<std::string>& get_rare_ammos();
	const std::vector<std::string>& get_legendary_ammos();
	const std::vector<std::string>& get_god_tier_ammos();

	const std::vector<std::string>& get_common_armors();
	const std::vector<std::string>& get_uncommon_armors();
	const std::vector<std::string>& get_rare_armors();
	const std::vector<std::string>& get_legendary_armors();
	const std::vector<std::string>& get_god_tier_armors();

	const std::vector<std::string>& get_common_rifles();
	const std::vector<std::string>& get_uncommon_rifles();
	const std::vector<std::string>& get_rare_rifles();
	const std::vector<std::string>& get_legendary_rifles();
	const std::vector<std::string>& get_god_tier_rifles();

	const std::vector<std::string>& get_common_explosives();
	const std::vector<std::string>& get_uncommon_explosives();
	const std::vector<std::string>& get_rare_explosives();
	const std::vector<std::string>& get_legendary_explosives();
	const std::vector<std::string>& get_god_tier_explosives();

};
#endif
