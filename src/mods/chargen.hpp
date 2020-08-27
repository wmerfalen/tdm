#ifndef __MENTOC_MODS_CHARGEN_HEADER__
#define __MENTOC_MODS_CHARGEN_HEADER__

#include <iostream>
#include <string>
#include "../structs.h"

namespace mods::chargen {
	std::string_view parse_primary_choice(char choice,int class_type);
	std::string_view primary_weapon_menu(player_class_t class_type);
	std::tuple<bool,std::string> make_char(player_ptr_t player);
	void undo_make_char(player_ptr_t player);
	void handle_primary_choice(player_ptr_t p,char choice,player_class_t class_type);
	std::string get_class_description(player_class_t p_class);
};

#endif
