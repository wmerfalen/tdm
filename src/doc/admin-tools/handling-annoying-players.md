# Handling annoying players

# Banning players
In the following example, the problematic player in question is 'Jake'.

Jake has tried to create a player, or has completed the player creation process.
For whatever reason, he needs to be banned from connecting.

Ban command: `admin_ban <player>`

This command will ban not only the player, but the IP address of that user.

Any attemps to try to create the same player name will add that IP address to the
banned players list and their account registration will be stopped.

# Deactivating user accounts in the db
In addition to banning, we can remove all instances of the player in the database.

Command: `admin_wipe <player>`


