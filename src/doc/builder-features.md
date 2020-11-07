# Inventory of features (mods directory only)

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
