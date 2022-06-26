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

namespace mods::radio {
	struct station_t {
		station_t() = delete;
		station_t(std::shared_ptr<mods::orm::radio_station> orm) :
			room_v(orm->rs_room_vnum),
			name(orm->rs_name),
			power(orm->rs_power) {
			std::cout << "[station_t]: initialized from db: " << orm->dump() << "\n";
		}
		room_vnum room_v;
		std::string name;
		uint16_t power;
	};
	using list_t = std::vector<player_ptr_t>;

	using station_ptr_t = std::shared_ptr<station_t>;
	static std::deque<station_ptr_t> stations;

	std::optional<station_ptr_t> get_station_by_name(std::string_view name) {
		for(const auto& station : stations) {
			if(str::starts_with_lower(name,station->name)) {
				return station;
			}
		}
		return std::nullopt;
	}

	list_t gather_players(station_ptr_t station) {
		list_t list;
		mods::loops::foreach_player<list_t>([&](player_ptr_t player, list_t* _list) -> bool {
			if(player->is_dead()) {
				return true;
			}
			_list->emplace_back(player);
			return true;
		},&list);

		return list;
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

		player->sendln(CAT("Sending message: \"",argat(1),"\""));

		for(const auto& player : gather_players(station_opt.value())) {
			player->sendln(CAT("{blu}[RADIO]{/blu}: ",argat(1)));
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
	void init() {
		ADD_BUILDER_COMMAND("admin:radio:transmit",do_admin_radio_transmit);
		ADD_BUILDER_COMMAND("admin:radio:station:create",do_admin_radio_station_create);

		mods::orm::radio_station_list() = mods::orm::load_all_radio_station_list();
		for(const auto& station : mods::orm::radio_station_list()) {
			stations.emplace_back(std::make_shared<station_t>(station));
		}
	}
};

#undef m_debug
