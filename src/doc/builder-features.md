# Inventory of features (mods directory only)

# 2020-12-04 
	- finding player by description relative to player
		- source file: mods/examine.cpp
		- `mods::examine::find_player_by_name(player_ptr_t& player, std::string_view arg)`
	- boolean support added to dynamic values system
		- source file(s): mods/values.(c|h)pp
	- player level mocking
		- `mock_level <int>`
			- example: `mock_level 30`
		- this is switched on or off using values system: `ALLOW_MOCK_LEVELS`
		- source file: mods/classes/super-user-fiddler.cpp
	- replenish values now powered by your player tier
	- sniper class
		- special ability 'xray_shot' now ready
			- needs balancing
	- breacher class 
		- special ability 'explosive shot' now ready
			- needs balancing
	- ghost class
		- special ability 'dissipate' now ready
			- needs balancing
	- engineer class
		- special ability 'drone_bomb' now ready
		- example: `drone_bomb north 2`

# 2020-11-29 classes system rework
	Up until today, there has been no definite scope to the player classes. It has all been one
	big blur of adding random features with no clear decision as to the definite direction
	of the game. I am taking the time now to describe the new class system which in turn
	also clearly defines the scope of this project. 

	For a little bit of background: I started this project with the idea of creating a 
	MUD that was inspired by Tom Clancy's Rainbow Six Siege. I have deviated from that
	initial goal. I have implemented lots of features but nothing cohesive enough to
	gaurantee a good multiplayer experience. That ends here. The following class
	system is set in stone and will not change.

# 2020 Class System Scope Revision
	Each player class consists of the following configuration:
	- Primary Weapon
	- Secondary Weapon
	- Tactical
	- Special Ability

	Each player class cannot swap their weapon out for another weapon.
	However, a weapon can have various attachments and upgrades.
	The PC weapon levels up as the player levels up.

	Base attributes for each PC all start off the same, however, the
	Speed/Armor triads can be configured by player upon character generation.

	This MUD is COMBAT-BASED, which makes the required features easier to
	tackle. It is very tempting to attempt to write a MUD that rewards the
	player for being stealthy/sneaky/clever, but that is very hard to get
	correct, and it complicates the reward/leveling system.

	Each loadout is damage based. The loadouts are meant to deal damage
	and are chosen specifically for each class to fit that PC's play style.
	For example, the Sniper class should have a DMR. Equipping the Sniper
	with a shotgun primary would make no sense.

	Each grenade type is coupled with the play style of each PC.

	The special ability of each PC is damage based as well. The ability
	should either prevent or enhance damage done/taken.

# 2020-11-29 REVISED CLASS SYSTEM

# SNIPER
	- Loadout:
		- Primary: PSG-1 Sniper Rifle
		- Secondary: CZP10 Pistol
		- Tactical: Claymore Mine
		- Ability: XRay Shot [ DONE ]
			- Command sequence:
				- Type 'mark <name>' to mark an opponent
				- Type 'engage' you will now specifically target only that mob
				- Everytime you type 'xray_shot' it will fire off a shot until you're out of charges
			- To disengage, type 'disengage'
			- Once target is dead, you no longer are in engage mode and have to mark/engage/xray_shot again
# MARINE
	- Loadout:
		- Primary: M4 Assault Rifle
		- Secondary: Magnum Revolver (exact type TBD)
		- Tactical: Fragmentation Grenade
		- Ability: Activate Underbarrel grenade launcher (x3 grenade rounds)
			- Command sequence
				- Type 'underbarrel activate'
				- Type 'snipe <target> <direction>'
			- Causes massive explosive damage to a room very far away
# BREACHER
	- Loadout:
		- Primary: MP-5 Sub Machine Gun
		- Secondary: SASG-12 Shotgun
			- Every 4th shot gains 1 push back charge
		- Tactical: Flashbang
		- Ability: Explosive shot [ DONE 2020-12-04 ]
			- Shot can breach doors/walls/surfaces, creating new tiles on the map
			- Same effect as a breach charge but without having to use one
# ENGINEER
	- Loadout:
		- Primary: UMP-45
		- Secondary: Glock
		- Tactical: Sensor Grenade
		- Ability: Drone Bomb [ DONE ]
			- Deploy a drone that drives in a direction N rooms
			- Example: `drone_bomb north 2`
			- Drone bomb is a remote explosive type stored at `objects/explosive/engineer-drone-bomb.yml`
				- Drone bomb is a globally available singleton guarded and accessible only through a function
