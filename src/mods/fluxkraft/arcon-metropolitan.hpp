#include "shape-dictionary.hpp"

#ifdef __MENTOC_SHOW_MODS_ARCON_METRO_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::arcon_metro]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif
namespace mods::globals {
	extern void glue_room_at_coordinates(int,int,int,room_vnum);
};

namespace mods::fluxkraft::arcon {
	using room_counter_t = uint8_t;
	struct attach_point_t {
		room_counter_t room;
		std::vector<char> attachment_points;
		std::shared_ptr<char_data> room_pointer;
	};

	struct metro {
		metro() = delete;
		metro(std::string_view shape_name,const std::vector<std::string>& directions) {
			name = shape_name;
			room_counter = 0;
			build_directions = directions;
		}
		void walk(player_ptr_t& player) {
		}
		std::string name;
		room_counter_t room_counter;
		std::vector<std::string> build_directions;
		std::vector<attach_point_t> attach_points;
	};

	void init() {
		//mods::interpreter::add_command("generate_zone", POS_RESTING, do_generate_zone, LVL_BUILDER,0);
	}
};//end namespace mods::fluxkraft
#undef m_debug
