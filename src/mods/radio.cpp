#include "radio.hpp"

#define __MENTOC_MODS_VALUES_SHOW_RADIO_OUTPUT__
#ifdef  __MENTOC_MODS_VALUES_SHOW_RADIO_OUTPUT__
#define m_debug(a) std::cerr << "[mods::radio]: '" << a << "'\n";
#else
#define m_debug(a)
#endif
#include "super-users.hpp"
#include "interpreter.hpp"
#include "loops.hpp"
#include "str.hpp"
#include "orm/radio-station.hpp"

namespace mods::radio {
	struct station_t {
		station_t() = delete;
		station_t(std::shared_ptr<mods::orm::radio_station> _orm) :
			room_v(_orm->rs_room_vnum),
			name(_orm->rs_name),
			power(_orm->rs_power),
			id(_orm->id),
			orm(_orm) {
			room = real_room(room_v);
			if(room == NOWHERE) {
				x = y = z = 0;
				log("WARNING: real_room for radio station %d is NOWHERE", _orm->id);
			} else {
				x = world[room].x;
				y = world[room].y;
				z = world[room].z;
			}
			std::cout << "[station_t]: initialized from db: " << _orm->dump() << "\n";
		}
		auto destroy() {
			return orm->destroy();
		}
		int x;
		int y;
		int z;
		room_rnum room;
		room_vnum room_v;
		std::string name;
		uint16_t power;
		uint64_t id;
		std::shared_ptr<mods::orm::radio_station> orm;
	};
	using list_t = std::vector<player_ptr_t>;

	using station_ptr_t = std::shared_ptr<station_t>;
	static std::deque<station_ptr_t> stations;

	std::size_t load_all() {
		std::size_t count = 0;
		stations.clear();
		mods::orm::radio_station_list() = mods::orm::load_all_radio_station_list();
		for(const auto& station : mods::orm::radio_station_list()) {
			stations.emplace_back(std::make_shared<station_t>(station));
			++count;
		}
		return count;
	}

	std::optional<station_ptr_t> get_station_by_name(std::string_view name) {
		for(const auto& station : stations) {
			if(str::starts_with_lower(name,station->name)) {
				return station;
			}
		}
		return std::nullopt;
	}

	bool within_reach_of_station(player_ptr_t player,station_ptr_t station) {
		auto room = player->room();
		if(room >= world.size()) {
			std::cerr << "[station_t]: player in an odd room id: " << room << "\n";
			return false;
		}
		const auto& player_room = world[room];
		if(station->room == NOWHERE) {
			log("Player not within reach due to station being NOWHERE: %s",station->name.c_str());
			return false;
		}
		if(station->room >= world.size()) {
			log("Player not within reach because station is outside world.size(): %s",station->name.c_str());
			return false;
		}
		const auto& station_room = world[station->room];
		bool x_passes = false, y_passes = false, z_passes = false;
		if(player_room.x == station_room.x) {
			m_debug("x passes [0]");
			x_passes = true;
		} else if(player_room.x < station_room.x) {
			m_debug("x passes [1]");
			x_passes = player_room.x >= station_room.x - station->power;
		} else if(player_room.x > station_room.x) {
			m_debug("x passes [2]");
			x_passes = player_room.x <= station_room.x + station->power;
		}
		if(player_room.y == station_room.y) {
			m_debug("y passes [0]");
			y_passes = true;
		} else if(player_room.y < station_room.y) {
			m_debug("y passes [1]");
			y_passes = player_room.y >= station_room.y - station->power;
		} else if(player_room.y > station_room.y) {
			m_debug("y passes [2]");
			y_passes = player_room.y <= station_room.y + station->power;
		}
		if(player_room.z == station_room.z) {
			m_debug("z passes [0]");
			z_passes = true;
		} else if(player_room.z < station_room.z) {
			m_debug("z passes [1]");
			z_passes = player_room.z >= station_room.z - station->power;
		} else if(player_room.z > station_room.z) {
			m_debug("z passes [2]");
			z_passes = player_room.z <= station_room.z + station->power;
		}
		return x_passes && y_passes && z_passes;
	}

	list_t gather_players(station_ptr_t station) {
		list_t list;
		mods::loops::foreach_player<list_t>([&](player_ptr_t player, list_t* _list) -> bool {
			if(player->is_dead()) {
				return true;
			}

			if(within_reach_of_station(player,station)) {
				list.emplace_back(player);
			}
			return true;
		},&list);

		return list;
	}

