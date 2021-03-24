#include "super-users.hpp"
#include "help.hpp"
#include "screen-searcher.hpp"
#include <set>

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

ACMD(do_add_super_user) {
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

ACMD(do_get_super_user_list) {
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
ACMD(do_remove_super_user) {
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

ACMD(do_go_invisible) {
	ADMIN_REJECT();
	DO_HELP("go_invisible");
	mods::super_users::go_invisible(player);
	ADMIN_DONE();
}
ACMD(do_go_visible) {
	ADMIN_REJECT();
	DO_HELP("go_visible");
	mods::super_users::go_visible(player);
	ADMIN_DONE();
}
ACMD(do_set_who_line) {
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
ACMD(do_clear_who_line) {
	ADMIN_REJECT();
	DO_HELP("clear_who_line");
	auto vec_args = PARSE_ARGS();
	for(auto name : vec_args) {
		mods::super_users::clear_who_line(name);
		ADMIN_DONE();
	}
	ADMIN_FAIL();
}
