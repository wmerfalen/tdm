#ifndef __MENTOC_MODS_EXAMINE_HEADER__
#define __MENTOC_MODS_EXAMINE_HEADER__

#include "../globals.hpp"

extern void show_obj_to_char(obj_ptr_t& object, player_ptr_t& player, int mode,int count);
namespace mods::examine {
	void list_obj_contents_to_char(obj_ptr_t& object,player_ptr_t& player,int mode,int show);
	uuid_t find_player_by_name(player_ptr_t& player, std::string_view arg);
};
#endif
