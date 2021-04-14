#include "shape-dictionary.hpp"
#include "../interpreter.hpp"
#include "../super-users.hpp"
#include "../builder.hpp"

#ifdef __MENTOC_SHOW_MODS_FORGE_ENGINE_DEBUG_OUTPUT__
#define m_debug(MSG) mentoc_prefix_debug("[mods::fluxkraft::shape_dictionary]")  << MSG << "\n";
#else
#define m_debug(MSG) ;;
#endif
namespace mods::globals {
	extern void glue_room_at_coordinates(int,int,int,room_vnum);
};

namespace mods::fluxkraft {
	using room_counter_t = uint8_t;
	struct attach_point_t {
		room_counter_t room;
		std::vector<char> attachment_points;
		std::shared_ptr<char_data> room_pointer;
	};

	struct shape_description {
		shape_description() = delete;
		shape_description(std::string_view shape_name,const std::vector<std::string>& directions) {
			name = shape_name;
			room_counter = 0;
			build_directions = directions;
		}
		void walk(player_ptr_t& player) {
			for(const auto& cmd : build_directions) {
				if(std::string::npos != cmd.find("procgen_attach")) {
					attach_point_t a;
					a.room = room_counter;
					auto spos = cmd.find_first_of(" ");
					if(std::string::npos != spos) {
						auto dirs = cmd.substr(spos);
						std::size_t i = 0;
						while(i < dirs.length()) {
							switch(dirs[i]) {
								default:
									++i;
									continue;
								case 'n':
								case 'e':
								case 's':
								case 'w':
								case 'u':
								case 'd':
									a.attachment_points.emplace_back(dirs[i]);
									break;
							}
							++i;
						}
					}
					attach_points.emplace_back(a);
					continue;
				}
				command_interpreter(player,cmd);
				room_counter++;
			}
		}
		std::string name;
		room_counter_t room_counter;
		std::vector<std::string> build_directions;
		std::vector<attach_point_t> attach_points;
	};

