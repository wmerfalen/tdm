# Builder documentation: integral objects

# Storage areas
	- all values are stored in lmdb with a PREFIX
		- weapon lockers
			LMDB KEY: "weapon-locker|room-vnum"

		- weapon locker quota
			LMDB KEY: "weapon-locker-quota|room-vnum"

		- armor lockers
			LMDB KEY: "armor-locker|room-vnum"

		- armor locker quota
			LMDB KEY: "armor-locker-quota|room-vnum"

# Installing weapon lockers
	- `install_weapon_locker <yaml>...[yaml-N]`

# Uninstalling weapon lockers
	- `uninstall_weapon_locker`
		- use this command while in a room that has a weapon locker installed

# Installing armor lockers
	- `install_armor_locker <yaml>...[yaml-N]`

# Uninstalling armor lockers
	- `uninstall_armor_locker`
		- use this command while in a room that has an armor locker installed

# Installing camera feeds
	- `install_camera_feed <room-vnum>...[room-vnum-N]`

# Uninstalling camera feeds
	- `uninstall_camera_feed <description>`
		- use this command while in a room that has a camera feed installed.
			- annoyingly, you have to specify a "description"
			- just type: `uninstall_camera_feed "Pi-Mx Deployable Camera"`
			- from what I can tell, this needs server reboot for the camera to be removed (lame)

