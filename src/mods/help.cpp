#include "help.hpp"
#include <map>
#include <string>
#include "player.hpp"
#include "interpreter-include.hpp"
#include "filesystem.hpp"

namespace mods::help::pages {
#define HELP_STR static constexpr const char*
	HELP_STR h_practice=
	    "usage: practice <help>\r\n"
	    "usage: practice <skill-shorthand>\r\n"
	    "see also:\r\n"
	    " skills train\r\n"
	    ;
	HELP_STR h_sniper=
	    "{/grn}|--------------------------------------------------------------------------------|\r\n"
	    "{/grn}|   ____________________                                                         |\r\n"
	    "{/grn}|  |   S N I P I N G    |                                                        |\r\n"
	    "{/grn}|  |____________________|________________________________________________________|\r\n"
	    "{/grn}|  | Ability:     X-ray shot                                                     |\r\n"
	    "{/grn}|  | Command:     xray_shot <no-args>                                            |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: Hit a target through walls or doors. X-ray shot does not requi-|\r\n"
	    "{/grn}|  |              you to have your target within your line of sight. You can be  |\r\n"
	    "{/grn}|  |              several rooms away and still do damage to your target.         |\r\n"
	    "{/grn}|  |                                                                             |\r\n"
	    "{/grn}|  | Usage: xray_shot                                                            |\r\n"
	    "{/grn}|  |                                                                             |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  |   E X A M P L E                                                             |\r\n"
	    "{/grn}|  |   --------------------------------------------------------------------------|\r\n"
	    "{/grn}|  |   {grn}$>{/grn} mark Enforcer                                                          |\r\n"
	    "{/grn}|  |   {grn}::{/grn} CONFIRM :: Marked target                                               |\r\n"
	    "{/grn}|  |   {grn}$>{/grn} engage                                                                 |\r\n"
	    "{/grn}|  |   {grn}::{/grn} CALIBRATE :: Okay -: Target locked :-                                  |\r\n"
	    "{/grn}|  |   {grn}$>{/grn} xray_shot                                                              |\r\n"
	    "{/grn}|  |   You OBLITERATE A Military Police enforcer with your deadly snipe!!        |\r\n"
	    "{/grn}|  |   {grn}$>{/grn} disengage                                                              |\r\n"
	    "{/grn}|  |   {grn}::{/grn} DISENGAGED                                                             |\r\n"
	    "{/grn}|  |   {grn}$>{/grn} xray_shot                                                              |\r\n"
	    "{/grn}|  |   {grn}::{/grn} NO TARGET CALIBRATED                                                   |\r\n"
	    "{/grn}|  |                                                                             |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Ability:     Tracking Shot                                                  |\r\n"
	    "{/grn}|  | Command:     tracking_shot <target> <direction>                             |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: targets that are marked take extra damage.                     |\r\n"
	    "{/grn}|  |                                                                             |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Ability:     Mark Target                                                    |\r\n"
	    "{/grn}|  | Command:     mark <target>                                                  |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: For use with x-ray shot.                                       |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Ability:     Mark Target                                                    |\r\n"
	    "{/grn}|  | Command:     mark <target>                                                  |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: For use with x-ray shot.                                       |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Ability:     Attach frag grenade underbarrel                                |\r\n"
	    "{/grn}|  | Command:     attach_frag                                                    |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: Attaches a fragmentation grenade launcher to your primary.     |\r\n"
	    "{/grn}|  | Once you've attached the launcher, use {grn}fire <direction> <count>{/grn}             |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  |   E X A M P L E                                                             |\r\n"
	    "{/grn}|  |   --------------------------------------------------------------------------|\r\n"
	    "{/grn}|  |   {grn}$>{/grn} attach_frag                                                            |\r\n"
	    "{/grn}|  |   {grn}You attach an PWM Grenade Launcher to your primary weapon{/grn}                 |\r\n"
	    "{/grn}|  |   {grn}$>{/grn} fire north 3                                                           |\r\n"
	    "{/grn}|  |   {grn}You launch a fragmentation grenade way off to the north!{/grn}                  |\r\n"
	    "{/grn}|  |-----------------------------------------------------------------------------|\r\n"
	    "{/grn}|  | Description: For use with x-ray shot.                                       |\r\n"

