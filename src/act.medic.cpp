#include "globals.hpp"
#include "conf.h"
#include "mods/extern.hpp"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "mods/classes/medic.hpp"
#include "mods/util.hpp"

using sptr_medic = std::shared_ptr<mods::classes::medic>;
using class_type = mods::player::class_type;
ACMD(do_heal) {
	MENTOC_PREAMBLE();

	if(!player->has_class_capability(class_type::MEDIC)) {
		*player << "You are not a medic\r\n";
		return;
	}

	auto ptr = dynamic_cast<mods::classes::medic*>(player->get_class(class_type::MEDIC).get());
	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	bool healed = false;
	unsigned room = static_cast<unsigned>(player->room());
	for(auto& v : vec_args) {
		if(mods::globals::room_list.size() > room){
			for(auto person : mods::globals::room_list[room]){
				if(mods::util::fuzzy_match(v,person->player.name)) {
					ptr->heal_player(person->player_ptr);
					healed = true;
					break;
				}
			}
		}else{
			std::cerr << "[bounds] player->room (" << room << ") >= room_list.size " << __FILE__ << "\n";
		}
	}
	if(!healed){
		*player << "You couldn't find your target!\n";
	}
}
