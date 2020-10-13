#ifndef __MENTOC_MODS_SUPER_USERS_HEADER__
#define __MENTOC_MODS_SUPER_USERS_HEADER__


#include <vector>
#include <string>
#include "../globals.hpp"
#include <functional>

namespace mods::interpreter {
	void init();
	typedef void (*acmd_function)(char_data*, char*, int, int, player_ptr_t&);
	struct custom_command_info {
		std::string command;
		byte minimum_position;
		void	(*command_pointer)
		(char_data *ch, char *argument, int cmd, int subcmd,player_ptr_t&);
		sh_int minimum_level;
		int	subcmd;
	};

	void add_command(std::string command_string, byte position, acmd_function, sh_int minimum_level,int subcmd);
	command_info& get_command(std::string_view,player_ptr_t&);
};

#endif
