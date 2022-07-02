# Building Raids

## Listing raids
	- `admin:raid:list`

## Reloading raids from the db
	- `admin:raid:reload-all`
		- this command cannot be ran while in `raid` mode

## Deleting raids
	- `admin:raid:delete <id>...<id-N>`

## Turn on `raid` mode
	- `admin:raid:pave on`
		- `admin:raid:pave <on> <raid-message> <level> <type>`
		- returns a generated raid id from the database.
		- Once ran, each mbuild command you run will tie that mob to the raid id.

## Creating mobs in `raid` mode
	- each mob you create will have their `raid id` filled with the current raid id set by `admin:raid:pave on`

## Set the spawn room
	- by setting a spawn room, you set the room players will teleport to when they join the raid
	- `admin:raid:set:spawn <room-vnum|this>`


## Broadcasting a raid
	- `admin:raid:broadcast <id>`

## Setting the raid radio message
	- the raid message is set when you run `pave on`
	- that message will be broadcast over mud radio
	- example:
	```
		> admin:raid:pave on "Hostile threat reported near hydro processing plant" 1-10 ELIMINATION
		Pave on for raid ID:17
		[+] It is done.

		> -- various mbuild instructions here --

		> admin:raid:pave off
		Success: closing

		> admin:raid:broadcast 17 
	```

		- the mud radio broadcast will be:
		```
		[mud radio]: Hostile threat reported near hydro processing plant. [ELIMINATION: 1-10][raid: 17]
		```

# Joining a raid
Raids will be broadcast over mud radio. People within a certain radius of the radio's power station will
be able to hear the broadcast. To join a raid, you have to use:
	- `raid:join <N>`
		- where `N` is the ID of the raid.
		- each raid broadcast will give you a simple to read raid ID:
		
