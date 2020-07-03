#include "rooms.hpp"
#include "weapon-types.hpp"

namespace mods::rooms {
	namespace affects {
		void process(){
			for(auto it = needs_dissolve.begin();it != needs_dissolve.end();++it){
				auto ticked = it->second.tick();
				auto fire_amount = it->second.get_affects()[(affect_t)room_data::texture_type_t::ON_FIRE];
				if(ticked == 0 && fire_amount == 0){
					mra_debug("Reached zero on: " << it->first);
					needs_dissolve.erase(it);
					mods::rooms::affects::process();
					return;
				}
			}
		}
	};
	bool has_textures(room_rnum r, std::vector<txt> textures){
		assert(r < world.size());
		for(auto & t : textures){
			if(world[r].has_texture(t) == false){
				return false;
			}
		}
		return true;
	}
	/**
	 * @brief entry point for setting a room on fire. use this
	 *
	 * @param room
	 */
	void start_fire_dissolver(room_rnum room) {
		static std::vector<txt> never_ignites = {
			txt::SEWER,txt::DAMP,txt::WATER,txt::UNDERWATER,
			txt::FROZEN
		};
		assert(room < world.size());
		if(is_peaceful(room)){
			log("Prevented peaceful room to catch on fire (%d)", room);
			return;
		}
		if(has_textures(room,never_ignites)){
			log("Room will never ignite because it has a texture that isn't flammable");
			return;
		}
		auto fire = fs::COMPLETELY_ON_FIRE;
		if(world[room].has_texture(txt::ON_FIRE) && affects::needs_dissolve.find(room) != affects::needs_dissolve.end()){
			log("Room already has fire dissolver running. Roomid: %d", room);
			return;
		}
		world[room].add_texture(txt::ON_FIRE);
		int ticks = FIRE_EVERY_N_TICKS;
		if(world[room].has_texture(txt::WOODEN_WALLS)){
			ticks += mods::values::FIRE_WOODEN_ADDITIONAL_TICKS;
		}
		if(world[room].has_texture(txt::CARPET)){
			ticks += mods::values::FIRE_CARPET_ADDITIONAL_TICKS;
		}
		auto initial_status = fs::KINDLING;
		if(has_textures(room, {txt::DRY, txt::GRASS})){
			initial_status = fs::COMPLETELY_ON_FIRE;
		}

		affects::add_room_dissolve_affect_every_n_tick(
				room,
				(affects::affect_t)fire,
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
	void set_flag_absolute(room_rnum room, int flag){
		world[room].room_flags = flag;
	}
	std::optional<sector_type_t> sector_from_string(std::string m){
		for(auto & pair : mods::rooms::sector_strings) {
			if(m.compare(pair.second) == 0){
				return pair.first;
			}
		}
		return std::nullopt;
	}
	void set_sector_type(room_rnum room_id, int sector_type){
		using txt = room_data::texture_type_t;
		switch((sector_type_t)sector_type){
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
			default:
			case sector_type_t::NONE:
				world[room_id].textures().clear();
				break;
		}
		world[room_id].sector_type = sector_type;
	}
};//End namespace
