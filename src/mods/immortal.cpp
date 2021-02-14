#include "immortal.hpp"
#include "quests.hpp"
#include <stdlib.h> //For itoa
#include "builder.hpp"
#include "util.hpp"
#include "jx.hpp"
#include "player.hpp"

extern std::deque<room_data> world;
namespace mods::immortal {

};
ACMD(do_rnumtele) {

	constexpr unsigned int max_char = 11;
	std::array<char,max_char> room_id;
	std::fill(room_id.begin(),room_id.end(),0);
	one_argument(argument,&room_id[0],max_char);
	auto r = mods::util::stoi(&room_id[0]);

	if(r.value_or(-1) == -1) {
		*player << "{red}Invalid room number{/red}\r\n";
		return;
	}

	char_from_room(player->cd());
	char_to_room(player->cd(),r.value());
	command_interpreter(player,"l");
}

/**
 * The only argument to this should be the page that you
 * are interested in.
 */
ACMD(do_rnumlist) {

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
