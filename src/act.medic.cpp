#include "globals.hpp"
#include "comm.h"
#include "conf.h"
#include "mods/extern.hpp"
#include "structs.h"
#include "utils.h"
#include "db.h"
#include "mods/util.hpp"
#include "mods/player.hpp"
#include "interpreter.h"

using sptr_medic = std::shared_ptr<mods::classes::medic>;
using class_type = mods::player::class_type;
int scaled_heal(player_ptr_t& target) {
	return (33 * (target->level() * 0.5));
}
void heal_player(player_ptr_t& medic,player_ptr_t& target) {
	int amount = scaled_heal(target);
	assert(amount > 0);
	target->hp() += amount;
	send_to_room(medic->room(),CAT("{grn}",medic->name()," heals ",target->name(),"{/grn}\r\n").c_str());
}
ACMD(do_heal) {

	if(!player->can("heal") && !(player->god_mode() || player->builder_mode() || player->implementor_mode())) {
		player->sendln("You are not a medic.");
		return;
	}

	auto vec_args = PARSE_ARGS();
	if(vec_args.size() < 1) {
		player->sendln("Specify who to heal.");
		return;
	}

	auto v = vec_args[0];
	if(ICMP(v,"me") || ICMP(v,"self") || ICMP(v,player->name())) {
		heal_player(player,player);
		return;
	}
	for(auto& plr : mods::globals::get_room_list(player->room())) {
		if(mods::util::fuzzy_match(v,plr->name().c_str())) {
			heal_player(player,plr);
			return;
		}
	}
	player->sendln("You couldn't find your target!");
}

ACMD(do_revive) {
	auto vec_args = PARSE_ARGS();

	unsigned room = static_cast<unsigned>(player->room());
	for(auto& v : vec_args) {
		if(mods::globals::room_list.size() > room) {
			for(auto& plr : mods::globals::get_room_list(room)) {
				if(mods::util::fuzzy_match(v,plr->name().c_str())) {
					player->block_for(mods::values::REVIVE_TICKS(),mods::deferred::EVENT_PLAYER_REVIVE_SUCCESSFUL,plr->uuid());
					player->sendln(
					    CAT(
					        "You begin reviving ",
					        plr->name().c_str(),
					        "..."
					    )
					);
					return;
				}
			}
		}
	}
}