# MEDIC
	- Loadout:
		- Primary: SAIGA-12 Automatic Shotgun
			- Every 3rd shot adds 1 push back charge
		- Secondary: MP9 Machine Pistol
		- Tactical: Smoke Grenade
		- Ability: Drop Healing Grenade
			- Team members can heal more by typing 'siphon' in the room a healing grenade exists in
# GHOST
	- Loadout:
		- Primary: Silenced G36C Assault Rifle
		- Secondary: Silenced PPK Pistol
		- Tactical: Smoke Grenade
		- Ability: Dissipate
			- Disappear and move throughout the map without being noticed [ DONE 2020-12-04 ]
			- Traps, doors, walls do not affect you [ DONE 2020-12-05 ]
			- Enemies cannot see or fire at you [ TODO ]
# SUPPORT
	- Loadout:
		- Primary: HK21 Light Machine Gun
		- Secondary: 
		- Tactical: Incendiary Grenade
		- Ability: Call in drone strike
			- Drones drop hellfire missiles on target room
# END OF 2020-11-29 REVISED CLASS SYSTEM



# sql 'up' migrations
	- run with: `./bin/circle --run-migration-up=<identifier>`
# sql 'down' migrations
  - run with: `./bin/circle --run-migration-down=<identifier>`

# help files
	- help command
		- now displays context related info about the user's class
		- can now search various help files
			- example: help throw
			- using help by itself will show you your class help file and the generic help file
			- example: help

# values system
	- source file: mods/values.cpp
	- usage: set_value <key> <value>
	- get_value <key>...<keyN>
	- revert_value_to_default <arg>...<argN>
	- list_values [searchable]

# armor lockers
	- install_armor_locker <yaml>...<yaml-N>
	- uninstall_armor_locker
	- quotas:
		- armor_locker_quota <integer>
	- source file: mods/integral-objects.cpp
	- replenish source file: mods/zone.cpp
		- TODO: use query container source code to keep quota up
	- feed source file: mods/integal-objects.cpp
	- db source file: mods/integral-objects-db.cpp

# weapon lockers
	- install_weapon_locker <yaml>...<yaml-N>
	- weapon_armor_locker
	- quotas:
		- weapon_locker_quota <integer>
	- source file: mods/integral-objects.cpp
	- replenish source file: mods/zone.cpp
		- TODO: use query container source code to keep quota up
	- feed source file: mods/integal-objects.cpp
	- db source file: mods/integral-objects-db.cpp

# quotas
	- see quotas in weapon/armor lockers section
	- db source file: mods/integral-objects-db.cpp

# camera feed
	- install_camera_feed <room-vnum>...<room-vnum-N>
	- todo: name the object properly. currently it's named "gadget"
	- source file: mods/integral-objects.cpp
	- feed source file: mods/integal-objects.cpp
	- replenish source file: mods/zone.cpp
	- db source file: mods/integral-objects-db.cpp

# catchy names
	- create_catchy_name decimator "The Decimator" g36c.yml{sight:acog.yml,under_barrel:gm32grenadelauncher.yml}
	- instantiate_catchy_name decimator
	- source file: mods/integral-objects.cpp
	
# querying for objects
	- query_room <yaml>
	- query_container <name> <yaml>...<yaml-N>
	- query_inventory <yaml>
	- source file: mods/count-objects.cpp

# target practice
	- source file: mods/target-practice(-db).cpp
	- install_dummy
	- uninstall_dummy
	- instantiate_dummy

# armor levels
	- basic protection
	- advanced protection
	- elite protection
	- source files: mods/armor/{type}-protection.cpp

# mini games
	- line up

# replenish
	- source file: mods/replenish.cpp
	- replenishes ghost class every 10 seconds, or 10-11 seconds for however many ticks the GHOST_REPLENISH_TICKS() dynamic value is set to. (can be set using set_value)

# ghost class
	- abilities that need writing:
		- intimidate
		- toss_cryogenic_grenade
		- use_flash_underbarrel
		- go_dark
		- conceal
		- feign_death
		- summon_extraction
		- xray_shot
	- in progress features:
		- plant claymore
# stat command
	- stat <object|weapon>
	- source file: act.wizard.cpp

# register_help_command
	- source file: mods/help.cpp
	- register_help_command("scan","help file contents");

# centralized sql library helpers
	- source file: mods/pq.hpp