	    "{grn}target_limb{/grn}: specify the limb to target.\r\n"
	    "  {grn}tracking_shot{/grn}: tracking your target.\r\n"
	    "  {yel}Usage:{/yel}{grn}tracking_shot <target> <direction>{/grn}\r\n"
	    "  {yel}Example:{/yel}\r\n"
	    "  {grn}$> tracking_shot enforcer east{/grn}\r\n"
	    "  {grn}You tag your target!{/grn}\r\n"
	    "  Tagged enemies take extra damage.\r\n"
	    "   ____________________ \r\n"
	    "  |      Healing       |\r\n"
	    "  |____________________|\r\n"
	    "  {grn}light_bandage{/grn}: use to regain some hp\r\n"
	    "  {grn}suture{/grn}: regain more hp than light_bandage but at the cost of movement points\r\n"
	    "  {grn}adrenaline_shot{/grn}: inject yourself with adrenaline and get +5 armor, +5 movement, and +3 strength\r\n"
	    "   ____________________ \r\n"
	    "  |     Demolitions    |\r\n"
	    "  |____________________|\r\n"
	    "  {grn}build_claymore{/grn}: creates a claymore in your inventory (if you have a charge left).\r\n"
	    "  {grn}build_shrapnel_claymore{/grn}: creates a {grn}shrapnel{/grn} claymore in your inventory (if you have a charge left).\r\n"
	    "  {grn}build_corrosive_claymore{/grn}: creates a {grn}corrosive{/grn} claymore in your inventory (if you have a charge left).\r\n"
	    "  {grn}guided_missile{/grn}: pre-program a guided missile to travel and detonate at the end of the path.\r\n"
	    "  {yel}Usage:{/yel}{grn}guided_missile <direction>...[direction]\r\n"
	    "  {yel}Example:{/yel}{grn}guided_missile n e e n n w{/grn}\r\n"
	    "  The above example will send a guided missile along the path until it reaches the last direction (west)\r\n"
	    "  upon which it will detonate in that room.\r\n"
	    "   ____________________ \r\n"
	    "  |     I N T E L      |\r\n"
	    "  |____________________|\r\n"
	    "{grn}build_emp{/grn}: creates an EMP grenade in your inventory (if you have a charge left).\r\n"
	    "{grn}build_chaff{/grn}: creates a Chaff grenade in your inventory (if you have a charge left).\r\n"
	    "{grn}build_sensor{/grn}: creates a Sensor grenade in your inventory (if you have a charge left).\r\n"
	    "{grn}request_recon{/grn}: creates a Sensor grenade in your inventory (if you have a charge left).\r\n"
	    "\r\n"
	    "{grn}attach_shotgun{/grn}: activate an underbarrel shotgun with 5 shells pre-loaded.\r\n"
	    "  {yel}Usage:{/yel}{grn}attach_shotgun{/grn}\r\n"
	    "  Once you've attached the shotgun, the next same-room fire fight you're in will use the underbarrel\r\n"
	    "  shotgun until it's ammo is depleted. Once the shotgun's ammo is depleted, you will switch back to your\r\n"
	    "  primary weapon's main attack.\r\n"
	    "{grn}attach_frag{/grn}: activate an underbarrel grenade launcher with 3 grenades pre-loaded.\r\n"
	    "  {yel}Usage:{/yel}{grn}attach_frag{/grn}\r\n"
	    "  Once you've attached the launcher, use {grn}fire <direction> <count>{/grn}\r\n"
	    "  {yel}Example:{/yel}\r\n"
	    "  {grn}$> attach_frag{/grn}\r\n"
	    "  {grn}You attach an PWM Grenade Launcher to your primary weapon{/grn}\r\n"
	    "  {grn}$> fire north 3{/grn}\r\n"
	    "  {grn}You launch a fragmentation grenade way off to the north!{/grn}\r\n"
	    "{grn}target_limb{/grn}: specify the limb to target.\r\n"
	    "  {yel}Usage:{/yel}{grn}target_limb <right-arm|left-arm|none>\r\n"
	    "  {yel}Example:{/yel}\r\n"
	    "  {grn}$> target_limb right-arm{/grn}\r\n"
	    "  {grn}You start targeting the right arm of your opponents.{/grn}\r\n"
	    "  {grn}$> snipe Enforcer west{/grn}\r\n"
	    "  {grn}You OBLITERATE A Military Police enforcer with your deadly snipe!!{/grn}\r\n"
	    "  {grn}*** LIMB DAMAGE (right-arm) [ INTEGRITY: POOR ] ***\r\n"
	    "  {grn}$> snipe Enforcer west{/grn}\r\n"
	    "  {grn}You OBLITERATE A Military Police enforcer with your deadly snipe!!{/grn}\r\n"
	    "  {grn}*** LIMB DAMAGE (right-arm) [ INTEGRITY: USELESS ] ***\r\n"
	    "  \r\n"
	    "  Once a limb reaches {yel}USELESS{/yel}, the limb is effectively destroyed.\r\n"
	    "  \r\n"
	    "  | A note on handed-ness                                                                                |\r\n"
	    "  |------------------------------------------------------------------------------------------------------|\r\n"
	    "  | Every NPC is right-handed by default. An NPC uses their left hand to utilize a secondary weapon such |\r\n"
	    "  | as a pistol. NPC's will use their right hand to utilize their primary weapon. This goes for rifles   |\r\n"
	    "  | and melee weapons.                                                                                   |\r\n"
	    "  |------------------------------------------------------------------------------------------------------|\r\n"
	    "  \r\n"
	    "  ----------------------------------------------------------------\r\n"
	    "  | Limb damage and effects on {grn}MELEE{/grn} primary weapon              |\r\n"
	    "  +--------------------------------------------------------------+\r\n"
	    "  | LIMB           | EFFECTS                                     |\r\n"
	    "  |----------------+---------------------------------------------|\r\n"
	    "  | Right arm:     | 1 or more of the below will occur...        |\r\n"
	    "  |                | * 30\% reduced chance to hit target         |\r\n"
	    "  |                | * 1l\% damage reduction for 33 ticks        |\r\n"
	    "  |                | * reduce dice sides by 10-25\% for 45 ticks |\r\n"
	    "  |----------------+---------------------------------------------|\r\n"
	    "  | Left arm:      | 1 or more of the below will occur...        |\r\n"
	    "  |                | * next 3 attacks do half damage             |\r\n"
	    "  |----------------+---------------------------------------------|\r\n"
	    "  | Limb damage and effects on {grn}RIFLE{/grn} primary weapon              |\r\n"
	    "  +----------------+---------------------------------------------+\r\n"
	    "  | LIMB           | EFFECTS                                     |\r\n"
	    "  |----------------|---------------------------------------------|\r\n"
	    "  | Right arm:     | 1 or more of the below will occur...        |\r\n"
	    "  |                | * 25\% reduced damage with primary weapon   |\r\n"
	    "  |                | * 33\% chance of jamming primary weapon     |\r\n"
	    "  |                | * 40\% penalty to accuracy                  |\r\n"
	    "  |                | * 20\% chance of dropping primary weapon    |\r\n"
	    "  |                | * 14\% chance of primary magazine emptying  |\r\n"
	    "  |----------------|---------------------------------------------|\r\n"
	    "  | Left arm:      | 1 or more of the below will occur...        |\r\n"
	    "  |                | * 35\% less likely to deal critical shots   |\r\n"
	    "  |                | * add 15 ticks to cooldown between shots    |\r\n"
	    "  |                | * 25\% damage reduction for 60 ticks        |\r\n"
	    "  |----------------|---------------------------------------------|\r\n"
	    "{grn}xray_shot{/grn}: specify the limb to target.\r\n"
	    "\r\n";

