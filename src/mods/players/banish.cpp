#include "banish.hpp"
#include "../replenish.hpp"
#include "../movement.hpp"
#include "../interpreter.hpp"

#define __MENTOC_SHOW_MODS_PLAYERS_banish_DEBUG_OUTPUT__
#ifdef __MENTOC_SHOW_MODS_PLAYERS_banish_DEBUG_OUTPUT__
#define m_debug(a) std::cerr << "[mods::players::banish][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#define m_crit(a) std::cerr << red_str("[CRITICAL]") << "mods::players::banish][file:" << __FILE__ << "][line:" << __LINE__ << "]->" << a << "\n";
#else
#define m_debug(a)
#define m_crit(a)
#endif


namespace mods::players::banish {
	SUPERCMD(do_force_out) {
		if(argshave()->size_gt(0)->first_is_any({"n","e","s","w","u","d"})->passed()) {
			player->sendln(CAT("Forcing this room to the ",argat(0)));
			auto dir_opt = world[player->room()].dir_option[NORTH];
			direction_t d = NORTH;
			switch(argat(0)[0]) {
				case 'n':
					d = NORTH;
					break;
				case 'e':
					d = EAST;
					break;
				case 's':
					d = SOUTH;
					break;
				case 'w':
					d = WEST;
					break;
				case 'd':
					d = DOWN;
					break;
				case 'u':
					d = UP;
					break;
				default:
					d = NORTH;
					break;
			}
			dir_opt = world[player->room()].dir_option[d];
			if(!dir_opt) {
				player->errorln("Invalid direction");
				return;
			}
			auto cnt = mods::movement::force_room_to(player->room(),dir_opt->to_room);
			player->sendln(CAT("Forced ",cnt," people out"));
			return;
		}
		player->errorln("usage: force_out <n|e|s|w|d|u>");
	}
	static room_rnum banish_room;
	SUPERCMD(do_set_banish_room) {
		if(argshave()->size_gt(0)->first_is_any({"this","here"})->passed()) {
			banish_room = player->room();
			player->sendln("Set banish room to this room");
			return;
		}
		if(argshave()->size_gt(0)->int_at(0)->passed()) {
			banish_room = real_room(intat(0));
			player->sendln(CAT("Set banish room to vnum: ",argat(0)," which is real_room id:",banish_room));
			return;
		}
		player->sendln(CAT("No arguments passed in, so here is the current banish room value (room_rnum):",banish_room));
		if(banish_room < world.size()) {
			player->sendln(CAT("Room vnum:", world[banish_room].number));
		} else {
			player->sendln(CAT("Banish room is out of bounds!"));
		}
	}
	SUPERCMD(do_banish) {
		if(argshave()->size_gt(1)->first_is("except")->passed()) {
			auto keep = args()->gather_uuids_starting_at(1);
			keep.emplace_back(player->uuid());
			auto cnt = mods::movement::banish_except(player->room(),banish_room,keep);
			player->sendln(CAT("Forced ",cnt," people out"));
			return;
		}
		if(argshave()->first_is("all")->passed()) {
			std::vector<uuid_t> keep = {player->uuid()};
			auto cnt = mods::movement::banish_except(player->room(),banish_room, keep);
			player->sendln(CAT("Forced ",cnt," people out"));
			return;
		}
		player->sendln("usage: banish <except> <uuid>...[uuid-N]");
		player->sendln("usage: banish <all> # will banish everyone besides you");
	}
	void init() {
		ADD_BUILDER_COMMAND("admin:force_out",do_force_out);
		ADD_BUILDER_COMMAND("admin:banish",do_banish);
		ADD_BUILDER_COMMAND("admin:set_banish_room",do_set_banish_room);
	}
};
