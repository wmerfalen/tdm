# Inventory of features

# rules for lighting a fire
	- a room can be lit on fire if it has the following:
		- has VOLATILE texture AND one or more of the following occur
			- an explosion occurs inside the room
			- incendiary or explosive elemental damage is done to any receiver of damage in the room
			- an incendiary or fragmentation grenade goes off in the room
				- either by throwing or underbarrel, the delivery method doesn't matter
			- an incendiary or explosive claymore goes off in the room
			- someone misses a rifle attack to someone in the room
	- propagating volatile fires in code
		- mods::explosive::append_explosive_damage_to_room(room_id,damage);
		- mods::explosive::append_incendiary_damage_to_room(room_id,damage);
		- if room has `GLASS_WINDOWS` or `GLASS_CONTENTS` texture
			- `mods::glass::scatter_room(room_id);` gets called and adds texture `SHATTERED_GLASS`
		- when player or NPC enters room with `SHATTERED_GLASS` texture:
			- if wearing footwear
			- chances to cause foot damage per footwear armor classification
				- basic: 80%
				- advanced: 20%
				- elite: 5%
			- if mods::rand::chance(classification) 
				- add BLEED to player
				- BLEED affects player for 60 ticks
					- causes 10hp damage every 10 ticks

# Trim down classes 
	- [ ] Marine
		- Melee and rifles
		- well-balanced

	- [ ] Ghost
		- sniper rifles and stealth
		- fast, but low armor

	- [ ] Breacher
		- Heavy Melee favorite
		- Tank character
		- Slow
	
	- [ ] Contagion
		- Necromancer spell caster
		- Decent weapons
		- fast, but low armor

# 2021-07-16
	- [ ] Implement these game dynamics for each class
		- [ ] GHOST
				 -- [x] expert at sniper rifles and ranged combat
				 	-- [x] Extra 10.5% damage per tier (each tier is ten levels)
				 -- [x] can attach underbarrels to sniper rifles
				 	-- [x] shotgun underbarrel
						-- [x] skill that gets an extra shot per half tier
				 	-- [x] frag launcher
						-- [x] skill that gets an extra shot per half tier
				 -- [delay until emp/chaff dynamics defined] has access to emp/chaff grenades (replenishes)
				 -- [x] can inject arenaline shot
				 	-- [ ] 20 free movement points per tier
				 	-- [ ] damage taken is reduced by 15% per tier
						-- [ ] tier 1 -> 50 ticks
						-- [ ] tier 2 -> 100 ticks
						-- [ ] tier 3 -> 170 ticks
				 -- [x] has xray shot
				 	-- [x] targets don't need to be in line of sight
				 -- can plant claymores of type:
				 	-- [x] corrosive 
						-- [x] corrosive damage is continuously dealt
						-- [x] target gets blinded
							-- [x] how long is determined by headgear/goggles/eyewear
							-- [x] eyewear/goggles/helmet are best defence
							-- [x] masks with high balisitic resistance percent work well too
					-- [ ] shrapnel
						-- [ ] shrapnel claymores cause BLEED
							-- [ ] bleed damage is continuously dealt
							-- [ ] bleed slows target by 10%
					-- [x] explosive
						-- [ ] explosions cause continuous fire damage
				  -- [x] time to install claymore mines are reduced by 50%
				 -- [ ] can mark and track an enemy
				 	-- [ ] tracked enemies take more damage
				 -- [ ] can target limb
				 	-- [ ] attacking right arm activates nerfs on target related to primary
				 	-- [ ] attacking left arm activates nerfs on target related to primary and secondary
				 -- [ ] can snipe objects (such as doors, cars, etc)
				 	-- [ ] cars will explode and light the room on fire
				 -- [ ] can fire "neutron" shell
				 	-- [ ] targets hit by shell cause radioactive and explosive damage to all room inhabitants
				 -- [ ] fleeing from melee is 25% more effective
				 -- [ ] immobilizing shot (passive)
				 	-- [ ] 15% chance of causing target to stay in room and not close distance for 50 ticks
				/**
				 * --[
				 *  -- uses stealth and intel to deal extra damage
				 *  -- can dissipate
				 *  	-- when dissipated, enemies cannot see or detect you
				 *  -- time to install claymore mines are reduced by 50%
				 *  -- ranged combat has a 75% chance of the target not knowing
				 *  	which direction the shot came from
				 *  -- SMG's are silenced
				 *  	-- first attack with SMG's cause the target to not know who
				 *  		the attacker is
				 *  -- SMG's have "initiative"
				 *  	-- when an enemy would attack you, you attack first
				 *  	-- when they attack, you immediately attack again
				 *  -- have access to smoke and flashbang grenades (replenishes)
				 *  -- "misdirection"
				 *  	-- spawn a holographic decoy that your enemies will attack instead of you
				 *	-- "Suffering Shot"
				 *  	-- fire special ammunition which sticks to the target and deals damage continuously
				 *  		for 50 ticks
				 *  -- [
				 */
				return {
					1, // MELEE
					2, // WEAPONS
					3, // INTEL
					3, // SPEED
					1, // ARMOR
				};
			case MARINE:
				/**
				 * --[
				 *  -- most well-balanced character
				 *  -- all wielded assault rifles get:
				 *  	-- [x] +10% critical chance
				 *  	-- [x] +15% free ammunition
				 *  	-- underbarrel frag launcher (ammo replenishes)
				 *  -- passive abilities
				 *  	-- [x] Assault rifle effective range increased by 2 rooms
				 *		-- [x] Assault rifles do 25% more damage
				 *  -- received shrapnel and incendiary damage is reduced by 25%
				 * 	-- "second wind" (passive)
				 * 		-- if a hit would cause death, you have a 23% chance of
				 * 			surviving with 20hp for 20 ticks. During this time
				 * 			you can move around but cannot attack. When over,
				 * 			your primary weapon does 10% more damage and gains
				 * 			10% explosive damage.
				 * 	-- received disorient affects are reduced in duration by 50%
				 * 	-- Shotguns deal same room damage at 2-3 rooms away
				 * 	-- "disarm"
				 * 		-- melee anatagonists get their primary dropped on the ground
				 * 		-- tier 3: melee antagonists get their primary weapon removed and destroyed
				 * 	-- assault rifles have a 10% chance of dealing incendiary damage
				 * --[
				 */
				return {
					2, // MELEE
					2, // WEAPONS
					2, // INTEL
					2, // SPEED
					2  // ARMOR
				};
			case BREACHER:
				/**
				 * -- [
				 *  -- expert with demolitions
				 *  -- breach charges replenish automatically
				 *  -- can retrofit shotguns with explosive breach underbarrel
				 *  -- all wielded weapons get +10% explosive damage
				 *  -- can remotely seal a room for the purpose of amplifying detonations
				 *  -- can walk in a direction and automatically breach through
				 *  	-- can sometimes make new rooms
				 *  -- breaching doors causes room inhabitants to take damage from explosion
				 *  -- "breach and clear"
				 *  	-- breach a door and immediatel throw a flash bang into the room
				 *  	-- inhabitants get disoriented and take more damage
				 *  -- "knockdown"
				 *  	-- fire a shot that knocks the enemy to the ground
				 *  	-- knocked down enemies take more damage and are vulnerable to "detain"
				 *  -- "detain"
				 *  	-- disarm and zip-tie an enemy
				 *  -- "entry denial"
				 *  	-- prevent any enemy from entering your room by guarding a specific entrance
				 *  	-- friendlies can snipe around you
				 *  	-- you can use your secondary to deal damage to NPCs trying to enter
				 *  -- Shotguns deal 10% extra damage
				 *  -- SMG's deal 10% extra damage
				 *  -- SMG's have corrossive damage
				 *  	-- corrossive damage from SMG's cause enemy armor to be less effective
				 *  -- SMG's in same-room engagements have a chance of dealing shotgun damage
				 *  -- received shrapnel and explosive damage reduced by 25%
				 *  -- [
				 */
				return {
					3, // MELEE
					2, // WEAPONS
					1, // INTEL
					1, // SPEED
					3 // ARMOR
				};