	HELP_STR h_contract = "usage: contract <list>\r\n"
	                      "usage: contract <join> <N>\r\n"
	                      "usage: contract <leave> <N>\r\n"
	                      "usage: contract <step>\r\n"
	                      "usage: contract <current>\r\n"
	                      "usage: contract <show|describe> <N>\r\n"
	                      "This documentation was written on 2021-06-06."
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
	HELP_STR h_reload = "usage: reload [primary|secondary]\r\n"
	                    "description: reloads either your primary or secondary. If neither is supplied will automatically reload your primary.\r\n"
	                    "example: reload primary\r\n"
	                    "example: reload # this is equivalent to {grn}reload primary{/grn}\r\n"
	                    "example: reload secondary\r\n"
	                    "\r\n"
	                    "this documentation was written on 2020-11-16."
	                    ;
	HELP_STR h_pathogen_ammunition \
	    = "usage: cast 'pathogen ammunition' [primary|secondary]\r\n"
	      "description: Loads a special magazine into your primary or secondary weapon.\r\n"
	      "Pathogen ammunition infects your ammunition with a genetically engineered\r\n"
	      "virus that continues to deal poison damage to a target for a period of time.\r\n"
	      "example: cast 'pathogen ammunition' primary\r\n"
	      "example: cast 'pathogen ammunition' # this is equivalent to {grn}cast 'pathogen ammunition' primary{/grn}\r\n"
	      "\r\n"
	      "this documentation was written on 2021-09-25."
	      ;
	HELP_STR grim_aura= "usage: cast 'grim aura'   \r\n"
	                    "description: \r\n"
	                    "- +15% damage done by melee/ranged attacks\r\n"
	                    "- damage taken is reduced by 15%\r\n"
	                    "- Player gets 15% of their max hp added to their current hp\r\n"
	                    "\r\n"
	                    "this documentation was written on 2021-09-25.";

	HELP_STR melt= "usage: cast 'melt' <target>  \r\n"
	               "- Player places hands on target\r\n"
	               "- Target is set on fire and blinded for 50 ticks\r\n"
	               "\r\n"
	               "this documentation was written on 2021-09-25.";

