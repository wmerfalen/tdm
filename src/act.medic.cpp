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

	if(player->get_class() != player_class_t::CLASS_MEDIC){
		*player << "You are not a medic\r\n";
		return;
	}

	auto vec_args = mods::util::arglist<std::vector<std::string>>(std::string(argument));

	bool healed = false;
	unsigned room = static_cast<unsigned>(player->room());
	for(auto& v : vec_args) {
		if(mods::globals::room_list.size() > room){
			for(auto & plr : mods::globals::get_room_list(room)){
				if(mods::util::fuzzy_match(v,plr->name().c_str())) {
					mods::classes::medic::heal_player(player,plr);
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

ACMD(do_revive) {
	auto vec_args = PARSE_ARGS();

	unsigned room = static_cast<unsigned>(player->room());
	for(auto& v : vec_args) {
		if(mods::globals::room_list.size() > room){
			for(auto & plr : mods::globals::get_room_list(room)){
				if(mods::util::fuzzy_match(v,plr->name().c_str())) {
					player->block_for(mods::values::REVIVE_TICKS,mods::deferred::EVENT_PLAYER_REVIVE_SUCCESSFUL,plr->uuid());
					player->send("You begin reviving %s...\r\n",plr->name().c_str());
					return;
				}
			}
		}
	}
}