# 2021-07-04
	- I made a list of skills and perks in `mods/levels.cpp`
	- I suggest you look there for next steps and which dynamics to implement
	- [ ] perform a review and add/remove skills
		- [ ] review list of skills in `get_triads_by_class(player_class_t c)`
		- [ ] add more skills to each class
		- [ ] add WEAPON perks to each class 
			-- Give concrete incentives for a player to choose a specific class
			-- Give nerfs/buffs based on weapon types
			-- for example: GHOST class has SMG perks







# 2021-05 new features
	- `build_contract(data)` javascript function now available. see conbuild.md for example usage

# deep object parsing supports levels now
	- deep object parser accepts the following syntax:
		- `rifle/psg1.yml{... data ... }#level:N` where N is the level. 
		- this *MUST* be at the very end of the string

# 2021-04 new features
 	- elemental damage
		- now honors rifle attachments
	- rifle attachment debugging
		- `list_rifle_attachments` with no args
			- will show you the current allocations
	- "rifle variants" or just "variants"
		- the new term for deep object parsed rifles
	- SUPERCMD is now the preferred way to build admin commands
		- it is automatically parsed from the codebase an included in `help` command
	- help command now lists all ACMD if you're a mortal
	- help command now lists all ACMD and SUPERCMD if you're an immortal
	- `builder_help` now includes `lib/SUPERCMD.list` contents
	- mob roaming data
		- mobs now honor mob roaming data
		- reading of mob zone commands now uses the same functions as `mbuild instantiate`
		- mobs instantiated with zone commands now get their equipment loaded from their `mob_equipment` profile
	- stats are increased/decreased based on eq
		- @see STATS section below
	- NEW CONTENT
		- PSG-1 sniper rifle variants
			- demon lance psg-1 
	- nickname helpers
		- @see NICKNAME HELPERS section below
	- zone command fixes and mob placements
		- fixed zone data in the following ways:
			- `room_data.zone` now holds the *real* `zone_rnum`
			- `zone_data.name` is no longer a char*, opting for std::string
			- `update_zone()` and other zone helpers now exist in `mods::zone` namespace
			- 'zbuild mob this 501 128 10 0' is now possible (using "this")
			- `reset_com.object_data` holds uuid_t list of instantiated mobs
			