	HELP_STR suffocate= "usage: cast 'suffocate' <target>  \r\n"
	                    "- Player summons a rope around target's neck\r\n"
	                    "- Target continues to suffocate, losing HP\r\n"
	                    "\r\n"
	                    "this documentation was written on 2021-09-25.";
	HELP_STR shredded_cantrip= "usage:     \r\n"
	                           "- Shredded cantrip\r\n"
	                           "- Place a trap in the room that causes targets to bleed\r\n"
	                           "\r\n"
	                           "this documentation was written on 2021-09-25.";
	HELP_STR muscle_memory= "usage: cast 'muscle memory'  \r\n"
	                        "- Muscle Memory\r\n"
	                        "- Once you die\r\n"
	                        " You can order your corpse to explode\r\n"
	                        "\r\n"
	                        "this documentation was written on 2021-09-25.";
	HELP_STR hellfire_circle= "usage: cast 'hellfire circle'   \r\n"
	                          "- Hellfire Circle\r\n"
	                          "	- Reduces incendiary, explosive damage\r\n"
	                          "	- Reduces CQC, melee damage by 25%\r\n"
	                          "\r\n"
	                          "this documentation was written on 2021-09-25.";
	HELP_STR particle_deceleration= "usage: cast 'particle deceleration'\r\n"
	                                "- Particle Deceleration\r\n"
	                                "	- Create an aura around player that slows the velocity of attacks\r\n"
	                                "		- Reduces damage done by ranged weapons\r\n"
	                                "\r\n"
	                                "this documentation was written on 2021-09-25.";
	HELP_STR ghastly_double= "usage:     \r\n"
	                         "- Ghastly double\r\n"
	                         "	- Create an illusion that there are two of you\r\n"
	                         "	- Chance of taking damage is reduced by 30%\r\n"
	                         "\r\n"
	                         "this documentation was written on 2021-09-25.";
	HELP_STR minor_shielding= "usage: cast 'minor shielding'   \r\n"
	                          "- Minor shielding\r\n"
	                          "	- Create a ballistic resistant shielding around self\r\n"
	                          "\r\n"
	                          "this documentation was written on 2021-09-25.";
	HELP_STR cursed_ballistics= "usage: cast 'cursed ballistics'    \r\n"
	                            "- Cursed Ballistics\r\n"
	                            "	- Worn armor becomes more effective at the cost of movement points\r\n"
	                            "\r\n"
	                            "this documentation was written on 2021-09-25.";
	HELP_STR neutron_shield= "usage: cast 'neutron shield'\r\n"
	                         "- Neutron Shield\r\n"
	                         "	- Creates a shield that\r\n"
	                         "		- dampens radioactive, cryogenic, and anti-matter damage\r\n"
	                         "	- Each bullet absorbed while Neutron Shield is active becomes a radioactive charge\r\n"
	                         "	- Radioactive charges can be released as a ranged attack but only while NS is active\r\n"
	                         "\r\n"
	                         "this documentation was written on 2021-09-25.";
	HELP_STR bladed_array= "usage: cast 'bladed array'  \r\n"
	                       "- Bladed Array\r\n"
	                       "	- Each piece of worn armor causes bladed knife damage to melee attackers\r\n"
	                       "	- CQC attacks:\r\n"
	                       "		- throw, wrestle, grab, clinch\r\n"
	                       "		- cause HP damage to attacker\r\n"
	                       "\r\n"
	                       "this documentation was written on 2021-09-25.";
	HELP_STR roots_of_mayhem= "usage: cast 'roots_of_mayhem' <target> <direction>\r\n"
	                          "- Roots of mayhem\r\n"
	                          "	- Cause the dead to reach up from beneath a target\r\n"
	                          "	- Target is stuck in place and can only use ranged attacks\r\n"
	                          "	- Target cannot move or flee until spell is over\r\n"
	                          "\r\n"
	                          "this documentation was written on 2021-09-25.";

