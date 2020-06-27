#include "injure.hpp"

namespace mods::injure {
	void injure_player(player_ptr_t& person){
		send_to_room_except(person->room(), person,"%s is injured!\r\n",person->name().c_str());
		person->position() = POS_INCAP;
		person->hp() = mods::values::INJURED_HP;
		if(IS_NPC(person->cd())){
			person->cd()->mob_specials.clear_behaviour_tree();
			return;
		}
		person->sendln("You are injured!");
	}
	bool do_injure_roll(uint8_t chance){
		return dice(1,100) <= chance;
	}
};
