#include "help.hpp"
namespace mods::help::pages {
	static constexpr const char* h_contract = "usage: contract <list>\r\n"
		"usage: contract <join> <N>\r\n"
		"usage: contract <leave> <N>\r\n"
		"This documentation was written on 2020-07-27."
		;
	static constexpr const char* h_grenade = "usage: throw <direction> [room_count=4]\r\n"
			"example: \r\n"
			" $ get frag backpack\r\n"
			" $ hold frag\r\n"
			" $ throw north 2\r\n"
			" This will throw a frag 2 rooms away\r\n"
			" NOTE:\r\n"
			"All grenades are thrown as far as they can up to a maximum amount of 4 rooms away\r\n"
			"or however many rooms before it reaches a dead-end\r\n"
			"see: help grenade"
		;
	static constexpr const char* h_install = "usage: install <object> <direction>\r\n"
			"description: the 'install' command is used to install \r\n" 
			"devices like cameras or claymore mines. To install a claymore\r\n" 
			"mine, you would simply type 'install claymore north'. This would\r\n" 
			"install the claymore to the north exit of the room. Any NPC that\r\n" 
			"leaves or enters the current room through the northern exit will\r\n" 
			"be met with an explosion.\r\n" 
			"\r\n" 
			"The other use case of the 'install' command is to install cameras\r\n" 
			"onto a wall inside a room. To install a camera on the north side of\r\n" 
			"the room, you would type 'install camera north'.\r\n" 
			"\r\n" 
			"To remove the device, use the 'uninstall' command\r\n" 
			"Type 'help uninstall' for more information.\r\n" 
			"\r\n" 
			"To cancel the installation of a device while you are currently\r\n" 
			"installing it, you must type 'cancel'\r\n" 
			"\r\n" 
			"For more information: see the help manual for the following keywords:\r\n" 
			"'cancel','camera','claymore','install','uninstall'\r\n" 
			"\r\n"  
			"This documentation was written on 2020-03-29." 
			;

	static constexpr const char* h_cancel = "usage: cancel\r\n"
			"description: the 'cancel' command is used to stop the install\r\n" 
			"command. If you are installing a camera on the wall, it takes a\r\n" 
			"certain amount of time before that process is done. In that time,\r\n" 
			"you are vulnerable to attacks.\r\n" 
			"\r\n" 
			"For more information: see the help manual for the following keywords:\r\n" 
			"'cancel','camera','claymore','install','uninstall'\r\n" 
			"\r\n"  
			"This documentation was written on 2020-03-29." 
	;

