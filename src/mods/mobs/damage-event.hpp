#ifndef __MENTOC_MODS_MOBS_DAMAGE_EVENT_HEADER__
#define  __MENTOC_MODS_MOBS_DAMAGE_EVENT_HEADER__

#include <iostream>
#include <map>
#include <string>
#include "../../globals.hpp"
#include "../damage-event.hpp"

namespace mods::mobs::damage_event {
	using uuid_t = uint64_t;
	void sniped(player_ptr_t& mob,const feedback_t& feedback);
	void injured(player_ptr_t& mob,const feedback_t& feedback);
	void meleed(player_ptr_t& mob,const feedback_t& feedback);
	void disoriented(player_ptr_t& mob,const feedback_t& feedback);
	void blinded(player_ptr_t& mob,const feedback_t& feedback);
	void claymored(player_ptr_t& mob,const feedback_t& feedback);
	void shrapneled(player_ptr_t& mob,const feedback_t& feedback);
	void grounded(player_ptr_t& mob,const feedback_t& feedback);
	void received_elemental_damage(player_ptr_t& mob,const feedback_t& feedback);
};
#endif
