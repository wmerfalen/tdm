#include "player-utils.hpp"
#include "player.hpp"

namespace mods::player_utils {
	std::pair<uint16_t,uint16_t> get_thermal_vision_range(player_ptr_t& player) {
		const auto& rct = player->get_ranged_combat_totals();
		if(rct->vision & mods::vision_t::HAS_THERMALS) {
			if(player->ghost()) {
				return {0,3};
			}
			if(player->marine()) {
				return {0,1};
			}
		}
		return {0,0};
	}
};//end namespace mods::player_utils
