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

	for(auto& v : vec_args) {
		if(v.compare("me") == 0 || v.compare("self") == 0 || v.compare(player->cd()->player.name) == 0) {
			ptr->heal_player(player);
			continue;
		}

		for(auto people = world[IN_ROOM(player->cd())].people; people; people = people->next) {
			if(mods::util::fuzzy_match(v.c_str(),people->player.name)) {
				ptr->heal_player(people->player_ptr);
				break;
			}
		}
	}
}
