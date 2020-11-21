#include "help.hpp"
#include <map>
#include <string>
namespace mods::help::pages {
#define HELP_STR static constexpr const char*
	HELP_STR h_contract = "usage: contract <list>\r\n"
		"usage: contract <join> <N>\r\n"
		"usage: contract <leave> <N>\r\n"
		"This documentation was written on 2020-07-27."
		;
	HELP_STR h_grenade = "usage: throw <direction> [room_count=4]\r\n"
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
	HELP_STR h_install = "usage: install <object> <direction>\r\n"
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

	HELP_STR h_cancel = "usage: cancel\r\n"
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

	HELP_STR h_view = "usage: view camera\r\n"
			"description: the 'view' command is used to look through the eyes \r\n" 
			"of a camera that has been installed.\r\n" 
			"\r\n" 
			"type 'view camera' and if you have a camera installed, you will see \r\n" 
			"the contents of the room as if you were there.\r\n" 
			"\r\n"  
			"this documentation was written on 2020-03-25." 
	;
	HELP_STR h_set_npc_position = "usage: set_npc_position <UUID> <POSITION>\r\n"
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
	HELP_STR h_set_position = "usage: set_position <POSITION>\r\n"
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
	HELP_STR h_yaml_log = "usage: yaml_log \r\n"
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
	HELP_STR h_allow_skill = "usage: allow_skill <player_name> <skill|all> \r\n"
			"description: this command will take a player name and a skill as the second\r\n"
			"argument. You can also pass in 'all' as the second argument and it will\r\n"
			"allow all skills available.\r\n"
			"Example: allow_skill player1 basic-armor\r\n"
			"Example: allow_skill player1 all\r\n"
			"\r\n"
			"this documentation was written on 2020-09-06." 
	;
	HELP_STR h_yaml_example = "usage: yaml_example <list> <object_type>\r\n"
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
	HELP_STR h_skills = "usage: skills [show] [skill_name]...[skill_N]\r\n"
			"description: This command will show you your proficiencies with each skill.\r\n"
			"To see a detailed description of a skill, type: {grn}skills show <skill>{/grn}\r\n"
			"{blu}Example: {yel}skills show spray-chance{/yel}\r\n"
			"{blu}NOTE: you can specify multiple skills\r\n"
			"Example: {yel}skills show spray-chance basic-armor mold{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-10.{/blu}"
	;
	HELP_STR h_enable_registration = "usage: enable_registration\r\n"
	 	"usage: disable_registration\r\n"
	 	"usage: registration_status\r\n"
			"description: depending on which command you call, it will enable/disable player\r\n"
			"registration until you specify otherwise.\r\n"
			"{blu}Example: {yel}enable_registration{/yel}\r\n"
			"registration_status will tell you if player registration is enabled or not.\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-11.{/blu}"
	;
	HELP_STR h_install_camera_feed = "usage: install_camera_feed <name> <id>...[id_N]\r\n"
			"description: installs a camera feed identified by 'name'.\r\n"
			"The room virtual numbers you pass after the name of the camera feed will be the rooms\r\n"
			"that you would like to be shown on the camera feed.\r\n"
			"{blu}Example: {yel}install_camera_feed \"Camera Feed A\" 20 21 22 23 24{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-26.{/blu}"
	;
	HELP_STR h_uninstall_camera_feed = "usage: uninstall_camera_feed <name>\r\n"
			"description: uninstalls the camera feed identified by 'name'.\r\n"
			"{blu}Example: {yel}uninstall_camera_feed \"Camera Feed A\"{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-26.{/blu}"
	;
	HELP_STR h_install_minigame = "usage: install_minigame <name> <type> <difficulty> <unlock-event>\r\n"
			"description: installs a mini game identified by 'name'.\r\n"
			"Valid types:\r\n"
			"{blu}line-up: Will show a series of rows that the user has to line up accordingly\r\n"
			"{blu}wires: will show a series of wires and allow the user to attach them\r\n"
			"Valid difficulties:\r\n"
			"easy\r\n"
			"medium\r\n"
			"hard\r\n"
			"impossible\r\n"
			"\r\n"
			"The unlock-event will be one of the following:\r\n"
			"unlock <direction>\r\n"
			"lock <direction>\r\n"
			"toggle <direction>\r\n"
			"\r\n"
			"The unlock-event can also work with several directions separated by commas\r\n"
			"The unlock-event can work with room virtual numbers and directions\r\n"
			"unlock vnum:40 <direction>\r\n"
			"lock vnum:40 <direction>\r\n"
			"toggle vnum:40 <direction>\r\n"
			"If the unlock-event is neither of the above strings, then it will be fed to the system\r\n"
			"and handled accordingly.\r\n"
			"Example of a custom event:\r\n"
			"{blu}Example: {yel}install_minigame \"North Door Lock\" medium custom \"custom event. handled by system\"{/yel}\r\n"
			"The 'custom' keyword takes whatever you pass in and the system interprets it. This is for adding custom events\r\n"
			"that might work in the future or if the developers have a custom even that allow you to do special things.\r\n"
			"As of this writing, there are no custom events. 2020-09-28\r\n"
			