# potential issues moving forward [ as of 2021-04-12 ]
	- `mods::npc::equip(obj_ptr_t,int)` is hopelessly broken
		- the only way to persist equipment is to call `npc->player_ptr()->equip(..)`

# SECTION [CLAYMORES]
	- `mods::char_move_to(room,player)`
	- claymore mines will prevent movement
		- if owner, will not detonate
		- now explodes, doesn't discriminate when dealing damage
		- if installed in direction heading and not owner, detonates
		- if installed in target room at exit heading toward and not owner, detonates
		- claymore mine damage+messages caused by `mods::projectile::exloode`
		- if moving towards claymore and it detonates, you get thrown back two rooms
		- new helper function: `world_size()`

# SECTION: [STATS]
	- equipping and unequipping armor increases/decreases
		- [x] stats (aff_abils())
		- [x] damage resistance
		- [x] weight
	- equipping and unequipping non deep object rifles affect
		- [x] stats (aff_abils())
		- [x] damage percent
	- equipping and unequipping deep object rifles affects 
		- [x] stats (aff_abils())
		- [x] damage percent

# SECTION: [NICKNAME HELPERS]
	- overview: these are NOT crud operations! For that, use `rbuild`
	- the following nickname commands are useful for fast traveling
	- list of helper commands:
		- `nhelp` see help page for nickname commands
		- `nset` set the nickname for the current room you're in
		- `ndelete` delete the nickname(s) you specify
		- `nfind` find the room rnum (and vnum) of the specified nickname(s)
		- `ngoto` goto the nickname you specify
	- examples:
		- `nset recall`
			- sets the current room's nickname to 'recall'
		- `nfind recall shipyard east-exit`
			- reports room id/vnum of nicknames (if they exist)
		- `ndelete recall shipyard east-exit`
			- deletes nicknames of rooms associated with the nicknames specified
		- `ngoto recall`
			- teleports you if the nickname exists
	- bookmarking rooms
		- the flow goes like:
			- `bookmark armory this`
			- ... go build other things ...
			- `print_vnum armory`
				- should print the room_vnum of the bookmarked armory


# SECTION: [BUILDER JS]
	- `ira('nickname')` function to ease instantiating deep object rifles
	- `list_op()` function will send you all currently instatiable variants


# next up
	- We could always use content:
	- hqbuild
		- [ ] phase 1
			- [ ] create a car port room that leads to the east near the cobalt fob east exit
			- [ ] if anyone at the shipyard (way east of the cobalt fob) gets attacked, dispatch a response SUV
				- [ ] response SUV drives along highway and hits anyone in the way depending on dice roll 
				- [ ] use values system to roll dice for car hit
				- [ ] use the damage system to deal blunt damage
					- [ ] if player gets hit, force their position as laying down
					- [ ] if player gets hit while laying down, they die no matter what
		- [ ] Functional rooms
			- [ ] Weapon vendors
				- [ ] Shotguns
				- [ ] Assault Rifles
				- [ ] SMG's
				- [ ] Sniper Rifles
				- [ ] Pistols
				- [ ] Machine Pistols
			- [ ] Ammunition Vendors
				- [ ] High Velocity
				- [ ] Incendiary
				- [ ] Cryogenic Rounds
				- [ ] Anti-Matter
			- [ ] Attachment vendors
				- [ ] Sights
				- [ ] Muzzles
				- [ ] Underbarrel
				- [ ] Magazines
	- [ ] create cobalt fob room to buy armor
	- [ ] create cobalt fob room to buy weapons
	- [ ] create cobalt fob room to buy explosives
# low priority next up
	- [ ] allow users to change weapon attachments
	- contracts (quests)
		- [ ] restrict contracts by player level
		- [ ] optional failure penalties
			- [ ] reduce xp
			- [ ] reduce money
		- # STRETCH GOALS (optional non-critical systems) 
			- [ ] mob speech trees [ LARGE TASK ]
				- [ ] when on specific contract step a mob's dialogue changes
	- verify
		- [ ] contract data saved through conbuild works after mud reboot
	- [ ] radio
		- [ ] test radio communication to/from hq
	- lowly security guard extended type
		- [ ] test `smart_mob` extended class type
		- [ ] test report hostile activity radio support
	- Mob reinforcements
	- [x] create car object type
		- [x] SUV types
			- [ ] Blacked out private contractor security force
			- [ ] Police SUV detainment vehicle
			- [ ] SWAT team van (16 persons)

