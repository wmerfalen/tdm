#include "rooms.hpp"
#include "weapon-types.hpp"
#include "weapons/damage-types.hpp"
#include "weapons/elemental.hpp"
#include "rand.hpp"
#include "builder.hpp"
#include "zone.hpp"
extern void	send_to_room(room_rnum room, const char *messg, ...) __attribute__((format(printf, 2, 3)));
extern void send_to_room_except(room_rnum room, std::vector<uuid_t> except, const char *messg, ...);
extern int next_room_vnum();
using coordinate_type_t = int32_t;
namespace mods::globals {
	extern void register_room(const room_rnum&);
	extern coordinate_type_t get_room_by_coordinates(coordinate_type_t target_x,coordinate_type_t target_y,coordinate_type_t target_z);
	extern void register_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room);
	extern void glue_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room);
};

namespace mods::rooms {
	void broadcast_injury(player_ptr_t victim) {
		const auto& u = victim->uuid();
		for(auto& p : mods::globals::get_room_list(victim->room())) {
			if(!p->is_npc() && u != p->uuid()) {
				p->sendln(CAT(victim->ucname()," is injured!"));
			}
		}
	}
	void broadcast_claymore_plant(const room_rnum& room,player_ptr_t& planter,const direction_t& direction,obj_ptr_t& claymore) {
		const auto& u = planter->uuid();
		for(auto& p : mods::globals::get_room_list(planter->room())) {
			if(!p->is_npc() && u != p->uuid()) {
				p->sendln(CAT(planter->ucname()," planted a ",claymore->name.c_str()," at the bottom of the ",dirstr(direction)," door."));
			}
		}

	}
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
	static inline std::map<std::string,room_rnum>& nicknames() {
		static std::map<std::string,room_rnum> list;
		return list;
	}
	void remove_nickname(const room_rnum& room) {
		for(const auto& pair : nicknames()) {
			if(pair.second == room) {
				nicknames().erase(pair.first);
				return;
			}
		}
	}
	void register_nickname(const room_rnum& room,std::string_view nick) {
		nicknames()[nick.data()] = room;
	}
	std::optional<room_rnum> find_nickname(std::string_view nick) {
		if(nicknames().find(nick.data()) == nicknames().end()) {
			return std::nullopt;
		}
		return nicknames()[nick.data()];
	}
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
#ifdef __MENTOC_SHOW_START_FIRE_DISSOLVER_DEBUG_OUTPUT__
		std::cerr << "###############################\n";
		std::cerr << "[start_fire_dissolver] ENTRY\n";
		std::cerr << "###############################\n";
#endif
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
		bool is_volatile = world[room].has_texture(txt::VOLATILE);
		if(is_volatile) {
			ticks += FIRE_VOLATILE_ADDITIONAL_TICKS();
		}
		if(world[room].has_texture(txt::WOODEN_WALLS)) {
			ticks += FIRE_WOODEN_ADDITIONAL_TICKS();
		}
		if(world[room].has_texture(txt::CARPET)) {
			ticks += mods::values::FIRE_CARPET_ADDITIONAL_TICKS();
		}
		fs initial_status = fs::KINDLING;
		if(has_textures(room, {txt::DRY, txt::GRASS}) || is_volatile) {
			initial_status = fs::COMPLETELY_ON_FIRE;
		}
		world[room].texture_level(txt::ON_FIRE) = initial_status;

#ifdef __MENTOC_SHOW_START_FIRE_DISSOLVER_DEBUG_OUTPUT__
		std::cerr << "###############################\n";
		std::cerr << "[start_fire_dissolver] TICKS: " << ticks << "\n";
		std::cerr << "###############################\n";
#endif
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
	void set_room_textures(room_rnum room_id, std::string textures) {
		if(textures.length() == 0) {
			world[room_id].textures().clear();
			return;
		}
		std::string current = "";
		for(const auto& ch : textures) {
			if(isspace(ch)) {
				continue;
			}
			if(ch == ',') {
				if(current.length()) {
					auto opt = texture_from_string(current);
					if(opt.has_value()) {
						world[room_id].add_texture(opt.value());
					}
					current.clear();
					continue;
				}
				continue;
			}
			current += ch;
		}
		if(current.length()) {
			auto opt = texture_from_string(current);
			if(opt.has_value()) {
				world[room_id].add_texture(opt.value());
			}
		}
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
			case sector_type_t::OUTSIDE_TRASHY_PARKING_LOT:
				world[room_id].add_textures({
					txt::CEMENT,
					txt::OUTSIDE,
					txt::DRY,
					txt::TRASHY
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
	void glue_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room) {
		mods::globals::glue_room_at_coordinates(x,y,z,room);
	}
	void register_room_at_coordinates(coordinate_type_t x, coordinate_type_t y, coordinate_type_t z, room_vnum room) {
		mods::globals::register_room_at_coordinates(x,y,z,room);
	}
	coordinate_type_t get_room_by_coordinates(coordinate_type_t target_x,coordinate_type_t target_y,coordinate_type_t target_z) {
		return mods::globals::get_room_by_coordinates(target_x,target_y,target_z);
	}
	void pave_once(room_rnum room,direction_t door) {
		int x = 0, y = 0, z = 0;
		switch(door) {
			case UP:
				z += 1;
				break;
			case SOUTH:
				y -= 1;
				break;
			case WEST:
				x -= 1;
				break;

			case EAST:
				x += 1;
				break;

			case NORTH:
				y += 1;
				break;

			case DOWN:
				z -= 1;
				break;
			default:
				return;
		}

		auto cached_room = room;
		int target_x = world[cached_room].x + x;
		int target_y = world[cached_room].y + y;
		int target_z = world[cached_room].z + z;
		room_vnum existing_room_vnum = get_room_by_coordinates(target_x,target_y,target_z);
		room_data* existing_room = nullptr;
		int real_room_id = 0;
		if(existing_room_vnum > -1) {
			real_room_id = real_room(existing_room_vnum);
			if(real_room_id != NOWHERE) {
				existing_room = &world[real_room_id];
			}
		}
		if(world[cached_room].dir_option[door] == nullptr) {
			if(existing_room) {
				world[cached_room].set_dir_option(
				    door,
				    "general_description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    real_room_id
				);
				existing_room->set_dir_option(
				    OPPOSITE_DIR(door),
				    "general description",
				    "keyword",
				    EX_ISDOOR,
				    0,
				    cached_room
				);
				glue_room_at_coordinates(existing_room->x,existing_room->y,existing_room->z,existing_room->number);
				return;
			}
			int new_room_rnum = 0;
			world.emplace_back();
			mods::globals::register_room(0);
			int new_room_vnum = next_room_vnum();
			auto& w = world.back();
			w.number = new_room_vnum;
			new_room_rnum = world.size() - 1;

			world[cached_room].set_dir_option(
			    door,
			    "general_description",
			    "keyword",
			    EX_ISDOOR,
			    0,
			    new_room_rnum
			);
			w.set_dir_option(
			    OPPOSITE_DIR(door),
			    "general description",
			    "keyword",
			    EX_ISDOOR,
			    0,
			    cached_room
			);
			w.x = target_x;
			w.y = target_y;
			w.z = target_z;
			register_room_at_coordinates(w.x,w.y,w.z,w.number);
			glue_room_at_coordinates(w.x,w.y,w.z,w.number);
		}
	}

	bool is_dark(room_rnum room) {
		return false;
		if(room >= world.size()) {
			log("SYSERR: %d is out of bounds",room);
			return false;
		}
		if(world[room].light) {
			return false;
		}
		bool fire_chance = 0;

		/** if on fire, check which constant and process */
		auto fire_status = world[room].fire_status();
		switch(fire_status) {
			case room_data::fire_status_t::NONE:
			case room_data::fire_status_t::OUT:
			case room_data::fire_status_t::SMOKING:
				break;
			case room_data::fire_status_t::SMOLDERING:
				/** 1 percent chance that smoldering remains will light the room */
				fire_chance = mods::rand::chance(1);
				break;
			case room_data::fire_status_t::COMPLETELY_ON_FIRE:
			case room_data::fire_status_t::KINDLING:
				return true;
			default:
				std::cerr << "warning, weird fire_status encountered. ignoring..\n";
				break;
		}


		using txt = room_data::texture_type_t;
		auto always_dark = {
			txt::SEWER,
			txt::TUNNEL, /** a tunnel made of any material */
			txt::UNDERWATER,
			txt::HAZARDOUS_SMOKE, /** think: gas attacks */
			txt::NON_HAZARDOUS_SMOKE, /** think: burning car */
		};
		/** TODO: honor this */
#if 0
		auto depends = {
			txt::GLASS_WINDOWS,
			txt::EMP, /** chaff or emp has been detonated */
			txt::FOREST,
			txt::LOW_ATMOSPHERE,	/** atmosphere too thin for helicopter to fly */
		};
#endif
		auto has_light = {
			txt::INSIDE,
			txt::ELEVATOR,
			txt::GLASS_WINDOWS,
			txt::LADDER,
			txt::RADIOACTIVE, /** actively emitting radioactivity */
			txt::METAL_HATCH,
			txt::METAL_WALL,
			txt::ROOFTOP,
			txt::SCANNED,
			txt::SERVER_ROOM,
		};
		for(auto& t : always_dark) {
			if(world[room].has_texture(t)) {
				return true;
			}
		}
		for(auto& t : has_light) {
			if(world[room].has_texture(t)) {
				return false;
			}
		}

		if(weather_info.sunlight == SUN_SET || weather_info.sunlight == SUN_DARK) {
			return true;
		}
		if(ROOM_FLAGGED(room, ROOM_DARK) && !fire_chance) {
			return true;
		}

		return false;
	}

	std::string m(std::map<std::string,std::string>& d,std::string_view key,std::string_view fallback) {
		if(d.find(key.data()) != d.cend()) {
			return d[key.data()];
		}
		return fallback.data();
	}
	template <typename T>
	T im(std::map<std::string,std::string>& d,std::string_view key,T fallback) {
		if(d.find(key.data()) != d.cend()) {
			return mods::util::stoi<T>(d[key.data()]);
		}
		return fallback;
	}

	/**
	 * For creating rooms in an ad-hoc way
	 */
	room_data& create_room(std::map<std::string,std::string> data) {

#define mapor(A,B) m(data,A,B)
#define imapor(A,B) im(data,A,B)
		room_data room;
		room.name.assign(mapor("name","name"));
		room.description.assign(mapor("description","description"));
#ifdef __MENTOC_SHOW_PARSE_SQL_ROOMS_DEBUG__
		log("DEBUG: room: %d name: (%s), description: (%s)",mods::util::stoi<int>(room_records_row["id"].c_str()),room.name.c_str(),room.description.c_str());
#endif
		room.number = imapor("room_number",next_room_vnum());
#ifdef __MENTOC_SHOW_PARSE_SQL_ROOMS_DEBUG__
		log("parse_sql_rooms: room.number (%d)",room.number);
#endif
		room.zone = real_zone(imapor("zone",0));
		room.sector_type = imapor("sector_type",0);
		room.light = imapor("light",0);

		world.emplace_back(room);
		mods::globals::register_room(world.size());
		mods::rooms::set_sector_type(world.size()-1,imapor("sector_type",0));
		mods::rooms::set_flag_absolute(world.size()-1,imapor("room_flag",0));
		top_of_world = world.size();
		std::string nick = mapor("nickname","");
		if(nick.length()) {
			mods::rooms::register_nickname(top_of_world,nick);
			world[top_of_world-1].nickname.assign(nick);
		}
		mods::zone::new_room(&room);
		return world.back();
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