	HELP_STR morbid_doubt= "usage: cast 'morbid doubt' <target>\r\n"
	                       "- Morbid doubt\r\n"
	                       "	- Forces target to turn their weapon against themselves\r\n"
	                       "\r\n"
	                       "this documentation was written on 2021-09-25.";
	HELP_STR intimidate= "usage: cast 'intimidate' <target>\r\n"
	                     "- Intimidate\r\n"
	                     "	- Target becomes fearful/intimidated for 33 ticks\r\n"
	                     "		- A fearful target is afraid to attack or respond to attacks\r\n"
	                     "\r\n"
	                     "this documentation was written on 2021-09-25.";
	HELP_STR force_out= "usage: cast 'force out' <target> <direction>\r\n"
	                    "- Force out\r\n"
	                    "	- Force target to move in a specific direction\r\n"
	                    "\r\n"
	                    "this documentation was written on 2021-09-25.";
	HELP_STR leech= "usage: cast 'leech' <target> <direction>\r\n"
	                "- Leech\r\n"
	                "	- Fire several devices at target.\r\n"
	                "	- Target gets BLEED for 30 ticks\r\n"
	                "		- Player gets hp equal to the bleed damage\r\n"
	                "\r\n"
	                "this documentation was written on 2021-09-25.";
	HELP_STR parasitic_corpse_tap= "usage: cast 'parasitic corpse tap' <target>\r\n"
	                               "- Parasitic Corpse Tap\r\n"
	                               "	- Walk up to any corpse and absorb hp,mana,movement points\r\n"
	                               "	- Corpse dissipates once done\r\n"
	                               "\r\n"
	                               "this documentation was written on 2021-09-25.";
	HELP_STR extract_organs= "usage: cast 'extract_organs' <target>\r\n"
	                         "- Extract Organs\r\n"
	                         "	- Player dissects a corpse and consumes it's organs\r\n"
	                         "		- Gains HP, Mana, Movement equal to 25% of the \r\n"
	                         "			HP, Mana, and Movement of the NPC or PC that died\r\n"
	                         "\r\n"
	                         "this documentation was written on 2021-09-25.";
	HELP_STR recruit= "usage: cast 'recruit' <target> \r\n"
	                  "- Recruit\r\n"
	                  "	- Walk up to any corpse\r\n"
	                  "	- Place hex on corpse\r\n"
	                  "	- Add 1 demonic incantation charge for every 2 corpse this is done to\r\n"
	                  "	- Corpse dissipates once done\r\n"
	                  "\r\n"
	                  "this documentation was written on 2021-09-25.";

	HELP_STR demonic_incantation= "usage:     \r\n";
	HELP_STR shadow_sight= "usage:     \r\n";
	HELP_STR morbid_insight= "usage:     \r\n";
	HELP_STR life_tap= "usage:     \r\n";
	HELP_STR corpse_explosion= "usage:     \r\n";
	HELP_STR attract= "usage:     \r\n";
	HELP_STR confuse= "usage:     \r\n";
	HELP_STR hellfire_incantation= "usage:     \r\n";
	HELP_STR detonate_limb= "usage:     \r\n";
};
#undef HELP_STR
};