# CONTRACT FEATURE GOALS
	- needed systems for contracts (listed in preferred implementation order)
		- [x] finalize rifle attachments class and give the player the equipment
			- [x] when rifle attachment is loaded, remember the last position (wear or equip) on player entering game
			- [x] when a player examines the rifle, display the `rifle_attachment_t::examine()` output
			- [x] create orm layer rifle-attachments class
			- [x] flush rifle attachments class to and from orm layer
				- [x] modify `mods/orm/inventory.cpp` to save/feed rifle attachments
		- [x] a central place to get contracts
			- just use the contracts ACMD and list all contracts
			- [x] mark an x near contracts that are being done for that player
		- [x] steps sub-command to ACMD do_contracts
			- [x] show the current step's description
		- [x] rewards system
			- [x] reward player with
				- [x] xp
				- [x] money
				- [x] deep object parsed items
					- [x] saved using object instance code (see object instance section below)
				- [x] support syntax:
					- [x] `#yaml|type/path.yml`
					- [x] `#deep|type/description`
		- # contract reward syntax [ NICE TO HAVE ]
			- returns a vector of `obj_ptr_t`'s of parsed objects
			- example:
				- `conbuild set-step-data 1 218 s_reward_1 #yaml|explosive/frag-grenade.yml`
				- `conbuild set-step-data 1 218 s_reward_2 #catchy|rifle|the-decimator`
				- `conbuild set-step-data 1 218 s_reward_3 #deep|rifle/g36c.yml{sight:acog.yml,muzzle:compensator.yml,under_barrel:gm32grenadelauncher.yml}`
			
# SECTION: [CONBUILD] 
	- cmd: `conbuild help`
	- source: `mods/builder/conbuild.hpp`
	- conbuild command list
		- `conbuild new <vnum-of-contract>`
		- `conbuild title <vnum> <text>...`
		- `conbuild description <vnum> <text>...`
	- listing steps per contract
		- `conbuild show-steps <vnum>`
	- loading a specific contract's steps
		- `conbuild load-steps <vnum>`
			- this is now implicitly called whenever you call the `show-steps <vnum>` sub-cmd
	- setting step data
		- `conbuild set-step-data <vnum> <step-id> <field> <value>`
		- note: the fields are exactly what the contract_step column names are in postgres
	- listing column names for step data
		- `conbuild columns`
	- saving steps
		- `conbuild save-step <vnum> <step-id>...[step-id-N]`
	- deleting steps
		- `conbuild delete-step <vnum> <step-id>...[step-id-N]`
	- ## VERY IMPORTANT ##
		- any steps you load/save through conbuild are stored separately in another data structure than from what may be in the db
		- this doesn't mean that it's not saved in the db. they are saved, it's just step data is localized for the sake of convenience when using conbuild
		- to get all steps in the db use other means
		- if you want to know exactly what's in the db, look through psql

# mods::players::messages
	- way to queue up data for the next screen for a player

# player contract instance
	- terms used here:
		- contracts mean quests
		- steps mean individual steps in a contract
		- player contract instance is data related to the player's currently tracked contract
	- orm layer
		- contracts
		- contract_steps
		- player_contract_state (orm for player_contract_instance wrapper)
	- creating contracts with several steps
	- see unit tests for pci on how to create/track a contract

# slotted builder class
	- handles a lot of the boilerplate for builder functions
	- source: `mods/builder/slotted-builder.hpp`
		- example usage: `mods/meqbuild.cpp`


# all extended mob types to use meqbuild
	- using the values system to assign extended mobs their eq is now DEPRECATED
	- use the meqbuild command

# recent enhancements
	- new `OUTSIDE_TRASHY_PARKING_LOT` sector type
		- source: mods/rooms.(c|h)pp
	- it is now possible to get a concise value from mbuild new
		- source: mods/builder.cpp
			- helpers:
				- `ENCODE_STR(integral)`
				- `ENCODE_INIT()`
				- `ENCODE_R("string-response")`
		- source: mods/player.hpp
			- `set_scripted_response(std::string_view);`
			- `std::string&& consume_scripted_response();`
	- exec('cmd') javascript function to get scripted response:
	 	- `js send(exec('mbuild new')); // will respond with rnum of newly built mob`
		- source: mods/js.cpp
	- commands that return scripted responses:
		- `mbuild new (returns mob_proto.size() after pushback)`
		- `mbuild pave on|off|continue (returns transact_id)`
		- `mbuild exists <mob_vnum> (mob vnum or "!" if not found)`
	- mob object format
		- created ideal way to create rooms and mobs
			- in js/far.js look for strings starting with: `/** -!- Poster child for: `

