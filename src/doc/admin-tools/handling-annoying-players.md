# Handling annoying players

# Banning players
In the following example, the problematic player in question is 'Jake'.

Jake has tried to create a player, or has completed the player creation process.
For whatever reason, he needs to be banned from connecting.

## Banning by IP adress

Ban command: `admin:ban:ip <player>...[playerN]`

This will ban the user by their IP address only. It will not ban them by
their hostname or username.

- To ban by IP, hostname (if any), and username use the `admin:ban:*` command.

## Banning by hostname

Ban command: `admin:ban:host <player>...[playerN]`

If a user has a hostname that they are connected via, this will ban by that hostname.

This will not ban the user by their username or IP.

- To ban by IP, hostname (if any), and username use the `admin:ban:*` command.

## Banning by user name

Ban command: `admin:ban:user <player>...[playerN]`

This will ban a user by their user name. It will not ban them by IP or hostname.

- To ban by IP, hostname (if any), and username use the `admin:ban:*` command.

## Banning by IP, hostname, and username

Ban command: `admin:ban:* <player>...[playerN]`

# Muting players
It's sometimes desirable to mute a player instead of outright banning them.
Banning a player has the side effect of not allowing the player the ability to
plead for forgiveness to the admins. Muting a player allows the player to still
play the game, but without the use of all the basic communication facilities like
chat, emotes, shout, say, etc.

Mute command: `admin:mute <player>`
Un-mute command: `admin:unmute <player>`

Muted players will be notified that their communication has been stopped and they
will be given precise instructions on who they can chat with. The only way a
muted player can communicate is through sending a tell to the admin that
muted them.

A muted player get to send one tell per minute. If they try to send more tells,
the system will simply ignore it and let the player know when they can send
another tell.

Tell messages from muted players will appear like this:

```
[muted-player][username: johnny]: "Hi, can you tell me why I'm muted?"
far::65000H 1049M 65000V > 
```

... assuming that "johnny" is the name of the muted player.


# Freezing players
Freezing a player has the following effects:
	- The player is banished to a room with no exits and no entrances
	- The player cannot send any communications, not even a tell to the admin that froze him
	- The player is presented with a message that tells them they are frozen and
		must wait for an admin to unfreeze them.

Freeze command: `admin:freeze <player>`
Un-freeze command: `admin:unfreeze <player>`

# Deactivating user accounts in the db
In addition to banning, we can remove all instances of the player in the database.

Command: `admin_wipe <player>`


