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
		if(vec_args.size() < 3) {
			player->errorln("Usage: bookmark nickname room-id");
			return;
		}
		auto i = mods::util::stoi(vec_args[2]).value_or(-1);
		if(i < 0) {
			player->errorln(CAT("Invalid room id: ",vec_args[2],". Skipping...").c_str());
			return;
		}
		player->builder_data->bookmarks[vec_args[1]] = i;
		ADMIN_DONE();
	}
	ACMD(do_goto) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("goto");
		auto vec_args = PARSE_ARGS();
		if(!player->builder_data) {
			mods::builder::initialize_builder(player);
		}
		auto room_id = player->builder_data->bookmarks[vec_args[1]];
		if(room_id >= world.size()) {
			player->errorln(CAT("Room id out of bounds. You asked for: ",room_id, " but world.size is:", world.size()));
			return;
		}
		char_from_room(player->cd());
		char_to_room(player->cd(),room_id);
	}

	void init() {
		mods::interpreter::add_command("bookmark", POS_RESTING, do_bookmark, LVL_BUILDER,0);
		mods::interpreter::add_command("goto", POS_RESTING, do_goto, LVL_BUILDER,0);
	}
};
