#include "mini-games.hpp"
#include "help.hpp"
#include "super-users.hpp"
#include "rand.hpp"
#include "orm/mini-game.hpp"

namespace mods::mini_games {
	static std::map<room_vnum,std::deque<line_up>> devices;
	line_up& game(room_vnum vnum,int id) {
		return devices[vnum][id];
	}
	void init() {
		mods::orm::mini_game db;
		db.load_all();
	}
	void load(mini_game_payload_t g) {
#ifdef __MENTOC_SHOW_MINI_GAME_LOAD_DEBUG__
		std::cerr << "[mods::mini_games::load]: " << g.primary_key << ", "
		          << "type:" << g.type << ", "
		          << "vnum:" << g.vnum << ", "
		          << "name:'" << g.name << "', "
		          << "action:'" << g.action << "', "
		          << "room:" << g.game_room_vnum  << ", "
		          << "order:" << g.order << "\n";
#endif
	}
	bool device_exists(room_vnum vnum,int device_id) {
		auto found = devices.find(vnum);
		if(found == devices.end()) {
			return false;
		}
		return devices[vnum].size() > device_id;
	}
	void list_devices(player_ptr_t& player) {
		if(devices.find(player->vnum()) == devices.end()) {
			player->sendln("No devices in this room");
			return;
		}
		player->sendln("Listing devices...");
		for(auto& hackable : devices[player->vnum()]) {
			player->sendln(CAT("device #", std::to_string(hackable.device_id()), "\r\n"));
		}
		player->sendln("Done listing devices.");
	}
	int random_odd_number(int from,int to) {
		int r = rand_number(from,to);
		if((r % 2)) {
			return r;
		}
		return random_odd_number(from,to);
	}
	int random_even_number(int from,int to) {
		int r = rand_number(from,to);
		if(!(r % 2)) {
			return r;
		}
		return random_even_number(from,to);
	}
};

SUPERCMD(do_install_minigame) {
	DO_HELP("install_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
SUPERCMD(do_uninstall_minigame) {
	DO_HELP("uninstall_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
SUPERCMD(do_list_minigame) {
	DO_HELP("list_minigame");
	ADMIN_REJECT();
	ADMIN_DONE();
}
