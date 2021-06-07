#include "super-users.hpp"
#include "help.hpp"
#include "screen-searcher.hpp"
#include <set>
#include "interpreter.hpp"
#include "movement.hpp"

namespace mods::super_users {
	static std::set<std::string> super_users;
	static std::set<std::string> invisible_super_users;
	static bool super_users_initialized = 0;
	static std::map<std::string,std::string> custom_who_lines;
	std::string who_line(player_ptr_t& player) {
		if(custom_who_lines.find(player->name()) != custom_who_lines.end()) {
			return custom_who_lines[player->name()];
		}
		return player->name();
	}
	void upkeep(player_ptr_t& player) {
		player->move() = 65000;
		player->max_move() = 65000;
		player->move() = 65000;
		player->hp() = 65000;
		player->max_hp() = 65000;
	}
	void set_who_line(std::string_view player_name,std::string_view line) {
		custom_who_lines[player_name.data()] = line;
	}
	void clear_who_line(std::string_view player_name) {
		custom_who_lines.erase(player_name.data());
	}
	bool player_name_is(std::string_view name) {
		if(!super_users_initialized) {
			for(auto user_name : EXPLODE(SUPER_USERS_LIST(),'|')) {
				super_users.emplace(user_name);
			}
			super_users_initialized = 1;
		}
		return std::find(super_users.begin(),super_users.end(),name) != super_users.end();
	}
	bool player_is(player_ptr_t& player) {
		return player_name_is(player->name());
	}
	bool add(std::string_view name) {
		super_users.emplace(name);
		return true;
	}
	bool remove(std::string_view name) {
		auto it = std::find(super_users.begin(),super_users.end(),name.data());
		if(it != super_users.end()) {
			super_users.erase(it);
			return true;
		}
		return false;
	}
	void go_invisible(std::string_view name) {
		invisible_super_users.emplace(name);
	}
	std::string get_list() {
		std::string r = "";
		for(auto user: super_users) {
			r += CAT("{grn}'", user,"'{/grn}\r\n");
		}
		return std::move(r);
	}
	bool invisible(player_ptr_t& player) {
		if(player_name_is(player->name()) && std::find(invisible_super_users.begin(),invisible_super_users.end(),player->name()) != invisible_super_users.end()) {
			return true;
		}
		return false;
	}
	void go_invisible(player_ptr_t& player) {
		invisible_super_users.emplace(player->name());
	}
	void go_visible(player_ptr_t& player) {
		invisible_super_users.extract(player->name());
	}
};

SUPERCMD(do_add_super_user) {
	DO_HELP("add_super_user");
	ADMIN_REJECT();

	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 0) {
		for(auto name : vec_args) {
			mods::super_users::add(name);
		}
		ADMIN_DONE();
		return;
	}
	ADMIN_FAIL();
}

SUPERCMD(do_get_super_user_list) {
	DO_HELP("get_super_user_list");
	ADMIN_REJECT();
	auto vec_args = PARSE_ARGS();
	if(vec_args.size()) {
		std::vector<std::string> screen;
		for(auto& user : mods::super_users::super_users) {
			screen.emplace_back(user);
		}
		mods::search_screen(player, screen, vec_args, 64);
		return;
	}
	player->send(mods::super_users::get_list().c_str());
}
SUPERCMD(do_remove_super_user) {
	DO_HELP("remove_super_user");
	ADMIN_REJECT();
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 0) {
		for(auto name : vec_args) {
			mods::super_users::remove(name);
		}
		ADMIN_DONE();
		return;
	}
	ADMIN_FAIL();
}

SUPERCMD(do_go_invisible) {
	ADMIN_REJECT();
	DO_HELP("go_invisible");
	mods::super_users::go_invisible(player);
	ADMIN_DONE();
}
SUPERCMD(do_go_visible) {
	ADMIN_REJECT();
	DO_HELP("go_visible");
	mods::super_users::go_visible(player);
	ADMIN_DONE();
}
SUPERCMD(do_set_who_line) {
	ADMIN_REJECT();
	DO_HELP("set_who_line");
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 1) {
		mods::super_users::set_who_line(vec_args[0],vec_args[1]);
		ADMIN_DONE();
		return;
	}
	ADMIN_FAIL();
}
SUPERCMD(do_clear_who_line) {
	ADMIN_REJECT();
	DO_HELP("clear_who_line");
	auto vec_args = PARSE_ARGS();
	for(auto name : vec_args) {
		mods::super_users::clear_who_line(name);
		ADMIN_DONE();
	}
	ADMIN_FAIL();
}
SUPERCMD(do_shutdown_mud) {
	ADMIN_REJECT();
	exit(0);
}
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
namespace mods::super_users {
	void init() {
		mods::interpreter::add_command("sdm", POS_RESTING, do_shutdown_mud, LVL_BUILDER,0);
		ADD_BUILDER_COMMAND("force_out",do_force_out);
		ADD_BUILDER_COMMAND("banish",do_banish);
		ADD_BUILDER_COMMAND("set_banish_room",do_set_banish_room);
	}
};
