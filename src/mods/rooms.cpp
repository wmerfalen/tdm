#include "rooms.hpp"
#include "weapon-types.hpp"
#include "weapons/damage-types.hpp"
#include "weapons/elemental.hpp"
extern void	send_to_room(room_rnum room, const char *messg, ...) __attribute__((format(printf, 2, 3)));
extern void send_to_room_except(room_rnum room, std::vector<uuid_t> except, const char *messg, ...);
extern int next_room_vnum();
namespace mods::globals {
	extern void register_room(const room_rnum&);
};

namespace mods::rooms::affects {
	void process() {
		for(auto it = needs_dissolve.begin(); it != needs_dissolve.end(); ++it) {
			auto ticked = it->second.tick();
			auto fire_amount = it->second.get_affects()[(affect_t)room_data::texture_type_t::ON_FIRE];
			if(ticked == 0 && fire_amount == fs::OUT) {
				mra_debug("Reached zero on: " << it->first);
				needs_dissolve.erase(it);
				mra_debug("recursion");
				mods::rooms::affects::process();
				return;
			}
		}
	}
	void trigger_callback_for_affect(room_rnum room,affect_t affect) {
		needs_dissolve[room].trigger_callback(affect);
	}
	void add_callback(room_rnum room, affect_t affect, callback_t f) {
		mra_debug("add_callback entry");
		mra_debug("SETTING CALLBACK on AFFECT: " << std::to_string(affect).c_str());
		needs_dissolve[room].set_callback(affect,f);
	}
	void on_room_fire_changed_affect(
	    room_rnum room,
	    affect_t affect, /* will always be ON_FIRE */
	    affect_amount_t amount
	) {
		fire_status_t fire_status = (fire_status_t)amount;
		world[room].fire_status() = fire_status;
		mra_debug("on_room_fire_Changed_affect [callback] -> affect:" << affect << ", amnt: " << amount << ", fire_status: " << fire_status);
		switch(fire_status) {
			case fire_status_t::KINDLING:
				send_to_room(room, "\r\n{red}A fire begins to form!{/red}\r\n");
				break;
			case fire_status_t::COMPLETELY_ON_FIRE:
				send_to_room(room, "\r\n{red}The room is {yel}TOTALLY ENGULFED{/yel}{red} in fire!{/red}\r\n");
				break;
			case fire_status_t::SMOLDERING:
				send_to_room(room, "\r\n{red}The fire loses some intesity and smoke begins to fill the room.{/red}\r\n");
				break;
			case fire_status_t::SMOKING:
				send_to_room(room, "\r\n{red}The room is filled with smoke as the fire dies...{/red}\r\n");
				break;
			case fire_status_t::OUT:
				send_to_room(room, "\r\n{red}The fire is put out.{/red}\r\n");
				world[room].remove_texture(txt::ON_FIRE);
				break;
			default:
				log((std::string("Invalid fire status: ") + std::to_string(fire_status) + std::string("on room:") + std::to_string(room)).c_str());
				break;
		}
	}
	void remove_room_dissolve_affect(room_rnum room, affect_t item) {
		needs_dissolve.erase(room);
	}
	void set_affect_max_amount(room_rnum room,affect_t affect,affect_amount_t amount) {
		needs_dissolve[room].set_max_amount(affect,amount);
		mra_debug("Set direction to increment for affect: " << affect << " with max_amount: " << amount);
	}
	void add_room_dissolve_affect_every_n_tick(room_rnum room, affect_t affect, affect_amount_t amt, uint32_t n_ticks) {
		needs_dissolve[room].entity_id = room;
		needs_dissolve[room].affect_every_n_ticks(affect,amt,n_ticks);
		std::cerr << "add_room_dissolve_affect_every_n_tick: affect: " << affect << ", amt: " << amt << ", nticks:" << n_ticks << "\n";
		mra_debug("Added room dissolve using ever n (" << n_ticks << ") ticks... to " << room);
	}
	void add_room_dissolve_affect(room_rnum room, affect_map_t& items) {
		mra_debug("Added mapped items to " << room);
		needs_dissolve[room].entity_id = room;
		needs_dissolve[room].affect_map(items);
	}
	void set_affect_to_decrement(room_rnum room,affect_t affect) {
		needs_dissolve[room].set_direction(affect,false);
		mra_debug("Set direction to decrement for affect: " << affect);
	}
	void set_affect_to_increment(room_rnum room,affect_t affect) {
		needs_dissolve[room].set_direction(affect,true);
		mra_debug("Set direction to increment for affect: " << affect);
	}
};//end mods::rooms::affects