			"{blu}Example: {yel}install_minigame \"North Door Lock\" line-up medium unlock north{/yel}\r\n"
			"{blu}Example: {yel}install_minigame \"North Door Lock\" line-up medium unlock vnum:40 north,south,east,west{/yel}\r\n"
			"{blu}Example: {yel}install_minigame \"North Door Lock\" line-up medium toggle vnum:40 south,west,up{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-28.{/blu}"
	;
	HELP_STR h_list_minigame = "usage: list_minigame\r\n"
			"description: lists all the mini games in the current room. This command is needed to pass the id of the minigame\r\n"
			"to the uninstall_minigame command.\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-28.{/blu}"
	;
	HELP_STR h_uninstall_minigame = "usage: uninstall_minigame <id>\r\n"
			"description: uninstalls the currently installed mini game identified by the id you pass in.\r\n"
			"To see the ID's of the mini games currently installed in this room, see the list_minigame command.\r\n"
			"{blu}Example: {yel}uninstall_minigame \"North Door Lock\"{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-09-28.{/blu}"
	;
	HELP_STR h_plug_cable = "usage: plug_cable <id>\r\n"
			"description: plugs into the ethernet port identified by 'id'.\r\n"
			"{blu}Example: {yel}plug_cable A{/yel}\r\n"
			"\r\n"
			"{blu}this documentation was written on 2020-10-02.{/blu}"
	;
	HELP_STR h_edit_object = "usage: edit_object <id> <attribute> <value>\r\n"
			"description: attributes include: \r\n" 
			"\r\n"
			"wear_flag\r\n"
			"\r\n"
			"This command is not case-sensitive.\r\n"
			"\r\n"
			"example: edit_object 3 wear_flag PRIMARY SECONDARY TAKE\r\n"
			"\r\n"
			"To see a list of valid wear_flags, type list_wear_flags\r\n"
			"\r\n"
			"this documentation was written on 2020-06-26." 
	;
	HELP_STR h_room_dark = "usage: room_dark <on|off>\r\n"
		"description: \r\n" 
		"\r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-15."
	;
	HELP_STR h_room_fire = "usage: room_fire <on|off> [level]\r\n"
		"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-15."
	;
	HELP_STR h_set_ammo = "usage: set_ammo <weapon> <number>\r\n"
		"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-15."
	;
		HELP_STR h_plant_claymore = "usage: plant_claymore <direction>\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_penetrating_shot = "usage: penetrating_shot <target>\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_intimidate = "usage: intimidate <target>\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_toss_cryogenic_grenade = "usage: toss_cryogenic_grenade <direction> <rooms>\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_use_flash_underbarrel = "usage: use_flash_underbarrel\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_go_dark = "usage: go_dark\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_conceal = "usage: conceal <item>\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_feign_death = "usage: feign_death\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_summon_extraction = "usage: summon_extraction\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
		HELP_STR h_xray_shot = "usage: xray_shot\r\n"
			"description: \r\n"
		"example: \r\n"
		"\r\n"
		"this documentation was written on 2020-11-16."
		;
#undef HELP_STR
};

