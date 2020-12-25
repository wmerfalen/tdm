#ifndef __MENTOC_INTERPRETER_INCLUDES_HEADER__
#define __MENTOC_INTERPRETER_INCLUDES_HEADER__
#include "super-users.hpp"
#include "interpreter.hpp"
#include "screen-searcher.hpp"

#ifndef ADD_INTERPRETER_COMMAND
#define ADD_INTERPRETER_COMMAND(COMMAND_NAME,ACCESS_TYPE) \
	mods::interpreter::add_command(#COMMAND_NAME, POS_RESTING, BOOST_PP_CAT(do_,COMMAND_NAME), ACCESS_TYPE,0);
#endif

#endif