# vertical slice gameplay goals [ first 10 levels ]
	- [ ] Content via Procedural Generation
		- [ ] Area Configuration (prefixed with arcon-* in the mods/fluxkraft folder)
			- An area configuration describes an area theme. In the first arcon in the game, we will
			be using the metropolitan area as a theme. 
			The general idea is that you have various manually built rooms/buildings/areas that
			are interconnected by random and procedurally generated content. Thes are prone to change
			but they allow us to generate areas with a bit of randomness to them but also with 
			central hubs of manually built content that has meaning.

		- [ ] Define differently shaped map components
			- [x] Room shapes
			- [ ] Macro vs Micro generation
				- [ ] Create macro patterns
					- [ ] i.e.: L-shaped macro pattern
				- [ ] Micro generation composes all room shapes within the confies of the macro pattern
			- [ ] Armor vendors
			- [ ] Consumable vendors
				- [ ] Steroids
					- [ ] Adds to strength
					- [ ] Adds to agility
				- [ ] HP Stim packs
				- [ ] Movement Stim Packs
				- [ ] Concentration
					- [ ] Adds to accuracy

	- [ ] create an area with mobs leveled 1-10
		- [ ] create behaviour trees for:
			- [ ] `MOB_AGGRESSIVE`
				- [ ] Will attack at first glance
			- [ ] `MOB_WIMPY`
				- [ ] More likely to flee than fight
			- [ ] `MOB_STAY_ZONE`
				- [ ] Will not leave room
			- [ ] `MOB_AGGR_GOOD`
				- [ ] Turns aggressive if witnesses injustice
			- [ ] `MOB_HELPER`
				- [ ] Helps out the innocent
		- [ ] Area idea 4: Mob levels 10-20
			# this area needs these features to be built
				- [ ] An RFID card scanner
					- [ ] when a card is swiped, the room vnum it unlocks is checked
				- [ ] An RFID card
				- [ ] swipe ACMD
					- [ ] `swipe <selector> <direction>`
			- [ ] quest failure eventing system
				- [ ] when a quest failure condition is met, you must wait again until the zone resets to try the contract again
			- [ ] Broadmore Drive -> U-Slurp's Video Shop
					- [ ] Situations:
						- [ ] Money Laundering
							- Intel suggests that the business is being used to funnel 
							money through. Records are appearing out of thin air with a long
							paper trail that ends up leading nowhere. This is potentially a 
							deep state insider with high level access to the mainframe of
							major banks.
							- Section: outside the office
								- There is a very sturdy door blocking the entrance to the administrative office.
									An RFID key swipe is attached to the door.
								- Objectives:
									- [ ] There is an office in the back near the video rooms.  Find a way into the office
										- [ ] get the RFID fob from around the clerk's neck
										- [ ] take RFID keyfob from his knecklace
											- `get keyfob clerk`
										- [ ] swipe
											- `swipe keyfob`
									- [ ] Figure out the passcode to the safe
									- [ ] Transmit the documents using your smart phone
										-[ ] photograph <documents|selector> using phone

		- [ ] Area idea 2: Mob levels 15-20
			- [ ] Downtown Northern Talon
			 - Area is a DMZ where lawlessness has taken hold of the city.
			 A militia group now controls this area with scattered levels
			 of resistance fighters. Almost all innocent citizens have fled.
			 The remaining non-hostile citizens still in this DMZ are slaves
			 or people who have been repurposed to serve some sort of agenda
			 or job as subserviants to the militia group.
			 - The majority of the hostile members you will meet outside of
			 the nexus are lower level grunts. They are all armed
			 - The central point of failure is the stadium which has been
			 controlled by the militia. Underground tunnels to and from the
			 stadium have been dug and are actively guarded. These tunnels
			 serve many purposes, but are mainly used to transfer goods
			 and arms without being seen by drones/satelites.

		- [ ] Area idea 3: Operations
				- [ ] Composed of three major military buildings
					- [ ] Operations Security (OPSEC) Building [ INTEL ]
						- [ ] Vertical takeoff planes and helicopters
					- [ ] Ballistics [ WEAPONS ]
					- [ ] Advanced Tech [ RESEARCH ]
				- The three military buildings are safe zones and not controlled by
				hostile NPCs. 
				- Each building has a perimeter that hostile forces cannot overcome.
				Though, as soon as you leave the perimeter, you have a high chance
				of coming into contact with hostile NPCs from the surrounding areas.
				- Areas outside the buildings begin to get hostile the further away
				you venture.
				- A major point of contention are the underground railway systems
				as guerilla operations are being conducted to hijack the cars
				that run on these tracks.
				- There are pockets of heavy resistance throughout the city 
				that have a sudden influx of militia members. Their goals are to
				gain control of buildings that serve as a hub for the non hostile
				NPCs that live near the area. Common targets are banks, mom+pop shops,
				gun stores, and liquor stores
				- 
		- [ ] Area idea 1:
			- Saltwater Spruce Blvd
				- [ ] La Posta Restaurant (Main Floor)
					- [ ] Mobs:
						- [ ] La Posta Chef
							- [ ] Dull Knife
						- [ ] MF-13 Initiate
							- [ ] Machete
						- [ ] MF-13 hoodlum
							- [ ] Mac-10 .45 Machine Pistol
								- [ ] high velocity rounds
								- [ ] drops HV rounds and Mac-10 on death
					- [ ] Situations:
						- [ ] MF-13 hoodlums seize control of block
							- [ ] Eliminate hoodlums
						- [ ] Plant explosives on lifted truck behind restaurant
				- [ ] La Posta Restaurant (Freezer room below Main Floor)
					- [ ] Mobs:
						- [ ] Butcher [HELPER]
							- [ ] Machete
							- [ ] Mac-10 MP
						- [ ] Crazed Butcher [AGGRESSIVE]
							- [ ] SASG-12
							- [ ] Bolo Knife
					- [ ] Situations:
						- [ ] Find dead bodies held in frozen containers
						- [ ] Plant explosives on frozen containers containing bodies
						- [ ] Search frozen containers for contractee's partner
						- [ ] Find the stash of SASG-12 shotgun shells
						- [ ] Break open the safe using the thermite bomb
						- [ ] Fight off the corrupted cops until reinforcements arrive
				- [ ] Pawn Shop (Main Floor)
					- [ ] Mobs:
						- [ ] Agitated drug addict
							- [ ] SIG SAUER
								- [ ] Extended magazine
				- [ ] Bank (Main Floor)
					- [ ] Mobs:
						- [ ] Bank Teller
						- [ ] Honest Patron (Male)
						- [ ] Poor woman
						- [ ] Old Decrepit Man
						- [ ] Agitated Drug Addict
							- [ ] Rusted Knife
						- [ ] Bank Security Guard
							- [ ] SIG SAUER
						- [ ] Homeless beggar
							- [ ] Rusted Knife
				- [ ] Bank Vault
					- Mobs:
						- [ ] Security Guard (4)
						- [ ] Female bank teller
						- [ ] Hidden door leads to secret tunnel
				- [ ] Bank (Secret Tunnel)
					- [ ] A bank vault that leads to a secret tunnel below
					- [ ] Mobs:
						- [ ] MF-13 Drug Cartel Thug
						- [ ] MF-13 Human Trafficking Kidnapper
						- [ ] Extorted Mining Expert
						- [ ] Heavily armed guard
						- [ ] Delusional Blade Wielder
						- [ ] Corrupt Police Officer
						- [ ] Drugged out Heroin Addict
					- [ ] Stashes:
						- [ ] Bin of Assault Rifles
						- [ ] Bin of Shotguns
	- [x] use zone commands to populate zone with mobs
	- [x] create a safe zone where players can respawn
	- on each kill:
		- [ ] award experience points
		- [ ] use forge engine to award a randomized loot item
	- on each level up:
		- [ ] award practice sessions
		- [ ] conditionally award new skills
	- create contracts (our term for quests):
		- [ ] recon
			- [ ] go to place, fight off enemies, report back
		- [ ] eliminate
			- [ ] go to place, eliminate enemies, report back
		- [ ] crafting
			- [ ] go to place A, get required part, report back
				- [ ] repeat until all parts gathered
			- [ ] go back to contractee and have contractee:
				- [ ] describe how to build the object
	- when user practices a skill:
		- [ ] have them be the lowest proficiency
			- [ ] only way to master is to use the skill
	- when user uses a skill:
		- [ ] increment player skill usage counters
		- [ ] if usage counter reaches a new level:
			- [ ] player becomes more proficient at skill
			- [ ] damage based skills deal more damage

