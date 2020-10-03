#ifndef __MENTOC_MODS_MINI_GAMES_HEADER__
#define __MENTOC_MODS_MINI_GAMES_HEADER__

#include "../globals.hpp"
#include "orm/integral-object.hpp"
#include "mini-games/line-up.hpp"

namespace mods::mini_games {
	struct mini_game_payload_t {
		int primary_key;
		std::string type;
		int vnum;
		std::string name;
		std::string action;
		int game_room_vnum;
		int order;
	};
	void load(mini_game_payload_t p);
	void init();
	bool device_exists(room_vnum vnum,int device_id);
	void list_devices(player_ptr_t& player);
	int random_odd_number(int from,int to);
	int random_even_number(int from,int to);
	line_up& game(room_vnum vnum,int id);
};

ACMD(do_install_minigame);
ACMD(do_uninstall_minigame);
ACMD(do_list_minigame);
ACMD(do_hack);
ACMD(do_rotate_right);
ACMD(do_rotate_left);
ACMD(do_reset_hack);
ACMD(do_next_row);

/** for the wires mini game */
ACMD(do_plug_cable);

#endif