	shape_dictionary::shape_dictionary() {
		this->init();
	}
	void shape_dictionary::init() {
		std::cerr << "init ";
	}
	shape_dictionary::~shape_dictionary() {
		std::cout << "[fluxkraft::shape_dictionary::~shape_dictionary]\n";
	}
	std::vector<shape_description>& shape_list() {
		static std::vector<shape_description> list = {
			/**
			 * __
			 * |A| START
			 * |_|
			 * |_|____
			 * | | |A| END
			 * -------
			 */
			shape_description("southwest_corner",{"procgen_attach n,e,w","s","s","s","e","e","procgen_attach n,s,e"}),
			/**
			 * ________
			 * |_|_|_|A| START
			 * |_|
			 * |A| END
			 * ---
			 */
			shape_description("northwest_corner",{"procgen_attach w,s,e","w","w","w","s","s","procgen_attach n,e,s"}),
			/**
			 * END
			 * _______
			 * |A|_|_|
			 *     |_|
			 *     |_|
			 *     |A| START
			 *     ---
			 */
			shape_description("northeast_corner",{"procgen_attach n,w,s","n","n","n","w","w","procgen_attach w,s,e"}),
			/**
			 *       ___
			 *       |A| END
			 * ______|_|
			 * |A|_|_|_|
			 * START
			 */
			shape_description("southeast_corner",{"procgen_attach n,w,s","e","e","e","n","n","procgen_attach w,n,e"}),
			/**    ___
			 *     |A| START
			 *     |_|
			 * ____|_|
			 * |A|_|_|
			 *
			 * END
			 */
			shape_description("southeast_corner_v2",{"procgen_attach s,w,n","s","s","s","w","w","procgen_attach w,n,e"}),

			/**
			 * START
			 * ___________
			 * |A|_|_|_|A|
			 *     |_|
			 *     |_|
			 *     |A| END
			 *     ---
			 */
			shape_description("T_shape",{"procgen_attach w,n,s","e","e","s","s","s","procgen_attach w,s,e","n","n","n","e","e","procgen_attach n,e,s"}),

			/**
			 *       ___
			 *       |A| START
			 *  _____|_|
			 * |A|_|_|_|
			 *  END  |_|
			 *       |A|
			 */
			shape_description("T_shape_rotate_90_clockwise",{"procgen_attach n,w,s","s","s","w","w","w","procgen_attach w,n,e","e","e","e","s","s","procgen_attach w,s,e"}),
			/**
			 *     ___
			 *     |A|
			 *     |_|
			 * ____|_|____
			 * |A|_|_|_|A| START
			 * END
			 *
			 */
			shape_description("T_shape_rotate_180_clockwise",{"procgen_attach n,w,s","w","w","n","n","n","procgen_attach w,n,e","s","s","s","w","w","procgen_attach n,e,s"}),
			/**
			 * ___
			 * |A| START
			 * |_|______
			 * |_|_|_|A| END
			 * |_|
			 * |A|
			 */
			shape_description("T_shape_rotate_90_counter_clockwise",{"procgen_attach w,n,e","s","s","e","e","e","procgen_attach n,e,s","w","w","w","s","s","procgen_attach w,s,e"}),
			/**
			 * START
			 * ___   ___
			 * |A|   |A| END
			 * |_|   |_|
			 * |_|___|_|
			 * |_|_|_|_|
			 */
			shape_description("U_shape",{"procgen_attach n,w","s","s","s","e","e","e","n","n","n","procgen_attach n,e"}),
			/**
			 *
			 * _________
			 * |_|_|_|A| START
			 * |_|
			 * |_|______
			 * |_|_|_|A| END
			 *
			 */
			shape_description("C_shape",{"procgen_attach n,e","w","w","w","s","s","s","e","e","e","procgen_attach e,s"}),
			/**
			 * _________
			 * |_|_|_|_|
			 * |_|   |_|
			 * |_|   |_|
			 * |A|   |A| END
			 *
			 * START
			 */
			shape_description("n_shape",{"procgen_attach w,s","n","n","n","e","e","e","s","s","s","procgen_attach e,s"}),
			/**
			 *
			 * START
			 * _________
			 * |A|_|_|_|
			 *       |_|
			 * ______|_|
			 * |A|_|_|_|
			 * END
			 */
			shape_description("C_shape_hflip",{"procgen_attach n,w","e","e","e","s","s","s","w","w","w","procgen_attach w,s"}),
			/**
			 *
			 * _________
			 * |_|_|_|A| START
			 * |_|
			 * |_|_____
			 * |_|_|_|_|
			 *       |_|
			 * ______|_|
			 * |A|_|_|_|
			 * END
			 */
			shape_description("S_shape",{"procgen_attach n,e","w","w","w","s","s","s","e","e","e","s","s","s","w","w","w","procgen_attach w,s"}),
			/**
			 * START
			 * _________
			 * |A|_|_|_|
			 *       |_|
			 * ______|_|
			 * |_|_|_|_|
			 * |_|
			 * |_|______
			 * |_|_|_|A| END
			 */
			shape_description("Z_shape",{"procgen_attach n,w","e","e","e","s","s","s","w","w","w","s","s","s","e","e","e","procgen_attach e,s"}),
			/**
			 * _________   ___
			 * |_|_|_|_|   |A| END
			 * |_|   |_|   |_|
			 * |_|   |_|___|_|
			 * |A|   |_|_|_|_|
			 * START
			 */
			shape_description("N_shape",{"procgen_attach w,s","n","n","n","e","e","e","s","s","s","e","e","e","n","n","n","procgen_attach n,e"}),
			/**
			 * START
			 * ___   _________
			 * |A|   |_|_|_|_|
			 * |_|   |_|   |_|
			 * |_|___|_|   |_|
			 * |_|_|_|_|   |A| END
			 */
			shape_description("N_shape_vflip",{"procgen_attach w,n","s","s","s","e","e","e","n","n","n","e","e","e","s","s","s","procgen_attach e,s"}),
			/**
			 * START
			 * _____________
			 * |A|_..N.._|A| END
			 */
			shape_description("horitontal_pipe",{"procgen_attach w","procgen_random_amount e","procgen_attach e"}),
			/**
			 * |A| END
			 * ...
			 *  N
			 * ...
			 * |A| START
			 *
			 */
			shape_description("vertical_pipe",{"procgen_attach s","procgen_random_amount n","procgen_attach n"})
		};
		return list;
	}
	SUPERCMD(do_procgen_random_amount) {
		ADMIN_REJECT();
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() < 1) {
			player->errorln("usage: procgen_random_amount <direction>\r\nexample: procgen_random_amount n\r\n");
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
	/******************/
	/** generate_zone */
	/******************/
	SUPERCMD(do_generate_zone) {
		ADMIN_REJECT();
		DO_HELP("generate_zone");
		const auto& vec_args = PARSE_ARGS();
		if(vec_args.size() < 4) {
			player->errorln("usage: generate_zone <count> x y z\r\nexample: generate_zone 20 -400 -400 -400");
			return;
		}
		auto oc = mods::util::stoi(vec_args[0]);
		if(!oc.has_value()) {
			player->errorln("Invalid count specified");
			return;
		}
		int count = oc.value();
		if(count <= 0) {
			player->errorln("non-positive count specified");
			return;
		}

		int x = 0, y = 0, z = 0;
		auto ox = mods::util::stoi(vec_args[1]);
		auto oy = mods::util::stoi(vec_args[2]);
		auto oz = mods::util::stoi(vec_args[3]);
		if(!ox.has_value()) {
			player->errorln("Invalid x coordinate");
			return;
		}
		if(!oy.has_value()) {
			player->errorln("Invalid y coordinate");
			return;
		}
		if(!oz.has_value()) {
			player->errorln("Invalid z coordinate");
			return;
		}
		x = ox.value();
		y = oy.value();
		z = oz.value();
		auto& shapes = shape_list();
		if(mods::builder::currently_paving(player)) {
			mods::builder::pave_off(player);
		}
		mods::builder::pave_continue(player);
		world[player->room()].x = x;
		world[player->room()].y = y;
		world[player->room()].z = z;
		world[player->room()].starting_point = true;
		std::vector<shape_description> created;
		for(unsigned i = 0; i < count; i++) {
			created.emplace_back(shapes.at(rand_number(0,shapes.size()-1)));
			created.back().walk(player);
		}
		for(const auto& r : world) {
			mods::globals::glue_room_at_coordinates(r.x,r.y,r.z,r.number);
		}
		mods::builder::pave_off(player);
		ADMIN_DONE();
	}

	void init() {
		mods::interpreter::add_command("generate_zone", POS_RESTING, do_generate_zone, LVL_BUILDER,0);
	}
};//end namespace mods::fluxkraft
#undef m_debug