# object instances [ PHASE 1 ]
	- Overview:
		The yaml object system contains files within `lib/objects/<type>/<item>.yml`. These are templates
		for the forge engine to roll random attributes against. These rolled items are not stored anywhere.
		The Object Instance system is here to remedy that. Any object that is generated by the forge system
		needs to be stored in postgres. This goes for all item types that get rolled (armor and rifles).
		In addition to keeping the rolled stats in an object instance table, we need to update the deep
		object parser to recognize items in the database. This is so that a player can have loadouts
		and customized weapons that survive mud shutdown/reboot.
	- postgres table: `rifle_instance`
		- [x] has all columns that are in the rifle tuple members (see `mods/item-types.hpp`)
		- [x] contains the rolled values from the forge engine
			- [x] as of 2021-06-16: forge engine rifles are persisted across logouts/reboots/shutdowns
	- postgres table: `armor_instance`
		- [ ] has all columns that are in the armor tuple members (see `mods/item-types.hpp`)
		- [ ] contains the rolled values from the forge engine
			- [ ] persist across logouts/shutdowns/reboots

# object instances [ PHASE 2 ]
	- Overview:
		Once phase 1 is finished, the deep object parser needs to be adapted to parse database ID's so
		that a customized rifle or piece of armor can survive shutdown/reboot. Store this in postgres.
			- [x] rifles persist
			- [ ] armors persist
	- [ ] modify deep object parser to recognize database ID's
		- [ ] load those object instances
	

# compound sql statements
	- it is now possible to load/delete rows using chained and/or logic
		- source: mods/orm/util.hpp
		- new overloaded template functions: `load_where`, `delete_where`

# leveling up
	- Experience allows you to advance a level
		- Experience is obtained by completing contracts and killing enemies
	- Practice sessions are awarded each level according to data within an sql table
		- Practice sessions are used to:
			- learn skills
			- see the `skills` command for a comprehensive list
	- the `train` command allows you to practice a specific skill
	- view how many skill points you have
		- sp
		- score
		- skills
		- train
	- practice sessions for each player level are stored in sql
		- source file: mods/orm/skill-points
		- table: `skill_points`
			- `id` primary key
			- `sp_level` player level
			- `sp_points` points to reward user on level up
			- `created/updated_at`
		- each level currently awards you 1 practice session
	- Familiarity
		- You can practice any skill, but in order to master it, you must use it
		- source file: `mods/orm/player-skill-usage.(h|c)pp`
			- table: `player_skill_usage`
			- `id`
			- `ps_player_id`
			- `ps_skill_id`
			- `ps_usage_count`
			- `created_at/updated_at`