# super user functions
	- add_super_user <user>...<userN>
	- get_super_user_list
	- remove_super_user <user>...<userN>
	- go_invisible
		- makes the user invoking it go invisible
	- go_visible
		- makes the user invoking it visible
	- set_who_line <player> <string>
	- clear_who_line <player>...<playerN>

# centralized player saving/loading
	- mods::players::db_load::set_class()
	- mods::players::db_load::save()
	- source file: mods/players/db-load.cpp

# skills (only implemented skills)
	- electronics
	- armor
		- injure resistance
		- munitions reflector
	- marksman
		- spray chance
		- headshot chance
	- sniping
	- demolitions
	- chemistry
	- weapon handling
		- increased injure chance
		- assault rifle shrapnel
	- strategy
	- medical

# skills system (how it works)
	- The player has various skills that are categorized into various sections. These sections are Electronics, Armor, Marksmanship, Sniping, Demolitions, Chemistry, Weapon handling, Strategy, and Medical
	- A player has the ability to allot skill points to each of these categories.
	- A player can train specific categories by using the 'train' command
	- By training a category you can then allot your available skill points to a specific skill within a category. For example, you can alot 3050 skill points to the injure-resistance skill under the Armor category.
		- A player would type train injure-resistance. If the player has enough skill points alotted to the Armor category, then those points will be added onto injure-resistance.
	- Each skill has 4 different levels of proficiency. 
		- NONE: you haven't learned this specific skill
		- INITIATE: you are new to this skill and this skill will work, but there's a chance it will fail
		- FAMLIAR: you are well-versed in this skill. This skill will rarely fail
		- MASTER: you have mastered this skill and are very effective with it

# adding class skills
	- go to mods/skills.hpp and add a new vector. for our example we will use the instructions for ghost_class:
		- add enum constants to the enum proficiency_name_t
			```
			GHOST_CLASS_DRONE_SCAN, ... GHOST_CLASS_N,...
			```
		- add a new vector of proficiency_t:
			- create the new vector: 
				```
				static std::vector<proficiency_t> ghost_class = {
					{GHOST_CLASS_DRONE_SCAN,"ghost-class-drone-scan", "description...",SKILL_GHOST_DRONE_SCAN(),1},
				```
			- if you want it to be powered by the mods::values system, create SKILL_GHOST_DRONE_SCAN() in mods::values
			- add the vector to the master list. look for comment labeled:
				```
				/** start class-specific proficiencies */
				```
				- make sure the first argument in the open bracket is ":" and the last character is ":", so for our example, it should be:
					```
					{":GHOST:", ... }
					```
				- our master list will look like this:
				```
					static std::vector<std::tuple<std::string,std::vector<proficiencies::proficiency_t>>> list = {
						{"Electronics",electronics},
						{"Armor",armor},
						{"Marksmanship",marksmanship},
						{"Sniping",sniping},
						{"Demolitions",demolitions},
						{"Chemistry",chemistry},
						{"Weapon Handling",weapon_handling},
						{"Strategy",strategy},
						{"Medical",medical},
						/** start class-specific proficiencies */
						{":GHOST:",ghost_class},
						/** end class-specific proficiencies */
					};
					```

	
# rate limiting
	- throttle an action so that a specific user can only successfully execute the desired command within a certain number of times.
	- `set_rate_limit <action> <times> <seconds>`
		- example: `set_rate_limit save 1 60`
			- users can save once per 60 seconds
	- how to write rate limiting into your ACMD function:
		1) you must first define a rate limit enum in mods::rate_limiting::action_t. let's use PLAYER_SAVE
		2) add "mods/rate-limiting.hpp" to your include list
		3) add this to your ACMD:
		```
			SHOULD_RATE_LIMIT(PLAYER_SAVE);
		```
	- source files: mods/rate-limiting.(hpp/cpp)

# advance_level now uses our library code
	- source files: class.cpp, mods/levels.(hpp/cpp), mods/classes/advance-levels.hpp
	- implemented generic `advance_level` functionality for classes that don't have an implementation
		- see:
		```
			void generic_advance(player_ptr_t& player);
		```
	-  creating the extern 
		- create the following extern in mods/classes/advance-levels.hpp
			- `extern CLASS_advance_level(player_ptr_t&);`
			- define said function in the mods/classes/CLASS.cpp file

# adding classes to "implemented" status
	- in mods/levels.hpp, add the `player_class_t` constant to the vector named `implemented_classes`
	- follow the instructions for `advance_level`, in particular: creating the extern
