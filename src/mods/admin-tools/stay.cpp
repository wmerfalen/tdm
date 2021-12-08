#include "stay.hpp"

#include "../db.hpp"
#include "../pq.hpp"
#include "../sql.hpp"
#include "../players/banish.hpp"
#include "../pfind.hpp"
#include "../world-configuration.hpp"
#include "../interpreter.hpp"
#include "../players/db-load.hpp"
#include "../message-server.hpp"

#include "../player.hpp"
#include "../orm/admin/stay.hpp"


namespace mods::admin_tools::stay {
	void remove_stay(player_ptr_t& douche) {
		mods::orm::admin::stay s_orm;
		s_orm.remove_for(douche);
	}
	void save_to_db(player_ptr_t& douche, room_vnum r_vnum) {
		mods::orm::admin::stay s_orm;
		s_orm.initialize_row(douche,r_vnum);
	}
	void stay_player(player_ptr_t& douche, room_vnum r_vnum) {
		douche->can_move() = true;
		char_from_room(douche->cd());
		char_to_room(douche->cd(),real_room(r_vnum));
		douche->can_move() = false;
	}
	void unstay_player(player_ptr_t& douche) {
		douche->can_move() = true;
		char_from_room(douche->cd());
		char_to_room(douche->cd(),mods::world_conf::real_mortal_start());
	}

	ADMINCMD(do_stay) {
		DO_HELP("admin:stay");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();

					player->send(CAT("[+] Pulling player '",name,"'...").c_str());
					stay_player(douche,player->vnum());
					remove_stay(douche);
					save_to_db(douche,player->vnum());
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}

	ADMINCMD(do_unstay) {
		DO_HELP("admin:unstay");
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() > 0) {
			for(auto name : vec_args) {
				auto p = mods::pfind::optby_name(name.c_str());
				if(!p.has_value()) {
					player->sendln(CAT("[ERROR] couldn't find player by name '",name,"'"));
				} else {
					auto douche = p.value();
					player->send("[+] Removing player from stand-still...");
					douche->can_move() = true;
					player->sendln("{grn}[DONE]{/grn}");

					player->send(CAT("[+] Pulling player '",name,"'...").c_str());
					char_from_room(douche->cd());
					char_to_room(douche->cd(),mods::world_conf::real_mortal_start());
					unstay_player(douche);
					remove_stay(douche);
					player->sendln("{grn}[DONE]{/grn}");
				}
			}
			ADMIN_DONE();
			return;
		}
		ADMIN_FAIL();
	}
	void init() {
		ADD_ADMIN_COMMAND("admin:stay",do_stay);
		ADD_ADMIN_COMMAND("admin:unstay",do_unstay);
	}
};//end namespace mods::ban_system