namespace mods::help {
	void fetch_mortal_help(std::vector<std::string>& screen) {
		std::string guts,error;
		mods::filesystem::file_get_contents("../lib/ACMD.list", guts,error);
		std::string current = "";
		for(auto ch : guts) {
			if(ch == '\n') {
				screen.emplace_back(current);
				current.clear();
				continue;
			}
			current += ch;
		}
	}
	void fetch_builder_help(std::vector<std::string>& screen) {
		static const std::vector<std::string> builder_screen = {
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Cameras ]:-                               {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}giveme_camera{/gld} -- {grn}give me deployable camera [feature-debug]{/grn}",
			"{gld}giveme_night_vision_camera{/gld} -- {grn}give me a night vision camera{/grn}",
			"{gld}giveme_thermal_camera{/gld} -- {grn}give me a thermal camera {/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Grenades ]:-                              {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}giveme_flash{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_frag{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_inc{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}giveme_sensor{/gld} -- {grn}give me sensor grenades [feature-debug]{/grn}",
			"{gld}giveme_smoke{/gld} -- {grn}give me frag grenades [feature-debug]{/grn}",
			"{gld}givemegold{/gld} -- {grn}give me gold [feature-debug]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Building ]:-                              {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			//"{gld}cambuild{/gld} -- {grn}create surveilance cameras that change their feed [builder-utils]{/grn}",
			"{gld}list_minigame{/gld} -- {grn}lists minigames currently installed in the room [builder-utils]{/grn}",
			"{gld}install_minigame{/gld} -- {grn}installs a minigame in the room [builder-utils]{/grn}",
			"{gld}uninstall_minigame{/gld} -- {grn}uninstalls a minigame in the room [builder-utils]{/grn}",
			"{gld}mbuild{/gld} -- {grn}mob builder [builder-utils]{/grn}",
			"{gld}obuild{/gld} -- {grn}object builder [builder-utils]{/grn}",
			"{gld}sbuild{/gld} -- {grn}shop builder [builder-utils]{/grn}",
			"{gld}rbuild{/gld} -- {grn}room builder [builder-utils]{/grn}",
			"{gld}rbuild_sandbox{/gld} -- {grn}room builder sandbox utility [builder-utils]{/grn}",
			"{gld}rnumlist{/gld} -- {grn}list rooms [feature-debug]{/grn}",
			"{gld}rnumtele{/gld} -- {grn}teleport to a room [feature-debug]{/grn}",
			"{gld}room_dark{/gld} -- {grn}usage: room_dark on, room_dark off [builder-utils][admin-utils]{/grn}",
			"{gld}room_fire{/gld} -- {grn}usage: room_fire <on|off> [level] [builder-utils][admin-utils]{/grn}",
			"{gld}room_list{/gld} -- {grn}lists rooms [builder-utils][admin-utils]{/grn}",
			"{gld}yaml_example{/gld} -- {grn}type 'yaml_example list' for a list of yaml files{/grn}",
			"{gld}yaml_import{/gld} -- {grn}type 'yaml_import <filename>' to import a yaml file{/grn}",
			"{gld}yaml_log{/gld} -- {grn}get the yaml log to find out which files failed to import{/grn}",
			"{gld}yaml_log_save{/gld} -- {grn}write the yaml log to disk{/grn}",
			"{gld}yaml_log_clear{/gld} -- {grn}clear the yaml log without flushing to disk{/grn}",
			"{gld}zbuild{/gld} -- {grn}zone builder [builder-utils]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Ammunition ]:-                            {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}ammo{/gld} -- {grn}give yourself ammo [feature-debug]{/grn}",
			"{gld}empty_clip{/gld} -- {grn}empty the clip in your primary weapon [feature-debug]{/grn}",
			"{gld}set_ammo{/gld} -- {grn}set ammo of currently wielded weapon{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                    -:[ Damage debugging ]:-                        {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}affect_me{/gld} -- {grn}give yourself an affect <BLIND|DISORIENT|POISON> [feature-debug]{/grn}",
			"{gld}heal{/gld} -- {grn}heal yourself [feature-debug][staging-feature][class-medic]{/grn}",
			"{gld}one_punch{/gld} -- {grn}immediately kill your currently fighting char with one punch{/grn}",
			"{gld}point_update{/gld} -- {grn}manually call point update [feature-debug]{/grn}",
			"{gld}set_position{/gld} -- {grn}set position to one of the POS_* constants [feature-debug]{/grn}",
			"",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Debugging ]:-                             {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}flush_player{/gld} -- {grn}calls the default flush_player method [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}feed_player{/gld} -- {grn}calls the default feed_player method [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}flush_holding{/gld} -- {grn}flush the item you are holding to the db [feature-debug][staging-feature][builder-utils]{/grn}",
			"{gld}get_ticks_per_minute{/gld} -- {grn}get ticks per minute [feature-debug]{/grn}",
			"{gld}histfile{/gld} -- {grn}start recording all commands. stop with 'histfile stop' [builder-utils][feature-debug]{/grn}",
			"{gld}idle{/gld} -- {grn}force your character into idle state [feature-debug]{/grn}",
			"{gld}js{/gld} -- {grn}Run javascript [feature-debug][admin-utils]{/grn}",
			"{gld}js_help{/gld} -- {grn}Show useful js commands [builder-utils][admin-utils]{/grn}",
			"{gld}jstest{/gld} -- {grn}run a javascript test [builder-utils][admin-tools]{/grn}",
			"{gld}newjs{/gld} -- {grn}create a new javascript context [admin-tools][feature-debug]{/grn}",
			"{gld}pref{/gld} -- {grn}preferences utility [staging-feature]{/grn}",
			"{gld}room_list_uuid{/gld} -- {grn}list uuids and names of everyone in the same room [builder-utils]{/grn}",
			"{gld}show_tics{/gld} -- {grn}toggle tics[builder-utils]{/grn}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Character Generation ]:-                  {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}disable_registration{/gld} -- {grn}yup[builder-utils]{/grn}",
			"{gld}enable_registration{/gld} -- {grn}yup[builder-utils]{/grn}",
			"{gld}registration_status{/gld} -- {grn}prints allowed or not allowed.[builder-utils]{/grn}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{yel} --                      -:[ Values System ]:-                         {/yel}",
			"{yel} ----------------------------------------------------------------------{/yel}",
			"{gld}disable_registration{/gld} -- {grn}yup[builder-utils]{/grn}",
			"{gld}revert_value_to_default{/gld} -- {grn}revert a value back to factory default{/grn}",
			"{gld}list_values{/gld} -- {grn}list all available values{/grn}",
			"{gld}list_value{/gld} -- {grn}alias of list_values{/grn}",
			"{gld}set_value{/gld} -- {grn}set value to specific value{/grn}",
			"{gld}get_value{/gld} -- {grn}display a specific value{/grn}",
		};
		for(const auto& line : builder_screen) {
			screen.emplace_back(line);
		}
		std::string guts,error;
		mods::filesystem::file_get_contents("../lib/SUPERCMD.list", guts,error);
		std::string current = "";
		for(auto ch : guts) {
			if(ch == '\n') {
				screen.emplace_back(current);
				current.clear();
				continue;
			}
			current += ch;
		}
	}
	static std::map<std::string,std::pair<player_level,std::string>> registered_help_commands;
	static std::map<std::string,std::string> registered_admin_help_commands;

	bool matches_many(const std::string& items,std::string_view from) {
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(items);
		while(std::getline(tokenStream, token, ',')) {
			if(token.compare(from.data()) == 0) {
				return true;
			}
		}
		return false;
	}


	bool send_help(std::string_view topic, std::shared_ptr<mods::player>& player) {
		auto it = registered_help_commands.find(topic.data());
		if(registered_help_commands.end() != it && ((int)player->level()) >= ((int)it->second.first)) {
			player->send("your player level: %d, second.first: %d\r\n",player->level(),it->second.first);
			player->sendln(CAT("{blu}",it->second.second,"{/blu}"));
			return false;
		}
#define M_MATCH(A,SUBJECT) if(matches_many(A,topic)){ player->sendln(CAT(pages::SUBJECT)); return false; }
		M_MATCH("practice",h_practice);
		M_MATCH("sniper",h_sniper);
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
		M_MATCH("reload",h_reload);
		M_MATCH("pathogen_ammunition",h_pathogen_ammunition);
		M_MATCH("grim_aura",grim_aura);
		M_MATCH("melt",melt);
		M_MATCH("suffocate",suffocate);
		M_MATCH("shredded_cantrip",shredded_cantrip);
		M_MATCH("muscle_memory",muscle_memory);
		M_MATCH("hellfire_circle",hellfire_circle);
		M_MATCH("particle_deceleration",particle_deceleration);
		M_MATCH("ghastly_double",ghastly_double);
		M_MATCH("minor_shielding",minor_shielding);
		M_MATCH("cursed_ballistics",cursed_ballistics);
		M_MATCH("neutron_shield",neutron_shield);
		M_MATCH("bladed_array",bladed_array);
		M_MATCH("roots_of_mayhem",roots_of_mayhem);
		M_MATCH("morbid_doubt",morbid_doubt);
		M_MATCH("intimidate",intimidate);
		M_MATCH("force_out",force_out);
		M_MATCH("leech",leech);
		M_MATCH("parasitic_corpse_tap",parasitic_corpse_tap);
		M_MATCH("extract_organs",extract_organs);
		M_MATCH("recruit",recruit);
		M_MATCH("demonic_incantation",demonic_incantation);
		M_MATCH("shadow_sight",shadow_sight);
		M_MATCH("morbid_insight",morbid_insight);
		M_MATCH("life_tap",life_tap);
		M_MATCH("corpse_explosion",corpse_explosion);
		M_MATCH("attract",attract);
		M_MATCH("confuse",confuse);
		M_MATCH("hellfire_incantation",hellfire_incantation);
		M_MATCH("detonate_limb",detonate_limb);
	};