namespace mods::rooms {
	/** continuation of namespace mods::rooms */
	/** continuation of namespace mods::rooms */
	/** continuation of namespace mods::rooms */
	bool has_emp(room_rnum r) {
		return has_textures(r, {txt::EMP});
	}
	bool has_textures(room_rnum r, std::vector<txt> textures) {
		assert(r < world.size());
		for(auto& t : textures) {
			if(world[r].has_texture(t) == false) {
				return false;
			}
		}
		return true;
	}
	void process_fire_damage() {
		for(auto& nd : affects::needs_dissolve) {
			if(!nd.second.has_affect(txt::ON_FIRE)) {
				continue;
			}

			uint16_t fire_damage = 0,smoke_damage =0;
			switch((fs)nd.second.get_affects()[txt::ON_FIRE]) {
				case fs::KINDLING:
					fire_damage = FIRE_STATUS_KINDLING_DAMAGE();
					break;
				case fs::COMPLETELY_ON_FIRE:
					fire_damage = FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE();
					smoke_damage = FIRE_STATUS_COMPLETELY_ON_FIRE_DAMAGE();
					break;
				case fs::SMOLDERING:
					fire_damage = FIRE_STATUS_SMOLDERING_DAMAGE();
					smoke_damage = FIRE_STATUS_SMOLDERING_DAMAGE();
					break;
				case fs::SMOKING:
					smoke_damage = FIRE_STATUS_SMOKING_DAMAGE();
					break;
				default:
					return;
			}
			if(fire_damage) {
				for(auto& player : room_list(nd.first)) {
					mods::weapons::elemental::room_fire_damage(player,fire_damage);
				}
			}
			if(smoke_damage) {
				for(auto& player : room_list(nd.first)) {
					mods::weapons::elemental::room_smoke_damage(player,smoke_damage);
				}
			}
		}
	}
	/**
	 * @brief entry point for setting a room on fire. use this
	 *
	 * @param room
	 */
	void start_fire_dissolver(room_rnum room) {
		std::cerr << "###############################\n";
		std::cerr << "[start_fire_dissolver] ENTRY\n";
		std::cerr << "###############################\n";
		static std::vector<txt> never_ignites = {
			txt::SEWER,txt::DAMP,txt::WATER,txt::UNDERWATER,
			txt::FROZEN,txt::SHALLOW_WATER
		};
		assert(room < world.size());
		if(is_peaceful(room)) {
			log("Prevented peaceful room to catch on fire (%d)", room);
			return;
		}
		if(has_textures(room,never_ignites)) {
			log("Room will never ignite because it has a texture that isn't flammable");
			return;
		}
		if(world[room].has_texture(txt::ON_FIRE) && affects::needs_dissolve.find(room) != affects::needs_dissolve.end()) {
			log("Room already has fire dissolver running. Roomid: %d", room);
			return;
		}
		world[room].add_texture(txt::ON_FIRE);
		uint32_t ticks = FIRE_EVERY_N_TICKS();
		if(world[room].has_texture(txt::WOODEN_WALLS)) {
			ticks += FIRE_WOODEN_ADDITIONAL_TICKS();
		}
		if(world[room].has_texture(txt::CARPET)) {
			ticks += mods::values::FIRE_CARPET_ADDITIONAL_TICKS();
		}
		fs initial_status = fs::KINDLING;
		if(has_textures(room, {txt::DRY, txt::GRASS})) {
			initial_status = fs::COMPLETELY_ON_FIRE;
		}
		world[room].texture_level(txt::ON_FIRE) = initial_status;

		std::cerr << "###############################\n";
		std::cerr << "[start_fire_dissolver] TICKS: " << ticks << "\n";
		std::cerr << "###############################\n";
		affects::add_room_dissolve_affect_every_n_tick(
		    room,
		    (affects::affect_t)txt::ON_FIRE,
		    initial_status,
		    ticks
		);

		affects::set_affect_to_increment(
		    room,
		    txt::ON_FIRE
		);

		affects::set_affect_max_amount(
		    room,
		    txt::ON_FIRE,
		    fire_status_t::OUT
		);

		affects::add_callback(room,
		                      txt::ON_FIRE,
		                      affects::on_room_fire_changed_affect
		                     );

		affects::trigger_callback_for_affect(room,txt::ON_FIRE);
	}
	void set_flag_absolute(room_rnum room, int flag) {
		world[room].room_flags = flag;
	}
	std::optional<sector_type_t> sector_from_string(std::string m) {
		for(auto& pair : mods::rooms::sector_strings) {
			if(m.compare(pair.second) == 0) {
				return pair.first;
			}
		}
		return std::nullopt;
	}
	void set_sector_type(room_rnum room_id, int sector_type) {
		using txt = room_data::texture_type_t;
		switch((sector_type_t)sector_type) {
			case sector_type_t::OUTSIDE_GRASSY:
				world[room_id].add_textures({
					txt::GRASS,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_CEMENT:
				world[room_id].add_textures({
					txt::CEMENT,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_FOREST:
				world[room_id].add_textures({
					txt::GRASS,
					txt::OUTSIDE,
					txt::DAMP,
					txt::FOREST
				});
				break;
			case sector_type_t::OUTSIDE_WOODEN_WALLS:
				world[room_id].add_textures({
					txt::OUTSIDE,
					txt::WOODEN_WALLS
				});
				break;
			case sector_type_t::OUTSIDE_DIRT:
				world[room_id].add_textures({
					txt::DIRT,
					txt::OUTSIDE,
					txt::DRY
				});
				break;
			case sector_type_t::OUTSIDE_DESERT:
				world[room_id].add_textures({
					txt::DIRT,
					txt::OUTSIDE,
					txt::DRY,
					txt::DESERT
				});
				break;
			case sector_type_t::OUTSIDE_WATER:
				world[room_id].add_textures({
					txt::DAMP,
					txt::OUTSIDE,
					txt::WATER
				});
				break;
			case sector_type_t::OUTSIDE_UNDERWATER:
				world[room_id].add_textures({
					txt::DAMP,
					txt::OUTSIDE,
					txt::WATER,
					txt::UNDERWATER
				});
				break;
			case sector_type_t::OUTSIDE_FROZEN:

				world[room_id].add_textures({
					txt::DAMP,
					txt::OUTSIDE,
					txt::FROZEN
				});
				break;
			case sector_type_t::OUTSIDE_HILLS:

				world[room_id].add_textures({
					txt::DESERT,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_MARKET_PLACE:
				world[room_id].add_textures({
					txt::OUTSIDE,
					txt::CEMENT,
				});
				break;
			case sector_type_t::OUTSIDE_METAL_HATCH:
				world[room_id].add_textures({
					txt::METAL_HATCH,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_TREE:

				world[room_id].add_textures({
					txt::TREE,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_ROOFTOP:

				world[room_id].add_textures({
					txt::ROOFTOP,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_WOODEN_WALL:

				world[room_id].add_textures({
					txt::WOODEN_WALLS,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_METAL_WALL:
				world[room_id].add_textures({
					txt::METAL_WALL,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_AIR:

				world[room_id].add_textures({
					txt::AIR,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::OUTSIDE_SHALLOW_WATER:

				world[room_id].add_textures({
					txt::SHALLOW_WATER,
					txt::DAMP,
					txt::OUTSIDE
				});
				break;
			case sector_type_t::INDOOR_WOODEN_WALLS:

				world[room_id].add_textures({
					txt::WOODEN_WALLS,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_CEMENT:
				world[room_id].add_textures({
					txt::CEMENT,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_CEMENT_WOODEN_WALLS:

				world[room_id].add_textures({
					txt::WOODEN_WALLS,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_CARPET_WOODEN_WALLS:

				world[room_id].add_textures({
					txt::WOODEN_WALLS,
					txt::INSIDE,
					txt::CARPET
				});
				break;
			case sector_type_t::INDOOR_TUNNEL:
				world[room_id].add_textures({
					txt::TUNNEL,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_METAL_WALLS:

				world[room_id].add_textures({
					txt::METAL_WALL,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_ELEVATOR:

				world[room_id].add_textures({
					txt::ELEVATOR,
					txt::METAL_WALL,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_SERVER_ROOM:

				world[room_id].add_textures({
					txt::SERVER_ROOM,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_SEWER:

				world[room_id].add_textures({
					txt::SEWER,
					txt::DAMP,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_MARKET_PLACE:

				world[room_id].add_textures({
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_METAL_HATCH:

				world[room_id].add_textures({
					txt::METAL_HATCH,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_ROOF:

				world[room_id].add_textures({
					txt::ROOFTOP,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_ATTIC:

				world[room_id].add_textures({
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_BASEMENT:

				world[room_id].add_textures({
					txt::DAMP,
					txt::INSIDE
				});
				break;
			case sector_type_t::INDOOR_SHALLOW_WATER:

				world[room_id].add_textures({
					txt::DAMP,
					txt::OUTSIDE,
					txt::SHALLOW_WATER
				});
				break;
			default:
			case sector_type_t::NONE:
				world[room_id].textures().clear();
				break;
		}
		world[room_id].sector_type = sector_type;
	}
	void word_wrap_description(player_ptr_t& player,room_rnum r) {
		int width = player->screen_width();
		if(width <= 0) {
			width = 80;
		}
		std::string f;
		int i = 0;
		for(auto ch : world[r].description.str()) {
			if(i >= (width - 6) && ch == ' ') {
				player->sendln(f);
				i = 0;
				f = "";
				continue;
			}
			if(i >= width && ch == ' ') {
				player->sendln(f);
				i = 0;
				f = "";
				continue;
			}
			f += ch;
			i++;
		}
		if(f.length()) {
			player->sendln(f);
		}
	}
	void pave_once(room_rnum room, direction_t direction) {
		auto cached_room = room;
		if(world[cached_room].dir_option[direction] == nullptr) {
			int new_room_rnum = 0;
			world.emplace_back();
			mods::globals::register_room(0);
			int new_room_vnum = next_room_vnum();
			auto& w = world.back();
			w.number = new_room_vnum;
			new_room_rnum = world.size() - 1;

			world[cached_room].set_dir_option(
			    direction,
			    "exploded_entrance",
			    "entrance",
			    EX_ISDOOR | EX_BREACHED,
			    0,
			    new_room_rnum
			);
			w.set_dir_option(
			    OPPOSITE_DIR(direction),
			    "general description",
			    "keyword",
			    EX_ISDOOR | EX_BREACHED,
			    0,
			    cached_room
			);
		}
	}
	bool is_dark(room_rnum room) {
		if(room >= world.size()) {
			log("SYSERR: %d is out of bounds",room);
			return false;
		}
		if(world[room].light) {
			return false;
		}

		if(ROOM_FLAGGED(room, ROOM_DARK)) {
			return true;
		}

		if(SECT(room) == SECT_INSIDE || SECT(room) == SECT_CITY) {
			return false;
		}

		if(weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK) {
			return true;
		}

		return false;
	}
};//End namespace mods::rooms

namespace mods::rooms::gods {
	void set_fire(room_rnum room,bool on,std::string& level,player_ptr_t& player) {
		std::string msg = "{yel}[debugging]";
		bool found = false;
		if(room >= world.size()) {
			msg = "invalid room id";
			goto cleanup;
		}
		if(on) {
			msg += "\r\nsetting room to on fire\r\n";
			start_fire_dissolver(room);
		} else {
			msg += "\r\nsetting room to __NOT__ on fire\r\n";
			stop_fire_dissolver(room);
		}
		if(level.length() == 0) {
			goto cleanup;
		}
		if(level.compare("NONE") == 0) {
			world[room].fire_status() = room_data::fire_status_t::OUT;
			msg += "set to OUT...";
			found = true;
			goto cleanup;
		}
		if(level.compare("KINDLING") == 0) {
			world[room].fire_status() = room_data::fire_status_t::KINDLING;
			msg += "set to kindling...";
			found = true;
			goto cleanup;
		}
		if(level.compare("COMPLETELY_ON_FIRE") == 0) {
			world[room].fire_status() = room_data::fire_status_t::COMPLETELY_ON_FIRE;
			msg += "set to cof...";
			found = true;
			goto cleanup;
		}
		if(level.compare("SMOLDERING") == 0) {
			world[room].fire_status() = room_data::fire_status_t::SMOLDERING;
			msg += "set to SMOLDERING...";
			found = true;
			goto cleanup;
		}
		if(level.compare("SMOKING") == 0) {
			world[room].fire_status() = room_data::fire_status_t::SMOKING;
			msg += "set to SMOKING...";
			found = true;
			goto cleanup;
		}
		if(level.compare("OUT") == 0) {
			world[room].fire_status() = room_data::fire_status_t::OUT;
			msg += "set to OUT...";
			found = true;
			goto cleanup;
		}
		if(!found) {
			player->sendln("Options are KINDLING,COMPLETELY_ON_FIRE,SMOLDERING,SMOKING,OUT,NONE");
		}
cleanup:
		msg += "\r\ncurrent fire status: '";
		msg += to_string(get_fire_status(room)) + "{/yel}'\r\n";
		player->sendln(msg);
		return;
	}
}; // end namespace mods::rooms::gods
