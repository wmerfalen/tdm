#include "bookmarks.hpp"
#include "../interpreter.hpp"
#include "../util.hpp"
#include "../builder_util.hpp"
#include <map>
#include "../builder-data.hpp"

namespace mods::builder::bookmarks {
	void add_room(player_ptr_t& player,const room_rnum& room_id);
	std::vector<room_rnum>& get_rooms(player_ptr_t& player);
	void teleport_to(player_ptr_t& player,const room_rnum& room);

	SUPERCMD(do_bookmark) {
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
		world[i].nickname.assign(vec_args[0]);
		player->builder_data->bookmarks[vec_args[0]] = i;
		ADMIN_DONE();
	}
	SUPERCMD(do_goto) {
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
		for(auto i=0; i < world.size(); i++) {
			if(world[i].nickname.str().compare(vec_args[0].c_str()) == 0) {
				char_from_room(player->cd());
				char_to_room(player->cd(),i);
				return;
			}
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
	SUPERCMD(do_print_vnum) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("print_vnum");
		auto vec_args = PARSE_ARGS();
		if(!player->builder_data) {
			mods::builder::initialize_builder(player);
		}
		if(vec_args.size() < 1) {
			player->errorln("You must specify a nickname");
			return;
		}
		for(auto i=0; i < world.size(); i++) {
			if(world[i].nickname.str().compare(vec_args[0].c_str()) == 0) {
				player->sendln(std::to_string(world[i].number));
				player->set_scripted_response(std::to_string(world[i].number));
				return;
			}
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
		player->sendln(std::to_string(world[room_id].number));
		player->set_scripted_response(std::to_string(world[room_id].number));
		ADMIN_DONE();
	}

	SUPERCMD(do_bookmark_list) {
		ADMIN_REJECT();
		DO_HELP_WITH_ZERO("bookmark:list");
		auto vec_args = PARSE_ARGS();
		if(!player->builder_data) {
			mods::builder::initialize_builder(player);
		}
		for(const auto& pair : player->builder_data->bookmarks) {
			player->sendln(CAT("[",pair.first,"]:",pair.second));
		}
		ADMIN_DONE();
	}

	void init() {
		mods::interpreter::add_command("bookmark", POS_RESTING, do_bookmark, LVL_BUILDER,0);
		mods::interpreter::add_command("goto", POS_RESTING, do_goto, LVL_BUILDER,0);
		mods::interpreter::add_command("print_vnum", POS_RESTING, do_print_vnum, LVL_BUILDER,0);
		mods::interpreter::add_command("bookmark:list", POS_RESTING, do_bookmark_list, LVL_BUILDER,0);
	}
};
