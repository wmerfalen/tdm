#include "bookmarks.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>

namespace mods::builder::bookmarks {
	void add_room(player_ptr_t& player,const room_rnum& room_id);
	std::vector<room_rnum>& get_rooms(player_ptr_t& player);
	void teleport_to(player_ptr_t& player,const room_rnum& room);

	ACMD(do_bookmark) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("bookmark");
		auto vec_args = PARSE_ARGS();
		if(!player->builder_data) {
			mods::builder::initialize_builder(player);
		}
		player->sendln(CAT("size: '",vec_args.size(),"'"));
		if(vec_args.size() < 2) {
			player->errorln("Usage: bookmark nickname <room-id|this>");
			return;
		}
		int i = player->room();
		if(vec_args[1].compare("this") != 0) {
			i = mods::util::stoi(vec_args[1]).value_or(-1);
			if(i < 0) {
				player->errorln(CAT("Invalid room id: ",vec_args[1],". Skipping...").c_str());
				return;
			}
		}
		player->builder_data->bookmarks[vec_args[0]] = i;
		ADMIN_DONE();
	}
	ACMD(do_goto) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("goto");
		auto vec_args = PARSE_ARGS();
		if(!player->builder_data) {
			mods::builder::initialize_builder(player);
		}
		if(vec_args.size() < 1) {
			player->errorln("You must specify a nickname");
			return;
		}
		auto room_id = player->builder_data->bookmarks[vec_args[0]];
		if(room_id >= world.size()) {
			player->errorln(CAT("Room id out of bounds. You asked for: ",room_id, " but world.size is:", world.size()));
			return;
		}
		if(room_id < 0) {
			player->errorln("Room id is a negative number. Ignoring...");
			return;
		}

		player->sendln("char_from_room...");
		char_from_room(player->cd());
		player->sendln("char_to_room...");
		char_to_room(player->cd(),room_id);
		ADMIN_DONE();
	}

	void init() {
		mods::interpreter::add_command("bookmark", POS_RESTING, do_bookmark, LVL_BUILDER,0);
		mods::interpreter::add_command("goto", POS_RESTING, do_goto, LVL_BUILDER,0);
	}
};
