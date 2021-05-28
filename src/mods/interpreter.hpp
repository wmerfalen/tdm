#ifndef __MENTOC_MODS_INTEPRETER_HEADER__
#define __MENTOC_MODS_INTEPRETER_HEADER__


#include <vector>
#include <string>
#include "../globals.hpp"
#include <functional>
#include "super-users.hpp"

namespace mods::interpreter {
	typedef void (*acmd_function)(char_data*, char*, int, int, player_ptr_t&);
	void add_command(std::string command_string, byte position, acmd_function, sh_int minimum_level,int subcmd);
	command_info& get_command(std::string_view,player_ptr_t&);
};

#ifndef ADD_BUILDER_COMMAND
#define ADD_BUILDER_COMMAND(MENTOC_ADC_STRING,MENTOC_ADC_ACTUAL) \
		mods::interpreter::add_command(MENTOC_ADC_STRING, POS_RESTING, MENTOC_ADC_ACTUAL, LVL_BUILDER,0)
#endif

#endif