#undef M_MATCH
	return true;
}



void register_help_command_with_permission(const std::string& command, const std::string& contents,player_level level) {
	registered_help_commands[command] = {level,contents};
}


bool should_continue(std::string_view from,std::string_view argument, std::shared_ptr<mods::player>& player,bool zero_is_help) {
	bool show = false;
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0 && zero_is_help) {
		show = true;
	}
	if(!show && !mods::util::parse_help(argument)) {
		return true;
	}
	return send_help(from,player);
}


void send_class_header(player_ptr_t& player,std::string_view class_name) {
	uint8_t len = class_name.length();
	uint8_t bar_count = len + 2 + strlen(" class menu =");
	std::string header = "";
	for(unsigned i=0; i < bar_count; ++i) {
		header += "=";
	}
	header += CAT("\r\n= ",class_name.data()," class menu =\r\n");
	for(unsigned i=0; i < bar_count; ++i) {
		header += "=";
	}
	player->sendln(header);
}


void send_class_footer(player_ptr_t& player,std::string_view class_name) {
	uint8_t len = class_name.length();
	uint8_t bar_count = len + 2 + strlen(" class menu =");
	std::string header = "";
	for(unsigned i=0; i < bar_count; ++i) {
		header += "=";
	}
	player->sendln(header);
}

void send_breacher_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = {
		"Special Ability - To use your explosive shot, simply move toward a closed door or surface twice. The first time you "
		" move toward the door/surface, it will tell you to move that direction once more to explode it.\r\n"
	};
	send_class_header(player,"BREACHER");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"BREACHER");
}

