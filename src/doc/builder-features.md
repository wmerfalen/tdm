# Inventory of features (mods directory only)

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
# stat command
	- stat <object|weapon>
	- source file: act.wizard.cpp

# register_help_command
	- source file: mods/help.cpp
	- register_help_command("scan","help file contents");

# centralized sql library helpers
	- source file: mods/pq.hpp

