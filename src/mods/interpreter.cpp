#include "interpreter.hpp"
#include "help.hpp"
#include "super-users.hpp"

namespace mods::super_users {
	extern bool player_is(player_ptr_t&);
};
namespace mods::interpreter {
	static std::map<std::string,std::string> custom_who_lines;
	static std::deque<command_info> custom_cmd_info;
	command_info no_command = {
		"\n",0,0,0,0
	};
	void init(){
	}
	command_info& get_command(std::string_view arg, player_ptr_t& player){
		int length = arg.length();
		for(auto & cmd : custom_cmd_info){
			if(!strncmp(cmd.command, arg.data(), length)){
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
		command_info c = {command_string.c_str(),position,func,minimum_level,subcmd,command_string};
		c.command = c.str_command.c_str();
		custom_cmd_info.emplace_back(c);
	}
};
