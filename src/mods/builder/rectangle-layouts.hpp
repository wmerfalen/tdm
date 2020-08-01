#ifndef __MENTOC_MODS_BUILDER_RECTANGLE_LAYOUTS_HEADER__
#define __MENTOC_MODS_BUILDER_RECTANGLE_LAYOUTS_HEADER__
#include "../../globals.hpp"

namespace mods::builder::rectangle_layouts {
	struct dimensions_t {
		uint8_t width;
		uint8_t height;
		uint8_t offset_x;
		uint8_t offest_y;
	};
	std::string_view explain_make();
	void make(dimensions_t dim);
	void send_rbuild_help(player_ptr_t& player);
	bool handled_rbuild_command(player_ptr_t& player,str_t arguments);
};

#endif