# forge engine
	- orm classes `rifle_index` and `armor_index`
	- use ACMD: `add_armor_index` 
		- example: `add_armor_index body ballistic-vest.yml iron-vest.yml python-vest.yml`
	- use ACMD: `add_rifle_index` 
		- example: `add_rifle_index ar g36c.yml scar-heavy.yml`
	- generated types
		- completely wrapped the logic to create a randomly generated rifle of various rifle types
			- `mods::forge_engine::generated_rifle`
			- `mods::forge_engine::generated_armor`
				- needs content
				- `armor_index` table currently empty
	- `mods::loot` namespace
		- source file: mods/loot.cpp
		- exposes central loot reward function
			- `mods::loot::reward_player(player_ptr_t& player);`
		- reward ACMD now available to debug loot system
		- `value_scaler` function now available to debug value scaling
	- value scaler class
		- TODO: Balance `mods::forge_engine::value_scaler()`
			- This is where you'll want to tweak the values for game balance
			- source file: mods/forge-engine/value-scaler.(h|c)pp
	- stats and resistances
		- wearing armor with elemental resistance percents will add to your resistances
		- wearing armor that adds stat bonuses will add to your affect abilities (`aff_abils()`)
	

# Cobalt FOB area scripts
	- source file: lib/js/profiles/far.js
	- As of 2020-12-17, the south eastern portion of the FOB has been written. (Level 1)
	- started creation of COBALT Forward Operating Base
	- Armory to the East.
	- Southern hallway done. Need to add armor locker and armor buy station in armory

# normalized query pattern
	- Normalized queries can be performed by following the same pattern as the `mob_roam` class in the orm folder
	- source file: mods/orm/mob-roam.(c|h)pp
	- TODO: Need to lock the backroom in the armory

# SQL DUMP
	- If you need an sql dump, use src/sql/migrations/beta.sql. That file will be updated as migrations are ran.
	- BEWARE: THIS FILE DROPS THE MUD DATABASE BEFORE RUNNING


# Visibility
	- Calculating visibility now the sole responsibility of `mods::calc_visibility::is_visible(observer,target)`
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
	- breacher class 
		- special ability 'explosive shot' now ready
			- needs balancing
	- ghost class
		- special ability 'dissipate' now ready
			- needs balancing
	- engineer class
		- special ability 'drone_bomb' now ready
		- example: `drone_bomb north 2`

# classes system rework
	Up until today, there has been no definite scope to the player classes. It has all been one
	big blur of adding random features with no clear decision as to the definite direction
	of the game. I am taking the time now to describe the new class system which in turn
	also clearly defines the scope of this project. 

	For a little bit of background: I started this project with the idea of creating a 
	MUD that was inspired by Tom Clancy's Rainbow Six Siege. I have deviated from that
	initial goal. I have implemented lots of features but nothing cohesive enough to
	gaurantee a good multiplayer experience. That ends here. The following class
	system is set in stone and will not change.

# Class System
	Each player class starts out with the following configuration:
	- Primary Weapon
	- Secondary Weapon
	- Tactical
	- Special Ability

	Base attributes for each PC all start off the same, however, the
	Speed/Armor triads can be configured by player upon character generation.

# SNIPER
	- Loadout:
		- Starting Primary: PSG-1 Sniper Rifle
		- Starting Secondary: CZP10 Pistol
		- Starting Tactical: Claymore Mine
		- Ability: XRay Shot [ DONE ]
			- Command sequence:
				- Type 'mark <name>' to mark an opponent
				- Type 'engage' you will now specifically target only that mob
				- Everytime you type 'xray_shot' it will fire off a shot until you're out of charges
			- To disengage, type 'disengage'
			- Once target is dead, you no longer are in engage mode and have to mark/engage/xray_shot again
		- Ability: Tracking Shot
			- Command sequence:
				- `tracking_shot johnny east`
					- this dispatches a rifle attack with feedback
					- will only track the target if the rifle attack hits
			- Target is affected by `AFF_TRACKED` for `SKILL_SNIPER_TRACKING_SHOT_TICKS()`
			- Players marked with the tracking shot are affected in the following ways:
				- take 15% more damage
				- will be visible even if they try to go invisible
					- see: `mods/calc-visibility.cpp`, `is_visible()` function
# MARINE
	- Loadout:
		- Starting Primary: M4 Assault Rifle
		- Starting Secondary: Magnum Revolver (exact type TBD)
		- Starting Tactical: Fragmentation Grenade
		- Ability: Activate Underbarrel grenade launcher (x3 grenade rounds)
			- Command sequence
				- Type 'underbarrel activate'
				- Type 'snipe <target> <direction>'
			- Causes massive explosive damage to a room very far away