namespace mods::help {
	static std::map<std::string,std::string> registered_help_commands;
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
			M_MATCH("install_camera_feed",h_install_camera_feed);
			M_MATCH("uninstall_camera_feed",h_uninstall_camera_feed);
			M_MATCH("install_minigame",h_install_minigame);
			M_MATCH("list_minigame",h_list_minigame);
			M_MATCH("uninstall_minigame",h_uninstall_minigame);
			M_MATCH("plug_cable",h_plug_cable);
			M_MATCH("room_dark",h_room_dark);
			M_MATCH("room_fire",h_room_fire);
			M_MATCH("set_ammo",h_set_ammo);

			M_MATCH("plant_claymore",h_plant_claymore);
			M_MATCH("penetrating_shot",h_penetrating_shot);
			M_MATCH("intimidate",h_intimidate);
			M_MATCH("toss_cryogenic_grenade",h_toss_cryogenic_grenade);
			M_MATCH("use_flash_underbarrel",h_use_flash_underbarrel);
			M_MATCH("go_dark",h_go_dark);
			M_MATCH("conceal",h_conceal);
			M_MATCH("feign_death",h_feign_death);
			M_MATCH("summon_extraction",h_summon_extraction);
			M_MATCH("xray_shot",h_xray_shot);
#undef M_MATCH
			if(registered_help_commands[from.data()].length()){
				player->sendln(CAT("{blu}",registered_help_commands[from.data()],"{/blu}"));
				return false;
			}
			return true;
	}
	void register_help_command(const std::string& command, const std::string& contents){
		registered_help_commands[command] = contents;
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
	void send_class_header(player_ptr_t& player,std::string_view class_name){
		uint8_t len = class_name.length();
		uint8_t bar_count = len + 2 + strlen(" class menu =");
		std::string header = "";
		for(unsigned i=0; i < bar_count;++i){
			header += "=";
		}
		header += CAT("\r\n= ",class_name.data()," class menu =\r\n");
		for(unsigned i=0; i < bar_count;++i){
			header += "=";
		}
		player->sendln(header);
	}
	void send_class_footer(player_ptr_t& player,std::string_view class_name){
		uint8_t len = class_name.length();
		uint8_t bar_count = len + 2 + strlen(" class menu =");
		std::string header = "";
		for(unsigned i=0; i < bar_count;++i){
			header += "=";
		}
		player->sendln(header);
	}


	void send_contagion_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = {
		};
		send_class_header(player,"CONTAGION");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"CONTAGION");
	}
	void send_ghost_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = {
			"plant_claymore",
			"penetrating_shot",
			"intimidate",
			"toss_cryogenic_grenade",
			"use_flash_underbarrel",
		};
		send_class_header(player,"GHOST");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"GHOST");
	}
	void send_marksman_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"MARKSMAN");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"MARKSMAN");
	
	}
	void send_bandit_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"BANDIT");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"BANDIT");
	
	}
	void send_butcher_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"BUTCHER");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"BUTCHER");
	
	}
	void send_striker_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"STRIKER");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"STRIKER");
	
	}
	void send_obstructor_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"OBSTRUCTOR");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"OBSTRUCTOR");
	
	}
	void send_malady_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"MALADY");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"MALADY");
	
	}
	void send_pyrexia_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"PYREXIA");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"PYREXIA");
	
	}
	void send_dealer_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"DEALER");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"DEALER");
	
	}
	void send_forge_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"FORGE");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"FORGE");
	
	}
	void send_syndrome_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"SYNDROME");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"SYNDROME");
	
	}
	void send_machinist_help_menu(player_ptr_t& player){
		static std::vector<std::string> topics = { };
		send_class_header(player,"MACHINIST");
		for(auto topic : topics){
			player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
		}
		send_class_footer(player,"MACHINIST");
	
	}
	void send_generic_help(player_ptr_t& player){
	
	}
};
