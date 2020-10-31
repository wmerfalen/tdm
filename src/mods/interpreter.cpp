#include "interpreter.hpp"
#include "help.hpp"
#include "super-users.hpp"
#ifdef __MENTOC_SHOW_MODS_INTERPRETER_DEBUG_OUTPUT__
#define mi_debug(a) mentoc_prefix_debug("mods::interpreter") << a << "\n";
#else
#define mi_debug(a) /**/
#endif

namespace mods::super_users {
	extern bool player_is(player_ptr_t&);
};
namespace mods::interpreter {
	static std::map<std::string,std::string> custom_who_lines;
	std::deque<command_info>& custom_cmd_info_list(){
		static std::deque<command_info> command_list;
		return command_list;
	}
	static command_info no_command = {
		"\n",0,0,0,0
	};
	command_info& get_command(std::string_view arg, player_ptr_t& player){
		for(auto & cmd : mods::interpreter::custom_cmd_info_list()){
			if(cmd.str_command.compare(arg.data()) == 0){
				if(mods::super_users::player_is(player) || player->god_mode()){
					return cmd;
				}
				if(GET_LEVEL(player->cd()) >= cmd.minimum_level) {
					return cmd;
				}
			}
		}
		return no_command;
	}

	void add_command(std::string command_string, byte position, acmd_function func, sh_int minimum_level,int subcmd){
		mi_debug("[ADDING COMMAND]:'" << command_string << "'");
		command_info c;
		c.str_command = command_string;
		c.minimum_position = position;
		c.command_pointer = func;
		c.minimum_level = minimum_level;
		c.subcmd = subcmd;
		c.command = c.str_command.c_str();

		mods::interpreter::custom_cmd_info_list().emplace_back(std::move(c));
	}
};
#undef mi_debug