# BREACHER
	- Loadout:
		- Starting Primary: MP-5 Sub Machine Gun
		- Starting Secondary: SASG-12 Shotgun
			- Every 4th shot gains 1 push back charge
		- Starting Tactical: Flashbang
		- Ability: Explosive shot [ DONE 2020-12-04 ]
			- Shot can breach doors/walls/surfaces, creating new tiles on the map
			- Same effect as a breach charge but without having to use one
# ENGINEER
	- Loadout:
		- Starting Primary: UMP-45
		- Starting Secondary: Glock
		- Starting Tactical: Sensor Grenade
		- Ability: Drone Bomb [ DONE ]
			- Deploy a drone that drives in a direction N rooms
			- Example: `drone_bomb north 2`
			- Drone bomb is a remote explosive type stored at `objects/explosive/engineer-drone-bomb.yml`
				- Drone bomb is a globally available singleton guarded and accessible only through a function
# MEDIC
	- Loadout:
		- Starting Primary: SAIGA-12 Automatic Shotgun
			- Every 3rd shot adds 1 push back charge
		- Starting Secondary: MP9 Machine Pistol
		- Starting Tactical: Smoke Grenade
		- Ability: Drop Healing Grenade
			- Team members can heal more by typing 'siphon' in the room a healing grenade exists in
# GHOST
	- Loadout:
		- Starting Primary: Silenced G36C Assault Rifle
		- Starting Secondary: Silenced PPK Pistol
		- Starting Tactical: Smoke Grenade
		- Ability: Dissipate
			- Disappear and move throughout the map without being noticed [ DONE 2020-12-04 ]
			- Traps, doors, walls do not affect you [ DONE 2020-12-05 ]
			- Enemies cannot see or fire at you [ TODO ]
			- Enemies cannot see you with scan functionality
# SUPPORT
	- Loadout:
		- Starting Primary: HK21 Light Machine Gun
		- Starting Secondary: 
		- Starting Tactical: Incendiary Grenade
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
	- usage: `set_value <key> <value>`
	- `get_value <key>...<keyN>`
	- `revert_value_to_default <arg>...<argN>`
	- `list_values [searchable]`

# armor lockers
	- `install_armor_locker <yaml>...<yaml-N>`
	- `uninstall_armor_locker`
	- quotas:
		- `armor_locker_quota <integer>`
	- source file: mods/integral-objects.cpp
	- replenish source file: mods/zone.cpp
		- TODO: use query container source code to keep quota up
	- feed source file: mods/integal-objects.cpp
	- db source file: mods/integral-objects-db.cpp

# weapon lockers
	- `install_weapon_locker <yaml>...<yaml-N>`
	- `weapon_armor_locker`
	- quotas:
		- `weapon_locker_quota <integer>`
	- source file: mods/integral-objects.cpp
	- replenish source file: mods/zone.cpp
		- TODO: use query container source code to keep quota up
	- feed source file: mods/integal-objects.cpp
	- db source file: mods/integral-objects-db.cpp

# quotas
	- see quotas in weapon/armor lockers section
	- db source file: mods/integral-objects-db.cpp

# camera feed
	- `install_camera_feed <room-vnum>...<room-vnum-N>`
	- todo: name the object properly. currently it's named "gadget"
	- source file: mods/integral-objects.cpp
	- feed source file: mods/integal-objects.cpp
	- replenish source file: mods/zone.cpp
	- db source file: mods/integral-objects-db.cpp

# catchy names
	- `create_catchy_name decimator "The Decimator" g36c.yml{sight:acog.yml,under_barrel:gm32grenadelauncher.yml}`
	- `instantiate_catchy_name decimator`
	- source file: mods/integral-objects.cpp
	
# querying for objects
	- `query_room <yaml>`
	- `query_container <name> <yaml>...<yaml-N>`
	- `query_inventory <yaml>`
	- source file: mods/count-objects.cpp

# target practice
	- source file: mods/target-practice(-db).cpp
	- `install_dummy`
	- `uninstall_dummy`
	- `instantiate_dummy`

# armor levels
	- basic protection
	- advanced protection
	- elite protection
	- source files: mods/armor/{type}-protection.cpp

# mini games
	- line up

# replenish
	- source file: mods/replenish.cpp
	- replenishes ghost class every 10 seconds, or 10-11 seconds for however many ticks the `GHOST_REPLENISH_TICKS()` dynamic value is set to. (can be set using `set_value`)

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

# `register_help_command`
	- source file: mods/help.cpp
	- `register_help_command("scan","help file contents");`

# centralized sql library helpers
	- source file: mods/pq.hpp

# super user functions
	- `add_super_user <user>...<userN>`
	- `get_super_user_list`
	- `remove_super_user <user>...<userN>`
	- `go_invisible`
		- makes the user invoking it go invisible
	- `go_visible`
		- makes the user invoking it visible
	- `set_who_line <player> <string>`
	- `clear_who_line <player>...<playerN>`

# centralized player saving/loading
	- `mods::players::db_load::set_class()`
	- `mods::players::db_load::save()`
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

# `advance_level` now uses our library code
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
