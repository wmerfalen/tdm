# Inventory of features as of 2020-09

- postgres
	- sql compositor
	- the feed(mods::pqxx::result) pattern
	- simple CRUD operations easily done via wrapped calls
	- simple pattern to reuse
	- several wrapper functions to do common CRUD calls in on line of code


- builder system
	- rbuild
	- obuild
	- mbuild
	- rbuild\_sandbox
	- cambuild
		- so that users can install permanent surveilance cam feeds in rooms

- duktape js scripting
	- profile scripts
	- include/require scripts
	- cmd/cmd\_exec calls
	- basic i/o

- mods::player
	- wrappers all over the place

- mods::weapon::sniper\_rifle

- chat system
	- subscribe to channels
	- dynamic creation of channels
	- add/remove/add\_private\_subscriber
	- chanmgr ACMD interface
	- create hidden channels
	- default channels powered by values system

- conf system
	- mods::conf rudimentary repo for configurations

- date/time interface
	- hourly granularity
	- moon phases

- debug
	- hell mode
	- mods::debug 
		- show tics

- basic classes
	- sniper
		- has sniper rifle class weapon
		- can only be wielded by sniper
		- snipe command [ in progress ]
	- medic
	- engineer
	- support
	- marine

- deferred
	- lambda functions can be executed N seconds in the future

- drone mode
	- switch to drone 
	- smart descriptor channeling
	- start/stop/get drone

- behaviour trees
	- basic snipe tracking tree
	- ability to add/delete nodes, thus plasticity

- npc c++ class
	- inherits from mods::player class

- overhead map
	- displays overhead map using basic squares
	- cache system to prevent recrawling

- user prefs 
	- key/value system
	- easily expandable

- quests system
	- executes js to check for quest completion status
	- open to expansion

- scan ACMD
	- scan system crawls rooms to find a target
	- solid, no bugs

- auto-login system
	- development mode only, not for live. dangerous if live
	- boot mud, any socket that connects auto logs in as said user

- jx encode/decode
	- encodes and decodes to duktape-friendly format
	- works just like json
	- commands will know if called by js and can format using jx encode

- unit tests framework
	- simple and powerful
	- can be called from cli

- ORM system
	- beta stage
	- framework for more ORM based code
	- needs work

- projectiles system
	- smoke grenades
	- flash grenades
	- incendiary grenades

- fire system
	- fires can be lit inside rooms
	- certain textures of rooms will exacerbate and prolong fires
	- fire damage done to players and objects inside a burning room
	- a system to process fires so that they have an attack, decay, sustain, release envelope

- cambuild system
	- allows a builder to create a surveilance camera feed that changes automatically
	- allows a builder to create a surveilance camera feed that changes by control of user

- mini games system
	- use the 'hack' command to unlock doors
	- rotate_right
	- rotate_left
	- reset_hack
	- "line up" even/odd game
		- randomly generates mini game

- builder commands for minigames
	- install_minigame
	- uninstall_minigame
	- list_minigame

- orm layer
	- all player classes have CRUD functionality
	- minigames can load from db
		- save/update/delete functionality TODO
	- camera feed objects
		- integral object. functionality TODO

- integral objects system
	- objects such as camera feeds are considered integral objects
		- CRUD ORM layer TODO

- quotes system
	- CRUD ORM layer
	- allows you to save quotes and quote authors to be displayed randomly or explicitly

- dice rolling system
	- accepts number and sides of dice
	- random number generator included

- values system
	- majority of static strings and A LOT of hardcoded values are changable at runtime or at boot time
	- mods::values system gives you the functionality to customize a large variety of values
		- allows you to change game balance by modifying combat integers/strings
	- js support for value load/save/revert

- hashed passwords
	- passwords no longer stored in plaintext

- specialized npc classes
	- mini gunner
		- has own behaviour trees
		- has disoriented behaviour tree
	- loadout customization
		- will load whatever yaml files you dictate
			- powered by the values system
		-

- shops system
	- CRUD ORM layer
	- allows you to install a shop in a room
		- can customize the items sold in the shop
	- can dictate which shop owner mob is spawned in the shop

- damage event system
	- when a player or npc gets hit, it spawns an event
		- event can be handled/subscribed to by different parts of the code
		- more flexible system to allow easier message passing and damage outputs

- peaceful rooms
	- npcs will notice and honor peaceful rooms
	- pcs cannot harm anything or anyone from within a peaceful room
	- ranged attacks will stop just short of a peaceful room

- autologin debug feature
	- all sockets that connect get logged in automatically
		- useful for fast debugging

- affects system
	- players and npcs can now be affected
	- affects are calculated and removed after a counter reaches a specific value
	- affects power the fire system
		- fires can be set and go through several different phases of fire intensity
		- room textures system affects the lifetime of various fire phases

- room textures
	- various room textures can be applied to a room
	- each texture has different effects on gameplay
	- overhead map can detect and draw rooms differently depending on texture type

- help system
	- standardized help system that recognizes and parses help for any command
	- TODO: load all help files from files/postgres/lmdb/values-system

- combat injury/revive
	- players can be injured
	- other players can revive

- player inventory flushed to fast lmdb database
	- inventory/wear list is synced to lmdb each time an item is added/removed from inv or wearing
	- TODO: compress list before saving

- deep object parser
	- can parse rifles with several different attachments

