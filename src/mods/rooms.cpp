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
