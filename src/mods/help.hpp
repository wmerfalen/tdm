#ifndef __MENTOC_MODS_HELP_HEADER__
#define __MENTOC_MODS_HELP_HEADER__

#include "item-types.hpp"
#include "../globals.hpp"
#include <istream>

namespace mods::object_utils {
	extern std::vector<std::string> object_types();
};
namespace mods::util {
	extern bool parse_help(std::string_view argument);
};
namespace mods::help {
	void register_help_command(const std::string& command, const std::string& contents);
	bool matches_many(const std::string& items,std::string_view from);
	bool send_help(std::string_view from,std::shared_ptr<mods::player>& player);
	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help);
				void send_contagion_help_menu(player_ptr_t&);
				void send_ghost_help_menu(player_ptr_t&);
				void send_marksman_help_menu(player_ptr_t&);
				void send_bandit_help_menu(player_ptr_t&);
				void send_butcher_help_menu(player_ptr_t&);
				void send_striker_help_menu(player_ptr_t&);
				void send_obstructor_help_menu(player_ptr_t&);
				void send_malady_help_menu(player_ptr_t&);
				void send_pyrexia_help_menu(player_ptr_t&);
				void send_dealer_help_menu(player_ptr_t&);
				void send_forge_help_menu(player_ptr_t&);
				void send_syndrome_help_menu(player_ptr_t&);
				void send_machinist_help_menu(player_ptr_t&);
				void send_generic_help(player_ptr_t&);
};
#endif