	static constexpr const char* h_view = "usage: view camera\r\n"
			"description: the 'view' command is used to look through the eyes \r\n" 
			"of a camera that has been installed.\r\n" 
			"\r\n" 
			"type 'view camera' and if you have a camera installed, you will see \r\n" 
			"the contents of the room as if you were there.\r\n" 
			"\r\n"  
			"this documentation was written on 2020-03-25." 
	;
	static constexpr const char* h_set_npc_position = "usage: set_npc_position <UUID> <POSITION>\r\n"
			"description: valid positions include: \r\n" 
			"DEAD        -> dead\r\n"
			"MORTALLYW   -> mortally wounded\r\n"
			"INCAP       -> incapacitated\r\n"
			"STUNNED     -> stunned\r\n"
			"SLEEPING    -> sleeping\r\n"
			"RESTING     -> resting\r\n"
			"SITTING     -> sitting\r\n"
			"FIGHTING    -> fighting\r\n"
			"STANDING    -> standing\r\n"
			"\r\n"  
			"This command is not case-sensitive.\r\n"
			"\r\n"
			"example: set_npc_position 45 INCAP\r\n"
			"\r\n"
			"Note: to grab an npc's uuid, go to the same room as it and type room_list_uuid\r\n"
			"\r\n"
			"For more information: see the help manual for the following keywords:\r\n" 
			"'room_list_uuid', 'set_npc_position', 'set_position'\r\n"
			"\r\n"
			"this documentation was written on 2020-06-26." 
	;
	static constexpr const char* h_set_position = "usage: set_position <POSITION>\r\n"
			"description: valid positions include: \r\n" 
			"DEAD        -> dead\r\n"
			"MORTALLYW   -> mortally wounded\r\n"
			"INCAP       -> incapacitated\r\n"
			"STUNNED     -> stunned\r\n"
			"SLEEPING    -> sleeping\r\n"
			"RESTING     -> resting\r\n"
			"SITTING     -> sitting\r\n"
			"FIGHTING    -> fighting\r\n"
			"STANDING    -> standing\r\n"
			"\r\n"  
			"This command is not case-sensitive.\r\n"
			"\r\n"
			"example: set_position INCAP\r\n"
			"\r\n"
			"this documentation was written on 2020-06-26." 
	;
	static constexpr const char* h_yaml_log = "usage: yaml_log \r\n"
			"description: the yaml_log command has two types of usages.\r\n"
			"1) calling yaml_log with no arguments will send you the current yaml log\r\n"
			"2) calling yaml_log the same way you would call yaml_import\r\n"
			"Example: yaml_log RIFLE g36c.yml\r\n"
			"The above example will attempt to import and give you the g36c.yml file.\r\n"
			"Should any exceptions with the yaml import occur, you can see the log\r\n"
			"of those errors by calling yaml_log with no arguments.\r\n"
			"\r\n"
			"this documentation was written on 2020-09-02." 
	;
	static constexpr const char* h_allow_skill = "usage: allow_skill <player_name> <skill|all> \r\n"
			"description: this command will take a player name and a skill as the second\r\n"
			"argument. You can also pass in 'all' as the second argument and it will\r\n"
			"allow all skills available.\r\n"
			"Example: allow_skill player1 basic-armor\r\n"
			"Example: allow_skill player1 all\r\n"
			"\r\n"
			"this documentation was written on 2020-09-06." 
	;
	static constexpr const char* h_yaml_example = "usage: yaml_example <list> <object_type>\r\n"
			"description: this command will take an object type and write an example yaml file.\r\n"
			"you can optionally send the string list as the only argument to this function\r\n"
			"and it will spit out all the possible object types.\r\n"
			"\r\n"
			"The main function of yaml_example is to write an example file for the object type you specify.\r\n"
			"\r\n"
			"Example: yaml_example drone\r\n"
			"The above example will write to /lib/objects/drone.yml\r\n"
			"\r\n"
			"this documentation was written on 2020-09-08." 
	;
	static constexpr const char* h_skills = "usage: skills [show] [skill_name]...[skill_N]\r\n"
			"description: This command will show you your proficiencies with each skill.\r\n"
			"To see a detailed description of a skill, type: {grn}skills show <skill>{/grn}\r\n"
			"{blu}Example: {yel}skills show spray-chance{/yel}\r\n"
			"{blu}NOTE: you can specify multiple skills\r\n"
			"Example: {yel}skills show spray-chance basic-armor mold{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-10.{/blu}"
	;
	static constexpr const char* h_enable_registration = "usage: enable_registration\r\n"
	 	"usage: disable_registration\r\n"
	 	"usage: registration_status\r\n"
			"description: depending on which command you call, it will enable/disable player\r\n"
			"registration until you specify otherwise.\r\n"
			"{blu}Example: {yel}enable_registration{/yel}\r\n"
			"registration_status will tell you if player registration is enabled or not.\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-11.{/blu}"
	;
};

namespace mods::help {
	bool matches_many(const std::string& items,std::string_view from){
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(items);
		while (std::getline(tokenStream, token, ',')) {
			if(token.compare(from.data()) == 0){
				return true;
			}
		}
		return false;
	}
	bool send_help(std::string_view from, std::shared_ptr<mods::player>& player){
#define M_MATCH(A,SUBJECT) if(matches_many(A,from)){ player->sendln(CAT("{blu}",pages::SUBJECT,"{/blu}")); return false; }
			M_MATCH("throw,grenade",h_grenade);
			M_MATCH("camera,claymore,install,uninstall",h_install);
			M_MATCH("cancel",h_cancel);
			M_MATCH("set_position",h_set_position);
			M_MATCH("set_npc_position",h_set_npc_position);
			M_MATCH("contract,contracts,quest,quests",h_contract);
			M_MATCH("yaml_log",h_yaml_log);
			M_MATCH("allow_skill",h_allow_skill);
			M_MATCH("yaml_example",h_yaml_example);
			M_MATCH("skills,skill",h_skills);
			M_MATCH("registration_status,enable_registration,disable_registration",h_enable_registration);
#undef M_MATCH
			return true;
	}
	bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help){
		bool show = false;
		auto vec_args = PARSE_ARGS();
		if(vec_args.size() == 0 && zero_is_help){
			show = true;
		}
		if(!show && !mods::util::parse_help(argument)){
			return true;
		}
		return send_help(from,player);
	}
};
