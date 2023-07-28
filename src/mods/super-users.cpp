#include "super-users.hpp"
#include "help.hpp"
#include "screen-searcher.hpp"
#include <set>
#include "interpreter.hpp"
#include "movement.hpp"
#include "pfind.hpp"
#include "../handler.h"
#include "world-configuration.hpp"
#include "jx.hpp"

extern void exit_with(int);
extern int destroy_player(player_ptr_t&& player);

namespace mods::super_users {
	std::vector<std::string> load();
	SUPERCMD(do_vnumtele) {
		ADMIN_REJECT();
		if(argshave()->int_at(0)->passed() == false) {
			player->sendln("Usage: vnumtele <room-vnum>");
			return;
		}
		auto vnum = intat(0);
		auto room_id = real_room(vnum);
		if(room_id == NOWHERE) {
			player->sendln("That room vnum goes to nowhere.");
			return;
		}
		char_from_room(player->cd());
		char_to_room(player->cd(),room_id);
		command_interpreter(player,"l");
	}

	SUPERCMD(do_admin_where) {
		ADMIN_REJECT();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln("Usage: admin:where <user>");
			return;
		}
		auto it = mods::globals::player_name_map.find(argat(0));
		if(it != mods::globals::player_name_map.end()) {
			auto ptr = mods::globals::player_name_map[argat(0)];
			if(ptr->room() < world.size()) {
				auto& room = world[ptr->room()];
				player->sendln(CAT("vnum:",room.number,":",room.name.c_str()));
				player->sendln(room.overhead(lense_type_t::NORMAL_SIGHT).data());
				return;
			} else {
				player->sendln("Unknown room(0)");
				return;
			}
		}
		player->sendln("Couldn't find player in player_name_map");
	}
	SUPERCMD(do_admin_tele) {
		ADMIN_REJECT();
		if(argshave()->size_gt(0)->passed() == false) {
			player->sendln("Usage: admin:tele <user>");
			return;
		}
		auto it = mods::globals::player_name_map.find(argat(0));
		if(it != mods::globals::player_name_map.end()) {
			auto ptr = mods::globals::player_name_map[argat(0)];
			if(ptr->room() < world.size()) {
				char_from_room(player->cd());
				char_to_room(player->cd(),ptr->room());
				return;
			} else {
				player->sendln("Unknown room(0)");
				return;
			}
		}
		player->sendln("Couldn't find player in player_name_map");
	}
	/**
	 * The only argument to this should be the page that you
	 * are interested in.
	 */
	SUPERCMD(do_rnumlist) {

		constexpr unsigned int max_char = 11;
		std::array<char,max_char> room_id;
		std::fill(room_id.begin(),room_id.end(),0);
		one_argument(argument,&room_id[0],max_char);
		auto r = mods::util::stoi(&room_id[0]);
		if(r.value_or(-1) == -1) {
			*player << "{red}Invalid room number{/red}\r\n";
			return;
		}

		mods::jx::compositor jx;
		unsigned room_ctr = 0;
		if(!player->is_executing_js()) {
			player->pager_start();
		} else {
			jx.array_start("rooms");
		}
		for(auto& room : world) {
			++room_ctr;
			if(player->is_executing_js()) {
				jx.object_start("")
				.push("index",room_ctr)
				.push("number",room.number)
				.push("zone",room.zone)
				.push("name",room.name)
				.object_end();
				if(room_ctr >= mods::builder::RNUMLIST_MAX_PER_CALL) {
					break;
				}
			} else {
				*player << "{gld}[" << std::to_string(room_ctr) << "]{/gld} :->{red} [" <<
				    room.name.c_str() << "]{/red}";
			}
		}
		if(player->is_executing_js()) {
			jx.array_end();
			*player << jx.get();
		} else {
			player->pager_end();
			player->page(0);
		}
	}
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
		player->visibility() = 0;
	}
	void set_who_line(std::string_view player_name,std::string_view line) {
		custom_who_lines[player_name.data()] = line;
	}
	void clear_who_line(std::string_view player_name) {
		custom_who_lines.erase(player_name.data());
	}
	std::vector<std::string>& always_super_users() {
		static std::vector<std::string> admins = {"xisop","mortis","grifter"};
		return admins;
	}
	bool player_name_is(std::string_view name) {
		if(!super_users_initialized) {
			for(auto user_name : load()) {
				super_users.emplace(user_name);
			}
			super_users_initialized = 1;
		}
		for(auto user_name : always_super_users()) {
			if(user_name.compare(name.data()) == 0) {
				return true;
			}
		}
		return std::find(super_users.begin(),super_users.end(),name) != super_users.end();
	}
	bool is(const uuid_t& player_uuid) {
		auto p = ptr_by_uuid(player_uuid);
		if(!p) {
			return false;
		}
		return player_is(p);
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
	void save() {
		std::string s;
		for(const auto& user : super_users) {
			s += user;
			s += "|";
		}
		mods::db::lmdb_put("SUPER_USERS_LIST",s);
	}
	std::vector<std::string> load() {
		std::vector<std::string> users;
		std::string list = mods::db::lmdb_get("SUPER_USERS_LIST");
		std::string current;
		for(const auto& ch : list) {
			if(ch == '|' && current.length()) {
				users.emplace_back(current);
				current.clear();
				continue;
			}
			current += ch;
		}
		if(current.length()) {
			users.emplace_back(current);
		}
		return users;
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
		mods::super_users::super_users_initialized = false;
		mods::super_users::save();
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
	player->sendln(mods::super_users::get_list().c_str());
}
SUPERCMD(do_remove_super_user) {
	DO_HELP("remove_super_user");
	ADMIN_REJECT();
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() > 0) {
		for(auto name : vec_args) {
			mods::super_users::remove(name);
		}
		mods::super_users::super_users_initialized = false;
		mods::super_users::save();
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
	exit_with(0);
}
SUPERCMD(do_admin_kick) {
	ADMIN_REJECT();
	DO_HELP("admin:kick");
	auto vec_args = PARSE_ARGS();
	for(const auto& name : vec_args) {
		for(auto& pair : mods::globals::socket_map) {
			if(mods::util::is_lower_match(pair.second->name(),name)) {
				destroy_player(std::move(pair.second));
			}
		}
	}

}
SUPERCMD(do_admin_goto) {
	ADMIN_REJECT();
	DO_HELP("admin:goto");
	auto vec_args = PARSE_ARGS();
	for(const auto& name : vec_args) {
		for(auto& pair : mods::globals::socket_map) {
			if(mods::util::is_lower_match(pair.second->name(),name)) {
				char_from_room(player->cd());
				char_to_room(player->cd(),pair.second->room());
				return;
			}
		}
	}

}
namespace mods::super_users {
	void init() {
		mods::interpreter::add_command("sdm", POS_RESTING, do_shutdown_mud, LVL_BUILDER,0);
		mods::interpreter::add_command("vnumtele", POS_RESTING, do_vnumtele, LVL_BUILDER,0);
		mods::interpreter::add_command("rnumlist", POS_RESTING, do_rnumlist, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:kick", POS_RESTING, do_admin_kick, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:goto", POS_RESTING, do_admin_goto, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:where", POS_RESTING, do_admin_where, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:tele", POS_RESTING, do_admin_tele, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:add", POS_RESTING, do_add_super_user, LVL_BUILDER,0);
		mods::interpreter::add_command("admin:remove", POS_RESTING, do_remove_super_user, LVL_BUILDER,0);
	}
};