void send_contagion_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = {
		"Pathogen Ammunition - help pathogen_ammunition",
		"Grim Aura - help grim_aura",
		"Melt - help melt",
		"Suffocate - help suffocate",
		"Shredded Cantrip - help shredded_cantrip",
		"Muscle Memory - help muscle_memory",
		"Hellfire Circle - help hellfire_circle",
		"Particle Deceleration - help particle_deceleration",
		"Ghastly Double - help ghastly_double",
		"Minor Shielding - help minor_shielding",
		"Cursed Ballistics - help cursed_ballistics",
		"Neutron Shield - help neutron_shield",
		"Bladed Array - help bladed_array",
		"Roots of Mayhem - help roots_of_mayhem",
		"Morbid Doubt - help morbid_doubt",
		"Intimidate - help intimidate",
		"Force Out - help force_out",
		"Leech - help leech",
		"Parasitic Corpse Tap - help parasitic_corpse_tap",
		"Extract Organs - help extract_organs",
		"Recruit - help recruit",
		"Demonic Incantation - help demonic_incantation",
		"Shadow Sight - help shadow_sight",
		"Morbid Insight - help morbid_insight",
		"Life Tap - help life_tap",
		"Corpse Explosion - help corpse_explosion",
		"Shrapnel Corpse Explosion - help corpse_explosion",
		"Corrosive Corpse Explosion - help corpse_explosion",
		"Hellfire Corpse Explosion - help corpse_explosion",
		"Attract - help attract",
		"Confuse - help confuse",
		"Hellfire Incantation - help hellfire_incantation",
		"Detonate Limb - help detonate_limb",
	};
	send_class_header(player,"CONTAGION");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"CONTAGION");
}
void send_ghost_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = {
		"plant_claymore",
		"penetrating_shot",
		"intimidate",
		"toss_cryogenic_grenade",
		"use_flash_underbarrel",
	};
	send_class_header(player,"GHOST");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"GHOST");
}
void send_marksman_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"MARKSMAN");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"MARKSMAN");

}
void send_bandit_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"BANDIT");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"BANDIT");

}
void send_butcher_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"BUTCHER");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"BUTCHER");

}
void send_striker_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"STRIKER");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"STRIKER");

}
void send_obstructor_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"OBSTRUCTOR");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"OBSTRUCTOR");

}
void send_malady_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"MALADY");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"MALADY");

}
void send_pyrexia_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"PYREXIA");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"PYREXIA");

}
void send_dealer_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"DEALER");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"DEALER");

}
void send_forge_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"FORGE");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"FORGE");

}
void send_syndrome_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"SYNDROME");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"SYNDROME");

}
void send_machinist_help_menu(player_ptr_t& player) {
	static std::vector<std::string> topics = { };
	send_class_header(player,"MACHINIST");
	for(auto topic : topics) {
		player->send(CAT("{grn}",topic,"{/grn}\r\n").c_str());
	}
	send_class_footer(player,"MACHINIST");

}


ACMD(do_help) {
	if(IS_NPC(ch)) {
		return;
	}
	std::vector<std::string> screen;

	mods::help::fetch_mortal_help(screen);
	auto vec_args = PARSE_ARGS();
	if(vec_args.size() == 0) {
		switch(player->get_class()) {
			case player_class_t::CLASS_CONTAGION:
				mods::help::send_contagion_help_menu(player);
				break;
			case player_class_t::CLASS_GHOST:
				mods::help::send_ghost_help_menu(player);
				break;
			case player_class_t::CLASS_MARKSMAN:
				mods::help::send_marksman_help_menu(player);
				break;
			case player_class_t::CLASS_BANDIT:
				mods::help::send_bandit_help_menu(player);
				break;
			case player_class_t::CLASS_BUTCHER:
				mods::help::send_butcher_help_menu(player);
				break;
			case player_class_t::CLASS_STRIKER:
				mods::help::send_striker_help_menu(player);
				break;
			case player_class_t::CLASS_OBSTRUCTOR:
				mods::help::send_obstructor_help_menu(player);
				break;
			case player_class_t::CLASS_MALADY:
				mods::help::send_malady_help_menu(player);
				break;
			case player_class_t::CLASS_PYREXIA:
				mods::help::send_pyrexia_help_menu(player);
				break;
			case player_class_t::CLASS_DEALER:
				mods::help::send_dealer_help_menu(player);
				break;
			case player_class_t::CLASS_FORGE:
				mods::help::send_forge_help_menu(player);
				break;
			case player_class_t::CLASS_SYNDROME:
				mods::help::send_syndrome_help_menu(player);
				break;
			case player_class_t::CLASS_MACHINIST:
				mods::help::send_machinist_help_menu(player);
				break;
			default:
				break;
		}
	}

	if(player->implementor_mode() || player->builder_mode()) {
		mods::help::fetch_builder_help(screen);
	}

	if(argshave()->size_gt(0)->passed()) {
		if(send_help(argat(0), player)) {
			player->pager_start();
			player->sendln("No specific help screen found for that. Searching every command...");
			mods::search_screen<player_ptr_t>(player,screen,vec_args,64);
			player->pager_end();
			player->page(0);
			return;
		}
	}
}

void init() {
	mods::interpreter::add_command("builder_help", POS_RESTING, do_help, LVL_BUILDER,0);
	mods::interpreter::add_command("help", POS_RESTING, do_help, 0,0);
}
};
