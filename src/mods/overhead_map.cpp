#include "overhead_map.hpp"
#include "player.hpp"

extern std::vector<room_data> world;
namespace mods::overhead_map {
	template <>
	std::string_view generate(mods::player out,const room_rnum& room_number){
		return world[room_number].overhead(out.get_lense());
	}
	template <>
	std::string_view generate(mods::player* out,const room_rnum& room_number){
		return mods::overhead_map::generate<mods::player>(*out,room_number);
	}
};
