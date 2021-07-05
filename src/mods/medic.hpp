#include "player.hpp"

namespace mods::medic {
	int scaled_heal(player_ptr_t& target);
	void heal_player(player_ptr_t& target);
	std::tuple<bool,std::string> set_is_suturing(player_ptr_t& player,const uint16_t& ticks);
};
