#include "shape-dictionary.hpp"
#include "../super-users.hpp"
#include "../interpreter.hpp"

#ifdef __MENTOC_SHOW_MODS_ARCON_BAR_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::arcon_bar]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif
namespace mods::globals {
	extern void glue_room_at_coordinates(int,int,int,room_vnum);
};

namespace mods::fluxkraft {
	namespace arcon {
		using room_counter_t = uint8_t;
		struct attach_point_t {
			room_counter_t room;
			std::vector<char> attachment_points;
			std::shared_ptr<char_data> room_pointer;
		};

		struct bar {
			bar() = delete;
			bar(std::string_view shape_name,const std::vector<std::string>& directions) {
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
	};

	namespace arcon_bar {
		SUPERCMD(do_generate_bar) {
			using namespace mods::fluxkraft::arcon;
			ADMIN_REJECT();
			auto vec_args = PARSE_ARGS();
			if(vec_args.size() < 1) {
				player->errorln("usage: generate_bar <direction>\r\nexample: generate_bar n\r\n");
				return;
			}
			auto d = mods::util::parse_direction(vec_args[0]);
			if(d < 0) {
				player->errorln("Invalid direction");
				return;
			}
			for(uint8_t i = rand_number(1,10); i > 0; --i) {
				command_interpreter(player,vec_args[0].substr(0,1));
			}
		}
		void init() {
			mods::interpreter::add_command("generate_bar", POS_RESTING, do_generate_bar, LVL_BUILDER,0);
		}
	}
};//end namespace mods::fluxkraft
#undef m_debug
