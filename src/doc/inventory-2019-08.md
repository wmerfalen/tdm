# Inventory of features (mods directory only)

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