	void transmit_globally(std::string_view msg) {
		list_t list;
		mods::loops::foreach_player<list_t>([&](player_ptr_t player, list_t* _list) -> bool {
			if(player->is_dead()) {
				return true;
			}
			player->sendln(CAT("{red}[HOSTILE RADIO{/red}: ",msg.data()));
			return true;
		},&list);

	}

	void add_station_to_list(auto& orm) {
		mods::orm::radio_station_list().emplace_back(std::make_shared<mods::orm::radio_station>(orm));
		stations.emplace_back(std::make_shared<station_t>(mods::orm::radio_station_list().back()));
	}


	SUPERCMD(do_admin_radio_transmit) {
		ADMIN_REJECT();
		if(!argshave()->size_gt(1)->passed()) {
			player->sendln("Usage: admin:radio:transmit <station-name> \"<message...>\"");
			return;
		}
		player->sendln(argat(0));
		auto station_opt = get_station_by_name(argat(0));
		if(!station_opt.has_value()) {
			player->sendln("Unable to find a station by that name.");
			return;
		}
		auto msg = argat(1);

		player->sendln(CAT("Sending message: \"",msg,"\""));

		for(const auto& player : gather_players(station_opt.value())) {
			player->sendln(CAT("{grn}[",station_opt.value()->name,"]: {/grn}",msg));
		}
		player->sendln("Done");

		ADMIN_DONE();
	}

	SUPERCMD(do_admin_radio_station_create) {
		ADMIN_REJECT();
		if(!argshave()->size_gt(1)->passed()) {
			player->sendln("Usage: admin:radio:station:create <station-name> <power>");
			return;
		}
		if(!argshave()->int_at(1)->passed()) {
			player->sendln("power must be a valid integer");
			return;
		}
		room_vnum room = player->vnum();
		auto power = intat(1);
		mods::orm::radio_station orm;
		auto status = orm.initialize_row(power,room,argat(0));
		if(status > 0) {
			player->sendln("Created radio station in your current room.");
		} else {
			player->sendln("Could not create a radio station in your current room.");
			return;
		}

		add_station_to_list(orm);

		player->sendln("Station added to global list.");

		ADMIN_DONE();
	}
	SUPERCMD(do_admin_radio_station_delete) {
		ADMIN_REJECT();
		if(!argshave()->size_gt(0)->passed()) {
			player->sendln("Usage: admin:radio:station:delete <station-name>");
			return;
		}
		auto station_opt = get_station_by_name(argat(0));
		if(!station_opt.has_value()) {
			player->sendln("Could not find a station by that name");
			return;
		}
		auto station = station_opt.value();
		auto status = station->destroy();
		player->sendln(CAT("delete status: ",std::get<1>(status)));

		ADMIN_DONE();
	}
	SUPERCMD(do_admin_radio_station_reload) {
		ADMIN_REJECT();

		auto count = load_all();
		player->sendln(CAT("Loaded ",count, " stations from the database."));

		ADMIN_DONE();
	}
	SUPERCMD(do_admin_radio_station_list) {
		ADMIN_REJECT();

		player->sendln("Listing currently loaded stations (not from the db)...");
		for(const auto& s : stations) {
			player->sendln(CAT("{grn}[",s->id,"]{/grn}:",s->name,"power:",s->power,",room_vnum:",s->room_v));
		}
		player->sendln("End of list");
		ADMIN_DONE();
	}
	SUPERCMD(do_admin_radio_help) {
		ADMIN_REJECT();
		player->sendln(
		    CAT(
		        "{grn}admin:radio commands\r\n"
		        "{grn}admin:radio:create <station-name> <power>\r\n"
		        "{grn}admin:radio:list\r\n"
		        "{grn}admin:radio:delete <station-name>\r\n"
		        "{grn}admin:radio:reload\r\n"
		        "{grn}admin:radio:transmit <station-name> <message>\r\n"
		    )
		);

		ADMIN_DONE();
	}
	void init() {
		ADD_BUILDER_COMMAND("admin:radio",do_admin_radio_help);
		ADD_BUILDER_COMMAND("admin:radio:transmit",do_admin_radio_transmit);
		ADD_BUILDER_COMMAND("admin:radio:create",do_admin_radio_station_create);
		ADD_BUILDER_COMMAND("admin:radio:list",do_admin_radio_station_list);
		ADD_BUILDER_COMMAND("admin:radio:delete",do_admin_radio_station_delete);
		ADD_BUILDER_COMMAND("admin:radio:reload",do_admin_radio_station_reload);

		load_all();
	}
};

#undef m_debug
