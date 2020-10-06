#ifndef __MENTOC_MODS_CHARGEN_HEADER__
#define __MENTOC_MODS_CHARGEN_HEADER__

#include <iostream>
#include <string>
#include "../structs.h"

namespace mods::chargen {
	int parse_primary_choice(char choice,int class_type);
	std::string primary_weapon_menu(player_class_t class_type);
	std::tuple<bool,std::string> make_char(player_ptr_t player,player_class_t class_type);
	void undo_make_char(player_ptr_t player);
	void handle_primary_choice(player_ptr_t p,char choice,player_class_t class_type);
	std::string get_class_description(player_class_t p_class);
	void show_triads(player_ptr_t& player, player_class_t class_type);
	void show_class_menu(player_ptr_t& player);
	const char* blind_friendly_prompt();
	void show_blind_friendly_chargen_prompt(player_ptr_t& player);
	void blind_chargen_take_over(player_ptr_t& player,std::string_view);
	int create_char_from_registration(player_ptr_t&,std::string);
};

#endif
