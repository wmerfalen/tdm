#include "medic.hpp"
#include "../player.hpp"

namespace mods::medic {
	int scaled_heal(player_ptr_t& target) {
		return (33 * (target->level() * 0.5));
	}
	void heal_player(player_ptr_t& target) {
		target->hp() += target->max_hp() * 0.15;
	}
	std::tuple<bool,std::string> set_is_suturing(player_ptr_t& player,const uint16_t& ticks) {
		if(player->position() != POS_RESTING) {
			return {0,"You must be resting in order to do this!"};
		}
		if(player->is_blocked()) {
			return {0,"You are currently doing something that is taking all your concentration!"};
		}
		player->block_for(ticks, mods::deferred::EVENT_PLAYER_UNBLOCK_HEALING,0);
		return {1,"You open up your medkit and start to apply healing to your wounds..."};
	}
};
